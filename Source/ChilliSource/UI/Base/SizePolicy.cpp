//
//  SizePolicy.cpp
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

#include <ChilliSource/UI/Base/SizePolicy.h>

#include <ChilliSource/Core/String/StringUtils.h>

namespace ChilliSource
{
    namespace UI
    {
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        SizePolicy ParseSizePolicy(const std::string& in_policy)
        {
            std::string lowerCase = in_policy;
            Core::StringUtils::ToLowerCase(lowerCase);
            
            if(lowerCase == "none") return SizePolicy::k_none;
            if(lowerCase == "usepreferredsize") return SizePolicy::k_usePreferredSize;
            if(lowerCase == "usewidthmaintainingaspect") return SizePolicy::k_useWidthMaintainingAspect;
            if(lowerCase == "useheightmaintainingaspect") return SizePolicy::k_useHeightMaintainingAspect;
            if(lowerCase == "fitmaintainingaspect") return SizePolicy::k_fitMaintainingAspect;
            if(lowerCase == "fillmaintainingaspect") return SizePolicy::k_fillMaintainingAspect;
            
            CS_LOG_FATAL("Cannot parse size policy type: " + in_policy);
            return SizePolicy::k_none;
        }
    }
}