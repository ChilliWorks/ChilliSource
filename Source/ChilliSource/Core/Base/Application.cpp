//
//  Application.cpp
//  ChilliSource
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
#include <ChilliSource/Core/Scene/Scene.h>
#include <ChilliSource/Core/State/State.h>
#include <ChilliSource/Core/State/StateManager.h>
#include <ChilliSource/Core/String/StringParser.h>
#include <ChilliSource/Core/Time/CoreTimer.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>

#include <ChilliSource/Input/DeviceButtons/DeviceButtonSystem.h>
#include <ChilliSource/Input/Gamepad/GamepadSystem.h>
#include <ChilliSource/Input/Keyboard/Keyboard.h>
#include <ChilliSource/Input/Pointer/PointerSystem.h>
#include <ChilliSource/Input/TextEntry/TextEntry.h>

#include <ChilliSource/Rendering/Base/CanvasRenderer.h>
#include <ChilliSource/Rendering/Base/RenderCapabilities.h>
#include <ChilliSource/Rendering/Base/RenderCommandBufferManager.h>
#include <ChilliSource/Rendering/Base/Renderer.h>
#include <ChilliSource/Rendering/Base/RenderSnapshot.h>
#include <ChilliSource/Rendering/Base/TargetType.h>
#include <ChilliSource/Rendering/Camera/CameraComponent.h>
#include <ChilliSource/Rendering/Font/Font.h>
#include <ChilliSource/Rendering/Font/FontProvider.h>
#include <ChilliSource/Rendering/Material/Material.h>
#include <ChilliSource/Rendering/Material/MaterialProvider.h>
#include <ChilliSource/Rendering/Material/MaterialFactory.h>
#include <ChilliSource/Rendering/Material/RenderMaterialGroupManager.h>
#include <ChilliSource/Rendering/Model/RenderMeshManager.h>
#include <ChilliSource/Rendering/Particle/CSParticleProvider.h>
#include <ChilliSource/Rendering/Particle/Affector/ParticleAffectorDefFactory.h>
#include <ChilliSource/Rendering/Particle/Drawable/ParticleDrawableDefFactory.h>
#include <ChilliSource/Rendering/Particle/Emitter/ParticleEmitterDefFactory.h>
#include <ChilliSource/Rendering/Shader/CSShaderProvider.h>
#include <ChilliSource/Rendering/Shader/RenderShaderManager.h>
#include <ChilliSource/Rendering/Target/RenderTargetGroupManager.h>
#include <ChilliSource/Rendering/Texture/RenderTextureManager.h>
#include <ChilliSource/Rendering/Texture/TextureAtlasProvider.h>
#include <ChilliSource/Rendering/Texture/TextureProvider.h>
#include <ChilliSource/Rendering/Texture/CubemapProvider.h>

#include <ChilliSource/UI/Base/UIComponentFactory.h>
#include <ChilliSource/UI/Base/WidgetDefProvider.h>
#include <ChilliSource/UI/Base/WidgetFactory.h>
#include <ChilliSource/UI/Base/WidgetTemplateProvider.h>

#include <algorithm>
#include <chrono>
#include <ctime>

#if defined(CS_TARGETPLATFORM_IOS) || defined(CS_TARGETPLATFORM_ANDROID) || defined(CS_TARGETPLATFORM_WINDOWS) || defined(CS_TARGETPLATFORM_RPI)
#   include <CSBackend/Rendering/OpenGL/Base/GLContextRestorer.h>
#endif

namespace ChilliSource
{
    namespace
    {
        constexpr f32 k_defaultUpdateInterval = 1.0f / 60.0f;
        constexpr f32 k_updateClampThreshold = 0.33f;
        constexpr f32 k_updateIntervalMax = k_updateClampThreshold;
        
        /// Snapshot the view/camera data from the given scene into a render camera
        ///
        /// @return Render camera
        ///
        RenderCamera GenerateRenderCamera(Scene* scene) noexcept
        {
            auto camera = scene->GetActiveCamera();
            
            RenderCamera renderCamera;
            if (camera)
            {
                Entity* cameraEntity = camera->GetEntity();
                CS_ASSERT(cameraEntity, "Active CameraComponent must be attached to an entity.");
                
                const auto& transform = cameraEntity->GetTransform();
                renderCamera = RenderCamera(transform.GetWorldTransform(), camera->GetProjection(), transform.GetWorldOrientation());
            }
            
            return renderCamera;
        }
    }
    
    Application* Application::s_application = nullptr;
    
    //------------------------------------------------------------------------------
    Application* Application::Get() noexcept
    {
        return s_application;
    }

    //------------------------------------------------------------------------------
    Application::Application(ChilliSource::SystemInfoCUPtr systemInfo) noexcept
        : m_updateInterval(k_defaultUpdateInterval), m_frameIndex(0), m_systemInfo(std::move(systemInfo))
    {
        m_appVersion = m_systemInfo->GetAppVersion();
    }

    //------------------------------------------------------------------------------
    const std::string& Application::GetAppVersion() const noexcept
    {
        return m_appVersion;
    }

    //------------------------------------------------------------------------------
    u32 Application::GetFrameIndex() const noexcept
    {
        return m_frameIndex;
    }

    //------------------------------------------------------------------------------
    TimeIntervalSecs Application::GetAppElapsedTime() const noexcept
    {
        CS_RELEASE_ASSERT(m_taskScheduler->IsMainThread(), "Can only be called from the main thread.");
        
        return m_currentAppTime;
    }

    //------------------------------------------------------------------------------
    TimeIntervalSecs Application::GetSystemTime() const noexcept
    {
        return std::chrono::duration_cast<std::chrono::seconds>((std::chrono::system_clock::now().time_since_epoch())).count();
    }

    //------------------------------------------------------------------------------
    TimeIntervalMs Application::GetSystemTimeInMilliseconds() const noexcept
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>((std::chrono::system_clock::now().time_since_epoch())).count();
    }

    //------------------------------------------------------------------------------
    void Application::SetUpdateInterval(f32 updateInterval) noexcept
    {
        m_updateInterval = updateInterval;
    }

    //------------------------------------------------------------------------------
    f32 Application::GetUpdateInterval() const noexcept
    {
        return m_updateInterval;
    }

    //------------------------------------------------------------------------------
    f32 Application::GetUpdateIntervalMax() const noexcept
    {
        return k_updateIntervalMax;
    }

    //------------------------------------------------------------------------------
    void Application::SetUpdateSpeed(f32 speed) noexcept
    {
        m_updateSpeed = speed;
    }

    //------------------------------------------------------------------------------
    void Application::Quit() noexcept
    {
        m_platformSystem->Quit();
    }
    
    //------------------------------------------------------------------------------
    StateManager* Application::GetStateManager() noexcept
    {
        return m_stateManager;
    }
    
    //------------------------------------------------------------------------------
    const StateManager* Application::GetStateManager() const noexcept
    {
        return m_stateManager;
    }
    
    //------------------------------------------------------------------------------
    FileSystem* Application::GetFileSystem() noexcept
    {
        return m_fileSystem;
    }
    
    //------------------------------------------------------------------------------
    const FileSystem* Application::GetFileSystem() const noexcept
    {
        return m_fileSystem;
    }
    
    //------------------------------------------------------------------------------
    TaggedFilePathResolver* Application::GetTaggedFilePathResolver() noexcept
    {
        return m_taggedPathResolver;
    }
    
    //------------------------------------------------------------------------------
    const TaggedFilePathResolver* Application::GetTaggedFilePathResolver() const noexcept
    {
        return m_taggedPathResolver;
    }
    
    //------------------------------------------------------------------------------
    TaskScheduler* Application::GetTaskScheduler() noexcept
    {
        return m_taskScheduler;
    }
    
    //------------------------------------------------------------------------------
    const TaskScheduler* Application::GetTaskScheduler() const noexcept
    {
        return m_taskScheduler;
    }
    
    //------------------------------------------------------------------------------
    ResourcePool* Application::GetResourcePool() noexcept
    {
        return m_resourcePool;
    }
    
    //------------------------------------------------------------------------------
    const ResourcePool* Application::GetResourcePool() const noexcept
    {
        return m_resourcePool;
    }
    
    //------------------------------------------------------------------------------
    AppConfig* Application::GetAppConfig() noexcept
    {
        return m_appConfig;
    }
    
    //------------------------------------------------------------------------------
    const AppConfig* Application::GetAppConfig() const noexcept
    {
        return m_appConfig;
    }
    
    //------------------------------------------------------------------------------
    Screen* Application::GetScreen() noexcept
    {
        return m_screen;
    }
    
    //------------------------------------------------------------------------------
    const Screen* Application::GetScreen() const noexcept
    {
        return m_screen;
    }
    
    //------------------------------------------------------------------------------
    WidgetFactory* Application::GetWidgetFactory() noexcept
    {
        return m_widgetFactory;
    }
    
    //------------------------------------------------------------------------------
    const WidgetFactory* Application::GetWidgetFactory() const noexcept
    {
        return m_widgetFactory;
    }

    //------------------------------------------------------------------------------
    void Application::CreateDefaultSystems() noexcept
    {
        //Core
        m_platformSystem = CreateSystem<PlatformSystem>();
        m_appConfig = CreateSystem<AppConfig>();
        CreateSystem<Device>(m_systemInfo->GetDeviceInfo());
        m_screen = CreateSystem<Screen>(m_systemInfo->GetScreenInfo());
        
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
        CreateSystem<Keyboard>();
        m_pointerSystem = CreateSystem<PointerSystem>();
        m_gamepadSystem = CreateSystem<GamepadSystem>();
        CreateSystem<DeviceButtonSystem>();
        CreateSystem<TextEntry>();
        
        //Rendering
#if defined(CS_TARGETPLATFORM_IOS) || defined(CS_TARGETPLATFORM_ANDROID) || defined(CS_TARGETPLATFORM_WINDOWS) || defined(CS_TARGETPLATFORM_RPI)
        CreateSystem<CSBackend::OpenGL::GLContextRestorer>();
#endif
        
        CreateSystem<RenderCommandBufferManager>();
        m_renderer = CreateSystem<Renderer>();
        m_renderMaterialGroupManager = CreateSystem<RenderMaterialGroupManager>();
        CreateSystem<RenderMeshManager>();
        CreateSystem<RenderShaderManager>();
        CreateSystem<RenderTextureManager>();
        CreateSystem<RenderTargetGroupManager>();
        CreateSystem<RenderCapabilities>(m_systemInfo->GetRenderInfo());
        CreateSystem<CanvasRenderer>();
        CreateSystem<MaterialFactory>();
        CreateSystem<MaterialProvider>();
        CreateSystem<CSShaderProvider>();
        CreateSystem<CubemapProvider>();
        CreateSystem<TextureAtlasProvider>();
        CreateSystem<TextureProvider>();
        CreateSystem<FontProvider>();
        
        //Particles
        CreateSystem<CSParticleProvider>();
        CreateSystem<ParticleAffectorDefFactory>();
        CreateSystem<ParticleDrawableDefFactory>();
        CreateSystem<ParticleEmitterDefFactory>();
        
        //UI
        CreateSystem<UIComponentFactory>();
        m_widgetFactory = CreateSystem<WidgetFactory>();
        CreateSystem<WidgetDefProvider>();
        CreateSystem<WidgetTemplateProvider>();

        //Create any platform specific default systems
        m_platformSystem->CreateDefaultSystems(this);

        //Clean up SystemInfo to prevent unintended uses
        m_systemInfo.reset();
    }

    //------------------------------------------------------------------------------
    void Application::PostCreateSystems() noexcept
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
        
        //Texture provider is a compound provider and needs to be informed when the other providers are created.
        GetSystem<TextureProvider>()->PostCreate();
        GetSystem<CubemapProvider>()->PostCreate();
        
        //Load the app config set preferred FPS.
        m_appConfig->Load();
    }

    //------------------------------------------------------------------------------
    void Application::ProcessRenderSnapshotEvent() noexcept
    {
        CS_RELEASE_ASSERT(ChilliSource::Application::Get()->GetTaskScheduler()->IsMainThread(), "Tried to render to target from background thread.");
        
        auto activeState = m_stateManager->GetActiveState();
        CS_ASSERT(activeState, "Must have active state.");
 
        auto scenes = activeState->GetScenes();
        CS_ASSERT(scenes.size() > 0, "No scenes on active state");
        
        m_pendingRenderSnapshots.reserve(scenes.size());

        // We handle scenes backwards as the main one is always first in the list and
        // we need to process that one last
        for(auto it = scenes.rbegin(); it != scenes.rend(); ++it)
        {
            auto scene = (*it);
            
            if(scene->IsEnabled())
            {
                RenderScene(scene);
            }
        }
    }
    
    //------------------------------------------------------------------------------
    void Application::RenderScene(Scene* scene, TargetGroup* target) noexcept
    {
        CS_RELEASE_ASSERT(GetTaskScheduler()->IsMainThread(), "Tried to render scene from background thread.");
        
        auto targetToUse = target == nullptr ? scene->GetRenderTarget() : target;
        auto clearColour = scene->GetClearColour();
        IAllocator* frameAllocator;
        Integer2 resolution;
        TargetType targetType;
        const RenderTargetGroup* targetGroup;
        
        if(targetToUse == nullptr)
        {
            // Main (screen) render target
            
            // Main render target claims the allocator
            frameAllocator = m_renderer->GetFrameAllocatorQueue().Pop();
            resolution = Integer2(s32(m_screen->GetResolution().x), s32(m_screen->GetResolution().y));
            targetType = TargetType::k_main;
            targetGroup = nullptr;
        }
        else
        {
            // Offscreen render target
            
            // Just borrows the allocator of the main target
            frameAllocator = m_renderer->GetFrameAllocatorQueue().Front();
            resolution = targetToUse->GetRenderTargetGroup()->GetResolution();
            targetType = TargetType::k_offscreen;
            targetGroup = targetToUse->GetRenderTargetGroup();
        }

        
        RenderSnapshot renderSnapshot = m_renderer->CreateRenderSnapshot(targetGroup, resolution, clearColour, GenerateRenderCamera(scene));
        
        for (const AppSystemUPtr& system : m_systems)
        {
            system->OnRenderSnapshot(targetType, renderSnapshot, frameAllocator);
        }
        
        m_stateManager->RenderSnapshotStates(targetType, renderSnapshot, frameAllocator);
        scene->RenderSnapshotEntities(renderSnapshot, frameAllocator);
        
        if(targetToUse == nullptr)
        {
            // The renderer takes ownership of the frame allocator and will return it to the allocator queue
            m_renderer->ProcessRenderSnapshots(frameAllocator, std::move(renderSnapshot), std::move(m_pendingRenderSnapshots));
        }
        else
        {
            m_pendingRenderSnapshots.push_back(std::move(renderSnapshot));
        }
    }
    
    //------------------------------------------------------------------------------
    void Application::Init() noexcept
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
        
        m_platformSystem->SetPreferredFPS(m_appConfig->GetPreferredFPS());
    }
    
    //------------------------------------------------------------------------------
    void Application::Resume() noexcept
    {
        CS_LOG_VERBOSE("App Resuming...");
        
        //resume all of the application systems
        for (const AppSystemUPtr& system : m_systems)
        {
            system->OnResume();
        }
        
        m_stateManager->ResumeStates();
        
        CS_LOG_VERBOSE("App Finished Resuming...");
    }
    
    //------------------------------------------------------------------------------
    void Application::Foreground() noexcept
    {
        for (const AppSystemUPtr& system : m_systems)
        {
            system->OnForeground();
        }
        
        m_stateManager->ForegroundStates();
    }
    
    //------------------------------------------------------------------------------
    void Application::Update(f32 deltaTime, TimeIntervalSecs timestamp) noexcept
    {

#if CS_ENABLE_DEBUG
        //When debugging we may have breakpoints so restrict the time between
        //updates to something feasible.
        deltaTime = std::min(deltaTime, 0.5f);
#endif
        
        deltaTime *= m_updateSpeed;
        m_currentAppTime = timestamp;
        
        //We do not need to render as often as we update so this callback will be triggered
        //less freqenctly than the update frequency suggests. We must work out how many times to update based on the time since last frame
        //and our actual update frequency. We carry the remainder to the next frame until we have a full update cycle
        m_updateIntervalRemainder = std::min(m_updateIntervalRemainder + deltaTime, GetUpdateIntervalMax());
        
        //process any queued input received by the pointer system and gamepad system.
        if(m_pointerSystem != nullptr)
        {
            m_pointerSystem->ProcessQueuedInput();
        }
        
        if(m_gamepadSystem != nullptr)
        {
            m_gamepadSystem->ProcessQueuedInput();
        }
        
        bool isFirstFrame = (m_frameIndex == 0);
        while((m_updateIntervalRemainder >= GetUpdateInterval()) || isFirstFrame)
        {
            m_updateIntervalRemainder -= GetUpdateInterval();
            
            //update all of the application systems
            for (const AppSystemUPtr& system : m_systems)
            {
                system->OnFixedUpdate(GetUpdateInterval());
            }
            
            m_stateManager->FixedUpdateStates(GetUpdateInterval());
            
            isFirstFrame = false;
        }
        
        CoreTimer::Update(deltaTime);
        
        //update all of the application systems
        for (const AppSystemUPtr& system : m_systems)
        {
            system->OnUpdate(deltaTime);
        }
        
        m_stateManager->UpdateStates(deltaTime);
        
        m_taskScheduler->ExecuteMainThreadTasks();
        
        ProcessRenderSnapshotEvent();
        
        ++m_frameIndex;
    }
    
    //------------------------------------------------------------------------------
    void Application::Render() noexcept
    {
        m_renderer->ProcessRenderCommandBuffer();
    }
    
    //------------------------------------------------------------------------------
    void Application::MemoryWarning() noexcept
    {
        CS_LOG_VERBOSE("Memory Warning. Clearing resource cache...");
        
        //update all of the application systems
        for (const AppSystemUPtr& system : m_systems)
        {
            system->OnMemoryWarning();
        }
        
        m_stateManager->MemoryWarningStates();
    }
    
    //------------------------------------------------------------------------------
    void Application::Background() noexcept
    {
        m_stateManager->BackgroundStates();
        
        for (const AppSystemUPtr& system : m_systems)
        {
            system->OnBackground();
        }
    }
    
    //------------------------------------------------------------------------------
    void Application::Suspend() noexcept
    {
        CS_LOG_VERBOSE("App Suspending...");

        m_stateManager->SuspendStates();
        
        //suspend all application systems in reverse order.
        for (std::vector<AppSystemUPtr>::const_reverse_iterator it = m_systems.rbegin(); it != m_systems.rend(); ++it)
        {
            (*it)->OnSuspend();
        }
        
        CS_LOG_VERBOSE("App Finished Suspending...");
    }
    
    //------------------------------------------------------------------------------
    void Application::Destroy() noexcept
    {
        m_taskScheduler->Destroy();
        
        OnDestroy();
        
        m_stateManager->DestroyStates();
        
        //suspend all application systems in reverse order.
        for (std::vector<AppSystemUPtr>::const_reverse_iterator it = m_systems.rbegin(); it != m_systems.rend(); ++it)
        {
            (*it)->OnDestroy();
        }

        m_resourcePool->Destroy();
        
        //Needs to be destroyed after the resource pool as materials rely on this system
        m_renderMaterialGroupManager->Destroy();
        
        m_systems.clear();
        
        Logging::Destroy();
        
        s_application = nullptr;
    }

    //------------------------------------------------------------------------------
    Application::~Application() noexcept
    {
        CS_ASSERT(s_application == nullptr, "Application destructed without calling OnDestroy!");
    }
}
