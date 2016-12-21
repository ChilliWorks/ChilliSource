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

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Rendering/Base/Renderer.h>
#include <ChilliSource/Rendering/Base/RenderSnapshot.h>
#include <ChilliSource/Rendering/Base/TargetType.h>
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
    void RenderCommandBufferManager::OnInit() noexcept
    {
        m_renderer = Application::Get()->GetSystem<Renderer>();
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
            for(auto& buffer : m_renderCommandBuffers)
            {
                RecycleRenderCommandBuffer(std::move(buffer));
            }
            
            m_renderCommandBuffers.clear();
        }
        
        lock.unlock();
        
        //Notify the condition since we have updated the command buffer
        m_renderCommandBuffersCondition.notify_all();
    }
    //------------------------------------------------------------------------------
    void RenderCommandBufferManager::OnRenderSnapshot(TargetType targetType, RenderSnapshot& renderSnapshot, IAllocator* frameAllocator) noexcept
    {
        if(!m_discardCommands && targetType == TargetType::k_main)
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
                preRenderCommandList->AddLoadTextureCommand(command.GetRenderTexture(), command.ClaimTextureData(), command.GetTextureDataSize());
            }
            
            for(auto& command : m_pendingCubemapLoadCommands)
            {
                preRenderCommandList->AddLoadCubemapCommand(command.GetRenderTexture(), command.ClaimTextureData(), command.GetTextureDataSize());
            }
            
            for(auto& command : m_pendingMeshLoadCommands)
            {
                preRenderCommandList->AddLoadMeshCommand(command.GetRenderMesh(), command.ClaimVertexData(), command.GetVertexDataSize(), command.ClaimIndexData(), command.GetIndexDataSize());
            }
            
            for(auto& command : m_pendingMaterialGroupLoadCommands)
            {
                preRenderCommandList->AddLoadMaterialGroupCommand(command.GetRenderMaterialGroup());
            }
            
            for(auto& command : m_pendingShaderUnloadCommands)
            {
                postRenderCommandList->AddUnloadShaderCommand(command.ClaimRenderShader());
            }
            
            for(auto& command : m_pendingTextureUnloadCommands)
            {
                postRenderCommandList->AddUnloadTextureCommand(command.ClaimRenderTexture());
            }
            
            for(auto& command : m_pendingCubemapUnloadCommands)
            {
                postRenderCommandList->AddUnloadCubemapCommand(command.ClaimRenderTexture());
            }
            
            for(auto& command : m_pendingMeshUnloadCommands)
            {
                postRenderCommandList->AddUnloadMeshCommand(command.ClaimRenderMesh());
            }
            
            for(auto& command : m_pendingMaterialGroupUnloadCommands)
            {
                postRenderCommandList->AddUnloadMaterialGroupCommand(command.ClaimRenderMaterialGroup());
            }
            
            m_pendingShaderLoadCommands.clear();
            m_pendingTextureLoadCommands.clear();
            m_pendingCubemapLoadCommands.clear();
            m_pendingMeshLoadCommands.clear();
            m_pendingMaterialGroupLoadCommands.clear();
            m_pendingShaderUnloadCommands.clear();
            m_pendingTextureUnloadCommands.clear();
            m_pendingCubemapUnloadCommands.clear();
            m_pendingMeshUnloadCommands.clear();
            m_pendingMaterialGroupUnloadCommands.clear();
        }
    }
    //------------------------------------------------------------------------------
    void RenderCommandBufferManager::RecycleRenderCommandBuffer(RenderCommandBufferUPtr buffer) noexcept
    {
        for(u32 i = 0; i < buffer->GetNumSlots(); ++i)
        {
            RecycleCommandList(buffer->GetRenderCommandList(i));
        }
        
        auto frameAllocator = buffer->GetFrameAllocator();
        buffer.reset();
        m_renderer->GetFrameAllocatorQueue().Push(frameAllocator);
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
    void RenderCommandBufferManager::RecycleCommand(RenderCommand* renderCommand) noexcept
    {
        switch(renderCommand->GetType())
        {
            case RenderCommand::Type::k_loadShader:
            {
                LoadShaderRenderCommand* command = static_cast<LoadShaderRenderCommand*>(renderCommand);
                m_pendingShaderLoadCommands.push_back(std::move(*command));
                break;
            }
            case RenderCommand::Type::k_loadTexture:
            {
                LoadTextureRenderCommand* command = static_cast<LoadTextureRenderCommand*>(renderCommand);
                m_pendingTextureLoadCommands.push_back(std::move(*command));
                break;
            }
            case RenderCommand::Type::k_loadCubemap:
            {
                LoadCubemapRenderCommand* command = static_cast<LoadCubemapRenderCommand*>(renderCommand);
                m_pendingCubemapLoadCommands.push_back(std::move(*command));
                break;
            }
            case RenderCommand::Type::k_loadMesh:
            {
                LoadMeshRenderCommand* command = static_cast<LoadMeshRenderCommand*>(renderCommand);
                m_pendingMeshLoadCommands.push_back(std::move(*command));
                break;
            }
            case RenderCommand::Type::k_loadMaterialGroup:
            {
                LoadMaterialGroupRenderCommand* command = static_cast<LoadMaterialGroupRenderCommand*>(renderCommand);
                m_pendingMaterialGroupLoadCommands.push_back(std::move(*command));
                break;
            }
            case RenderCommand::Type::k_unloadShader:
            {
                UnloadShaderRenderCommand* command = static_cast<UnloadShaderRenderCommand*>(renderCommand);
                m_pendingShaderUnloadCommands.push_back(std::move(*command));
                break;
            }
            case RenderCommand::Type::k_unloadTexture:
            {
                UnloadTextureRenderCommand* command = static_cast<UnloadTextureRenderCommand*>(renderCommand);
                m_pendingTextureUnloadCommands.push_back(std::move(*command));
                break;
            }
            case RenderCommand::Type::k_unloadCubemap:
            {
                UnloadCubemapRenderCommand* command = static_cast<UnloadCubemapRenderCommand*>(renderCommand);
                m_pendingCubemapUnloadCommands.push_back(std::move(*command));
                break;
            }
            case RenderCommand::Type::k_unloadMesh:
            {
                UnloadMeshRenderCommand* command = static_cast<UnloadMeshRenderCommand*>(renderCommand);
                m_pendingMeshUnloadCommands.push_back(std::move(*command));
                break;
            }
            case RenderCommand::Type::k_unloadMaterialGroup:
            {
                UnloadMaterialGroupRenderCommand* command = static_cast<UnloadMaterialGroupRenderCommand*>(renderCommand);
                m_pendingMaterialGroupUnloadCommands.push_back(std::move(*command));
                break;
            }
            default:
            {
                break;
            }
        }
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
            RecycleRenderCommandBuffer(std::move(renderCommandBuffer));
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
        
        auto buffer = std::move(m_renderCommandBuffers.front());
        m_renderCommandBuffers.pop_front();
        
        m_renderCommandBuffersCondition.notify_all();
        
        return std::move(buffer);
    }
}
