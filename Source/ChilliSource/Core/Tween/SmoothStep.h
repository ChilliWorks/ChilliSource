//
//  SmoothStep.h
//  Chilli Source
//  Created by Scott Downie on 04/07/2014.
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

#ifndef _CHILLISOURCE_CORE_TWEEN_SMOOTHSTEP_H_
#define _CHILLISOURCE_CORE_TWEEN_SMOOTHSTEP_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Math/MathUtils.h>
#include <ChilliSource/Core/Tween/Tween.h>

namespace ChilliSource
{
	namespace Core
	{
		//-----------------------------------------------------------------------
		/// Functor that performs smoothstep interpolation on the start and end
		/// values based on the given normalised T (0 - 1)
		///
		/// @author S Downie
		//-----------------------------------------------------------------------
		template <typename TValueType> struct SmoothStepInterpolate
		{
			TValueType m_startValue;
			TValueType m_endValue;
            
			//-----------------------------------------------------------------------
			/// Smoothly interpolate between the start and end
			/// values based on the given normalised T (0 - 1)
			///
			/// @author S Downie
			///
			/// @param Normalised T (0 - 1)
			///
			/// @return SmoothStep interpolated type
			//-----------------------------------------------------------------------
			TValueType operator()(f32 in_t) const
			{
				return MathUtils::SmoothStep(in_t, m_startValue, m_endValue);
			}
		};
		template <typename TValueType> using SmoothStepTween = Tween<SmoothStepInterpolate<TValueType>>;
		//-----------------------------------------------------------------------
		/// Convenience method to create an tween that performs a smoothstep
		/// interpolation
        ///
        /// @author S Downie
		///
		/// @param Start value
		/// @param End value
		/// @param Duration in seconds
		/// @param Start delay in seconds
        /// @param End delay in seconds
		///
		/// @return Tween with smoothstep interp function
		//-----------------------------------------------------------------------
		template <typename TValueType>
		SmoothStepTween<TValueType> MakeSmoothStepTween(TValueType in_start, TValueType in_end, f32 in_duration, f32 in_startDelay = 0.0f, f32 in_endDelay = 0.0f)
		{
			SmoothStepInterpolate<TValueType> func;
			func.m_startValue = in_start;
			func.m_endValue = in_end;
			return SmoothStepTween<TValueType>(func, in_duration, in_startDelay, in_endDelay);
		}
	}
}

#endif
