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

#include <ChilliSource/Backend/Platform/Windows/Core/Base/PlatformSystem.h>

#include <ChilliSource/Audio/Base/AudioLoader.h>
#include <ChilliSource/Audio/Base/AudioPlayer.h>
#include <ChilliSource/Backend/Audio/FMOD/Base/AudioLoader.h>
#include <ChilliSource/Backend/Audio/FMOD/Base/FMODSystem.h>
#include <ChilliSource/Backend/Platform/Windows/Core/DialogueBox/DialogueBoxSystem.h>
#include <ChilliSource/Backend/Platform/Windows/Core/File/FileSystem.h>
#include <ChilliSource/Backend/Platform/Windows/Core/Image/ImageLoader.h>
#include <ChilliSource/Backend/Platform/Windows/Input/Base/InputSystem.h>
#include <ChilliSource/Backend/Platform/Windows/Networking/Http/HttpConnectionSystem.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Image/MoImageProvider.h>
#include <ChilliSource/GUI/Base/GUIViewFactory.h>
#include <ChilliSource/Rendering/Base/Renderer.h>
#include <ChilliSource/Rendering/Font/FontLoader.h>
#include <ChilliSource/Rendering/Material/MaterialLoader.h>
#include <ChilliSource/Rendering/Material/MaterialFactory.h>
#include <ChilliSource/Rendering/Model/AnimatedMeshComponentUpdater.h>
#include <ChilliSource/Rendering/Sprite/SpriteSheetLoader.h>
#include <ChilliSource/Rendering/Sprite/XMLSpriteSheetLoader.h>

#include <windows.h>

//As the opengl classes need to include glfw.h, they need to be included after windows.h to avoid macro redefinitions.
#include <ChilliSource/Backend/Rendering/OpenGL/Base/RenderCapabilities.h>
#include <ChilliSource/Backend/Rendering/OpenGL/Base/RenderSystem.h>

namespace ChilliSource 
{
	//This is global as LARGE_INTEGER is defined in windows.h. Including windows.h in PlatformSystemWindows.h will cause compiler errors
	//in FileSystemWindows.h
	LARGE_INTEGER gFrequency;

	namespace Windows
	{
		//-----------------------------------------
		/// Constructor
		///
		/// Default
		//-----------------------------------------
		PlatformSystem::PlatformSystem() : mbIsRunning(true), mbIsSuspended(false), muddwAppStartTime(0), mffAppPreviousTime(0.0)
		{
			//CNotificationScheduler::Initialise(new CLocalNotificationScheduler(), new CRemoteNotificationScheduler());
			Core::Application::Get()->SetFileSystem(new Windows::FileSystem());
			Core::Logging::Init();
		}
		//-----------------------------------------
		/// Init
		///
		/// Create the GLFW window
		//-----------------------------------------
		void PlatformSystem::Init()
		{
			QueryPerformanceFrequency(&gFrequency);

			if(!glfwInit())
			{
				CS_LOG_FATAL("Cannot initialise GLFW");
			}

			//Set the window based on the retina resolution
			if(glfwOpenWindow(960, 640, 8, 8, 8, 8, 8, 0, GLFW_WINDOW) != GL_TRUE)
			{
				glfwTerminate();
				CS_LOG_FATAL("Cannot create GLFW window");
			}

			glfwSetWindowTitle("MoFlow");

			//Register callbacks
			glfwSetWindowSizeCallback((GLFWwindowsizefun)&PlatformSystem::OnWindowResized);
			glfwSetWindowCloseCallback((GLFWwindowclosefun)&PlatformSystem::OnWindowClosed);

            //Initialise GUI factory
            GUI::GUIViewFactory::RegisterDefaults();
		}
		//-------------------------------------------------
		/// Add Default Systems
		///
		/// Adds default systems to the applications system
		/// list.
		///
		/// @param the system list
		//-------------------------------------------------
		void PlatformSystem::CreateDefaultSystems(std::vector<Core::SystemSPtr> & inaSystems)
		{
			//create the main systems
			OpenGL::RenderSystem* pRenderSystem = new OpenGL::RenderSystem();
			inaSystems.push_back(Core::SystemSPtr(pRenderSystem));
			Core::Application::Get()->SetRenderSystem(pRenderSystem);

			Input::InputSystem* pInputSystem = new Windows::InputSystem();
			inaSystems.push_back(Core::SystemSPtr(pInputSystem));
			Core::Application::Get()->SetInputSystem(pInputSystem);

			Audio::AudioSystem* pAudioSystem = new FMOD::FMODSystem();
			inaSystems.push_back(Core::SystemSPtr(pAudioSystem));
			inaSystems.push_back(Core::SystemSPtr(new FMOD::AudioLoader(pAudioSystem)));
			Core::Application::Get()->SetAudioSystem(pAudioSystem);

			//create other important systems
			OpenGL::RenderCapabilities* pRenderCapabilities = new OpenGL::RenderCapabilities();
			inaSystems.push_back(Core::SystemSPtr(pRenderCapabilities));
			inaSystems.push_back(Core::SystemSPtr(new Windows::ImageLoader()));
			inaSystems.push_back(Core::SystemSPtr(new Core::MoImageProvider()));
			inaSystems.push_back(Core::SystemSPtr(new Rendering::SpriteSheetLoader()));
			inaSystems.push_back(Core::SystemSPtr(new Rendering::XMLSpriteSheetLoader()));
			inaSystems.push_back(Core::SystemSPtr(new Rendering::MaterialLoader(pRenderCapabilities)));
			inaSystems.push_back(Core::SystemSPtr(new Rendering::FontLoader()));
			inaSystems.push_back(Core::SystemSPtr(new Rendering::AnimatedMeshComponentUpdater()));
			inaSystems.push_back(Core::SystemSPtr(new Rendering::MaterialFactory(pRenderSystem->GetTextureManager(), pRenderSystem->GetShaderManager(), pRenderSystem->GetCubemapManager(), pRenderCapabilities)));
			inaSystems.push_back(Core::SystemSPtr(Core::DialogueBoxSystem::Create()));

			//Initialise the render system
			Core::Application::Get()->GetRenderSystem()->Init((u32)Core::Screen::GetRawDimensions().x, (u32)Core::Screen::GetRawDimensions().y);
            
			//Create the renderer
			Core::Application::SetRenderer(new Rendering::Renderer(Core::Application::GetRenderSystemPtr()));
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
			if(Core::Application::GetAudioSystemPtr() != NULL)
			{
				Audio::AudioPlayer::Init();
			}
		}
		//-----------------------------------------
		/// Run
		///
		/// Begin the game loop
		//-----------------------------------------
		void PlatformSystem::Run()
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
					Core::Application::Get()->OnUpdate(fDt, uddwAppRunningTime);

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
		void PlatformSystem::SetUpdaterActive(bool inbIsActive)
		{
			mbIsSuspended = !inbIsActive;
		}
		//--------------------------------------------
		/// Terminate Updater
		///
		/// Stops the update loop causing the 
		/// application to terminate
		//--------------------------------------------
		void PlatformSystem::TerminateUpdater()
		{
			mbIsRunning = false;
		}
		//--------------------------------------------
		/// Can Create System With Interface 
		///
		/// @param Interface ID
		/// @param Whether system can be created
		//--------------------------------------------
		bool PlatformSystem::CanCreateSystemWithInterface(Core::InterfaceIDType inInterfaceID) const
		{
			return	inInterfaceID == Networking::HttpConnectionSystem::InterfaceID;
		}
		//--------------------------------------------
		/// Create and Add System With Interface
		///
		/// @param Interface ID
		/// @param Vector of exisiting systems to append
		/// @return Pointer to the given system or nullptr
		//--------------------------------------------
		Core::System* PlatformSystem::CreateAndAddSystemWithInterface(Core::InterfaceIDType inInterfaceID, std::vector<Core::SystemSPtr>& inaExisitingSystems) const
		{
			//Check if it already exists to prevent multiples
			for(std::vector<Core::SystemSPtr>::const_iterator it = inaExisitingSystems.begin(); it != inaExisitingSystems.end(); ++it)
			{
				if((*it)->IsA(inInterfaceID))
				{
					return (*it).get();
				}
			}

			Core::System* pSystem = nullptr;

			if(inInterfaceID == Networking::HttpConnectionSystem::InterfaceID)
			{
				pSystem = new HttpConnectionSystem();
				inaExisitingSystems.push_back(Core::SystemSPtr(pSystem));
			}

			return pSystem;
		}
		//--------------------------------------------
		/// Can Create Activity With Interface
		///
		/// @param Interface ID
		/// @return Whether activity can be created
		//--------------------------------------------
		bool PlatformSystem::CanCreateActivityWithInterface(Core::InterfaceIDType inInterfaceID) const
		{
			return false;
		}
		//--------------------------------------------
		/// Create Activity With Interface
		///
		/// @param Interface ID
		/// @return Ownership of activity instance or nullptr
		//--------------------------------------------
		Core::Activity* PlatformSystem::CreateActivityWithInterface(Core::InterfaceIDType inInterfaceID) const
		{
			return nullptr;
		}
		//--------------------------------------------
		/// Can Create Information Provider With Interface
		///
		/// @param Interface ID
		/// @return Whether provider can be created
		//--------------------------------------------
		bool PlatformSystem::CanCreateInformationProviderWithInterface(Core::InterfaceIDType inInterfaceID) const
		{
			return false;
		}
		//--------------------------------------------
		/// Create Information Provider With Interface
		///
		/// @param Interface ID
		/// @return Ownership of provider instance or nullptr
		//--------------------------------------------
		Core::IInformationProvider* PlatformSystem::CreateInformationProviderWithInterface(Core::InterfaceIDType inInterfaceID) const
		{
			return nullptr;
		}
		//-----------------------------------------------------------------------------------------------------------
		/// Get Screen Dimensions
		///
		/// Retrieves the screen dimensions. These dimensions are always in the default orientation for the device.
		/// @return A Vector2 containing the screen size in it's x + y components
		//-----------------------------------------------------------------------------------------------------------
		Core::Vector2 PlatformSystem::GetScreenDimensions() const
		{
			Core::Vector2 Result;

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
		std::string PlatformSystem::GetOSVersion() const
		{
			OSVERSIONINFOEX osvi;
			ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX)); osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
			GetVersionEx((OSVERSIONINFO*) &osvi);
			return std::string(Core::ToString((u32)osvi.dwMajorVersion) + "." + Core::ToString((u32)osvi.dwMinorVersion));
		}
		//--------------------------------------------------------------
		/// Get Locale
		///
		/// Get the active language locale of the device
		/// @return Locale ID
		//--------------------------------------------------------------
		Core::Locale PlatformSystem::GetLocale() const
		{
			wchar_t localeName[LOCALE_NAME_MAX_LENGTH]={0};

			if(GetUserDefaultLocaleName(localeName, LOCALE_NAME_MAX_LENGTH * sizeof(wchar_t)))
			{
				if(wcscmp(localeName, L"en") == 0)	return Core::Locale("en");
				if(wcscmp(localeName, L"fr") == 0)	return Core::Locale("fr");
				if(wcscmp(localeName, L"it") == 0)  return Core::Locale("it");
				if(wcscmp(localeName, L"de") == 0)  return Core::Locale("de");
				if(wcscmp(localeName, L"es") == 0)  return Core::Locale("es");
				if(wcscmp(localeName, L"jp") == 0)  return Core::Locale("jp");
			}

			//Just default to english
			return Core::Locale("en");
		}
		//--------------------------------------------------------------
		/// Get Device Model Name
		///
		/// @return The above information stringified
		//--------------------------------------------------------------
		std::string PlatformSystem::GetDeviceModelName() const
		{
			return "Windows";
		}
		//--------------------------------------------------------------
		/// Get Device Model Type Name
		///
		/// @return The above information stringified
		//--------------------------------------------------------------
		std::string PlatformSystem::GetDeviceModelTypeName() const
		{
			return "PC";
		}
		//--------------------------------------------------------------
		/// Get Device Manufacturer Name
		///
		/// @return The above information stringified
		//--------------------------------------------------------------
		std::string PlatformSystem::GetDeviceManufacturerName() const
		{
			return "Microsoft";
		}
		//--------------------------------------------------------------
		/// Get Langauge
		///
		/// Get the active language of the device in locale format
		/// @return Locale ID
		//--------------------------------------------------------------
		Core::Locale PlatformSystem::GetLanguage() const
		{
			wchar_t localeName[LOCALE_NAME_MAX_LENGTH]={0};

			if(GetUserDefaultLocaleName(localeName, LOCALE_NAME_MAX_LENGTH * sizeof(wchar_t)))
			{
				if(wcscmp(localeName, L"en") == 0)	return Core::Locale("en");
				if(wcscmp(localeName, L"fr") == 0)	return Core::Locale("fr");
				if(wcscmp(localeName, L"it") == 0)  return Core::Locale("it");
				if(wcscmp(localeName, L"de") == 0)  return Core::Locale("de");
				if(wcscmp(localeName, L"es") == 0)  return Core::Locale("es");
				if(wcscmp(localeName, L"jp") == 0)  return Core::Locale("jp");
			}

			//Just default to english
			return Core::Locale("en");
		}
		//-------------------------------------------------
		/// Get Screen Density
		///
		/// @return The density scale factor of the screen
		/// to convert from DIPS to physical pixels
		//-------------------------------------------------
		f32 PlatformSystem::GetScreenDensity() const
		{
			return 1.0f;
		}
		//-------------------------------------------------
		/// Get Device ID
		///
		/// @return The UDID of the device
		//-------------------------------------------------
		std::string PlatformSystem::GetDeviceID()
		{
			CS_LOG_ERROR("PlatformSystem::GetDeviceID() has not been implemented!");
			return "FAKE ID";
		}
		//--------------------------------------------------------------
		/// Get Number Of CPU Cores
		///
		/// @return The number of cores available
		//--------------------------------------------------------------
		u32 PlatformSystem::GetNumberOfCPUCores() const
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
		u64 PlatformSystem::GetSystemTimeMS() const
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
		f32 PlatformSystem::GetPhysicalScreenSize()
		{
			CS_LOG_WARNING("GetPhysicalScreenSize() is not implemented on windows!");
			return 0.0f;
		}
		//--------------------------------------------------------------------------------------------------
		/// Make Toast
		///
		/// Display a toast notification with the given text
		///
		/// @param Text
		//--------------------------------------------------------------------------------------------------
		void PlatformSystem::MakeToast(const Core::UTF8String& instrText) const
		{
			CS_LOG_WARNING("Toast not available on Windows");
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
		void PlatformSystem::ShowSystemConfirmDialog(u32 inudwID, const Core::UTF8String& instrTitle, const Core::UTF8String& instrMessage, const Core::UTF8String& instrConfirm, const Core::UTF8String& instrCancel) const
		{
			if(MessageBoxA(NULL, instrTitle.ToASCII().c_str(), instrMessage.ToASCII().c_str(), MB_OKCANCEL) == IDOK)
			{
				Core::Application::OnSystemConfirmDialogResult(inudwID, Core::SystemConfirmDialog::Result::k_confirm);
			}
			else
			{
				Core::Application::OnSystemConfirmDialogResult(inudwID, Core::SystemConfirmDialog::Result::k_cancel);
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
		void PlatformSystem::ShowSystemDialog(u32 inudwID, const Core::UTF8String& instrTitle, const Core::UTF8String& instrMessage, const Core::UTF8String& instrConfirm) const
		{
			MessageBoxA(nullptr, instrTitle.ToASCII().c_str(), instrMessage.ToASCII().c_str(), MB_OK);
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
		void PlatformSystem::OnWindowResized(s32 indwWidth, s32 indwHeight)
		{
			Core::Application::Get()->OnScreenResized((u32)indwWidth, (u32)indwHeight);
		}
		//-------------------------------------------------
		/// On Window Closed (GLFW)
		///
		/// Triggered when glfw exits the window
		//-------------------------------------------------
		void PlatformSystem::OnWindowClosed()
		{
			Core::Application::Get()->OnSuspend();
			Core::Application::Get()->Quit();
			glfwTerminate();
		}
		//-----------------------------------------
		/// Destructor
		///
		/// 
		//-----------------------------------------
		PlatformSystem::~PlatformSystem()
		{
			glfwTerminate();
		}
	}
}
