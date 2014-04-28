//
//  PlatformSystem.h
//  Chilli Source
//
//  Created by Ian Copland on 24/11/2010.
//  Copyright (c) 2014 Tag Games Ltd. All rights reserved.
//
#ifndef _CHILLISOURCE_BACKEND_PLATFORM_ANDROID_CORE_BASE_PLATFORMSYSTEM_H_
#define _CHILLISOURCE_BACKEND_PLATFORM_ANDROID_CORE_BASE_PLATFORMSYSTEM_H_

#include <ChilliSource/Core/Base/PlatformSystem.h>

namespace ChilliSource
{
	namespace Android
	{
		//---------------------------------------------
		/// Allows generic access to platform
		/// specific code via common function calls
		///
		/// @author I Copland
		//-------------------------------------------
		class PlatformSystem final : public Core::PlatformSystem
		{
		public:

			//-----------------------------------------
			/// Init
			///
			/// @author S Downie
			//-----------------------------------------
			void Init() override;
			//-------------------------------------------------
			/// Adds default systems to the applications system
			/// list.
            ///
            /// @author S Downie
			///
			/// @param Application instance to add the default
            /// platform systems to.
			//-------------------------------------------------
			void CreateDefaultSystems(Core::Application* in_application) override;
			//-------------------------------------------------
			/// Deals with anything that needs to be handled after
			/// creating the systems.
			///
			/// @author S Downie
			//-------------------------------------------------
			void PostCreateSystems() override;
			//-----------------------------------------
            /// Begin the game loop
			///
			/// @author S Downie
			//-----------------------------------------
			void Run() override {}
            //-----------------------------------------
            /// Lock the frame rate to the given FPS
			///
			/// @author S Downie
            ///
            /// @param The maximum frames per second
            /// to clamp to. This should be in multiples
            /// of 15 (15, 30, 60)
            //-----------------------------------------
            void SetMaxFPS(u32 in_fps) override;
			//-----------------------------------------
            /// Stops the update loop causing
            /// the application to terminate
			///
			/// @author S Downie
            //-----------------------------------------
            void TerminateUpdater() override;
			//-----------------------------------------
			/// Starts or stops the platforms 
			/// update loop.
			///
			/// @author S Downie
			///
			/// @param Whether to end or begin
			//-----------------------------------------
			void SetUpdaterActive(bool in_isActive) override {}
	        //-------------------------------------------------
	        /// @author S Downie
	        ///
	        /// @return The version of the application as found
			/// in the manifest
	        //-------------------------------------------------
	        std::string GetAppVersion() const override;
			//--------------------------------------------------------------
			/// @author S Downie
			///
			/// @return The current time in milliseconds
			//--------------------------------------------------------------
			TimeIntervalMs GetSystemTimeMS() const override;
		private:

            friend Core::PlatformSystemUPtr Core::PlatformSystem::Create();
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
