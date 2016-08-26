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

#ifndef _CHILLISOURCE_RENDERING_BASE_FORWARDRENDERPASSCOMPILER_H_
#define _CHILLISOURCE_RENDERING_BASE_FORWARDRENDERPASSCOMPILER_H_

#include <ChilliSource/ChilliSource.h>

#include <ChilliSource/Rendering/Base/IRenderPassCompiler.h>
#include <ChilliSource/Rendering/Base/CameraRenderPassGroup.h>

namespace ChilliSource
{
    /// Compiles the RenderPasses required when forward rendering. See ForwardRenderPasses.h for more
    /// information on these passes.
    ///
    /// This breaks the RenderFrame down into a series of RenderPasses ensuring each pass only contains
    /// the relevant objects by filtering for material type and visibility. Objects within a render pass
    /// are also sorted into an appropriate order for the type of pass. These render passes are contained
    /// by a TargetRenderPassGroup, which groups passes based on the framebuffer they are targetting.
    /// All this is processed in a series of background tasks.
    ///
    class ForwardRenderPassCompiler final : public IRenderPassCompiler
    {
    public:
        
        /// Gather all render objects in the frame into a list of target render pass groups.
        ///
        /// @param taskContext
        ///     Context to manage any spawned tasks
        /// @param renderFrames
        ///     Current frame data
        ///
        /// @return The list of target render pass groups
        ///
        std::vector<TargetRenderPassGroup> CompileTargetRenderPassGroups(const TaskContext& taskContext, std::vector<RenderFrame>&& renderFrames) noexcept override;
    };
}

#endif
