/*
* File: WindowsPlatformSystem.cpp
* Date: 24/11/2010 2010 
* Description: 
*/

/*
* Author: Scott Downie
* Version: v 1.0
* Copyright ©2010 Tag Games Limited - All rights reserved 
*/

#define WIN32_LEAN_AND_MEAN

#include <ChilliSource/Platform/Windows/PlatformSystemWindows.h>
#include <ChilliSource/Platform/Windows/ImageLoaderWindows.h>
#include <ChilliSource/Platform/Windows/Input/InputSystemWindows.h>
#include <ChilliSource/Platform/Windows/FileIO/FileSystemWindows.h>
#include <ChilliSource/Platform/Windows/HttpConnectionSystemWindows.h>
#include <ChilliSource/Platform/Windows/FMOD/FMODSystemWindows.h>
#include <ChilliSource/Platform/Windows/FMOD/FMODAudioLoaderWindows.h>

#include <ChilliSource/RenderingPlatform/OpenGL/RenderSystem.h>
#include <ChilliSource/RenderingPlatform/OpenGL/RenderCapabilities.h>

#include <ChilliSource/Rendering/MaterialLoader.h>
#include <ChilliSource/Rendering/MaterialFactory.h>
#include <ChilliSource/Rendering/SpriteSheetLoader.h>
#include <ChilliSource/Rendering/XMLSpriteSheetLoader.h>
#include <ChilliSource/Rendering/FontLoader.h>
#include <ChilliSource/Rendering/Renderer.h>
#include <ChilliSource/Rendering/ComponentUpdaters/AnimatedMeshComponentUpdater.h>

#include <ChilliSource/Audio/AudioLoader.h>
#include <ChilliSource/Audio/AudioPlayer.h>

#include <ChilliSource/Core/Application.h>
#include <ChilliSource/Core/MoImageProvider.h>

#include <ChilliSource/GUI/GUIViewFactory.h>

#include <windows.h>

namespace moFlo 
{
	DEFINE_CREATABLE(IPlatformSystem, WindowsPlatform::CPlatformSystem);

	//This is global as LARGE_INTEGER is defined in windows.h. Including windows.h in PlatformSystemWindows.h will cause compiler errors
	//in FileSystemWindows.h
	LARGE_INTEGER gFrequency;

	namespace WindowsPlatform
	{
		//-----------------------------------------
		/// Constructor
		///
		/// Default
		//-----------------------------------------
		CPlatformSystem::CPlatformSystem() : mbIsRunning(true), mbIsSuspended(false), muddwAppStartTime(0), mffAppPreviousTime(0.0)
		{
			//CNotificationScheduler::Initialise(new CLocalNotificationScheduler(), new CRemoteNotificationScheduler());
			Core::CApplication::SetFileSystem(new WindowsPlatform::CFileSystem());
			moFlo::CLogging::Init();
		}
		//-----------------------------------------
		/// Init
		///
		/// Create the GLFW window
		//-----------------------------------------
		void CPlatformSystem::Init()
		{
			QueryPerformanceFrequency(&gFrequency);

			if(!glfwInit())
			{
				FATAL_LOG("Cannot initialise GLFW");
			}

			//Set the window based on the retina resolution
			if(glfwOpenWindow(960, 640, 8, 8, 8, 8, 8, 0, GLFW_WINDOW) != GL_TRUE)
			{
				glfwTerminate();
				FATAL_LOG("Cannot create GLFW window");
			}

			glfwSetWindowTitle("MoFlow");

			//Register callbacks
			glfwSetWindowSizeCallback((GLFWwindowsizefun)&CPlatformSystem::OnWindowResized);
			glfwSetWindowCloseCallback((GLFWwindowclosefun)&CPlatformSystem::OnWindowClosed);

            //Initialise GUI factory
            GUI::CGUIViewFactory::RegisterDefaults();
		}
		//-------------------------------------------------
		/// Add Default Systems
		///
		/// Adds default systems to the applications system
		/// list.
		///
		/// @param the system list
		//-------------------------------------------------
		void CPlatformSystem::CreateDefaultSystems(DYNAMIC_ARRAY<Core::SystemPtr> & inaSystems)
		{
			//create the main systems
			Rendering::IRenderSystem* pRenderSystem = new OpenGL::CRenderSystem();
			inaSystems.push_back(Core::SystemPtr(pRenderSystem));
			Core::CApplication::SetRenderSystem(pRenderSystem);

			Input::IInputSystem* pInputSystem = new WindowsPlatform::CInputSystem();
			inaSystems.push_back(Core::SystemPtr(pInputSystem));
			Core::CApplication::SetInputSystem(pInputSystem);

			Audio::IAudioSystem* pAudioSystem = new WindowsPlatform::CFMODSystem();
			inaSystems.push_back(Core::SystemPtr(pAudioSystem));
			inaSystems.push_back(Core::SystemPtr(new WindowsPlatform::CFMODAudioLoader(pAudioSystem)));
			Core::CApplication::SetAudioSystem(pAudioSystem);

			//create other important systems
			OpenGL::CRenderCapabilities* pRenderCapabilities = new OpenGL::CRenderCapabilities();
			inaSystems.push_back(Core::SystemPtr(pRenderCapabilities));
			inaSystems.push_back(Core::SystemPtr(new WindowsPlatform::ImageLoader()));
			inaSystems.push_back(Core::SystemPtr(new CMoImageProvider()));
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
		///
		/// Deals with anything that needs to be handled after
		/// creating the systems.
		///
		/// @param the system list
		//-------------------------------------------------
		void CPlatformSystem::PostCreateSystems()
		{
			if(Core::CApplication::GetAudioSystemPtr() != NULL)
			{
				Audio::CAudioPlayer::Init();
			}
		}
		//-----------------------------------------
		/// Run
		///
		/// Begin the game loop
		//-----------------------------------------
		void CPlatformSystem::Run()
		{
			muddwAppStartTime = (u64)glfwGetTime();

			while(mbIsRunning)
			{
				if(!mbIsSuspended)
				{
					f64 ffAppCurrentTime = glfwGetTime();

					f32 fDt = (f32)(ffAppCurrentTime - mffAppPreviousTime);
					u64 uddwAppRunningTime = ((u64)mffAppPreviousTime - muddwAppStartTime);

					//Update event
					Core::CApplication::OnFrameBegin(fDt, uddwAppRunningTime);

					mffAppPreviousTime = ffAppCurrentTime;
				}
			}
		}
		//-----------------------------------------
		/// Set Updater Active
		///
		/// Starts or stops the platforms 
		/// update loop.
		///
		/// @param Whether to end or begin
		//-----------------------------------------
		void CPlatformSystem::SetUpdaterActive(bool inbIsActive)
		{
			mbIsSuspended = !inbIsActive;
		}
		//--------------------------------------------
		/// Terminate Updater
		///
		/// Stops the update loop causing the 
		/// application to terminate
		//--------------------------------------------
		void CPlatformSystem::TerminateUpdater()
		{
			mbIsRunning = false;
		}
		//--------------------------------------------
		/// Can Create System With Interface 
		///
		/// @param Interface ID
		/// @param Whether system can be created
		//--------------------------------------------
		bool CPlatformSystem::CanCreateSystemWithInterface(Core::InterfaceIDType inInterfaceID) const
		{
			return	inInterfaceID == Networking::IHttpConnectionSystem::InterfaceID;
		}
		//--------------------------------------------
		/// Create and Add System With Interface
		///
		/// @param Interface ID
		/// @param Vector of exisiting systems to append
		/// @return Pointer to the given system or NULL
		//--------------------------------------------
		Core::ISystem* CPlatformSystem::CreateAndAddSystemWithInterface(Core::InterfaceIDType inInterfaceID, DYNAMIC_ARRAY<Core::SystemPtr>& inaExisitingSystems) const
		{
			//Check if it already exists to prevent multiples
			for(DYNAMIC_ARRAY<Core::SystemPtr>::const_iterator it = inaExisitingSystems.begin(); it != inaExisitingSystems.end(); ++it)
			{
				if((*it)->IsA(inInterfaceID))
				{
					return (*it).get();
				}
			}

			Core::ISystem* pSystem = NULL;

			if(inInterfaceID == Networking::IHttpConnectionSystem::InterfaceID)
			{
				pSystem = new CHttpConnectionSystem();
				inaExisitingSystems.push_back(Core::SystemPtr(pSystem));
			}

			return pSystem;
		}
		//--------------------------------------------
		/// Can Create Activity With Interface
		///
		/// @param Interface ID
		/// @return Whether activity can be created
		//--------------------------------------------
		bool CPlatformSystem::CanCreateActivityWithInterface(Core::InterfaceIDType inInterfaceID) const
		{
			return false;
		}
		//--------------------------------------------
		/// Create Activity With Interface
		///
		/// @param Interface ID
		/// @return Ownership of activity instance or NULL
		//--------------------------------------------
		IActivity* CPlatformSystem::CreateActivityWithInterface(Core::InterfaceIDType inInterfaceID) const
		{
			return NULL;
		}
		//--------------------------------------------
		/// Can Create Information Provider With Interface
		///
		/// @param Interface ID
		/// @return Whether provider can be created
		//--------------------------------------------
		bool CPlatformSystem::CanCreateInformationProviderWithInterface(Core::InterfaceIDType inInterfaceID) const
		{
			return false;
		}
		//--------------------------------------------
		/// Create Information Provider With Interface
		///
		/// @param Interface ID
		/// @return Ownership of provider instance or NULL
		//--------------------------------------------
		IInformationProvider* CPlatformSystem::CreateInformationProviderWithInterface(Core::InterfaceIDType inInterfaceID) const
		{
			return NULL;
		}
		//-----------------------------------------------------------------------------------------------------------
		/// Get Screen Dimensions
		///
		/// Retrieves the screen dimensions. These dimensions are always in the default orientation for the device.
		/// @return A CVector2 containing the screen size in it's x + y components
		//-----------------------------------------------------------------------------------------------------------
		Core::CVector2 CPlatformSystem::GetScreenDimensions() const
		{
			Core::CVector2 Result;

			s32 dwWidth, dwHeight = 0;
			glfwGetWindowSize(&dwWidth, &dwHeight);

			Result.x = (f32)dwWidth;
			Result.y = (f32)dwHeight;

			return Result;
		}
		//--------------------------------------------------------------
		/// Get OS Version
		///
		/// @return  String containing the OS version of the device
		//--------------------------------------------------------------
		std::string CPlatformSystem::GetOSVersion() const
		{
			OSVERSIONINFOEX osvi;
			ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX)); osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
			GetVersionEx((OSVERSIONINFO*) &osvi);
			return std::string(STRING_CAST((u32)osvi.dwMajorVersion) + "." + STRING_CAST((u32)osvi.dwMinorVersion));
		}
		//--------------------------------------------------------------
		/// Get Locale
		///
		/// Get the active language locale of the device
		/// @return Locale ID
		//--------------------------------------------------------------
		Core::CLocale CPlatformSystem::GetLocale() const
		{
			wchar_t localeName[LOCALE_NAME_MAX_LENGTH]={0};

			if(GetUserDefaultLocaleName(localeName, LOCALE_NAME_MAX_LENGTH * sizeof(wchar_t)))
			{
				if(wcscmp(localeName, L"en") == 0)	return Core::CLocale("en");
				if(wcscmp(localeName, L"fr") == 0)	return Core::CLocale("fr");
				if(wcscmp(localeName, L"it") == 0)  return Core::CLocale("it");
				if(wcscmp(localeName, L"de") == 0)  return Core::CLocale("de");
				if(wcscmp(localeName, L"es") == 0)  return Core::CLocale("es");
				if(wcscmp(localeName, L"jp") == 0)  return Core::CLocale("jp");
			}

			//Just default to english
			return Core::CLocale("en");
		}
		//--------------------------------------------------------------
		/// Get Device Model Name
		///
		/// @return The above information stringified
		//--------------------------------------------------------------
		std::string CPlatformSystem::GetDeviceModelName() const
		{
			return "Windows";
		}
		//--------------------------------------------------------------
		/// Get Device Model Type Name
		///
		/// @return The above information stringified
		//--------------------------------------------------------------
		std::string CPlatformSystem::GetDeviceModelTypeName() const
		{
			return "PC";
		}
		//--------------------------------------------------------------
		/// Get Device Manufacturer Name
		///
		/// @return The above information stringified
		//--------------------------------------------------------------
		std::string CPlatformSystem::GetDeviceManufacturerName() const
		{
			return "Microsoft";
		}
		//--------------------------------------------------------------
		/// Get Langauge
		///
		/// Get the active language of the device in locale format
		/// @return Locale ID
		//--------------------------------------------------------------
		Core::CLocale CPlatformSystem::GetLanguage() const
		{
			wchar_t localeName[LOCALE_NAME_MAX_LENGTH]={0};

			if(GetUserDefaultLocaleName(localeName, LOCALE_NAME_MAX_LENGTH * sizeof(wchar_t)))
			{
				if(wcscmp(localeName, L"en") == 0)	return Core::CLocale("en");
				if(wcscmp(localeName, L"fr") == 0)	return Core::CLocale("fr");
				if(wcscmp(localeName, L"it") == 0)  return Core::CLocale("it");
				if(wcscmp(localeName, L"de") == 0)  return Core::CLocale("de");
				if(wcscmp(localeName, L"es") == 0)  return Core::CLocale("es");
				if(wcscmp(localeName, L"jp") == 0)  return Core::CLocale("jp");
			}

			//Just default to english
			return Core::CLocale("en");
		}
		//-------------------------------------------------
		/// Get Screen Density
		///
		/// @return The density scale factor of the screen
		/// to convert from DIPS to physical pixels
		//-------------------------------------------------
		f32 CPlatformSystem::GetScreenDensity() const
		{
			return 1.0f;
		}
		//-------------------------------------------------
		/// Get Device ID
		///
		/// @return The UDID of the device
		//-------------------------------------------------
		std::string CPlatformSystem::GetDeviceID()
		{
			ERROR_LOG("CPlatformSystem::GetDeviceID() has not been implemented!");
			return "FAKE ID";
		}
		//--------------------------------------------------------------
		/// Get Number Of CPU Cores
		///
		/// @return The number of cores available
		//--------------------------------------------------------------
		u32 CPlatformSystem::GetNumberOfCPUCores() const
		{
			SYSTEM_INFO SysInfo;
			GetSystemInfo(&SysInfo);
			return SysInfo.dwNumberOfProcessors;
		}
		//--------------------------------------------------------------
		/// Get System Time
		///
		/// @return the current time in milliseconds
		//--------------------------------------------------------------
		u64 CPlatformSystem::GetSystemTimeMS() const
		{
			LARGE_INTEGER currentTime;
            QueryPerformanceCounter(&currentTime);
            return (u64)((currentTime.QuadPart) * 1000.0 / gFrequency.QuadPart);
		}
		//-------------------------------------------------
		/// Get Physical Screen Size
		///
		/// @return The physical size of the screen in
		/// inches.
		//-------------------------------------------------
		f32 CPlatformSystem::GetPhysicalScreenSize()
		{
			WARNING_LOG("GetPhysicalScreenSize() is not implemented on windows!");
			return 0.0f;
		}
		//--------------------------------------------------------------------------------------------------
		/// Make Toast
		///
		/// Display a toast notification with the given text
		///
		/// @param Text
		//--------------------------------------------------------------------------------------------------
		void CPlatformSystem::MakeToast(const UTF8String& instrText) const
		{
			WARNING_LOG("Toast not available on Windows");
		}
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
		void CPlatformSystem::ShowSystemConfirmDialog(u32 inudwID, const UTF8String& instrTitle, const UTF8String& instrMessage, const UTF8String& instrConfirm, const UTF8String& instrCancel) const
		{
			if(MessageBoxA(NULL, instrTitle.ToASCII().c_str(), instrMessage.ToASCII().c_str(), MB_OKCANCEL) == IDOK)
			{
				Core::CApplication::OnSystemConfirmDialogResult(inudwID, moFlo::SystemConfirmDialog::CONFIRM);
			}
			else
			{
				Core::CApplication::OnSystemConfirmDialogResult(inudwID, moFlo::SystemConfirmDialog::CANCEL);
			} 
		}
		//--------------------------------------------------------------------------------------------------
        /// Show System Dialog
        ///
        /// Display a system confirmation dialog with the given ID and delegate
        ///
        /// @param ID
        /// @param Title text
        /// @param Message text
        /// @param Confirm text
        //--------------------------------------------------------------------------------------------------
        void CPlatformSystem::ShowSystemDialog(u32 inudwID, const UTF8String& instrTitle, const UTF8String& instrMessage, const UTF8String& instrConfirm) const
		{
			MessageBoxA(NULL, instrTitle.ToASCII().c_str(), instrMessage.ToASCII().c_str(), MB_OK);
		}
		//---GLFW Delegates
		//-------------------------------------------------
		/// On Window Resized (GLFW)
		///
		/// Triggered when glfw resizes the window
		///
		/// @param Window width
		/// @param Window Height
		//-------------------------------------------------
		void CPlatformSystem::OnWindowResized(s32 indwWidth, s32 indwHeight)
		{
			Core::CApplication::OnScreenResized((u32)indwWidth, (u32)indwHeight);
		}
		//-------------------------------------------------
		/// On Window Closed (GLFW)
		///
		/// Triggered when glfw exits the window
		//-------------------------------------------------
		void CPlatformSystem::OnWindowClosed()
		{
			Core::CApplication::Suspend();
			Core::CApplication::Quit();
			glfwTerminate();
		}
		//-----------------------------------------
		/// Destructor
		///
		/// 
		//-----------------------------------------
		CPlatformSystem::~CPlatformSystem()
		{
			glfwTerminate();
		}
	}
}
