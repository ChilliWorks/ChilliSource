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

#ifndef _CHILLISOURCE_RENDERING_BASE_CANVASDRAWMODE_H_
#define _CHILLISOURCE_RENDERING_BASE_CANVASDRAWMODE_H_

#include <ChilliSource/ChilliSource.h>

namespace ChilliSource
{
    /// Describes how the a canvas shape should be rendered
    ///
    /// - Standard - Renders to canvas in the standard way
    /// - Mask - Renders to canvas and also creates a mask
    /// - Mask Only - Doesn't render to canvas, only creates an invisible mask
    ///
    enum class CanvasDrawMode
    {
        k_standard,
        k_mask,
        k_maskOnly
    };
    
    /// Converts a string to a draw mode (case insensitive)
    /// Fails hard if mode unknown
    ///
    /// @param modeString
    ///     Draw mode in string format
    ///
    /// @return CanvasDrawMode
    ///
    CanvasDrawMode ParseCanvasDrawMode(const std::string& modeString) noexcept;
}

#endif
