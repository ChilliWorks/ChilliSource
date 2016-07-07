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

#include <ChilliSource/Rendering/Target/RenderTargetGroupManager.h>

#include <ChilliSource/Rendering/Base/RenderSnapshot.h>

namespace ChilliSource
{
    CS_DEFINE_NAMEDTYPE(RenderTargetGroupManager);

    //------------------------------------------------------------------------------
    RenderTargetGroupManagerUPtr RenderTargetGroupManager::Create() noexcept
    {
        return RenderTargetGroupManagerUPtr(new RenderTargetGroupManager());
    }

    //------------------------------------------------------------------------------
    bool RenderTargetGroupManager::IsA(InterfaceIDType interfaceId) const noexcept
    {
        return (RenderTargetGroupManager::InterfaceID == interfaceId);
    }

    //------------------------------------------------------------------------------
    const RenderTargetGroup* RenderTargetGroupManager::CreateRenderTargetGroup(const RenderTexture* colourTarget, const RenderTexture* depthTarget) noexcept
    {
        CS_ASSERT(colourTarget, "Must supply a colour target.");
        CS_ASSERT(depthTarget, "Must supply a depth target.");
        
        RenderTargetGroupUPtr tenderTargetGroup(new RenderTargetGroup(colourTarget, depthTarget, false));
        auto tenderTargetGroupRaw = tenderTargetGroup.get();
        AddRenderTargetGroup(std::move(tenderTargetGroup));
        
        return tenderTargetGroupRaw;
    }

    //------------------------------------------------------------------------------
    const RenderTargetGroup* RenderTargetGroupManager::CreateColourRenderTargetGroup(const RenderTexture* colourTarget, bool shouldUseDepthBuffer) noexcept
    {
        CS_ASSERT(colourTarget, "Must supply a colour target.");
        
        RenderTargetGroupUPtr tenderTargetGroup(new RenderTargetGroup(colourTarget, nullptr, shouldUseDepthBuffer));
        auto tenderTargetGroupRaw = tenderTargetGroup.get();
        AddRenderTargetGroup(std::move(tenderTargetGroup));
        
        return tenderTargetGroupRaw;
    }

    //------------------------------------------------------------------------------
    const RenderTargetGroup* RenderTargetGroupManager::CreateDepthRenderTargetGroup(const RenderTexture* depthTarget) noexcept
    {
        CS_ASSERT(depthTarget, "Must supply a depth target.");
        
        RenderTargetGroupUPtr tenderTargetGroup(new RenderTargetGroup(nullptr, depthTarget, false));
        auto tenderTargetGroupRaw = tenderTargetGroup.get();
        AddRenderTargetGroup(std::move(tenderTargetGroup));
        
        return tenderTargetGroupRaw;
    }

    //------------------------------------------------------------------------------
    void RenderTargetGroupManager::DestroyRenderTargetGroup(const RenderTargetGroup* renderTargetGroup) noexcept
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        
        for (auto it = m_renderTargetGroups.begin(); it != m_renderTargetGroups.end(); ++it)
        {
            if (it->get() == renderTargetGroup)
            {
                m_pendingUnloadCommands.push_back(std::move(*it));
                
                it->swap(m_renderTargetGroups.back());
                m_renderTargetGroups.pop_back();
                return;
            }
        }
        
        CS_LOG_FATAL("RenderTargetGroup does not exist.");
    }
    
    //------------------------------------------------------------------------------
    void RenderTargetGroupManager::AddRenderTargetGroup(RenderTargetGroupUPtr renderTargetGroup) noexcept
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        
        m_pendingLoadCommands.push_back(renderTargetGroup.get());
        m_renderTargetGroups.push_back(std::move(renderTargetGroup));
    }

    //------------------------------------------------------------------------------
    void RenderTargetGroupManager::OnRenderSnapshot(RenderSnapshot& renderSnapshot) noexcept
    {
        auto preRenderCommandList = renderSnapshot.GetPreRenderCommandList();
        auto postRenderCommandList = renderSnapshot.GetPostRenderCommandList();
        
        std::unique_lock<std::mutex> lock(m_mutex);
        
        for (auto& loadCommand : m_pendingLoadCommands)
        {
            preRenderCommandList->AddLoadTargetGroupCommand(loadCommand);
        }
        m_pendingLoadCommands.clear();
        
        for (auto& unloadCommand : m_pendingUnloadCommands)
        {
            postRenderCommandList->AddUnloadTargetGroupCommand(std::move(unloadCommand));
        }
        m_pendingUnloadCommands.clear();
    }
    
    //------------------------------------------------------------------------------
    RenderTargetGroupManager::~RenderTargetGroupManager() noexcept
    {
        CS_ASSERT(m_renderTargetGroups.size() == 0, "Render target groups have not been correctly released.");
    }
}
