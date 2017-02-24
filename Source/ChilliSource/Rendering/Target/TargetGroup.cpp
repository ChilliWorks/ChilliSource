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
        /// Creates a RenderTargetGroup for the passed in combination of colour/depth targets.
        ///
        /// @param colourTarget
        ///     The colour render target. Can be null if no colour target is needed.
        /// @param depthTarget
        ///     The depth render target. Can be null if no depth target is needed.
        /// @param type
        ///    Whether or not an internal, efficient, depth or stencil buffer should be used.
        ///
        UniquePtr<RenderTargetGroup> CreateRenderTargetGroup(const RenderTexture* colourTarget, const RenderTexture* depthTarget, RenderTargetGroupType type)
        {
            auto renderTargetGroupManager = Application::Get()->GetSystem<RenderTargetGroupManager>();
            if (colourTarget && depthTarget)
            {
                return renderTargetGroupManager->CreateRenderTargetGroup(colourTarget, depthTarget);
            }
            else if (colourTarget)
            {
                return renderTargetGroupManager->CreateColourRenderTargetGroup(colourTarget, type);
            }
            else if (depthTarget)
            {
                return renderTargetGroupManager->CreateDepthRenderTargetGroup(depthTarget);
            }
            
            return nullptr;
        }
    }
    
    //------------------------------------------------------------------------------
    TargetGroupUPtr TargetGroup::CreateTargetGroup(const TextureCSPtr& colourTarget, const TextureCSPtr& depthTarget) noexcept
    {
        return TargetGroupUPtr(new TargetGroup(colourTarget, depthTarget, RenderTargetGroupType::k_colour));
    }
    
    //------------------------------------------------------------------------------
    TargetGroupUPtr TargetGroup::CreateColourTargetGroup(const TextureCSPtr& colourTarget, RenderTargetGroupType type) noexcept
    {
        return TargetGroupUPtr(new TargetGroup(colourTarget, nullptr, type));
    }
    
    //------------------------------------------------------------------------------
    TargetGroupUPtr TargetGroup::CreateDepthTargetGroup(const TextureCSPtr& depthTarget) noexcept
    {
        return TargetGroupUPtr(new TargetGroup(nullptr, depthTarget, RenderTargetGroupType::k_colour));
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
            
            m_renderTargetGroup = CreateRenderTargetGroup(m_cachedColourTargetRenderTexture, m_cachedDepthTargetRenderTexture, m_type);
        }
        
        return m_renderTargetGroup.get();
    }
    
    //------------------------------------------------------------------------------
    void TargetGroup::DestroyRenderTargetGroup() noexcept
    {
        if (m_renderTargetGroup)
        {
            auto renderTargetGroupManager = Application::Get()->GetSystem<RenderTargetGroupManager>();
            CS_ASSERT(renderTargetGroupManager, "RenderTargetGroupManager is required.");
            
            renderTargetGroupManager->DestroyRenderTargetGroup(std::move(m_renderTargetGroup));
        }
        
        if(m_cachedDepthTargetTexture)
        {
            m_cachedDepthTargetTexture.reset();
        }
        
        if(m_cachedColourTargetTexture)
        {
            m_cachedColourTargetTexture.reset();
        }
    }
    
    //------------------------------------------------------------------------------
    TargetGroup::TargetGroup(const TextureCSPtr& colourTarget, const TextureCSPtr& depthTarget, RenderTargetGroupType type) noexcept
        : m_cachedColourTargetTexture(colourTarget), m_cachedDepthTargetTexture(depthTarget), m_type(type)
    {
        m_cachedColourTargetRenderTexture = colourTarget == nullptr ? nullptr : colourTarget->GetRenderTexture();
        m_cachedDepthTargetRenderTexture = depthTarget == nullptr ? nullptr : depthTarget->GetRenderTexture();
        
        m_renderTargetGroup = CreateRenderTargetGroup(m_cachedColourTargetRenderTexture, m_cachedDepthTargetRenderTexture, type);
    }
    
    //------------------------------------------------------------------------------
    TargetGroup::~TargetGroup() noexcept
    {
        DestroyRenderTargetGroup();
    }
}
