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
		struct GLFWwindow;

		//---------------------------------------------
		/// Allows generic access to platform 
		/// specific code via common function calls
		///
		/// @author S Downie
		//---------------------------------------------
		class PlatformSystem final : public Core::PlatformSystem
		{
		public:
			//--------------------------------------------------
			/// Destructor
			///
			/// @author S Downie
			//--------------------------------------------------
			~PlatformSystem();
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
			/// @return The density scale factor of the screen
			/// to convert from DIPS to physical pixels
			//-------------------------------------------------
			f32 GetScreenDensity() const override;
			//-------------------------------------------------
	        /// @author S Downie
	        ///
	        /// @return The version of the application
	        //-------------------------------------------------
			std::string GetAppVersion() const override { return ""; }
			//------------------------------------------------
			/// @author S Downie
			///
			/// @return the current time in milliseconds
			//-------------------------------------------------
			u64 GetSystemTimeMS() const override;

		private:
			friend Core::PlatformSystemUPtr Core::PlatformSystem::Create();
			//--------------------------------------------------
			/// Private constructor to enforce use of create
			/// method.
			///
			/// @author S Downie
			//-------------------------------------------------
			PlatformSystem();

			//---GLFW Delegates
			//-------------------------------------------------
			/// Triggered when glfw resizes the window
			///
			/// @author S Downie
			///
			/// @param Window width
			/// @param Window Height
			/// @param Window that resized
			//-------------------------------------------------
			static void OnWindowResized(GLFWwindow* in_window, s32 in_width, s32 in_height);
			//-------------------------------------------------
			/// Triggered when glfw exits the window
			///
			/// @author S Downie
			///
			/// @param Window that closed
			//-------------------------------------------------
			static void OnWindowClosed(GLFWwindow* in_window);
			//-------------------------------------------------
			/// Triggered when glfw window gains or loses
			/// focus
			///
			/// @author S Downie
			///
			/// @param Window
			/// @param Int representing focused or not
			//-------------------------------------------------
			static void OnWindowFocusChanged(GLFWwindow* in_window, s32 in_isFocused);
				
		private:
			bool m_isRunning;
			bool m_isSuspended;

			u64 m_appStartTime;
			f64 m_appPreviousTime;
		};
	}
}

#endif
