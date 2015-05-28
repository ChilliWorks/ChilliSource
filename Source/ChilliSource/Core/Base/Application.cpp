//
//  Application.cpp
//  Chilli Source
//  Created by Scott Downie on 23/09/2010.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2010 Tag Games Limited
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

#include <ChilliSource/Core/Base/Application.h>

#include <ChilliSource/Core/Base/AppConfig.h>
#include <ChilliSource/Core/Base/Device.h>
#include <ChilliSource/Core/Base/Logging.h>
#include <ChilliSource/Core/Base/PlatformSystem.h>
#include <ChilliSource/Core/Base/Screen.h>
#include <ChilliSource/Core/DialogueBox/DialogueBoxSystem.h>
#include <ChilliSource/Core/File/AppDataStore.h>
#include <ChilliSource/Core/File/TaggedFilePathResolver.h>
#include <ChilliSource/Core/Image/CSImageProvider.h>
#include <ChilliSource/Core/Image/PNGImageProvider.h>
#include <ChilliSource/Core/Localisation/LocalisedText.h>
#include <ChilliSource/Core/Localisation/LocalisedTextProvider.h>
#include <ChilliSource/Core/Resource/ResourcePool.h>
#include <ChilliSource/Core/State/State.h>
#include <ChilliSource/Core/State/StateManager.h>
#include <ChilliSource/Core/String/StringParser.h>
#include <ChilliSource/Core/Time/CoreTimer.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>

#include <ChilliSource/Input/DeviceButtons/DeviceButtonSystem.h>
#include <ChilliSource/Input/Keyboard/Keyboard.h>
#include <ChilliSource/Input/Pointer/PointerSystem.h>
#include <ChilliSource/Input/TextEntry/TextEntry.h>

#include <ChilliSource/Rendering/Base/CanvasRenderer.h>
#include <ChilliSource/Rendering/Base/Renderer.h>
#include <ChilliSource/Rendering/Base/RenderCapabilities.h>
#include <ChilliSource/Rendering/Base/RenderComponentFactory.h>
#include <ChilliSource/Rendering/Base/RenderSystem.h>
#include <ChilliSource/Rendering/Camera/CameraComponent.h>
#include <ChilliSource/Rendering/Font/Font.h>
#include <ChilliSource/Rendering/Font/FontProvider.h>
#include <ChilliSource/Rendering/Material/Material.h>
#include <ChilliSource/Rendering/Material/MaterialProvider.h>
#include <ChilliSource/Rendering/Material/MaterialFactory.h>
#include <ChilliSource/Rendering/Model/Mesh.h>
#include <ChilliSource/Rendering/Particle/CSParticleProvider.h>
#include <ChilliSource/Rendering/Particle/Affector/ParticleAffectorDefFactory.h>
#include <ChilliSource/Rendering/Particle/Drawable/ParticleDrawableDefFactory.h>
#include <ChilliSource/Rendering/Particle/Emitter/ParticleEmitterDefFactory.h>
#include <ChilliSource/Rendering/Texture/CubemapProvider.h>
#include <ChilliSource/Rendering/Texture/TextureAtlasProvider.h>
#include <ChilliSource/Rendering/Texture/TextureProvider.h>

#include <ChilliSource/UI/Base/ComponentFactory.h>
#include <ChilliSource/UI/Base/WidgetDefProvider.h>
#include <ChilliSource/UI/Base/WidgetFactory.h>
#include <ChilliSource/UI/Base/WidgetTemplateProvider.h>

#include <algorithm>
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
            : m_updateInterval(k_defaultUpdateInterval)
		{
		}
        //----------------------------------------------------
        //----------------------------------------------------
		std::string Application::GetAppVersion() const
		{
			return m_platformSystem->GetAppVersion();
		}
        //----------------------------------------------------
        //----------------------------------------------------
		TimeIntervalSecs Application::GetAppElapsedTime() const
		{
			return m_currentAppTime;
		}
        //----------------------------------------------------
        //----------------------------------------------------
        TimeIntervalSecs Application::GetSystemTime() const
        {
            return time(0);
        }
        //----------------------------------------------------
        //----------------------------------------------------
		TimeIntervalMs Application::GetSystemTimeInMilliseconds() const
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
		f32 Application::GetUpdateInterval() const
		{
			return m_updateInterval;
		}
        //----------------------------------------------------
        //----------------------------------------------------
		f32 Application::GetUpdateIntervalMax() const
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
            m_platformSystem->Quit();
        }
        //----------------------------------------------------
        //----------------------------------------------------
		void Application::Init()
		{
            CS_ASSERT(s_application == nullptr, "Application already initialised!");
            s_application = this;
            
			Logging::Create();

            //Create all application systems.
            m_isSystemCreationAllowed = true;
            CreateDefaultSystems();
			CreateSystems();
            m_isSystemCreationAllowed = false;
            
			PostCreateSystems();
            
            //initialise all of the application systems.
            for (const AppSystemUPtr& system : m_systems)
            {
                system->OnInit();
            }
            
            OnInit();
            PushInitialState();
		}
        //----------------------------------------------------
        //----------------------------------------------------
		void Application::Resume()
		{
            m_shouldNotifyConnectionsResumeEvent = true;
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
            
#if CS_ENABLE_DEBUG
            //When debugging we may have breakpoints so restrict the time between
            //updates to something feasible.
            in_deltaTime = std::min(in_deltaTime, 0.5f);
#endif
            
			//Update the app time since start
			m_currentAppTime = in_timestamp;
            
			m_taskScheduler->ExecuteMainThreadTasks();
            
            //We do not need to render as often as we update so this callback will be triggered
            //less freqenctly than the update frequency suggests. We must work out how many times to update based on the time since last frame
            //and our actual update frequency. We carry the remainder to the next frame until we have a full update cycle
            m_updateIntervalRemainder = std::min(m_updateIntervalRemainder + in_deltaTime, GetUpdateIntervalMax());
            
			//process any queued input received by the pointer system.
			if(m_pointerSystem != nullptr)
			{
				m_pointerSystem->ProcessQueuedInput();
			}
            
            while((m_updateIntervalRemainder >= GetUpdateInterval()) || m_isFirstFrame)
            {
                m_updateIntervalRemainder -=  GetUpdateInterval();
                
                //update all of the application systems
                for (const AppSystemUPtr& system : m_systems)
                {
                    system->OnFixedUpdate(GetUpdateInterval());
                }
                
                m_stateManager->FixedUpdateStates(GetUpdateInterval());
                
                m_isFirstFrame = false;
            }
            
            //Tell the state manager to update the active state
            OnUpdate(in_deltaTime);
		}
        //----------------------------------------------------
        //----------------------------------------------------
        void Application::Render()
        {
            if(m_isSuspending)
            {
                // Updating after told to suspend so early out
                return;
            }
            
            //Render the scene
            m_renderer->RenderToScreen(m_stateManager->GetActiveState()->GetScene(), m_stateManager->GetActiveState()->GetUICanvas());
        }
        //----------------------------------------------------
        //----------------------------------------------------
		void Application::ApplicationMemoryWarning()
		{
			CS_LOG_VERBOSE("Memory Warning. Clearing resource cache...");
            
            //update all of the application systems
            for (const AppSystemUPtr& system : m_systems)
            {
                system->OnMemoryWarning();
            }
            
            m_stateManager->MemoryWarningStates();
		}
        //----------------------------------------------------
        //----------------------------------------------------
        void Application::Background()
        {
            if(m_shouldNotifyConnectionsForegroundEvent == true)
			{
				m_shouldNotifyConnectionsForegroundEvent = false;
                return;
            }
            
            m_stateManager->BackgroundStates();
            
            for (const AppSystemUPtr& system : m_systems)
            {
                system->OnBackground();
            }
        }
        //----------------------------------------------------
        //----------------------------------------------------
		void Application::Suspend()
		{
            if(m_shouldNotifyConnectionsResumeEvent == true)
			{
				m_shouldNotifyConnectionsResumeEvent = false;
                return;
            }
            
            CS_LOG_VERBOSE("App Suspending...");
            
			m_isSuspending = true;
            
            m_stateManager->SuspendStates();
            
            //suspend all application systems in reverse order.
            for (std::vector<AppSystemUPtr>::const_reverse_iterator it = m_systems.rbegin(); it != m_systems.rend(); ++it)
            {
                (*it)->OnSuspend();
            }
            
            m_renderSystem->Suspend();
            
			CS_LOG_VERBOSE("App Finished Suspending...");
		}
        //----------------------------------------------------
        //----------------------------------------------------
        void Application::Destroy()
        {
			m_taskScheduler->Destroy();
            
            OnDestroy();

            m_stateManager->DestroyStates();

            //suspend all application systems in reverse order.
            for (std::vector<AppSystemUPtr>::const_reverse_iterator it = m_systems.rbegin(); it != m_systems.rend(); ++it)
            {
                (*it)->OnDestroy();
            }
            
            m_renderSystem->Destroy();
			m_resourcePool->Destroy();

            m_systems.clear();

            Logging::Destroy();
            
            s_application = nullptr;
        }
        //------------------------------------------------------
        //------------------------------------------------------
        void Application::CreateDefaultSystems()
        {
            //Core
            m_platformSystem = CreateSystem<PlatformSystem>();
            m_appConfig = CreateSystem<AppConfig>();
            CreateSystem<Device>();
            m_screen = CreateSystem<Screen>();
            
			m_taskScheduler = CreateSystem<TaskScheduler>();
            m_fileSystem = CreateSystem<FileSystem>();
            m_stateManager = CreateSystem<StateManager>();
            m_resourcePool = CreateSystem<ResourcePool>();
            m_taggedPathResolver = CreateSystem<TaggedFilePathResolver>();
            CreateSystem<AppDataStore>();
            CreateSystem<CSImageProvider>();
            CreateSystem<PNGImageProvider>();
            CreateSystem<DialogueBoxSystem>();
            CreateSystem<LocalisedTextProvider>();

            //Input
            CreateSystem<Input::Keyboard>();
            m_pointerSystem = CreateSystem<Input::PointerSystem>();
            CreateSystem<Input::DeviceButtonSystem>();
            CreateSystem<Input::TextEntry>();
            
            //Rendering
            Rendering::RenderCapabilities* renderCapabilities = CreateSystem<Rendering::RenderCapabilities>();
            m_renderSystem = CreateSystem<Rendering::RenderSystem>(renderCapabilities);
            m_renderer = CreateSystem<Rendering::Renderer>(m_renderSystem);
            CreateSystem<Rendering::CanvasRenderer>();
            CreateSystem<Rendering::MaterialFactory>(renderCapabilities);
            CreateSystem<Rendering::MaterialProvider>(renderCapabilities);
            CreateSystem<Rendering::TextureAtlasProvider>();
            CreateSystem<Rendering::TextureProvider>();
            CreateSystem<Rendering::CubemapProvider>();
            CreateSystem<Rendering::FontProvider>();
            CreateSystem<Rendering::RenderComponentFactory>();
            
            //Particles
            CreateSystem<Rendering::CSParticleProvider>();
            CreateSystem<Rendering::ParticleAffectorDefFactory>();
            CreateSystem<Rendering::ParticleDrawableDefFactory>();
            CreateSystem<Rendering::ParticleEmitterDefFactory>();
            
            //UI
            CreateSystem<UI::ComponentFactory>();
            m_widgetFactory = CreateSystem<UI::WidgetFactory>();
            CreateSystem<UI::WidgetDefProvider>();
            CreateSystem<UI::WidgetTemplateProvider>();

            //Create any platform specific default systems
            m_platformSystem->CreateDefaultSystems(this);
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
			}
            
            //Initialise the render system prior to the OnInit() event.
            m_renderSystem->Init();
            
            //Texture/Cubemap provider is a compound provider and needs to be informed when the other providers are created.
            GetSystem<Rendering::TextureProvider>()->PostCreate();
            GetSystem<Rendering::CubemapProvider>()->PostCreate();
            
            //Load the app config set preferred FPS.
            m_appConfig->Load();
            m_platformSystem->SetPreferredFPS(m_appConfig->GetPreferredFPS());
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
            
            m_stateManager->UpdateStates(in_deltaTime);
		}
        //----------------------------------------------------
        //----------------------------------------------------
		void Application::OnResume()
		{
			CS_LOG_VERBOSE("App Resuming...");
            
			m_isSuspending = false;
            
            m_renderSystem->Resume();
            
            //resume all of the application systems
            for (const AppSystemUPtr& system : m_systems)
            {
                system->OnResume();
            }
            
            m_stateManager->ResumeStates();
			
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
            
            m_stateManager->ForegroundStates();
        }
        //-----------------------------------------------------
        //-----------------------------------------------------
        StateManager* Application::GetStateManager()
        {
            return m_stateManager;
        }
        //-----------------------------------------------------
        //-----------------------------------------------------
        const StateManager* Application::GetStateManager() const
        {
            return m_stateManager;
        }
        //-----------------------------------------------------
        //-----------------------------------------------------
        Rendering::Renderer* Application::GetRenderer()
        {
            return m_renderer;
        }
        //-----------------------------------------------------
        //-----------------------------------------------------
        const Rendering::Renderer* Application::GetRenderer() const
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
        const Rendering::RenderSystem* Application::GetRenderSystem() const
        {
            return m_renderSystem;
        }
        //-----------------------------------------------------
        //-----------------------------------------------------
        FileSystem* Application::GetFileSystem()
        {
            return m_fileSystem;
        }
        //-----------------------------------------------------
        //-----------------------------------------------------
        const FileSystem* Application::GetFileSystem() const
        {
            return m_fileSystem;
        }
        //-----------------------------------------------------
        //-----------------------------------------------------
        TaggedFilePathResolver* Application::GetTaggedFilePathResolver()
        {
            return m_taggedPathResolver;
        }
        //-----------------------------------------------------
        //-----------------------------------------------------
        const TaggedFilePathResolver* Application::GetTaggedFilePathResolver() const
        {
            return m_taggedPathResolver;
        }
        //-----------------------------------------------------
        //-----------------------------------------------------
        TaskScheduler* Application::GetTaskScheduler()
        {
            return m_taskScheduler;
        }
        //-----------------------------------------------------
        //-----------------------------------------------------
        const TaskScheduler* Application::GetTaskScheduler() const
        {
            return m_taskScheduler;
        }
        //-----------------------------------------------------
        //-----------------------------------------------------
        ResourcePool* Application::GetResourcePool()
        {
            return m_resourcePool;
        }
        //-----------------------------------------------------
        //-----------------------------------------------------
        const ResourcePool* Application::GetResourcePool() const
        {
            return m_resourcePool;
        }
        //-----------------------------------------------------
        //-----------------------------------------------------
        AppConfig* Application::GetAppConfig()
        {
            return m_appConfig;
        }
        //-----------------------------------------------------
        //-----------------------------------------------------
        const AppConfig* Application::GetAppConfig() const
        {
            return m_appConfig;
        }
        //-----------------------------------------------------
        //-----------------------------------------------------
        Screen* Application::GetScreen()
        {
            return m_screen;
        }
        //-----------------------------------------------------
        //-----------------------------------------------------
        const Screen* Application::GetScreen() const
        {
            return m_screen;
        }
        //-----------------------------------------------------
        //-----------------------------------------------------
        UI::WidgetFactory* Application::GetWidgetFactory()
        {
            return m_widgetFactory;
        }
        //-----------------------------------------------------
        //-----------------------------------------------------
        const UI::WidgetFactory* Application::GetWidgetFactory() const
        {
            return m_widgetFactory;
        }
        //----------------------------------------------------
        //----------------------------------------------------
		Application::~Application()
		{
            CS_ASSERT(s_application == nullptr, "Application destructed without calling OnDestroy!");
		}
	}
}
