//
//  SizePolicy.h
//  ChilliSource
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

#ifndef _CHILLISOURCE_RENDERING_BASE_SIZEPOLICY_H_
#define _CHILLISOURCE_RENDERING_BASE_SIZEPOLICY_H_

#include <ChilliSource/ChilliSource.h>

namespace ChilliSource
{
    //------------------------------------------------------------------------------
    /// Identifiers for functions that maintain the aspect ratio of the widget based
    /// on current size and preferred size
    ///
    /// @author S Downie
    //------------------------------------------------------------------------------
    enum class SizePolicy
    {
        k_none,
        k_usePreferredSize,
        k_useWidthMaintainingAspect,
        k_useHeightMaintainingAspect,
        k_fitMaintainingAspect,
        k_fillMaintainingAspect,
        k_totalNum
    };
    //------------------------------------------------------------------------------
    /// Parse the string and return the size policy
    ///
    /// @author S Downie
    ///
    /// @param String
    ///
    /// @return Size policy
    //------------------------------------------------------------------------------
    SizePolicy ParseSizePolicy(const std::string& in_policy);
}

#endif
