//
//  RenderCapabilities.h
//  moFlow
//
//  Created by Ian Copland on 27/01/2014.
//  Copyright (c) 2014 Tag Games Ltd. All rights reserved.
//

#ifndef _MOFLOW_RENDERINGPLATFORM_OPENGL_RENDERCAPABILITIES_H_
#define _MOFLOW_RENDERINGPLATFORM_OPENGL_RENDERCAPABILITIES_H_

#include <ChilliSource/RenderingPlatform/OpenGL/ForwardDeclarations.h>
#include <ChilliSource/Rendering/RenderCapabilities.h>


namespace moFlo
{
    namespace OpenGL
    {
        //========================================================
        /// Render Capabilities
        ///
        /// Describes the opengl rendering capabilities available
        /// on the running device. This cannot be used until
        /// Init() is called on the render system.
        //========================================================
        class CRenderCapabilities : public Rendering::IRenderCapabilities
        {
        public:
            DECLARE_NAMED_INTERFACE(CRenderCapabilities);
            //-------------------------------------------------
			/// Constructor
			//-------------------------------------------------
			CRenderCapabilities();
            //-------------------------------------------------
			/// Is A
			///
			/// Query the interface type
            ///
			/// @param The interface to compare
			/// @return Whether the object implements that
            /// interface
			//-------------------------------------------------
			bool IsA(Core::InterfaceIDType inInterfaceID) const;
            //-------------------------------------------------
			/// Calculate Capabilities
            ///
            /// Calculates each of the capabilities. This needs
            /// to be called after the render system has been
            /// initialised.
			//-------------------------------------------------
			void CalculateCapabilities();
            //-------------------------------------------------
            /// Is Shadow Mapping Supported
            ///
            /// @return Whether or not shadow mapping is
            /// supported.
            //-------------------------------------------------
            bool IsShadowMappingSupported() const;
            //-------------------------------------------------
            /// Are Depth Texture Supported
            ///
            /// @return Whether or not depth textures are
            /// supported.
            //-------------------------------------------------
            bool AreDepthTexturesSupported() const;
            //-------------------------------------------------
            /// Get Max Texture Size
            ///
            /// @return The maximum texture size available on
            /// this device.
            //-------------------------------------------------
            u32 GetMaxTextureSize() const;
            //-------------------------------------------------
            /// Get Num Texture Units
            ///
            /// @return The number of texture units supported
            /// by this device.
            //-------------------------------------------------
            u32 GetNumTextureUnits() const;
        private:
            bool mbShadowsSupported;
            bool mbDepthTexturesSupported;
            bool mbHighPrecisionInFragmentShaderSupported;
            u32 mudwMaxTextureSize;
            u32 mudwNumTextureUnits;
        };
    }
}

#endif
