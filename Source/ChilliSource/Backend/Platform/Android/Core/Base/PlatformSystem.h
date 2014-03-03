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
		/// Delegates
		//=============================================
		typedef std::function<Core::Activity*()> ActivityCreationFunction;
		typedef std::function<Core::System*(std::vector<Core::SystemSPtr>&)> SystemCreationFunction;
		typedef std::function<Core::IInformationProvider*()> InfoProviderCreationFunction;
		//=============================================
		/// Platform System
		///
		/// Android platform system
		//=============================================
		class CPlatformSystem : public Core::PlatformSystem
		{
		public:
			CPlatformSystem();
			virtual ~CPlatformSystem();
			
			//-----------------------------------------
			/// Init
			//-----------------------------------------
			void Init();
			//-------------------------------------------------
			/// Create Default Systems
			///
			/// Adds default systems to the applications system
			/// list.
			///
			/// @param the system list
			//-------------------------------------------------
			void CreateDefaultSystems(std::vector<Core::SystemSPtr> & inaSystems);
			//-------------------------------------------------
			/// Post Create Systems
			///
			/// Deals with anything that needs to be handled after
			/// creating the systems.
			///
			/// @param the system list
			//-------------------------------------------------
			void PostCreateSystems();
			//-----------------------------------------
			/// Run
			///
            /// Begin the game loop
			//-----------------------------------------
			void Run(){}
            //-----------------------------------------
            /// Set Max FPS
            ///
            /// @param The maximum frames per second
            /// to clamp to. This should be in multiples
            /// of 15 (15, 30, 60)
            //-----------------------------------------
            void SetMaxFPS(u32 inudwFPS);
			//-----------------------------------------
            /// Terminate Updater
            ///
            /// Stops the update loop causing
            /// the application to terminate
            //-----------------------------------------
            void TerminateUpdater();
			//-----------------------------------------
			/// Set Updater Active
			///
			/// Starts or stops the platforms 
			/// update loop.
			///
			/// @param Whether to end or begin
			//-----------------------------------------
			void SetUpdaterActive(bool inbIsActive){}
			//-----------------------------------------
            /// Can Create System With Interface
            ///
            /// @param Interface ID
            /// @return Whether system can be created
            //----------------------------------------
			bool CanCreateSystemWithInterface(Core::InterfaceIDType inInterfaceID) const;
			//-----------------------------------------
            /// Create and Add System With Interface
            ///
			/// Tries to create a platform specific implementation with the given interface
			///
			/// @param InterfaceID to generate
			/// @param Vector of existing systems. The return value is added to this vector if not NULL.
			/// @return A handle to the given system or NULL if the platform cannot support it
			//-----------------------------------------
			Core::System* CreateAndAddSystemWithInterface(Core::InterfaceIDType inInterfaceID, std::vector<Core::SystemSPtr> & inaExistingSystems) const;
			//==========================================
			//--- Activity Creation
			//==========================================
			//-----------------------------------------
            /// Can Create Activity With Interface
            ///
            /// @param Interface ID
            /// @return Whether system can be created
            //----------------------------------------
			virtual bool CanCreateActivityWithInterface(Core::InterfaceIDType inInterfaceID) const;
			//-----------------------------------------
            /// Create Activity With Interface
            ///
			/// Tries to create a platform specific implementation with the given interface
			///
			/// @param InterfaceID to generate
			/// @return A handle to the given activity or NULL if the platform cannot support it
			//-----------------------------------------
			virtual Core::Activity* CreateActivityWithInterface(Core::InterfaceIDType inInterfaceID) const;
			//==========================================
			//---InformationProvider Creation
			//==========================================
			//-----------------------------------------
            /// Can Create Information Provider With Interface
            ///
			/// @param Interface ID
			/// @return Whether system can be created
			//----------------------------------------
			virtual bool CanCreateInformationProviderWithInterface(Core::InterfaceIDType inInterfaceID) const;
			//-----------------------------------------
            /// Can Create Information Provider With Interface
            ///
			/// Tries to create a platform specific implementation with the given interface
			///
			/// @param InterfaceID to generate
			/// @return A handle to the given system or NULL if the platform cannot support it
			//-----------------------------------------
			virtual Core::IInformationProvider* CreateInformationProviderWithInterface(Core::InterfaceIDType inInterfaceID) const;
			//-----------------------------------------------------------------------------------------------------------
			/// Get Screen Dimensions
			///
			/// Retrieves the screen dimensions. These dimensions are always in the default orientation for the device.
			/// @return A Vector2 containing the screen size in its x + y components
			//-----------------------------------------------------------------------------------------------------------
			Core::Vector2 GetScreenDimensions() const;
			//--------------------------------------------------------------
			/// Get Device Model Name
			///
			/// @return The above information stringified
			//--------------------------------------------------------------
            std::string GetDeviceModelName() const;
			//--------------------------------------------------------------
			/// Get Device Model Type Name
			///
			/// @return The above information stringified
			//--------------------------------------------------------------
            std::string GetDeviceModelTypeName() const;
			//--------------------------------------------------------------
			/// Get Device Manufacturer Name
			///
			/// @return The above information stringified
			//--------------------------------------------------------------
            std::string GetDeviceManufacturerName() const;
            //--------------------------------------------------------------
			/// Get OS Version
			///
			/// @return  String containing the OS version of the device
			//--------------------------------------------------------------
			std::string GetOSVersion() const;
			//--------------------------------------------------------------
			/// Get Locale
			///
			/// Get the active locale of the device
			/// @return Locale ID
			//--------------------------------------------------------------
            Core::Locale GetLocale() const;
			//--------------------------------------------------------------
			/// Get Langauge
			///
			/// Get the active language of the device in locale format
			/// @return Locale ID
			//--------------------------------------------------------------
            Core::Locale GetLanguage() const;
			//--------------------------------------------
			/// Create Default Image Loader
			///
			/// Creates a resource provider capable of 
			/// loading images for the target platform
			///
			/// @return A pointer to the ResourceProvider
			//--------------------------------------------
			Core::ResourceProvider* CreateDefaultImageLoader(std::vector<Core::SystemSPtr>& inSystems) const;
			//-------------------------------------------------
			/// Get Screen Density
			///
			/// @return The density scale factor of the screen
			/// to convert from DIPS to physical pixels
			//-------------------------------------------------
			f32 GetScreenDensity() const;
			//-------------------------------------------------
			/// Get Device ID
			///
			/// @return The UDID of the device
			//-------------------------------------------------
			std::string GetDeviceID();
	        //-------------------------------------------------
	        /// Get App Version
	        ///
	        /// @return The version of the application as found
			/// in the manifest
	        //-------------------------------------------------
	        std::string GetAppVersion() const;
			//--------------------------------------------------------------
			/// Get Number Of CPU Cores
			///
			/// @return The number of cores available
			//--------------------------------------------------------------
			u32 GetNumberOfCPUCores() const;
			//--------------------------------------------------------------
			/// Get System Time
			///
			/// @return The current time in milliseconds
			//--------------------------------------------------------------
			TimeIntervalMs GetSystemTimeMS() const;
            //--------------------------------------------------------------------------------------------------
            /// Make Toast
            ///
            /// Display a toast notification with the given text
            ///
            /// @param Text
            //--------------------------------------------------------------------------------------------------
            void MakeToast(const Core::UTF8String& instrText) const;
            //--------------------------------------------------------------------------------------------------
            /// Show System Confirm Dialog
            ///
            /// Display a system confirmation dialog with the given ID and delegate
            ///
            /// @param ID
            /// @param Title text
            /// @param Message text
            /// @param Confirm text
            /// @param Cancel text
            //--------------------------------------------------------------------------------------------------
    		void ShowSystemConfirmDialog(u32 inudwID, const Core::UTF8String& instrTitle, const Core::UTF8String& instrMessage, const Core::UTF8String& instrConfirm, const Core::UTF8String& instrCancel) const;
            //--------------------------------------------------------------------------------------------------
            /// Show System Dialog
            ///
            /// Display a system confirmation dialog with the given ID and delegate
            ///
            /// @param ID
            /// @param Title text
            /// @param Message text
            /// @param Confirm text
            //--------------------------------------------------------------------------------------------------
    		void ShowSystemDialog(u32 inudwID, const Core::UTF8String& instrTitle, const Core::UTF8String& instrMessage, const Core::UTF8String& instrConfirm) const;
		private:
			//--------------------------------------------
			/// Create Http Connection System
			///
			/// Creates an http connection system for this
			///	platform
			///
			/// @return A pointer to the system
			//--------------------------------------------
			Core::System* CreateHttpConnectionSystem(std::vector<Core::SystemSPtr>& inSystems) const;
			//--------------------------------------------
			/// Create Activities
			///
			/// Creates an instance of the activity
			///
			/// @return Ownership of the activity
			//--------------------------------------------
			Core::Activity* CreateSMSCompositionActivity() const;
			Core::Activity* CreateWebViewActivity() const;
			Core::Activity* CreateEmailCompositionActivity() const;
			Core::Activity* CreateDefaultVideoPlayerActivity() const;
			//--------------------------------------------
			/// Create Information Providers
			///
			/// Creates an instance of the info provider
			///
			/// @return Ownership of the info provider
			//--------------------------------------------
			Core::IInformationProvider* CreateContactInformationProvider() const;
			//--------------------------------------------
			/// Add System Function
			///
			/// Map the creation function with the
			/// system type
			///
			/// @param System interface ID
			/// @param Creation delegate
			//-------------------------------------------
			void AddSystemFunc(Core::InterfaceIDType inInterfaceID, SystemCreationFunction inFunction);
			//-------------------------------------------
			/// Find System Implementing
			///
			/// Identify if the system already exists
			/// to prevent creation of duplicate
			/// systems
			///
			/// @param Interface ID
			/// @param Exisiting systems
			/// @return Pointer to system
			//-------------------------------------------
			Core::System* FindSystemImplementing(Core::InterfaceIDType inInterfaceID, const std::vector<Core::SystemSPtr>& inSystems) const;
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
			//--------------------------------------------
			/// Set Working Directory To Resource Folder
			///
			/// Sets the working directory for this application
			/// to the root of the application directory
			//--------------------------------------------
			void SetWorkingDirectoryToResourceFolder();
			//-------------------------------------------------
			/// Get Physical Screen Size
			///
			/// @return The physical size of the screen in
			/// inches.
			//-------------------------------------------------
			f32 GetPhysicalScreenSize();

		private:
			
			CUDIDManager mUDIDManager;

			typedef std::unordered_map<Core::InterfaceIDType, SystemCreationFunction> MapInterfaceIDToSystemFunc;
			MapInterfaceIDToSystemFunc mmapInterfaceIDToSystemFunc;

			typedef std::unordered_map<Core::InterfaceIDType, ActivityCreationFunction> MapInterfaceIDToActivityFunc;
			MapInterfaceIDToActivityFunc mmapInterfaceIDToActivityFunc;

			typedef std::unordered_map<Core::InterfaceIDType, InfoProviderCreationFunction> MapInterfaceIDToInfoProviderFunc;
			MapInterfaceIDToInfoProviderFunc mmapInterfaceIDToInfoProviderFunc;
		};
	}
}

#endif
