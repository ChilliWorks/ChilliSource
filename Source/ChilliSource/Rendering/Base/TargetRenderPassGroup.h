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

#ifndef _CHILLISOURCE_RENDERING_BASE_TARGETRENDERPASSGROUP_H_
#define _CHILLISOURCE_RENDERING_BASE_TARGETRENDERPASSGROUP_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/Colour.h>
#include <ChilliSource/Rendering/Base/CameraRenderPassGroup.h>

namespace ChilliSource
{
    /// Provides the ability to group a number of camera render pass groups based on
    /// them sharing the same render target
    ///
    /// This is immutable and therefore thread-safe
    ///
    class TargetRenderPassGroup final
    {
    public:
        CS_DECLARE_NOCOPY(TargetRenderPassGroup);
        
        TargetRenderPassGroup() = default;
        TargetRenderPassGroup(TargetRenderPassGroup&&) = default;
        TargetRenderPassGroup& operator=(TargetRenderPassGroup&&) = default;
        
        /// Creates a new instance which will render to the default render target.
        ///
        /// @param resolution
        ///     The resolution of the screen.
        /// @param clearColour
        ///     The targets clear colour.
        /// @param cameraRenderPassGroups
        ///     The list of camera render pass groups to render to the target. Must be moved.
        ///
        TargetRenderPassGroup(const Integer2& resolution, const Colour& clearColour, std::vector<CameraRenderPassGroup> cameraRenderPassGroups) noexcept;
        
        /// Creates a new instance which will render to the given render target group.
        ///
        /// @param renderTargetGroup
        ///     The RenderTargetGroup that should be rendered into.
        /// @param clearColour
        ///     The targets clear colour.
        /// @param cameraRenderPassGroups
        ///     The list of camera render pass groups to render to the target. Must be moved.
        ///
        TargetRenderPassGroup(const RenderTargetGroup* renderTargetGroup, const Colour& clearColour, std::vector<CameraRenderPassGroup> cameraRenderPassGroups) noexcept;
        
        /// @return The render target group that the contained passes should be applied to. Null
        ///     indicates that the default render target should be used.
        ///
        const RenderTargetGroup* GetRenderTargetGroup() const noexcept { return m_renderTargetGroup; }
        
        /// @return The resolution of the target.
        ///
        const Integer2& GetResolution() const noexcept { return m_resolution; }
        
        /// @return The targets clear colour.
        ///
        const Colour& GetClearColour() const noexcept { return m_clearColour; }
        
        /// @return The list of render camera groups
        ///
        const std::vector<CameraRenderPassGroup>& GetRenderCameraGroups() const noexcept { return m_renderCameraGroups; }

    private:
        const RenderTargetGroup* m_renderTargetGroup = nullptr;
        Integer2 m_resolution;
        Colour m_clearColour;
        std::vector<CameraRenderPassGroup> m_renderCameraGroups;
    };
}

#endif
