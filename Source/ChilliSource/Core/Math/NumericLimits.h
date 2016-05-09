//
//  NumericLimits.h
//  Chilli Source
//  Created by Ian Copland on 10/03/2015.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2015 Tag Games Limited
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

#ifndef _CHILLISOURCE_CORE_MATH_NUMERICLIMITS_H_
#define _CHILLISOURCE_CORE_MATH_NUMERICLIMITS_H_

#include <ChilliSource/ChilliSource.h>

#include <limits>

namespace ChilliSource
{
    //------------------------------------------------------------------------------
    /// A series of functions for acquiring the highest and lowest values for a
    /// specific type. Typically std::numeric_limits should be used for this purpose.
    /// However, the Visual C++ compiler cannot handle the use of std::numeric_limits
    /// in default function / method parameters. This can be used instead.
    ///
    /// @author Ian Copland
    //------------------------------------------------------------------------------
    namespace NumericLimits
    {
        //------------------------------------------------------------------------------
        /// @author Ian Copland
        ///
        /// @param The lowest possible value for the requested type.
        //------------------------------------------------------------------------------
        template <typename TType> TType Lowest();
        //------------------------------------------------------------------------------
        /// @author Ian Copland
        ///
        /// @param The highest possible value for the requested type.
        //------------------------------------------------------------------------------
        template <typename TType> TType Highest();

        //------------------------------------------------------------------------------.
        //------------------------------------------------------------------------------
        template <typename TType> TType Lowest()
        {
            return std::numeric_limits<TType>::lowest();
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename TType> TType Highest()
        {
            return std::numeric_limits<TType>::max();
        }
    }
}

#endif
