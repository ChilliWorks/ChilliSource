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

#ifndef _CHILLISOURCE_RENDERING_BASE_RENDERFRAMECOMPILER_H_
#define _CHILLISOURCE_RENDERING_BASE_RENDERFRAMECOMPILER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Rendering/Base/RenderFrame.h>

#include <vector>

namespace ChilliSource
{
    /// A container for functions relating to the Compile Render Frame stage of the
    /// render pipeline.
    ///
    namespace RenderFrameCompiler
    {
        /// Performs the Compile Frame Frame state of the render pipeline. This takes
        /// all render primitives from the render snapshot phase and compiles them into
        /// a single immutable construct. During compilation all render jobs are performed
        /// to generate additional render primitives.
        ///
        /// @param renderTarget
        ///     The render target to render into, if null renders to screen (frame buffer)
        /// @param resolution
        ///     The resolution of the viewport.
        /// @param clearColour
        ///     The clar colour of the screen.
        /// @param renderCamera
        ///     The camera used for the frame.
        /// @param renderAmbientLights
        ///     A list of ambient lights in the frame.
        /// @param renderDirectionalLights
        ///     A list of directional lights in the frame.
        /// @param renderPointLights
        ///     A list of point lights in the frame.
        /// @param renderObjects
        ///     A list of objects in the frame.
        ///
        /// @return The compiled render frame.
        ///
        RenderFrame CompileRenderFrame(const RenderTargetGroup* renderTarget, const Integer2& resolution, const Colour& clearColour, const RenderCamera& renderCamera, const std::vector<AmbientRenderLight>& renderAmbientLights,
                                       const std::vector<DirectionalRenderLight>& renderDirectionalLights, const std::vector<PointRenderLight>& renderPointLights,
                                       const std::vector<RenderObject>& renderObjects) noexcept;
    }
}

#endif
