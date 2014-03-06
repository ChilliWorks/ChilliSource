/*
 *  moFloApplication.cpp
 *  moFlo
 *
 *  Created by Scott Downie on 23/09/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Core/Base/Application.h>

#include <ChilliSource/Audio/Base/AudioSystem.h>

#include <ChilliSource/Core/Base/ApplicationEvents.h>
#include <ChilliSource/Core/DialogueBox/DialogueBoxSystem.h>
#include <ChilliSource/Core/System/SystemConcepts.h>
#include <ChilliSource/Core/Resource/ResourceProvider.h>
#include <ChilliSource/Core/Entity/ComponentFactory.h>
#include <ChilliSource/Core/Time/CoreTimer.h>
#include <ChilliSource/Core/File/TweakableConstants.h>
#include <ChilliSource/Core/File/LocalDataStore.h>
#include <ChilliSource/Core/Localisation/LocalisedText.h>
#include <ChilliSource/Core/State/StateManager.h>
#include <ChilliSource/Core/Resource/ResourceGroupManager.h>
#include <ChilliSource/Core/Base/Screen.h>
#include <ChilliSource/Core/Base/Logging.h>
#include <ChilliSource/Core/Base/Device.h>
#include <ChilliSource/Core/System/System.h>
#include <ChilliSource/Core/Base/Utils.h>
#include <ChilliSource/Core/Resource/ResourceManager.h>
#include <ChilliSource/Core/Resource/ResourceManagerDispenser.h>
#include <ChilliSource/Core/Entity/ComponentFactoryDispenser.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>
#include <ChilliSource/Core/Base/PlatformSystem.h>
#include <ChilliSource/Core/Notifications/NotificationScheduler.h>
#include <ChilliSource/Core/Math/MathUtils.h>

#include <ChilliSource/Rendering/Material/Material.h>
#include <ChilliSource/Rendering/Font/Font.h>
#include <ChilliSource/Rendering/Model/Mesh.h>
#include <ChilliSource/Rendering/Base/Renderer.h>
#include <ChilliSource/Rendering/Base/RenderSystem.h>
#include <ChilliSource/Rendering/Camera/CameraComponent.h>

#include <ChilliSource/Input/Base/InputSystem.h>

#include <ctime>

namespace ChilliSource
{
	namespace Core
	{
        namespace
        {
            const f32 k_defaultUpdateInterval = 1.0f/60.0f;
            const f32 k_updateClampThreshold = 0.33f;
            const f32 k_updateIntervalMax = k_updateClampThreshold;
            
            //----------------------------------------------------
            /// TODO:
            //----------------------------------------------------
            bool ResolutionSortPredicate(const ResourceDirectoryInfo& in_lhs, const ResourceDirectoryInfo& in_rhs)
            {
                return in_lhs.m_maxRes < in_rhs.m_maxRes;
            }
        }
        
        Application* Application::s_application = nullptr;
        
        //----------------------------------------------------
        //----------------------------------------------------
        Application* Application::Get()
        {
            return s_application;
        }
        //----------------------------------------------------
        //----------------------------------------------------
		Application::Application()
        : m_currentAppTime(0), m_updateInterval(k_defaultUpdateInterval), m_updateSpeed(1.0f), m_renderSystem(nullptr), m_inputSystem(nullptr), m_audioSystem(nullptr),
        m_renderer(nullptr), m_fileSystem(nullptr), m_defaultOrientation(ScreenOrientation::k_landscapeRight), m_resourceManagerDispenser(nullptr), m_updateIntervalRemainder(0.0f),
        m_shouldNotifyConnectionsResumeEvent(false), m_isFirstFrame(true), m_isSuspending(false), m_isSystemCreationAllowed(false)
		{
		}
        //----------------------------------------------------
        //----------------------------------------------------
		std::string Application::GetAppVersion()
		{
			return m_platformSystem->GetAppVersion();
		}
        //----------------------------------------------------
        //----------------------------------------------------
		TimeIntervalSecs Application::GetAppElapsedTime()
		{
			return m_currentAppTime;
		}
        //----------------------------------------------------
        //----------------------------------------------------
        TimeIntervalSecs Application::GetSystemTime()
        {
            return time(0);
        }
        //----------------------------------------------------
        //----------------------------------------------------
		TimeIntervalMs Application::GetSystemTimeInMilliseconds()
		{
			return m_platformSystem->GetSystemTimeMS();
		}
        //----------------------------------------------------
        //----------------------------------------------------
		void Application::SetUpdateInterval(f32 infUpdateInterval)
		{
			m_updateInterval = infUpdateInterval;
		}
        //----------------------------------------------------
        //----------------------------------------------------
		f32 Application::GetUpdateInterval()
		{
			return m_updateInterval;
		}
        //----------------------------------------------------
        //----------------------------------------------------
		f32 Application::GetUpdateIntervalMax()
		{
			return k_updateIntervalMax;
		}
        //----------------------------------------------------
        //----------------------------------------------------
        void Application::SetUpdateSpeed(f32 infSpeed)
        {
            m_updateSpeed = infSpeed;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        void Application::Quit()
        {
            m_platformSystem->TerminateUpdater();
        }
        //----------------------------------------------------
        //----------------------------------------------------
        const Rendering::FontSPtr& Application::GetDefaultFont()
        {
            return m_defaultFont;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        const Rendering::MeshSPtr& Application::GetDefaultMesh()
        {
            return m_defaultMesh;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        const Rendering::MaterialSPtr& Application::GetDefaultMaterial()
        {
            return m_defaultMaterial;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        StateManager* Application::GetStateManager()
        {
            return &m_stateManager;
        }
        //-----------------------------------------------------
        //-----------------------------------------------------
        Rendering::Renderer* Application::GetRenderer()
        {
            return m_renderer;
        }
        //-----------------------------------------------------
        //-----------------------------------------------------
        Rendering::RenderSystem* Application::GetRenderSystem()
        {
            return m_renderSystem;
        }
        //-----------------------------------------------------
        //-----------------------------------------------------
        PlatformSystem* Application::GetPlatformSystem()
        {
            return m_platformSystem.get();
        }
        //-----------------------------------------------------
        //-----------------------------------------------------
        Input::InputSystem* Application::GetInputSystem()
        {
            return m_inputSystem;
        }
        //-----------------------------------------------------
        //-----------------------------------------------------
        Audio::AudioSystem* Application::GetAudioSystem()
        {
            return m_audioSystem;
        }
        //-----------------------------------------------------
        //-----------------------------------------------------
        FileSystem* Application::GetFileSystem()
        {
            return m_fileSystem;
        }
        //-----------------------------------------------------
        //-----------------------------------------------------
        void Application::SetRenderer(Rendering::Renderer* in_system)
        {
            m_renderer = in_system;
        }
        //----------------------------------------------------
        //----------------------------------------------------
		void Application::OnInitialise()
		{
            CS_ASSERT(s_application == nullptr, "Application already initialised!");
            s_application = this;
            
            m_resourceManagerDispenser = new ResourceManagerDispenser(this);
            m_componentFactoryDispenser = new ComponentFactoryDispenser(this);
            m_stateManager.SetOwningApplication(this);
            
#ifdef CS_TARGETPLATFORM_WINDOWS
			//Because windows by default is landscape, this needs to be flipped.
			m_defaultOrientation = Core::ScreenOrientation::k_portraitUp;
#endif
            
			//Initialise the platform specific API's
            m_platformSystem = PlatformSystem::Create();
			m_platformSystem->Init();
            
			//Set the screen helper classes dimensions
            Core::Screen::SetRawDimensions(m_platformSystem->GetScreenDimensions());
            Core::Screen::SetOrientation(m_defaultOrientation);
            Core::Screen::SetDensity(m_platformSystem->GetScreenDensity());
            
            DetermineResourceDirectories();
            
            //Set up the device helper
            Device::Init(m_platformSystem.get());
            
			//Set up the task scheduler
			TaskScheduler::Init(Core::Device::GetNumCPUCores() * 2);
            
			//System setup
            m_isSystemCreationAllowed = true;
            CreateDefaultSystems();
			m_platformSystem->CreateDefaultSystems(this);
			CreateSystems();
            m_isSystemCreationAllowed = false;
			PostCreateSystems();
            
            //init tweakable constants and local data store.
			new TweakableConstants();
			new LocalDataStore();
            
            LoadDefaultResources();
			OnScreenChangedOrientation(m_defaultOrientation);
            
            Initialise();
            
			if (m_stateManager.GetActiveScenePtr() == nullptr)
            {
				PushInitialState();
            }
            
			//Register for update events
			LocalDataStore::GetSingleton().SubscribeToApplicationSuspendEvent();
            
			//Begin the update loop
			m_platformSystem->Run();
		}
        //----------------------------------------------------
        //----------------------------------------------------
		void Application::OnResume()
		{
            m_shouldNotifyConnectionsResumeEvent = true;
            
			//We must restart the application timer. This will automatically restart system updates
			m_platformSystem->SetUpdaterActive(true);
		}
        //----------------------------------------------------
        //----------------------------------------------------
		void Application::OnUpdate(f32 in_deltaTime, TimeIntervalSecs in_timestamp)
		{
            if(m_shouldNotifyConnectionsResumeEvent == true)
			{
				m_shouldNotifyConnectionsResumeEvent = false;
				ResumeApplication();
			}
            
            if(m_isSuspending)
            {
                // Updating after told to suspend so early out
                return;
            }
            
#ifdef CS_ENABLE_DEBUGSTATS
            Debugging::DebugStats::RecordEvent("FrameTime", infDt);
			Debugging::DebugStats::RecordEvent("FPS", 1.0f/infDt);
#endif
            
			//Update the app time since start
			m_currentAppTime = in_timestamp;
            
			TaskScheduler::ExecuteMainThreadTasks();
            
            //We do not need to render as often as we update so this callback will be triggered
            //less freqenctly than the update frequency suggests. We must work out how many times to update based on the time since last frame
            //and our actual update frequency. We carry the remainder to the next frame until we have a full update cycle
            m_updateIntervalRemainder = MathUtils::Min(m_updateIntervalRemainder + in_deltaTime, GetUpdateIntervalMax());
            
			//Force the input system to distribute any buffered input
			if(m_inputSystem != nullptr)
			{
				m_inputSystem->FlushBufferedInput();
			}
            
            while((m_updateIntervalRemainder >= Application::GetUpdateInterval()) || m_isFirstFrame)
            {
                m_updateIntervalRemainder -=  Application::GetUpdateInterval();
                m_stateManager.FixedUpdate(Application::GetUpdateInterval());
                
                m_isFirstFrame = false;
            }
            
            //Tell the state manager to update the active state
            Update(in_deltaTime);
            
            //Render the scene
            m_renderer->RenderToScreen(m_stateManager.GetActiveScenePtr());
            
#ifdef CS_ENABLE_DEBUGSTATS
			DebugStats::Clear();
#endif
		}
        //----------------------------------------------------
        //----------------------------------------------------
		void Application::OnScreenChangedOrientation(ScreenOrientation in_orientation)
		{
			Screen::SetOrientation(in_orientation);
            
			if(m_renderSystem)
			{
				m_renderSystem->OnScreenOrientationChanged(Core::Screen::GetOrientedWidth(), Core::Screen::GetOrientedHeight());
			}
            
			//Flip the screen
			SetOrientation(in_orientation);
			ApplicationEvents::GetScreenOrientationChangedEvent().NotifyConnections(in_orientation);
            
			CS_LOG_DEBUG("Screen Oriented Notification");
		}
        //----------------------------------------------------
        //----------------------------------------------------
		void Application::OnScreenResized(u32 in_width, u32 in_height)
		{
			Screen::SetRawDimensions(Core::Vector2((f32)in_width, (f32)in_height));

			if (m_renderSystem)
			{
				m_renderSystem->OnScreenOrientationChanged(in_width, in_height);
			}

			if (m_inputSystem != nullptr)
			{
				Input::TouchScreen * pTouchScreen = m_inputSystem->GetTouchScreen();
				if (pTouchScreen != nullptr)
				{
					pTouchScreen->SetScreenHeight(Screen::GetOrientedHeight());
				}
			}
			ApplicationEvents::GetScreenResizedEvent().NotifyConnections(in_width, in_height);
            
			CS_LOG_DEBUG("Screen resized Notification");
		}
        //----------------------------------------------------
        //----------------------------------------------------
		void Application::OnApplicationMemoryWarning()
		{
			CS_LOG_DEBUG("Memory Warning. Clearing resource cache...");
			ResourceManagerDispenser::GetSingletonPtr()->FreeResourceCaches();
			ApplicationEvents::GetLowMemoryEvent().NotifyConnections();
		}
        //----------------------------------------------------
        //----------------------------------------------------
		void Application::OnGoBack()
		{
			CS_LOG_DEBUG("Go back event.");
			m_stateManager.GetActiveState()->OnGoBack();
			ApplicationEvents::GetGoBackEvent().NotifyConnections();
		}
        //----------------------------------------------------
        //----------------------------------------------------
		void Application::OnSuspend()
		{
            CS_LOG_DEBUG("App Suspending...");
            
			m_isSuspending = true;
            
			//Tell the active state to save it's data etc
			m_stateManager.Pause();
            
			//We must invalidate the application timer. This will stop sub-system updates
			m_platformSystem->SetUpdaterActive(false);
            
			//We need to rebind or rebuild the context if it was stolen
			if(m_renderSystem)
			{
				m_renderSystem->Suspend();
			}
            
			ApplicationEvents::GetSuspendEvent().NotifyConnections();
			ApplicationEvents::GetLateSuspendEvent().NotifyConnections();
			
			CS_LOG_DEBUG("App Finished Suspending...");
		}
        //----------------------------------------------------
        //----------------------------------------------------
        void Application::OnDestroy()
        {
            Destroy();

            m_stateManager.DestroyAll();

			m_defaultFont.reset();
			m_defaultMesh.reset();
			m_defaultMaterial.reset();
            
            m_platformSystem.reset();
            CS_SAFEDELETE(m_resourceManagerDispenser);
            CS_SAFEDELETE(m_componentFactoryDispenser);
            
			//We have an issue with the order of destruction of systems.
			while(m_systems.empty() == false)
			{
				m_systems.pop_back();
			}
            
            s_application = nullptr;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        void Application::AddSystem(SystemUPtr in_system)
        {
            CS_ASSERT(m_isSystemCreationAllowed == true, "Application systems cannot be created outwith the creation phase");
            m_systems.push_back(std::move(in_system));
        }
        //----------------------------------------------------
        //----------------------------------------------------
		System* Application::GetSystem(InterfaceIDType in_interfaceID)
		{
			for (std::vector<SystemUPtr>::const_iterator it = m_systems.begin(); it != m_systems.end(); ++it)
			{
				if ((*it)->IsA(in_interfaceID))
				{
					return (*it).get();
				}
			}
			
			CS_LOG_WARNING("Application cannot find implementing systems");
			return nullptr;
		}
        //------------------------------------------------------
        //------------------------------------------------------
        void Application::CreateDefaultSystems()
        {
            AddSystem(DialogueBoxSystem::Create());
        }
        //----------------------------------------------------
        //----------------------------------------------------
		void Application::PostCreateSystems()
		{
            //Loop round all the created systems and categorise them
			for(std::vector<SystemUPtr>::iterator it = m_systems.begin(); it != m_systems.end(); ++it)
			{
                System* pSystem = it->get();
                
                //Updateables
				if(pSystem->IsA(IUpdateable::InterfaceID))
				{
					m_updateableSystems.push_back(pSystem->GetInterface<IUpdateable>());
				}
                
                //Component producers
				if(pSystem->IsA(IComponentProducer::InterfaceID))
				{
                    IComponentProducer* pProducer = pSystem->GetInterface<IComponentProducer>();
                    u32 udwNumFactoriesInSystem = pProducer->GetNumComponentFactories();

                    for(u32 i=0; i<udwNumFactoriesInSystem; ++i)
                    {
                        m_componentFactoryDispenser->RegisterComponentFactory(pProducer->GetComponentFactoryPtr(i));
                    }
				}
                
                //Resource providers
				if(pSystem->IsA(ResourceProvider::InterfaceID))
				{
					m_resourceProviders.push_back(pSystem->GetInterface<ResourceProvider>());
				}
                
                //Common systems
                if(pSystem->IsA(Audio::AudioSystem::InterfaceID))
                {
                    m_audioSystem = static_cast<Audio::AudioSystem*>(pSystem);
                }
                if(pSystem->IsA(Input::InputSystem::InterfaceID))
                {
                    m_inputSystem = static_cast<Input::InputSystem*>(pSystem);
                }
                if(pSystem->IsA(Rendering::RenderSystem::InterfaceID))
                {
                    m_renderSystem = static_cast<Rendering::RenderSystem*>(pSystem);
                }
                if(pSystem->IsA(FileSystem::InterfaceID))
                {
                    m_fileSystem = static_cast<FileSystem*>(pSystem);
                }
			}

            //Give the resource managers their providers
            m_resourceManagerDispenser->SetResourceProviders(m_resourceProviders);

            m_platformSystem->PostCreateSystems();
		}
        //----------------------------------------------------
        //----------------------------------------------------
        void Application::LoadDefaultResources()
        {
            Json::Value jRoot;
            if(Utils::ReadJson(StorageLocation::k_package, "App.config", &jRoot) == true)
            {
                if(jRoot.isMember("MaxFPS"))
                {
                    u32 udwMaxFPS = jRoot["MaxFPS"].asUInt();
                    m_platformSystem->SetMaxFPS(udwMaxFPS);
                }
                
                if(jRoot.isMember("MasterText"))
                {
                    StorageLocation eStorageLocation = GetStorageLocationFromString(jRoot["MasterText"].get("Location", "Package").asString());
                    std::string strPath = jRoot["MasterText"].get("Path", "").asString();
                    LocalisedText::RefreshMasterText(eStorageLocation, strPath);
                }
                
                if(jRoot.isMember("DefaultMesh"))
                {
                    StorageLocation eStorageLocation = GetStorageLocationFromString(jRoot["DefaultMesh"].get("Location", "Package").asString());
                    std::string strPath = jRoot["DefaultMesh"].get("Path", "").asString();
                    m_defaultMesh = LOAD_RESOURCE(Rendering::Mesh, eStorageLocation, strPath);
                }
                
                if(jRoot.isMember("DefaultFont"))
                {
                    StorageLocation eStorageLocation = GetStorageLocationFromString(jRoot["DefaultFont"].get("Location", "Package").asString());
                    std::string strPath = jRoot["DefaultFont"].get("Path", "").asString();
                    m_defaultFont = LOAD_RESOURCE(Rendering::Font, eStorageLocation, strPath);
                }
                
                if(jRoot.isMember("DefaultMaterial"))
                {
                    StorageLocation eStorageLocation = GetStorageLocationFromString(jRoot["DefaultMaterial"].get("Location", "Package").asString());
                    std::string strPath = jRoot["DefaultMaterial"].get("Path", "").asString();
                    m_defaultMaterial = LOAD_RESOURCE(Rendering::Material, eStorageLocation, strPath);
                }
            }
        }
        //----------------------------------------------------
        //----------------------------------------------------
        void Application::DetermineResourceDirectories()
        {
            //Get a list of the resource directories and determine which one this device should be
            //loading from based on it's screen
            std::vector<ResourceDirectoryInfo> aDirectoryInfos;
            std::string strDefaultDir, strDeviceDir, strDefaultDeviceDir;
            SetResourceDirectories(aDirectoryInfos, strDefaultDeviceDir, strDefaultDir);
            
            //Sort the info by resolution low to high
            std::sort(aDirectoryInfos.begin(), aDirectoryInfos.end(), ResolutionSortPredicate);
            
            u32 udwCurrentRes = Screen::GetOrientedWidth() * Screen::GetOrientedHeight();
            f32 fCurrenctDensity = Screen::GetDensity();
            f32 fAssetDensity = 1.0f;
            for(std::vector<ResourceDirectoryInfo>::const_iterator it = aDirectoryInfos.begin(); it != aDirectoryInfos.end(); ++it)
            {
                //The density and the resolution must both be under the maximum for the directory to be selected.
                if(udwCurrentRes <= it->m_maxRes && fCurrenctDensity <= it->m_maxDensity)
                {
                    strDeviceDir = it->m_directory;
                    fAssetDensity = it->m_resourcesDensity;
                    break;
                }
            }
            
            if(strDeviceDir.empty())
            {
                CS_LOG_WARNING("No resource folder can be found for this device switching to default directory");
                strDeviceDir = strDefaultDeviceDir;
            }
            
            FileSystem::SetResourceDirectories(strDeviceDir, strDefaultDeviceDir, strDefaultDir, fAssetDensity);
        }
        //----------------------------------------------------
        //----------------------------------------------------
		void Application::Update(f32 in_deltaTime)
		{
            in_deltaTime *= m_updateSpeed;
            
			CoreTimer::Update(in_deltaTime);
            
            NotificationScheduler::Update(in_deltaTime);
            
			//Update sub systems
            for(std::vector<IUpdateable*>::iterator it = m_updateableSystems.begin(); it != m_updateableSystems.end(); ++it)
            {
                (*it)->Update(in_deltaTime);
            }
			
			//Tell the state manager to update the active state
			m_stateManager.Update(in_deltaTime);
		}
        //----------------------------------------------------
        //----------------------------------------------------
		void Application::SetOrientation(ScreenOrientation in_orientation)
		{
			if(m_renderer->GetActiveCameraPtr())
			{
				m_renderer->GetActiveCameraPtr()->SetViewportOrientation(in_orientation);
			}
            
            Input::TouchScreen * pTouchScreen = GetSystem(Input::InputSystem::InterfaceID)->GetInterface<Input::InputSystem>()->GetTouchScreen();
            if (pTouchScreen != nullptr)
            {
                pTouchScreen->SetScreenHeight(Screen::GetOrientedHeight());
            }
		}
        //----------------------------------------------------
        //----------------------------------------------------
		void Application::ResumeApplication()
		{
			CS_LOG_DEBUG("App Resuming...");
            
			if(m_renderSystem != nullptr)
			{
				m_renderSystem->Resume();
			}
            
			m_isSuspending = false;
			ApplicationEvents::GetResumeEvent().NotifyConnections();
            
			//Tell the active state to continue
			m_stateManager.Resume();
			
			CS_LOG_DEBUG("App Finished Resuming...");
		}
        //----------------------------------------------------
        //----------------------------------------------------
		Application::~Application()
		{
            CS_ASSERT(s_application == nullptr, "Application destructed without calling OnDestroy!");
		}
	}
}
