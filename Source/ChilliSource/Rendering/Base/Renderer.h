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

#ifndef _CHILLISOURCE_RENDERING_BASE_RENDERER_H_
#define _CHILLISOURCE_RENDERING_BASE_RENDERER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/System/AppSystem.h>

namespace ChilliSource
{
    /// Provodes render API agnostic multi threaded render pipeline. This supports both forward
    /// and deferred rendering.
    ///
    /// The render pipeline consists of five stages:
    ///
    /// * The Scene Snapshot stage queries every Entity in the scene as well as all active
    ///   systems for a snapshot of their current render state. This occurs on the main
    ///   thread.
    ///
    /// * The Frame Compilation stage takes the snapshotted data and processes it into a
    ///   RenderFrame construct, which describes all the information required to render
    ///   a single frame. This is processed in a series of background tasks.
    ///
    /// * The Pass Compilation stage breaks the RenderFrame down into a series of
    ///   RenderPasses ensuring each pass only contains the relevant objects by filtering for
    ///   material type and visibility. Objects within a render pass are also sorted into an
    ///   appropriate order for the type of pass. This also takes into account whether the
    ///   renderer is forward or deferred, compiling appropriate passes for each. This is
    ///   processed in a series of background tasks.
    ///
    /// * The Render Command Queue Compilation stage takes the series of passes and breaks
    ///   them down further into a queue of render commands. This is processed as a series
    ///   of background tasks.
    ///
    /// * The Render Command Processing stage takes the generated command queue and processes
    ///   each depending on render API (i.e OpenGL) that is being used. This is processed on
    ///   the render thread.
    ///
    /// This is thread safe, though certain methods need to be called on certain threads.
    ///
    class Renderer final : public AppSystem
    {
    public:
        CS_DECLARE_NAMEDTYPE(Renderer);
        
        /// Allows querying of whether or not this system implements the interface described by the
        /// given interface Id. Typically this is not called directly as the templated equivalent
        /// IsA<Interface>() is preferred.
        ///
        /// @param interfaceId
        ///     The Id of the interface.
        ///
        /// @return Whether or not the interface is implemented.
        ///
        bool IsA(InterfaceIDType interfaceId) const noexcept override;
        
        /// Performs the Scene Snapshot through to the Render Command Queue Compilation Stages and
        /// then stores the output render command queue render to later be processed by the
        /// ProcessRenderCommandQueue() method.
        ///
        /// If the render pipeline is busy this will block until is it ready for the next snapshot
        /// to be processed.
        ///
        /// This must be called from the main thread.
        ///
        /// @param renderSnapshot
        ///     The render snapshot to process. This should have already been populated by passing it
        ///     to each system and component in the scene. This will be modified internally and should
        ///     not be reused.
        ///
        void ProcessRenderSnapshot(RenderSnapshot& renderSnapshot) noexcept;
        
        /// Processes the next render command queue. If there is no render command queue ready to be
        /// processed then this will block until there is.
        ///
        /// This must be called from the render thread.
        ///
        void ProcessRenderCommandQueue() noexcept;
        
    private:
        friend class Application;
        
        /// A factory method for creating new instances of the system. This must be called by
        /// Application.
        ///
        /// @return The new instance of the system.
        ///
        static RendererUPtr Create() noexcept;
        
        Renderer() noexcept;
    };
}

#endif
