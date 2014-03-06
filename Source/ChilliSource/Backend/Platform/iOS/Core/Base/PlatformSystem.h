/*
 * File: iOSPlatformSystem.h
 * Date: 24/11/2010 2010 
 * Description: 
 */

/*
 * Author: Scott Downie
 * Version: v 1.0
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#ifndef _MO_FLO_PLATFORM_IOS_PLATFORM_SYSTEM_H_
#define _MO_FLO_PLATFORM_IOS_PLATFORM_SYSTEM_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Backend/Platform/iOS/ForwardDeclarations.h>
#include <ChilliSource/Core/Base/PlatformSystem.h>

#include <functional>

namespace ChilliSource
{
	namespace iOS
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
        
			//-----------------------------------------
			/// Init
			//-----------------------------------------
			void Init();
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
			/// Post Create Systems
			///
			/// Deals with anything that needs to be handled after
			/// creating the systems.
			///
			/// @param the system list
			//-------------------------------------------------
			void PostCreateSystems() override;
            //-----------------------------------------
            /// Set Max FPS
            ///
            /// @param The maximum frames per second
            /// to clamp to. This should be in multiples
            /// of 15 (15, 30, 60)
            //-----------------------------------------
            void SetMaxFPS(u32 inudwFPS) override;
            //-----------------------------------------
			/// Run
			///
            /// Begin the game loop
			//-----------------------------------------
			void Run() override;
			//-----------------------------------------
			/// Set Updater Active
			///
			/// Starts or stops the platforms 
			/// update loop.
			///
			/// @param Whether to end or begin
			//-----------------------------------------
			void SetUpdaterActive(bool inbIsActive) override;
            //-----------------------------------------
            /// Terminate Updater
            ///
            /// Stops the update loop causing
            /// the application to terminate
            //-----------------------------------------
            void TerminateUpdater() override;
			
			//==========================================
			//--- Activity Creation
			//==========================================
			//-----------------------------------------
            /// Can Create Activity With Interface
            ///
            /// @param Interface ID
            /// @return Whether system can be created
            //----------------------------------------
            bool CanCreateActivityWithInterface(Core::InterfaceIDType inInterfaceID) const override;
			//-----------------------------------------
            /// Create Activity With Interface
            ///
			/// Tries to create a platform specific implementation with the given interface
			///
			/// @param InterfaceID to generate
			/// @return A handle to the given activity or nullptr if the platform cannot support it
			//-----------------------------------------
            Core::Activity* CreateActivityWithInterface(Core::InterfaceIDType inInterfaceID) const override;
			
			//==========================================
			//---InformationProvider Creation
			//==========================================
			//-----------------------------------------
            /// Can Create Information Provider With Interface
            ///
			/// @param Interface ID
			/// @return Whether system can be created
			//----------------------------------------
            bool CanCreateInformationProviderWithInterface(Core::InterfaceIDType inInterfaceID) const override;
			
			//-----------------------------------------
            /// Can Create Information Provider With Interface
            ///
			/// Tries to create a platform specific implementation with the given interface
			///
			/// @param InterfaceID to generate
			/// @return A handle to the given system or nullptr if the platform cannot support it
			//-----------------------------------------
            Core::IInformationProvider* CreateInformationProviderWithInterface(Core::InterfaceIDType inInterfaceID) const override;
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
			/// @return String containing the OS version of the device
			//--------------------------------------------------------------
			std::string GetOSVersion() const override;
			//--------------------------------------------------------------
			/// Get Locale
			///
			/// Get the active locale of the device
			/// @return Locale ID
			//--------------------------------------------------------------
            Core::Locale GetLocale() const override;
			//--------------------------------------------------------------
			/// Get Locale
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
            std::string GetDeviceID();
            //-------------------------------------------------
            /// Get App Version
            ///
            /// @return The bundle version as found in the plist
            //-------------------------------------------------
            std::string GetAppVersion() const override;
			//--------------------------------------------------------------
			/// Get Number Of CPU Cores
			///
			/// @return The number of cores available
			//--------------------------------------------------------------
			u32 GetNumberOfCPUCores() const override;
			//--------------------------------------------------------------
			/// Get System Time
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
            PlatformSystem();
            //--------------------------------------------
			/// Create Activities
			///
			/// Creates an instance of the activity
			///
			/// @return Ownership of the activity
			//--------------------------------------------
			Core::Activity* CreateSMSCompositionActivity() const;
			Core::Activity* CreateEmailCompositionActivity() const;
			
            //--------------------------------------------
            /// Create Information Providers
            ///
            /// Creates an instance of the info provider
            ///
            /// @return Ownership of the info provider
            //--------------------------------------------
			Core::IInformationProvider* CreateContactInformationProvider() const;
			
            typedef std::function<Core::Activity*()> ActivityCreationFunction;
			
			//--------------------------------------------
            /// Add Activity Function
            ///
            /// Map the creation function with the
            /// system type
            ///
            /// @param System interface ID
            /// @param Creation delegate
            //-------------------------------------------
            void AddActivityFunc(Core::InterfaceIDType inInterfaceID, ActivityCreationFunction inFunction);
			
            typedef std::function<Core::IInformationProvider*()> InfoProviderCreationFunction;
			
            //--------------------------------------------
            /// Add Info Provider Function
            ///
            /// Map the creation function with the
            /// system type
            ///
            /// @param Info provider interface ID
            /// @param Creation delegate
            //-------------------------------------------
            void AddInfoProviderFunc(Core::InterfaceIDType inInterfaceID, InfoProviderCreationFunction inFunction);
    		//-------------------------------------------------
    		/// Get Physical Screen Size
    		///
    		/// @return The physical size of the screen in
    		/// inches.
    		//-------------------------------------------------
    		f32 GetPhysicalScreenSize();
			
		private:
        
			typedef std::unordered_map<Core::InterfaceIDType, ActivityCreationFunction> MapInterfaceIDToActivityFunc;
			MapInterfaceIDToActivityFunc mmapInterfaceIDToActivityFunc;
			
			typedef std::unordered_map<Core::InterfaceIDType, InfoProviderCreationFunction> MapInterfaceIDToInfoProviderFunc;
			MapInterfaceIDToInfoProviderFunc mmapInterfaceIDToInfoProviderFunc;
            
            f32 mfPhysicalScreenSize;
		};
	}
}

#endif
