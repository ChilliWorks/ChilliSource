//
//  Animation.h
//  Chilli Source
//  Created by Stuart McGaw on 26/10/2010.
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

#ifndef _CHILLISOURCE_CORE_ANIMATION_ANIMATION_H_
#define _CHILLISOURCE_CORE_ANIMATION_ANIMATION_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Math/MathUtils.h>

#include <functional>

namespace ChilliSource
{
	namespace Core
	{
		//-----------------------------------------------------------------------
		/// List of play modes for animations.
		///
		/// @author S Downie
		//-----------------------------------------------------------------------
		enum class AnimationPlayMode
		{
			k_once,
			k_looping,
			k_pingPong,
			k_pingPongLooping,
			k_onceReverse,
			k_loopingReverse,
			k_pingPongReverse,
			k_pingPongLoopingReverse
		};
		//-----------------------------------------------------------------------
		/// Functor that performs linear interpolation on the start and end
		/// values based on the given normalised progress (0 - 1)
		///
		/// @author S Downie
		//-----------------------------------------------------------------------
		template <typename TValueType> struct LinearInterpolate
		{
			TValueType m_startValue;
			TValueType m_endValue;
			//-----------------------------------------------------------------------
			/// Linearly interpolate between the start and end
			/// values based on the given normalised progress (0 - 1)
			///
			/// @author S Downie
			///
			/// @param Normalised progress (0 - 1)
			///
			/// @return Linearly interpolated type
			//-----------------------------------------------------------------------
			TValueType operator()(f32 in_progress) const
			{
				return MathUtils::Lerp(in_progress, m_startValue, m_endValue);
			}
		};

		template <typename TValueType> using LinearAnimation = Animation<LinearInterpolate<TValueType>>;

		template <typename TValueType>
		LinearAnimation<TValueType> MakeLinearAnim(TValueType in_start, TValueType in_end, f32 in_duration, f32 in_delay = 0.0f)
		{
			LinearInterpolate<TValueType> func;
			func.m_startValue = in_start;
			func.m_endValue = in_end;
			return LinearAnimation<TValueType>(func, in_duration, in_delay);
		}

		//-----------------------------------------------------------------------
		/// Generic animation class that combines value types and an interpolation
		/// function to animate values over time.
		///
		/// @author S Downie
		//-----------------------------------------------------------------------
		template <typename TInterpFunc> class Animation
		{
		public:
			//-----------------------------------------------------------------------
			/// Delegate that is called on animation state change i.e. begin and end
			///
			/// @author S Downie
			///
			/// @param Animation on which state has changed
			//-----------------------------------------------------------------------
			using StateChangeDelegate = std::function<void(Animation*)>;
			//-----------------------------------------------------------------------
			/// Constructor for initialising later
			///
			/// @author S Downie
			//-----------------------------------------------------------------------
			Animation()
			{

			}
			//-----------------------------------------------------------------------
			/// Constructor
			///
			/// @author S Downie
			///
			/// @param Interpolation function
			/// @param Start value
			/// @param End value
			/// @param Duration of a single cycle in seconds
			/// @param Delay of initial cycle in seconds
			//-----------------------------------------------------------------------
			Animation(const TInterpFunc& in_interpFunc, f32 in_duration, f32 in_delay = 0.0f)
				: m_interpFunc(in_interpFunc), m_duration(in_duration), m_startDelay(in_delay)
			{

			}
			//-----------------------------------------------------------------------
			/// @author S Downie
			///
			/// @param Function that is used to interpolate values based on
			/// the current progress
			//-----------------------------------------------------------------------
			void SetInterpolateFunction(const TInterpFunc& in_interpFunc);
			//-----------------------------------------------------------------------
			/// @author S Downie
			///
			/// @param Delegate to be triggered on animation start
			//-----------------------------------------------------------------------
			void SetOnStartDelegate(const StateChangeDelegate& in_delegate);
			//-----------------------------------------------------------------------
			/// @author S Downie
			///
			/// @param Delegate to be triggered on animation end (not on looping)
			//-----------------------------------------------------------------------
			void SetOnEndDelegate(const StateChangeDelegate& in_delegate);
			//-----------------------------------------------------------------------
			/// @author S Downie
			///
			/// @param Delay in seconds of start of initial cycle
			//-----------------------------------------------------------------------
			void SetStartDelay(f32 in_delay);
			//-----------------------------------------------------------------------
			/// @author S Downie
			///
			/// @param Duration in seconds of one cycle
			//-----------------------------------------------------------------------
			void SetDuration(f32 in_duration);
			//-----------------------------------------------------------------------
			/// Scales the update time to slow down or speed up the animation
			///
			/// @author S Downie
			///
			/// @param Scaler
			//-----------------------------------------------------------------------
			void SetTimeScaleFactor(f32 in_scaler);
			//-----------------------------------------------------------------------
			/// Start the animation and set the play mode
			///
			/// @author S Downie
			///
			/// @param Play mode
			//-----------------------------------------------------------------------
			void Play(AnimationPlayMode in_playMode);
			//-----------------------------------------------------------------------
			/// Stop playback of the animation
			///
			/// @author S Downie
			//-----------------------------------------------------------------------
			void Stop();
			//-----------------------------------------------------------------------
			/// @author S Downie
			///
			/// @return Whether the animation is playing (animation is not playing if
			/// delay is counting down)
			//-----------------------------------------------------------------------
			bool IsPlaying() const;
			//-----------------------------------------------------------------------
			/// @author S Downie
			///
			/// @return Whether the animation has finished (can't be finished unless
			/// started).
			//-----------------------------------------------------------------------
			bool IsFinished() const;
			//-----------------------------------------------------------------------
			/// @author S Downie
			///
			/// @return Whether the animation has started (once finished it is no
			/// longer started).
			//-----------------------------------------------------------------------
			bool IsStarted() const;
			//-----------------------------------------------------------------------
			/// Update the progress of the animation with respect to time
			///
			/// @author S Downie
			///
			/// @param Time in seconds since last update
			//-----------------------------------------------------------------------
			void Update(f32 in_timeSinceLastUpdate);
			//-----------------------------------------------------------------------
			/// @author S Downie
			///
			/// @return Normalised progress (0 - 1)
			//-----------------------------------------------------------------------
			f32 GetProgress() const;
			//-----------------------------------------------------------------------
			/// @author S Downie
			///
			/// @param Normalised progress (0 - 1)
			//-----------------------------------------------------------------------
			void SetProgress(f32 in_progress);
			//-----------------------------------------------------------------------
			/// Uses the set interpolation function to calculate an interpolated
			/// value (based on start and end) from the current progress
			///
			/// @author S Downie
			///
			/// @return Interpolated value at current progress
			//-----------------------------------------------------------------------
			auto GetValue() const -> decltype(std::declval<TInterpFunc>()(0.0f))
			{
				return m_interpFunc(m_currentProgress);
			}

		private:

			StateChangeDelegate m_startDelegate;
			StateChangeDelegate m_endDelegate;

			TInterpFunc m_interpFunc;

			f32 m_duration = 0.0f;
			f32 m_startDelay = 0.0f;
			f32 m_currentDelay = 0.0f;
			f32 m_currentTime = 0.0f;
			f32 m_currentDirection = 1.0f;
			f32 m_timeScaler = 1.0f;
			f32 m_currentProgress = 0.0f;
			f32 m_startTime = 0.0f;
			f32 m_endTime = 0.0f;

			bool m_isPlaying = false;
			bool m_isFinished = false;
			bool m_isStarted = false;
			bool m_isLoopingMode = false;
			bool m_isLoopReverse = false;
		};
		//-----------------------------------------------------------------------
		//-----------------------------------------------------------------------
		template <typename TInterpFunc> void Animation<TInterpFunc>::SetInterpolateFunction(const TInterpFunc& in_interpFunc)
		{
			m_interpFunc = in_interpFunc;
		}
		//-----------------------------------------------------------------------
		//-----------------------------------------------------------------------
		template <typename TInterpFunc> void Animation<TInterpFunc>::SetOnStartDelegate(const StateChangeDelegate& in_delegate)
		{
			m_startDelegate = in_delegate;
		}
		//-----------------------------------------------------------------------
		//-----------------------------------------------------------------------
		template <typename TInterpFunc> void Animation<TInterpFunc>::SetOnEndDelegate(const StateChangeDelegate& in_delegate)
		{
			m_endDelegate = in_delegate;
		}
		//-----------------------------------------------------------------------
		//-----------------------------------------------------------------------
		template <typename TInterpFunc> void Animation<TInterpFunc>::SetStartDelay(f32 in_delay)
		{
			m_startDelay = in_delay;
		}
		//-----------------------------------------------------------------------
		//-----------------------------------------------------------------------
		template <typename TInterpFunc> void Animation<TInterpFunc>::SetDuration(f32 in_duration)
		{
			m_duration = in_duration;
		}
		//-----------------------------------------------------------------------
		//-----------------------------------------------------------------------
		template <typename TInterpFunc> void Animation<TInterpFunc>::SetTimeScaleFactor(f32 in_scaler)
		{
			m_timeScaler = in_scaler;
		}
		//-----------------------------------------------------------------------
		//-----------------------------------------------------------------------
		template <typename TInterpFunc> void Animation<TInterpFunc>::Play(AnimationPlayMode in_playMode)
		{
			//TODO: Fix bug with ping pong. Fix bug with reverse
			switch (in_playMode)
			{
			case AnimationPlayMode::k_once:
				m_currentTime = 0.0f;
				m_startTime = 0.0f;
				m_endTime = m_duration;
				m_currentDirection = 1.0f;
				m_isLoopingMode = false;
				m_isLoopReverse = false;
				break;
			case AnimationPlayMode::k_looping:
			case AnimationPlayMode::k_pingPong:
			case AnimationPlayMode::k_pingPongLooping:
				m_currentTime = 0.0f;
				m_startTime = 0.0f;
				m_endTime = m_duration;
				m_currentDirection = 1.0f;
				m_isLoopingMode = true;
				m_isLoopReverse = false;
				break;
			case AnimationPlayMode::k_onceReverse:
				m_currentTime = m_duration;
				m_startTime = m_duration;
				m_endTime = 0.0f;
				m_currentDirection = -1.0f;
				m_isLoopingMode = false;
				m_isLoopReverse = false;
				break;
			case AnimationPlayMode::k_loopingReverse:
			case AnimationPlayMode::k_pingPongReverse:
			case AnimationPlayMode::k_pingPongLoopingReverse:
				m_currentTime = m_duration;
				m_startTime = m_duration;
				m_endTime = 0.0f;
				m_currentDirection = -1.0f;
				m_isLoopingMode = true;
				m_isLoopReverse = true;
				break;
			}

			m_currentDelay = m_startDelay;

			m_isStarted = false;
			m_isPlaying = true;
			m_isFinished = false;
		}
		//-----------------------------------------------------------------------
		//-----------------------------------------------------------------------
		template <typename TInterpFunc> void Animation<TInterpFunc>::Stop()
		{
			m_isPlaying = false;
		}
		//-----------------------------------------------------------------------
		//-----------------------------------------------------------------------
		template <typename TInterpFunc> bool Animation<TInterpFunc>::IsPlaying() const
		{
			return m_isPlaying;
		}
		//-----------------------------------------------------------------------
		//-----------------------------------------------------------------------
		template <typename TInterpFunc> bool Animation<TInterpFunc>::IsFinished() const
		{
			return m_isFinished;
		}
		//-----------------------------------------------------------------------
		//-----------------------------------------------------------------------
		template <typename TInterpFunc> bool Animation<TInterpFunc>::IsStarted() const
		{
			return m_isStarted;
		}
		//-----------------------------------------------------------------------
		//-----------------------------------------------------------------------
		template <typename TInterpFunc> void Animation<TInterpFunc>::Update(f32 in_timeSinceLastUpdate)
		{
			//Return if not playing
			if (m_isPlaying == false)
			{
				return;
			}

#if CS_ENABLE_DEBUG
			in_timeSinceLastUpdate = std::min(in_timeSinceLastUpdate, 0.333f);
#endif
			in_timeSinceLastUpdate *= m_timeScaler;

			//We may have a countdown before starting
			m_currentDelay -= in_timeSinceLastUpdate;
			if (m_currentDelay > 0.0f)
			{
				return;
			}

			//Inform the delegate that the animation has begun
			if (m_isStarted == false)
			{
				m_isStarted = true;

				if (m_startDelegate != nullptr)
				{
					m_startDelegate(this);
				}
			}

			m_currentTime += (in_timeSinceLastUpdate * m_currentDirection);
			m_currentTime = MathUtils::Clamp(m_currentTime, 0.0f, m_duration);

			m_currentProgress = GetProgress();

			if (m_currentProgress >= 1.0f)
			{
				if (m_isLoopingMode == false)
				{
					m_isStarted = false;
					m_isPlaying = false;
					m_isFinished = true;

					if (m_endDelegate != nullptr)
					{
						m_endDelegate(this);
					}
				}
				else
				{
					if (m_isLoopReverse == false)
					{
						m_currentTime = 0.0f;
						m_currentProgress = 0.0f;
						m_currentDirection = 1.0f;
					}
					else
					{
						m_currentProgress = 1.0f;
						m_currentDirection = -1.0f;
					}
				}
			}
		}
		//-----------------------------------------------------------------------
		//-----------------------------------------------------------------------
		template <typename TInterpFunc> f32 Animation<TInterpFunc>::GetProgress() const
		{
			return MathUtils::NormalisedRange(m_currentTime, m_startTime, m_endTime);
		}
		//-----------------------------------------------------------------------
		//-----------------------------------------------------------------------
		template <typename TInterpFunc> void Animation<TInterpFunc>::SetProgress(f32 in_progress)
		{
			f32 progress = MathUtils::Clamp(in_progress, 0.0f, 1.0f);

			if (m_currentDirection < 0.0f)
			{
				fProgress = 1.0f - fProgress;
			}

			m_currentTime = progress * m_duration;
			m_currentProgress = progress;
		}
	}
}

#endif
