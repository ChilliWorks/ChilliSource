//
//  RenderingCapabilities.cpp
//  Chilli Source
//  Created by Ian Copland on 27/01/2014.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2010 Tag Games Limited
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

#include <CSBackend/Rendering/OpenGL/Base/RenderCapabilities.h>

#include <CSBackend/Rendering/OpenGL/Base/GLError.h>
#include <CSBackend/Rendering/OpenGL/Base/GLIncludes.h>

namespace CSBackend
{
    namespace OpenGL
    {
        namespace
        {
            //----------------------------------------------------------
            /// Check for OpenGL Extension support
            ///
            /// @author Ian Copland
            ///
            /// @param Extension ID
            ///
            /// @return Whether it is supported
            //----------------------------------------------------------
            bool CheckForOpenGLExtension(const std::string& in_extension)
            {
                std::string extensions = (const char*)glGetString(GL_EXTENSIONS);
                return extensions.find(in_extension) != extensions.npos;
            }
        }
        
        CS_DEFINE_NAMEDTYPE(RenderCapabilities);
        //-------------------------------------------------
        //-------------------------------------------------
        RenderCapabilities::RenderCapabilities()
        : m_areShadowMapsSupported(false), m_areDepthTexturesSupported(false), m_areMapBuffersSupported(false), m_maxTextureSize(0), m_maxTextureUnits(0)
        {
        }
        //-------------------------------------------------
        //-------------------------------------------------
        bool RenderCapabilities::IsA(CSCore::InterfaceIDType in_interfaceId) const
        {
            return (CSRendering::RenderCapabilities::InterfaceID == in_interfaceId || RenderCapabilities::InterfaceID == in_interfaceId);
        }
        //-------------------------------------------------
        //-------------------------------------------------
        void RenderCapabilities::DetermineCapabilities()
        {
			bool areHighPrecFragmentsSupported = true;
            m_areMapBuffersSupported = true;

#ifdef CS_OPENGLVERSION_ES
            s32 fragmentHighRanges[2];
            s32 fragmentHighPrecision;
            
            glGetShaderPrecisionFormat(GL_FRAGMENT_SHADER, GL_HIGH_FLOAT, fragmentHighRanges, &fragmentHighPrecision);
            areHighPrecFragmentsSupported = fragmentHighPrecision != 0 && fragmentHighRanges[0] != 0 && fragmentHighRanges[1] != 0;
            
#ifdef CS_TARGETPLATFORM_ANDROID
            //Check for map buffer support
            m_areMapBuffersSupported = CheckForOpenGLExtension("GL_OES_mapbuffer");
#endif
            
#endif

#ifdef CS_OPENGLVERSION_STANDARD
			m_areDepthTexturesSupported = CheckForOpenGLExtension("GL_ARB_depth_texture");
#elif defined(CS_OPENGLVERSION_ES)
			m_areDepthTexturesSupported = CheckForOpenGLExtension("GL_OES_depth_texture");
#endif
            m_areShadowMapsSupported = (m_areDepthTexturesSupported && areHighPrecFragmentsSupported);
            glGetIntegerv(GL_MAX_TEXTURE_SIZE, (s32*)&m_maxTextureSize);
            glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, (s32*)&m_maxTextureUnits);
            
            CS_ASSERT_NOGLERROR("An OpenGL error occurred while getting render capabilities.");
        }
        //-------------------------------------------------
        //-------------------------------------------------
        bool RenderCapabilities::IsShadowMappingSupported() const
        {
            return m_areShadowMapsSupported;
        }
        //-------------------------------------------------
        //-------------------------------------------------
        bool RenderCapabilities::IsDepthTextureSupported() const
        {
            return m_areDepthTexturesSupported;
        }
        //-------------------------------------------------
        //-------------------------------------------------
        bool RenderCapabilities::IsMapBufferSupported() const
        {
            return m_areMapBuffersSupported;
        }
        //-------------------------------------------------
        //-------------------------------------------------
        u32 RenderCapabilities::GetMaxTextureSize() const
        {
            return m_maxTextureSize;
        }
        //-------------------------------------------------
        //-------------------------------------------------
        u32 RenderCapabilities::GetNumTextureUnits() const
        {
            return m_maxTextureUnits;
        }
    }
}
