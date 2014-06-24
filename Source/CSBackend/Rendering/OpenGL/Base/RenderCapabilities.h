//
//  RenderingCapabilities.h
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

#ifndef _CSBACKEND_RENDERING_OPENGL_BASE_RENDERCAPABILITIES_H_
#define _CSBACKEND_RENDERING_OPENGL_BASE_RENDERCAPABILITIES_H_

#include <ChilliSource/ChilliSource.h>
#include <CSBackend/Rendering/OpenGL/ForwardDeclarations.h>
#include <ChilliSource/Rendering/Base/RenderCapabilities.h>

namespace CSBackend
{
    namespace OpenGL
    {
        //---------------------------------------------------------
        /// Describes the opengl rendering capabilities available
        /// on the running device.
        ///
        /// @author Ian Copland
        //---------------------------------------------------------
        class RenderCapabilities final : public CSRendering::RenderCapabilities
        {
        public:
            CS_DECLARE_NAMEDTYPE(RenderCapabilities);

            //-------------------------------------------------
			/// Query the interface type
            ///
            /// @author Ian Copland
            ///
			/// @param The interface to compare
            ///
			/// @return Whether the object implements that
            /// interface
			//-------------------------------------------------
			bool IsA(CSCore::InterfaceIDType in_interfaceId) const override;
            //-------------------------------------------------
            /// Determine the capabilities of the device
            /// in terms of GL and render support. This needs
            /// to be called after the render system has been
            /// initialised.
            ///
            /// @author Ian Copland
			//-------------------------------------------------
			void DetermineCapabilities();
            //-------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return Whether or not shadow mapping is
            /// supported.
            //-------------------------------------------------
            bool IsShadowMappingSupported() const override;
            //-------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return Whether or not depth textures are
            /// supported.
            //-------------------------------------------------
            bool IsDepthTextureSupported() const override;
            //-------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return Whether or not map buffer is supported
            //-------------------------------------------------
            bool IsMapBufferSupported() const override;
            //-------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The maximum texture size available on
            /// this device.
            //-------------------------------------------------
            u32 GetMaxTextureSize() const override;
            //-------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The number of texture units supported
            /// by this device.
            //-------------------------------------------------
            u32 GetNumTextureUnits() const override;
            
        private:
            
            friend CSRendering::RenderCapabilitiesUPtr CSRendering::RenderCapabilities::Create();
            //-------------------------------------------------
			/// Private constructor to enforce use of the
            /// factory method.
            ///
            /// @author Ian Copland
			//-------------------------------------------------
			RenderCapabilities();
            
        private:
            
            bool m_areShadowMapsSupported;
            bool m_areDepthTexturesSupported;
            bool m_areMapBuffersSupported;

            u32 m_maxTextureSize;
            u32 m_maxTextureUnits;
        };
    }
}

#endif
