//
//  SliderDirection.h
//  Chilli Source
//  Created by Ian Copland on 18/12/2014.
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

#ifndef _CHILLISOURCE_UI_SLIDER_SLIDERDIRECTION_H_
#define _CHILLISOURCE_UI_SLIDER_SLIDERDIRECTION_H_

#include <ChilliSource/ChilliSource.h>

namespace ChilliSource
{
    namespace UI
    {
        //------------------------------------------------------------------------
        /// An enum describing the direction of a slider: vertical or
        /// horizontal.
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------
        enum class SliderDirection
        {
            k_horizontal,
            k_vertical
        };
        //------------------------------------------------------------------------
        /// Parse a slider direction from string. This is case insensitive.
        /// If the string is not a valid direction then the app is considered to
        /// be in an irrecoverable state and will terminate.
        ///
        /// @author Ian Copland
        ///
        /// @param The string to parse.
        ///
        /// @return The parsed direction.
        //------------------------------------------------------------------------
        SliderDirection ParseSliderDirection(const std::string& in_stringDirection);
        //------------------------------------------------------------------------
        /// Converts the given slider direction to a string.
        ///
        /// @author Ian Copland
        ///
        /// @param The slider direction to convert to string.
        ///
        /// @return The direction in string form.
        //------------------------------------------------------------------------
        std::string ToString(SliderDirection in_direction);
    }
}

#endif
