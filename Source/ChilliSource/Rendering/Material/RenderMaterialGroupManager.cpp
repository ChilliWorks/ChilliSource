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

#include <ChilliSource/Rendering/Material/RenderMaterialGroupManager.h>

#include <ChilliSource/Rendering/Base/RenderSnapshot.h>
#include <ChilliSource/Rendering/Material/ForwardRenderMaterialGroupManager.h>
#include <ChilliSource/Rendering/RenderCommand/RenderCommandList.h>

namespace ChilliSource
{
    CS_DEFINE_NAMEDTYPE(RenderMaterialGroupManager);

    //------------------------------------------------------------------------------
    RenderMaterialGroupManagerUPtr RenderMaterialGroupManager::Create() noexcept
    {
        //TODO: Handle creation of deferred rendering version of the manager
        return RenderMaterialGroupManagerUPtr(new ForwardRenderMaterialGroupManager());
    }
    
    //------------------------------------------------------------------------------
    void RenderMaterialGroupManager::DestroyRenderMaterialGroup(const RenderMaterialGroup* renderMaterial) noexcept
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        
        for (auto it = m_renderMaterialGroups.begin(); it != m_renderMaterialGroups.end(); ++it)
        {
            if (it->get() == renderMaterial)
            {
                m_pendingUnloadCommands.push_back(std::move(*it));
                
                it->swap(m_renderMaterialGroups.back());
                m_renderMaterialGroups.pop_back();
                return;
            }
        }
        
        CS_LOG_FATAL("RenderMaterialGroup does not exist.");
    }
    
    //------------------------------------------------------------------------------
    void RenderMaterialGroupManager::AddRenderMaterialGroup(RenderMaterialGroupUPtr renderMaterialGroup) noexcept
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        
        m_pendingLoadCommands.push_back(renderMaterialGroup.get());
        m_renderMaterialGroups.push_back(std::move(renderMaterialGroup));
    }
    
    //------------------------------------------------------------------------------
    void RenderMaterialGroupManager::OnRenderSnapshot(RenderSnapshot& renderSnapshot) noexcept
    {
        auto preRenderCommandList = renderSnapshot.GetPreRenderCommandList();
        auto postRenderCommandList = renderSnapshot.GetPostRenderCommandList();
        
        std::unique_lock<std::mutex> lock(m_mutex);
        
        for (auto& loadCommand : m_pendingLoadCommands)
        {
            preRenderCommandList->AddLoadMaterialGroupCommand(loadCommand);
        }
        m_pendingLoadCommands.clear();
        
        for (auto& unloadCommand : m_pendingUnloadCommands)
        {
            postRenderCommandList->AddUnloadMaterialGroupCommand(std::move(unloadCommand));
        }
        m_pendingUnloadCommands.clear();
    }
}
