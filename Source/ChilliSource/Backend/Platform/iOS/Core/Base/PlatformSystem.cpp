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
#include <ChilliSource/Backend/Platform/iOS/Social/Communications/SMSCompositionActivity.h>
#include <ChilliSource/Backend/Platform/iOS/Social/Communications/EmailCompositionActivity.h>
#include <ChilliSource/Backend/Platform/iOS/Social/Communications/ContactInformationProvider.h>
#include <ChilliSource/Core/Base/MakeDelegate.h>
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
		/// Constructor
		///
		/// Default
		//-----------------------------------------
		PlatformSystem::PlatformSystem()
        : mfPhysicalScreenSize(-1.0f)
		{
            //---Systems
            AddActivityFunc(Social::EmailCompositionActivity::InterfaceID, Core::MakeDelegate(this, &PlatformSystem::CreateEmailCompositionActivity));
 
            //---Activities
			if(SMSCompositionActivity::SupportedByDevice())
            {
				AddActivityFunc(Social::SMSCompositionActivity::InterfaceID, Core::MakeDelegate(this, &PlatformSystem::CreateSMSCompositionActivity));
            }
			
            //---Info providers
			AddInfoProviderFunc(Social::ContactInformationProvider::InterfaceID, Core::MakeDelegate(this, &PlatformSystem::CreateContactInformationProvider));
		}
        //--------------------------------------------
        /// Add Activity Function
        ///
        /// Map the creation function with the
        /// activity type
        ///
        /// @param Activity interface ID
        /// @param Creation delegate
        //-------------------------------------------
		void PlatformSystem::AddActivityFunc(Core::InterfaceIDType inInterfaceID, ActivityCreationFunction inFunction)
        {
			mmapInterfaceIDToActivityFunc.insert(std::make_pair(inInterfaceID,inFunction));
		}
        //--------------------------------------------
        /// Add Info Provider Function
        ///
        /// Map the creation function with the
        /// system type
        ///
        /// @param Info provider interface ID
        /// @param Creation delegate
        //-------------------------------------------
		void PlatformSystem::AddInfoProviderFunc(Core::InterfaceIDType inInterfaceID, InfoProviderCreationFunction inFunction)
        {
			mmapInterfaceIDToInfoProviderFunc.insert(std::make_pair(inInterfaceID,inFunction));
		}
		//-----------------------------------------
		/// Init
		//-----------------------------------------
		void PlatformSystem::Init()
		{

		}
		//-------------------------------------------------
		/// Create Default Systems
		///
		/// Adds default systems to the applications system
		/// list.
		///
		/// @param the system list
		//-------------------------------------------------
		void PlatformSystem::CreateDefaultSystems(Core::Application* in_application)
		{

		}
		//-------------------------------------------------
		/// Post Create Systems
		///
		/// Deals with anything that needs to be handled after
		/// creating the systems.
		///
		/// @param the system list
		//-------------------------------------------------
		void PlatformSystem::PostCreateSystems()
		{

		}
        //-----------------------------------------
        /// Run
        ///
        /// Begin the game loop
        //-----------------------------------------
        void PlatformSystem::Run()
        {
            iOSInit();
        }
        //-----------------------------------------
        /// Set Max FPS
        ///
        /// @param The maximum frames per second
        /// to clamp to. This should be in multiples
        /// of 15 (15, 30, 60)
        //-----------------------------------------
        void PlatformSystem::SetMaxFPS(u32 inudwFPS)
        {
            iOSSetMaxFPS(inudwFPS);
        }
		//-----------------------------------------
		/// Set Updater Active
		///
		/// Starts or stops the platforms 
		/// update loop.
		///
		/// @param Whether to end or begin
		//-----------------------------------------
		void PlatformSystem::SetUpdaterActive(bool inbIsActive)
		{
			iOSSetUpdaterActive(inbIsActive);
		}
        //-----------------------------------------
        /// Terminate Updater
        ///
        /// Stops the update loop causing
        /// the application to terminate
        //-----------------------------------------
        void PlatformSystem::TerminateUpdater() 
        {
            iOSInvalidateUpdater();
        }
		//-----------------------------------------
        /// Can Create Activity With Interface
        ///
		/// @param Interface ID
		/// @return Whether activity can be created
		//----------------------------------------
		bool PlatformSystem::CanCreateActivityWithInterface(Core::InterfaceIDType inInterfaceID) const
        {
			MapInterfaceIDToActivityFunc::const_iterator pFunc(mmapInterfaceIDToActivityFunc.find(inInterfaceID));
			
			return pFunc != mmapInterfaceIDToActivityFunc.end();
		}
		//-----------------------------------------
        /// Create Activity With Interface
        ///
		/// Tries to create a platform specific implementation with the given interface
		///
		/// @param InterfaceID to generate
		/// @return A handle to the given activity or nullptr if the platform cannot support it
		//-----------------------------------------
		Core::Activity* PlatformSystem::CreateActivityWithInterface(Core::InterfaceIDType inInterfaceID) const
        {
			MapInterfaceIDToActivityFunc::const_iterator pFunc(mmapInterfaceIDToActivityFunc.find(inInterfaceID));
			
			if (pFunc != mmapInterfaceIDToActivityFunc.end())
            {
				return pFunc->second();
			}
			
			return nullptr;
		}
		//-----------------------------------------
        /// Can Create Information Provider With Interface
        ///
		/// @param Interface ID
		/// @return Whether system can be created
		//----------------------------------------
		bool PlatformSystem::CanCreateInformationProviderWithInterface(Core::InterfaceIDType inInterfaceID) const
        {
			MapInterfaceIDToInfoProviderFunc::const_iterator pFunc(mmapInterfaceIDToInfoProviderFunc.find(inInterfaceID));
			
			return pFunc != mmapInterfaceIDToInfoProviderFunc.end();
		}
		//-----------------------------------------
        /// Create Information Provider With Interface
        ///
		/// Tries to create a platform specific implementation with the given interface
		///
		/// @param InterfaceID to generate
		/// @return A handle to the given system or nullptr if the platform cannot support it
		//-----------------------------------------
		Core::IInformationProvider* PlatformSystem::CreateInformationProviderWithInterface(Core::InterfaceIDType inInterfaceID) const
        {
			MapInterfaceIDToInfoProviderFunc::const_iterator pFunc(mmapInterfaceIDToInfoProviderFunc.find(inInterfaceID));
			if (pFunc != mmapInterfaceIDToInfoProviderFunc.end())
            {
				return pFunc->second();
			}
			return nullptr;
		}
        //--------------------------------------------
        /// Create Activities
        ///
        /// Creates an instance of the activity
        ///
        /// @return Ownership of the activity
        //--------------------------------------------
		Core::Activity* PlatformSystem::CreateSMSCompositionActivity() const
        {
			return new SMSCompositionActivity();
		}
		Core::Activity* PlatformSystem::CreateEmailCompositionActivity() const
        {
			return new EmailCompositionActivity();
		}
        //--------------------------------------------
        /// Create Information Providers
        ///
        /// Creates an instance of the info provider
        ///
        /// @return Ownership of the info provider
        //--------------------------------------------
		Core::IInformationProvider* PlatformSystem::CreateContactInformationProvider() const
        {
			return new ContactInformationProvider();
		}
		//-----------------------------------------------------------------------------------------------------------
		/// Get Screen Dimensions
		///
		/// Retrieves the screen dimensions. These dimensions are always in the default orientation for the device.
		/// @return A Vector2 containing the screen size in it's x + y components
        ///
        /// The dimensions are always for the base screen size and the density scale factor is use to scale the screen
        /// to the correct resolution. i.e. the Retina screen will be 320x480 and a density of 2.0
		//-----------------------------------------------------------------------------------------------------------
		Core::Vector2 PlatformSystem::GetScreenDimensions() const
		{
			Core::Vector2 Result;
			CGSize Size = [[UIScreen mainScreen] bounds].size;
            
            f32 fScale = 1.0f;
            
            bool bIsiOS4_0 = ([[[UIDevice currentDevice] systemVersion] floatValue] >= 4.0f);
            
            if(bIsiOS4_0)
            {
                fScale = [UIScreen mainScreen].scale;
            }
            
			Result.x = Size.width * fScale;
			Result.y = Size.height * fScale;
            
			return Result;
		}
		//--------------------------------------------------------------
		/// Get Device Model Name
		///
		/// @return The above information stringified
		//--------------------------------------------------------------
		std::string PlatformSystem::GetDeviceModelName() const
		{
			NSString * nsType = [[UIDevice currentDevice] model];

			return (ChilliSource::Core::StringUtils::NSStringToString(nsType));
		}
		//--------------------------------------------------------------
		/// Get Device Model Type Name
		///
		/// @return The above information stringified
		//--------------------------------------------------------------
		std::string PlatformSystem::GetDeviceModelTypeName() const
		{
			size_t size = 0;
			sysctlbyname("hw.machine", nullptr, &size, nullptr, 0);
			char* machine = (char*)malloc(size);
			sysctlbyname("hw.machine", machine, &size, nullptr, 0);
			NSString *platform = [NSString stringWithCString:machine encoding:NSASCIIStringEncoding];
			free(machine);

			std::string strOutput;
			std::string strModelType = ChilliSource::Core::StringUtils::NSStringToString(platform);
			bool bRecord = false;
			for(std::string::const_iterator it = strModelType.begin(); it != strModelType.end(); ++it)
			{
				if(isdigit(*it))
				{
					bRecord = true;
				}
				
				if(bRecord)
				{
					strOutput += (*it);
				}
			}
			return strOutput;
		}
		//--------------------------------------------------------------
		/// Get Device Manufacturer Name
		///
		/// @return The above information stringified
		//--------------------------------------------------------------
		std::string PlatformSystem::GetDeviceManufacturerName() const
		{
			return std::string("Apple");
		}
        //--------------------------------------------------------------
        /// Get OS Version
        ///
        /// @return String containing the OS version of the device
        //--------------------------------------------------------------
        std::string PlatformSystem::GetOSVersion() const
        {
            NSString* NSVersion = [[UIDevice currentDevice] systemVersion];
			return ChilliSource::Core::StringUtils::NSStringToString(NSVersion);
        }
        //--------------------------------------------------------------
		/// Get Locale
		///
		/// Get the active locale of the device
		/// @return Locale ID
		//--------------------------------------------------------------
		Core::Locale PlatformSystem::GetLocale() const
		{
			NSLocale *pcLocale = [NSLocale currentLocale];
			NSString *pcCountryCode = [pcLocale objectForKey:NSLocaleCountryCode];
			std::string strCountryCode = [pcCountryCode UTF8String];
			NSString *pcLanguageCode = [pcLocale objectForKey:NSLocaleLanguageCode];
			std::string strLanguageCode = [pcLanguageCode UTF8String];

			//Just default to english
			return ChilliSource::Core::Locale(strLanguageCode, strCountryCode);
		}
        //--------------------------------------------------------------
		/// Get Language
		///
		/// Get the active language of the device in locale format
		/// @return Locale ID
		//--------------------------------------------------------------
		Core::Locale PlatformSystem::GetLanguage() const
		{
			NSUserDefaults* UserDefaults = [NSUserDefaults standardUserDefaults];
			NSArray* SupportedLanguages = [UserDefaults objectForKey:@"AppleLanguages"];
			NSString* NSUserLocale = [SupportedLanguages objectAtIndex:0];
			std::string strLocaleCode = [NSUserLocale UTF8String];

			//break this locale into parts(language/country code/extra)
			std::vector<std::string> strLocaleBrokenUp = ChilliSource::Core::StringUtils::Split(strLocaleCode, "-", 0);

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
        //-------------------------------------------------
        /// Get Screen Density
        ///
        /// @return The density scale factor of the screen
        /// to convert from DIPS to physical pixels
        //-------------------------------------------------
        f32 PlatformSystem::GetScreenDensity() const
        {
            if ([[[UIDevice currentDevice] systemVersion] floatValue] >= 4.0f) 
            {
				CGFloat fScale = [UIScreen mainScreen].scale;
                
                return fScale;
			}
            
            return 1.0f;
        }
        //-------------------------------------------------
        /// Get Device ID
        ///
        /// @return The UDID of the device
        //-------------------------------------------------
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
                NSString* strUDID = [[UIDevice currentDevice] uniqueGlobalDeviceIdentifier];
                return ChilliSource::Core::StringUtils::NSStringToString(strUDID);
            }
        }
        //-------------------------------------------------
        /// Get App Version
        ///
        /// @return The bundle version as found in the plist
        //-------------------------------------------------
        std::string PlatformSystem::GetAppVersion() const
        {
            NSString* strVersion = [[NSBundle mainBundle] objectForInfoDictionaryKey:(NSString *)kCFBundleVersionKey];
            return ChilliSource::Core::StringUtils::NSStringToString(strVersion);
        }
		//--------------------------------------------------------------
		/// Get Number Of CPU Cores
		///
		/// @return The number of cores available
		//--------------------------------------------------------------
		u32 PlatformSystem::GetNumberOfCPUCores() const
		{
			u32 udwNumCores = 1;
			size_t udwSize = sizeof(udwNumCores);
			
			if(sysctlbyname("hw.ncpu", &udwNumCores, &udwSize, nullptr, 0)) 
			{
				return 1;
			}
			else
			{
				return udwNumCores;
			}
		}
		//--------------------------------------------------------------
		/// Get System Time
		///
		/// @return The current time in milliseconds
		//--------------------------------------------------------------
		TimeIntervalMs PlatformSystem::GetSystemTimeMS() const
		{
			return GetSystemTimeInNanoSeconds() / 1000000;
		}
		//-------------------------------------------------
		/// Get Physical Screen Size
		///
		/// @return The physical size of the screen in
		/// inches.
		//-------------------------------------------------
		f32 PlatformSystem::GetPhysicalScreenSize()
		{
            if (mfPhysicalScreenSize < 0.0f)
            {
                size_t size = 0;
                sysctlbyname("hw.machine", nullptr, &size, nullptr, 0);
                char* machine = (char*)malloc(size);
                sysctlbyname("hw.machine", machine, &size, nullptr, 0);
                NSString *platform = [NSString stringWithCString:machine encoding:NSASCIIStringEncoding];
                free(machine);
                
                std::string strDeviceName = ChilliSource::Core::StringUtils::NSStringToString(platform);
                
                //3.5 inch screens
                if (strDeviceName == "iPhone1,1" || strDeviceName == "iPhone1,2" || strDeviceName == "iPhone2,1" || strDeviceName == "iPhone3,1" || strDeviceName == "iPhone3,2" ||
                    strDeviceName == "iPhone3,3" || strDeviceName == "iPhone4,1" || strDeviceName == "iPod1,1" || strDeviceName == "iPod2,1" || strDeviceName == "iPod3,1" || strDeviceName == "iPod4,1")
                {
                    mfPhysicalScreenSize = 3.5f;
                }
                
                //4 inch screens
                else if (strDeviceName == "iPhone5,1" || strDeviceName == "iPhone5,2" || strDeviceName == "iPod5,1")
                {
                    mfPhysicalScreenSize = 4.0f;
                }
                
                //7.9 inch screens
                else if (strDeviceName == "iPad2,5" || strDeviceName == "iPad2,6" || strDeviceName == "iPad2,7")
                {
                    mfPhysicalScreenSize = 7.9f;
                }
                
                //9.7 inch screens
                else if (strDeviceName == "iPad1,1" || strDeviceName == "iPad2,1" || strDeviceName == "iPad2,2" || strDeviceName == "iPad2,3" || strDeviceName == "iPad2,4" || strDeviceName == "iPad3,1" ||
                         strDeviceName == "iPad3,2" || strDeviceName == "iPad3,3" || strDeviceName == "iPad3,4" || strDeviceName == "iPad3,5" || strDeviceName == "iPad3,6")
                {
                    mfPhysicalScreenSize = 9.7f;
                }
                
                //Simulator
                else if (strDeviceName == "x86_64" || strDeviceName == "x86_32")
                {
                    std::string strSimulatorName = GetDeviceModelName();
                    
                    //iphone simulator
                    if (strSimulatorName == "iPhone Simulator")
                    {
                        const u32 kudwIPhone5Width = 640;
                        
                        //3.5 inch screens
                        if (GetScreenDimensions().x < kudwIPhone5Width)
                        {
                            mfPhysicalScreenSize = 3.5f;
                        }
                        
                        //4.0 inch screens
                        else if (GetScreenDimensions().x >= kudwIPhone5Width)
                        {
                            mfPhysicalScreenSize = 4.0f;
                        }
                        
                        //unknown
                        else
                        {
                            mfPhysicalScreenSize = 0.0f;
                        }
                    }
                    
                    //iPad simulator
                    else if (strSimulatorName == "iPad Simulator")
                    {
                        mfPhysicalScreenSize = 9.7f;
                    }
                }
                
                //unknown
                else
                {
                    mfPhysicalScreenSize = 0.0f;
                }
            }
            
            return mfPhysicalScreenSize;
		}
	}
}
