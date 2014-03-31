//
//  Application.h
//  Chilli Source
//
//  Created by Scott Downie on 23/09/2010.
//  Copyright 2010 Tag Games. All rights reserved.
//

#include <ChilliSource/Core/Base/Application.h>

#include <ChilliSource/Audio/Base/AudioSystem.h>
#include <ChilliSource/Audio/Base/AudioLoader.h>
#include <ChilliSource/Audio/Base/AudioPlayer.h>

#include <ChilliSource/Core/Base/ApplicationEvents.h>
#include <ChilliSource/Core/Base/Device.h>
#include <ChilliSource/Core/Base/Logging.h>
#include <ChilliSource/Core/Base/PlatformSystem.h>
#include <ChilliSource/Core/Base/Screen.h>
#include <ChilliSource/Core/Base/Utils.h>
#include <ChilliSource/Core/DialogueBox/DialogueBoxSystem.h>
#include <ChilliSource/Core/Entity/ComponentFactory.h>
#include <ChilliSource/Core/Entity/ComponentFactoryDispenser.h>
#include <ChilliSource/Core/File/LocalDataStore.h>
#include <ChilliSource/Core/File/TweakableConstants.h>
#include <ChilliSource/Core/Image/CSImageProvider.h>
#include <ChilliSource/Core/JSON/json.h>
#include <ChilliSource/Core/Localisation/LocalisedText.h>
#include <ChilliSource/Core/Math/MathUtils.h>
#include <ChilliSource/Core/Resource/ResourceGroupManager.h>
#include <ChilliSource/Core/Resource/ResourceManager.h>
#include <ChilliSource/Core/Resource/ResourceManagerDispenser.h>
#include <ChilliSource/Core/Resource/ResourceProvider.h>
#include <ChilliSource/Core/State/State.h>
#include <ChilliSource/Core/State/StateManager.h>
#include <ChilliSource/Core/System/System.h>
#include <ChilliSource/Core/System/SystemConcepts.h>
#include <ChilliSource/Core/Time/CoreTimer.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>

#include <ChilliSource/GUI/Base/GUIViewFactory.h>

#include <ChilliSource/Input/Keyboard/Keyboard.h>
#include <ChilliSource/Input/Pointer/PointerSystem.h>

#include <ChilliSource/Rendering/Base/Renderer.h>
#include <ChilliSource/Rendering/Base/RenderCapabilities.h>
#include <ChilliSource/Rendering/Base/RenderSystem.h>
#include <ChilliSource/Rendering/Camera/CameraComponent.h>
#include <ChilliSource/Rendering/Font/Font.h>
#include <ChilliSource/Rendering/Font/FontProvider.h>
#include <ChilliSource/Rendering/Material/Material.h>
#include <ChilliSource/Rendering/Material/MaterialProvider.h>
#include <ChilliSource/Rendering/Material/MaterialFactory.h>
#include <ChilliSource/Rendering/Model/Mesh.h>
#include <ChilliSource/Rendering/Sprite/SpriteSheetProvider.h>

#include <ctime>

//TODO: Remove
#include <ChilliSource/Backend/Rendering/OpenGL/Base/RenderSystem.h>

namespace ChilliSource
{
	namespace Core
	{
        namespace
        {
            const f32 k_defaultUpdateInterval = 1.0f/60.0f;
            const f32 k_updateClampThreshold = 0.33f;
            const f32 k_updateIntervalMax = k_updateClampThreshold;
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
        : m_currentAppTime(0), m_updateInterval(k_defaultUpdateInterval), m_updateSpeed(1.0f), m_renderSystem(nullptr), m_pointerSystem(nullptr), m_audioSystem(nullptr),
        m_renderer(nullptr), m_fileSystem(nullptr), m_stateManager(nullptr), m_defaultOrientation(ScreenOrientation::k_landscapeRight), m_resourceManagerDispenser(nullptr), m_updateIntervalRemainder(0.0f),
        m_shouldNotifyConnectionsResumeEvent(false), m_shouldNotifyConnectionsForegroundEvent(false), m_isFirstFrame(true), m_isSuspending(false), m_isSystemCreationAllowed(false)
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
            return m_stateManager;
        }
        //-----------------------------------------------------
        //-----------------------------------------------------
        Rendering::Renderer* Application::GetRenderer()
        {
            return m_renderer.get();
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
        //----------------------------------------------------
        //----------------------------------------------------
		void Application::Init()
		{
            CS_ASSERT(s_application == nullptr, "Application already initialised!");
            s_application = this;
            
            m_resourceManagerDispenser = new ResourceManagerDispenser(this);
            m_componentFactoryDispenser = new ComponentFactoryDispenser(this);
            
#ifdef CS_TARGETPLATFORM_WINDOWS
			//Because windows by default is landscape, this needs to be flipped.
			m_defaultOrientation = Core::ScreenOrientation::k_portraitUp;
#endif
            
			Core::Logging::Init();
            
            GUI::GUIViewFactory::RegisterDefaults();

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
            
            m_renderSystem->Init();
            
            //TODO: Once renderer becomes a system we can remove this stuff from here
            m_renderer = Rendering::Renderer::Create(m_renderSystem);
            m_renderer->Init();

            LoadDefaultResources();
			ScreenChangedOrientation(m_defaultOrientation);

            //initialise all of the application systems.
            for (const AppSystemUPtr& system : m_systems)
            {
                system->OnInit();
            }
            
            OnInit();
            PushInitialState();

			//Register for update events
			LocalDataStore::GetSingleton().SubscribeToApplicationSuspendEvent();

			//Begin the update loop
			m_platformSystem->Run();
		}
        //----------------------------------------------------
        //----------------------------------------------------
		void Application::Resume()
		{
            m_shouldNotifyConnectionsResumeEvent = true;
            
			//We must restart the application timer. This will automatically restart system updates
			m_platformSystem->SetUpdaterActive(true);
		}
        //----------------------------------------------------
        //----------------------------------------------------
        void Application::Foreground()
        {
            m_shouldNotifyConnectionsForegroundEvent = true;
        }
        //----------------------------------------------------
        //----------------------------------------------------
		void Application::Update(f32 in_deltaTime, TimeIntervalSecs in_timestamp)
		{
            if(m_shouldNotifyConnectionsResumeEvent == true)
			{
				m_shouldNotifyConnectionsResumeEvent = false;
				OnResume();
			}
            
            if(m_shouldNotifyConnectionsForegroundEvent == true)
			{
				m_shouldNotifyConnectionsForegroundEvent = false;
				OnForeground();
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
            
			//process any queued input received by the pointer system.
			if(m_pointerSystem != nullptr)
			{
				m_pointerSystem->ProcessQueuedInput();
			}
            
            while((m_updateIntervalRemainder >= Application::GetUpdateInterval()) || m_isFirstFrame)
            {
                m_updateIntervalRemainder -=  Application::GetUpdateInterval();
                
                //update all of the application systems
                for (const AppSystemUPtr& system : m_systems)
                {
                    system->OnFixedUpdate(Application::GetUpdateInterval());
                }
                
                m_isFirstFrame = false;
            }
            
            //Tell the state manager to update the active state
            OnUpdate(in_deltaTime);
            
            //Render the scene
            m_renderer->RenderToScreen(m_stateManager->GetActiveState()->GetScene());
            
#ifdef CS_ENABLE_DEBUGSTATS
			DebugStats::Clear();
#endif
		}
        //----------------------------------------------------
        //----------------------------------------------------
		void Application::ScreenChangedOrientation(ScreenOrientation in_orientation)
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
		void Application::ScreenResized(u32 in_width, u32 in_height)
		{
			Screen::SetRawDimensions(Core::Vector2((f32)in_width, (f32)in_height));

			if (m_renderSystem)
			{
				m_renderSystem->OnScreenOrientationChanged(in_width, in_height);
			}

			ApplicationEvents::GetScreenResizedEvent().NotifyConnections(in_width, in_height);
            
			CS_LOG_DEBUG("Screen resized Notification");
		}
        //----------------------------------------------------
        //----------------------------------------------------
		void Application::ApplicationMemoryWarning()
		{
			CS_LOG_DEBUG("Memory Warning. Clearing resource cache...");
			ResourceManagerDispenser::GetSingletonPtr()->FreeResourceCaches();
			ApplicationEvents::GetLowMemoryEvent().NotifyConnections();
            
            //update all of the application systems
            for (const AppSystemUPtr& system : m_systems)
            {
                system->OnMemoryWarning();
            }
		}
        //----------------------------------------------------
        //----------------------------------------------------
		void Application::GoBack()
		{
			CS_LOG_DEBUG("Go back event.");
			//TODO: Feed this to the application another way. m_stateManager.GetActiveState()->OnGoBack();
			ApplicationEvents::GetGoBackEvent().NotifyConnections();
		}
        //----------------------------------------------------
        //----------------------------------------------------
        void Application::Background()
        {
            for (const AppSystemUPtr& system : m_systems)
            {
                system->OnBackground();
            }
        }
        //----------------------------------------------------
        //----------------------------------------------------
		void Application::Suspend()
		{
            CS_LOG_DEBUG("App Suspending...");
            
			m_isSuspending = true;
            
            //suspend all application systems in reverse order.
            for (std::vector<AppSystemUPtr>::const_reverse_iterator it = m_systems.rbegin(); it != m_systems.rend(); ++it)
            {
                (*it)->OnSuspend();
            }
            
            m_renderSystem->Suspend();
            
			//We must invalidate the application timer. This will stop sub-system updates
			m_platformSystem->SetUpdaterActive(false);
            
			ApplicationEvents::GetSuspendEvent().NotifyConnections();
			ApplicationEvents::GetLateSuspendEvent().NotifyConnections();
			
			CS_LOG_DEBUG("App Finished Suspending...");
		}
        //----------------------------------------------------
        //----------------------------------------------------
        void Application::Destroy()
        {
            m_stateManager->DestroyStates();
            
            OnDestroy();

            //suspend all application systems in reverse order.
            for (std::vector<AppSystemUPtr>::const_reverse_iterator it = m_systems.rbegin(); it != m_systems.rend(); ++it)
            {
                (*it)->OnDestroy();
            }
            
			m_defaultFont.reset();
			m_defaultMesh.reset();
			m_defaultMaterial.reset();
            
            m_renderSystem->Destroy();
            
            m_platformSystem.reset();
			m_renderer.reset();
            CS_SAFEDELETE(m_resourceManagerDispenser);
            CS_SAFEDELETE(m_componentFactoryDispenser);
            
			//We have an issue with the order of destruction of systems.
			while(m_systemsOld.empty() == false)
			{
				m_systemsOld.pop_back();
			}
            
            s_application = nullptr;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        void Application::AddSystem_Old(SystemUPtr in_system)
        {
            CS_ASSERT(m_isSystemCreationAllowed == true, "Application systems cannot be created outwith the creation phase");
			if (in_system != nullptr)
			{
				m_systemsOld.push_back(std::move(in_system));
			}
        }
        //------------------------------------------------------
        //------------------------------------------------------
        void Application::CreateDefaultSystems()
        {
            //Core
            AddSystem_Old(FileSystem::Create());
            m_stateManager = CreateSystem<StateManager>();

            //TODO: Change this to a PNG image provider.
            CreateSystem<ImageProvider>();
            CreateSystem<CSImageProvider>();
            AddSystem_Old(DialogueBoxSystem::Create());

            //Audio
            m_audioSystem = CreateSystem<Audio::AudioSystem>();
            CreateSystem<Audio::AudioLoader>(m_audioSystem);

            //Input
            CreateSystem<Input::Keyboard>();
            m_pointerSystem = CreateSystem<Input::PointerSystem>();

            //Rendering
            Rendering::RenderCapabilities* renderCapabilities = CreateSystem<Rendering::RenderCapabilities>();
            
            //TODO: Don't assume this will be a GL render system. We only do this temporarily
            //in order to access the managers. This will change.
            m_renderSystem = CreateSystem<Rendering::RenderSystem>(renderCapabilities);
            OpenGL::RenderSystem* renderSystem = (OpenGL::RenderSystem*)m_renderSystem;
            CreateSystem<Rendering::MaterialFactory>(renderSystem->GetShaderManager(), renderCapabilities);
            CreateSystem<Rendering::MaterialProvider>(renderCapabilities);
            CreateSystem<Rendering::SpriteSheetProvider>();
            CreateSystem<Rendering::FontProvider>();
        }
        //----------------------------------------------------
        //----------------------------------------------------
		void Application::PostCreateSystems()
		{
            //Loop round all the created systems and categorise them
			for(std::vector<SystemUPtr>::iterator it = m_systemsOld.begin(); it != m_systemsOld.end(); ++it)
			{
                System* pSystem = it->get();
                
                //Updateables
				if(pSystem->IsA(IUpdateable::InterfaceID))
				{
					m_updateableSystems.push_back(dynamic_cast<IUpdateable*>(pSystem));
				}
                
                //Component producers
				if(pSystem->IsA(IComponentProducer::InterfaceID))
				{
                    IComponentProducer* pProducer = dynamic_cast<IComponentProducer*>(pSystem);
                    u32 udwNumFactoriesInSystem = pProducer->GetNumComponentFactories();
                    
                    for(u32 i=0; i<udwNumFactoriesInSystem; ++i)
                    {
                        m_componentFactoryDispenser->RegisterComponentFactory(pProducer->GetComponentFactoryPtr(i));
                    }
				}
                
                //Resource providers
				if(pSystem->IsA(ResourceProvider::InterfaceID))
				{
					m_resourceProviders.push_back(dynamic_cast<ResourceProvider*>(pSystem));
				}
                
                //Common systems
                if(pSystem->IsA(FileSystem::InterfaceID))
                {
                    m_fileSystem = static_cast<FileSystem*>(pSystem);
                }
			}
            
            //Loop round all the created app systems and categorise them
			for(const AppSystemUPtr& system : m_systems)
			{
                //Resource providers
				if(system->IsA(ResourceProvider::InterfaceID))
				{
					m_resourceProviders.push_back(dynamic_cast<ResourceProvider*>(system.get()));
				}
                
                //TODO: Remove this when all Component producers have been changed to systems
				if(system->IsA(IComponentProducer::InterfaceID))
				{
                    IComponentProducer* pProducer = dynamic_cast<IComponentProducer*>(system.get());
                    u32 udwNumFactoriesInSystem = pProducer->GetNumComponentFactories();
                    
                    for(u32 i=0; i<udwNumFactoriesInSystem; ++i)
                    {
                        m_componentFactoryDispenser->RegisterComponentFactory(pProducer->GetComponentFactoryPtr(i));
                    }
				}
			}

            //Give the resource managers their providers
            m_resourceManagerDispenser->SetResourceProviders(m_resourceProviders);

            Audio::AudioPlayer::Init();

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
            std::sort(aDirectoryInfos.begin(), aDirectoryInfos.end(), [] (const ResourceDirectoryInfo& in_lhs, const ResourceDirectoryInfo& in_rhs)
            {
                return (in_lhs.m_maxRes < in_rhs.m_maxRes);
            });
            
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
		void Application::OnUpdate(f32 in_deltaTime)
		{
            in_deltaTime *= m_updateSpeed;
            
			CoreTimer::Update(in_deltaTime);
            
			//Update sub systems
            for(std::vector<IUpdateable*>::iterator it = m_updateableSystems.begin(); it != m_updateableSystems.end(); ++it)
            {
                (*it)->Update(in_deltaTime);
            }
            
            //update all of the application systems
            for (const AppSystemUPtr& system : m_systems)
            {
                system->OnUpdate(in_deltaTime);
            }
		}
        //----------------------------------------------------
        //----------------------------------------------------
		void Application::OnResume()
		{
			CS_LOG_DEBUG("App Resuming...");
            
			m_isSuspending = false;
			ApplicationEvents::GetResumeEvent().NotifyConnections();
            
            m_renderSystem->Resume();
            
            //resume all of the application systems
            for (const AppSystemUPtr& system : m_systems)
            {
                system->OnResume();
            }
			
			CS_LOG_DEBUG("App Finished Resuming...");
		}
        //---------------------------------------------------
        //---------------------------------------------------
        void Application::OnForeground()
        {
            for (const AppSystemUPtr& system : m_systems)
            {
                system->OnForeground();
            }
        }
        //----------------------------------------------------
        //----------------------------------------------------
		void Application::SetOrientation(ScreenOrientation in_orientation)
		{
			if(m_renderer->GetActiveCameraPtr())
			{
				m_renderer->GetActiveCameraPtr()->SetViewportOrientation(in_orientation);
			}
		}
        //----------------------------------------------------
        //----------------------------------------------------
		Application::~Application()
		{
            CS_ASSERT(s_application == nullptr, "Application destructed without calling OnDestroy!");
		}
	}
}
