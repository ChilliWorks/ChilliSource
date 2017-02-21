//
//  SliderDirection.cpp
//  ChilliSource
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

#include <ChilliSource/UI/Slider/SliderDirection.h>

#include <ChilliSource/Core/String/StringUtils.h>

namespace ChilliSource
{
    const char k_directionHorizontal[] = "horizontal";
    const char k_directionVertical[] = "vertical";
    
    //------------------------------------------------------------------------
    //------------------------------------------------------------------------
    SliderDirection ParseSliderDirection(const std::string& in_stringDirection)
    {
        std::string lowerDirection = in_stringDirection;
        StringUtils::ToLowerCase(lowerDirection);
        
        if (lowerDirection == k_directionHorizontal)
        {
            return SliderDirection::k_horizontal;
        }
        else if (lowerDirection == k_directionVertical)
        {
            return SliderDirection::k_vertical;
        }
        
        CS_LOG_FATAL("Could not parse slider direction: " + in_stringDirection);
        return SliderDirection::k_horizontal;
    }
    //------------------------------------------------------------------------
    //------------------------------------------------------------------------
    std::string ToString(SliderDirection in_direction)
    {
        switch (in_direction)
        {
            case SliderDirection::k_horizontal:
                return k_directionHorizontal;
            case SliderDirection::k_vertical:
                return k_directionVertical;
            default:
                CS_LOG_FATAL("Invalid slider direction.");
                return k_directionHorizontal;
                
        }
    }
}
