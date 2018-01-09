//
//  Application.h
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

#ifndef _CHILLISOURCE_CORE_BASE_APPLICATION_H_
#define _CHILLISOURCE_CORE_BASE_APPLICATION_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/Screen.h>
#include <ChilliSource/Core/Base/SystemInfo.h>
#include <ChilliSource/Core/File/FileSystem.h>
#include <ChilliSource/Core/State/StateManager.h>
#include <ChilliSource/Core/System/AppSystem.h>

#include <limits>
#include <atomic>

namespace ChilliSource
{
    const u32 k_undefinedMaxResolution = std::numeric_limits<u32>::max();
    const f32 k_undefinedMaxDensity = std::numeric_limits<f32>::max();
    struct ResourceDirectoryInfo
    {
        std::string m_directory;
        u32 m_maxRes;
        f32 m_maxDensity;
        f32 m_resourcesDensity;
    };
    
    
    /// The main hub of the engine. The Application controls the overall application flow
    /// and contains all engine systems. This also acts as the entry point into a
    /// ChilliSource application. The user should override this class to include desired
    /// systems and add the initial State.
    ///
    /// Some methods in Application are thread-safe, but others are not.
    ///
    class Application
    {
    public:
        CS_DECLARE_NOCOPY(Application);
        
        /// This is thread safe.
        ///
        /// @return The application singleton instance pointer.
        ///
        static Application* Get() noexcept;
        
        /// Constructor
        ///
        Application(ChilliSource::SystemInfoCUPtr systemInfo) noexcept;
        
        /// Creates a new instance of the given system and adds it to the application.
        ///
        /// @param args
        ///     The arguments to the system constructor.
        ///
        /// @return A raw pointer to the new system.
        ///
        template <typename TSystem, typename... TArgs> TSystem* CreateSystem(TArgs&&... args) noexcept;
        
        /// Looks for a system that implements the queryable interface provided as a
        /// template parameter.
        ///
        /// This is thread-safe as long as it is called between the OnInit() and OnDestroy()
        /// application lifecycle events, inclusive.
        ///
        /// @return A pointer to the first system found that implements the named interface.
        ///
        template <typename TNamedType> TNamedType* GetSystem() noexcept;

        /// Looks for a system that implements the queryable interface provided as a
        /// template parameter.
        ///
        /// This is thread-safe as long as it is called between the OnInit() and OnDestroy()
        /// application lifecycle events, inclusive.
        ///
        /// @return A const pointer to the first system found that implements the named
        /// interface.
        ///
        template <typename TNamedType> const TNamedType* GetSystem() const noexcept;
        
        /// Looks for all systems that implement the given queryable interface provided
        /// as a template parameter.
        ///
        /// This is thread-safe as long as it is called between the OnInit() and
        /// OnDestroy() application lifecycle events, inclusive.
        ///
        /// @return A list of pointers to the systems that implement the given interface.
        ///
        template <typename TNamedType> std::vector<TNamedType*> GetSystems() noexcept;

        /// Looks for all systems that implement the given queryable interface provided
        /// as a template parameter.
        ///
        /// This is thread-safe as long as it is called between the OnInit() and
        /// OnDestroy() application lifecycle events, inclusive.
        ///
        /// @return A list of const pointers to the systems that implement the given
        /// interface.
        ///
        template <typename TNamedType> std::vector<const TNamedType*> GetSystems() const noexcept;

        /// Returns the version number of the application on the current platform
        /// as a string.
        ///
        /// @return The version string.
        ///
        const std::string& GetAppVersion() const noexcept;

        /// This is thread-safe.
        ///
        /// @return The unique index of the current frame.
        ///
        u32 GetFrameIndex() const noexcept;
        
        /// Returns the elapsed time since the application started running in
        /// seconds.
        ///
        /// This is not thread-safe and should only be called from the main thread.
        ///
        /// @return The time in seconds.
        ///
        TimeIntervalSecs GetAppElapsedTime() const noexcept;

        /// Returns the system clock time in seconds since epoch.
        ///
        /// @return The time in seconds.
        ///
        TimeIntervalSecs GetSystemTime() const noexcept;

        /// Return the system clock time in milliseconds since epoch.
        ///
        /// @return The time in milliseconds.
        ///
        TimeIntervalSecs GetSystemTimeInMilliseconds() const noexcept;

        /// Set the time between update calls to adjust the frame rate.
        ///
        /// @param updateInterval
        ///     Time between update calls
        ///
        void SetUpdateInterval(f32 updateInterval) noexcept;
        
        /// This is not thread-safe and should only be called on the main thread.
        ///
        /// @return The time between update calls.
        ///
        f32 GetUpdateInterval() const noexcept;

        /// Returns the maximum amount of time to be processed a single update frame.
        ///
        /// This is not thread-safe and should only be called on the main thread.
        ///
        /// @return Max time to be processed in a single frame.
        ///
        f32 GetUpdateIntervalMax() const noexcept;

        /// Sets a multiplier for slowing or speeding up the delta time passed to
        /// each system and state.
        ///
        /// This is not thread-safe and should only be called on the main thread.
        ///
        /// @param speed
        ///     Scaler to speed up or slow down update time.
        ///
        void SetUpdateSpeed(f32 speed) noexcept;

        /// Stop the application and exit gracefully
        ///
        /// This is not thread-safe and should only be called on the main thread.
        ///
        void Quit() noexcept;
        
        /// This is thread-safe as long as it is called between the init and
        /// destroy application lifecycle events, inclusive.
        ///
        /// @return A pointer to the state manager.
        ///
        StateManager* GetStateManager() noexcept;

        /// This is thread-safe as long as it is called between the init and
        /// destroy application lifecycle events, inclusive.
        ///
        /// @return A const pointer to the state manager.
        ///
        const StateManager* GetStateManager() const noexcept;

        /// This is thread-safe as long as it is called between the init and
        /// destroy application lifecycle events, inclusive.
        ///
        /// @return A pointer to the file system.
        ///
        FileSystem* GetFileSystem() noexcept;

        /// This is thread-safe as long as it is called between the init and
        /// destroy application lifecycle events, inclusive.
        ///
        /// @return A const pointer to the file system.
        ///
        const FileSystem* GetFileSystem() const noexcept;

        /// This is thread-safe as long as it is called between the init and
        /// destroy application lifecycle events, inclusive.
        ///
        /// @return A pointer to the system that resolves path based on the device
        /// config
        ///
        TaggedFilePathResolver* GetTaggedFilePathResolver() noexcept;

        /// This is thread-safe as long as it is called between the init and
        /// destroy application lifecycle events, inclusive.
        ///
        /// @return A const pointer to the system that resolves path based on the
        /// device config
        ///
        const TaggedFilePathResolver* GetTaggedFilePathResolver() const noexcept;

        /// This is thread-safe as long as it is called between the init and
        /// destroy application lifecycle events, inclusive.
        ///
        /// @return A pointer to the task scheduler.
        ///
        TaskScheduler* GetTaskScheduler() noexcept;

        /// This is thread-safe as long as it is called between the init and
        /// destroy application lifecycle events, inclusive.
        ///
        /// @author Ian Copland
        ///
        /// @return A const pointer to the task scheduler.
        ///
        const TaskScheduler* GetTaskScheduler() const noexcept;

        /// This is thread-safe as long as it is called between the init and
        /// destroy application lifecycle events, inclusive.
        ///
        /// @return A pointer to the resource pool system.
        ///
        ResourcePool* GetResourcePool() noexcept;
        
        /// This is thread-safe as long as it is called between the init and
        /// destroy application lifecycle events, inclusive.
        ///
        /// @return A const pointer to the resource pool system.
        ///
        const ResourcePool* GetResourcePool() const noexcept;
        
        /// This is thread-safe as long as it is called between the init and
        /// destroy application lifecycle events, inclusive.
        ///
        /// @return A pointer to the App Config.
        ///
        AppConfig* GetAppConfig() noexcept;
        
        /// This is thread-safe as long as it is called between the init and
        /// destroy application lifecycle events, inclusive.
        ///
        /// @return A const pointer to the App Config.
        ///
        const AppConfig* GetAppConfig() const noexcept;
        
        /// This is thread-safe as long as it is called between the init and
        /// destroy application lifecycle events, inclusive.
        ///
        /// @return A pointer to the screen system.
        ///
        Screen* GetScreen() noexcept;
        
        /// This is thread-safe as long as it is called between the init and
        /// destroy application lifecycle events, inclusive.
        ///
        /// @return A const pointer to the screen system.
        ///
        const Screen* GetScreen() const noexcept;
        
        /// This is thread-safe as long as it is called between the init and
        /// destroy application lifecycle events, inclusive.
        ///
        /// @return A pointer to the widget factory system.
        ///
        WidgetFactory* GetWidgetFactory() noexcept;
        
        /// This is thread-safe as long as it is called between the init and
        /// destroy application lifecycle events, inclusive.
        ///
        /// @return A const pointer to the widget factory system.
        ///
        const WidgetFactory* GetWidgetFactory() const noexcept;

        virtual ~Application() noexcept;
        
    protected:
        /// The users application should override this to add desired systems.
        /// Systems can only be added in this method.
        ///
        /// This will always be called on the main thread.
        ///
        virtual void CreateSystems() noexcept = 0;
        
        /// Initialisation method called after all systems have been set up and
        /// before the first state is pushed. Application initialisation code
        /// should be in here.
        ///
        /// This will always be called on the main thread.
        ///
        virtual void OnInit() noexcept = 0;
        
        /// Give the state manager the initial state. This should be overriden by
        /// the users application to add initial state.
        ///
        /// This will always be called on the main thread.
        ///
        virtual void PushInitialState() noexcept = 0;
        
        /// Destuction method called just before all systems and states are released.
        /// Application destruction code should be in here.
        ///
        /// This will always be called on the main thread.
        ///
        virtual void OnDestroy() noexcept = 0;
        
    private:
        friend class LifecycleManager;
        
        /// Systems that are required by CS are added in this method
        ///
        void CreateDefaultSystems() noexcept;
        
        /// Once the systems have been created they are then added to the pool and
        /// initialised.
        ///
        void PostCreateSystems() noexcept;
        
        /// Sends the render snapshot event to all active systems and components
        /// that are current in the scene to generate the render snapshot. This
        /// snapshot is then passed to the render pipeline for processing. This is done
        /// for all enabled scenes
        ///
        void ProcessRenderSnapshotEvent() noexcept;
        
        /// Initialises the application, creating all systems and sending the init
        /// lifecycle event to all systems.
        ///
        /// This is called by the LifecycleManager.
        ///
        void Init() noexcept;
        
        /// Resumes the application from the suspended state, and sends the resume
        /// lifecycle event to all systems.
        ///
        /// This is called by the LifecycleManager.
        ///
        void Resume() noexcept;
        
        /// Foregrounds the application and sends the foreground lifecycle event to
        /// all systems.
        ///
        /// This is called by the LifecycleManager.
        ///
        void Foreground() noexcept;
        
        /// Updates the application and sends the update lifecycle event and fixed
        /// update lifecycle event to all systems.
        ///
        /// This is called by the LifecycleManager.
        ///
        /// @param deltaTime
        ///     The time that has passed since the last update.
        /// @param timestamp
        ///     The frame timestamp.
        ///
        void Update(f32 deltaTime, TimeIntervalSecs timestamp) noexcept;
        
        /// Sends the render snapshot event to all systems and then initiates the
        /// render pipeline.
        ///
        /// This is called on the render thread.
        ///
        void Render() noexcept;
        
        /// Triggered on receiving a "application memory warning" event. This will
        /// notify active resource managers to purge their caches.
        ///
        /// This is called by the LifecycleManager.
        ///
        void MemoryWarning() noexcept;
        
        /// Backgrounds the application and sends the background lifecycle event to
        /// all systems.
        ///
        /// This is called by the LifecycleManager.
        ///
        void Background() noexcept;
        
        /// Suspends the application and sends the suspend lifecycle event to all
        /// systems.
        ///
        /// This is called by the LifecycleManager.
        ///
        void Suspend() noexcept;
        
        /// Sends the destroy lifecycle event to all systems and then deallocates
        /// them. All background tasks will be processed before the destroy event.
        ///
        /// This is called by the LifecycleManager.
        ///
        void Destroy() noexcept;
        
        friend class Scene;
        
        /// Takes a snapshot of the given scene and renders it to its
        /// render target during the render stage of the application.
        /// This is used mostly for single shot render to texture
        /// NOTE: This means the texture of the target will not be populated til next frame.
        ///
        /// @param scene
        ///     Scene that should be rendered
        /// @param target
        ///     Optional, if wanting to render the scene to a target other than its own
        ///
        void RenderScene(Scene* scene, TargetGroup* target = nullptr) noexcept;
        
        std::vector<RenderSnapshot> m_pendingRenderSnapshots;
        std::vector<AppSystemUPtr> m_systems;
        
        ResourcePool* m_resourcePool = nullptr;
        StateManager* m_stateManager = nullptr;
        TaskScheduler* m_taskScheduler = nullptr;
        Renderer* m_renderer = nullptr;
        RenderMaterialGroupManager* m_renderMaterialGroupManager = nullptr;
        Screen* m_screen = nullptr;
        PlatformSystem* m_platformSystem = nullptr;
        FileSystem* m_fileSystem = nullptr;
        TaggedFilePathResolver* m_taggedPathResolver = nullptr;
        PointerSystem* m_pointerSystem = nullptr;
        GamepadSystem* m_gamepadSystem = nullptr;
        AppConfig* m_appConfig = nullptr;
        WidgetFactory* m_widgetFactory = nullptr;

        SystemInfoCUPtr m_systemInfo;
        
        std::atomic<u32> m_frameIndex;
        TimeIntervalSecs m_currentAppTime = 0;
        f32 m_updateInterval;
        f32 m_updateSpeed = 1.0f;
        f32 m_updateIntervalRemainder = 0.0f;
        bool m_isSystemCreationAllowed = false;
        std::string m_appVersion;
        
        static Application* s_application;
    };
    
    //------------------------------------------------------------------------------
    template <typename TSystem, typename... TArgs> TSystem* Application::CreateSystem(TArgs&&... args) noexcept
    {
        CS_ASSERT(m_isSystemCreationAllowed == true, "Cannot add systems outwith the creation phase");
        
        std::unique_ptr<TSystem> newSystem = TSystem::Create(std::forward<TArgs>(args)...);
        TSystem* output = newSystem.get();
        if (newSystem != nullptr)
        {
            m_systems.push_back(std::move(newSystem));
        }
        
        return output;
    }
    
    //------------------------------------------------------------------------------
    template <typename TNamedType> TNamedType* Application::GetSystem() noexcept
    {
        for (const auto& system : m_systems)
        {
            if (system->IsA<TNamedType>() == true)
            {
                return static_cast<TNamedType*>(system.get());
            }
        }
        
        return nullptr;
    }
    
    //------------------------------------------------------------------------------
    template <typename TNamedType> const TNamedType* Application::GetSystem() const noexcept
    {
        for (const auto& system : m_systems)
        {
            if (system->IsA<TNamedType>() == true)
            {
                return static_cast<TNamedType*>(system.get());
            }
        }
        
        return nullptr;
    }
    
    //------------------------------------------------------------------------------
    template <typename TNamedType> std::vector<TNamedType*> Application::GetSystems() noexcept
    {
        std::vector<TNamedType*> output;
        
        for (const auto& system : m_systems)
        {
            if (system->IsA<TNamedType>() == true)
            {
                output.push_back(static_cast<TNamedType*>(system.get()));
            }
        }
        
        return output;
    }
    
    //------------------------------------------------------------------------------
    template <typename TNamedType> std::vector<const TNamedType*> Application::GetSystems() const noexcept
    {
        std::vector<const TNamedType*> output;
        
        for (const auto& system : m_systems)
        {
            if (system->IsA<TNamedType>() == true)
            {
                output.push_back(static_cast<TNamedType*>(system.get()));
            }
        }
        
        return output;
    }
}

/// Factory method that must be implemented by the the derived application.
///
/// This is not thread safe and must always be called on the system thread.
///
/// @return Instance of concrete CS application
///
ChilliSource::Application* CreateApplication(ChilliSource::SystemInfoCUPtr systemInfo) noexcept;

#endif
