//
//  ByteColour.h
//  Chilli Source
//
//  Created by Scott Downie on 03/04/2014.
//  Copyright 2014 Tag Games. All rights reserved.
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