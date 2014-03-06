//
//  Application.h
//  Chilli Source
//
//  Created by Scott Downie on 23/09/2010.
//  Copyright 2010 Tag Games. All rights reserved.
//


#ifndef _CHILLISOURCE_CORE_BASE_APPLICATION_H_
#define _CHILLISOURCE_CORE_BASE_APPLICATION_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/State/StateManager.h>
#include <ChilliSource/Core/Base/Screen.h>
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
		//-----------------------------------------------------------
		class Application
		{
		public:
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
            void AddSystem(SystemUPtr in_system);
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
            //----------------------------------------------------
			/// Looks for a system that implements the queryable
            /// interface provided as the first template parameter
            /// and casts it to the type provided in the second.
            ///
            /// @author I Copland
            ///
			/// @return The first system found that implements
            /// the named interface.
			//----------------------------------------------------
			template <typename TCastType, typename TNamedType> TCastType* GetSystem();
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
            /// @author I Copland.
            ///
			/// @return The version string.
			//-----------------------------------------------------
			std::string GetAppVersion();
			//-----------------------------------------------------
			/// Returns the elapsed time since the application
            /// started running in seconds.
			///
            /// @author I Copland
            ///
			/// @return The time in seconds.
			//-----------------------------------------------------
			TimeIntervalSecs GetAppElapsedTime();
            //-----------------------------------------------------
            /// Returns the system clock time in seconds since epoch.
            ///
            /// @author I Copland
            ///
            /// @return The time in seconds.
            //-----------------------------------------------------
            TimeIntervalSecs GetSystemTime();
			//-----------------------------------------------------
            /// Return the system clock time in milliseconds since
            /// epoch.
            ///
            /// @author I Copland
            ///
            /// @return The time in milliseconds.
            //-----------------------------------------------------
            TimeIntervalSecs GetSystemTimeInMilliseconds();
			//-----------------------------------------------------
			/// Set the time between update calls to adjust the
            /// frame rate.
            ///
            /// @author I Copland
			///
			/// @param Time between update calls
			//-----------------------------------------------------
			void SetUpdateInterval(f32 in_updateInterval);
			//-----------------------------------------------------
			/// Returns the time between update calls.
            ///
            /// @author I Copland.
			///
			/// @return Time between update calls
			//-----------------------------------------------------
			f32 GetUpdateInterval();
			//-----------------------------------------------------
			/// Returns the maximum amount of time to be processed
            /// a single update frame.
            ///
            /// @author I Copland
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
            /// @author I Copland
            //-----------------------------------------------------
            void Quit();
            //-----------------------------------------------------
			/// Returns the default font described in the App.config
            /// file.
			///
            /// @author I Copland
            ///
            /// @return The default font.
            //-----------------------------------------------------
            const Rendering::FontSPtr& GetDefaultFont();
            //-----------------------------------------------------
			/// Returns the default mesh described in the App.config
            /// file.
            ///
            /// @author I Copland
			///
            /// @return The default mesh.
            //-----------------------------------------------------
            const Rendering::MeshSPtr& GetDefaultMesh();
            //-----------------------------------------------------
			/// Returns the default material described in the
            /// App.config file.
            ///
            /// @author I Copland
			///
            /// @return The default material.
            //-----------------------------------------------------
            const Rendering::MaterialSPtr& GetDefaultMaterial();
			//-----------------------------------------------------
			/// Returns a pointer to the state manager.
            ///
            /// @author I Copland
			///
			/// @return Handle to application state manager
			//-----------------------------------------------------
			StateManager* GetStateManager();
			//-----------------------------------------------------
			/// Returns a pointer to the renderer.
            ///
            /// @author I Copland
			///
			/// @return Handle application renderer
			//-----------------------------------------------------
			Rendering::Renderer* GetRenderer();
			//-----------------------------------------------------
			/// Returns a pointer to the render system.
            ///
            /// @author I Copland
			///
			/// @return Handle to platfrom specific render system
			//-----------------------------------------------------
			Rendering::RenderSystem* GetRenderSystem();
            //-----------------------------------------------------
			/// Returns a pointer to the platform system.
            ///
            /// @author I Copland
			///
            /// @return Pointer to the platform system
            //-----------------------------------------------------
			PlatformSystem* GetPlatformSystem();
			//-----------------------------------------------------
			/// Returns a pointer to the input system.
            ///
            /// @author I Copland
			///
			/// @return Pointer to the input system
			//-----------------------------------------------------
			Input::InputSystem* GetInputSystem();
			//-----------------------------------------------------
			/// Returns a pointer to the audio system.
            ///
            /// @author I Copland
			///
			/// @return Pointer to the input system
			//-----------------------------------------------------
			Audio::AudioSystem* GetAudioSystem();
			//-----------------------------------------------------
			/// Returns a pointer to the file system.
            ///
            /// @author I Copland
			///
			/// @return Pointer to the file system
			//-----------------------------------------------------
			FileSystem* GetFileSystem();
            //-----------------------------------------------------
			/// Sets the renderer.
            ///
            /// @author I Copland
			///
			/// @param Handle application renderer
			//-----------------------------------------------------
			void SetRenderer(Rendering::Renderer* in_system);
            //----------------------------------------------------
			/// Initialises the application and kicks off the update
            /// loop. This should not be called by a users application.
            ///
            /// @author I Copland
			//----------------------------------------------------
			void OnInitialise();
            //----------------------------------------------------
			/// Resumes application from suspended state. This should
            /// not be called by a users application.
            ///
            /// @author I Copland
			//----------------------------------------------------
			void OnResume();
            //----------------------------------------------------
			/// Triggered by an update event. This will update
            /// the application, systems and states. This should
            /// not be called by a users application.
            ///
            /// @author I Copland
            ///
            /// @param The delta time.
            /// @param The frame timestamp.
			//----------------------------------------------------
			void OnUpdate(f32 in_deltaTime, TimeIntervalSecs in_timestamp);
            //----------------------------------------------------
			/// Triggered on receiving a "orientation changed"
            /// event. Used to tell the camera and input to rotate.
            /// This should not be called by a users application.
            ///
            /// @author I Copland
            ///
            /// @param The new orientation.
			//----------------------------------------------------
			void OnScreenChangedOrientation(ScreenOrientation in_orientation);
			//----------------------------------------------------
			/// Triggered on receiving a "screen resized" event.
            /// This should not be called by a users application.
            ///
            /// @author I Copland
            ///
            /// @param The new width.
            /// @param The new height.
			//----------------------------------------------------
			void OnScreenResized(u32 in_width, u32 in_height);
			//----------------------------------------------------
			/// Triggered on receiving a "application memory warning"
            /// event. This will notify active resource managers to
            /// purge their caches. This should not be called by
            /// a users application.
            ///
            /// @author I Copland
			//----------------------------------------------------
			void OnApplicationMemoryWarning();
            //----------------------------------------------------
			/// Triggered on receiving a "go back" event. This is
            /// typically caused by pressing a physical back button
            /// on the device, For example the Android back button.
            /// This should not be called by a users application.
            ///
            /// @author I Copland
			//----------------------------------------------------
			void OnGoBack();
            //----------------------------------------------------
            /// Triggered on receiving a "application will suspend"
            /// message. This will notify active states to pause
            /// and tell the sub systems to stop. This should not
            /// be called by a users application.
            ///
            /// @author I Copland
			//----------------------------------------------------
			void OnSuspend();
            //----------------------------------------------------
            /// Releases all systems and states and cleans up the
            /// application. This should not be called by a users
            /// application.
            ///
            /// @author I Copland
			//----------------------------------------------------
			void OnDestroy();
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
            /// @author I Copland
			//------------------------------------------------------
			virtual void CreateSystems() = 0;
            //------------------------------------------------------
			/// Initialisation method called after all systems have
            /// been set up and before the first state is pushed.
            /// Application initialisation code should be in here.
            ///
            /// @author I Copland
			//------------------------------------------------------
			virtual void Initialise() = 0;
            //------------------------------------------------------
			/// Give the state manager the initial state. This should
            /// be overriden by the users application to add initial
            /// state.
            ///
            /// @author I Copland
			//------------------------------------------------------
			virtual void PushInitialState() = 0;
            //------------------------------------------------------
            /// Give the engine the available resource directories
            /// and the info required to decide which directory the
            /// current device should use.
            ///
            /// @author I Copland
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
			virtual void Destroy() = 0;
        private:
            //----------------------------------------------------
            /// Looks for a system that implements the given
            /// queryable interface.
            ///
            /// @author I Copland
            ///
            /// @param The interface ID for the requested named
            /// interface.
            /// @return The first system found that implements
            /// the named interface.
 			//----------------------------------------------------
            System* GetSystem(InterfaceIDType in_interfaceID);
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
            /// @author I Copland
			//------------------------------------------------------
			void PostCreateSystems();
            //------------------------------------------------------
            /// Open the app config file and load any of the specified
            /// resources to act as the defaults.
            ///
            /// @author I Copland
            //------------------------------------------------------
            void LoadDefaultResources();
            //------------------------------------------------------
            /// Depedending on the device decide which folders
            /// resources should be loaded from.
            ///
            /// @author I Copland
            //------------------------------------------------------
            void DetermineResourceDirectories();
            //------------------------------------------------------
            /// A single update cycle that updates all updateables,
            /// timers and the active state This can be called multiple
            /// times per frame depending on fixed updates.
            ///
            /// @author I Copland
            ///
            /// @param Time between frames
            //------------------------------------------------------
			void Update(f32 in_deltaTime);
            //------------------------------------------------------
			/// Tell the active camera to roate its view and if we
            /// are using touch input we must rotate the input
            /// co-ordinates.
            ///
            /// @author I Copland
            ///
			/// @param Screen orientation flag
			//------------------------------------------------------
			void SetOrientation(ScreenOrientation inOrientation);
            //---------------------------------------------------
            /// Resumes the application. This will be called when
            /// at the start of the next update following the On
            /// Resume event.
            ///
            /// @author I Copland
            //---------------------------------------------------
            void ResumeApplication();

        private:
            std::vector<SystemUPtr> m_systems;
            std::vector<IUpdateable*> m_updateableSystems;
            
			StateManager m_stateManager;
			Rendering::Renderer* m_renderer;
            Rendering::RenderSystem* m_renderSystem;
			Input::InputSystem * m_inputSystem;
            PlatformSystemUPtr m_platformSystem;
            Audio::AudioSystem* m_audioSystem;
            FileSystem* m_fileSystem;

			ScreenOrientation m_defaultOrientation;
            
            ResourceManagerDispenser* m_resourceManagerDispenser;
            ComponentFactoryDispenser* m_componentFactoryDispenser;
			std::vector<ResourceProvider*> m_resourceProviders;

			TimeIntervalSecs m_currentAppTime;
            Rendering::FontSPtr m_defaultFont;
            Rendering::MeshSPtr m_defaultMesh;
            Rendering::MaterialSPtr m_defaultMaterial;
			f32 m_updateInterval;
            f32 m_updateSpeed;
            
            f32 m_updateIntervalRemainder;
            bool m_shouldNotifyConnectionsResumeEvent;
            bool m_isFirstFrame;
            bool m_isSuspending;
            bool m_isSystemCreationAllowed;
            
            static Application* s_application;
		};
        //----------------------------------------------------
        //----------------------------------------------------
        template <typename TNamedType> TNamedType* Application::GetSystem()
        {
            System* system = GetSystem(TNamedType::InterfaceID);
            
            if (system != nullptr)
            {
                return static_cast<TNamedType*>(system);
            }
            
            return nullptr;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        template <typename TCastType, typename TNamedType> TCastType* Application::GetSystem()
        {
            System* system = GetSystem(TNamedType::InterfaceID);
            
            if (system != nullptr)
            {
                return system->GetInterface<TCastType>();
            }
            
            return nullptr;
        }
        //-----------------------------------------------------
        //-----------------------------------------------------
        template <typename TNamedType> void Application::GetSystems(std::vector<TNamedType*> & out_systems)
        {
            for (size_t systemIndex = 0; systemIndex < m_systems.size(); systemIndex++)
            {
                if (m_systems[systemIndex]->IsA(TNamedType::InterfaceID))
                {
                    out_systems.push_back(static_cast<TNamedType*>(m_systems[systemIndex].get()));
                }
            }
        }
	}
}

#endif
