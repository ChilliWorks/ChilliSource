//
//  PlatformSystem.h
//  Chilli Source
//
//  Created by Scott Downie on 24/11/2010.
//  Copyright (c) 2014 Tag Games Ltd. All rights reserved.
//

#ifndef _CHILLISOURCE_BACKEND_PLATFORM_WINDOWS_CORE_BASE_PLATFORMSYSTEM_H_
#define _CHILLISOURCE_BACKEND_PLATFORM_WINDOWS_CORE_BASE_PLATFORMSYSTEM_H_

#include <ChilliSource/Backend/Platform/Windows/ForwardDeclarations.h>
#include <ChilliSource/Core/Base/ApplicationEvents.h>
#include <ChilliSource/Core/Base/PlatformSystem.h>

namespace ChilliSource
{
	namespace Windows
	{
		//---------------------------------------------------------
		/// Allows generic access to platform specific code via 
		/// common function calls
		//---------------------------------------------------------
		class PlatformSystem : public Core::PlatformSystem
		{
		public:
			PlatformSystem();
			virtual ~PlatformSystem();

			//--------------------------------------------------
			/// Create the GLFW window.
			///
			/// @author S Downie
			//-------------------------------------------------
			void Init() override;
			//-------------------------------------------------
			/// Adds default systems to the applications system
			/// list.
			///
			/// @author S Downie
			///
			/// @param the system list
			//-------------------------------------------------
			void CreateDefaultSystems(Core::Application* in_application) override;
			//-------------------------------------------------
			/// Deals with anything that needs to be handled after
			/// creating the systems.
			///
			/// @author S Downie
			///
			/// @param the system list
			//-------------------------------------------------
			void PostCreateSystems() override;
			//-------------------------------------------------
			/// Begin the game loop.
			///
			/// @author S Downie
			//-------------------------------------------------
			void Run() override;
			//-------------------------------------------------
            /// @param The maximum frames per second
            /// to clamp to. This should be in multiples
            /// of 15 (15, 30, 60)
            //-------------------------------------------------
			void SetMaxFPS(u32 in_fps) override {}
			//-------------------------------------------------
			/// Starts or stops the platforms 
			/// update loop.
			///
			/// @param Whether to end or begin
			//-------------------------------------------------
			void SetUpdaterActive(bool in_isActive) override;
			//-------------------------------------------------
			/// Stops the update loop causing the 
			/// application to terminate
			//-------------------------------------------------
			void TerminateUpdater() override;
			//-------------------------------------------------
			/// Retrieves the screen dimensions. These dimensions 
			/// are always in the default orientation for the device.
			///
			/// @author S Downie
			///
			/// @return A Vector2 containing the screen size in 
			/// its x + y components
			//-------------------------------------------------
			Core::Vector2 GetScreenDimensions() const override;
			//-------------------------------------------------
			/// @author S Downie
			///
			/// @return The above information stringified
			//------------------------------------------------
			std::string GetDeviceModelName() const override;
			//-------------------------------------------------
			/// @author S Downie
			///
			/// @return The above information stringified
			//-------------------------------------------------
			std::string GetDeviceModelTypeName() const override;
			//-------------------------------------------------
			/// @author S Downie
			///
			/// @return The above information stringified
			//-------------------------------------------------
			std::string GetDeviceManufacturerName() const override;
            //-------------------------------------------------
			/// @author S Downie
			///
			/// @return  String containing the OS version of the device
			//-------------------------------------------------
			std::string GetOSVersion() const override;
			//-------------------------------------------------
			/// @author S Downie
			///
			/// Get the active language locale of the device
			/// @return Locale ID
			//-------------------------------------------------
			Core::Locale GetLocale() const override;
			//-------------------------------------------------
			/// @author S Downie
			///
			/// Get the active language of the device in locale format
			/// @return Locale ID
			//-------------------------------------------------
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
	        /// @return The version of the application
	        //-------------------------------------------------
			std::string GetAppVersion() const override { return ""; }
			//-------------------------------------------------
			/// @author S Downie
			///
			/// @return The number of cores available
			//------------------------------------------------
			u32 GetNumberOfCPUCores() const override;
			//------------------------------------------------
			/// @author S Downie
			///
			/// @return the current time in milliseconds
			//-------------------------------------------------
			u64 GetSystemTimeMS() const override;
			//-------------------------------------------------
			/// @author S Downie
			///
			/// @return The physical size of the screen in
			/// inches.
			//-------------------------------------------------
			f32 GetPhysicalScreenSize() override;
		private:

			//---GLFW Delegates
			//-------------------------------------------------
			/// Triggered when glfw resizes the window
			///
			/// @author S Downie
			///
			/// @param Window width
			/// @param Window Height
			//-------------------------------------------------
			static void OnWindowResized(s32 in_width, s32 in_height);
			//-------------------------------------------------
			/// Triggered when glfw exits the window
			///
			/// @author S Downie
			//-------------------------------------------------
			static void OnWindowClosed();

		private:
			bool m_isRunning;
			bool m_isSuspended;

			u64 m_appStartTime;
			f64 m_appPreviousTime;
		};
	}
}

#endif
