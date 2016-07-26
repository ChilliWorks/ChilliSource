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

#include <ChilliSource/Rendering/Target/RenderTargetGroup.h>

#include <ChilliSource/Rendering/Texture/RenderTexture.h>

namespace ChilliSource
{
    //------------------------------------------------------------------------------
    RenderTargetGroup::RenderTargetGroup(const RenderTexture* colourTarget, const RenderTexture* depthTarget, RenderTargetGroupType type) noexcept
        : m_colourTarget(colourTarget), m_depthTarget(depthTarget), m_type(type)
    {
        CS_ASSERT(colourTarget || depthTarget, "Must supply either a colour target or a depth target.");
        
        if (colourTarget && depthTarget)
        {
            CS_ASSERT(colourTarget->GetDimensions() == depthTarget->GetDimensions(), "All render targets must have the same dimensions");
        }
        
        if (colourTarget)
        {
            CS_ASSERT(colourTarget->GetImageFormat() != ImageFormat::k_Depth16 && colourTarget->GetImageFormat() != ImageFormat::k_Depth32, "Colour target cannot be depth texture.");
            CS_ASSERT(colourTarget->GetImageCompression() == ImageCompression::k_none, "Colour target cannot be compressed.");
            CS_ASSERT(colourTarget->IsMipmapped() == false, "Colour target cannot be mipmapped.");
            
            m_resolution = colourTarget->GetDimensions();
        }
        
        if (depthTarget)
        {
            CS_ASSERT(depthTarget->GetImageFormat() == ImageFormat::k_Depth16 || depthTarget->GetImageFormat() == ImageFormat::k_Depth32, "Depth target must be depth texture.");
            CS_ASSERT(depthTarget->GetImageCompression() == ImageCompression::k_none, "Depth target cannot be compressed.");
            CS_ASSERT(depthTarget->IsMipmapped() == false, "Depth target cannot be mipmapped.");
            CS_ASSERT(m_type == RenderTargetGroupType::k_colour, "If a depth target is supplied, type must be colour only.");
            
            m_resolution = depthTarget->GetDimensions();
        }
    }
}
