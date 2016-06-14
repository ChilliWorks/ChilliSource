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

#ifndef _CSBACKEND_RENDERING_OPENGL_BASE_RENDERCOMMANDPROCESSOR_H_
#define _CSBACKEND_RENDERING_OPENGL_BASE_RENDERCOMMANDPROCESSOR_H_

#include <CSBackend/Rendering/OpenGL/ForwardDeclarations.h>

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Rendering/Base/IRenderCommandProcessor.h>

namespace CSBackend
{
    namespace OpenGL
    {
        /// An OpenGL implementation of the IRenderCommandProcessor interface. This processes
        /// queues of render commands using the OpenGL render API.
        ///
        /// This is not thread-safe and must be called on the render thread.
        ///
        class RenderCommandProcessor final : public ChilliSource::IRenderCommandProcessor
        {
        public:
            RenderCommandProcessor() = default;
            
            /// Processes the given render command queue, performing the required actions for
            /// the command using OpenGL.
            ///
            /// @param renderCommandQueue
            ///     The queue of render commands that should be processed.
            ///
            void Process(const ChilliSource::RenderCommandQueue& renderCommandQueue) noexcept override;
        };
    }
}

#endif
