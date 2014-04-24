//
//  Device.mm
//  Chilli Source
//  Created by Ian Copland on 24/04/2014.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2014 Tag Games Limited
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//

#import <ChilliSource/Backend/Platform/iOS/Core/Base/Device.h>
#import <ChilliSource/Backend/Platform/iOS/Core/String/NSStringUtils.h>

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <sys/types.h>
#import <sys/sysctl.h>
#import <AdSupport/ASIdentifierManager.h>

namespace ChilliSource
{
    namespace iOS
    {
        namespace
        {
            //----------------------------------------------
            //----------------------------------------------
            std::string GetDeviceModel()
            {
                @autoreleasepool
                {
                    NSString * type = [[UIDevice currentDevice] model];
                    return [NSStringUtils newStringWithNSString:type];
                }
            }
            //----------------------------------------------
            //----------------------------------------------
            std::string GetDeviceModelType()
            {
                size_t size = 0;
                sysctlbyname("hw.machine", nullptr, &size, nullptr, 0);
                s8* machine = new s8[size];
                sysctlbyname("hw.machine", machine, &size, nullptr, 0);
                std::string modelType(machine);
                CS_SAFEDELETE(machine);
                
                std::string output;
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
            std::string GetDeviceManufacturer()
            {
                return "Apple";
            }
            //---------------------------------------------
            //---------------------------------------------
            std::string GetOSVersion()
            {
                @autoreleasepool
                {
                    NSString* version = [[UIDevice currentDevice] systemVersion];
                    return [NSStringUtils newStringWithNSString:version];
                }
            }
            //---------------------------------------------
            //---------------------------------------------
            Core::Locale GetLocale()
            {
                @autoreleasepool
                {
                    NSLocale *locale = [NSLocale currentLocale];
                    NSString *countryCodeObjc = [locale objectForKey:NSLocaleCountryCode];
                    std::string countryCode = [countryCodeObjc UTF8String];
                    NSString *languageCodeObjc = [locale objectForKey:NSLocaleLanguageCode];
                    std::string languageCode = [languageCodeObjc UTF8String];
                    
                    return ChilliSource::Core::Locale(languageCode, countryCode);
                }
            }
            //---------------------------------------------
            //---------------------------------------------
            Core::Locale GetLanguage()
            {
                @autoreleasepool
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
                    {
                        return Core::kUnknownLocale;
                    }
                }
            }
            //-----------------------------------------------
            //-----------------------------------------------
            std::string GetUDID()
            {
                @autoreleasepool
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
                        //TODO: remove this?
                        NSString* udid = [[UIDevice currentDevice] uniqueGlobalDeviceIdentifier];
                        return [NSStringUtils newStringWithNSString:udid];
                    }
                }
            }
            //-------------------------------------------------
            //-------------------------------------------------
            u32 GetNumberOfCPUCores()
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
        }
        
        CS_DEFINE_NAMEDTYPE(Device);
        //----------------------------------------------------
        //----------------------------------------------------
        Device::Device()
            : m_locale(Core::kUnknownLocale), m_language(Core::kUnknownLocale)
        {
            m_model = GetModel();
            m_modelType = GetModelType();
            m_manufacturer = GetManufacturer();
            m_locale = GetLocale();
            m_language = GetLanguage();
            m_osVersion = GetOSVersion();
            m_udid = GetUDID();
            m_numCPUCores = iOS::GetNumberOfCPUCores();
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        bool Device::IsA(Core::InterfaceIDType in_interfaceId) const
        {
            return (Core::Device::InterfaceID == in_interfaceId || Device::InterfaceID == in_interfaceId);
        }
        //---------------------------------------------------
        //---------------------------------------------------
        const std::string& Device::GetModel()
        {
            return m_model;
        }
        //---------------------------------------------------
        //---------------------------------------------------
        const std::string& Device::GetModelType()
        {
            return m_modelType;
        }
        //---------------------------------------------------
        //---------------------------------------------------
        const std::string& Device::GetManufacturer()
        {
            return m_manufacturer;
        }
        //---------------------------------------------------
        //---------------------------------------------------
        Core::Locale& Device::GetLocale()
        {
            return m_locale;
        }
        //---------------------------------------------------
        //---------------------------------------------------
        Core::Locale& Device::GetLanguage()
        {
            return m_language;
        }
        //---------------------------------------------------
        //---------------------------------------------------
        const std::string& Device::GetOSVersion()
        {
            return m_osVersion;
        }
        //---------------------------------------------------
        //---------------------------------------------------
        const std::string& Device::GetUDID()
        {
            return m_udid;
        }
        //---------------------------------------------------
        //---------------------------------------------------
        u32 Device::GetNumberOfCPUCores()
        {
            return m_numCPUCores;
        }
    }
}


