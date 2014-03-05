//
//  RenderingCapabilities.cpp
//  FestivalFever
//
//  Created by Tag Games on 27/01/2014.
//  Copyright (c) 2014 Tag Games Ltd. All rights reserved.
//

#include <ChilliSource/Backend/Rendering/OpenGL/Base/RenderCapabilities.h>

#include <ChilliSource/Backend/Rendering/OpenGL/Base/GLIncludes.h>
#include <ChilliSource/Backend/Rendering/OpenGL/Base/RenderSystem.h>

namespace ChilliSource
{
    namespace OpenGL
    {
        CS_DEFINE_NAMEDTYPE(RenderCapabilities);
        //-------------------------------------------------
        /// Constructor
        //-------------------------------------------------
        RenderCapabilities::RenderCapabilities()
        : mbShadowsSupported(false), mbDepthTexturesSupported(false), mudwMaxTextureSize(0), mudwNumTextureUnits(0)
        {
        }
        //-------------------------------------------------
        /// Is A
        //-------------------------------------------------
        bool RenderCapabilities::IsA(Core::InterfaceIDType inInterfaceID) const
        {
            return (Rendering::RenderCapabilities::InterfaceID == inInterfaceID || RenderCapabilities::InterfaceID == inInterfaceID);
        }
        //-------------------------------------------------
        /// Calculate Capabilities
        //-------------------------------------------------
        void RenderCapabilities::CalculateCapabilities()
        {
			mbHighPrecisionInFragmentShaderSupported = true;

#ifdef CS_OPENGLVERSION_ES
            s32 adwFragmentHighRange[2], dwFragmentHighPrecision;
            glGetShaderPrecisionFormat(GL_FRAGMENT_SHADER, GL_HIGH_FLOAT, adwFragmentHighRange, &dwFragmentHighPrecision);
            if (dwFragmentHighPrecision == 0 && adwFragmentHighRange[0] == 0 && adwFragmentHighRange[1] == 0)
            {
                mbHighPrecisionInFragmentShaderSupported = false;
            }
            else
            {
                mbHighPrecisionInFragmentShaderSupported = true;
            }
#endif
            
            mbDepthTexturesSupported = RenderSystem::CheckForOpenGLExtension("GL_OES_depth_texture");
            mbShadowsSupported = (mbDepthTexturesSupported && mbHighPrecisionInFragmentShaderSupported);
            glGetIntegerv(GL_MAX_TEXTURE_SIZE, (s32*)&mudwMaxTextureSize);
            glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, (s32*)&mudwNumTextureUnits);
        }
        //-------------------------------------------------
        /// Is Shadow Mapping Supported
        //-------------------------------------------------
        bool RenderCapabilities::IsShadowMappingSupported() const
        {
            return mbShadowsSupported;
        }
        //-------------------------------------------------
        /// Are Depth Texture Supported
        //-------------------------------------------------
        bool RenderCapabilities::AreDepthTexturesSupported() const
        {
            return mbDepthTexturesSupported;
        }
        //-------------------------------------------------
        /// Get Max Texture Size
        //-------------------------------------------------
        u32 RenderCapabilities::GetMaxTextureSize() const
        {
            return mudwMaxTextureSize;
        }
        //-------------------------------------------------
        /// Get Num Texture Units
        //-------------------------------------------------
        u32 RenderCapabilities::GetNumTextureUnits() const
        {
            return mudwNumTextureUnits;
        }
    }
}
