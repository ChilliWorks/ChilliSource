//
//  LinearAnimation.h
//  Chilli Source
//  Created by Scott Downie on 12/05/2014.
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

#ifndef _CHILLISOURCE_CORE_ANIMATION_LINEARANIMATION_H_
#define _CHILLISOURCE_CORE_ANIMATION_LINEARANIMATION_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Animation/Animation.h>
#include <ChilliSource/Core/Math/MathUtils.h>

namespace ChilliSource
{
	namespace Core
	{
		//-----------------------------------------------------------------------
		/// Functor that performs linear interpolation on the start and end
		/// values based on the given normalised T (0 - 1)
		///
		/// @author S Downie
		//-----------------------------------------------------------------------
		template <typename TValueType> struct LinearInterpolate
		{
			TValueType m_startValue;
			TValueType m_endValue;
			//-----------------------------------------------------------------------
			/// Linearly interpolate between the start and end
			/// values based on the given normalised T (0 - 1)
			///
			/// @author S Downie
			///
			/// @param Normalised T (0 - 1)
			///
			/// @return Linearly interpolated type
			//-----------------------------------------------------------------------
			TValueType operator()(f32 in_t) const
			{
				return MathUtils::Lerp(in_t, m_startValue, m_endValue);
			}
		};
		template <typename TValueType> using LinearAnimation = Animation<LinearInterpolate<TValueType>>;
		//-----------------------------------------------------------------------
		/// Convenience method to create an animation that performs a linear
		/// interpolation
		///
		/// @param Start value
		/// @param End value
		/// @param Duration in seconds
		/// @param Start delay in seconds
		///
		/// @return Animation with linear interp function
		//-----------------------------------------------------------------------
		template <typename TValueType>
		LinearAnimation<TValueType> MakeLinearAnim(TValueType in_start, TValueType in_end, f32 in_duration, f32 in_delay = 0.0f)
		{
			LinearInterpolate<TValueType> func;
			func.m_startValue = in_start;
			func.m_endValue = in_end;
			return LinearAnimation<TValueType>(func, in_duration, in_delay);
		}
	}
}

#endif
