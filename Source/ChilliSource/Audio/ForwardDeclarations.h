/*
 * File: ForwardDeclarations.h
 * Date: 16/11/2010 2010 
 * Description: 
 */

/*
 * Author: Scott Downie
 * Version: v 1.0
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#ifndef _MOFLO_AUDIO_FORWARDDECLARATIONS_H_
#define _MOFLO_AUDIO_FORWARDDECLARATIONS_H_

namespace ChilliSource
{
	namespace Audio
	{
		class IAudioSystem;
		class IAudioManager;
		
		class CAudioComponent;
        class CAudioListenerComponent;
        
        class IAudioResource;
		class IAudioListener;
		
		class CSoundEffect;
		class CAudioLoader;
		
		class IAudioComponentFactory;
		
		typedef std::shared_ptr<CAudioComponent> AudioComponentPtr;
		typedef std::weak_ptr<CAudioComponent> AudioComponentWeakPtr;

		typedef std::shared_ptr<CAudioListenerComponent> AudioListenerComponentPtr;
		typedef std::weak_ptr<CAudioListenerComponent> AudioListenerComponentWeakPtr;
        
        typedef std::shared_ptr<IAudioResource> AudioResourcePtr;
		typedef std::weak_ptr<IAudioResource> AudioResourceWeakPtr;
	}
}

#endif