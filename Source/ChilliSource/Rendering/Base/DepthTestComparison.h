//
//  DepthTestComparison.h
//  Chilli Source
//  Created by Scott Downie on 09/04/2014.
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

#ifndef _CHILLISOURCE_RENDERING_BASE_DEPTHTESTCOMPARISON_H_
#define _CHILLISOURCE_RENDERING_BASE_DEPTHTESTCOMPARISON_H_

namespace ChilliSource
{
	namespace Rendering
	{
        //---------------------------------------------
        /// Used in the depth test function to decide
        /// whether a pixel should be rendered or not.
        /// These types describe which comparison operator
        /// should be used
        ///
        /// @author S Downie
        //---------------------------------------------
        enum class DepthTestComparison
        {
            k_less,
            k_equal,
            k_lequal
        };
	}
}

#endif