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
        const u32 kudwUndefinedMaxResolution = std::numeric_limits<u32>::max();
        const f32 kfUndefinedMaxDensity = std::numeric_limits<f32>::max();
        struct ResourceDirectoryInfo
        {
            std::string strDirectory;
            u32 udwMaxRes;
            f32 fMaxDensity;
            f32 fResourcesDensity;
        };
        
        namespace SystemConfirmDialog
        {
            enum class Result
            {
                k_confirm,
                k_cancel
            };
            
            typedef std::function<void(u32, Result)> Delegate;
        }
        
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
			/// Launch the application's setup code and cause
            /// it to begin it's update loop.
            ///
            /// @author I Copland
			//----------------------------------------------------
			void Run();
			//----------------------------------------------------
			/// Looks for a system that implements the queryable
            /// interface provided as a template parameter.
            ///
            /// @author I Copland
            ///
			/// @return The first system found that implements
            /// the named interface.
			//----------------------------------------------------
			template <typename T> T* GetSystemImplementing();
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
			template <typename T, typename U> T* GetSystemImplementing();
			//-----------------------------------------------------
			/// Looks for a all systems that implement the given
            /// queryable interface provided as a template parameter.
			///
            /// @author I Copland
            ///
			/// @param [Out] The list of systems that implement the
            /// queryable interface.
			//-----------------------------------------------------
            template <typename T> void GetSystemsImplementing(std::vector<T*> & outSystems);
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
			void SetUpdateInterval(f32 infUpdateInterval);
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
			void SetUpdateSpeed(f32 infSpeed);
            //------------------------------------------------------
			/// Enabled and disabled the updating of "updateable"
            /// systems.
			///
            /// @author I Copland
            ///
			/// @param whether or not to update.
			//------------------------------------------------------
			void EnableSystemUpdating(bool inbEnable);
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
			StateManager* GetStateManagerPtr();
			//-----------------------------------------------------
			/// Returns a pointer to the renderer.
            ///
            /// @author I Copland
			///
			/// @return Handle application renderer
			//-----------------------------------------------------
			Rendering::Renderer* GetRendererPtr();
			//-----------------------------------------------------
			/// Returns a pointer to the render system.
            ///
            /// @author I Copland
			///
			/// @return Handle to platfrom specific render system
			//-----------------------------------------------------
			Rendering::RenderSystem* GetRenderSystemPtr();
            //-----------------------------------------------------
			/// Returns a pointer to the platform system.
            ///
            /// @author I Copland
			///
            /// @return Pointer to the platform system
            //-----------------------------------------------------
			PlatformSystem* GetPlatformSystemPtr();
			//-----------------------------------------------------
			/// Returns a pointer to the input system.
            ///
            /// @author I Copland
			///
			/// @return Pointer to the input system
			//-----------------------------------------------------
			Input::InputSystem* GetInputSystemPtr();
			//-----------------------------------------------------
			/// Returns a pointer to the audio system.
            ///
            /// @author I Copland
			///
			/// @return Pointer to the input system
			//-----------------------------------------------------
			Audio::AudioSystem* GetAudioSystemPtr();
			//-----------------------------------------------------
			/// Returns a pointer to the file system.
            ///
            /// @author I Copland
			///
			/// @return Pointer to the file system
			//-----------------------------------------------------
			FileSystem* GetFileSystemPtr();
            //-----------------------------------------------------
			/// Sets the renderer.
            ///
            /// @author I Copland
			///
			/// @param Handle application renderer
			//-----------------------------------------------------
			void SetRenderer(Rendering::Renderer* in_system);
			//-----------------------------------------------------
			/// Sets the render system.
            ///
            /// @author I Copland
			///
			/// @param Handle to platfrom specific render system
			//-----------------------------------------------------
			void SetRenderSystem(Rendering::RenderSystem* in_system);
            //-----------------------------------------------------
			/// Sets the platform system.
            ///
            /// @author I Copland
			///
            /// @param Pointer to the platform system
            //-----------------------------------------------------
			void SetPlatformSystem(PlatformSystem* in_system);
			//-----------------------------------------------------
			/// Sets the input system.
            ///
            /// @author I Copland
			///
			/// @param Pointer to the input system
			//-----------------------------------------------------
			void SetInputSystem(Input::InputSystem* in_system);
			//-----------------------------------------------------
			/// Sets the audio system.
            ///
            /// @author I Copland
			///
			/// @param Pointer to the input system
			//-----------------------------------------------------
			void SetAudioSystem(Audio::AudioSystem* in_system);
			//-----------------------------------------------------
			/// Sets the file system.
            ///
            /// @author I Copland
			///
			/// @return Pointer to the file system
			//-----------------------------------------------------
			void SetFileSystem(FileSystem* in_system);
            //----------------------------------------------------
			/// Triggered by an frame begin event. 
            ///
            /// @author I Copland
            ///
            /// @param The delta time.
            /// @param The frame timestamp.
			//----------------------------------------------------
			void OnFrameBegin(f32 infDt, TimeIntervalSecs inuddwTimestamp);
            //----------------------------------------------------
            /// Triggered on receiving a "application will suspend"
            /// message. This will notify active states to pause
            /// and tell the sub systems to stop.
            ///
            /// @author I Copland
			//----------------------------------------------------
			void OnSuspend();
            //----------------------------------------------------
			/// Resumes application from suspended state.
            ///
            /// @author I Copland
			//----------------------------------------------------
			void OnResume();
            //----------------------------------------------------
			/// Triggered on receiving a "orientation changed"
            /// event. Used to tell the camera and input to rotate.
            ///
            /// @author I Copland
            ///
            /// @param The new orientation.
			//----------------------------------------------------
			void OnScreenChangedOrientation(ScreenOrientation ineOrientation);
			//----------------------------------------------------
			/// Triggered on receiving a "screen resized" event.
            ///
            /// @author I Copland
            ///
            /// @param The new width.
            /// @param The new height.
			//----------------------------------------------------
			void OnScreenResized(u32 inudwWidth, u32 inudwHeight);
			//----------------------------------------------------
			/// Triggered on receiving a "application memory warning"
            /// event. This will notify active resource managers to
            /// purge their caches.
            ///
            /// @author I Copland
			//----------------------------------------------------
			void OnApplicationMemoryWarning();
            //----------------------------------------------------
			/// Triggered on receiving a "go back" event. This is
            /// typically caused by pressing a physical back button
            /// on the device, For example the Android back button.
            ///
            /// @author I Copland
			//----------------------------------------------------
			void OnGoBack();
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
            virtual void SetResourceDirectories(std::vector<ResourceDirectoryInfo>& outaResDependantDirectoryInfos, std::string& outstrResDefaultDirectory, std::string& outstrDefaultDirectory) = 0;
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
            System* GetSystemImplementing(InterfaceIDType inInterfaceID);
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
			void Update(f32 infDT);
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
            //------------------------------------------------------
			/// Sets whether or not touch input is available on
            /// this platform.
            ///
            /// @author I Copland
			///
			/// @param whether or not touch input is available.
			//------------------------------------------------------
			void SetHasTouchInput(bool inbTouchInput);
			//------------------------------------------------------
			/// Returns whether or not touch input is available on
            /// this platform.
			///
			/// @return whether or not touch input is available.
			//------------------------------------------------------
			bool HasTouchInput();

		protected:
			
            std::vector<SystemSPtr> mSystems;
            std::vector<IUpdateable*> mUpdateableSystems;
            
        private:
			StateManager mStateMgr;
            
			Rendering::Renderer* mpRenderer;
			
            Rendering::RenderSystem* mpRenderSystem;
			Input::InputSystem * mpInputSystem;
            PlatformSystem* pPlatformSystem;
            Audio::AudioSystem* pAudioSystem;
            FileSystem* mspFileSystem;

			ScreenOrientation meDefaultOrientation;
            
            ResourceManagerDispenser* mpResourceManagerDispenser;
            ComponentFactoryDispenser* mpComponentFactoryDispenser;
		
			bool mbHasTouchInput;
            bool mbUpdateSystems;
			
			std::vector<ResourceProvider*> mResourceProviders;

			SystemConfirmDialog::Delegate mActiveSysConfirmDelegate;
        
			TimeIntervalSecs uddwCurrentAppTime;
            Rendering::FontSPtr pDefaultFont;
            Rendering::MeshSPtr pDefaultMesh;
            Rendering::MaterialSPtr pDefaultMaterial;
			f32 mfUpdateInterval;
            f32 mfUpdateSpeed;
            
            f32 s_updateIntervalRemainder;
            bool s_shouldNotifyConnectionsResumeEvent;
            bool s_isFirstFrame;
            bool s_isSuspending;
            
            static Application* s_application;
		};
        //----------------------------------------------------
        //----------------------------------------------------
        template <typename T> T* Application::GetSystemImplementing()
        {
            System* pSystem = GetSystemImplementing(T::InterfaceID);
            
            if (pSystem != nullptr)
            {
                return static_cast<T*>(pSystem);
            }
            
            return nullptr;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        template <typename T, typename U> T* Application::GetSystemImplementing()
        {
            System* pSystem = GetSystemImplementing(U::InterfaceID);
            
            if (pSystem != nullptr)
            {
                return pSystem->GetInterface<T>();
            }
            
            return nullptr;
        }
        //-----------------------------------------------------
        //-----------------------------------------------------
        template <typename T> void Application::GetSystemsImplementing(std::vector<T*> & outSystems)
        {
            for (size_t nSystem = 0; nSystem < mSystems.size(); nSystem++)
            {
                if (mSystems[nSystem]->IsA(T::InterfaceID)) 
                {
                    outSystems.push_back(static_cast<T*>(mSystems[nSystem].get()));
                }
            }
        }
	}
}

#endif
