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

#include <ChilliSource/Rendering/Shader/RenderShaderManager.h>

#include <ChilliSource/Rendering/Base/RenderSnapshot.h>
#include <ChilliSource/Rendering/Base/TargetType.h>

namespace ChilliSource
{
    namespace
    {
        constexpr u32 k_shaderPoolSize = 30;
    }
    
    CS_DEFINE_NAMEDTYPE(RenderShaderManager);
    
    //------------------------------------------------------------------------------
    RenderShaderManagerUPtr RenderShaderManager::Create() noexcept
    {
        return RenderShaderManagerUPtr(new RenderShaderManager());
    }
    
    //------------------------------------------------------------------------------
    RenderShaderManager::RenderShaderManager()
    : m_renderShaderPool(k_shaderPoolSize, ObjectPoolAllocatorLimitPolicy::k_expand)
    {
        
    }
    
    //------------------------------------------------------------------------------
    bool RenderShaderManager::IsA(InterfaceIDType interfaceId) const noexcept
    {
        return (RenderShaderManager::InterfaceID == interfaceId);
    }
    
    //------------------------------------------------------------------------------
    UniquePtr<RenderShader> RenderShaderManager::CreateRenderShader(const std::string& vertexShader, const std::string& fragmentShader) noexcept
    {
        UniquePtr<RenderShader> renderShader(MakeUnique<RenderShader>(m_renderShaderPool));
        auto rawRenderShader = renderShader.get();
        
        PendingLoadCommand loadCommand;
        loadCommand.m_vertexShader = vertexShader;
        loadCommand.m_fragmentShader = fragmentShader;
        loadCommand.m_renderShader = rawRenderShader;
        
        std::unique_lock<std::mutex> lock(m_mutex);
        m_pendingLoadCommands.push_back(std::move(loadCommand));
        
        return renderShader;
    }
    
    //------------------------------------------------------------------------------
    void RenderShaderManager::DestroyRenderShader(UniquePtr<RenderShader> renderShader) noexcept
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_pendingUnloadCommands.push_back(std::move(renderShader));
    }
    
    //------------------------------------------------------------------------------
    void RenderShaderManager::OnRenderSnapshot(TargetType targetType, RenderSnapshot& renderSnapshot, IAllocator* frameAllocator) noexcept
    {
        if(targetType == TargetType::k_main)
        {
            auto preRenderCommandList = renderSnapshot.GetPreRenderCommandList();
            auto postRenderCommandList = renderSnapshot.GetPostRenderCommandList();
            
            std::unique_lock<std::mutex> lock(m_mutex);
            
            for (auto& loadCommand : m_pendingLoadCommands)
            {
                preRenderCommandList->AddLoadShaderCommand(loadCommand.m_renderShader, loadCommand.m_vertexShader, loadCommand.m_fragmentShader);
            }
            m_pendingLoadCommands.clear();
            
            for (auto& unloadCommand : m_pendingUnloadCommands)
            {
                postRenderCommandList->AddUnloadShaderCommand(std::move(unloadCommand));
            }
            m_pendingUnloadCommands.clear();
        }
    }
}
