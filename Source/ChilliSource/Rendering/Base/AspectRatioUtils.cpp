//
//  AspectRatioUtils.cpp
//  Chilli Source
//  Created by S Downie on 24/06/2014.
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

#include <ChilliSource/Rendering/Base/AspectRatioUtils.h>

#include <ChilliSource/Core/Math/Vector2.h>

namespace ChilliSource
{
    namespace Rendering
    {
        namespace AspectRatioUtils
        {
            //----------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------
            Core::Vector2 KeepOriginalWidthAdaptHeight(const Core::Vector2& in_originalSize, f32 in_preferredAspectRatio)
            {
                CS_ASSERT(in_preferredAspectRatio > 0.0f, "Aspect ratio must be greater than zero");
                
                f32 targetAspectRatio = 1.0f/in_preferredAspectRatio;
                f32 originalHeight = (targetAspectRatio * in_originalSize.x);
                return Core::Vector2(in_originalSize.x, originalHeight);
            }
            //----------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------
            Core::Vector2 KeepOriginalHeightAdaptWidth(const Core::Vector2& in_originalSize, f32 in_preferredAspectRatio)
            {
                f32 originalWidth = (in_preferredAspectRatio * in_originalSize.y);
                return Core::Vector2(originalWidth, in_originalSize.y);
            }
            //----------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------
            Core::Vector2 FillOriginal(const Core::Vector2& in_originalSize, f32 in_preferredAspectRatio)
            {
                f32 originalRatio = in_originalSize.x / in_originalSize.y;
                
                if(in_preferredAspectRatio <= originalRatio)
                {
                    return KeepOriginalWidthAdaptHeight(in_originalSize, in_preferredAspectRatio);
                }
                else
                {
                    return KeepOriginalHeightAdaptWidth(in_originalSize, in_preferredAspectRatio);
                }
            }
            //----------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------
            Core::Vector2 FitOriginal(const Core::Vector2& in_originalSize, f32 in_preferredAspectRatio)
            {
                f32 originalRatio = in_originalSize.x / in_originalSize.y;
                
                if(in_preferredAspectRatio > originalRatio)
                {
                    return KeepOriginalWidthAdaptHeight(in_originalSize, in_preferredAspectRatio);
                }
                else
                {
                    return KeepOriginalHeightAdaptWidth(in_originalSize, in_preferredAspectRatio);
                }
            }
        }
    }
}