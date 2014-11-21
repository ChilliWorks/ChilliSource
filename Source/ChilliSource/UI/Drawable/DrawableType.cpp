//
//  DrawableType.cpp
//  Chilli Source
//  Created by Scott Downie on 25/07/2014.
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

#include <ChilliSource/UI/Drawable/DrawableType.h>

#include <ChilliSource/Core/String/StringUtils.h>

namespace ChilliSource
{
    namespace UI
    {
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        DrawableType ParseDrawableType(const std::string& in_type)
        {
            std::string lowerCase = in_type;
            Core::StringUtils::ToLowerCase(lowerCase);
            
            if(lowerCase == "none") return DrawableType::k_none;
            if(lowerCase == "standard") return DrawableType::k_standard;
            if(lowerCase == "ninepatch") return DrawableType::k_ninePatch;
            if(lowerCase == "threepatch") return DrawableType::k_threePatch;
            
            CS_LOG_FATAL("Cannot parse drawable type: " + in_type);
            return DrawableType::k_none;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        std::string ToString(DrawableType in_drawable)
        {
            switch (in_drawable)
            {
                case DrawableType::k_none:
                    return "none";
                case DrawableType::k_standard:
                    return "standard";
                case DrawableType::k_threePatch:
                    return "threepatch";
                case DrawableType::k_ninePatch:
                    return "ninepatch";
                default:
                    CS_LOG_FATAL("Invalid drawable type.");
                    return "";
            }
        }
    }
}