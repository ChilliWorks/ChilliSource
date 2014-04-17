//
//  PlatformSystem.cpp
//  Chilli Source
//
//  Created by Scott Downie on 24/11/2010.
//  Copyright (c) 2014 Tag Games Ltd. All rights reserved.
//
#include <ChilliSource/Backend/Platform/iOS/Core/Base/PlatformSystem.h>

#include <ChilliSource/Backend/Platform/iOS/Core/String/NSStringUtils.h>
#include <ChilliSource/Backend/Platform/iOS/Core/String/NSString+MD5Addition.h>
#include <ChilliSource/Backend/Platform/iOS/Core/Base/UIDevice+IdentifierAddition.h>
#include <ChilliSource/Backend/Platform/iOS/Core/Base/NativeSystem.h>
#include <ChilliSource/Backend/Rendering/OpenGL/Shader/GLSLShaderProvider.h>
#include <ChilliSource/Backend/Rendering/OpenGL/Texture/TextureUnitSystem.h>
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
            in_application->CreateSystem<OpenGL::GLSLShaderProvider>();
            in_application->CreateSystem<OpenGL::TextureUnitSystem>();
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

			return [NSStringUtils newStringWithNSString:type];
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
			std::string modelType = [NSStringUtils newStringWithNSString:platform];
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
			return [NSStringUtils newStringWithNSString:version];
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
                
                return [NSStringUtils newStringWithNSString:[uid UUIDString]];
            }
            else
            {
                NSString* udid = [[UIDevice currentDevice] uniqueGlobalDeviceIdentifier];
                return [NSStringUtils newStringWithNSString:udid];
            }
        }
        //-------------------------------------------------
        //-------------------------------------------------
        std::string PlatformSystem::GetAppVersion() const
        {
            NSString* version = [[NSBundle mainBundle] objectForInfoDictionaryKey:(NSString *)kCFBundleVersionKey];
            return [NSStringUtils newStringWithNSString:version];
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
	}
}
