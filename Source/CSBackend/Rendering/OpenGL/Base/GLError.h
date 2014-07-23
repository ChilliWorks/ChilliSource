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

#ifndef _CSBACKEND_RENDERING_OPENGL_BASE_GLERROR_H_
#define _CSBACKEND_RENDERING_OPENGL_BASE_GLERROR_H_

#include <ChilliSource/ChilliSource.h>
#include <CSBackend/Rendering/OpenGL/ForwardDeclarations.h>

#ifdef CS_ENABLE_DEBUG
#define CS_ASSERT_NOGLERROR(in_message) {CSBackend::OpenGL::GLError::AssertNoGLError(in_message);}
#else
#define CS_ASSERT_NOGLERROR(in_message) {}
#endif

namespace CSBackend
{
    namespace OpenGL
    {
        //---------------------------------------------------------
        /// A small collection of utility functions for error
        /// checking in OpenGl.
        ///
        /// @author Ian Copland
        //---------------------------------------------------------
        namespace GLError
        {
            //---------------------------------------------------
            /// When debug mode is enabled (CS_ENABLE_DEBUG) this
            /// prints any Open GL errors that have occurred.
            ///
            /// @author Ian Copland
            //---------------------------------------------------
            void Print();
            //---------------------------------------------------
            /// When debug mode is enabled (CS_ENABLE_DEBUG) this
            /// will assert if a GL error has occurred, printing
            /// the given message.
            ///
            /// @author Ian Copland
            ///
            /// @param The message to print if an error has
            /// occurred.
            //---------------------------------------------------
            void AssertNoGLError(const std::string in_message);
        }
    }
}

#endif
