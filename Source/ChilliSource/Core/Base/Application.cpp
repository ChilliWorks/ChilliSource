//
//  Application.h
//  Chilli Source
//
//  Created by Scott Downie on 23/09/2010.
//  Copyright 2010 Tag Games. All rights reserved.
//

#include <ChilliSource/Core/Base/Application.h>

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
#include <ChilliSource/Core/Image/ImageProvider.h>
#include <ChilliSource/Core/Image/CSImageProvider.h>
#include <ChilliSource/Core/JSON/json.h>
#include <ChilliSource/Core/Localisation/LocalisedText.h>
#include <ChilliSource/Core/Math/MathUtils.h>
#include <ChilliSource/Core/Resource/ResourcePool.h>
#include <ChilliSource/Core/State/State.h>
#include <ChilliSource/Core/State/StateManager.h>
#include <ChilliSource/Core/String/StringParser.h>
#include <ChilliSource/Core/System/SystemConcepts.h>
#include <ChilliSource/Core/Time/CoreTimer.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>

#ifdef CS_ENABLE_DEBUGSTATS
#include <ChilliSource/Debugging/Base/DebugStats.h>
#endif

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
#include <ChilliSource/Rendering/Texture/CubemapProvider.h>
#include <ChilliSource/Rendering/Texture/TextureProvider.h>
#include <ChilliSource/Rendering/Texture/TextureAtlasProvider.h>

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
        : m_currentAppTime(0), m_updateInterval(k_defaultUpdateInterval), m_updateSpeed(1.0f), m_renderSystem(nullptr), m_pointerSystem(nullptr), m_resourcePool(nullptr),
        m_renderer(nullptr), m_fileSystem(nullptr), m_stateManager(nullptr), m_taskScheduler(nullptr), m_updateIntervalRemainder(0.0f),
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
        const Rendering::FontCSPtr& Application::GetDefaultFont() const
        {
            return m_defaultFont;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        const Rendering::MeshCSPtr& Application::GetDefaultMesh() const
        {
            return m_defaultMesh;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        const Rendering::MaterialCSPtr& Application::GetDefaultMaterial() const
        {
            return m_defaultMaterial;
        }
        //----------------------------------------------------
        //----------------------------------------------------
		void Application::Init()
		{
            CS_ASSERT(s_application == nullptr, "Application already initialised!");
            s_application = this;
            
            m_componentFactoryDispenser = new ComponentFactoryDispenser(this);
            
#ifdef CS_TARGETPLATFORM_WINDOWS
			//Because windows by default is landscape, this needs to be flipped.
			m_defaultOrientation = Core::ScreenOrientation::k_portraitUp;
#endif
            
			Logging::Create();
            
            GUI::GUIViewFactory::RegisterDefaults();

            //Initialise the platform specific API's
            m_platformSystem = PlatformSystem::Create();
			m_platformSystem->Init();
            
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

            //initialise all of the application systems.
            for (const AppSystemUPtr& system : m_systems)
            {
                system->OnInit();
            }
            
            LoadDefaultResources();
            
            OnInit();
            PushInitialState();

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
            Debugging::DebugStats::RecordEvent("FrameTime", in_deltaTime);
			Debugging::DebugStats::RecordEvent("FPS", 1.0f/in_deltaTime);
#endif
            
			//Update the app time since start
			m_currentAppTime = in_timestamp;
            
			m_taskScheduler->ExecuteMainThreadTasks();
            
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
            Debugging::DebugStats::Clear();
#endif
		}
        //----------------------------------------------------
        //----------------------------------------------------
		void Application::ApplicationMemoryWarning()
		{
			CS_LOG_VERBOSE("Memory Warning. Clearing resource cache...");
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
			CS_LOG_VERBOSE("Go back event.");
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
            CS_LOG_VERBOSE("App Suspending...");
            
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
			
			CS_LOG_VERBOSE("App Finished Suspending...");
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
            CS_SAFEDELETE(m_componentFactoryDispenser);
            
            m_resourcePool->Destroy();
            
            Logging::Destroy();
            
            s_application = nullptr;
        }
        //------------------------------------------------------
        //------------------------------------------------------
        void Application::CreateDefaultSystems()
        {
            //Core
            CreateSystem<Device>();
            CreateSystem<Screen>();
            
            //TODO: This should be moved to a separate system.
            DetermineResourceDirectories();
            
			m_taskScheduler = CreateSystem<TaskScheduler>();
            m_fileSystem = CreateSystem<FileSystem>();
            m_stateManager = CreateSystem<StateManager>();
            m_resourcePool = CreateSystem<ResourcePool>();
            CreateSystem<LocalDataStore>();
            CreateSystem<CSImageProvider>();
            CreateSystem<DialogueBoxSystem>();
            
            //TODO: Change this to a PNG image provider.
            CreateSystem<ImageProvider>();

            //Input
            CreateSystem<Input::Keyboard>();
            m_pointerSystem = CreateSystem<Input::PointerSystem>();

            //Rendering
            Rendering::RenderCapabilities* renderCapabilities = CreateSystem<Rendering::RenderCapabilities>();
            
            m_renderSystem = CreateSystem<Rendering::RenderSystem>(renderCapabilities);
            CreateSystem<Rendering::MaterialFactory>(renderCapabilities);
            CreateSystem<Rendering::MaterialProvider>(renderCapabilities);
            CreateSystem<Rendering::TextureAtlasProvider>();
            CreateSystem<Rendering::TextureProvider>();
            CreateSystem<Rendering::CubemapProvider>();
            CreateSystem<Rendering::FontProvider>();
        }
        //----------------------------------------------------
        //----------------------------------------------------
		void Application::PostCreateSystems()
		{
            //Loop round all the created app systems and categorise them
			for(const AppSystemUPtr& system : m_systems)
			{
                //Resource providers
				if(system->IsA(ResourceProvider::InterfaceID))
				{
                    m_resourcePool->AddProvider(dynamic_cast<ResourceProvider*>(system.get()));
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

            m_platformSystem->PostCreateSystems();
		}
        //----------------------------------------------------
        //----------------------------------------------------
        void Application::LoadDefaultResources()
        {
            //TODO: This should be moved to a separate system.
            
            CS_ASSERT(m_resourcePool, "Resource pool must be available when loading default resources");
            
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
                    StorageLocation eStorageLocation = ParseStorageLocation(jRoot["MasterText"].get("Location", "Package").asString());
                    std::string strPath = jRoot["MasterText"].get("Path", "").asString();
                    LocalisedText::RefreshMasterText(eStorageLocation, strPath);
                }
                
                if(jRoot.isMember("DefaultMesh"))
                {
                    StorageLocation eStorageLocation = ParseStorageLocation(jRoot["DefaultMesh"].get("Location", "Package").asString());
                    std::string strPath = jRoot["DefaultMesh"].get("Path", "").asString();
                    m_defaultMesh = m_resourcePool->LoadResource<Rendering::Mesh>(eStorageLocation, strPath);
                }
                
                if(jRoot.isMember("DefaultFont"))
                {
                    StorageLocation eStorageLocation = ParseStorageLocation(jRoot["DefaultFont"].get("Location", "Package").asString());
                    std::string strPath = jRoot["DefaultFont"].get("Path", "").asString();
                    m_defaultFont = m_resourcePool->LoadResource<Rendering::Font>(eStorageLocation, strPath);
                }
                
                if(jRoot.isMember("DefaultMaterial"))
                {
                    StorageLocation eStorageLocation = ParseStorageLocation(jRoot["DefaultMaterial"].get("Location", "Package").asString());
                    std::string strPath = jRoot["DefaultMaterial"].get("Path", "").asString();
                    m_defaultMaterial = m_resourcePool->LoadResource<Rendering::Material>(eStorageLocation, strPath);
                }
            }
        }
        //----------------------------------------------------
        //----------------------------------------------------
        void Application::DetermineResourceDirectories()
        {
            //TODO: This should be moved to a separate system.
            
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
            
            Screen* screen = GetSystem<Screen>();
            u32 udwCurrentRes = (u32)(screen->GetResolution().x * screen->GetResolution().y);
            f32 fCurrenctDensity = screen->GetDensityScale();
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
			CS_LOG_VERBOSE("App Resuming...");
            
			m_isSuspending = false;
			ApplicationEvents::GetResumeEvent().NotifyConnections();
            
            m_renderSystem->Resume();
            
            //resume all of the application systems
            for (const AppSystemUPtr& system : m_systems)
            {
                system->OnResume();
            }
			
			CS_LOG_VERBOSE("App Finished Resuming...");
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
		Application::~Application()
		{
            CS_ASSERT(s_application == nullptr, "Application destructed without calling OnDestroy!");
		}
	}
}
