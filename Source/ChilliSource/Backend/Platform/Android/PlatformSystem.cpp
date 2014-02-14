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

#include <ChilliSource/Platform/Android/PlatformSystem.h>
#include <ChilliSource/Platform/Android/ImageLoader.h>
#include <ChilliSource/Platform/Android/Input/InputSystem.h>
#include <ChilliSource/Platform/Android/WebViewActivity.h>
#include <ChilliSource/Platform/Android/Video/VideoPlayerActivity.h>

#include <ChilliSource/Backend/Platform/Android/JavaInterface/JavaInterfaceManager.h>
#include <ChilliSource/Platform/Android/JavaInterface/CoreJavaInterface.h>
#include <ChilliSource/Platform/Android/FMOD/FMODSystem.h>
#include <ChilliSource/Platform/Android/FMOD/FMODAudioLoader.h>
#include <ChilliSource/Platform/Android/HttpConnectionSystem.h>
#include <ChilliSource/Platform/Android/FileIO/FileSystem.h>
#include <ChilliSource/Platform/Android/LocalNotificationScheduler.h>

#include <ChilliSource/Platform/Android/Social/Communications/EmailCompositionActivity.h>
#include <ChilliSource/Platform/Android/Social/Communications/SMSCompositionActivity.h>
#include <ChilliSource/Platform/Android/Social/Communications/ContactInformationProvider.h>

#include <ChilliSource/Networking/IAPSystem.h>

#include <ChilliSource/Rendering/Main/RenderSystem.h>
#include <ChilliSource/Rendering/Renderer.h>
#include <ChilliSource/Rendering/SpriteSheetLoader.h>
#include <ChilliSource/Rendering/XMLSpriteSheetLoader.h>
#include <ChilliSource/Rendering/MaterialLoader.h>
 #include <ChilliSource/Rendering/MaterialFactory.h>
#include <ChilliSource/Rendering/FontLoader.h>
#include <ChilliSource/Rendering/ComponentUpdaters/AnimatedMeshComponentUpdater.h>

#include <ChilliSource/Core/Main/Utils.h>
#include <ChilliSource/Core/File/FileSystem.h>
#include <ChilliSource/Core/Notifications/NotificationScheduler.h>
#include <ChilliSource/Core/Image/MoImageProvider.h>
#include <ChilliSource/Core/Image/ETC1ImageProvider.h>

#include <ChilliSource/Audio/Main/AudioSystem.h>
#include <ChilliSource/Audio/Main/AudioPlayer.h>

#include <ChilliSource/Rendering/GUI/GUIViewFactory.h>

#include <ChilliSource/Input/Main/InputSystem.h>

#include <ChilliSource/Backend/Rendering/OpenGL/RenderSystem.h>
#include <ChilliSource/Backend/Rendering/OpenGL/RenderCapabilities.h>

namespace moFlo 
{
	namespace AndroidPlatform
	{
		DEFINE_CREATABLE(IPlatformSystem, CPlatformSystem);

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
			Core::CApplication::SetFileSystem(new AndroidPlatform::CFileSystem());
			moFlo::CLogging::Init();
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
		Core::ISystem* CPlatformSystem::FindSystemImplementing(Core::InterfaceIDType inInterfaceID, const DYNAMIC_ARRAY<Core::SystemPtr>& inSystems) const
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
            GUI::CGUIViewFactory::RegisterDefaults();
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
		void CPlatformSystem::CreateDefaultSystems(DYNAMIC_ARRAY<Core::SystemPtr> & inaSystems)
		{
			//create the main systems
			Rendering::IRenderSystem* pRenderSystem = new OpenGL::CRenderSystem();
			inaSystems.push_back(Core::SystemPtr(pRenderSystem));
			Core::CApplication::SetRenderSystem(pRenderSystem);

			Input::IInputSystem* pInputSystem = new AndroidPlatform::CInputSystem();
			inaSystems.push_back(Core::SystemPtr(pInputSystem));
			Core::CApplication::SetInputSystem(pInputSystem);

			Audio::IAudioSystem* pAudioSystem = new AndroidPlatform::CFMODSystem();
			inaSystems.push_back(Core::SystemPtr(pAudioSystem));
			inaSystems.push_back(Core::SystemPtr(new AndroidPlatform::CFMODAudioLoader(pAudioSystem)));
			Core::CApplication::SetAudioSystem(pAudioSystem);

			//create other important systems
			OpenGL::CRenderCapabilities* pRenderCapabilities = new OpenGL::CRenderCapabilities();
			inaSystems.push_back(Core::SystemPtr(pRenderCapabilities));
			inaSystems.push_back(Core::SystemPtr(new AndroidPlatform::ImageLoader()));
			inaSystems.push_back(Core::SystemPtr(new CMoImageProvider()));
			inaSystems.push_back(Core::SystemPtr(new CETC1ImageProvider()));
			inaSystems.push_back(Core::SystemPtr(new Rendering::CSpriteSheetLoader()));
			inaSystems.push_back(Core::SystemPtr(new Rendering::CXMLSpriteSheetLoader()));
			inaSystems.push_back(Core::SystemPtr(new Rendering::CMaterialLoader(pRenderCapabilities)));
			inaSystems.push_back(Core::SystemPtr(new Rendering::CFontLoader()));
			inaSystems.push_back(Core::SystemPtr(new Rendering::CAnimatedMeshComponentUpdater()));
			inaSystems.push_back(Core::SystemPtr(new Rendering::CMaterialFactory()));

			//Initialise the render system
			Core::CApplication::GetRenderSystemPtr()->Init((u32)Core::CScreen::GetRawDimensions().x, (u32)Core::CScreen::GetRawDimensions().y);

			//Create the renderer
			Core::CApplication::SetRenderer(new Rendering::CRenderer(Core::CApplication::GetRenderSystemPtr()));

			//Initialise the input system
			if(Core::CApplication::GetInputSystemPtr() != NULL)
			{
				Core::CApplication::SetHasTouchInput((Core::CApplication::GetInputSystemPtr()->GetTouchScreenPtr() != NULL));
			}
		}
		//-------------------------------------------------
		/// Post Create Systems
		//-------------------------------------------------
		void CPlatformSystem::PostCreateSystems()
		{
			if(Core::CApplication::GetAudioSystemPtr() != NULL)
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
		Core::ISystem* CPlatformSystem::CreateAndAddSystemWithInterface(Core::InterfaceIDType inInterfaceID, DYNAMIC_ARRAY<Core::SystemPtr> & inaExistingSystems) const
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
		Core::ISystem * CPlatformSystem::CreateHttpConnectionSystem(DYNAMIC_ARRAY<Core::SystemPtr>& inSystems) const
		{
			return new AndroidPlatform::CHttpConnectionSystem();
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
		Core::CVector2 CPlatformSystem::GetScreenDimensions() const
		{
			Core::CVector2 Result;
			CoreJavaInterfacePtr pCoreJI = CJavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<CCoreJavaInterface>();
			Result.x = pCoreJI->GetScreenWidth();
			Result.y = pCoreJI->GetScreenHeight();

			s32 dwOrientation = pCoreJI->GetOrientation();
#ifdef DEBUG
			if(dwOrientation < 0)
				ERROR_LOG("CPlatformSystem::GetScreenDimensions() - Could not get orientation of device!");
#endif
			if(moFlo::Core::LANDSCAPE_RIGHT == dwOrientation)
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
			DYNAMIC_ARRAY<std::string> strLocaleBrokenUp = moFlo::Core::CStringUtils::Split(strLocale, "_", 0);

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
