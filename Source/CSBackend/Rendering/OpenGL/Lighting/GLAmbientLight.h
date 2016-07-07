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

#ifndef _CSBACKEND_RENDERING_OPENGL_LIGHTING_GLAMBIENTLIGHT_H_
#define _CSBACKEND_RENDERING_OPENGL_LIGHTING_GLAMBIENTLIGHT_H_

#include <CSBackend/Rendering/OpenGL/ForwardDeclarations.h>
#include <CSBackend/Rendering/OpenGL/Lighting/GLLight.h>

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/Colour.h>

namespace CSBackend
{
    namespace OpenGL
    {
        /// The ambient OpenGL light object, which stores the ambient light colour and provides
        /// the means to apply this colour to a shader.
        ///
        /// This is immutable and therefore thread-safe, but apply must be called on the render
        /// thread.
        ///
        class GLAmbientLight final : public GLLight
        {
        public:
            /// Creates a new instance with the given colour.
            ///
            /// @param colour
            ///     The ambient light colour.
            ///
            GLAmbientLight(const ChilliSource::Colour& colour) noexcept;
            
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
        };
    }
}

#endif
