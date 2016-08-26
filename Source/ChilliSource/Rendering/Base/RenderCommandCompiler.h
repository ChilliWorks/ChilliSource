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

#ifndef _CHILLISOURCE_RENDERING_BASE_RENDERCOMMANDCOMPILER_H_
#define _CHILLISOURCE_RENDERING_BASE_RENDERCOMMANDCOMPILER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Rendering/RenderCommand/RenderCommandBuffer.h>

#include <vector>

namespace ChilliSource
{
    /// A container for functions relating to the Compile Render Commands stage of the
    /// render pipeline.
    ///
    namespace RenderCommandCompiler
    {
        /// Performs the Compile Render Commands stage of the render pipeline. This takes the
        /// render passes generated during the Compile Render Passes stage and breaks them down
        /// into a series of render commands. The output is an immutable render command buffer.
        ///
        /// @param frameAllocator
        ///     Allocator that was used to build this render frame
        /// @param taskContext
        ///     The task context that child tasks should be run within. This assumes that the
        ///     method is being called from within another task, as per the render pipeline
        ///     design.
        /// @param targetRenderPassGroups
        ///     The list of TargetRenderPassGroups containing all of the render passes.
        /// @param preRenderCommandList
        ///     The RenderCommandList that should be included first. Must be moved.
        /// @param postRenderCommandList
        ///     The RenderCommandList that should be included last. Must be moved.
        /// @param renderFramesData
        ///     List of containers for all data allocated this frame that must persist to the end of the
        ///     render pipeline. Must be moved.
        ///
        /// @return The render command buffer.
        ///
        RenderCommandBufferUPtr CompileRenderCommands(const TaskContext& taskContext, IAllocator* frameAllocator, const std::vector<TargetRenderPassGroup>& targetRenderPassGroups, RenderCommandListUPtr preRenderCommandList,
                                                      RenderCommandListUPtr postRenderCommandList, std::vector<RenderFrameData> renderFramesData) noexcept;
    }
}

#endif
