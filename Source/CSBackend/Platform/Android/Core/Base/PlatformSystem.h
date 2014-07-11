//
//  PlatformSystem.h
//  Chilli Source
//  Created by Ian Copland on 24/11/2010.
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

#ifdef CS_TARGETPLATFORM_ANDROID

#ifndef _CSBACKEND_PLATFORM_ANDROID_CORE_BASE_PLATFORMSYSTEM_H_
#define _CSBACKEND_PLATFORM_ANDROID_CORE_BASE_PLATFORMSYSTEM_H_

#include <ChilliSource/ChilliSource.h>
#include <CSBackend/Platform/Android/ForwardDeclarations.h>
#include <ChilliSource/Core/Base/PlatformSystem.h>

namespace CSBackend
{
	namespace Android
	{
		//---------------------------------------------------------------
		/// The Android backend for the platform system. This creates the
		/// Android specfic default systems and sets up the updater.
		///
		/// @author Ian Copland
		//---------------------------------------------------------------
		class PlatformSystem final : public CSCore::PlatformSystem
		{
		public:
			CS_DECLARE_NAMEDTYPE(PlatformSystem);
            //-------------------------------------------------------
			/// Queries whether or not this system implements the
            /// interface with the given Id.
			///
            /// @author Ian Copland
            ///
			/// @param The interface Id.
			/// @param Whether system is of given type.
			//-------------------------------------------------------
			bool IsA(CSCore::InterfaceIDType in_interfaceId) const override;
			//-------------------------------------------------------
			/// Adds default systems to the applications system
			/// list.
            ///
            /// @author S Downie
			///
			/// @param Application instance to add the default
            /// platform systems to.
			//-------------------------------------------------------
			void CreateDefaultSystems(CSCore::Application* in_application) override;
			//-------------------------------------------------------
            /// Lock the frame rate to the given FPS
			///
			/// @author S Downie
            ///
            /// @param The maximum frames per second to clamp to.
			/// This should be in multiples of 15 (15, 30, 60)
			//-------------------------------------------------------
            void SetPreferredFPS(u32 in_fps) override;
            //---------------------------------------------------
            /// NOTE: Not supported on Android
            ///
            /// @author S Downie
            ///
            /// @param Enable/Disable
            //---------------------------------------------------
            void SetVSyncEnabled(bool in_enabled) override {};
            //-------------------------------------------------------
            /// Terminate the app
			///
			/// @author S Downie
            //-------------------------------------------------------
            void Quit() override;
			//-------------------------------------------------------
	        /// @author S Downie
	        ///
	        /// @return The version of the application as found
			/// in the manifest
			//-------------------------------------------------------
	        std::string GetAppVersion() const override;
	        //-------------------------------------------------------
			/// @author S Downie
			///
			/// @return The current time in milliseconds
	        //-------------------------------------------------------
			TimeIntervalMs GetSystemTimeMS() const override;
		private:
            friend CSCore::PlatformSystemUPtr CSCore::PlatformSystem::Create();
            //-------------------------------------------------------
            /// Private constructor to force use of factory method
            ///
            /// @author S Downie
            //-------------------------------------------------------
            PlatformSystem() = default;
		};
	}
}

#endif

#endif
