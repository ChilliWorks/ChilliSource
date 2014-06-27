//
//  AspectRatioUtils.h
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

#ifndef _CHILLISOURCE_RENDERING_BASE_ASPECTRATIOUTILS_H_
#define _CHILLISOURCE_RENDERING_BASE_ASPECTRATIOUTILS_H_

#include <ChilliSource/ChilliSource.h>

namespace ChilliSource
{
    namespace Rendering
    {
        //--------------------------------------------------------------------
        /// Utility methods for calculating a size based on the given
        /// size and preferred aspect ratio.
        ///
        /// @author S Downie
        //--------------------------------------------------------------------
        namespace AspectRatioUtils
        {
            //----------------------------------------------------------------------------------------
            /// Aspect ratio maintaining function that keeps the original width but adapts
            /// the height to maintain the aspect ratio
            ///
            /// @author S Downie
            ///
            /// @param Original size
            /// @param Preferred aspect ratio
            ///
            /// @return Size with aspect maintained
            //----------------------------------------------------------------------------------------
            Core::Vector2 KeepOriginalWidthAdaptHeight(const Core::Vector2& in_originalSize, f32 in_preferredAspectRatio);
            //----------------------------------------------------------------------------------------
            /// Aspect ratio maintaining function that keeps the original height but adapts
            /// the width to maintain the aspect ratio
            ///
            /// @author S Downie
            ///
            /// @param Original size
            /// @param Preferred aspect ratio
            ///
            /// @return Size with aspect maintained
            //----------------------------------------------------------------------------------------
            Core::Vector2 KeepOriginalHeightAdaptWidth(const Core::Vector2& in_originalSize, f32 in_preferredAspectRatio);
            //----------------------------------------------------------------------------------------
            /// Aspect ratio maintaining function that maintains the given target aspect ratio
            /// while ensuring the size does not DROP BELOW the original size
            ///
            /// @author S Downie
            ///
            /// @param Original size
            /// @param Preferred aspect ratio
            ///
            /// @return Size with aspect maintained
            //----------------------------------------------------------------------------------------
            Core::Vector2 FillOriginal(const Core::Vector2& in_originalSize, f32 in_preferredAspectRatio);
            //----------------------------------------------------------------------------------------
            /// Aspect ratio maintaining function that maintains the given target aspect ratio
            /// while ensuring the size does not EXCEED the original size
            ///
            /// @author S Downie
            ///
            /// @param Original size
            /// @param Preferred aspect ratio
            ///
            /// @return Size with aspect maintained
            //----------------------------------------------------------------------------------------
            Core::Vector2 FitOriginal(const Core::Vector2& in_originalSize, f32 in_preferredAspectRatio);
        }
    }
}

#endif
