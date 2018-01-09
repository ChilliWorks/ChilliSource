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

#include <CSBackend/Rendering/OpenGL/Base/RenderInfoFactory.h>

#include <CSBackend/Rendering/OpenGL/Base/GLError.h>
#include <CSBackend/Rendering/OpenGL/Base/GLIncludes.h>

namespace CSBackend
{
    namespace OpenGL
    {
        namespace
        {
            /// Check for OpenGL Extension support
            ///
            /// @param extension
            ///     The extension to check
            ///
            /// @return Whether it is supported
            ///
            bool CheckForOpenGLExtension(const std::string& extension)
            {
                std::string extensions = (const char*)glGetString(GL_EXTENSIONS);
                return extensions.find(extension) != extensions.npos;
            }
        }
        
        //-------------------------------------------------------
        ChilliSource::RenderInfo RenderInfoFactory::CreateRenderInfo() noexcept
        {
            bool areHighPrecFragmentsSupported = true;
            bool areMapBuffersSupported = true;
            bool areVAOsSupported = true;
            bool areDepthTexturesSupported = false;
            bool areShadowMapsSupported = false;
            
            u32 maxTextureSize = 0;
            u32 maxTextureUnits = 0;
            u32 maxVertexAttribs = 0;
            
#ifdef CS_OPENGLVERSION_ES
            s32 fragmentHighRanges[2];
            s32 fragmentHighPrecision;
            
            glGetShaderPrecisionFormat(GL_FRAGMENT_SHADER, GL_HIGH_FLOAT, fragmentHighRanges, &fragmentHighPrecision);
            areHighPrecFragmentsSupported = fragmentHighPrecision != 0 && fragmentHighRanges[0] != 0 && fragmentHighRanges[1] != 0;
            
#ifdef CS_TARGETPLATFORM_ANDROID
            //Check for map buffer support
            areMapBuffersSupported = CheckForOpenGLExtension("GL_OES_mapbuffer");
            areVAOsSupported = CheckForOpenGLExtension("GL_OES_vertex_array_object");
#endif
            
#ifdef CS_TARGETPLATFORM_RPI
            areVAOsSupported = false;
#endif
            
#endif
            
#ifdef CS_OPENGLVERSION_STANDARD
            areDepthTexturesSupported = CheckForOpenGLExtension("GL_ARB_depth_texture");
#elif defined(CS_OPENGLVERSION_ES)
            areDepthTexturesSupported = CheckForOpenGLExtension("GL_OES_depth_texture");
#endif
            areShadowMapsSupported = (areDepthTexturesSupported && areHighPrecFragmentsSupported);
            glGetIntegerv(GL_MAX_TEXTURE_SIZE, (GLint*)&maxTextureSize);
            glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, (GLint*)&maxTextureUnits);
            glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, (GLint*)&maxVertexAttribs);
            
            CS_ASSERT_NOGLERROR("An OpenGL error occurred while getting render capabilities.");
            
            ChilliSource::RenderInfo renderInfo(areShadowMapsSupported, areDepthTexturesSupported, areMapBuffersSupported, areVAOsSupported, areHighPrecFragmentsSupported, maxTextureSize, maxTextureUnits, maxVertexAttribs);
            
            return renderInfo;
        }
    }
} 
