//
//  Colour.cpp
//  Chilli Source
//
//  Created by Scott Downie on 27/09/2010.
//  Copyright 2010 Tag Games. All rights reserved.
//

#include <ChilliSource/Core/Base/Colour.h>

namespace ChilliSource
{
	namespace Core
	{
		const Colour Colour::k_white(1.0f,1.0f,1.0f,1.0f);
		const Colour Colour::k_transparent(0.0f,0.0f,0.0f,0.0f);
		const Colour Colour::k_black(0.0f,0.0f,0.0f,1.0f);
		const Colour Colour::k_grey(0.5f,0.5f,0.5f,1.0f);
		const Colour Colour::k_red(1.0f,0.0f,0.0f,1.0f);
		const Colour Colour::k_green(0.0f,1.0f,0.0f,1.0f);
		const Colour Colour::k_blue(0.0f,0.0f,1.0f,1.0f);
		const Colour Colour::k_magenta(1.0f,0.0f,1.0f,1.0f);
		const Colour Colour::k_yellow(1.0f,1.0f,0.0f,1.0f);
		const Colour Colour::k_cyan(0.0f,1.0f,1.0f,1.0f);
		const Colour Colour::k_cornflowerBlue(0.4f,0.5f,0.8f,1.0f);
		const Colour Colour::k_lightGrey(0.8f,0.8f,0.8f,1.0f);
		const Colour Colour::k_turquoise(0.25f,0.88f,0.82f,1.0f);
		const Colour Colour::k_orange(0.9f,0.4f,0.12f,1.0f);
		const Colour Colour::k_purple(0.5f,0.0f,0.5f,1.0f);
		const Colour Colour::k_pink(0.94f,0.36f,0.47f,1.0f);
		
        //-----------------------------------------------------------
		//-----------------------------------------------------------
		Colour::Colour(f32 in_red, f32 in_green, f32 in_blue, f32 in_alpha)
        : r(in_red), g(in_green), b(in_blue), a(in_alpha)
		{
		}
	}
}
