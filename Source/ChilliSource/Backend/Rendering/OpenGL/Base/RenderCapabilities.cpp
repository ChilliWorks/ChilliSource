//
//  RenderingCapabilities.cpp
//  FestivalFever
//
//  Created by Tag Games on 27/01/2014.
//  Copyright (c) 2014 Tag Games Ltd. All rights reserved.
//

#include <ChilliSource/Backend/Rendering/OpenGL/Base/RenderCapabilities.h>
#include <ChilliSource/Backend/Rendering/OpenGL/Base/RenderSystem.h>
#include <ChilliSource/Backend/Rendering/OpenGL/Base/GLIncludes.h>

namespace moFlo
{
    namespace OpenGL
    {
        DEFINE_NAMED_INTERFACE(CRenderCapabilities);
        //-------------------------------------------------
        /// Constructor
        //-------------------------------------------------
        CRenderCapabilities::CRenderCapabilities()
        : mbShadowsSupported(false), mbDepthTexturesSupported(false), mudwMaxTextureSize(0), mudwNumTextureUnits(0)
        {
        }
        //-------------------------------------------------
        /// Is A
        //-------------------------------------------------
        bool CRenderCapabilities::IsA(Core::InterfaceIDType inInterfaceID) const
        {
            return (Rendering::IRenderCapabilities::InterfaceID == inInterfaceID || CRenderCapabilities::InterfaceID == inInterfaceID);
        }
        //-------------------------------------------------
        /// Calculate Capabilities
        //-------------------------------------------------
        void CRenderCapabilities::CalculateCapabilities()
        {
			mbHighPrecisionInFragmentShaderSupported = true;

#ifdef MOFLOW_OPENGLES2
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
            
            mbDepthTexturesSupported = CRenderSystem::CheckForOpenGLExtension("GL_OES_depth_texture");
            mbShadowsSupported = (mbDepthTexturesSupported && mbHighPrecisionInFragmentShaderSupported);
            glGetIntegerv(GL_MAX_TEXTURE_SIZE, (s32*)&mudwMaxTextureSize);
            glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, (s32*)&mudwNumTextureUnits);
        }
        //-------------------------------------------------
        /// Is Shadow Mapping Supported
        //-------------------------------------------------
        bool CRenderCapabilities::IsShadowMappingSupported() const
        {
            return mbShadowsSupported;
        }
        //-------------------------------------------------
        /// Are Depth Texture Supported
        //-------------------------------------------------
        bool CRenderCapabilities::AreDepthTexturesSupported() const
        {
            return mbDepthTexturesSupported;
        }
        //-------------------------------------------------
        /// Get Max Texture Size
        //-------------------------------------------------
        u32 CRenderCapabilities::GetMaxTextureSize() const
        {
            return mudwMaxTextureSize;
        }
        //-------------------------------------------------
        /// Get Num Texture Units
        //-------------------------------------------------
        u32 CRenderCapabilities::GetNumTextureUnits() const
        {
            return mudwNumTextureUnits;
        }
    }
}
