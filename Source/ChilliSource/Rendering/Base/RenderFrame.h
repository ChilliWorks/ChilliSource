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

#ifndef _CHILLISOURCE_RENDERING_BASE_RENDERFRAME_H_
#define _CHILLISOURCE_RENDERING_BASE_RENDERFRAME_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/Colour.h>
#include <ChilliSource/Core/Math/Vector2.h>
#include <ChilliSource/Rendering/Base/RenderObject.h>
#include <ChilliSource/Rendering/Camera/RenderCamera.h>
#include <ChilliSource/Rendering/Lighting/AmbientRenderLight.h>
#include <ChilliSource/Rendering/Lighting/DirectionalRenderLight.h>
#include <ChilliSource/Rendering/Lighting/PointRenderLight.h>

#include <vector>

namespace ChilliSource
{
    /// A container for the camera and all lights and objects within a single frame.
    ///
    /// This is immutable and therefore thread safe.
    ///
    class RenderFrame final
    {
    public:
        
        RenderFrame() = default;
        
        /// Creates a new instance with the given camera, lights and objects.
        ///
        /// @param renderTarget
        ///     The render target to render into, if null renders to screen (frame buffer)
        /// @param resolution
        ///     The resolution of the viewport.
        /// @param clearColour
        ///     The clear colour of the default render target.
        /// @param renderCamera
        ///     The camera used for the frame.
        /// @param renderAmbientLight
        ///     An ambient light that represents the combination of all ambient lights in the
        ///     scene.
        /// @param renderDirectionalLights
        ///     A list of directional lights in the frame.
        /// @param renderPointLights
        ///     A list of point lights in the frame.
        /// @param renderObjects
        ///     A list of objects in the frame.
        ///
        RenderFrame(const RenderTargetGroup* renderTarget, const Integer2& resolution, const Colour& clearColour, const RenderCamera& renderCamera, const AmbientRenderLight& renderAmbientLight, const std::vector<DirectionalRenderLight>& renderDirectionalLights,
                    const std::vector<PointRenderLight>& renderPointLights, const std::vector<RenderObject>& renderObjects) noexcept;
        
        /// @return The resolution of the viewport.
        ///
        const Integer2& GetResolution() const noexcept { return m_resolution; }
        
        /// @return The clear colour of the default render target.
        ///
        const Colour& GetClearColour() const noexcept { return m_clearColour; }
        
        /// @return The camera used for the frame.
        ///
        const RenderCamera& GetRenderCamera() const noexcept { return m_renderCamera; }
        
        /// @return An ambient light that represents the combination of all ambient lights in the
        ///     scene.
        ///
        const AmbientRenderLight& GetAmbientRenderLight() const noexcept { return m_renderAmbientLight; }
        
        /// @return A list of directional lights in the frame.
        ///
        const std::vector<DirectionalRenderLight>& GetDirectionalRenderLights() const noexcept { return m_renderDirectionalLights; }
        
        /// @return A list of point lights in the frame.
        ///
        const std::vector<PointRenderLight>& GetPointRenderLights() const noexcept { return m_renderPointLights; }
        
        /// @return A list of objects in the frame.
        ///
        const std::vector<RenderObject>& GetRenderObjects() const noexcept { return m_renderObjects; }
        
        ///@return Render target for this frame, if null defaults to the frame buffer
        ///
        const RenderTargetGroup* GetOffscreenRenderTarget() const noexcept { return m_offscreenRenderTarget; }
        
    private:
        Integer2 m_resolution;
        Colour m_clearColour;
        RenderCamera m_renderCamera;
        AmbientRenderLight m_renderAmbientLight;
        std::vector<DirectionalRenderLight> m_renderDirectionalLights;
        std::vector<PointRenderLight> m_renderPointLights;
        std::vector<RenderObject> m_renderObjects;
        const RenderTargetGroup* m_offscreenRenderTarget;
    };
}

#endif
