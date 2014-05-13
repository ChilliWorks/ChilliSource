//
//  Animation.h
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
			/// @param Normalised T (0 - 1)
			//-----------------------------------------------------------------------
			void SetT(f32 in_t);
			//-----------------------------------------------------------------------
			/// Uses the set interpolation function to calculate an interpolated
			/// value from the current normalised T
			///
			/// @author S Downie
			///
			/// @return Interpolated value at current T
			//-----------------------------------------------------------------------
			auto GetValue() const -> decltype(std::declval<TInterpFunc>()(0.0f))
			{
				return m_interpFunc(m_currentT);
			}

		private:

			StateChangeDelegate m_startDelegate;
			StateChangeDelegate m_endDelegate;

			TInterpFunc m_interpFunc;

			f32 m_duration = 0.0f;
			f32 m_stepDuration = 0.0f;
			f32 m_startDelay = 0.0f;
			f32 m_currentDelay = 0.0f;
			f32 m_currentTime = 0.0f;
			f32 m_timeScaler = 1.0f;
			f32 m_currentT = 0.0f;

			u32 m_currentStep = 0;
			u32 m_steps = 0;

			bool m_isPlaying = false;
			bool m_isFinished = false;
			bool m_isStarted = false;
			bool m_isReverse = false;
			bool m_isPingPong = false;
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
			CS_ASSERT(m_duration >= 0.0f, "Must set a valid duration");

			switch (in_playMode)
			{
			case AnimationPlayMode::k_once:
				m_steps = 1;
				m_isReverse = false;
				m_isPingPong = false;
				m_stepDuration = m_duration;
				break;
			case AnimationPlayMode::k_pingPong:
				m_steps = 2;
				m_isReverse = false;
				m_isPingPong = true;
				m_stepDuration = m_duration * 0.5f;
				break;
			case AnimationPlayMode::k_looping:
				//Zero is looping
				m_steps = 0;
				m_isReverse = false;
				m_isPingPong = false;
				m_stepDuration = m_duration;
				break;
			case AnimationPlayMode::k_pingPongLooping:
				//Zero is looping
				m_steps = 0;
				m_isReverse = false;
				m_isPingPong = true;
				m_stepDuration = m_duration * 0.5f;
				break;
			case AnimationPlayMode::k_onceReverse:
				m_steps = 1;
				m_isReverse = true;
				m_isPingPong = false;
				m_stepDuration = m_duration;
				break;
			case AnimationPlayMode::k_loopingReverse:
				//Zero is looping
				m_steps = 0;
				m_isReverse = true;
				m_isPingPong = false;
				m_stepDuration = m_duration;
				break;
			case AnimationPlayMode::k_pingPongReverse:
				m_steps = 2;
				m_isReverse = true;
				m_isPingPong = true;
				m_stepDuration = m_duration * 0.5f;
				break;
			case AnimationPlayMode::k_pingPongLoopingReverse:
				//Zero is looping
				m_steps = 0;
				m_isReverse = true;
				m_isPingPong = true;
				m_stepDuration = m_duration * 0.5f;
				break;
			}

			m_currentDelay = m_startDelay;
			m_currentTime = 0.0f;
			m_currentStep = 0;

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
			if (m_isPlaying == false)
			{
				return;
			}

#if CS_ENABLE_DEBUG
			in_timeSinceLastUpdate = std::min(in_timeSinceLastUpdate, 0.333f);
#endif
			in_timeSinceLastUpdate *= m_timeScaler;

			//Inform the delegate that the animation has begun
			if (m_isStarted == false)
			{
				//We may have a countdown before starting
				m_currentDelay -= in_timeSinceLastUpdate;
				if (m_currentDelay > 0.0f)
				{
					return;
				}

				m_isStarted = true;

				if (m_startDelegate != nullptr)
				{
					m_startDelegate(this);
				}
			}

			m_currentTime = std::min(m_currentTime + in_timeSinceLastUpdate, m_duration);

			m_currentT = m_currentTime / m_stepDuration;
			if (m_isReverse)
				m_currentT = 1.0f - m_currentT;

			if (m_currentTime >= m_stepDuration)
			{
				m_currentStep++;

				if (m_isPingPong == true)
				{
					m_isReverse = !m_isReverse;
				}

				//Step number of zero = looping
				if (m_steps > 0 && m_currentStep >= m_steps)
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
					m_currentTime = 0.0f;
				}
			}
		}
		//-----------------------------------------------------------------------
		//-----------------------------------------------------------------------
		template <typename TInterpFunc> void Animation<TInterpFunc>::SetT(f32 in_t)
		{
			CS_ASSERT(in_t >= 0.0f && in_t <= 1.0f, "T must be between 0 and 1");

			m_currentTime = in_t * m_duration;
			m_currentT = in_t;
		}
	}
}

#endif
