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

#ifndef _CHILLISOURCE_RENDERING_BASE_STENCILOP_H_
#define _CHILLISOURCE_RENDERING_BASE_STENCILOP_H_

namespace ChilliSource
{
    /// The stencil ops are used to determine which action to take
    /// on a pixel after the result of stencil testing
    ///
    enum class StencilOp
    {
        k_keep,             // Keeps the current value
        k_zero,             // Sets the stencil buffer value to zero
        k_replace,          // Sets the stencil bufer value as specified by the stencil func
        k_increment,        // Increments the stencil buffer value, clamps to max
        k_incrementWrap,    // Increments the stencil buffer value, wraps to zero
        k_decrement,        // Decrements the stencil buffer value, clamps to zero
        k_decrementWrap,    // Decrements the stencil buffer value, wraps to max
        k_invert            // Inverts the stencil buffer value
    };
}

#endif
