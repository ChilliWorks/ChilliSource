//
//  RenderCapabilities.h
//  moFlow
//
//  Created by Ian Copland on 27/01/2014.
//  Copyright (c) 2014 Tag Games Ltd. All rights reserved.
//

#ifndef _MOFLOW_RENDERING_RENDERCAPABILITIES_H_
#define _MOFLOW_RENDERING_RENDERCAPABILITIES_H_

namespace moFlo
{
    namespace Rendering
    {
        //===============================================================
        /// Render Capabilities
        ///
        /// A system for getting information on the capabilities of the
        /// renderer on this specfic device. This includes information
        /// like whether specific features are available or the maximum
        /// texture size. This should not be used prior to the render
        /// system being initialised.
        //===============================================================
        class IRenderCapabilities : public moFlo::Core::ISystem
        {
        public:
            DECLARE_NAMED_INTERFACE(IRenderCapabilities);
            //-------------------------------------------------------
            /// Is Shadow Mapping Supported
            ///
            /// @return Whether or not shadow mapping is supported.
            //-------------------------------------------------------
            virtual bool IsShadowMappingSupported() const = 0;
            //-------------------------------------------------------
            /// Are Depth Texture Supported
            ///
            /// @return Whether or not depth textures are supported.
            //-------------------------------------------------------
            virtual bool AreDepthTexturesSupported() const = 0;
            //-------------------------------------------------------
            /// Get Max Texture Size
            ///
            /// @return The maximum texture size available on this
            /// device.
            //-------------------------------------------------------
            virtual u32 GetMaxTextureSize() const = 0;
            //-------------------------------------------------------
            /// Get Num Texture Units
            ///
            /// @return The number of texture units supported by this
            /// device.
            //-------------------------------------------------------
            virtual u32 GetNumTextureUnits() const = 0;
            //-------------------------------------------------------
            /// Destructor
            //-------------------------------------------------------
            virtual ~IRenderCapabilities(){};
        };
    }
}

#endif
