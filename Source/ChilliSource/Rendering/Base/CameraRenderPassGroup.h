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

#ifndef _CHILLISOURCE_RENDERING_BASE_CAMERARENDERPASSGROUP_H_
#define _CHILLISOURCE_RENDERING_BASE_CAMERARENDERPASSGROUP_H_

#include <ChilliSource/ChilliSource.h>

#include <ChilliSource/Rendering/Base/RenderPass.h>
#include <ChilliSource/Rendering/Camera/RenderCamera.h>

#include <vector>

namespace ChilliSource
{
    /// Provides the ability to group a number of render passes based on them sharing
    /// the same camera
    ///
    /// This is immutable and therefore thread-safe
    ///
    class CameraRenderPassGroup final
    {
    public:
        CS_DECLARE_NOCOPY(CameraRenderPassGroup);
        
        CameraRenderPassGroup() = default;
        CameraRenderPassGroup(CameraRenderPassGroup&&) = default;
        CameraRenderPassGroup& operator=(CameraRenderPassGroup&&) = default;
        
        /// @param camera
        ///     The camera to use for this pass
        /// @param renderPasses
        ///     The list of render passes to carry out with the camera. Must be moved.
        ///
        CameraRenderPassGroup(const RenderCamera& camera, std::vector<RenderPass> renderPasses) noexcept;
        
        /// @return The camera to use for rendering the group
        ///
        const RenderCamera& GetCamera() const noexcept { return m_camera; }
        
        /// @return The list of render passes in this group
        ///
        const std::vector<RenderPass>& GetRenderPasses() const noexcept { return m_renderPasses; }
        
    private:
        RenderCamera m_camera;
        std::vector<RenderPass> m_renderPasses;
    };
}

#endif
