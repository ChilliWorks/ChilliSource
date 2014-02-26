/*
 * File: SoundBatch.h
 * Date: 22/11/2010 2010 
 * Description: Allows multiple sounds to be played with one play call. Allows for time offsets to be given to each sound
 */

/*
 * Author: Scott Downie
 * Version: v 1.0
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#ifndef _MO_FLO_AUDIO_SOUND_EVENT_H_
#define _MO_FLO_AUDIO_SOUND_EVENT_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Audio/3D/AudioComponent.h>

namespace ChilliSource
{
	namespace Audio
	{
		class CSoundEvent
		{
		public:
			CSoundEvent();
			
			//-------------------------------------------------------------
			/// Add Sound
			///
			/// Add sound to batch with given delay. i.e. when the batch
			/// is played the sound will be offset by that amount.
			///
			/// @param Sound sample
			/// @param Delay (seconds)
			//-------------------------------------------------------------
			void AddSound(const AudioComponentPtr &inSound, f32 infDelayInSecs);
			//-------------------------------------------------------------
			/// Play
			///
			/// Play all sounds in the batch
			//-------------------------------------------------------------
			void Play();
			//-------------------------------------------------------------
			/// Pause
			///
			/// Pause all sounds in the batch
			//-------------------------------------------------------------
			void Pause();
			//-------------------------------------------------------------
			/// Update
			///
			/// @param Time between frames
			//-------------------------------------------------------------
			void Update(f32 dt);
		private:
			
			std::vector< std::pair<AudioComponentPtr, f32> > mSoundAndDelay;
			std::vector<f32> mPlayedIndices;
			
			f32 mfTimeSincePlay;
			bool mbIsPlaying;
		};
	}
}

#endif