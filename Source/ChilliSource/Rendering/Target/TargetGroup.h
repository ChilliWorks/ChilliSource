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

namespace ChilliSource
{
    /// Used to create and manage access to a RenderTargetGroup.
    ///
    /// If the colour/depth target has changed since the last access, then a
    /// new RenderTargetGroup will be created.
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
        static TargetGroupUPtr CreateTargetGroup(const Texture* colourTarget, const Texture* depthTarget) noexcept;
        
        /// Creates a new colour only TargetGroup.
        ///
        /// @param colourTarget
        ///     The texture that will be used as the colour target. must not be null.
        /// @param shouldUseDepthBuffer
        ///     Whether or not an internal, efficient, depth buffer should be used.
        ///
        /// @return The new render group instance.
        ///
        static TargetGroupUPtr CreateColourTargetGroup(const Texture* colourTarget, bool shouldUseDepthBuffer = true) noexcept;
        
        /// Creates a new depth only TargetGroup
        ///
        /// @param depthTarget
        ///     The texture that will be used as the depth target. must not be null.
        ///
        /// @return The new render group instance.
        ///
        static TargetGroupUPtr CreateDepthTargetGroup(const Texture* depthTarget) noexcept;
        
        /// @return The render target group.
        ///
        const RenderTargetGroup* GetRenderTargetGroup() noexcept;
        
        /// Destroys the render target group if there is one.
        ///
        void DestroyRenderTargetGroup() noexcept;
        
    private:
        
        /// @param colourTarget
        ///     The colour render target. Can be null if no colour target is needed.
        /// @param depthTarget
        ///     The depth render target. Can be null if no depth target is needed.
        /// @param shouldUseDepthBuffer
        ///     Whether or not to use an internal depth buffer if no depth buffer was specified.
        ///     This should always be false if a depth target was supplied.
        ///
        TargetGroup(const Texture* colourTarget, const Texture* depthTarget, bool shouldUseDepthBuffer) noexcept;
        
    private:
        
        const RenderTargetGroup* m_renderTargetGroup = nullptr;
        
        const Texture* m_cachedColourTargetTexture = nullptr;
        const Texture* m_cachedDepthTargetTexture = nullptr;
        
        const RenderTexture* m_cachedColourTargetRenderTexture = nullptr;
        const RenderTexture* m_cachedDepthTargetRenderTexture = nullptr;
        
        bool m_shouldUseDepthBuffer = true;
    };
}

#endif
