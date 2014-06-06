//
//  ByteColour.h
//  Chilli Source
//  Created by Scott Downie on 03/04/2014.
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

#ifndef _CHILLISOURCE_CORE_BASE_BYTECOLOUR_H_
#define _CHILLISOURCE_CORE_BASE_BYTECOLOUR_H_

#include <ChilliSource/ChilliSource.h>

namespace ChilliSource
{
	namespace Core
	{
        //-----------------------------------------------------------
        /// Container for RGBA colour as 0 - 255 values
        ///
        /// @author S Downie
		//-----------------------------------------------------------
		class ByteColour
		{
		public:

            //-----------------------------------------------------------
            /// Constructor
            ///
            /// @author S Downie
            ///
            /// @param Red 0 - 255
            /// @param Green 0 - 255
            /// @param Blue 0 - 255
            /// @param Alpha 0 - 255
            //-----------------------------------------------------------
            ByteColour(u8 in_red = 255, u8 in_green = 255, u8 in_blue = 255, u8 in_alpha = 255);


		public:
			
			u8 r, g, b, a;
		};
        
        //-----------------------------------------------------------
        /// @author S Downie
        ///
        /// @param LHS of comparison
        /// @param RHS of comparison
        ///
        /// @return Whether the two colours have equal RGBA components
        //-----------------------------------------------------------
        inline bool operator==(const ByteColour& in_lhs, const ByteColour& in_rhs)
        {
            return in_lhs.r == in_rhs.r && in_lhs.g == in_rhs.g && in_lhs.b == in_rhs.b && in_lhs.a == in_rhs.a;
        }
        //-----------------------------------------------------------
        /// @author S Downie
        ///
        /// @param LHS of comparison
        /// @param RHS of comparison
        ///
        /// @return Whether the two colours have unequal RGBA components
        //-----------------------------------------------------------
        inline bool operator!=(const ByteColour& in_lhs, const ByteColour& in_rhs)
        {
            return in_lhs.r != in_rhs.r || in_lhs.g != in_rhs.g || in_lhs.b != in_rhs.b || in_lhs.a != in_rhs.a;
        }
	}
}

#endif