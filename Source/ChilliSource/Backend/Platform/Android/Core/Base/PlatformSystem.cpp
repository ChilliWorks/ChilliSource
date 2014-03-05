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

#include <ChilliSource/Audio/Base/AudioSystem.h>
#include <ChilliSource/Audio/Base/AudioPlayer.h>
#include <ChilliSource/Backend/Audio/FMOD/Base/AudioLoader.h>
#include <ChilliSource/Backend/Audio/FMOD/Base/FMODSystem.h>
#include <ChilliSource/Backend/Platform/Android/Core/Base/CoreJavaInterface.h>
#include <ChilliSource/Backend/Platform/Android/Core/File/FileSystem.h>
#include <ChilliSource/Backend/Platform/Android/Core/Image/ImageLoader.h>
#include <ChilliSource/Backend/Platform/Android/Core/JNI/JavaInterfaceManager.h>
#include <ChilliSource/Backend/Platform/Android/Core/Notifications/LocalNotificationScheduler.h>
#include <ChilliSource/Backend/Platform/Android/Input/Base/InputSystem.h>
#include <ChilliSource/Backend/Platform/Android/Networking/Http/HttpConnectionSystem.h>
#include <ChilliSource/Backend/Platform/Android/Social/Communications/ContactInformationProvider.h>
#include <ChilliSource/Backend/Platform/Android/Social/Communications/EmailCompositionActivity.h>
#include <ChilliSource/Backend/Platform/Android/Social/Communications/SMSCompositionActivity.h>
#include <ChilliSource/Backend/Platform/Android/Video/Base/VideoPlayerActivity.h>
#include <ChilliSource/Backend/Platform/Android/Web/Base/WebViewActivity.h>
#include <ChilliSource/Backend/Rendering/OpenGL/Base/RenderSystem.h>
#include <ChilliSource/Backend/Rendering/OpenGL/Base/RenderCapabilities.h>
#include <ChilliSource/Core/Base/MakeDelegate.h>
#include <ChilliSource/Core/Base/Utils.h>
#include <ChilliSource/Core/File/FileSystem.h>
#include <ChilliSource/Core/Image/ETC1ImageProvider.h>
#include <ChilliSource/Core/Image/MoImageProvider.h>
#include <ChilliSource/Core/Notifications/NotificationScheduler.h>
#include <ChilliSource/GUI/Base/GUIViewFactory.h>
#include <ChilliSource/Input/Base/InputSystem.h>
#include <ChilliSource/Networking/IAP/IAPSystem.h>
#include <ChilliSource/Rendering/Base/RenderSystem.h>
#include <ChilliSource/Rendering/Base/Renderer.h>
#include <ChilliSource/Rendering/Font/FontLoader.h>
#include <ChilliSource/Rendering/Material/MaterialLoader.h>
#include <ChilliSource/Rendering/Material/MaterialFactory.h>
#include <ChilliSource/Rendering/Model/AnimatedMeshComponentUpdater.h>
#include <ChilliSource/Rendering/Sprite/SpriteSheetLoader.h>
#include <ChilliSource/Rendering/Sprite/XMLSpriteSheetLoader.h>

namespace ChilliSource 
{
	namespace Android
	{
		//-----------------------------------------
		/// Constructor
		//-----------------------------------------
		PlatformSystem::PlatformSystem()
		{
			//add system creator functions
			AddSystemFunc(Networking::HttpConnectionSystem::InterfaceID, Core::MakeDelegate(this, &PlatformSystem::CreateHttpConnectionSystem));

			//add activity creator functions
			AddActivityFunc(Video::VideoPlayerActivity::InterfaceID, Core::MakeDelegate(this, &PlatformSystem::CreateDefaultVideoPlayerActivity));
			AddActivityFunc(Web::WebViewActivity::InterfaceID, Core::MakeDelegate(this, &PlatformSystem::CreateWebViewActivity));
			AddActivityFunc(Social::EmailCompositionActivity::InterfaceID, Core::MakeDelegate(this, &PlatformSystem::CreateEmailCompositionActivity));

			if (SMSCompositionActivity::SupportedByDevice())
				AddActivityFunc(Social::SMSCompositionActivity::InterfaceID, Core::MakeDelegate(this, &PlatformSystem::CreateSMSCompositionActivity));

			AddInfoProviderFunc(Social::ContactInformationProvider::InterfaceID, Core::MakeDelegate(this, &PlatformSystem::CreateContactInformationProvider));

			Core::NotificationScheduler::Initialise(new LocalNotificationScheduler());
			Core::Application::Get()->SetFileSystem(new FileSystem());
			Core::Logging::Init();
		}
		//--------------------------------------------
		/// Add System Function
		//-------------------------------------------
		void PlatformSystem::AddSystemFunc(Core::InterfaceIDType inInterfaceID, SystemCreationFunction inFunction)
		{
			mmapInterfaceIDToSystemFunc.insert(std::make_pair(inInterfaceID,inFunction));
		}
		//--------------------------------------------
		/// Add Activity Function
		//-------------------------------------------
		void PlatformSystem::AddActivityFunc(Core::InterfaceIDType inInterfaceID, ActivityCreationFunction inFunction)
		{
			mmapInterfaceIDToActivityFunc.insert(std::make_pair(inInterfaceID,inFunction));
		}
		//--------------------------------------------
		/// Add Info Provider Function
		//-------------------------------------------
		void PlatformSystem::AddInfoProviderFunc(Core::InterfaceIDType inInterfaceID, InfoProviderCreationFunction inFunction)
		{
			mmapInterfaceIDToInfoProviderFunc.insert(std::make_pair(inInterfaceID,inFunction));
		}
		//-------------------------------------------
		/// Find System Implementing
		//-------------------------------------------
		Core::System* PlatformSystem::FindSystemImplementing(Core::InterfaceIDType inInterfaceID, const std::vector<Core::SystemSPtr>& inSystems) const
		{
			for(u32 nSystem = 0; nSystem < inSystems.size(); nSystem++)
			{
				if (inSystems[nSystem]->IsA(inInterfaceID))
				{
					return inSystems[nSystem].get();
				}
			}

			return NULL;
		}
		//-----------------------------------------
		/// Init
		//-----------------------------------------
		void PlatformSystem::Init()
		{
            //Initialise GUI factory
            GUI::GUIViewFactory::RegisterDefaults();
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
		//-------------------------------------------------
		/// Create Default Systems
		//-------------------------------------------------
		void PlatformSystem::CreateDefaultSystems(std::vector<Core::SystemSPtr> & inaSystems)
		{
			//create the main systems
			OpenGL::RenderSystem* pRenderSystem = new OpenGL::RenderSystem();
			inaSystems.push_back(Core::SystemSPtr(pRenderSystem));
			Core::Application::Get()->SetRenderSystem(pRenderSystem);

			Input::InputSystem* pInputSystem = new Android::InputSystem();
			inaSystems.push_back(Core::SystemSPtr(pInputSystem));
			Core::Application::Get()->SetInputSystem(pInputSystem);

			Audio::AudioSystem* pAudioSystem = new FMOD::FMODSystem();
			inaSystems.push_back(Core::SystemSPtr(pAudioSystem));
			inaSystems.push_back(Core::SystemSPtr(new FMOD::AudioLoader(pAudioSystem)));
			Core::Application::Get()->SetAudioSystem(pAudioSystem);

			//create other important systems
			OpenGL::RenderCapabilities* pRenderCapabilities = new OpenGL::RenderCapabilities();
			inaSystems.push_back(Core::SystemSPtr(pRenderCapabilities));
			inaSystems.push_back(Core::SystemSPtr(new Android::ImageLoader()));
			inaSystems.push_back(Core::SystemSPtr(new Core::MoImageProvider()));
			inaSystems.push_back(Core::SystemSPtr(new Core::ETC1ImageProvider()));
			inaSystems.push_back(Core::SystemSPtr(new Rendering::SpriteSheetLoader()));
			inaSystems.push_back(Core::SystemSPtr(new Rendering::XMLSpriteSheetLoader()));
			inaSystems.push_back(Core::SystemSPtr(new Rendering::MaterialLoader(pRenderCapabilities)));
			inaSystems.push_back(Core::SystemSPtr(new Rendering::FontLoader()));
			inaSystems.push_back(Core::SystemSPtr(new Rendering::AnimatedMeshComponentUpdater()));
			inaSystems.push_back(Core::SystemSPtr(new Rendering::MaterialFactory(pRenderSystem->GetTextureManager(), pRenderSystem->GetShaderManager(), pRenderSystem->GetCubemapManager(), pRenderCapabilities)));

			//Initialise the render system
			Core::Application::Get()->GetRenderSystem()->Init((u32)Core::Screen::GetRawDimensions().x, (u32)Core::Screen::GetRawDimensions().y);

			//Create the renderer
			Core::Application::Get()->SetRenderer(new Rendering::Renderer(Core::Application::Get()->GetRenderSystem()));
		}
		//-------------------------------------------------
		/// Post Create Systems
		//-------------------------------------------------
		void PlatformSystem::PostCreateSystems()
		{
			if(Core::Application::Get()->GetAudioSystem() != NULL)
			{
				Audio::AudioPlayer::Init();
			}
		}
		//-----------------------------------------
		/// Can Create System With Interface
		//----------------------------------------
		bool PlatformSystem::CanCreateSystemWithInterface(Core::InterfaceIDType inInterfaceID) const
		{
			MapInterfaceIDToSystemFunc::const_iterator pFunc(mmapInterfaceIDToSystemFunc.find(inInterfaceID));
			return pFunc != mmapInterfaceIDToSystemFunc.end();
		}
		//-----------------------------------------
		/// Create and Add System With Interface
		//-----------------------------------------
		Core::System* PlatformSystem::CreateAndAddSystemWithInterface(Core::InterfaceIDType inInterfaceID, std::vector<Core::SystemSPtr> & inaExistingSystems) const
		{
			Core::System * pResult = NULL;

			MapInterfaceIDToSystemFunc::const_iterator pFunc(mmapInterfaceIDToSystemFunc.find(inInterfaceID));

			if (pFunc != mmapInterfaceIDToSystemFunc.end())
			{
				pResult = pFunc->second(inaExistingSystems);
			}
			
			if (pResult)
			{
				inaExistingSystems.push_back(Core::SystemSPtr(pResult));
			}


			return pResult;
		}
		//--------------------------------------------
		/// Can Create Activity With Interface
		//--------------------------------------------
		bool PlatformSystem::CanCreateActivityWithInterface(Core::InterfaceIDType inInterfaceID) const
		{
			MapInterfaceIDToActivityFunc::const_iterator pFunc(mmapInterfaceIDToActivityFunc.find(inInterfaceID));

			return pFunc != mmapInterfaceIDToActivityFunc.end();
		}
		//--------------------------------------------
		/// Create Activity With Interface
		//--------------------------------------------
		Core::Activity* PlatformSystem::CreateActivityWithInterface(Core::InterfaceIDType inInterfaceID) const
		{
			MapInterfaceIDToActivityFunc::const_iterator pFunc(mmapInterfaceIDToActivityFunc.find(inInterfaceID));

			if (pFunc != mmapInterfaceIDToActivityFunc.end())
			{
				return pFunc->second();
			}

			return NULL;
		}
		//--------------------------------------------
		/// Can Create Information Provider With Interface
		//--------------------------------------------
		bool PlatformSystem::CanCreateInformationProviderWithInterface(Core::InterfaceIDType inInterfaceID) const
		{
			MapInterfaceIDToInfoProviderFunc::const_iterator pFunc(mmapInterfaceIDToInfoProviderFunc.find(inInterfaceID));

			return pFunc != mmapInterfaceIDToInfoProviderFunc.end();
		}
		//--------------------------------------------
		/// Create Information Provider With Interface
		//--------------------------------------------
		Core::IInformationProvider* PlatformSystem::CreateInformationProviderWithInterface(Core::InterfaceIDType inInterfaceID) const
		{
			MapInterfaceIDToInfoProviderFunc::const_iterator pFunc(mmapInterfaceIDToInfoProviderFunc.find(inInterfaceID));
			if (pFunc != mmapInterfaceIDToInfoProviderFunc.end())
			{
				return pFunc->second();
			}
			return NULL;
		}
		//--------------------------------------------
		/// Create Http Connection System
		//--------------------------------------------
		Core::System* PlatformSystem::CreateHttpConnectionSystem(std::vector<Core::SystemSPtr>& inSystems) const
		{
			return new Android::HttpConnectionSystem();
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
		Core::Activity * PlatformSystem::CreateWebViewActivity() const
		{
			return new WebViewActivity();
		}
		Core::Activity* PlatformSystem::CreateEmailCompositionActivity() const
		{
			return new EmailCompositionActivity();
		}
		Core::Activity * PlatformSystem::CreateDefaultVideoPlayerActivity() const
        {
            return new VideoPlayerActivity();
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
		//---------------------------------------------
		/// Get Screen Dimensions
		//---------------------------------------------
		Core::Vector2 PlatformSystem::GetScreenDimensions() const
		{
			Core::Vector2 Result;
			CoreJavaInterfacePtr pCoreJI = JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<CoreJavaInterface>();
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
        //--------------------------------------------------------------------------------------------------
        /// Make Toast
        //--------------------------------------------------------------------------------------------------
        void PlatformSystem::MakeToast(const Core::UTF8String& instrText) const
        {
        	JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<CoreJavaInterface>()->MakeToast(instrText);
        }
        //--------------------------------------------------------------------------------------------------
        /// Show System Confirm Dialog
        //--------------------------------------------------------------------------------------------------
		void PlatformSystem::ShowSystemConfirmDialog(u32 inudwID, const Core::UTF8String& instrTitle, const Core::UTF8String& instrMessage, const Core::UTF8String& instrConfirm, const Core::UTF8String& instrCancel) const
		{
			JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<CoreJavaInterface>()->ShowSystemConfirmDialog(inudwID, instrTitle, instrMessage, instrConfirm, instrCancel);
		}
        //--------------------------------------------------------------------------------------------------
        /// Show System Dialog
        //--------------------------------------------------------------------------------------------------
		void PlatformSystem::ShowSystemDialog(u32 inudwID, const Core::UTF8String& instrTitle, const Core::UTF8String& instrMessage, const Core::UTF8String& instrConfirm) const
		{
			JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<CoreJavaInterface>()->ShowSystemDialog(inudwID, instrTitle, instrMessage, instrConfirm);
		}
		//-----------------------------------------
		/// Destructor
		//-----------------------------------------
		PlatformSystem::~PlatformSystem()
		{
			
		}
	}
}
