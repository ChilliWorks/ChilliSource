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
#include <ChilliSource/Rendering/Base/TargetType.h>

namespace ChilliSource
{
    namespace
    {
        constexpr u32 k_groupPoolSize = 10;
    }
    
    CS_DEFINE_NAMEDTYPE(RenderTargetGroupManager);

    //------------------------------------------------------------------------------
    RenderTargetGroupManagerUPtr RenderTargetGroupManager::Create() noexcept
    {
        return RenderTargetGroupManagerUPtr(new RenderTargetGroupManager());
    }
    
    //------------------------------------------------------------------------------
    RenderTargetGroupManager::RenderTargetGroupManager()
    : m_renderTargetGroupPool(k_groupPoolSize, ObjectPoolAllocatorLimitPolicy::k_expand)
    {
        
    }

    //------------------------------------------------------------------------------
    bool RenderTargetGroupManager::IsA(InterfaceIDType interfaceId) const noexcept
    {
        return (RenderTargetGroupManager::InterfaceID == interfaceId);
    }

    //------------------------------------------------------------------------------
    UniquePtr<RenderTargetGroup> RenderTargetGroupManager::CreateRenderTargetGroup(const RenderTexture* colourTarget, const RenderTexture* depthTarget) noexcept
    {
        CS_ASSERT(colourTarget, "Must supply a colour target.");
        CS_ASSERT(depthTarget, "Must supply a depth target.");
        
        UniquePtr<RenderTargetGroup> renderTargetGroup(MakeUnique<RenderTargetGroup>(m_renderTargetGroupPool, colourTarget, depthTarget, RenderTargetGroupType::k_colour));
        auto renderTargetGroupRaw = renderTargetGroup.get();
        
        std::unique_lock<std::mutex> lock(m_mutex);
        m_renderTargetGroups.push_back(renderTargetGroupRaw);
        m_pendingLoadCommands.push_back(renderTargetGroupRaw);
        
        return renderTargetGroup;
    }

    //------------------------------------------------------------------------------
    UniquePtr<RenderTargetGroup> RenderTargetGroupManager::CreateColourRenderTargetGroup(const RenderTexture* colourTarget, RenderTargetGroupType type) noexcept
    {
        CS_ASSERT(colourTarget, "Must supply a colour target.");
        
        UniquePtr<RenderTargetGroup> renderTargetGroup(MakeUnique<RenderTargetGroup>(m_renderTargetGroupPool, colourTarget, nullptr, type));
        auto renderTargetGroupRaw = renderTargetGroup.get();
        
        std::unique_lock<std::mutex> lock(m_mutex);
        m_renderTargetGroups.push_back(renderTargetGroupRaw);
        m_pendingLoadCommands.push_back(renderTargetGroupRaw);
        
        return renderTargetGroup;
    }

    //------------------------------------------------------------------------------
    UniquePtr<RenderTargetGroup> RenderTargetGroupManager::CreateDepthRenderTargetGroup(const RenderTexture* depthTarget) noexcept
    {
        CS_ASSERT(depthTarget, "Must supply a depth target.");
        
        UniquePtr<RenderTargetGroup> renderTargetGroup(MakeUnique<RenderTargetGroup>(m_renderTargetGroupPool, nullptr, depthTarget, RenderTargetGroupType::k_colour));
        auto renderTargetGroupRaw = renderTargetGroup.get();
        
        std::unique_lock<std::mutex> lock(m_mutex);
        m_renderTargetGroups.push_back(renderTargetGroupRaw);
        m_pendingLoadCommands.push_back(renderTargetGroupRaw);
        
        return renderTargetGroup;
    }
    
    //------------------------------------------------------------------------------
    std::vector<const RenderTargetGroup*> RenderTargetGroupManager::GetRenderTargetGroups() noexcept
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        return m_renderTargetGroups;
    }
    
    //------------------------------------------------------------------------------
    void RenderTargetGroupManager::DestroyRenderTargetGroup(UniquePtr<RenderTargetGroup> renderTargetGroup) noexcept
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        
        for (auto it = m_renderTargetGroups.begin(); it != m_renderTargetGroups.end(); ++it)
        {
            if (*it == renderTargetGroup.get())
            {
                std::swap(m_renderTargetGroups.back(), *it);
                m_renderTargetGroups.pop_back();
                break;
            }
        }
        
        m_pendingUnloadCommands.push_back(std::move(renderTargetGroup));
    }

    //------------------------------------------------------------------------------
    void RenderTargetGroupManager::OnRenderSnapshot(TargetType targetType, RenderSnapshot& renderSnapshot, IAllocator* frameAllocator) noexcept
    {
        if(targetType == TargetType::k_main)
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
    }
}
