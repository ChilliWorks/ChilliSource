//
//  RenderCapabilities.h
//  ChilliSource
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
#include <ChilliSource/Core/Base/RenderInfo.h>
#include <ChilliSource/Core/System/AppSystem.h>

namespace ChilliSource
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
    class RenderCapabilities final: public AppSystem
    {
    public:
        CS_DECLARE_NAMEDTYPE(RenderCapabilities);
        
        /// Create the platform dependent backend
        ///
        /// @param renderInfo
        ///     The object containing the render info for the current device
        ///
        /// @return New RenderCapabilities instance
        ///
        static RenderCapabilitiesUPtr Create(const RenderInfo& renderInfo) noexcept;
        
        /// Query the interface type
        ///
        /// @param The interface to compare
        ///
        /// @return Whether the object implements that interface
        ///
        bool IsA(ChilliSource::InterfaceIDType interfaceId) const override;

        /// @return Whether or not shadow mapping is supported.
        ///
        bool IsShadowMappingSupported() const noexcept;
        
        /// @return Whether or not depth textures are supported.
        ///
        bool IsDepthTextureSupported() const noexcept;
        
        /// @return Whether or not map buffer is supported
        ///
        bool IsMapBufferSupported() const noexcept;
        
        /// @return Whether or not vertex array objects are supported.
        ///
        bool IsVAOSupported() const noexcept;
        
        /// @return Whether or not the fragment shader supports highp floats.
        ///
        bool IsHighPrecisionFloatsSupported() const noexcept;
        
        /// @return The maximum texture size available on this device.
        ///
        u32 GetMaxTextureSize() const noexcept;
        
        /// @return The number of texture units supported by this device.
        ///
        u32 GetNumTextureUnits() const noexcept;
        
        /// @return The number of vertex attributes supported by this device.
        ///
        u32 GetNumVertexAttributes() const noexcept;
       
    private:
        
        /// @param isShadowMapsSupported
        ///         Whether or not shadow mapping is supported.
        /// @param isDepthTexturesSupported
        ///         Whether or not depth textures are supported.
        /// @param isMapBuffersSupported
        ///         Whether or not map buffer is supported.
        /// @param isVAOSupported
        ///     Whether vertex array objects are supported
        /// @param isHighPrecisionFloatsSupported
        ///         Whether or not the fragment shader supports highp floats.
        /// @param maxTextureSize
        ///         The maximum texture size available on this device.
        /// @param numTextureUnits
        ///         The number of texture units supported by this device.
        /// @param maxVertexAttribs
        ///         The max. number of vertex attributes supported by this device.
        ///
        RenderCapabilities(bool isShadowMapsSupported, bool isDepthTexturesSupported, bool isMapBuffersSupported, bool isVAOSupported, bool isHighPrecisionFloatsSupported, u32 maxTextureSize, u32 numTextureUnits, u32 maxVertexAttribs);
        
    private:
        
        bool m_isShadowMapsSupported;
        bool m_isDepthTexturesSupported;
        bool m_isMapBuffersSupported;
        bool m_isVAOSupported;
        bool m_isHighPrecisionFloatsSupported;
        
        u32 m_maxTextureSize;
        u32 m_maxTextureUnits;
        u32 m_maxVertexAttribs;
    };
}

#endif
