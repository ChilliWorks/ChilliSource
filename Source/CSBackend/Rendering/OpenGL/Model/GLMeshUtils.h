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

#ifndef _CSBACKEND_RENDERING_OPENGL_MODEL_GLMESHUTILS_H_
#define _CSBACKEND_RENDERING_OPENGL_MODEL_GLMESHUTILS_H_

#include <CSBackend/Rendering/OpenGL/ForwardDeclarations.h>
#include <CSBackend/Rendering/OpenGL/Base/GLIncludes.h>

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Rendering/Model/VertexFormat.h>

namespace CSBackend
{
    namespace OpenGL
    {
        /// A series of utility methods pertaining to OpenGL meshes, such as conversions
        /// from ChilliSource formats to OpenGL formats.
        ///
        /// This is not thread-safe and should only be accessed from the render thread.
        ///
        namespace GLMeshUtils
        {
            /// @param dataType
            ///     The vertex element data type.
            ///
            /// @return the OpenGL type for the requested data type.
            ///
            GLenum GetGLType(ChilliSource::VertexFormat::DataType dataType) noexcept;
            
            /// @param elementType
            ///     The vertex element type.
            ///
            /// @return Whether or not the value is normalised for the given element type.
            ///
            GLboolean IsNormalised(ChilliSource::VertexFormat::ElementType elementType) noexcept;
            
            /// @param elementType
            ///     The vertex element type.
            ///
            /// @return The attribute name for the given element type.
            ///
            const char* GetAttributeName(ChilliSource::VertexFormat::ElementType elementType) noexcept;
        }
    }
}

#endif
