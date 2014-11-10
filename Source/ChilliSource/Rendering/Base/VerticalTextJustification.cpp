//
//  VerticalTextJustification.cpp
//  Chilli Source
//  Created by Ian Copland on 10/11/2014.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2011 Tag Games Limited
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

#include <ChilliSource/Rendering/Base/VerticalTextJustification.h>

#include <ChilliSource/Core/String/StringUtils.h>

namespace ChilliSource
{
    namespace Rendering
    {
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        VerticalTextJustification ParseVerticalTextJustification(const std::string& in_string)
        {
            std::string verticalJustificationString = in_string;
            Core::StringUtils::ToLowerCase(verticalJustificationString);
            
            if (verticalJustificationString == "bottom")
            {
                return Rendering::VerticalTextJustification::k_bottom;
            }
            else if (verticalJustificationString == "centre")
            {
                return Rendering::VerticalTextJustification::k_centre;
            }
            else if (verticalJustificationString == "top")
            {
                return Rendering::VerticalTextJustification::k_top;
            }
            
            CS_LOG_FATAL("Invalid vertical justification.");
            return Rendering::VerticalTextJustification::k_centre;
        }
    }
}