//
//  RenderCapabilities.h
//  Chilli Source
//  Created by Ian Copland on 27/01/2014.
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

#ifndef _CHILLISOURCE_RENDERING_BASE_RENDERCAPABILITIES_H_
#define _CHILLISOURCE_RENDERING_BASE_RENDERCAPABILITIES_H_

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
        /// @author Ian Copland
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
            /// @author Ian Copland
            ///
            /// @return Whether or not shadow mapping is supported.
            //-------------------------------------------------------
            virtual bool IsShadowMappingSupported() const = 0;
            //-------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return Whether or not depth textures are supported.
            //-------------------------------------------------------
            virtual bool IsDepthTextureSupported() const = 0;
            //-------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return Whether or not map buffer is supported
            //-------------------------------------------------
            virtual bool IsMapBufferSupported() const = 0;
            //-------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The maximum texture size available on this
            /// device.
            //-------------------------------------------------------
            virtual u32 GetMaxTextureSize() const = 0;
            //-------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The number of texture units supported by this
            /// device.
            //-------------------------------------------------------
            virtual u32 GetNumTextureUnits() const = 0;
            //-------------------------------------------------------
            /// Destructor
            ///
            /// @author Ian Copland
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
