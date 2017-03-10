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

#ifndef _CHILLISOURCE_CORE_BASE_RENDERINFO_H_
#define _CHILLISOURCE_CORE_BASE_RENDERINFO_H_

#include <ChilliSource/ChilliSource.h>

namespace ChilliSource
{
    /// This class contains information related to render
    /// capabilities for the current device.
    ///
    /// This class is immutable, and therefore thread-safe.
    ///
    class RenderInfo final
    {
    public:
        /// @param isShadowMapsSupported
        ///         Whether or not shadow mapping is supported.
        /// @param isDepthTexturesSupported
        ///         Whether or not depth textures is supported.
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
        RenderInfo(bool isShadowMapsSupported, bool isDepthTexturesSupported, bool isMapBuffersSupported, bool isVAOSupported, bool isHighPrecisionFloatsSupported, u32 maxTextureSize, u32 numTextureUnits, u32 maxVertexAttribs) noexcept;
       
        /// @return Whether or not shadow mapping is supported.
        ///
        bool IsShadowMappingSupported() const noexcept { return m_isShadowMapsSupported; }
        
        /// @return Whether or not depth textures is supported.
        ///
        bool IsDepthTextureSupported() const noexcept { return m_isDepthTexturesSupported; }
        
        /// @return Whether or not map buffer is supported.
        ///
        bool IsMapBufferSupported() const noexcept { return m_isMapBuffersSupported; }
        
        /// @return Whether or not vertex array objects are supported.
        ///
        bool IsVAOSupported() const noexcept { return m_isVAOSupported; }
        
        /// @return Whether or not the fragment shader supports highp floats.
        ///
        bool IsHighPrecisionFloatsSupported() const noexcept { return m_isHighPrecisionFloatsSupported; }
        
        /// @return The maximum texture size available on this device.
        ///
        u32 GetMaxTextureSize() const noexcept { return m_maxTextureSize; }
        
        /// @return The number of texture units supported by this device.
        ///
        u32 GetNumTextureUnits() const noexcept { return m_maxTextureUnits; }
        
        /// @return The max. number of vertex attributes supported by this device.
        ///
        u32 GetNumVertexAttributes() const noexcept { return m_maxVertexAttributes; }

    private:
        
        bool m_isShadowMapsSupported;
        bool m_isDepthTexturesSupported;
        bool m_isMapBuffersSupported;
        bool m_isVAOSupported;
        bool m_isHighPrecisionFloatsSupported;
        
        u32 m_maxTextureSize;
        u32 m_maxTextureUnits;
        u32 m_maxVertexAttributes;
    };
}

#endif
