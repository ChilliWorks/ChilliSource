//
//  The MIT License (MIT)
//
//  Copyright (c) 2016 Tag Games Limited
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//

#include <ChilliSource/Rendering/Base/RenderCommandBufferManager.h>

#include <ChilliSource/Rendering/Base/RenderSnapshot.h>
#include <ChilliSource/Rendering/RenderCommand/Commands/LoadMaterialGroupRenderCommand.h>
#include <ChilliSource/Rendering/RenderCommand/Commands/LoadMeshRenderCommand.h>
#include <ChilliSource/Rendering/RenderCommand/Commands/LoadShaderRenderCommand.h>
#include <ChilliSource/Rendering/RenderCommand/Commands/LoadTextureRenderCommand.h>
#include <ChilliSource/Rendering/RenderCommand/Commands/UnloadMaterialGroupRenderCommand.h>
#include <ChilliSource/Rendering/RenderCommand/Commands/UnloadMeshRenderCommand.h>
#include <ChilliSource/Rendering/RenderCommand/Commands/UnloadShaderRenderCommand.h>
#include <ChilliSource/Rendering/RenderCommand/Commands/UnloadTextureRenderCommand.h>
#include <ChilliSource/Rendering/RenderCommand/RenderCommandBuffer.h>

namespace ChilliSource
{
    namespace
    {
        constexpr u32 k_maxQueueSize = 1;
    }
    
    CS_DEFINE_NAMEDTYPE(RenderCommandBufferManager);
    
    //------------------------------------------------------------------------------
    RenderCommandBufferManagerUPtr RenderCommandBufferManager::Create() noexcept
    {
        return RenderCommandBufferManagerUPtr(new RenderCommandBufferManager());
    }
    //------------------------------------------------------------------------------
    RenderCommandBufferManager::RenderCommandBufferManager()
    : m_discardCommands(false)
    {
    }
    //------------------------------------------------------------------------------
    bool RenderCommandBufferManager::IsA(InterfaceIDType interfaceId) const noexcept
    {
        return (RenderCommandBufferManager::InterfaceID == interfaceId);
    }
    //------------------------------------------------------------------------------
    void RenderCommandBufferManager::OnResume() noexcept
    {
        m_discardCommands = false;
    }
    //------------------------------------------------------------------------------
    void RenderCommandBufferManager::OnSystemSuspend() noexcept
    {
        m_discardCommands = true;

        std::unique_lock<std::mutex> lock(m_renderCommandBuffersMutex);
        
        if(m_renderCommandBuffers.size() > 0)
        {
            for(const auto& buffer : m_renderCommandBuffers)
            {
                RecycleRenderCommandBuffer(buffer.get());
            }
            
            m_renderCommandBuffers.clear();
        }
        
        lock.unlock();
        
        //Notify the condition since we have updated the command buffer
        m_renderCommandBuffersCondition.notify_all();
    }
    //------------------------------------------------------------------------------
    void RenderCommandBufferManager::OnRenderSnapshot(RenderSnapshot& renderSnapshot) noexcept
    {
        if(!m_discardCommands)
        {
            auto preRenderCommandList = renderSnapshot.GetPreRenderCommandList();
            auto postRenderCommandList = renderSnapshot.GetPostRenderCommandList();
            
            std::unique_lock<std::mutex>(m_commandBufferMutex);
            
            for(auto& command : m_pendingShaderLoadCommands)
            {
                preRenderCommandList->AddLoadShaderCommand(command.GetRenderShader(), command.GetVertexShader(), command.GetFragmentShader());
            }
            
            for(auto& command : m_pendingTextureLoadCommands)
            {
                preRenderCommandList->AddLoadTextureCommand(command.GetRenderTexture(), std::move(command.ClaimTextureData()), command.GetTextureDataSize());
            }
            
            for(auto& command : m_pendingMeshLoadCommands)
            {
                preRenderCommandList->AddLoadMeshCommand(command.GetRenderMesh(), std::move(command.ClaimVertexData()), command.GetVertexDataSize(), std::move(command.ClaimIndexData()), command.GetIndexDataSize());
            }
            
            for(auto& command : m_pendingShaderUnloadCommands)
            {
                postRenderCommandList->AddUnloadShaderCommand(std::move(command));
            }
            
            for(auto& command : m_pendingTextureUnloadCommands)
            {
                postRenderCommandList->AddUnloadTextureCommand(std::move(command));
            }
            
            for(auto& command : m_pendingMeshUnloadCommands)
            {
                postRenderCommandList->AddUnloadMeshCommand(std::move(command));
            }
            
            m_pendingShaderLoadCommands.clear();
            m_pendingTextureLoadCommands.clear();
            m_pendingMeshLoadCommands.clear();
            m_pendingShaderUnloadCommands.clear();
            m_pendingTextureUnloadCommands.clear();
            m_pendingMeshUnloadCommands.clear();
        }
    }
    //------------------------------------------------------------------------------
    void RenderCommandBufferManager::RecycleRenderCommandBuffer(RenderCommandBuffer* commands) noexcept
    {
        for(u32 i = 0; i < commands->GetNumSlots(); ++i)
        {
            RecycleCommandList(commands->GetRenderCommandList(i));
        }
    }
    //------------------------------------------------------------------------------
    void RenderCommandBufferManager::RecycleCommandList(RenderCommandList* renderCommandList) noexcept
    {
        std::unique_lock<std::mutex>(m_commandBufferMutex);
        
        for(u32 i = 0; i < renderCommandList->GetNumCommands(); ++i)
        {
            RecycleCommand(renderCommandList->GetCommand(i));
        }
    }
    //------------------------------------------------------------------------------
    bool RenderCommandBufferManager::RecycleCommand(RenderCommand* renderCommand) noexcept
    {
        switch(renderCommand->GetType())
        {
            case RenderCommand::Type::k_loadShader:
            {
                LoadShaderRenderCommand* command = dynamic_cast<LoadShaderRenderCommand*>(renderCommand);
                m_pendingShaderLoadCommands.push_back(std::move(*command));
                break;
            }
            case RenderCommand::Type::k_loadTexture:
            {
                LoadTextureRenderCommand* command = dynamic_cast<LoadTextureRenderCommand*>(renderCommand);
                m_pendingTextureLoadCommands.push_back(std::move(*command));
                break;
            }
            case RenderCommand::Type::k_loadMesh:
            {
                LoadMeshRenderCommand* command = dynamic_cast<LoadMeshRenderCommand*>(renderCommand);
                m_pendingMeshLoadCommands.push_back(std::move(*command));
                break;
            }
            case RenderCommand::Type::k_unloadShader:
            {
                UnloadShaderRenderCommand* command = dynamic_cast<UnloadShaderRenderCommand*>(renderCommand);
                m_pendingShaderUnloadCommands.push_back(std::move(command->ClaimRenderShader()));
                break;
            }
            case RenderCommand::Type::k_unloadTexture:
            {
                UnloadTextureRenderCommand* command = dynamic_cast<UnloadTextureRenderCommand*>(renderCommand);
                m_pendingTextureUnloadCommands.push_back(std::move(command->ClaimRenderTexture()));
                break;
            }
            case RenderCommand::Type::k_unloadMesh:
            {
                UnloadMeshRenderCommand* command = dynamic_cast<UnloadMeshRenderCommand*>(renderCommand);
                m_pendingMeshUnloadCommands.push_back(std::move(command->ClaimRenderMesh()));
                break;
            }
            default:
            {
                return false;
            }
        }
        
        return true;
    }
    //------------------------------------------------------------------------------
    void RenderCommandBufferManager::WaitThenPushCommandBuffer(RenderCommandBufferUPtr renderCommandBuffer) noexcept
    {
        std::unique_lock<std::mutex> lock(m_renderCommandBuffersMutex);
        
        while (m_renderCommandBuffers.size() >= k_maxQueueSize)
        {
            m_renderCommandBuffersCondition.wait(lock);
        }
        
        if(m_discardCommands)
        {
            RecycleRenderCommandBuffer(renderCommandBuffer.get());
        }
        else
        {
            m_renderCommandBuffers.push_back(std::move(renderCommandBuffer));
        }
        
        m_renderCommandBuffersCondition.notify_all();
    }
    //------------------------------------------------------------------------------
    RenderCommandBufferCUPtr RenderCommandBufferManager::WaitThenPopCommandBuffer() noexcept
    {
        std::unique_lock<std::mutex> lock(m_renderCommandBuffersMutex);
        
        while (m_renderCommandBuffers.empty())
        {
            m_renderCommandBuffersCondition.wait(lock);
        }
        
        auto renderCommandBuffer = std::move(m_renderCommandBuffers.front());
        m_renderCommandBuffers.pop_front();
        
        m_renderCommandBuffersCondition.notify_all();
        
        return std::move(renderCommandBuffer);
    }
}