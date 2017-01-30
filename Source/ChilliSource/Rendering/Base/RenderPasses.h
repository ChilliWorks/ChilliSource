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

#ifndef _CHILLISOURCE_RENDERING_BASE_RENDERPASSES_H_
#define _CHILLISOURCE_RENDERING_BASE_RENDERPASSES_H_

#include <ChilliSource/ChilliSource.h>

namespace ChilliSource
{
    /// An enum describing each of the passes in the multi-pass renderer. This consists of
    ///
    /// * The shadow map pass. This renders all shadow casting opaque objects in the scene to
    ///   acquire the shadow map used by each shadow casting light.
    /// * The base pass. This renders all opaque objects to the scene with only ambient lighting.
    /// * The directional light pass. For each directional light in the scene which doesn't cast
    ///   shadows, a pass over all lit, opaque objects in the scene is performed. This renders the
    ///   applied lighting additively to the base pass and does not write to the depth buffer.
    /// * The directional light shadows pass. For each directional light in the scene which does
    ///   cast shadows, a pass over all lit, opaque objects in the scene is performed. This renders
    ///   the applied lighting additively to the base pass and does not write to the depth buffer.
    /// * The point light pass. For each point light in the scene, a pass over all lit opaque objects
    ///   within the range of influence of the light is performed. This renders the applied lighting
    ///   additively to the previous passes and does not write to the depth buffer.
    /// * The transparent pass. This render all transparent objects in the scene with only ambient
    ///   lighting. The depth buffer is not written to; the objects are first sorted to ensure no
    ///   artefacts occur.
    /// * The skybox pass. This renders any skyboxes after the rest of the scene has been rendered and relies
    ///   on the material settings to ensure that the skybox appears beind the rest of the scene. The skybox
    ///   is rendered using the camera orientation but does not take into account position.
    ///
    enum class RenderPasses
    {
        k_shadowMap,
        k_base,
        k_directionalLight,
        k_directionalLightShadows,
        k_pointLight,
        k_transparent,
        k_skybox
    };
}

#endif
