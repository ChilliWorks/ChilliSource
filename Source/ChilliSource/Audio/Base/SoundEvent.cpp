/*
 * File: SoundBatch.cpp
 * Date: 22/11/2010 2010 
 * Description: 
 */

/*
 * Author: Scott Downie
 * Version: v 1.0
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#include <ChilliSource/Audio/Base/SoundEvent.h>

namespace ChilliSource
{
	namespace Audio
	{
		//-------------------------------------------------------------
		/// Constructor
		///
		/// Default
		//-------------------------------------------------------------
		CSoundEvent::CSoundEvent() : mbIsPlaying(false), mfTimeSincePlay(0.0f)
		{
		}
		//-------------------------------------------------------------
		/// Add Sound
		///
		/// Add sound to batch with given delay. i.e. when the batch
		/// is played the sound will be offset by that amount.
		///
		/// @param Sound sample
		/// @param Delay (seconds)
		//-------------------------------------------------------------
		void CSoundEvent::AddSound(const AudioComponentPtr &inSound, f32 infDelayInSecs)
		{
			mSoundAndDelay.push_back(std::make_pair(inSound, infDelayInSecs));
		}
		//-------------------------------------------------------------
		/// Play
		///
		/// Play all sounds in the batch
		//-------------------------------------------------------------
		void CSoundEvent::Play()
		{
			//Begin timing for offsets
			mfTimeSincePlay = 0.0f;
			mbIsPlaying = true;
			mPlayedIndices.reserve(mSoundAndDelay.size());
		}
		//-------------------------------------------------------------
		/// Pause
		///
		/// Pause all sounds in the batch
		//-------------------------------------------------------------
		void CSoundEvent::Pause()
		{
			for(std::vector< std::pair<AudioComponentPtr, f32> >::iterator it = mSoundAndDelay.begin(); it != mSoundAndDelay.end(); ++it)
			{
				it->first->Pause();
			}
			
			mbIsPlaying = false;
		}
		//-------------------------------------------------------------
		/// Update
		///
		/// @param Time between frames
		//-------------------------------------------------------------
		void CSoundEvent::Update(f32 dt)
		{
			if(mbIsPlaying)
			{
				mfTimeSincePlay += dt;
				
				//Check all the sounds and see if we should play them
				for(std::vector< std::pair<AudioComponentPtr, f32> >::iterator it = mSoundAndDelay.begin(); it != mSoundAndDelay.end(); ++it)
				{
					if(it->second >= 0.0f && mfTimeSincePlay >= it->second)
					{
						//Play the sound and set it's delay to be negative so we don't play it again
						it->first->Play();
						//Store it's delay so we can restore it once all the sounds have played
						mPlayedIndices.push_back(it->second);
						it->second = -1.0f;
						
						if(mPlayedIndices.size() == mSoundAndDelay.size())
						{
							//All the sounds have played from this batch. We are done
							mbIsPlaying = false;
							u32 i = 0;
							for(std::vector< std::pair<AudioComponentPtr, f32> >::iterator it = mSoundAndDelay.begin(); it != mSoundAndDelay.end(); ++it)
							{
								it->second = mPlayedIndices[i];
								i++;
							}
							mPlayedIndices.clear();
							return;
						}
					}
				}
			}
		}
	}
}