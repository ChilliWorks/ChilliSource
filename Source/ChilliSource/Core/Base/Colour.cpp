//
//  Colour.cpp
//  Chilli Source
//  Created by Scott Downie on 27/09/2010.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2010 Tag Games Limited
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

#include <ChilliSource/Core/Base/Colour.h>

#include <ChilliSource/Core/Math/MathUtils.h>

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
        Colour Colour::Clamp(const Colour& in_value, const Colour& in_min, const Colour& in_max)
        {
            Colour result;
            result.r = MathUtils::Clamp(in_value.r, in_min.r, in_max.r);
            result.g = MathUtils::Clamp(in_value.g, in_min.g, in_max.g);
            result.b = MathUtils::Clamp(in_value.b, in_min.b, in_max.b);
            result.a = MathUtils::Clamp(in_value.a, in_min.a, in_max.a);
            return result;
        }
        //-----------------------------------------------------------
		//-----------------------------------------------------------
		Colour::Colour(f32 in_red, f32 in_green, f32 in_blue, f32 in_alpha)
        : r(in_red), g(in_green), b(in_blue), a(in_alpha)
		{
		}
	}
}
