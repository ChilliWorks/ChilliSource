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

#include <ChilliSource/Rendering/Target/TargetGroup.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Rendering/Target/RenderTargetGroupManager.h>
#include <ChilliSource/Rendering/Texture/Texture.h>

namespace ChilliSource
{
    namespace
    {
        const RenderTargetGroup* CreateRenderTargetGroup(const RenderTexture* colourTarget, const RenderTexture* depthTarget, bool shouldUseDepthBuffer)
        {
            auto renderTargetGroupManager = Application::Get()->GetSystem<RenderTargetGroupManager>();
            if (colourTarget && depthTarget)
            {
                return renderTargetGroupManager->CreateRenderTargetGroup(colourTarget, depthTarget);
            }
            else if (colourTarget)
            {
                return renderTargetGroupManager->CreateColourRenderTargetGroup(colourTarget, shouldUseDepthBuffer);
            }
            else if (depthTarget)
            {
                return renderTargetGroupManager->CreateDepthRenderTargetGroup(depthTarget);
            }
            
            return nullptr;
        }
    }
    
    //------------------------------------------------------------------------------
    TargetGroupUPtr TargetGroup::CreateTargetGroup(const Texture* colourTarget, const Texture* depthTarget) noexcept
    {
        return TargetGroupUPtr(new TargetGroup(colourTarget, depthTarget, false));
    }
    
    //------------------------------------------------------------------------------
    TargetGroupUPtr TargetGroup::CreateColourTargetGroup(const Texture* colourTarget, bool shouldUseDepthBuffer) noexcept
    {
        return TargetGroupUPtr(new TargetGroup(colourTarget, nullptr, shouldUseDepthBuffer));
    }
    
    //------------------------------------------------------------------------------
    TargetGroupUPtr TargetGroup::CreateDepthTargetGroup(const Texture* depthTarget) noexcept
    {
        return TargetGroupUPtr(new TargetGroup(nullptr, depthTarget, false));
    }
    
    //------------------------------------------------------------------------------
    const RenderTargetGroup* TargetGroup::GetRenderTargetGroup() noexcept
    {
        //Check if the cached textures are invalid
        if((m_cachedColourTargetTexture != nullptr && m_cachedColourTargetTexture->GetRenderTexture() != m_cachedColourTargetRenderTexture) ||
           (m_cachedDepthTargetTexture != nullptr && m_cachedDepthTargetTexture->GetRenderTexture() != m_cachedDepthTargetRenderTexture))
        {
            DestroyRenderTargetGroup();
            
            m_cachedColourTargetRenderTexture = m_cachedColourTargetTexture == nullptr ? nullptr : m_cachedColourTargetTexture->GetRenderTexture();
            m_cachedDepthTargetRenderTexture = m_cachedDepthTargetTexture == nullptr ? nullptr : m_cachedDepthTargetTexture->GetRenderTexture();
            
            m_renderTargetGroup = CreateRenderTargetGroup(m_cachedColourTargetRenderTexture, m_cachedDepthTargetRenderTexture, m_shouldUseDepthBuffer);
        }
        
        return m_renderTargetGroup;
    }
    
    //------------------------------------------------------------------------------
    void TargetGroup::DestroyRenderTargetGroup() noexcept
    {
        if (m_renderTargetGroup)
        {
            auto renderTargetGroupManager = Application::Get()->GetSystem<RenderTargetGroupManager>();
            CS_ASSERT(renderTargetGroupManager, "RenderTargetGroupManager is required.");
            
            renderTargetGroupManager->DestroyRenderTargetGroup(m_renderTargetGroup);
            m_renderTargetGroup = nullptr;
        }
    }
    
    //------------------------------------------------------------------------------
    TargetGroup::TargetGroup(const Texture* colourTarget, const Texture* depthTarget, bool shouldUseDepthBuffer) noexcept
        : m_cachedColourTargetTexture(colourTarget), m_cachedDepthTargetTexture(depthTarget), m_shouldUseDepthBuffer(shouldUseDepthBuffer)
    {
        m_cachedColourTargetRenderTexture = colourTarget == nullptr ? nullptr : colourTarget->GetRenderTexture();
        m_cachedDepthTargetRenderTexture = depthTarget == nullptr ? nullptr : depthTarget->GetRenderTexture();
        
        m_renderTargetGroup = CreateRenderTargetGroup(m_cachedColourTargetRenderTexture, m_cachedDepthTargetRenderTexture, m_shouldUseDepthBuffer);
    }
}