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
        /// @param areShadowMapsSupported
        ///         Whether or not shadow mapping is supported.
        /// @param areDepthTexturesSupported
        ///         Whether or not depth textures are supported.
        /// @param areMapBuffersSupported
        ///         Whether or not map buffer is supported.
        /// @param maxTextureSize
        ///         The maximum texture size available on this device.
        /// @param numTextureUnits
        ///         The number of texture units supported by this device.
        ///
        RenderInfo(bool areShadowMapsSupported, bool areDepthTexturesSupported, bool areMapBuffersSupported, u32 maxTextureSize, u32 numTextureUnits) noexcept;
       
        /// @return Whether or not shadow mapping is supported.
        ///
        bool IsShadowMappingSupported() const noexcept { return m_areShadowMapsSupported; }
        
        /// @return Whether or not depth textures are supported.
        ///
        bool IsDepthTextureSupported() const noexcept { return m_areDepthTexturesSupported; }
        
        /// @return Whether or not map buffer is supported.
        ///
        bool IsMapBufferSupported() const noexcept { return m_areMapBuffersSupported; }
        
        /// @return The maximum texture size available on this device.
        ///
        u32 GetMaxTextureSize() const noexcept { return m_maxTextureSize; }
        
        /// @return The number of texture units supported by this device.
        ///
        u32 GetNumTextureUnits() const noexcept { return m_maxTextureUnits; }

    private:
        
        bool m_areShadowMapsSupported;
        bool m_areDepthTexturesSupported;
        bool m_areMapBuffersSupported;
        
        u32 m_maxTextureSize;
        u32 m_maxTextureUnits;
    };
}

#endif
