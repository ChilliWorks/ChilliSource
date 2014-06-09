//
//  GLError.cpp
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

#include <CSBackend/Rendering/OpenGL/Base/GLError.h>

#include <CSBackend/Rendering/OpenGL/Base/GLIncludes.h>

#include <vector>

namespace CSBackend
{
    namespace OpenGL
    {
        namespace GLError
        {
            //---------------------------------------------------
            //---------------------------------------------------
            void Print()
            {
#ifdef CS_ENABLE_DEBUG
                //get an array of all the errors that have occurred
                std::vector<GLenum> errors;
                GLenum currentError = glGetError();
                while (currentError != GL_NO_ERROR)
                {
                    errors.push_back(currentError);
                    currentError = glGetError();
                }
                
                //print out the meaning of each error found
                for (const GLenum& error : errors)
                {
                    switch (error)
                    {
                        case GL_INVALID_ENUM:
                            CS_LOG_ERROR("GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument. The offending command is ignored and has no other side effect than to set the error flag.");
                            break;
                        case GL_INVALID_VALUE:
                            CS_LOG_ERROR("GL_INVALID_VALUE: A numeric argument is out of range. The offending command is ignored and has no other side effect than to set the error flag.");
                            break;
                        case GL_INVALID_OPERATION:
                            CS_LOG_ERROR("GL_INVALID_OPERATION: The specified operation is not allowed in the current state. The offending command is ignored and has no other side effect than to set the error flag.");
                            break;
                        case GL_INVALID_FRAMEBUFFER_OPERATION:
                            CS_LOG_ERROR("GL_INVALID_FRAMEBUFFER_OPERATION: The command is trying to render to or read from the framebuffer while the currently bound framebuffer is not framebuffer complete (i.e. the return value from glCheckFramebufferStatus is not GL_FRAMEBUFFER_COMPLETE). The offending command is ignored and has no other side effect than to set the error flag.");
                            break;
                        case GL_OUT_OF_MEMORY:
                            CS_LOG_ERROR("GL_OUT_OF_MEMORY: There is not enough memory left to execute the command. The state of the GL is undefined, except for the state of the error flags, after this error is recorded.");
                            break;
                        case GL_NO_ERROR:
                        default:
                            CS_LOG_ERROR("An unknown OpenGL error has occurred!");
                            break;
                    }
                }
#endif
            }
            //---------------------------------------------------
            //---------------------------------------------------
            void AssertNoGLError(const std::string in_message)
            {
#ifdef CS_ENABLE_DEBUG
                std::vector<GLenum> errors;
                GLenum currentError = glGetError();
                while (currentError != GL_NO_ERROR)
                {
                    errors.push_back(currentError);
                    currentError = glGetError();
                }
                
                if (errors.size() > 0)
                {
                    std::string errorText = in_message + " (";
                    bool first = true;
                    for (const GLenum& error : errors)
                    {
                        if (first == false)
                        {
                            errorText += ", ";
                        }
                        first = false;
                        
                        switch (error)
                        {
                            case GL_INVALID_ENUM:
                                errorText += "GL_INVALID_ENUM";
                                break;
                            case GL_INVALID_VALUE:
                                errorText += "GL_INVALID_VALUE";
                                break;
                            case GL_INVALID_OPERATION:
                                errorText += "GL_INVALID_OPERATION";
                                break;
                            case GL_INVALID_FRAMEBUFFER_OPERATION:
                                errorText += "GL_INVALID_FRAMEBUFFER_OPERATION";
                                break;
                            case GL_OUT_OF_MEMORY:
                                errorText += "GL_OUT_OF_MEMORY";
                                break;
                            case GL_NO_ERROR:
                            default:
                                errorText += "?";
                                break;
                        }
                    }
                    errorText += ")";
                    CS_LOG_FATAL(errorText);
                }
#endif
            }
        }
    }
}
