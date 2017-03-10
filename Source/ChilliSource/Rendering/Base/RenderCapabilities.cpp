//
//  RenderCapabilities.cpp
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

#include <ChilliSource/Rendering/Base/RenderCapabilities.h>

namespace ChilliSource
{
    CS_DEFINE_NAMEDTYPE(RenderCapabilities);
    
    //-------------------------------------------------------
    RenderCapabilitiesUPtr RenderCapabilities::Create(const RenderInfo& renderInfo) noexcept
    {
        return RenderCapabilitiesUPtr(new RenderCapabilities(renderInfo.IsShadowMappingSupported(), renderInfo.IsDepthTextureSupported(), renderInfo.IsMapBufferSupported(), renderInfo.IsVAOSupported(),
                                                             renderInfo.IsHighPrecisionFloatsSupported(), renderInfo.GetMaxTextureSize(), renderInfo.GetNumTextureUnits(), renderInfo.GetNumVertexAttributes()));
    }
    
    //-------------------------------------------------------
    RenderCapabilities::RenderCapabilities(bool isShadowMapsSupported, bool isDepthTexturesSupported, bool isMapBuffersSupported, bool isVAOSupported, bool isHighPrecisionFloatsSupported, u32 maxTextureSize, u32 numTextureUnits, u32 maxVertexAttribs)
    : m_isShadowMapsSupported(isShadowMapsSupported), m_isDepthTexturesSupported(isDepthTexturesSupported), m_isMapBuffersSupported(isMapBuffersSupported), m_isVAOSupported(isVAOSupported),
    m_isHighPrecisionFloatsSupported(isHighPrecisionFloatsSupported), m_maxTextureSize(maxTextureSize), m_maxTextureUnits(numTextureUnits), m_maxVertexAttribs(maxVertexAttribs)
    {
    }
    
    //-------------------------------------------------------
    bool RenderCapabilities::IsA(ChilliSource::InterfaceIDType interfaceId) const
    {
        return (RenderCapabilities::InterfaceID == interfaceId);
    }
    
    //-------------------------------------------------------
    bool RenderCapabilities::IsShadowMappingSupported() const noexcept
    {
        return m_isShadowMapsSupported;
    }
    
    //-------------------------------------------------------
    bool RenderCapabilities::IsDepthTextureSupported() const noexcept
    {
        return m_isDepthTexturesSupported;
    }
    
    //-------------------------------------------------------
    bool RenderCapabilities::IsMapBufferSupported() const noexcept
    {
        return m_isMapBuffersSupported;
    }
    
    //-------------------------------------------------------
    bool RenderCapabilities::IsVAOSupported() const noexcept
    {
        return m_isVAOSupported;
    }
    
    //-------------------------------------------------------
    bool RenderCapabilities::IsHighPrecisionFloatsSupported() const noexcept
    {
        return m_isHighPrecisionFloatsSupported;
    }
    
    //-------------------------------------------------------
    u32 RenderCapabilities::GetMaxTextureSize() const noexcept
    {
        return m_maxTextureSize;
    }
    
    //-------------------------------------------------------
    u32 RenderCapabilities::GetNumTextureUnits() const noexcept
    {
        return m_maxTextureUnits;
    }
    
    //-------------------------------------------------------
    u32 RenderCapabilities::GetNumVertexAttributes() const noexcept
    {
        return m_maxVertexAttribs;
    }
}
