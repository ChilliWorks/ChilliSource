/*
 *  Colour.h
 *  moFlo
 *
 *  Created by Scott Downie on 27/09/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#ifndef _MO_FLO_CORE_COLOUR_H_
#define _MO_FLO_CORE_COLOUR_H_

#include <ChilliSource/ChilliSource.h>

namespace ChilliSource
{
	namespace Core
	{
		//==========================================================
		/// Description:
		///
		/// A wrapper around normalised rgba colours.
		//==========================================================
		class Colour
		{
		public:
            struct ByteColour
            {
                u8 r;
                u8 g;
                u8 b;
                u8 a;
            };
            
            Colour();
			explicit Colour(f32 inR, f32 inG, f32 inB, f32 inA);
			explicit Colour(u8 inR, u8 inG, u8 inB, u8 inA);
            explicit Colour(u32 inudwRGBA);
			static Colour FromByteRGBA(u8 inR, u8 inG, u8 inB, u8 inA);
            static Colour::ByteColour ColourToByteColour(const Colour& inCol);
			
			//---------------------------------
			// Operators
			//---------------------------------
			Colour operator=(Colour Col){r = Col.r; g = Col.g; b = Col.b; a = Col.a; return *this;};
			
			Colour operator+(const Colour Col) const
			{Colour Result; Result.r = this->r + Col.r; Result.g = this->g + Col.g; Result.b = this->b + Col.b; Result.a = this->a + Col.a; return Result;}
			
			Colour operator+(const f32 Scale) const
			{Colour Result; Result.r = this->r + Scale; Result.g = this->g + Scale; Result.b = this->b + Scale; Result.a = this->a + Scale; return Result;}
			
			Colour operator-(const Colour Col) const
			{Colour Result; Result.r = this->r - Col.r; Result.g = this->g - Col.g; Result.b = this->b - Col.b; Result.a = this->a - Col.a; return Result;}
			
			Colour operator-(const f32 Scale) const
			{Colour Result; Result.r = this->r - Scale; Result.g = this->g - Scale; Result.b = this->b - Scale; Result.a = this->a - Scale; return Result;}
			
			Colour operator*(const Colour Col) const
			{Colour Result; Result.r = this->r * Col.r; Result.g = this->g * Col.g; Result.b = this->b * Col.b; Result.a = this->a * Col.a; return Result;}
			
			Colour operator*(const f32 Scale) const
			{Colour Result; Result.r = this->r * Scale; Result.g = this->g * Scale; Result.b = this->b * Scale; Result.a = this->a * Scale; return Result;}
			
			const Colour& operator+=(const Colour &Col){this->r += Col.r; this->g += Col.g; this->b += Col.b; this->a += Col.a; return *this;}
			
			const Colour& operator-=(const Colour &Col){this->r -= Col.r; this->g -= Col.g; this->b -= Col.b; this->a -= Col.a; return *this;}
			
			const Colour& operator*=(const Colour &Col){this->r *= Col.r; this->g *= Col.g; this->b *= Col.b; this->a *= Col.a; return *this;}
			
			const Colour& operator+=(const f32 Scale){this->r += Scale; this->g += Scale; this->b += Scale; this->a += Scale; return *this;}
			
			const Colour& operator-=(const f32 Scale){this->r -= Scale; this->g -= Scale; this->b -= Scale; this->a -= Scale; return *this;}
			
			const Colour& operator*=(const f32 Scale){this->r *= Scale; this->g *= Scale; this->b *= Scale; this->a *= Scale; return *this;}
			
			bool operator==(const Colour &Col) const{return (Col.r == r && Col.g == g && Col.b == b && Col.a == a);}
			
			bool operator!=(const Colour &Col) const{return (Col.r != r || Col.g != g || Col.b != b || Col.a != a);}
			
			bool operator>(const Colour &Col) const{return Col.r > r || Col.g > g || Col.b > b || Col.a > a;}
			
			bool operator<(const Colour &Col) const{return Col.r < r || Col.g < g || Col.b < b || Col.a < a;}
			
			bool operator>=(const Colour &Col) const{return Col.r >= r || Col.g >= g || Col.b >= b || Col.a >= a;}
			
			bool operator<=(const Colour &Col) const{return Col.r <= r || Col.g <= g || Col.b <= b || Col.a <= a;}
			
			static const Colour k_white;
			static const Colour k_transparent;
			static const Colour k_black;
			static const Colour k_grey;
			static const Colour k_red;
			static const Colour k_green;
			static const Colour k_blue;
			static const Colour k_magenta;
			static const Colour k_yellow;
			static const Colour k_cyan;
			static const Colour k_cornflowerBlue;
            static const Colour k_lightGrey;
            static const Colour k_turquoise;
            static const Colour k_orange;
            static const Colour k_purple;
            static const Colour k_pink;
			
		public:
			
			float r, g, b, a;
		};
	}
}

#endif