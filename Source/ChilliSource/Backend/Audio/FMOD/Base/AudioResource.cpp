/*
 * File: FMODSoundEffect.cpp
 * Date: 16/11/2010 2010 
 * Description: 
 */

/*
 * Author: Scott Downie
 * Version: v 1.0
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#include <ChilliSource/Backend/Audio/FMOD/Base/AudioResource.h>

#include <ChilliSource/Backend/Audio/FMOD/Base/FMODSystem.h>

namespace ChilliSource
{
	namespace FMOD
	{
		//-------------------------------------------------
		/// Constructor
		//-------------------------------------------------
		AudioResource::AudioResource() : mpFMODSound(nullptr)
		{
            Audio::AudioResource::SetStreamed(false);
		}
        //-------------------------------------------------
        /// Is A
        //-------------------------------------------------
        bool AudioResource::IsA(ChilliSource::Core::InterfaceIDType inInterfaceID) const
        {
            return (inInterfaceID == Audio::AudioResource::InterfaceID);
        }
		//--------------------------------------------------
		/// Get Length
		//--------------------------------------------------
		f32 AudioResource::GetLength()
		{
            u32 udwLengthInMs = 0;
            
            if (mpFMODSound != nullptr)
            {
                mpFMODSound->getLength(&udwLengthInMs, FMOD_TIMEUNIT_MS);
            }
            
			f32 fLengthInSecs = udwLengthInMs * 0.001f;
			return fLengthInSecs;
		}
        //--------------------------------------------------
        /// Set Looping
        //--------------------------------------------------
        void AudioResource::SetLooping(bool inbShouldLoop)
        {
            if (mpFMODSound != nullptr)
            {
                inbShouldLoop ? mpFMODSound->setMode(FMOD_LOOP_NORMAL) : mpFMODSound->setMode(FMOD_LOOP_OFF);
            }
        }
		//--------------------------------------------------
		/// Destructor
		//--------------------------------------------------
		AudioResource::~AudioResource()
		{
            if (mpFMODSound != nullptr)
            {
                mpFMODSound->release();
            }
		}
	}
}