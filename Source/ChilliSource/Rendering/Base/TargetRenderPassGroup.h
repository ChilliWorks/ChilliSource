//
//  The MIT License (MIT)
//
//  Copyright (c) 2010 Tag Games Limited
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

#include <ChilliSource/Rendering/Base/CameraRenderPassGroup.h>

namespace ChilliSource
{
    namespace Rendering
    {
        //TODO::Remove when implemented
        class RenderTarget {};
        
        /// Provides the ability to group a number of camera render pass groups based on
        /// them sharing the same render target
        ///
        /// This is immutable and therefore thread-safe
        ///
        class TargetRenderPassGroup final
        {
        public:
            
            ///
            /// @param renderTarget
            ///     The render target to render the group to
            /// @param cameraRenderPassGroups
            ///     The list of camera render pass groups to render to the target
            ///
            TargetRenderPassGroup(const RenderTarget& renderTarget, const std::vector<CameraRenderPassGroup>& cameraRenderPassGroups) noexcept;
            
            ///
            /// @return The render target to use for rendering the group
            ///
            const RenderTarget& GetRenderTarget() const noexcept { return m_renderTarget; }
            
            ///
            /// @return The list of render camera groups
            ///
            const std::vector<CameraRenderPassGroup>& GetRenderCameraGroups() const noexcept { return m_renderCameraGroups; }
            
        private:
            RenderTarget m_renderTarget;
            std::vector<CameraRenderPassGroup> m_renderCameraGroups;
        };
    }
}

#endif
