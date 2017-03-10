//
//  The MIT License (MIT)
//
//  Copyright (c) 2017 Tag Games Limited
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

#ifndef _CHILLISOURCE_RENDERINGPLATFORM_OPENGL_GLEXTENSIONS_H_
#define _CHILLISOURCE_RENDERINGPLATFORM_OPENGL_GLEXTENSIONS_H_

#include <CSBackend/Rendering/OpenGL/Base/GLIncludes.h>

#if defined CS_TARGETPLATFORM_ANDROID || defined CS_TARGETPLATFORM_RPI

extern PFNGLGENVERTEXARRAYSOESPROC glGenVertexArraysOESEXT;
extern PFNGLBINDVERTEXARRAYOESPROC glBindVertexArrayOESEXT;
extern PFNGLDELETEVERTEXARRAYSOESPROC glDeleteVertexArraysOESEXT;

#   define glGenVertexArraysOES glGenVertexArraysOESEXT
#   define glBindVertexArrayOES glBindVertexArrayOESEXT
#   define glDeleteVertexArraysOES glDeleteVertexArraysOESEXT
#endif

#ifdef CS_OPENGLVERSION_ES
#   define glDeleteVertexArrays glDeleteVertexArraysOES
#   define glBindVertexArray glBindVertexArrayOES
#   define glGenVertexArrays glGenVertexArraysOES

#   define GL_WRITE_ONLY GL_WRITE_ONLY_OES
#   define glMapBuffer glMapBufferOES
#   define glUnmapBuffer glUnmapBufferOES
#endif

namespace CSBackend
{
    namespace OpenGL
    {
        namespace GLExtensions
        {
            /// Grab the functions pointers for any GL extensions required by the current platform
            ///
            void InitExtensions();
        }
    }
}

#endif
