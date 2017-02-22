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

#ifndef _CHILLISOURCE_RENDERING_TARGET_RENDERTARGETGROUP_H_
#define _CHILLISOURCE_RENDERING_TARGET_RENDERTARGETGROUP_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Math/Vector2.h>

namespace ChilliSource
{
    enum class RenderTargetGroupType
    {
        k_colour,
        k_colourDepth,
        k_colourDepthStencil
    };
    
    /// Represents a group of colour and depth render targets that can be bound for rendering.
    /// There are two types of render target: colour and depth. Fragment colour data will be
    /// written to the colour target, while fragment depth data will be written to the depth
    /// target.
    ///
    /// This is immutable and therefore thread safe, aside from extra data which should only
    /// be accessed from the render thread.
    ///
    class RenderTargetGroup final
    {
    public:
        
        CS_DECLARE_NOCOPY(RenderTargetGroup);
        
        /// Creates a new instance with the given colour and depth targets. At least one target
        /// must be supplied or this will assert.
        ///
        /// SHOULD ONLY BE CREATED VIA RenderTargetGroupManager
        ///
        /// @param colourTarget
        ///     The colour render target. Can be null if no colour target is needed.
        /// @param depthTarget
        ///     The depth render target. Can be null if no depth target is needed.
        /// @param type
        ///     Whether or not to use an internal depth buffer or stencil buffer.
        ///     NOTE: Stencil requires depth and type must be colour only if explicit depth target is supplied
        ///
        RenderTargetGroup(const RenderTexture* colourTarget, const RenderTexture* depthTarget, RenderTargetGroupType type) noexcept;
        
        /// @return The colour render target. Can be null if no colour target is needed.
        ///
        const RenderTexture* GetColourTarget() const noexcept { return m_colourTarget; }
        
        /// @return The depth render target. Can be null if no depth target is needed.
        ///
        const RenderTexture* GetDepthTarget() const noexcept { return m_depthTarget; }
        
        /// @return Whether or not to use an internal depth buffer if no depth target was specified.
        ///     This should always be false if a depth target was supplied.
        ///
        bool ShouldUseDepthBuffer() const noexcept { return m_type == RenderTargetGroupType::k_colourDepth || m_type == RenderTargetGroupType::k_colourDepthStencil; }
        
        /// @return Whether or not to use an internal stencil buffer.
        ///     This should always be false if not using a depth buffer.
        ///
        bool ShouldUseStencilBuffer() const noexcept { return m_type == RenderTargetGroupType::k_colourDepthStencil; }
        
        /// @return The resolution of the render target.
        ///
        const Integer2& GetResolution() const noexcept { return m_resolution; }
        
        /// This is not thread safe and should only be called from the render thread.
        ///
        /// @return A pointer to render system specific additional information.
        ///
        void* GetExtraData() const noexcept { return m_extraData; }
        
        /// This is not thread safe and should only be called from the render thread.
        ///
        /// @param extraData
        ///     A pointer to render system specific additional information.
        ///
        void SetExtraData(void* extraData) noexcept { m_extraData = extraData; }
        
    private:
        
        const RenderTexture* m_colourTarget;
        const RenderTexture* m_depthTarget;
        RenderTargetGroupType m_type;
        Integer2 m_resolution;
        void* m_extraData = nullptr;
    };
}

#endif
