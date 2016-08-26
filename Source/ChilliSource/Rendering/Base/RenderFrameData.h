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

#ifndef _CHILLISOURCE_RENDERING_BASE_RENDERFRAMEDATA_H_
#define _CHILLISOURCE_RENDERING_BASE_RENDERFRAMEDATA_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Memory/IAllocator.h>
#include <ChilliSource/Rendering/Model/RenderDynamicMesh.h>
#include <ChilliSource/Rendering/Model/RenderSkinnedAnimation.h>

#include <vector>

namespace ChilliSource
{
    /// A container for data which needs to persist for the full length of a frame, being passed
    /// down the render pipeline. This includes dynamically created meshes and animation data.
    /// This data is typically allocated frame allocator, so a handle to it is also held.
    ///
    /// This is not thread-safe and must only be accessed by one thread at a time.
    ///
    class RenderFrameData final
    {
    public:
        CS_DECLARE_NOCOPY(RenderFrameData);
        
        RenderFrameData(RenderFrameData&&) = default;
        RenderFrameData& operator=(RenderFrameData&&) = default;
        RenderFrameData() noexcept;
        
        /// Adds a RenderDynamicMesh. This will be deleted at the end of the frame.
        ///
        /// @param renderDynamicMesh
        ///     The render dynamic mesh.
        ///
        void AddRenderDynamicMesh(RenderDynamicMeshAUPtr renderDynamicMesh) noexcept;
        
        /// Adds a RenderSkinnedAnimation. This will be deleted at the end of the frame.
        ///
        /// @param renderSkinnedAnimation
        ///     The render skinned animation.
        ///
        void AddRenderSkinnedAnimation(RenderSkinnedAnimationAUPtr renderSkinnedAnimation) noexcept;
        
    private:
        std::vector<RenderDynamicMeshAUPtr> m_renderDynamicMeshes;
        std::vector<RenderSkinnedAnimationAUPtr> m_renderSkinnedAnimations;
    };
};

#endif
