//
//  AudioLoader.h
//  Chilli Source
//
//  Created by Scott Downie on 16/11/2010.
//  Copyright 2010 Tag Games. All rights reserved.
//

#ifndef _CHILLISOURCE_AUDIO_BASE_AUDIOLOADER_H_
#define _CHILLISOURCE_AUDIO_BASE_AUDIOLOADER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Resource/ResourceProvider.h>
#include <ChilliSource/Audio/Base/AudioManager.h>
#include <ChilliSource/Audio/Base/AudioSystem.h>

namespace ChilliSource
{
	namespace Audio
	{
        //-------------------------------------------------------
        /// Factory class for loading audio resources. Concrete
        /// audio systems implement this interface
        ///
        /// @author S Downie
        //-------------------------------------------------------
		class AudioLoader : public Core::ResourceProvider
		{
		public:
            
            //-------------------------------------------------------
            /// Create the platform dependent backend
            ///
            /// @author S Downie
            ///
            /// @param Audio system
            ///
            /// @return New backend instance
            //-------------------------------------------------------
            static AudioLoaderUPtr Create(AudioSystem* in_system);
            //----------------------------------------------------------------------------
			/// Stream audio resource from the given file
            ///
            /// @author S Downie
            ///
            /// @param The storage location to load from
			/// @param Filename
			/// @param [Out] Resource object
            ///
			/// @return Success
			//----------------------------------------------------------------------------
			virtual bool StreamResourceFromFile(Core::StorageLocation in_storageLocation, const std::string& in_filePath, Core::ResourceSPtr& out_resource) = 0;
			//----------------------------------------------------------------------------
			/// Create Audio Listener
			///
			/// @return Audio listener
			//----------------------------------------------------------------------------
			virtual AudioListenerUPtr CreateAudioListener() = 0;
            //-------------------------------------------------------
            /// Virtual destructor
            ///
            /// @author S Downie
            //-------------------------------------------------------
			virtual ~AudioLoader(){}
            
        protected:
            
            //-------------------------------------------------------
            /// Private constructor to force use of factory method
            ///
            /// @author S Downie
            //-------------------------------------------------------
            AudioLoader(){}
		};
	}
}

#endif

