//
//  EaseBack.h
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

#ifndef _CHILLISOURCE_CORE_TWEEN_EASEBACK_H_
#define _CHILLISOURCE_CORE_TWEEN_EASEBACK_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Tween/Tween.h>

namespace ChilliSource
{
	namespace Core
	{
        namespace EaseBackTween
        {
            const f32 k_overshootFactor = 1.70158f;
            
            //-----------------------------------------------------------------------
            /// Functor that performs easing in with back overshoot on the start and end
            /// values based on the given normalised T (0 - 1)
            ///
            /// @author S Downie
            //-----------------------------------------------------------------------
            template <typename TValueType> struct EaseInInterpolate
            {
                TValueType m_startValue;
                TValueType m_endValue;
                
                //-----------------------------------------------------------------------
                /// Ease in with acceleration and inital back overshoot between the start and end
                /// values based on the given normalised T (0 - 1)
                ///
                /// @author S Downie
                ///
                /// @param Normalised T (0 - 1)
                ///
                /// @return Back eased-in type
                //-----------------------------------------------------------------------
                TValueType operator()(f32 in_t) const
                {
                    return (m_endValue - m_startValue) * in_t * in_t * ((k_overshootFactor + 1.0f) * in_t - k_overshootFactor) + m_startValue;
                }
            };
            
            //-----------------------------------------------------------------------
            /// Functor that performs easing out with overshoot on the start and end
            /// values based on the given normalised T (0 - 1)
            ///
            /// @author S Downie
            //-----------------------------------------------------------------------
            template <typename TValueType> struct EaseOutInterpolate
            {
                TValueType m_startValue;
                TValueType m_endValue;

                //-----------------------------------------------------------------------
                /// Ease out with deceleration and overshoot between the start and end
                /// values based on the given normalised T (0 - 1)
                ///
                /// @author S Downie
                ///
                /// @param Normalised T (0 - 1)
                ///
                /// @return Back eased-out type
                //-----------------------------------------------------------------------
                TValueType operator()(f32 in_t) const
                {
                    in_t -= 1.0f;
                    return (m_endValue - m_startValue) * (in_t * in_t * ((k_overshootFactor + 1.0f) * in_t + k_overshootFactor) + 1.0f) + m_startValue;
                }
            };
            
            //-----------------------------------------------------------------------
            /// Functor that performs easing in and out with overshoots
            /// on the start and end values based on the given normalised T (0 - 1)
            ///
            /// @author S Downie
            //-----------------------------------------------------------------------
            template <typename TValueType> struct EaseInOutInterpolate
            {
                EaseInInterpolate<TValueType> m_easeIn;
                EaseOutInterpolate<TValueType> m_easeOut;
                //-----------------------------------------------------------------------
                /// Ease in with acceleration until halfway then begin easing out
                /// with deceleration. Overshoots on both ends.
                /// This occurs between the start and end values based on the given normalised T (0 - 1)
                ///
                /// @author S Downie
                ///
                /// @param Normalised T (0 - 1)
                ///
                /// @return Back eased-in/out type
                //-----------------------------------------------------------------------
                TValueType operator()(f32 in_t) const
                {
                    if (in_t < 0.5f)
                    {
                        return m_easeIn(in_t * 2.0f);
                    }
                    
                    return m_easeOut((in_t - 0.5f) * 2.0f);
                }
            };
        }

		template <typename TValueType> using EaseInBackTween = Tween<EaseBackTween::EaseInInterpolate<TValueType>>;
		//-----------------------------------------------------------------------
		/// Convenience method to create an tween that performs
		/// easing in with initial overshoot
		///
        /// @author S Downie
        ///
		/// @param Start value
		/// @param End value
		/// @param Duration in seconds
		/// @param Start delay in seconds
        /// @param End delay in seconds
		///
		/// @return Tween with back ease-in function
		//-----------------------------------------------------------------------
		template <typename TValueType>
		EaseInBackTween<TValueType> MakeEaseInBackTween(TValueType in_start, TValueType in_end, f32 in_duration, f32 in_startDelay = 0.0f, f32 in_endDelay = 0.0f)
		{
			EaseBackTween::EaseInInterpolate<TValueType> func;
			func.m_startValue = in_start;
			func.m_endValue = in_end;
			return EaseInBackTween<TValueType>(func, in_duration, in_startDelay, in_endDelay);
		}
        
        template <typename TValueType> using EaseOutBackTween = Tween<EaseBackTween::EaseOutInterpolate<TValueType>>;
		//-----------------------------------------------------------------------
		/// Convenience method to create an tween that performs easing out
        /// with overshoot
        ///
        /// @author S Downie
		///
		/// @param Start value
		/// @param End value
		/// @param Duration in seconds
		/// @param Start delay in seconds
        /// @param End delay in seconds
		///
		/// @return Tween with back ease-in function
		//-----------------------------------------------------------------------
		template <typename TValueType>
		EaseOutBackTween<TValueType> MakeEaseOutBackTween(TValueType in_start, TValueType in_end, f32 in_duration, f32 in_startDelay = 0.0f, f32 in_endDelay = 0.0f)
		{
			EaseBackTween::EaseOutInterpolate<TValueType> func;
			func.m_startValue = in_start;
			func.m_endValue = in_end;
			return EaseOutBackTween<TValueType>(func, in_duration, in_startDelay, in_endDelay);
		}
        
        template <typename TValueType> using EaseInOutBackTween = Tween<EaseBackTween::EaseInOutInterpolate<TValueType>>;
		//-----------------------------------------------------------------------
		/// Convenience method to create an tween that performs
		/// easing in til halfway then easing out with overshoots
        ///
        /// @author S Downie
		///
		/// @param Start value
		/// @param End value
		/// @param Duration in seconds
		/// @param Start delay in seconds
        /// @param End delay in seconds
		///
		/// @return Tween with back ease-in/out function
		//-----------------------------------------------------------------------
		template <typename TValueType>
		EaseInOutBackTween<TValueType> MakeEaseInOutBackTween(TValueType in_start, TValueType in_end, f32 in_duration, f32 in_startDelay = 0.0f, f32 in_endDelay = 0.0f)
		{
			EaseBackTween::EaseInOutInterpolate<TValueType> func;
			func.m_easeIn.m_startValue = in_start;
            func.m_easeIn.m_endValue = (in_start + in_end)/2.0f;
            func.m_easeOut.m_startValue = func.m_easeIn.m_endValue;
            func.m_easeOut.m_endValue = in_end;
			return EaseInOutBackTween<TValueType>(func, in_duration, in_startDelay, in_endDelay);
		}
	}
}

#endif
