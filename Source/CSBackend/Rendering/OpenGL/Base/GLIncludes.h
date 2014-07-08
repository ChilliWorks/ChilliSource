//
//  GLIncludes.h
//  Chilli Source
//  Created by Ian Copland on 14/01/2011.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2011 Tag Games Limited
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

#ifndef _CHILLISOURCE_RENDERINGPLATFORM_OPENGL_GLINCLUDES_H_
#define _CHILLISOURCE_RENDERINGPLATFORM_OPENGL_GLINCLUDES_H_

//---Missing OpenGL definitions
#define GL_GLEXT_PROTOTYPES
#ifdef CS_TARGETPLATFORM_ANDROID
typedef char GLchar;
#endif

#ifdef CS_TARGETPLATFORM_IOS
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#include <OpenGLES/EAGL.h>
#include <OpenGLES/EAGLDrawable.h>
#elif defined CS_TARGETPLATFORM_WINDOWS
#include <glew.h>
#include <SFML/OpenGL.hpp>
#include <CSBackend/Platform/Windows/SFML/Base/SFMLWindow.h>
#else
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif

#if defined CS_TARGETPLATFORM_IOS || defined CS_TARGETPLATFORM_ANDROID
#define CS_OPENGLVERSION_ES
#elif defined CS_TARGETPLATFORM_WINDOWS
#define CS_OPENGLVERSION_STANDARD
#endif

#endif
