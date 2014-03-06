/*
 * File: Android/PlatformSystem.cpp
 * Date: 24/11/2010 2010 
 * Description: 
 */

/*
 * Author: Scott Downie / Ian Copland
 * Version: v 1.0
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#include <ChilliSource/Backend/Platform/Android/Core/Base/PlatformSystem.h>

#include <ChilliSource/Backend/Platform/Android/Core/Base/CoreJavaInterface.h>
#include <ChilliSource/Backend/Platform/Android/Core/JNI/JavaInterfaceManager.h>
#include <ChilliSource/Core/Image/ETC1ImageProvider.h>

namespace ChilliSource 
{
	namespace Android
	{
		//-------------------------------------------------
		//-------------------------------------------------
		void PlatformSystem::CreateDefaultSystems(Core::Application* in_application)
		{
			inaSystems.push_back(Core::ETC1ImageProvider::Create());
		}
		//-------------------------------------------------
		//-------------------------------------------------
		void PlatformSystem::PostCreateSystems()
		{

		}
        //-----------------------------------------
        //-----------------------------------------
        void PlatformSystem::SetMaxFPS(u32 inudwFPS)
        {
        	JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<CoreJavaInterface>()->SetMaxFPS(inudwFPS);
        }
		//-----------------------------------------
        /// Terminate Updater
        ///
        /// Stops the update loop causing
        /// the application to terminate
        //-----------------------------------------
        void PlatformSystem::TerminateUpdater()
        {
        	JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<CoreJavaInterface>()->ForceQuit();
        }
		//---------------------------------------------
		/// Get Screen Dimensions
		//---------------------------------------------
		Core::Vector2 PlatformSystem::GetScreenDimensions() const
		{
			Core::Vector2 Result;
			CoreJavaInterfaceSPtr pCoreJI = JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<CoreJavaInterface>();
			Result.x = pCoreJI->GetScreenWidth();
			Result.y = pCoreJI->GetScreenHeight();

			s32 dwOrientation = pCoreJI->GetOrientation();
#ifdef DEBUG
			if(dwOrientation < 0)
				CS_LOG_ERROR("PlatformSystem::GetScreenDimensions() - Could not get orientation of device!");
#endif
			if(Core::ScreenOrientation::k_landscapeRight == (Core::ScreenOrientation)dwOrientation)
			{
				// Swap round as we want dimensions the other way
				u32 udwSavedX = Result.x;
				Result.x = Result.y;
				Result.y = udwSavedX;
			}

			return Result;
		}
		//--------------------------------------------------------------
		/// Get Device Model Name
		//--------------------------------------------------------------
        std::string PlatformSystem::GetDeviceModelName() const
		{
			return JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<CoreJavaInterface>()->GetDeviceModel();
		}
		//--------------------------------------------------------------
		/// Get Device Model Type Name
		//--------------------------------------------------------------
        std::string PlatformSystem::GetDeviceModelTypeName() const
		{
			return JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<CoreJavaInterface>()->GetDeviceModelType();
		}
		//--------------------------------------------------------------
		/// Get Device Manufacturer Name
		//--------------------------------------------------------------
        std::string PlatformSystem::GetDeviceManufacturerName() const
		{
			return JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<CoreJavaInterface>()->GetDeviceManufacturer();
		}
		//--------------------------------------------------------------
		/// Get OS Version
		//--------------------------------------------------------------
		std::string PlatformSystem::GetOSVersion() const
		{
			return Core::ToString(JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<CoreJavaInterface>()->GetOSVersionCode());
		}
		//--------------------------------------------------------------
		/// Get Locale
		//--------------------------------------------------------------
		Core::Locale PlatformSystem::GetLocale() const
		{
			//get the locale from android
			std::string strLocale = JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<CoreJavaInterface>()->GetDefaultLocaleCode();

			//break this locale into parts(language/country code/extra)
			std::vector<std::string> strLocaleBrokenUp = ChilliSource::Core::StringUtils::Split(strLocale, "_", 0);

			if (strLocaleBrokenUp.size() > 1)
			{
				return Core::Locale(strLocaleBrokenUp[0],strLocaleBrokenUp[1]);
			}
			else if (strLocaleBrokenUp.size() == 1)
			{
				return Core::Locale(strLocaleBrokenUp[0]);
			}
			else
				return Core::kUnknownLocale;
		}
		//--------------------------------------------------------------
		/// Get Language
		//--------------------------------------------------------------
		Core::Locale PlatformSystem::GetLanguage() const
		{
			return GetLocale();
		}
		//-------------------------------------------------
		/// Get Screen Density
		//-------------------------------------------------
		f32 PlatformSystem::GetScreenDensity() const
		{
			return JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<CoreJavaInterface>()->GetScreenDensity();
		}
        //-------------------------------------------------
        /// Get App Version
        ///
        /// @return The version of the application as found
		/// in the manifest
        //-------------------------------------------------
        std::string PlatformSystem::GetAppVersion() const
		{
			return JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<CoreJavaInterface>()->GetApplicationVersionName();
		}
		//-------------------------------------------------
		/// Get Device ID
		//-------------------------------------------------
		std::string PlatformSystem::GetDeviceID()
		{
			return mUDIDManager.GetUDID();
		}
		//--------------------------------------------------------------
		/// Get Number Of CPU Cores
		//--------------------------------------------------------------
		u32 PlatformSystem::GetNumberOfCPUCores() const
		{
			return JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<CoreJavaInterface>()->GetNumberOfCores();
		}
		//--------------------------------------------------------------
		/// Get System Time
		//--------------------------------------------------------------
		TimeIntervalMs PlatformSystem::GetSystemTimeMS() const
		{
			return JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<CoreJavaInterface>()->GetSystemTimeInMilliseconds();
		}
		//-------------------------------------------------
		/// Get Physical Screen Size
		//-------------------------------------------------
		f32 PlatformSystem::GetPhysicalScreenSize()
		{
			return JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<CoreJavaInterface>()->GetPhysicalScreenSize();
		}
		//-----------------------------------------
		/// Destructor
		//-----------------------------------------
		PlatformSystem::~PlatformSystem()
		{
			
		}
	}
}
