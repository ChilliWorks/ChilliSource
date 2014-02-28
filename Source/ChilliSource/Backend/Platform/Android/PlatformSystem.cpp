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

#include <ChilliSource/Backend/Platform/Android/PlatformSystem.h>
#include <ChilliSource/Backend/Platform/Android/ImageLoader.h>
#include <ChilliSource/Backend/Platform/Android/Input/InputSystem.h>
#include <ChilliSource/Backend/Platform/Android/WebViewActivity.h>
#include <ChilliSource/Backend/Platform/Android/Video/VideoPlayerActivity.h>

#include <ChilliSource/Backend/Platform/Android/JavaInterface/JavaInterfaceManager.h>
#include <ChilliSource/Backend/Platform/Android/JavaInterface/CoreJavaInterface.h>
#include <ChilliSource/Backend/Platform/Android/FMOD/FMODSystem.h>
#include <ChilliSource/Backend/Platform/Android/FMOD/FMODAudioLoader.h>
#include <ChilliSource/Backend/Platform/Android/HttpConnectionSystem.h>
#include <ChilliSource/Backend/Platform/Android/FileIO/FileSystem.h>
#include <ChilliSource/Backend/Platform/Android/LocalNotificationScheduler.h>

#include <ChilliSource/Backend/Platform/Android/Social/Communications/EmailCompositionActivity.h>
#include <ChilliSource/Backend/Platform/Android/Social/Communications/SMSCompositionActivity.h>
#include <ChilliSource/Backend/Platform/Android/Social/Communications/ContactInformationProvider.h>

#include <ChilliSource/Networking/IAP/IAPSystem.h>

#include <ChilliSource/Rendering/Base/RenderSystem.h>
#include <ChilliSource/Rendering/Base/Renderer.h>
#include <ChilliSource/Rendering/Sprite/SpriteSheetLoader.h>
#include <ChilliSource/Rendering/Sprite/XMLSpriteSheetLoader.h>
#include <ChilliSource/Rendering/Material/MaterialLoader.h>
 #include <ChilliSource/Rendering/Material/MaterialFactory.h>
#include <ChilliSource/Rendering/Font/FontLoader.h>
#include <ChilliSource/Rendering/Model/AnimatedMeshComponentUpdater.h>

#include <ChilliSource/Core/Base/Utils.h>
#include <ChilliSource/Core/File/FileSystem.h>
#include <ChilliSource/Core/Notifications/NotificationScheduler.h>
#include <ChilliSource/Core/Image/MoImageProvider.h>
#include <ChilliSource/Core/Image/ETC1ImageProvider.h>

#include <ChilliSource/Audio/Base/AudioSystem.h>
#include <ChilliSource/Audio/Base/AudioPlayer.h>

#include <ChilliSource/Rendering/GUI/GUIViewFactory.h>

#include <ChilliSource/Input/Base/InputSystem.h>

#include <ChilliSource/Backend/Rendering/OpenGL/Base/RenderSystem.h>
#include <ChilliSource/Backend/Rendering/OpenGL/Base/RenderCapabilities.h>

namespace ChilliSource 
{
	namespace Android
	{
		//-----------------------------------------
		/// Constructor
		//-----------------------------------------
		CPlatformSystem::CPlatformSystem()
		{
			//add system creator functions
			AddSystemFunc(Networking::IHttpConnectionSystem::InterfaceID,SystemCreationFunction(this, &CPlatformSystem::CreateHttpConnectionSystem));

			//add activity creator functions
			AddActivityFunc(Video::IVideoPlayerActivity::InterfaceID, ActivityCreationFunction(this, &CPlatformSystem::CreateDefaultVideoPlayerActivity));
			AddActivityFunc(IWebViewActivity::InterfaceID, ActivityCreationFunction(this, &CPlatformSystem::CreateWebViewActivity));

			AddActivityFunc(Social::IEmailCompositionActivity::InterfaceID, ActivityCreationFunction(this, &CPlatformSystem::CreateEmailCompositionActivity));

			if (CSMSCompositionActivity::SupportedByDevice())
				AddActivityFunc(Social::ISMSCompositionActivity::InterfaceID, ActivityCreationFunction(this, &CPlatformSystem::CreateSMSCompositionActivity));

			AddInfoProviderFunc(Social::IContactInformationProvider::InterfaceID, InfoProviderCreationFunction(this, &CPlatformSystem::CreateContactInformationProvider));

			CNotificationScheduler::Initialise(new CLocalNotificationScheduler());
			Core::Application::SetFileSystem(new Android::CFileSystem());
			ChilliSource::CLogging::Init();
		}
		//--------------------------------------------
		/// Add System Function
		//-------------------------------------------
		void CPlatformSystem::AddSystemFunc(Core::InterfaceIDType inInterfaceID, SystemCreationFunction inFunction)
		{
			mmapInterfaceIDToSystemFunc.insert(std::make_pair(inInterfaceID,inFunction));
		}
		//--------------------------------------------
		/// Add Activity Function
		//-------------------------------------------
		void CPlatformSystem::AddActivityFunc(Core::InterfaceIDType inInterfaceID, ActivityCreationFunction inFunction)
		{
			mmapInterfaceIDToActivityFunc.insert(std::make_pair(inInterfaceID,inFunction));
		}
		//--------------------------------------------
		/// Add Info Provider Function
		//-------------------------------------------
		void CPlatformSystem::AddInfoProviderFunc(Core::InterfaceIDType inInterfaceID, InfoProviderCreationFunction inFunction)
		{
			mmapInterfaceIDToInfoProviderFunc.insert(std::make_pair(inInterfaceID,inFunction));
		}
		//-------------------------------------------
		/// Find System Implementing
		//-------------------------------------------
		Core::ISystem* CPlatformSystem::FindSystemImplementing(Core::InterfaceIDType inInterfaceID, const std::vector<Core::SystemPtr>& inSystems) const
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
		void CPlatformSystem::Init()
		{
            //Initialise GUI factory
            Rendering::CGUIViewFactory::RegisterDefaults();
		}
        //-----------------------------------------
        /// Set Max FPS
        ///
        /// @param The maximum frames per second
        /// to clamp to. This should be in multiples
        /// of 15 (15, 30, 60)
        //-----------------------------------------
        void CPlatformSystem::SetMaxFPS(u32 inudwFPS)
        {
        	CJavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<CCoreJavaInterface>()->SetMaxFPS(inudwFPS);
        }
		//-----------------------------------------
        /// Terminate Updater
        ///
        /// Stops the update loop causing
        /// the application to terminate
        //-----------------------------------------
        void CPlatformSystem::TerminateUpdater()
        {
        	CJavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<CCoreJavaInterface>()->ForceQuit();
        }
		//-------------------------------------------------
		/// Create Default Systems
		//-------------------------------------------------
		void CPlatformSystem::CreateDefaultSystems(std::vector<Core::SystemPtr> & inaSystems)
		{
			//create the main systems
			Rendering::IRenderSystem* pRenderSystem = new OpenGL::CRenderSystem();
			inaSystems.push_back(Core::SystemPtr(pRenderSystem));
			Core::Application::SetRenderSystem(pRenderSystem);

			Input::IInputSystem* pInputSystem = new Android::CInputSystem();
			inaSystems.push_back(Core::SystemPtr(pInputSystem));
			Core::Application::SetInputSystem(pInputSystem);

			Audio::IAudioSystem* pAudioSystem = new Android::CFMODSystem();
			inaSystems.push_back(Core::SystemPtr(pAudioSystem));
			inaSystems.push_back(Core::SystemPtr(new Android::CFMODAudioLoader(pAudioSystem)));
			Core::Application::SetAudioSystem(pAudioSystem);

			//create other important systems
			OpenGL::CRenderCapabilities* pRenderCapabilities = new OpenGL::CRenderCapabilities();
			inaSystems.push_back(Core::SystemPtr(pRenderCapabilities));
			inaSystems.push_back(Core::SystemPtr(new Android::ImageLoader()));
			inaSystems.push_back(Core::SystemPtr(new CMoImageProvider()));
			inaSystems.push_back(Core::SystemPtr(new CETC1ImageProvider()));
			inaSystems.push_back(Core::SystemPtr(new Rendering::CSpriteSheetLoader()));
			inaSystems.push_back(Core::SystemPtr(new Rendering::CXMLSpriteSheetLoader()));
			inaSystems.push_back(Core::SystemPtr(new Rendering::CMaterialLoader(pRenderCapabilities)));
			inaSystems.push_back(Core::SystemPtr(new Rendering::CFontLoader()));
			inaSystems.push_back(Core::SystemPtr(new Rendering::CAnimatedMeshComponentUpdater()));
			inaSystems.push_back(Core::SystemPtr(new Rendering::CMaterialFactory()));

			//Initialise the render system
			Core::Application::GetRenderSystemPtr()->Init((u32)Core::CScreen::GetRawDimensions().x, (u32)Core::CScreen::GetRawDimensions().y);

			//Create the renderer
			Core::Application::SetRenderer(new Rendering::CRenderer(Core::Application::GetRenderSystemPtr()));

			//Initialise the input system
			if(Core::Application::GetInputSystemPtr() != NULL)
			{
				Core::Application::SetHasTouchInput((Core::Application::GetInputSystemPtr()->GetTouchScreenPtr() != NULL));
			}
		}
		//-------------------------------------------------
		/// Post Create Systems
		//-------------------------------------------------
		void CPlatformSystem::PostCreateSystems()
		{
			if(Core::Application::GetAudioSystemPtr() != NULL)
			{
				Audio::CAudioPlayer::Init();
			}
		}
		//-----------------------------------------
		/// Can Create System With Interface
		//----------------------------------------
		bool CPlatformSystem::CanCreateSystemWithInterface(Core::InterfaceIDType inInterfaceID) const
		{
			MapInterfaceIDToSystemFunc::const_iterator pFunc(mmapInterfaceIDToSystemFunc.find(inInterfaceID));
			return pFunc != mmapInterfaceIDToSystemFunc.end();
		}
		//-----------------------------------------
		/// Create and Add System With Interface
		//-----------------------------------------
		Core::ISystem* CPlatformSystem::CreateAndAddSystemWithInterface(Core::InterfaceIDType inInterfaceID, std::vector<Core::SystemPtr> & inaExistingSystems) const
		{
			Core::ISystem * pResult = NULL;

			MapInterfaceIDToSystemFunc::const_iterator pFunc(mmapInterfaceIDToSystemFunc.find(inInterfaceID));

			if (pFunc != mmapInterfaceIDToSystemFunc.end())
			{
				pResult = pFunc->second(inaExistingSystems);
			}
			
			if (pResult)
			{
				inaExistingSystems.push_back(Core::SystemPtr(pResult));
			}


			return pResult;
		}
		//--------------------------------------------
		/// Can Create Activity With Interface
		//--------------------------------------------
		bool CPlatformSystem::CanCreateActivityWithInterface(Core::InterfaceIDType inInterfaceID) const
		{
			MapInterfaceIDToActivityFunc::const_iterator pFunc(mmapInterfaceIDToActivityFunc.find(inInterfaceID));

			return pFunc != mmapInterfaceIDToActivityFunc.end();
		}
		//--------------------------------------------
		/// Create Activity With Interface
		//--------------------------------------------
		IActivity* CPlatformSystem::CreateActivityWithInterface(Core::InterfaceIDType inInterfaceID) const
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
		bool CPlatformSystem::CanCreateInformationProviderWithInterface(Core::InterfaceIDType inInterfaceID) const
		{
			MapInterfaceIDToInfoProviderFunc::const_iterator pFunc(mmapInterfaceIDToInfoProviderFunc.find(inInterfaceID));

			return pFunc != mmapInterfaceIDToInfoProviderFunc.end();
		}
		//--------------------------------------------
		/// Create Information Provider With Interface
		//--------------------------------------------
		IInformationProvider* CPlatformSystem::CreateInformationProviderWithInterface(Core::InterfaceIDType inInterfaceID) const
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
		Core::ISystem * CPlatformSystem::CreateHttpConnectionSystem(std::vector<Core::SystemPtr>& inSystems) const
		{
			return new Android::CHttpConnectionSystem();
		}
		//--------------------------------------------
		/// Create Activities
		///
		/// Creates an instance of the activity
		///
		/// @return Ownership of the activity
		//--------------------------------------------
		IActivity* CPlatformSystem::CreateSMSCompositionActivity() const
		{
			return new CSMSCompositionActivity();
		}
		IActivity * CPlatformSystem::CreateWebViewActivity() const
		{
			return new CWebViewActivity();
		}
		IActivity* CPlatformSystem::CreateEmailCompositionActivity() const
		{
			return new CEmailCompositionActivity();
		}
		IActivity * CPlatformSystem::CreateDefaultVideoPlayerActivity() const
        {
            return new CVideoPlayerActivity();
        }
		//--------------------------------------------
		/// Create Information Providers
		///
		/// Creates an instance of the info provider
		///
		/// @return Ownership of the info provider
		//--------------------------------------------
		IInformationProvider* CPlatformSystem::CreateContactInformationProvider() const
		{
			return new CContactInformationProvider();
		}
		//---------------------------------------------
		/// Get Screen Dimensions
		//---------------------------------------------
		Core::Vector2 CPlatformSystem::GetScreenDimensions() const
		{
			Core::Vector2 Result;
			CoreJavaInterfacePtr pCoreJI = CJavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<CCoreJavaInterface>();
			Result.x = pCoreJI->GetScreenWidth();
			Result.y = pCoreJI->GetScreenHeight();

			s32 dwOrientation = pCoreJI->GetOrientation();
#ifdef DEBUG
			if(dwOrientation < 0)
				CS_LOG_ERROR("CPlatformSystem::GetScreenDimensions() - Could not get orientation of device!");
#endif
			if(ChilliSource::Core::LANDSCAPE_RIGHT == dwOrientation)
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
        std::string CPlatformSystem::GetDeviceModelName() const
		{
			return CJavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<CCoreJavaInterface>()->GetDeviceModel();
		}
		//--------------------------------------------------------------
		/// Get Device Model Type Name
		//--------------------------------------------------------------
        std::string CPlatformSystem::GetDeviceModelTypeName() const
		{
			return CJavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<CCoreJavaInterface>()->GetDeviceModelType();
		}
		//--------------------------------------------------------------
		/// Get Device Manufacturer Name
		//--------------------------------------------------------------
        std::string CPlatformSystem::GetDeviceManufacturerName() const
		{
			return CJavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<CCoreJavaInterface>()->GetDeviceManufacturer();
		}
		//--------------------------------------------------------------
		/// Get OS Version
		//--------------------------------------------------------------
		std::string CPlatformSystem::GetOSVersion() const
		{
			return STRING_CAST(CJavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<CCoreJavaInterface>()->GetOSVersionCode());
		}
		//--------------------------------------------------------------
		/// Get Locale
		//--------------------------------------------------------------
		Core::CLocale CPlatformSystem::GetLocale() const
		{
			//get the locale from android
			std::string strLocale = CJavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<CCoreJavaInterface>()->GetDefaultLocaleCode();

			//break this locale into parts(language/country code/extra)
			std::vector<std::string> strLocaleBrokenUp = ChilliSource::Core::StringUtils::Split(strLocale, "_", 0);

			if (strLocaleBrokenUp.size() > 1)
			{
				return Core::CLocale(strLocaleBrokenUp[0],strLocaleBrokenUp[1]);
			}
			else if (strLocaleBrokenUp.size() == 1)
			{
				return Core::CLocale(strLocaleBrokenUp[0]);
			}
			else
				return Core::kUnknownLocale;
		}
		//--------------------------------------------------------------
		/// Get Language
		//--------------------------------------------------------------
		Core::CLocale CPlatformSystem::GetLanguage() const
		{
			return GetLocale();
		}
		//-------------------------------------------------
		/// Get Screen Density
		//-------------------------------------------------
		f32 CPlatformSystem::GetScreenDensity() const
		{
			return CJavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<CCoreJavaInterface>()->GetScreenDensity();
		}
        //-------------------------------------------------
        /// Get App Version
        ///
        /// @return The version of the application as found
		/// in the manifest
        //-------------------------------------------------
        std::string CPlatformSystem::GetAppVersion() const
		{
			return CJavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<CCoreJavaInterface>()->GetApplicationVersionName();
		}
		//-------------------------------------------------
		/// Get Device ID
		//-------------------------------------------------
		std::string CPlatformSystem::GetDeviceID()
		{
			return mUDIDManager.GetUDID();
		}
		//--------------------------------------------------------------
		/// Get Number Of CPU Cores
		//--------------------------------------------------------------
		u32 CPlatformSystem::GetNumberOfCPUCores() const
		{
			return CJavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<CCoreJavaInterface>()->GetNumberOfCores();
		}
		//--------------------------------------------------------------
		/// Get System Time
		//--------------------------------------------------------------
		TimeIntervalMs CPlatformSystem::GetSystemTimeMS() const
		{
			return CJavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<CCoreJavaInterface>()->GetSystemTimeInMilliseconds();
		}
		//-------------------------------------------------
		/// Get Physical Screen Size
		//-------------------------------------------------
		f32 CPlatformSystem::GetPhysicalScreenSize()
		{
			return CJavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<CCoreJavaInterface>()->GetPhysicalScreenSize();
		}
        //--------------------------------------------------------------------------------------------------
        /// Make Toast
        //--------------------------------------------------------------------------------------------------
        void CPlatformSystem::MakeToast(const UTF8String& instrText) const
        {
        	CJavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<CCoreJavaInterface>()->MakeToast(instrText);
        }
        //--------------------------------------------------------------------------------------------------
        /// Show System Confirm Dialog
        //--------------------------------------------------------------------------------------------------
		void CPlatformSystem::ShowSystemConfirmDialog(u32 inudwID, const UTF8String& instrTitle, const UTF8String& instrMessage, const UTF8String& instrConfirm, const UTF8String& instrCancel) const
		{
			CJavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<CCoreJavaInterface>()->ShowSystemConfirmDialog(inudwID, instrTitle, instrMessage, instrConfirm, instrCancel);
		}
        //--------------------------------------------------------------------------------------------------
        /// Show System Dialog
        //--------------------------------------------------------------------------------------------------
		void CPlatformSystem::ShowSystemDialog(u32 inudwID, const UTF8String& instrTitle, const UTF8String& instrMessage, const UTF8String& instrConfirm) const
		{
			CJavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<CCoreJavaInterface>()->ShowSystemDialog(inudwID, instrTitle, instrMessage, instrConfirm);
		}
		//-----------------------------------------
		/// Destructor
		//-----------------------------------------
		CPlatformSystem::~CPlatformSystem()
		{
			
		}
	}
}
