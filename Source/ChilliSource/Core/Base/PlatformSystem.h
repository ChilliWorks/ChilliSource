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
            static PlatformSystemUPtr Create();
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
            //-------------------------------------------------
            /// Adds default systems to the applications system
			/// list.
            ///
            /// @author S Downie
			///
			/// @param Application instance to add the default
            /// platform systems to.
            //-------------------------------------------------
            virtual void CreateDefaultSystems(Application* in_application) = 0;
            //-------------------------------------------------
            /// Post Create Systems
            ///
            /// Deals with anything that needs to be handled after
            /// creating the systems.
            ///
            /// @param the system list
            //-------------------------------------------------
            virtual void PostCreateSystems() = 0;
            //-----------------------------------------------------------------------------------------------------------
            /// Get Screen Dimensions
            ///
            /// Retrieves the screen dimensions. These dimensions are always in the default orientation for the device.
            /// @return A Vector2Old containing the screen size in its x + y components
            //-----------------------------------------------------------------------------------------------------------
            virtual Core::Vector2Old GetScreenDimensions() const = 0;
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
            
        protected:
            
            //-----------------------------------------
            /// Private constructor to force use of
            /// factory method
            ///
            /// @author S Downie
            //-----------------------------------------
            PlatformSystem() = default;
        };
    }
}

#endif
