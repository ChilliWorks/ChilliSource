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

namespace ChilliSource
{
    CS_DEFINE_NAMEDTYPE(RenderShaderManager);
    
    //------------------------------------------------------------------------------
    RenderShaderManagerUPtr RenderShaderManager::Create() noexcept
    {
        return RenderShaderManagerUPtr(new RenderShaderManager());
    }
    
    //------------------------------------------------------------------------------
    bool RenderShaderManager::IsA(InterfaceIDType interfaceId) const noexcept
    {
        return (RenderShaderManager::InterfaceID == interfaceId);
    }
    
    //------------------------------------------------------------------------------
    const RenderShader* RenderShaderManager::CreateRenderShader(const std::string& vertexShader, const std::string& fragmentShader) noexcept
    {
        RenderShaderUPtr renderShader(new RenderShader());
        auto rawRenderShader = renderShader.get();
        
        PendingLoadCommand loadCommand;
        loadCommand.m_vertexShader = vertexShader;
        loadCommand.m_fragmentShader = fragmentShader;
        loadCommand.m_renderShader = rawRenderShader;
        
        std::unique_lock<std::mutex> lock(m_mutex);
        m_renderShaders.push_back(std::move(renderShader));
        m_pendingLoadCommands.push_back(std::move(loadCommand));
        
        return rawRenderShader;
    }
    
    //------------------------------------------------------------------------------
    void RenderShaderManager::DestroyRenderShader(const RenderShader* renderShader) noexcept
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        
        for (auto it = m_renderShaders.begin(); it != m_renderShaders.end(); ++it)
        {
            if (it->get() == renderShader)
            {
                m_pendingUnloadCommands.push_back(std::move(*it));
                
                it->swap(m_renderShaders.back());
                m_renderShaders.pop_back();
                return;
            }
        }
        
        CS_LOG_FATAL("Render texture does not exist.");
    }
    
    //------------------------------------------------------------------------------
    void RenderShaderManager::OnRenderSnapshot(RenderSnapshot& renderSnapshot) noexcept
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
