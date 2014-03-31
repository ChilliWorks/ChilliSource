//
//  RenderCapabilities.h
//  Chilli Source
//
//  Created by Ian Copland on 27/01/2014.
//  Copyright (c) 2014 Tag Games Ltd. All rights reserved.
//

#ifndef _MOFLOW_RENDERING_RENDERCAPABILITIES_H_
#define _MOFLOW_RENDERING_RENDERCAPABILITIES_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/System/AppSystem.h>

namespace ChilliSource
{
    namespace Rendering
    {
        //---------------------------------------------------------------
        /// A system for getting information on the capabilities of the
        /// renderer on this specfic device. This includes information
        /// like whether specific features are available or the maximum
        /// texture size. This should not be used prior to the render
        /// system being initialised.
        ///
        /// @author I Copland
        //---------------------------------------------------------------
        class RenderCapabilities : public Core::AppSystem
        {
        public:
            CS_DECLARE_NAMEDTYPE(RenderCapabilities);
            
            //-------------------------------------------------------
            /// Create the platform dependent backend
            ///
            /// @author S Downie
            ///
            /// @return New backend instance
            //-------------------------------------------------------
            static RenderCapabilitiesUPtr Create();
            //-------------------------------------------------------
            /// @author I Copland
            ///
            /// @return Whether or not shadow mapping is supported.
            //-------------------------------------------------------
            virtual bool IsShadowMappingSupported() const = 0;
            //-------------------------------------------------------
            /// @author I Copland
            ///
            /// @return Whether or not depth textures are supported.
            //-------------------------------------------------------
            virtual bool IsDepthTextureSupported() const = 0;
            //-------------------------------------------------
            /// @author I Copland
            ///
            /// @return Whether or not map buffer is supported
            //-------------------------------------------------
            virtual bool IsMapBufferSupported() const = 0;
            //-------------------------------------------------------
            /// @author I Copland
            ///
            /// @return The maximum texture size available on this
            /// device.
            //-------------------------------------------------------
            virtual u32 GetMaxTextureSize() const = 0;
            //-------------------------------------------------------
            /// @author I Copland
            ///
            /// @return The number of texture units supported by this
            /// device.
            //-------------------------------------------------------
            virtual u32 GetNumTextureUnits() const = 0;
            //-------------------------------------------------------
            /// Destructor
            ///
            /// @author I Copland
            //-------------------------------------------------------
            virtual ~RenderCapabilities(){};
            
        protected:
            
            //-------------------------------------------------------
            /// Private constructor to force use of factory method
            ///
            /// @author S Downie
            //-------------------------------------------------------
            RenderCapabilities() = default;
        };
    }
}

#endif
