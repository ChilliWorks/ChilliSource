//
//  Colour.h
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

#ifndef _CHILLISOURCE_CORE_BASE_COLOUR_H_
#define _CHILLISOURCE_CORE_BASE_COLOUR_H_

#include <ChilliSource/ChilliSource.h>

namespace ChilliSource
{
	namespace Core
	{
        //-----------------------------------------------------------
        /// Container for RGBA colour as normalised 0.0 - 1.0 values
        ///
        /// @author S Downie
		//-----------------------------------------------------------
		class Colour
		{
		public:

            //Commonly used colours
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
            
            //-----------------------------------------------------------
            /// Clamp the given colour between the min and max values
            ///
            /// @author S Downie
            ///
            /// @param Value to clamp
            /// @param Min
            /// @param Max
            ///
            /// @return Clamped colour
            //-----------------------------------------------------------
            static Colour Clamp(const Colour& in_value, const Colour& in_min = k_transparent, const Colour& in_max = k_white);
            //-----------------------------------------------------------
            /// Constructor
            ///
            /// @author S Downie
            ///
            /// @param Normalised red 0.0 - 1.0
            /// @param Normalised green 0.0 - 1.0
            /// @param Normalised blue 0.0 - 1.0
            /// @param Normalised alpha 0.0 - 1.0
            //-----------------------------------------------------------
            Colour(f32 in_red = 1.0f, f32 in_green = 1.0f, f32 in_blue = 1.0f, f32 in_alpha = 1.0f);
            //-----------------------------------------------------------
            /// @author S Downie
            ///
            /// @param RHS of addition
            ///
            /// @return Component wise addition of colour to this
            //-----------------------------------------------------------
			inline const Colour& operator+=(const Colour& in_rhs)
            {
                r += in_rhs.r;
                g += in_rhs.g;
                b += in_rhs.b;
                a += in_rhs.a;
                
                return *this;
            }
            //-----------------------------------------------------------
            /// @author S Downie
            ///
            /// @param RHS of addition (scaler)
            ///
            /// @return Scaler addition to this
            //-----------------------------------------------------------
			inline const Colour& operator+=(f32 in_scaler)
            {
                r += in_scaler;
                g += in_scaler;
                b += in_scaler;
                a += in_scaler;
                
                return *this;
            }
            //-----------------------------------------------------------
            /// @author S Downie
            ///
            /// @param RHS of subtraction
            ///
            /// @return Component wise subtraction of colour to this
            //-----------------------------------------------------------
			inline const Colour& operator-=(const Colour& in_rhs)
            {
                r -= in_rhs.r;
                g -= in_rhs.g;
                b -= in_rhs.b;
                a -= in_rhs.a;
                
                return *this;
            }
            //-----------------------------------------------------------
            /// @author S Downie
            ///
            /// @param RHS of subtraction (scaler)
            ///
            /// @return Scaler subtraction to this
            //-----------------------------------------------------------
			inline const Colour& operator-=(f32 in_scaler)
            {
                r -= in_scaler;
                g -= in_scaler;
                b -= in_scaler;
                a -= in_scaler;
                
                return *this;
            }
            //-----------------------------------------------------------
            /// @author S Downie
            ///
            /// @param RHS of multiplication
            ///
            /// @return Component wise multiplication of colour to this
            //-----------------------------------------------------------
			inline const Colour& operator*=(const Colour& in_rhs)
            {
                r *= in_rhs.r;
                g *= in_rhs.g;
                b *= in_rhs.b;
                a *= in_rhs.a;
                
                return *this;
            }
            //-----------------------------------------------------------
            /// @author S Downie
            ///
            /// @param RHS of multiplication (scaler)
            ///
            /// @return Scaler multiplication to this
            //-----------------------------------------------------------
			inline const Colour& operator*=(f32 in_scaler)
            {
                r *= in_scaler;
                g *= in_scaler;
                b *= in_scaler;
                a *= in_scaler;
                
                return *this;
            }
            //-----------------------------------------------------------
            /// @author S Downie
            ///
            /// @param RHS of division
            ///
            /// @return Component wise division of colour to this
            //-----------------------------------------------------------
			inline const Colour& operator/=(const Colour& in_rhs)
            {
                r /= in_rhs.r;
                g /= in_rhs.g;
                b /= in_rhs.b;
                a /= in_rhs.a;
                
                return *this;
            }
            //-----------------------------------------------------------
            /// @author S Downie
            ///
            /// @param RHS of division (scaler)
            ///
            /// @return Scaler division to this
            //-----------------------------------------------------------
			inline const Colour& operator/=(f32 in_scaler)
            {
                r /= in_scaler;
                g /= in_scaler;
                b /= in_scaler;
                a /= in_scaler;
                
                return *this;
            }

		public:
			
			f32 r, g, b, a;
		};
        
        //-----------------------------------------------------------
        /// @author S Downie
        ///
        /// @param LHS of addition
        /// @param RHS of addition
        ///
        /// @return Component wise addition of colours
        //-----------------------------------------------------------
        inline Colour operator+(const Colour& in_lhs, const Colour& in_rhs)
        {
            return Colour(in_lhs.r + in_rhs.r, in_lhs.g + in_rhs.g, in_lhs.b + in_rhs.b, in_lhs.a + in_rhs.a);
        }
        //-----------------------------------------------------------
        /// @author S Downie
        ///
        /// @param LHS of addition (colour)
        /// @param RHS of addition (scaler)
        ///
        /// @return Scaler addition of colours
        //-----------------------------------------------------------
        inline Colour operator+(const Colour& in_colour, f32 in_scaler)
        {
            return Colour(in_colour.r + in_scaler, in_colour.g + in_scaler, in_colour.b + in_scaler, in_colour.a + in_scaler);
        }
        //-----------------------------------------------------------
        /// @author S Downie
        ///
        /// @param LHS of subtraction
        /// @param RHS of subtraction
        ///
        /// @return Component wise subtraction of colours
        //-----------------------------------------------------------
        inline Colour operator-(const Colour& in_lhs, const Colour& in_rhs)
        {
            return Colour(in_lhs.r - in_rhs.r, in_lhs.g - in_rhs.g, in_lhs.b - in_rhs.b, in_lhs.a - in_rhs.a);
        }
        //-----------------------------------------------------------
        /// @author S Downie
        ///
        /// @param LHS of subtraction (colour)
        /// @param RHS of subtraction (scaler)
        ///
        /// @return Scaler subtraction of colours
        //-----------------------------------------------------------
        inline Colour operator-(const Colour& in_colour, f32 in_scaler)
        {
            return Colour(in_colour.r - in_scaler, in_colour.g - in_scaler, in_colour.b - in_scaler, in_colour.a - in_scaler);
        }
        //-----------------------------------------------------------
        /// @author S Downie
        ///
        /// @param LHS of mulitplication
        /// @param RHS of mulitplication
        ///
        /// @return Component wise mulitplication of colours
        //-----------------------------------------------------------
        inline Colour operator*(const Colour& in_lhs, const Colour& in_rhs)
        {
            return Colour(in_lhs.r * in_rhs.r, in_lhs.g * in_rhs.g, in_lhs.b * in_rhs.b, in_lhs.a * in_rhs.a);
        }
        //-----------------------------------------------------------
        /// @author S Downie
        ///
        /// @param LHS of mulitplication (colour)
        /// @param RHS of mulitplication (scaler)
        ///
        /// @return Scaler mulitplication of colours
        //-----------------------------------------------------------
        inline Colour operator*(const Colour& in_colour, f32 in_scaler)
        {
            return Colour(in_colour.r * in_scaler, in_colour.g * in_scaler, in_colour.b * in_scaler, in_colour.a * in_scaler);
        }
        //-----------------------------------------------------------
        /// @author S Downie
        ///
        /// @param LHS of division
        /// @param RHS of division
        ///
        /// @return Component wise division of colours
        //-----------------------------------------------------------
        inline Colour operator/(const Colour& in_lhs, const Colour& in_rhs)
        {
            return Colour(in_lhs.r / in_rhs.r, in_lhs.g / in_rhs.g, in_lhs.b / in_rhs.b, in_lhs.a / in_rhs.a);
        }
        //-----------------------------------------------------------
        /// @author S Downie
        ///
        /// @param LHS of division (colour)
        /// @param RHS of division (scaler)
        ///
        /// @return Scaler division of colours
        //-----------------------------------------------------------
        inline Colour operator/(const Colour& in_colour, f32 in_scaler)
        {
            return Colour(in_colour.r / in_scaler, in_colour.g / in_scaler, in_colour.b / in_scaler, in_colour.a / in_scaler);
        }
        //-----------------------------------------------------------
        /// @author S Downie
        ///
        /// @param LHS of comparison
        /// @param RHS of comparison
        ///
        /// @return Whether the two colours have equal RGBA components
        //-----------------------------------------------------------
        inline bool operator==(const Colour& in_lhs, const Colour& in_rhs)
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
        inline bool operator!=(const Colour& in_lhs, const Colour& in_rhs)
        {
            return in_lhs.r != in_rhs.r || in_lhs.g != in_rhs.g || in_lhs.b != in_rhs.b || in_lhs.a != in_rhs.a;
        }
	}
}

#endif