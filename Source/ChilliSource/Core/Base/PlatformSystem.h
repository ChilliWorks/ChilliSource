/*
 *  PlatformSystem.h
 *  moFlo
 *
 *  Created by Scott Downie on 24/09/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#ifndef _MOFLOW_CORE_PLATFORMSYSTEM_H_
#define _MOFLOW_CORE_PLATFORMSYSTEM_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Localisation/Locales.h>
#include <ChilliSource/Core/Base/Application.h>

namespace ChilliSource
{
    namespace Core
    {
        //---------------------------------------------
        /// Platform Interface
        ///
        /// Allows generic access to platform
        /// specific code via common function calls
        //---------------------------------------------
        class PlatformSystem
        {
        public:
            //----------------------------------------- 
            /// Creates a new platform specfic instance
            /// of the platform system.
            ///
            /// @author Ian Copland
            /// @return The new instance.
            //-----------------------------------------
            static PlatformSystem* Create();
            //-----------------------------------------
            /// Destructor
            //-----------------------------------------
            virtual ~PlatformSystem(){}
            //-----------------------------------------
            /// Init
            ///
            /// Initialise platform specific API's
            //-----------------------------------------
            virtual void Init() = 0;
            //-----------------------------------------
            /// Run
            ///
            /// Begin the game loop
            //-----------------------------------------
            virtual void Run() = 0;
            //-----------------------------------------
            /// Set Max FPS
            ///
            /// @param The maximum frames per second
            /// to clamp to. This should be in multiples
            /// of 15 (15, 30, 60)
            //-----------------------------------------
            virtual void SetMaxFPS(u32 inudwFPS) = 0;
            //-----------------------------------------
            /// Set Updater Active
            ///
            /// Starts or stops the platforms
            /// update loop.
            ///
            /// @param Whether to end or begin
            //-----------------------------------------
            virtual void SetUpdaterActive(bool inbIsActive) = 0;
            //-----------------------------------------
            /// Terminate Updater
            ///
            /// Stops the update loop causing
            /// the application to terminate
            //-----------------------------------------
            virtual void TerminateUpdater() = 0;

            //=========================================
            //--- System Creation
            //=========================================
            //-------------------------------------------------
            /// Add Default Systems
            ///
            /// Adds default systems to the applications system
            /// list.
            ///
            /// @param the system list
            //-------------------------------------------------
            virtual void CreateDefaultSystems(std::vector<Core::SystemSPtr> & inaSystems) = 0;
            //-------------------------------------------------
            /// Post Create Systems
            ///
            /// Deals with anything that needs to be handled after
            /// creating the systems.
            ///
            /// @param the system list
            //-------------------------------------------------
            virtual void PostCreateSystems() = 0;
            //-----------------------------------------
            /// Can Create System With Interface
            ///
            /// @param Interface ID
            /// @return Whether system can be created
            //----------------------------------------
            virtual bool CanCreateSystemWithInterface(Core::InterfaceIDType inInterfaceID) const = 0;
            //-----------------------------------------
            /// Can Create System With Interface
            ///
            /// @param Interface ID
            /// @return Whether system can be created
            //----------------------------------------
            template <typename T> bool CanCreateSystemWithInterface() const
            {
                return CanCreateSystemWithInterface(T::InterfaceID);
            }
            //-----------------------------------------
            /// Create and Add System With Interface
            ///
            /// Tries to create a platform specific implementation with the given interface
            ///
            /// @param InterfaceID to generate
            /// @param Vector of existing systems. The return value is added to this vector if not nullptr.
            /// @return A handle to the given system or nullptr if the platform cannot support it
            //-----------------------------------------
            virtual Core::System* CreateAndAddSystemWithInterface(Core::InterfaceIDType inInterfaceID, std::vector<Core::SystemSPtr> & inaExistingSystems) const = 0;
            //-----------------------------------------
            /// Create and Add System With Interface
            ///
            /// Convenience template method of the above returning the needed interface type.
            ///
            /// @param InterfaceID to generate
            /// @param Vector of existing systems
            /// @return A handle to the given system or nullptr if the platform cannot support it
            //-----------------------------------------
            template <typename T> T* CreateAndAddSystemWithInterface(std::vector<Core::SystemSPtr> & inaExistingSystems) const
            {
                return static_cast<T*> (CreateAndAddSystemWithInterface(T::InterfaceID, inaExistingSystems));
            }
            //=========================================
            //--- Activity Creation
            //=========================================
            //-----------------------------------------
            /// Can Create Activity With Interface
            ///
            /// @param Interface ID
            /// @return Whether activity can be created
            //----------------------------------------
            virtual bool CanCreateActivityWithInterface(Core::InterfaceIDType inInterfaceID) const = 0;
            //-----------------------------------------
            /// Can Create Activity With Interface
            ///
            /// @param Interface ID
            /// @return Whether activity can be created
            //----------------------------------------
            template <typename T> bool CanCreateActivityWithInterface() const
            {
                return CanCreateActivityWithInterface(T::InterfaceID);
            }
            //-----------------------------------------
            /// Tries to create a platform specific implementation with the given interface
            ///
            /// @param InterfaceID to generate
            /// @return A handle to the given activity or nullptr if the platform cannot support it
            //-----------------------------------------
            virtual Activity* CreateActivityWithInterface(Core::InterfaceIDType inInterfaceID) const = 0;
            //-----------------------------------------
            /// Create Activity With Interface
            ///
            /// Convenience template method of the above returning the needed interface type.
            ///
            /// @param InterfaceID to generate
            /// @return A handle to the given activity or nullptr if the platform cannot support it
            //-----------------------------------------
            template <typename T> T* CreateActivityWithInterface() const
            {
                return static_cast<T*> (CreateActivityWithInterface(T::InterfaceID));
                
            }

            //=========================================
            //--- InformationProvider Creation
            //=========================================
            //-----------------------------------------
            /// Can Create Information Provider With Interface
            ///
            /// @param Interface ID
            /// @return Whether system can be created
            //----------------------------------------
            virtual bool CanCreateInformationProviderWithInterface(Core::InterfaceIDType inInterfaceID) const = 0;
            //-----------------------------------------
            /// Can Create Information Provider With Interface
            ///
            /// @param Interface ID
            /// @return Whether system can be created
            //----------------------------------------
            template <typename T> bool CanCreateInformationProviderWithInterface() const
            {
                return CanCreateInformationProviderWithInterface(T::InterfaceID);
            }
            //-----------------------------------------
            /// Create and Add System With Interface
            ///
            /// Tries to create a platform specific implementation with the given interface
            ///
            /// @param InterfaceID to generate
            /// @return A handle to the given system or nullptr if the platform cannot support it
            //-----------------------------------------
            virtual IInformationProvider* CreateInformationProviderWithInterface(Core::InterfaceIDType inInterfaceID) const = 0;
            //-----------------------------------------
            /// Create and Add System With Interface
            ///
            /// Convenience template method of the above returning the needed interface type.
            ///
            /// @param InterfaceID to generate
            /// @param Vector of existing systems
            /// @return A handle to the given system or nullptr if the platform cannot support it
            //-----------------------------------------
            template <typename T> T* CreateInformationProviderWithInterface() const
            {
                return static_cast<T*> (CreateInformationProviderWithInterface(T::InterfaceID));
            }
                
            //-----------------------------------------------------------------------------------------------------------
            /// Get Screen Dimensions
            ///
            /// Retrieves the screen dimensions. These dimensions are always in the default orientation for the device.
            /// @return A Vector2 containing the screen size in its x + y components
            //-----------------------------------------------------------------------------------------------------------
            virtual Core::Vector2 GetScreenDimensions() const = 0;
            //--------------------------------------------------------------
            /// Get Device Model Name
            ///
            /// @return The above information stringified
            //--------------------------------------------------------------
            virtual std::string GetDeviceModelName() const = 0;
            //--------------------------------------------------------------
            /// Get Device Model Type Name
            ///
            /// @return The above information stringified
            //--------------------------------------------------------------
            virtual std::string GetDeviceModelTypeName() const = 0;
            //--------------------------------------------------------------
            /// Get Device Manufacturer Name
            ///
            /// @return The above information stringified
            //--------------------------------------------------------------
            virtual std::string GetDeviceManufacturerName() const = 0;
            //--------------------------------------------------------------
            /// Get Number Of CPU Cores
            ///
            /// @return The number of cores available
            //--------------------------------------------------------------
            virtual u32 GetNumberOfCPUCores() const = 0;
            //--------------------------------------------------------------
            /// Get System Time
            ///
            /// @return the current time in milliseconds
            //--------------------------------------------------------------
            virtual TimeIntervalMs GetSystemTimeMS() const = 0;
            //--------------------------------------------------------------
            /// Get OS Version
            ///
            /// @return  String containing the OS version of the device
            //--------------------------------------------------------------
            virtual std::string GetOSVersion() const = 0;
            //--------------------------------------------------------------
            /// Get Locale
            ///
            /// Get the active language locale of the device
            /// @return Locale ID
            //--------------------------------------------------------------
            virtual Core::Locale GetLocale() const = 0;
            //--------------------------------------------------------------
            /// Get Language
            ///
            /// Get the active locale of the device in locale format
            /// @return Locale ID
            //--------------------------------------------------------------
            virtual Core::Locale GetLanguage() const = 0;
            //-------------------------------------------------
            /// Get Screen Density
            ///
            /// @return The density scale factor of the screen
            /// to convert from DIPS to physical pixels
            //-------------------------------------------------
            virtual f32 GetScreenDensity() const = 0;
            //-------------------------------------------------
            /// Get Device ID
            ///
            /// @return The UDID of the device
            //-------------------------------------------------
            virtual std::string GetDeviceID() = 0;
            //-------------------------------------------------
            /// Get App Version
            ///
            /// @return The version of the application
            //-------------------------------------------------
            virtual std::string GetAppVersion() const = 0;
            //-------------------------------------------------
            /// Get Physical Screen Size
            ///
            /// @return The physical size of the screen in
            /// inches.
            //-------------------------------------------------
            virtual f32 GetPhysicalScreenSize() = 0;
            //-------------------------------------------------
            /// Make Toast
            ///
            /// Display a toast notification with the given text
            ///
            /// @param Text
            //-------------------------------------------------
            virtual void MakeToast(const UTF8String& instrText) const = 0;
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
            virtual void ShowSystemConfirmDialog(u32 inudwID, const UTF8String& instrTitle, const UTF8String& instrMessage, const UTF8String& instrConfirm, const UTF8String& instrCancel) const = 0;
            //--------------------------------------------------------------------------------------------------
            /// Show System Dialog
            ///
            /// Display a system dialog with the given ID and delegate
            ///
            /// @param ID
            /// @param Title text
            /// @param Message text
            /// @param Confirm text
            //--------------------------------------------------------------------------------------------------
            virtual void ShowSystemDialog(u32 inudwID, const UTF8String& instrTitle, const UTF8String& instrMessage, const UTF8String& instrConfirm) const = 0;
        };
    }
}

#endif
