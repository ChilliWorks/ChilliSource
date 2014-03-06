/*
* File: WindowsPlatformSystem.h
* Date: 24/11/2010 2010 
* Description: 
*/

/*
* Author: Scott Downie
* Version: v 1.0
* Copyright ©2010 Tag Games Limited - All rights reserved 
*/

#ifndef _MO_FLO_PLATFORM_WINDOWS_PLATFORM_SYSTEM_H_
#define _MO_FLO_PLATFORM_WINDOWS_PLATFORM_SYSTEM_H_

#include <ChilliSource/Backend/Platform/Windows/ForwardDeclarations.h>
#include <ChilliSource/Core/Base/ApplicationEvents.h>
#include <ChilliSource/Core/Base/PlatformSystem.h>

namespace ChilliSource
{
	namespace Windows
	{
		//---------------------------------------------
		/// Platform Interface
		///
		/// Allows generic access to platform 
		/// specific code via common function calls
		//---------------------------------------------
		class PlatformSystem : public Core::PlatformSystem
		{
		public:
			PlatformSystem();
			virtual ~PlatformSystem();

			//-----------------------------------------
			/// Init
			///
			/// Create the GLFW window
			//-----------------------------------------
			void Init() override;
			//-------------------------------------------------
			/// Create Default Systems
			///
			/// Adds default systems to the applications system
			/// list.
			///
			/// @param the system list
			//-------------------------------------------------
			void CreateDefaultSystems(Core::Application* in_application) override;
			//-------------------------------------------------
			/// Post Create Systems
			///
			/// Deals with anything that needs to be handled after
			/// creating the systems.
			///
			/// @param the system list
			//-------------------------------------------------
			void PostCreateSystems() override;
			//-----------------------------------------
			/// Run
			///
			/// Begin the game loop
			//-----------------------------------------
			void Run() override;
			//-----------------------------------------
            /// Set Max FPS
            ///
            /// @param The maximum frames per second
            /// to clamp to. This should be in multiples
            /// of 15 (15, 30, 60)
            //-----------------------------------------
			void SetMaxFPS(u32 inudwFPS) override {}
			//-----------------------------------------
			/// Set Updater Active
			///
			/// Starts or stops the platforms 
			/// update loop.
			///
			/// @param Whether to end or begin
			//-----------------------------------------
			void SetUpdaterActive(bool inbIsActive) override;
			//--------------------------------------------
			/// Terminate Updater
			///
			/// Stops the update loop causing the 
			/// application to terminate
			//--------------------------------------------
			void TerminateUpdater() override;
			//-----------------------------------------------------------------------------------------------------------
			/// Get Screen Dimensions
			///
			/// Retrieves the screen dimensions. These dimensions are always in the default orientation for the device.
			/// @return A Vector2 containing the screen size in its x + y components
			//-----------------------------------------------------------------------------------------------------------
			Core::Vector2 GetScreenDimensions() const override;
			//--------------------------------------------------------------
			/// Get Device Model Name
			///
			/// @return The above information stringified
			//--------------------------------------------------------------
			std::string GetDeviceModelName() const override;
			//--------------------------------------------------------------
			/// Get Device Model Type Name
			///
			/// @return The above information stringified
			//--------------------------------------------------------------
			std::string GetDeviceModelTypeName() const override;
			//--------------------------------------------------------------
			/// Get Device Manufacturer Name
			///
			/// @return The above information stringified
			//--------------------------------------------------------------
			std::string GetDeviceManufacturerName() const override;
            //--------------------------------------------------------------
			/// Get OS Version
			///
			/// @return  String containing the OS version of the device
			//--------------------------------------------------------------
			std::string GetOSVersion() const override;
			//--------------------------------------------------------------
			/// Get Locale
			///
			/// Get the active language locale of the device
			/// @return Locale ID
			//--------------------------------------------------------------
			Core::Locale GetLocale() const override;
			//--------------------------------------------------------------
			/// Get Langauge
			///
			/// Get the active language of the device in locale format
			/// @return Locale ID
			//--------------------------------------------------------------
			Core::Locale GetLanguage() const override;
			//-------------------------------------------------
			/// Get Screen Density
			///
			/// @return The density scale factor of the screen
			/// to convert from DIPS to physical pixels
			//-------------------------------------------------
			f32 GetScreenDensity() const override;
			//-------------------------------------------------
			/// Get Device ID
			///
			/// @return The UDID of the device
			//-------------------------------------------------
			std::string GetDeviceID() override;
			//-------------------------------------------------
	        /// Get App Version
	        ///
	        /// @return The version of the application
	        //-------------------------------------------------
			std::string GetAppVersion() const override { return ""; }
			//--------------------------------------------------------------
			/// Get Number Of CPU Cores
			///
			/// @return The number of cores available
			//--------------------------------------------------------------
			u32 GetNumberOfCPUCores() const override;
			//--------------------------------------------------------------
			/// Get System Time
			///
			/// @return the current time in milliseconds
			//--------------------------------------------------------------
			u64 GetSystemTimeMS() const override;
			//-------------------------------------------------
			/// Get Physical Screen Size
			///
			/// @return The physical size of the screen in
			/// inches.
			//-------------------------------------------------
			f32 GetPhysicalScreenSize() override;
		private:

			//---GLFW Delegates
			//-------------------------------------------------
			/// On Window Resized (GLFW)
			///
			/// Triggered when glfw resizes the window
			///
			/// @param Window width
			/// @param Window Height
			//-------------------------------------------------
			static void OnWindowResized(s32 indwWidth, s32 indwHeight);
			//-------------------------------------------------
			/// On Window Closed (GLFW)
			///
			/// Triggered when glfw exits the window
			//-------------------------------------------------
			static void OnWindowClosed();

		private:
			bool mbIsRunning;
			bool mbIsSuspended;

			u64 muddwAppStartTime;
			f64 mffAppPreviousTime;
		};
	}
}

#endif
