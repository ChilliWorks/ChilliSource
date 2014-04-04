//
//  Application.h
//  Chilli Source
//
//  Created by S Downie on 23/09/2010.
//  Copyright 2010 Tag Games. All rights reserved.
//


#ifndef _CHILLISOURCE_CORE_BASE_APPLICATION_H_
#define _CHILLISOURCE_CORE_BASE_APPLICATION_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/State/StateManager.h>
#include <ChilliSource/Core/Base/Screen.h>
#include <ChilliSource/Core/System/AppSystem.h>
#include <ChilliSource/Core/System/System.h>
#include <ChilliSource/Core/File/FileSystem.h>

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
            /// Adds a system to the application. This should only
            /// be called inside the CreateSystem() method.
            ///
            /// @author I Copland
            ///
            /// @param The new system to add to the application.
 			//----------------------------------------------------
            void AddSystem_Old(SystemUPtr in_system);
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
            template <typename TSystem, typename... TArgs> TSystem* CreateSystem(TArgs... in_args);
			//----------------------------------------------------
			/// Looks for a system that implements the queryable
            /// interface provided as a template parameter.
            ///
            /// @author I Copland
            ///
			/// @return The first system found that implements
            /// the named interface.
			//----------------------------------------------------
			template <typename TNamedType> TNamedType* GetSystem_Old();
			//-----------------------------------------------------
			/// Looks for a all systems that implement the given
            /// queryable interface provided as a template parameter.
			///
            /// @author I Copland
            ///
			/// @param [Out] The list of systems that implement the
            /// queryable interface.
			//-----------------------------------------------------
            template <typename TNamedType> void GetSystems_Old(std::vector<TNamedType*> & out_systems);
            //----------------------------------------------------
			/// Looks for a system that implements the queryable
            /// interface provided as a template parameter.
            ///
            /// @author I Copland
            ///
			/// @return The first system found that implements
            /// the named interface.
			//----------------------------------------------------
			template <typename TNamedType> TNamedType* GetSystem();
			//-----------------------------------------------------
			/// Looks for a all systems that implement the given
            /// queryable interface provided as a template parameter.
			///
            /// @author I Copland
            ///
			/// @param [Out] The list of systems that implement the
            /// queryable interface.
			//-----------------------------------------------------
            template <typename TNamedType> void GetSystems(std::vector<TNamedType*> & out_systems);
			//-----------------------------------------------------
            /// Returns the version number of the application on
            /// the current platform as a string.
            ///
            /// @author S Downie.
            ///
			/// @return The version string.
			//-----------------------------------------------------
			std::string GetAppVersion();
			//-----------------------------------------------------
			/// Returns the elapsed time since the application
            /// started running in seconds.
			///
            /// @author S Downie.
            ///
			/// @return The time in seconds.
			//-----------------------------------------------------
			TimeIntervalSecs GetAppElapsedTime();
            //-----------------------------------------------------
            /// Returns the system clock time in seconds since epoch.
            ///
            /// @author S Downie.
            ///
            /// @return The time in seconds.
            //-----------------------------------------------------
            TimeIntervalSecs GetSystemTime();
			//-----------------------------------------------------
            /// Return the system clock time in milliseconds since
            /// epoch.
            ///
            /// @author S Downie.
            ///
            /// @return The time in milliseconds.
            //-----------------------------------------------------
            TimeIntervalSecs GetSystemTimeInMilliseconds();
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
			f32 GetUpdateInterval();
			//-----------------------------------------------------
			/// Returns the maximum amount of time to be processed
            /// a single update frame.
            ///
            /// @author S Downie.
			///
			/// @return Max time to be processed in a single frame.
			//-----------------------------------------------------
			f32 GetUpdateIntervalMax();
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
			/// Returns the default font described in the App.config
            /// file.
			///
            /// @author S Downie.
            ///
            /// @return The default font.
            //-----------------------------------------------------
            const Rendering::FontSPtr& GetDefaultFont() const;
            //-----------------------------------------------------
			/// Returns the default mesh described in the App.config
            /// file.
            ///
            /// @author S Downie.
			///
            /// @return The default mesh.
            //-----------------------------------------------------
            const Rendering::MeshCSPtr& GetDefaultMesh() const;
            //-----------------------------------------------------
			/// Returns the default material described in the
            /// App.config file.
            ///
            /// @author S Downie.
			///
            /// @return The default material.
            //-----------------------------------------------------
            const Rendering::MaterialSPtr& GetDefaultMaterial() const;
			//-----------------------------------------------------
			/// Returns a pointer to the state manager.
            ///
            /// @author S Downie.
			///
			/// @return Handle to application state manager
			//-----------------------------------------------------
			inline StateManager* GetStateManager()
            {
                return m_stateManager;
            }
			//-----------------------------------------------------
			/// Returns a pointer to the renderer.
            ///
            /// @author S Downie.
			///
			/// @return Handle application renderer
			//-----------------------------------------------------
			inline Rendering::Renderer* GetRenderer()
            {
                return m_renderer.get();
            }
			//-----------------------------------------------------
			/// Returns a pointer to the render system.
            ///
            /// @author S Downie.
			///
			/// @return Handle to platfrom specific render system
			//-----------------------------------------------------
			inline Rendering::RenderSystem* GetRenderSystem()
            {
                return m_renderSystem;
            }
            //-----------------------------------------------------
			/// Returns a pointer to the platform system.
            ///
            /// @author S Downie.
			///
            /// @return Pointer to the platform system
            //-----------------------------------------------------
			inline PlatformSystem* GetPlatformSystem()
            {
                return m_platformSystem.get();
            }
			//-----------------------------------------------------
			/// Returns a pointer to the audio system.
            ///
            /// @author S Downie.
			///
			/// @return Pointer to the input system
			//-----------------------------------------------------
			inline Audio::AudioSystem* GetAudioSystem()
            {
                return m_audioSystem;
            }
			//-----------------------------------------------------
			/// Returns a pointer to the file system.
            ///
            /// @author I Copland
			///
			/// @return Pointer to the file system
			//-----------------------------------------------------
			inline FileSystem* GetFileSystem()
            {
                return m_fileSystem;
            }
            //-----------------------------------------------------
			/// Returns a pointer to the resource pool system.
            ///
            /// @author S Downie
			///
			/// @return Pointer to the resource pool
			//-----------------------------------------------------
			inline ResourcePool* GetResourcePool()
            {
                return m_resourcePool;
            }
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
			/// Triggered on receiving a "orientation changed"
            /// event. Used to tell the camera and input to rotate.
            /// This should not be called by a users application.
            ///
            /// @author S Downie
            ///
            /// @param The new orientation.
			//----------------------------------------------------
			void ScreenChangedOrientation(ScreenOrientation in_orientation);
			//----------------------------------------------------
			/// Triggered on receiving a "screen resized" event.
            /// This should not be called by a users application.
            ///
            /// @author S Downie
            ///
            /// @param The new width.
            /// @param The new height.
			//----------------------------------------------------
			void ScreenResized(u32 in_width, u32 in_height);
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
			/// Triggered on receiving a "go back" event. This is
            /// typically caused by pressing a physical back button
            /// on the device, For example the Android back button.
            /// This should not be called by a users application.
            ///
            /// @author I Copland
			//----------------------------------------------------
			void GoBack();
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
            /// Give the engine the available resource directories
            /// and the info required to decide which directory the
            /// current device should use.
            ///
            /// @author S Downie
            ///
            /// @param [Out] Vector of directory info
            /// @param [Out] The name of the directory to fall back
            /// on for resolution dependant assets
            /// @param [Out] The name of the directory to use as the
            /// default (i.e. for shared assets)
            //------------------------------------------------------
            virtual void SetResourceDirectories(std::vector<ResourceDirectoryInfo>& out_resDependantDirectoryInfos, std::string& out_resDefaultDirectory, std::string& out_defaultDirectory) = 0;
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
            /// Open the app config file and load any of the specified
            /// resources to act as the defaults.
            ///
            /// @author S Downie
            //------------------------------------------------------
            void LoadDefaultResources();
            //------------------------------------------------------
            /// Depedending on the device decide which folders
            /// resources should be loaded from.
            ///
            /// @author S Downie
            //------------------------------------------------------
            void DetermineResourceDirectories();
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
            //------------------------------------------------------
			/// Tell the active camera to roate its view and if we
            /// are using touch input we must rotate the input
            /// co-ordinates.
            ///
            /// @author S Downie
            ///
			/// @param Screen orientation flag
			//------------------------------------------------------
			void SetOrientation(ScreenOrientation inOrientation);

        private:
            std::vector<AppSystemUPtr> m_systems;
            
            std::vector<SystemUPtr> m_systemsOld;
            std::vector<IUpdateable*> m_updateableSystems;
            
            ResourcePool* m_resourcePool;
			StateManager* m_stateManager;
			Rendering::RendererUPtr m_renderer;
            Rendering::RenderSystem* m_renderSystem;
            PlatformSystemUPtr m_platformSystem;
            Audio::AudioSystem* m_audioSystem;
            FileSystem* m_fileSystem;
            Input::PointerSystem* m_pointerSystem;

			ScreenOrientation m_defaultOrientation;
            
            ResourceManagerDispenser* m_resourceManagerDispenser;
            ComponentFactoryDispenser* m_componentFactoryDispenser;
			std::vector<ResourceProviderOld*> m_ResourceProviderOlds;

			TimeIntervalSecs m_currentAppTime;
            Rendering::FontSPtr m_defaultFont;
            Rendering::MeshCSPtr m_defaultMesh;
            Rendering::MaterialSPtr m_defaultMaterial;
			f32 m_updateInterval;
            f32 m_updateSpeed;
            
            f32 m_updateIntervalRemainder;
            bool m_shouldNotifyConnectionsResumeEvent;
            bool m_shouldNotifyConnectionsForegroundEvent;
            bool m_isFirstFrame;
            bool m_isSuspending;
            bool m_isSystemCreationAllowed;
            
            static Application* s_application;
		};
        //----------------------------------------------------
        //----------------------------------------------------
        template <typename TSystem, typename... TArgs> TSystem* Application::CreateSystem(TArgs... in_args)
        {
            CS_ASSERT(m_isSystemCreationAllowed == true, "Cannot add systems outwith the creation phase");
            
            std::unique_ptr<TSystem> newSystem = TSystem::Create(in_args...);
			TSystem* output = newSystem.get();
			if (newSystem != nullptr)
			{
				m_systems.push_back(std::move(newSystem));
			}
            
            return output;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        template <typename TNamedType> TNamedType* Application::GetSystem_Old()
        {
            for (std::vector<SystemUPtr>::const_iterator it = m_systemsOld.begin(); it != m_systemsOld.end(); ++it)
			{
				if ((*it)->IsA(TNamedType::InterfaceID))
				{
					return static_cast<TNamedType*>((*it).get());
				}
			}
			
			CS_LOG_WARNING("Application cannot find system: " + TNamedType::TypeName);
			return nullptr;
        }
        //-----------------------------------------------------
        //-----------------------------------------------------
        template <typename TNamedType> void Application::GetSystems_Old(std::vector<TNamedType*>& out_systems)
        {
            for (size_t systemIndex = 0; systemIndex < m_systemsOld.size(); systemIndex++)
            {
                if (m_systemsOld[systemIndex]->IsA(TNamedType::InterfaceID))
                {
                    out_systems.push_back(static_cast<TNamedType*>(m_systemsOld[systemIndex].get()));
                }
            }
        }
        //----------------------------------------------------
        //----------------------------------------------------
        template <typename TNamedType> TNamedType* Application::GetSystem()
        {
            for (std::vector<AppSystemUPtr>::const_iterator it = m_systems.begin(); it != m_systems.end(); ++it)
			{
				if ((*it)->IsA(TNamedType::InterfaceID))
				{
					return static_cast<TNamedType*>((*it).get());
				}
			}
			
			CS_LOG_WARNING("Application cannot find system: " + TNamedType::TypeName);
			return nullptr;
        }
        //-----------------------------------------------------
        //-----------------------------------------------------
        template <typename TNamedType> void Application::GetSystems(std::vector<TNamedType*>& out_systems)
        {
            for (size_t systemIndex = 0; systemIndex < m_systems.size(); systemIndex++)
            {
                if (m_systems[systemIndex]->IsA(TNamedType::InterfaceID))
                {
                    out_systems.push_back(static_cast<TNamedType*>(m_systemsOld[systemIndex].get()));
                }
            }
        }
	}
}

#endif
