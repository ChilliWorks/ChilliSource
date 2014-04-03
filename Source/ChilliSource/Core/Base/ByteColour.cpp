//
//  ByteColour.cpp
//  Chilli Source
//
//  Created by Scott Downie on 03/04/2014.
//  Copyright 2014 Tag Games. All rights reserved.
//

#include <ChilliSource/Core/Base/ByteColour.h>

namespace ChilliSource
{
	namespace Core
	{
        //-----------------------------------------------------------
		//-----------------------------------------------------------
		ByteColour::ByteColour(u8 in_red, u8 in_green, u8 in_blue, u8 in_alpha)
        : r(in_red), g(in_green), b(in_blue), a(in_alpha)
		{
		}
	}
}
