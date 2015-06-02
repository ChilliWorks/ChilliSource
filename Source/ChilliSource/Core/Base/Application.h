//
//  Application.h
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

#ifndef _CHILLISOURCE_CORE_BASE_APPLICATION_H_
#define _CHILLISOURCE_CORE_BASE_APPLICATION_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/Screen.h>
#include <ChilliSource/Core/File/FileSystem.h>
#include <ChilliSource/Core/State/StateManager.h>
#include <ChilliSource/Core/System/AppSystem.h>

#include <limits>

namespace ChilliSource
{
	namespace Core
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
        
		//-----------------------------------------------------------
		/// The main hub of the engine. The Application controls the
        /// overall application flow and contains all engine systems.
        /// This also acts as the entry point into a Chilli Source
        /// application. The user should override this class to include
        /// desired systems and add the initial State.
        ///
        /// @author S Downie
		//-----------------------------------------------------------
		class Application
		{
		public:
            CS_DECLARE_NOCOPY(Application);
            //----------------------------------------------------
			/// Returns the global application instance.
            ///
            /// @author I Copland
            ///
            /// @return The application instance pointer.
			//----------------------------------------------------
            static Application* Get();
            //----------------------------------------------------
			/// Constructor
            ///
            /// @author S Downie
			//----------------------------------------------------
			Application();
            //----------------------------------------------------
            /// Creates a new instance of the given system and
            /// adds it to the application.
            ///
            /// @author I Copland
            ///
            /// @param The arguments to the system constructor.
            ///
            /// @return A raw pointer to the new system.
 			//----------------------------------------------------
            template <typename TSystem, typename... TArgs> TSystem* CreateSystem(TArgs&&... in_args);
            //----------------------------------------------------
            /// Looks for a system that implements the queryable
            /// interface provided as a template parameter.
            ///
            /// @author I Copland
            ///
            /// @return A pointer to the first system found that
            /// implements the named interface.
            //----------------------------------------------------
            template <typename TNamedType> TNamedType* GetSystem();
            //----------------------------------------------------
			/// Looks for a system that implements the queryable
            /// interface provided as a template parameter.
            ///
            /// @author I Copland
            ///
			/// @return A const pointer to the first system found
            /// that implements the named interface.
			//----------------------------------------------------
			template <typename TNamedType> const TNamedType* GetSystem() const;
            //-----------------------------------------------------
            /// Looks for all systems that implement the given
            /// queryable interface provided as a template parameter.
            ///
            /// @author I Copland
            ///
            /// @return A list of pointers to the systems that
            /// implement the given interface.
            //-----------------------------------------------------
            template <typename TNamedType> std::vector<TNamedType*> GetSystems();
			//-----------------------------------------------------
			/// Looks for all systems that implement the given
            /// queryable interface provided as a template parameter.
			///
            /// @author I Copland
            ///
			/// @return A list of const pointers to the systems that
            /// implement the given interface.
			//-----------------------------------------------------
            template <typename TNamedType> std::vector<const TNamedType*> GetSystems() const;
			//-----------------------------------------------------
            /// Returns the version number of the application on
            /// the current platform as a string.
            ///
            /// @author S Downie.
            ///
			/// @return The version string.
			//-----------------------------------------------------
			std::string GetAppVersion() const;
			//-----------------------------------------------------
			/// Returns the elapsed time since the application
            /// started running in seconds.
			///
            /// @author S Downie.
            ///
			/// @return The time in seconds.
			//-----------------------------------------------------
			TimeIntervalSecs GetAppElapsedTime() const;
            //-----------------------------------------------------
            /// Returns the system clock time in seconds since epoch.
            ///
            /// @author S Downie.
            ///
            /// @return The time in seconds.
            //-----------------------------------------------------
            TimeIntervalSecs GetSystemTime() const;
			//-----------------------------------------------------
            /// Return the system clock time in milliseconds since
            /// epoch.
            ///
            /// @author S Downie.
            ///
            /// @return The time in milliseconds.
            //-----------------------------------------------------
            TimeIntervalSecs GetSystemTimeInMilliseconds() const;
			//-----------------------------------------------------
			/// Set the time between update calls to adjust the
            /// frame rate.
            ///
            /// @author S Downie.
			///
			/// @param Time between update calls
			//-----------------------------------------------------
			void SetUpdateInterval(f32 in_updateInterval);
			//-----------------------------------------------------
			/// Returns the time between update calls.
            ///
            /// @author S Downie.
			///
			/// @return Time between update calls
			//-----------------------------------------------------
			f32 GetUpdateInterval() const;
			//-----------------------------------------------------
			/// Returns the maximum amount of time to be processed
            /// a single update frame.
            ///
            /// @author S Downie.
			///
			/// @return Max time to be processed in a single frame.
			//-----------------------------------------------------
			f32 GetUpdateIntervalMax() const;
            //-----------------------------------------------------
			/// Sets a multiplier for slowing or speeding up the
            /// delta time passed to each system and state.
            ///
            /// @author I Copland
            ///
			/// @param Scaler to speed up or slow down update time.
			//-----------------------------------------------------
			void SetUpdateSpeed(f32 in_speed);
            //-----------------------------------------------------
            /// Stop the application and exit gracefully
            ///
            /// @author S Downie.
            //-----------------------------------------------------
            void Quit();
            //-----------------------------------------------------
            /// @author S Downie
            ///
            /// @return A pointer to the state manager.
            //-----------------------------------------------------
            StateManager* GetStateManager();
            //-----------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return A const pointer to the state manager.
            //-----------------------------------------------------
            const StateManager* GetStateManager() const;
            //-----------------------------------------------------
            /// @author S Downie.
            ///
            /// @return A pointer to the renderer.
            //-----------------------------------------------------
            Rendering::Renderer* GetRenderer();
            //-----------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return A const pointer to the renderer.
            //-----------------------------------------------------
            const Rendering::Renderer* GetRenderer() const;
            //-----------------------------------------------------
            /// @author S Downie.
            ///
            /// @return A pointer to the platform specific render
            /// system
            //-----------------------------------------------------
            Rendering::RenderSystem* GetRenderSystem();
            //-----------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return A const pointer to the platform specific
            /// render system
            //-----------------------------------------------------
            const Rendering::RenderSystem* GetRenderSystem() const;
            //-----------------------------------------------------
            /// @author I Copland
            ///
            /// @return A pointer to the file system.
            //-----------------------------------------------------
            FileSystem* GetFileSystem();
            //-----------------------------------------------------
            /// @author I Copland
            ///
            /// @return A const pointer to the file system.
            //-----------------------------------------------------
            const FileSystem* GetFileSystem() const;
            //-----------------------------------------------------
            /// @author S Downie
            ///
            /// @return A pointer to the system that resolves path
            /// based on the device config
            //-----------------------------------------------------
            TaggedFilePathResolver* GetTaggedFilePathResolver();
            //-----------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return A const pointer to the system that resolves
            /// path based on the device config
            //-----------------------------------------------------
            const TaggedFilePathResolver* GetTaggedFilePathResolver() const;
            //-----------------------------------------------------
            /// @author S Downie
            ///
            /// @return A pointer to the task scheduler.
            //-----------------------------------------------------
            TaskScheduler* GetTaskScheduler();
            //-----------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return A const pointer to the task scheduler.
            //-----------------------------------------------------
            const TaskScheduler* GetTaskScheduler() const;
            //-----------------------------------------------------
            /// @author S Downie
            ///
            /// @return A pointer to the resource pool system.
            //-----------------------------------------------------
            ResourcePool* GetResourcePool();
            //-----------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return A const pointer to the resource pool system.
            //-----------------------------------------------------
            const ResourcePool* GetResourcePool() const;
            //-----------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return A pointer to the App Config.
            //-----------------------------------------------------
            AppConfig* GetAppConfig();
            //-----------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return A const pointer to the App Config.
            //-----------------------------------------------------
            const AppConfig* GetAppConfig() const;
            //-----------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return A pointer to the screen system.
            //-----------------------------------------------------
            Screen* GetScreen();
            //-----------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return A const pointer to the screen system.
            //-----------------------------------------------------
            const Screen* GetScreen() const;
            //-----------------------------------------------------
            /// @author S Downie
            ///
            /// @return A pointer to the widget factory system.
            //-----------------------------------------------------
            UI::WidgetFactory* GetWidgetFactory();
            //-----------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return A const pointer to the widget factory system.
            //-----------------------------------------------------
            const UI::WidgetFactory* GetWidgetFactory() const;
            //----------------------------------------------------
			/// Initialises the application and kicks off the update
            /// loop. This should not be called by a users application.
            ///
            /// @author I Copland
			//----------------------------------------------------
			void Init();
            //----------------------------------------------------
			/// Resumes application from suspended state. This should
            /// not be called by a users application.
            ///
            /// @author  S Downie.
			//----------------------------------------------------
			void Resume();
            //----------------------------------------------------
			/// Triggered when the application is pushed
            /// to the front of the view stack. This should
            /// not be called by a users application.
            ///
            /// @author S Downie
			//----------------------------------------------------
			void Foreground();
            //----------------------------------------------------
			/// Triggered by an update event. This will update
            /// the application, systems and states. This should
            /// not be called by a users application.
            ///
            /// @author S Downie
            ///
            /// @param The delta time.
            /// @param The frame timestamp.
			//----------------------------------------------------
			void Update(f32 in_deltaTime, TimeIntervalSecs in_timestamp);
            //----------------------------------------------------
            /// The main draw loop of the application that will
            /// render to the screen. This should not be called
            /// by a users application.
            //----------------------------------------------------
            void Render();
			//----------------------------------------------------
			/// Triggered on receiving a "application memory warning"
            /// event. This will notify active resource managers to
            /// purge their caches. This should not be called by
            /// a users application.
            ///
            /// @author S Downie
			//----------------------------------------------------
			void ApplicationMemoryWarning();
            //----------------------------------------------------
			/// Triggered when the application is pushed back from
            /// the front of the view stack. This should
            /// not be called by a users application.
            ///
            /// @author S Downie
			//----------------------------------------------------
			void Background();
            //----------------------------------------------------
            /// Triggered on receiving a "application will suspend"
            /// message. This will notify active states to pause
            /// and tell the sub systems to stop. This should not
            /// be called by a users application.
            ///
            /// @author S Downie
			//----------------------------------------------------
			void Suspend();
            //----------------------------------------------------
            /// Releases all systems and states and cleans up the
            /// application. This should not be called by a users
            /// application.
            ///
            /// @author I Copland
			//----------------------------------------------------
			void Destroy();
            //------------------------------------------------------
			/// Destructor
			//------------------------------------------------------
            virtual ~Application();
		protected:
            //------------------------------------------------------
			/// The users application should override this to add
            /// desired systems. Systems can only be added in this
            /// method.
            ///
            /// @author S Downie
			//------------------------------------------------------
			virtual void CreateSystems() = 0;
            //------------------------------------------------------
			/// Initialisation method called after all systems have
            /// been set up and before the first state is pushed.
            /// Application initialisation code should be in here.
            ///
            /// @author I Copland
			//------------------------------------------------------
			virtual void OnInit() = 0;
            //------------------------------------------------------
			/// Give the state manager the initial state. This should
            /// be overriden by the users application to add initial
            /// state.
            ///
            /// @author S Downie
			//------------------------------------------------------
			virtual void PushInitialState() = 0;
            //------------------------------------------------------
			/// Destuction method called just before all systems
            /// and states are released. Application destruction
            /// code should be in here.
            ///
            /// @author I Copland
			//------------------------------------------------------
			virtual void OnDestroy() = 0;
        private:
            //------------------------------------------------------
            /// Systems that are required by CS are added in this
            /// method
            ///
            /// @author S Downie
			//------------------------------------------------------
            void CreateDefaultSystems();
            //------------------------------------------------------
			/// Once the systems have been created they are then
            /// added to the pool and initialised.
            ///
            /// @author S Downie
			//------------------------------------------------------
			void PostCreateSystems();
            //------------------------------------------------------
            /// A single update cycle that updates all updateables,
            /// timers and the active state This can be called multiple
            /// times per frame depending on fixed updates.
            ///
            /// @author S Downie
            ///
            /// @param Time between frames
            //------------------------------------------------------
			void OnUpdate(f32 in_deltaTime);
            //---------------------------------------------------
            /// Resumes the application. This will be called when
            /// at the start of the next update following the On
            /// Resume event.
            ///
            /// @author S Downie
            //---------------------------------------------------
            void OnResume();
            //---------------------------------------------------
            /// Foregrounds the application. This will be called when
            /// at the start of the next update following the On
            /// Foreground event.
            ///
            /// @author S Downie
            //---------------------------------------------------
            void OnForeground();

        private:
            std::vector<AppSystemUPtr> m_systems;
            
            ResourcePool* m_resourcePool = nullptr;
			StateManager* m_stateManager = nullptr;
			TaskScheduler* m_taskScheduler = nullptr;
            Screen* m_screen = nullptr;
			Rendering::Renderer* m_renderer = nullptr;
            Rendering::RenderSystem* m_renderSystem = nullptr;
            PlatformSystem* m_platformSystem = nullptr;
            FileSystem* m_fileSystem = nullptr;
            TaggedFilePathResolver* m_taggedPathResolver = nullptr;
            Input::PointerSystem* m_pointerSystem = nullptr;
            AppConfig* m_appConfig = nullptr;
            UI::WidgetFactory* m_widgetFactory = nullptr;
            
			TimeIntervalSecs m_currentAppTime = 0;
			f32 m_updateInterval;
            f32 m_updateSpeed = 1.0f;
            f32 m_updateIntervalRemainder = 0.0f;
            
            bool m_shouldNotifyConnectionsResumeEvent = false;
            bool m_shouldNotifyConnectionsForegroundEvent = false;
            bool m_isFirstFrame = true;
            bool m_isSuspending = false;
            bool m_isSystemCreationAllowed = false;
            
            static Application* s_application;
		};
        //----------------------------------------------------
        //----------------------------------------------------
        template <typename TSystem, typename... TArgs> TSystem* Application::CreateSystem(TArgs&&... in_args)
        {
            CS_ASSERT(m_isSystemCreationAllowed == true, "Cannot add systems outwith the creation phase");
            
            std::unique_ptr<TSystem> newSystem = TSystem::Create(std::forward<TArgs>(in_args)...);
			TSystem* output = newSystem.get();
			if (newSystem != nullptr)
			{
				m_systems.push_back(std::move(newSystem));
			}
            
            return output;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        template <typename TNamedType> TNamedType* Application::GetSystem()
        {
            for (const auto& system : m_systems)
            {
                if (system->IsA<TNamedType>() == true)
                {
                    return static_cast<TNamedType*>(system.get());
                }
            }
            
            CS_LOG_WARNING("Application cannot find system: " + TNamedType::TypeName);
            return nullptr;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        template <typename TNamedType> const TNamedType* Application::GetSystem() const
        {
            for (const auto& system : m_systems)
            {
                if (system->IsA<TNamedType>() == true)
                {
                    return static_cast<TNamedType*>(system.get());
                }
            }
			
			CS_LOG_WARNING("Application cannot find system: " + TNamedType::TypeName);
			return nullptr;
        }
        //-----------------------------------------------------
        //-----------------------------------------------------
        template <typename TNamedType> std::vector<TNamedType*> Application::GetSystems()
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
        //-----------------------------------------------------
        //-----------------------------------------------------
        template <typename TNamedType> std::vector<const TNamedType*> Application::GetSystems() const
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
}

//--------------------------------------------------------
/// Factory method that must be implemented by the
/// the derived application.
///
/// @author S Downie
///
/// @return Instance of concrete CS application
//--------------------------------------------------------
CSCore::Application* CreateApplication();

#endif
