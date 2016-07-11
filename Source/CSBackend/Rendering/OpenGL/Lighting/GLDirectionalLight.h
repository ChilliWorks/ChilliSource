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

#ifndef _CSBACKEND_RENDERING_OPENGL_LIGHTING_GLDIRECTIONALLIGHT_H_
#define _CSBACKEND_RENDERING_OPENGL_LIGHTING_GLDIRECTIONALLIGHT_H_

#include <CSBackend/Rendering/OpenGL/ForwardDeclarations.h>
#include <CSBackend/Rendering/OpenGL/Lighting/GLLight.h>

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/Colour.h>
#include <ChilliSource/Core/Math/Matrix4.h>
#include <ChilliSource/Core/Math/Vector3.h>

namespace CSBackend
{
    namespace OpenGL
    {
        /// The directional OpenGL light object, which stores the direction light data and provides
        /// the means to apply the data to a shader.
        ///
        /// This is immutable and therefore thread-safe, but apply must be called on the render
        /// thread.
        ///
        class GLDirectionalLight final : public GLLight
        {
        public:
            /// Creates a new instance with the given directional light information. 
            ///
            /// @param colour
            ///     The colour of the light.
            /// @param direction
            ///     The direction of the light.
            /// @param lightViewProjection
            ///     The view projection matrix of the light which is used as the camera when rendering
            ///     the shadow map.
            /// @param shadowTolerance
            ///     The tolerence used to judge if an object is in shadow.
            /// @param shadowMapRenderTexture
            ///     The render texture which should be used for the shadow map. Can be null if there is no
            ///     shadow map.
            ///
            GLDirectionalLight(const ChilliSource::Colour& colour, const ChilliSource::Vector3& direction, const ChilliSource::Matrix4& lightViewProjection, f32 shadowTolerance,
                               const ChilliSource::RenderTexture* shadowMapRenderTexture) noexcept;
            
            /// Applies the light to the given shader.
            ///
            /// This must be called on the render thread.
            ///
            /// @param glShader
            ///     The shader the light data should be applied to.
            /// @param glTextureUnitManager
            ///     The texture unit manager which can be used to bind additional textures required by a light
            ///     such as a shadow map.
            ///
            void Apply(GLShader* glShader, GLTextureUnitManager* glTextureUnitManager) const noexcept override;
            
        private:
            ChilliSource::Colour m_colour;
            ChilliSource::Vector3 m_direction;
            ChilliSource::Matrix4 m_lightViewProjection;
            f32 m_shadowTolerance;
            const ChilliSource::RenderTexture* m_shadowMapRenderTexture;
        };
    }
}

#endif
