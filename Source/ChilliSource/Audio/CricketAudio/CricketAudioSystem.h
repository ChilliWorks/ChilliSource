//
//  CricketAudioSystem.h
//  Chilli Source
//  Created by Ian Copland on 30/12/2014.
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

#ifndef _CHILLISOURCE_AUDIO_CRICKETAUDIO_CKSYSTEM_H_
#define _CHILLISOURCE_AUDIO_CRICKETAUDIO_CKSYSTEM_H_

#include <ChilliSource/ChilliSource.h>

#include <ChilliSource/Core/System/AppSystem.h>

namespace ChilliSource 
{
	namespace Audio
	{
		//------------------------------------------------------------------------------
		/// The Cricket Audio system. This manages the underlying cricket audio system,
		/// initialising it and destroying when needed. 
		///
        /// The Cricket Audio systems, including this, are not added to Application by
        /// default. If you intend to use Cricket Audio you will need to add CricketAudioSystem,
        /// and CkBankProvider during the Application::AddSystems() life cycle event.
        /// You will also need to add CkAudioPlayer during the State::AddSystems() life
        /// cycle event for any states in which you wish to use it.
        ///
        /// Chilli Source has obtained special permission to include Cricket Audio as
        /// part of the engine. You are free to use Cricket Audio as part of Chilli
        /// Source within your app subject to the Cricket Audio free license
        /// (http://www.crickettechnology.com/free_license) as outline below:
        ///
        /// Cricket Audio free license
        ///
        /// Cricket Audio is available FREE in binary form for iOS, Android, Windows
        /// Phone 8, OS X, Windows, and Linux.
        ///
        /// This free license has a few requirements:
        ///
        /// * Include the following message somewhere in your app, visible to users
        ///   (for example, on a credits screen):
        ///                     Built with Cricket Audio
        ///                     www.crickettechnology.com
        ///
        /// * You may optionally include our logo (available in .svg or .png format).
        ///
        /// * Do not distribute our software to anyone, or post it for download on any
        ///   site; refer them to our website instead. This license does not permit
        ///   you to distribute Cricket Audio in an SDK, library, or other software
        ///   development product. Contact us at info@crickettechnology.com for
        ///   additional licensing options.
        ///
        /// * If you release an app that uses Cricket Audio, you must let Cricket
        ///   Technology know. Cricket Technology will include a link to it on our
        ///   customers page.
        ///
        /// If you do not want to include the credit in your app, or if you do not want
        /// to notify us of your app's release, or if you want the source code, you can
        /// purchase a source code license (http://www.crickettechnology.com/source_license).
		///
		/// @author Ian Copland
		//------------------------------------------------------------------------------
		class CricketAudioSystem final : public Core::AppSystem
		{
		public:
			CS_DECLARE_NAMEDTYPE(CricketAudioSystem);
			//------------------------------------------------------------------------------
			/// Allows querying of whether or not this system implements the interface
			/// described by the given interface Id. Typically this is not called directly
			/// as the templated equivalent IsA<Interface>() is preferred.
			///
			/// @author Ian Copland
			///
			/// @param The interface Id.
			///
			/// @return Whether or not the interface is implemented.
			//------------------------------------------------------------------------------
			bool IsA(CSCore::InterfaceIDType in_interfaceId) const override;
		private:
			friend class CSCore::Application;
			friend class CkSound;
			//------------------------------------------------------------------------------
			/// A factory method for creating new instances of the Cricket Audio System.
			///
			/// @author Ian Copland
			//------------------------------------------------------------------------------
			static CricketAudioSystemUPtr Create();
			//------------------------------------------------------------------------------
			/// Default constructor. Declared private to ensure this can only be created
			/// through Application::CreateSystem<CricketAudioSystem>().
			///
			/// @author Ian Copland
			//------------------------------------------------------------------------------
			CricketAudioSystem() = default;
			//------------------------------------------------------------------------------
			/// Registers a CkSound with the system so that it receives update events.
			///
			/// @author Ian Copland
			///
			/// @param The pointer to the CkSound.
			//------------------------------------------------------------------------------
			void Register(CkSound* in_ckAudio);
			//------------------------------------------------------------------------------
			/// De-registers a CkSound from the system so that it no longer receives update
			/// events.
			///
			/// @author Ian Copland
			///
			/// @param The pointer to the CkSound.
			//------------------------------------------------------------------------------
			void Deregister(CkSound* in_ckAudio);
			//------------------------------------------------------------------------------
			/// Initialises the Cricket Audio system.
			///
			/// @author Ian Copland
			//------------------------------------------------------------------------------
			void OnInit() override;
			//------------------------------------------------------------------------------
			/// Resumes the Cricket Audio system, resuming any audio that was paused during
			/// a suspend.
			///
			/// @author Ian Copland
			//------------------------------------------------------------------------------
			void OnResume() override;
			//------------------------------------------------------------------------------
			/// Updates the Cricket Audio system.
			///
			/// @author Ian Copland
			///
			/// @param The delta time.
			//------------------------------------------------------------------------------
			void OnUpdate(f32 in_deltaTime) override;
			//------------------------------------------------------------------------------
			/// Suspends the cricket audio system. This ensures that all audio is currectly
			/// paused while the app is not active.
			///
			/// @author Ian Copland
			//------------------------------------------------------------------------------
			void OnSuspend() override;
			//------------------------------------------------------------------------------
			/// Shuts down the cricket audio system.
			///
			/// @author Ian Copland
			///------------------------------------------------------------------------------
			void OnDestroy() override;

			std::vector<CkSound*> m_ckAudioList;
		};
	}
}

#endif
