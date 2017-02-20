//
//  ProgressBarType.cpp
//  ChilliSource
//  Created by Ian Copland on 17/12/2014.
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

#include <ChilliSource/UI/ProgressBar/ProgressBarType.h>

#include <ChilliSource/Core/String/StringUtils.h>

namespace ChilliSource
{
    namespace
    {
        const char k_typeFill[] = "fill";
        const char k_typeStretch[] = "stretch";
    }
    
    //------------------------------------------------------------------------
    //------------------------------------------------------------------------
    ProgressBarType ParseProgressBarType(const std::string& in_stringType)
    {
        std::string lowerType = in_stringType;
        StringUtils::ToLowerCase(lowerType);
        
        if (lowerType == k_typeFill)
        {
            return ProgressBarType::k_fill;
        }
        else if (lowerType == k_typeStretch)
        {
            return ProgressBarType::k_stretch;
        }
        
        CS_LOG_FATAL("Could not parse progress bar type: " + in_stringType);
        return ProgressBarType::k_fill;
    }
    //------------------------------------------------------------------------
    //------------------------------------------------------------------------
    std::string ToString(ProgressBarType in_direction)
    {
        switch (in_direction)
        {
            case ProgressBarType::k_fill:
                return k_typeFill;
            case ProgressBarType::k_stretch:
                return k_typeStretch;
            default:
                CS_LOG_FATAL("Invalid progress bar type.");
                return k_typeFill;
                
        }
    }
}
