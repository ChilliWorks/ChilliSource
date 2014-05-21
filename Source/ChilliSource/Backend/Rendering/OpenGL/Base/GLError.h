//
//  GLError.h
//  Chilli Source
//  Created by Ian Copland on 21/05/2014.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2014 Tag Games Limited
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

#ifndef _CHILLISOURCE_BACKEND_RENDERING_OPENGL_BASE_GLUTILS_H_
#define _CHILLISOURCE_BACKEND_RENDERING_OPENGL_BASE_GLUTILS_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Backend/Rendering/OpenGL/ForwardDeclarations.h>

#ifdef CS_ENABLE_DEBUG
#define CS_OPENGL_ASSERT(in_message) {ChilliSource::OpenGL::GLError::Assert(in_message);}
#else
#define CS_OPENGL_ASSERT(in_message) {}
#endif

namespace ChilliSource
{
    namespace OpenGL
    {
        //---------------------------------------------------------
        /// A small collection of utility functions for error
        /// checking in OpenGl.
        ///
        /// @author I Copland
        //---------------------------------------------------------
        namespace GLError
        {
            //---------------------------------------------------
            /// When debug mode is enabled (CS_ENABLE_DEBUG) this
            /// prints any Open GL errors that have occurred.
            ///
            /// @author I Copland
            //---------------------------------------------------
            void Print();
            //---------------------------------------------------
            /// When debug mode is enabled (CS_ENABLE_DEBUG) this
            /// will assert if a GL error has occurred, printing
            /// the given message.
            ///
            /// @author I Copland
            ///
            /// @param The message to print if an error has
            /// occurred.
            //---------------------------------------------------
            void Assert(const std::string in_message);
        }
    }
}

#endif
