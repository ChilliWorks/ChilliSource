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

namespace moFlo
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
		
		typedef SHARED_PTR<CAudioComponent> AudioComponentPtr;
		typedef WEAK_PTR<CAudioComponent> AudioComponentWeakPtr;

		typedef SHARED_PTR<CAudioListenerComponent> AudioListenerComponentPtr;
		typedef WEAK_PTR<CAudioListenerComponent> AudioListenerComponentWeakPtr;
        
        typedef SHARED_PTR<IAudioResource> AudioResourcePtr;
		typedef WEAK_PTR<IAudioResource> AudioResourceWeakPtr;
	}
}

#endif