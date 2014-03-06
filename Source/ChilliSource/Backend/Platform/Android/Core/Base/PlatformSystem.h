/*
 * File: Android/PlatformSystem.h
 * Date: 23/03/2011
 * Description: 
 */

/*
 * Author: Ian Copland
 * Version: v 1.0
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#ifndef _MO_FLO_PLATFORM_ANDROID_PLATFORM_SYSTEM_H_
#define _MO_FLO_PLATFORM_ANDROID_PLATFORM_SYSTEM_H_

#include <ChilliSource/Backend/Platform/Android/Core/Base/UDIDManager.h>
#include <ChilliSource/Core/Base/PlatformSystem.h>

namespace ChilliSource
{
	namespace Android
	{
		//=============================================
		/// Platform System
		///
		/// Android platform system
		//=============================================
		class PlatformSystem : public Core::PlatformSystem
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
			//-----------------------------------------------------------------------------------------------------------
			/// Retrieves the screen dimensions. These dimensions are always in the default orientation for the device.
			///
			/// @author S Downie
			///
			/// @return A Vector2 containing the screen size in its x, y components
			//-----------------------------------------------------------------------------------------------------------
			Core::Vector2 GetScreenDimensions() const override;
			//--------------------------------------------------------------
			/// @author S Downie
			///
			/// @return The device model name stringified
			//--------------------------------------------------------------
            std::string GetDeviceModelName() const override;
			//--------------------------------------------------------------
			/// @author S Downie
			///
			/// @return The device model type stringified
			//--------------------------------------------------------------
            std::string GetDeviceModelTypeName() const override;
			//--------------------------------------------------------------
			/// @author S Downie
			///
			/// @return The manufacturer stringified
			//--------------------------------------------------------------
            std::string GetDeviceManufacturerName() const override;
            //--------------------------------------------------------------
			/// @author S Downie
			///
			/// @return String containing the OS version of the device
			//--------------------------------------------------------------
			std::string GetOSVersion() const override;
			//--------------------------------------------------------------
			/// @author S Downie
			///
			/// @return active locale of the device
			//--------------------------------------------------------------
            Core::Locale GetLocale() const override;
			//--------------------------------------------------------------
			/// @author S Downie
			///
			/// @return active language of the device
			//--------------------------------------------------------------
            Core::Locale GetLanguage() const override;
			//-------------------------------------------------
            /// @author S Downie
            ///
			/// @return The density scale factor of the screen
			/// to convert from DIPS to physical pixels
			//-------------------------------------------------
			f32 GetScreenDensity() const override;
			//-------------------------------------------------
			/// @author S Downie
			///
			/// @return The UDID of the device
			//-------------------------------------------------
			std::string GetDeviceID() override;
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
			/// @return The number of cores available
			//--------------------------------------------------------------
			u32 GetNumberOfCPUCores() const override;
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
            PlatformSystem(){}
			//--------------------------------------------
			/// Sets the working directory for this application
			/// to the root of the application directory
            ///
            /// @author S Downie
			//--------------------------------------------
			void SetWorkingDirectoryToResourceFolder();
			//-------------------------------------------------
			/// @author S Downie
			///
			/// @return The physical size of the screen in
			/// inches.
			//-------------------------------------------------
			f32 GetPhysicalScreenSize();

		private:
			
			UDIDManager mUDIDManager;
		};
	}
}

#endif
