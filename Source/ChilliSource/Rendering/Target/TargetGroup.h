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

#ifndef _CHILLISOURCE_RENDERING_TARGET_TARGETGROUP_H_
#define _CHILLISOURCE_RENDERING_TARGET_TARGETGROUP_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Memory/UniquePtr.h>

namespace ChilliSource
{
    /// Represents a group of colour and depth render targets that can be bound for rendering.
    /// There are two types of render target: colour and depth. Fragment colour data will be
    /// written to the colour target, while fragment depth data will be written to the depth
    /// target.
    ///
    /// If creating a target group that only contains a colour target then, optionally,
    /// an efficient depth buffer can be created.
    ///
    /// This is mutable and therefore not thread safe
    ///
    class TargetGroup final
    {
    public:
        
        /// Creates a new TargetGroup
        ///
        /// @param colourTarget
        ///     The texture that will be used as the colour target. must not be null.
        /// @param depthTarget
        ///     The texture that will be used as the depth target. must not be null.
        ///
        /// @return The new render group instance.
        ///
        static TargetGroupUPtr CreateTargetGroup(const TextureCSPtr& colourTarget, const TextureCSPtr& depthTarget) noexcept;
        
        /// Creates a new colour only TargetGroup.
        ///
        /// @param colourTarget
        ///     The texture that will be used as the colour target. must not be null.
        /// @param shouldUseDepthBuffer
        ///     Whether or not an internal, efficient, depth or stencil buffer should be used.
        ///
        /// @return The new render group instance.
        ///
        static TargetGroupUPtr CreateColourTargetGroup(const TextureCSPtr& colourTarget, RenderTargetGroupType type) noexcept;
        
        /// Creates a new depth only TargetGroup
        ///
        /// @param depthTarget
        ///     The texture that will be used as the depth target. must not be null.
        ///
        /// @return The new render group instance.
        ///
        static TargetGroupUPtr CreateDepthTargetGroup(const TextureCSPtr& depthTarget) noexcept;
        
        /// Gets the current RenderTargetGroup. If the colour/depth target has
        /// changed since the last access, then a new RenderTargetGroup will
        /// be created.
        ///
        /// @return The render target group.
        ///
        const RenderTargetGroup* GetRenderTargetGroup() noexcept;
        
        ~TargetGroup() noexcept;
        
    private:
        
        /// @param colourTarget
        ///     The colour render target. Can be null if no colour target is needed.
        /// @param depthTarget
        ///     The depth render target. Can be null if no depth target is needed.
        /// @param type
        ///     Whether or not an internal, efficient, depth or stencil buffer should be used.
        ///
        TargetGroup(const TextureCSPtr& colourTarget, const TextureCSPtr& depthTarget, RenderTargetGroupType type) noexcept;
        
        /// Destroys the render target group if there is one.
        ///
        void DestroyRenderTargetGroup() noexcept;
        
    private:
        
        UniquePtr<RenderTargetGroup> m_renderTargetGroup;
        
        TextureCSPtr m_cachedColourTargetTexture = nullptr;
        TextureCSPtr m_cachedDepthTargetTexture = nullptr;
        
        const RenderTexture* m_cachedColourTargetRenderTexture = nullptr;
        const RenderTexture* m_cachedDepthTargetRenderTexture = nullptr;
        
        RenderTargetGroupType m_type;
    };
}

#endif
