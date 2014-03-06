/*
 * File: iOSPlatformSystem.cpp
 * Date: 24/11/2010 2010 
 * Description: 
 */

/*
 * Author: Scott Downie
 * Version: v 1.0
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#include <ChilliSource/Backend/Platform/iOS/Core/Base/PlatformSystem.h>

#include <ChilliSource/Backend/Platform/iOS/Core/String/NSString+MD5Addition.h>
#include <ChilliSource/Backend/Platform/iOS/Core/Base/UIDevice+IdentifierAddition.h>
#include <ChilliSource/Backend/Platform/iOS/Core/Base/NativeSystem.h>
#include <ChilliSource/Core/String/StringUtils.h>

#include <UIKit/UIKit.h>
#include <sys/types.h>
#include <sys/sysctl.h>
#include <AdSupport/ASIdentifierManager.h>

namespace ChilliSource 
{
	namespace iOS
	{   
		//-----------------------------------------
		//-----------------------------------------
		PlatformSystem::PlatformSystem()
        : m_physicalScreenSize(-1.0f)
		{
		}
		//-----------------------------------------
		//-----------------------------------------
		void PlatformSystem::Init()
		{

		}
		//-------------------------------------------------
		//-------------------------------------------------
		void PlatformSystem::CreateDefaultSystems(Core::Application* in_application)
		{

		}
		//-------------------------------------------------
		//-------------------------------------------------
		void PlatformSystem::PostCreateSystems()
		{

		}
        //-----------------------------------------
        //-----------------------------------------
        void PlatformSystem::Run()
        {
            iOSInit();
        }
        //-----------------------------------------
        //-----------------------------------------
        void PlatformSystem::SetMaxFPS(u32 in_fps)
        {
            iOSSetMaxFPS(in_fps);
        }
		//-----------------------------------------
		//-----------------------------------------
		void PlatformSystem::SetUpdaterActive(bool in_isActive)
		{
			iOSSetUpdaterActive(in_isActive);
		}
        //-----------------------------------------
        //-----------------------------------------
        void PlatformSystem::TerminateUpdater() 
        {
            iOSInvalidateUpdater();
        }
		//------------------------------------------
		//------------------------------------------
		Core::Vector2 PlatformSystem::GetScreenDimensions() const
		{
			Core::Vector2 result;
			CGSize size = [[UIScreen mainScreen] bounds].size;
            
            f32 scale = 1.0f;
            
            bool isiOS4_0 = ([[[UIDevice currentDevice] systemVersion] floatValue] >= 4.0f);
            
            if(isiOS4_0)
            {
                scale = [UIScreen mainScreen].scale;
            }
            
			result.x = size.width * scale;
			result.y = size.height * scale;
            
			return result;
		}
		//----------------------------------------------
		//----------------------------------------------
		std::string PlatformSystem::GetDeviceModelName() const
		{
			NSString * type = [[UIDevice currentDevice] model];

			return (ChilliSource::Core::StringUtils::NSStringToString(type));
		}
		//----------------------------------------------
		//----------------------------------------------
		std::string PlatformSystem::GetDeviceModelTypeName() const
		{
			size_t size = 0;
			sysctlbyname("hw.machine", nullptr, &size, nullptr, 0);
			char* machine = (char*)malloc(size);
			sysctlbyname("hw.machine", machine, &size, nullptr, 0);
			NSString *platform = [NSString stringWithCString:machine encoding:NSASCIIStringEncoding];
			free(machine);

			std::string output;
			std::string modelType = ChilliSource::Core::StringUtils::NSStringToString(platform);
			bool record = false;
			for(std::string::const_iterator it = modelType.begin(); it != modelType.end(); ++it)
			{
				if(isdigit(*it))
				{
					record = true;
				}
				
				if(record)
				{
					output += (*it);
				}
			}
			return output;
		}
		//---------------------------------------------
		//---------------------------------------------
		std::string PlatformSystem::GetDeviceManufacturerName() const
		{
			return std::string("Apple");
		}
        //---------------------------------------------
        //---------------------------------------------
        std::string PlatformSystem::GetOSVersion() const
        {
            NSString* version = [[UIDevice currentDevice] systemVersion];
			return ChilliSource::Core::StringUtils::NSStringToString(version);
        }
        //---------------------------------------------
		//---------------------------------------------
		Core::Locale PlatformSystem::GetLocale() const
		{
			NSLocale *locale = [NSLocale currentLocale];
			NSString *countryCodeObjc = [locale objectForKey:NSLocaleCountryCode];
			std::string countryCode = [countryCodeObjc UTF8String];
			NSString *languageCodeObjc = [locale objectForKey:NSLocaleLanguageCode];
			std::string languageCode = [languageCodeObjc UTF8String];

			//Just default to english
			return ChilliSource::Core::Locale(languageCode, countryCode);
		}
        //---------------------------------------------
		//---------------------------------------------
		Core::Locale PlatformSystem::GetLanguage() const
		{
			NSUserDefaults* userDefaults = [NSUserDefaults standardUserDefaults];
			NSArray* supportedLanguages = [userDefaults objectForKey:@"AppleLanguages"];
			NSString* userLocale = [supportedLanguages objectAtIndex:0];
			std::string localeCode = [userLocale UTF8String];

			//break this locale into parts(language/country code/extra)
			std::vector<std::string> localeBrokenUp = ChilliSource::Core::StringUtils::Split(localeCode, "-", 0);

			if (localeBrokenUp.size() > 1)
			{
				return Core::Locale(localeBrokenUp[0],localeBrokenUp[1]);
			}
			else if (localeBrokenUp.size() == 1)
			{
				return Core::Locale(localeBrokenUp[0]);
			}
			else
				return Core::kUnknownLocale;
		}
        //----------------------------------------------
        //----------------------------------------------
        f32 PlatformSystem::GetScreenDensity() const
        {
            if ([[[UIDevice currentDevice] systemVersion] floatValue] >= 4.0f) 
            {
				CGFloat scale = [UIScreen mainScreen].scale;
                
                return scale;
			}
            
            return 1.0f;
        }
        //-----------------------------------------------
        //-----------------------------------------------
        std::string PlatformSystem::GetDeviceID()
        {
            if ([[[UIDevice currentDevice] systemVersion] floatValue] >= 6.0f)
            {
                NSUUID* uid = nil;
                
                if([ASIdentifierManager sharedManager].advertisingTrackingEnabled)
                {
                    uid = [ASIdentifierManager sharedManager].advertisingIdentifier;
                }
                else
                {
                    uid = [UIDevice currentDevice].identifierForVendor;
                }
                
                return ChilliSource::Core::StringUtils::NSStringToString([uid UUIDString]);
            }
            else
            {
                NSString* udid = [[UIDevice currentDevice] uniqueGlobalDeviceIdentifier];
                return ChilliSource::Core::StringUtils::NSStringToString(udid);
            }
        }
        //-------------------------------------------------
        //-------------------------------------------------
        std::string PlatformSystem::GetAppVersion() const
        {
            NSString* version = [[NSBundle mainBundle] objectForInfoDictionaryKey:(NSString *)kCFBundleVersionKey];
            return ChilliSource::Core::StringUtils::NSStringToString(version);
        }
		//-------------------------------------------------
		//-------------------------------------------------
		u32 PlatformSystem::GetNumberOfCPUCores() const
		{
			u32 numCores = 1;
			size_t size = sizeof(numCores);
			
			if(sysctlbyname("hw.ncpu", &numCores, &size, nullptr, 0))
			{
				return 1;
			}
			else
			{
				return numCores;
			}
		}
		//---------------------------------------------------
		//---------------------------------------------------
		TimeIntervalMs PlatformSystem::GetSystemTimeMS() const
		{
			return GetSystemTimeInNanoSeconds() / 1000000;
		}
		//---------------------------------------------------
		//---------------------------------------------------
		f32 PlatformSystem::GetPhysicalScreenSize()
		{
            if (m_physicalScreenSize < 0.0f)
            {
                size_t size = 0;
                sysctlbyname("hw.machine", nullptr, &size, nullptr, 0);
                char* machine = (char*)malloc(size);
                sysctlbyname("hw.machine", machine, &size, nullptr, 0);
                NSString *platform = [NSString stringWithCString:machine encoding:NSASCIIStringEncoding];
                free(machine);
                
                std::string deviceName = ChilliSource::Core::StringUtils::NSStringToString(platform);
                
                //3.5 inch screens
                if (deviceName == "iPhone1,1" || deviceName == "iPhone1,2" || deviceName == "iPhone2,1" || deviceName == "iPhone3,1" || deviceName == "iPhone3,2" ||
                    deviceName == "iPhone3,3" || deviceName == "iPhone4,1" || deviceName == "iPod1,1" || deviceName == "iPod2,1" || deviceName == "iPod3,1" || deviceName == "iPod4,1")
                {
                    m_physicalScreenSize = 3.5f;
                }
                
                //4 inch screens
                else if (deviceName == "iPhone5,1" || deviceName == "iPhone5,2" || deviceName == "iPod5,1")
                {
                    m_physicalScreenSize = 4.0f;
                }
                
                //7.9 inch screens
                else if (deviceName == "iPad2,5" || deviceName == "iPad2,6" || deviceName == "iPad2,7")
                {
                    m_physicalScreenSize = 7.9f;
                }
                
                //9.7 inch screens
                else if (deviceName == "iPad1,1" || deviceName == "iPad2,1" || deviceName == "iPad2,2" || deviceName == "iPad2,3" || deviceName == "iPad2,4" || deviceName == "iPad3,1" ||
                         deviceName == "iPad3,2" || deviceName == "iPad3,3" || deviceName == "iPad3,4" || deviceName == "iPad3,5" || deviceName == "iPad3,6")
                {
                    m_physicalScreenSize = 9.7f;
                }
                
                //Simulator
                else if (deviceName == "x86_64" || deviceName == "x86_32")
                {
                    std::string simulatorName = GetDeviceModelName();
                    
                    //iphone simulator
                    if (simulatorName == "iPhone Simulator")
                    {
                        const u32 kudwIPhone5Width = 640;
                        
                        //3.5 inch screens
                        if (GetScreenDimensions().x < kudwIPhone5Width)
                        {
                            m_physicalScreenSize = 3.5f;
                        }
                        
                        //4.0 inch screens
                        else if (GetScreenDimensions().x >= kudwIPhone5Width)
                        {
                            m_physicalScreenSize = 4.0f;
                        }
                        
                        //unknown
                        else
                        {
                            m_physicalScreenSize = 0.0f;
                        }
                    }
                    
                    //iPad simulator
                    else if (simulatorName == "iPad Simulator")
                    {
                        m_physicalScreenSize = 9.7f;
                    }
                }
                
                //unknown
                else
                {
                    m_physicalScreenSize = 0.0f;
                }
            }
            
            return m_physicalScreenSize;
		}
	}
}
