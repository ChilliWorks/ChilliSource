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
            /// Triggered on receiving a "application will suspend"
            /// message. This will notify active states to pause
            /// and tell the sub systems to stop.
            ///
            /// @author I Copland
			//----------------------------------------------------
			static void Suspend();
			//----------------------------------------------------
			/// Resumes application from suspended state.
            ///
            /// @author I Copland
			//----------------------------------------------------
			static void Resume();
			//----------------------------------------------------
			/// Triggered on receiving a "orientation changed"
            /// event. Used to tell the camera and input to rotate.
            ///
            /// @author I Copland
            ///
            /// @param The new orientation.
			//----------------------------------------------------
			static void OnScreenChangedOrientation(ScreenOrientation ineOrientation);
			//----------------------------------------------------
			/// Triggered on receiving a "screen resized" event.
            ///
            /// @author I Copland
            ///
            /// @param The new width.
            /// @param The new height.
			//----------------------------------------------------
			static void OnScreenResized(u32 inudwWidth, u32 inudwHeight);
			//----------------------------------------------------
			/// Triggered on receiving a "application memory warning"
            /// event. This will notify active resource managers to
            /// purge their caches.
            ///
            /// @author I Copland
			//----------------------------------------------------
			static void OnApplicationMemoryWarning();
			//----------------------------------------------------
			/// Triggered on receiving a "go back" event. This is
            /// typically caused by pressing a physical back button
            /// on the device, For example the Android back button.
            ///
            /// @author I Copland
			//----------------------------------------------------
			static void OnGoBack();
            //----------------------------------------------------
			/// Triggered on receiving a "frame started" event.
            /// This will then update the systems and the active
            /// state.
            ///
            /// @author I Copland
            ///
            /// @param The delta time.
            /// @param The frame timestamp.
			//----------------------------------------------------
			static void OnFrameBegin(f32 infDt, TimeIntervalSecs inuddwTimestamp);
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
			static SystemSPtr GetSystemImplementing(InterfaceIDType inInterfaceID);
			//----------------------------------------------------
			/// Looks for a system that implements the queryable
            /// interface provided as a template parameter.
            ///
            /// @author I Copland
            ///
			/// @return The first system found that implements
            /// the named interface.
			//----------------------------------------------------
			template <typename T> static T* GetSystemImplementing()
			{
				SystemSPtr pSystem = GetSystemImplementing(T::InterfaceID);
				
				if (pSystem != nullptr) 
				{
					return static_cast<T*>(pSystem.get());
				}
				
				return nullptr;
			}
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
			template <typename T, typename U> static T* GetSystemImplementing()
			{
				SystemSPtr pSystem = GetSystemImplementing(U::InterfaceID);
				
				if (pSystem != nullptr) 
				{
					return pSystem->GetInterface<T>();
				}
				
				return nullptr;
			}
			//-----------------------------------------------------
            /// Looks for a all systems that implement the given
            /// queryable interface.
            ///
            /// @author I Copland
            ///
            /// @param The queryable interface.
			/// @param [Out] The list of systems that implement the
            /// queryable interface.
			//-----------------------------------------------------
			static void GetSystemsImplementing(InterfaceIDType inInterfaceID, std::vector<SystemSPtr> & outSystems);
			//-----------------------------------------------------
			/// Looks for a all systems that implement the given
            /// queryable interface provided as a template parameter.
			///
            /// @author I Copland
            ///
			/// @param [Out] The list of systems that implement the
            /// queryable interface.
			//-----------------------------------------------------
            template <typename T> static void GetSystemsImplementing(std::vector<T*> & outSystems)
			{
				for (size_t nSystem = 0; nSystem < mSystems.size(); nSystem++) 
				{
					if (mSystems[nSystem]->IsA(T::InterfaceID)) 
					{
						outSystems.push_back(static_cast<T*>(mSystems[nSystem].get()));
					}
				}
			}
			//-----------------------------------------------------
			/// Looks for a resource provider that can create a
            /// resource of the given type.
            ///
            /// @author I Copland
            ///
			/// @param The type ID of the resource you wish to create,
            /// i.e. Model, Texture.
			/// @return Resource provider that loads the resource type.
			//-----------------------------------------------------
			ResourceProvider* GetResourceProviderProducing(InterfaceIDType inInterfaceID, const std::string & inExtension);
			//-----------------------------------------------------
            /// Returns the version number of the application on
            /// the current platform as a string.
            ///
            /// @author I Copland.
            ///
			/// @return The version string.
			//-----------------------------------------------------
			static std::string GetAppVersion();
            //-----------------------------------------------------
            /// Display a toast notification with the given text
            ///
            /// @author I Copland
            ///
            /// @param The text to display.
            //-----------------------------------------------------
            static void MakeToast(const UTF8String& instrText);
			//-----------------------------------------------------
			/// Returns the elapsed time since the application
            /// started running in seconds.
			///
            /// @author I Copland
            ///
			/// @return The time in seconds.
			//-----------------------------------------------------
			static TimeIntervalSecs GetAppElapsedTime();
			//-----------------------------------------------------
			/// Set the elapsed time since the application started
            /// running in seconds.
            ///
            /// @author I Copland
            ///
			/// @param The time elapsed since the application began
			//-----------------------------------------------------
			static void SetAppElapsedTime(TimeIntervalSecs inElapsedTime);
            //-----------------------------------------------------
            /// Returns the system clock time in seconds since epoch.
            ///
            /// @author I Copland
            ///
            /// @return The time in seconds.
            //-----------------------------------------------------
            static TimeIntervalSecs GetSystemTime();
			//-----------------------------------------------------
            /// Return the system clock time in milliseconds since
            /// epoch.
            ///
            /// @author I Copland
            ///
            /// @return The time in milliseconds.
            //-----------------------------------------------------
            static TimeIntervalSecs GetSystemTimeInMilliseconds();
			//-----------------------------------------------------
			/// Set the time between update calls to adjust the
            /// frame rate.
            ///
            /// @author I Copland
			///
			/// @param Time between update calls
			//-----------------------------------------------------
			static void SetUpdateInterval(f32 infUpdateInterval);
			//-----------------------------------------------------
			/// Returns the time between update calls.
            ///
            /// @author I Copland.
			///
			/// @return Time between update calls
			//-----------------------------------------------------
			static f32 GetUpdateInterval();
			//-----------------------------------------------------
			/// Returns the maximum amount of time to be processed
            /// a single update frame.
            ///
            /// @author I Copland
			///
			/// @return Max time to be processed in a single frame.
			//-----------------------------------------------------
			static f32 GetUpdateIntervalMax();
            //-----------------------------------------------------
			/// Sets a multiplier for slowing or speeding up the
            /// delta time passed to each system and state.
            ///
            /// @author I Copland
            ///
			/// @param Scaler to speed up or slow down update time.
			//-----------------------------------------------------
			static void SetUpdateSpeed(f32 infSpeed);
			//-----------------------------------------------------
			/// Returns the state manager.
            ///
            /// @author I Copland
			///
			/// @return Application state manager
			//-----------------------------------------------------
			static StateManager& GetStateManager();
			//-----------------------------------------------------
			/// Returns a pointer to the state manager.
            ///
            /// @author I Copland
			///
			/// @return Handle to application state manager
			//-----------------------------------------------------
			static StateManager* GetStateManagerPtr();
			//-----------------------------------------------------
			/// Returns a pointer to the renderer.
            ///
            /// @author I Copland
			///
			/// @return Handle application renderer
			//-----------------------------------------------------
			static Rendering::Renderer* GetRendererPtr() {return mpRenderer;}
			//-----------------------------------------------------
			/// Returns a pointer to the render system.
            ///
            /// @author I Copland
			///
			/// @return Handle to platfrom specific render system
			//-----------------------------------------------------
			static Rendering::RenderSystem* GetRenderSystemPtr() {return mpRenderSystem;}
            //-----------------------------------------------------
			/// Returns a pointer to the platform system.
            ///
            /// @author I Copland
			///
            /// @return Pointer to the platform system
            //-----------------------------------------------------
			static PlatformSystem* GetPlatformSystemPtr(){return pPlatformSystem;}
			//-----------------------------------------------------
			/// Returns a pointer to the input system.
            ///
            /// @author I Copland
			///
			/// @return Pointer to the input system
			//-----------------------------------------------------
			static Input::InputSystem* GetInputSystemPtr(){return mpInputSystem;}
			//-----------------------------------------------------
			/// Returns a pointer to the audio system.
            ///
            /// @author I Copland
			///
			/// @return Pointer to the input system
			//-----------------------------------------------------
			static Audio::AudioSystem* GetAudioSystemPtr(){return pAudioSystem;}
			//-----------------------------------------------------
			/// Returns a pointer to the file system.
            ///
            /// @author I Copland
			///
			/// @return Pointer to the file system
			//-----------------------------------------------------
			static FileSystem* GetFileSystemPtr(){return mspFileSystem;}
            //-----------------------------------------------------
			/// Refreshes the master text. This should be called
            /// whenever the master text changes on disc, for example
            /// after a DLC update.
            ///
            /// @author I Copland
            ///
            /// @param The storage location of the master text.
            /// @param Directory path excluding name, i.e. if root then
            /// just "" should be passed.
            //-----------------------------------------------------
            static void RefreshMasterText(StorageLocation ineStorageLocation, const std::string& instrDirectory);
            //-----------------------------------------------------
			/// Returns the default font described in the App.config
            /// file.
			///
            /// @author I Copland
            ///
            /// @return The default font.
            //-----------------------------------------------------
            static const Rendering::FontSPtr& GetDefaultFont();
            //-----------------------------------------------------
			/// Returns the default mesh described in the App.config
            /// file.
            ///
            /// @author I Copland
			///
            /// @return The default mesh.
            //-----------------------------------------------------
            static const Rendering::MeshSPtr& GetDefaultMesh();
            //-----------------------------------------------------
			/// Returns the default material described in the
            /// App.config file.
            ///
            /// @author I Copland
			///
            /// @return The default material.
            //-----------------------------------------------------
            static const Rendering::MaterialSPtr& GetDefaultMaterial();
            //-----------------------------------------------------
            /// Stop the application and exit gracefully
            ///
            /// @author I Copland
            //-----------------------------------------------------
            static void Quit();
            //-----------------------------------------------------
            /// Display a system confirmation dialog with the given
            /// ID and delegate.
            ///
            /// @author I Copland
            ///
            /// @param ID
            /// @param SystemConfirmDialogDelegate
            /// @param Title text
            /// @param Message text
            /// @param Confirm text
            /// @param Cancel text
            //-----------------------------------------------------
            static void ShowSystemConfirmDialog(u32 inudwID, const SystemConfirmDialog::Delegate& inDelegate, const UTF8String& instrTitle, const UTF8String& instrMessage, const UTF8String& instrConfirm, const UTF8String& instrCancel);
            //-----------------------------------------------------
            /// Display a system dialog with the given ID and delegate
            ///
            /// @author I Copland
            ///
            /// @param ID
            /// @param SystemConfirmDialogDelegate
            /// @param Title text
            /// @param Message text
            /// @param Confirm text
            //------------------------------------------------------
            static void ShowSystemDialog(u32 inudwID, const SystemConfirmDialog::Delegate& inDelegate, const UTF8String& instrTitle, const UTF8String& instrMessage, const UTF8String& instrConfirm);
            //------------------------------------------------------
            /// Triggered from a system dialog confirmation event
            ///
            /// @author I Copland
            ///
            /// @param ID
            /// @param Result
            //------------------------------------------------------
            static void OnSystemConfirmDialogResult(u32 inudwID, SystemConfirmDialog::Result ineResult);
            //------------------------------------------------------
			/// Sets the render system.
            ///
            /// @author I Copland
			///
			/// @param the system pointer.
			//------------------------------------------------------
			static void SetRenderSystem(Rendering::RenderSystem* inpSystem);
            //------------------------------------------------------
			/// Sets the input system.
            ///
            /// @author I Copland
			///
			/// @param the system pointer.
			//------------------------------------------------------
			static void SetInputSystem(Input::InputSystem* inpSystem);
            //------------------------------------------------------
			/// Sets the audio system.
            ///
            /// @author I Copland
			///
			/// @param the system pointer.
			//------------------------------------------------------
			static void SetAudioSystem(Audio::AudioSystem* inpSystem);
            //------------------------------------------------------
			/// Sets the renderer
            ///
            /// @author I Copland
			///
			/// @param the renderer
			//------------------------------------------------------
			static void SetRenderer(Rendering::Renderer* inpSystem);
			//------------------------------------------------------
			/// Sets the file system
            ///
            /// @author I Copland
			///
			/// @param the file system
			//------------------------------------------------------
			static void SetFileSystem(FileSystem* inpSystem);
			//------------------------------------------------------
			/// Sets whether or not touch input is available on
            /// this platform.
            ///
            /// @author I Copland
			///
			/// @param whether or not touch input is available.
			//------------------------------------------------------
			static void SetHasTouchInput(bool inbTouchInput);
			//------------------------------------------------------
			/// Returns whether or not touch input is available on
            /// this platform.
			///
			/// @return whether or not touch input is available.
			//------------------------------------------------------
			static bool HasTouchInput();
            //------------------------------------------------------
			/// Tell the active camera to roate its view and if we
            /// are using touch input we must rotate the input
            /// co-ordinates.
            ///
            /// @author I Copland
            ///
			/// @param Screen orientation flag
			//------------------------------------------------------
			static void SetOrientation(ScreenOrientation inOrientation);
			//------------------------------------------------------
			/// Enabled and disabled the updating of "updateable"
            /// systems.
			///
            /// @author I Copland
            ///
			/// @param whether or not to update.
			//------------------------------------------------------
			static void EnableSystemUpdating(bool inbEnable);
            //------------------------------------------------------
            /// A single update cycle that updates all updateables,
            /// timers and the active state This can be called multiple
            /// times per frame depending on fixed updates.
            ///
            /// @author I Copland
            ///
            /// @param Time between frames
            //------------------------------------------------------
			static void Update(f32 infDT);
			//------------------------------------------------------
			/// Give the state manager the initial state. This should
            /// be overriden by the users application to add initial
            /// state.
            ///
            /// @author I Copland
			//------------------------------------------------------
			virtual void PushInitialState() = 0;
            //------------------------------------------------------
			/// Destructor
			//------------------------------------------------------
            virtual ~Application();
		protected:
            
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
			//------------------------------------------------------
			/// The users application should override this to add
            /// desired systems. Systems can only be added in this
            /// method.
            ///
            /// @author I Copland
			//------------------------------------------------------
			virtual void CreateSystems() = 0;	
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
        
        private:
            //---------------------------------------------------
            /// Resumes the application. This will be called when
            /// at the start of the next update following Resume
            /// being called.
            ///
            /// @author I Copland
            //---------------------------------------------------
            static void OnApplicationResumed();
            
		protected: //---Members
			
			static StateManager mStateMgr;		//Handles the state updating and transitioning
            
			static Rendering::Renderer* mpRenderer;
			
            //---Systems
            static Rendering::RenderSystem* mpRenderSystem;
			static Input::InputSystem * mpInputSystem;
            static PlatformSystem* pPlatformSystem;	//Interface to platform specific API's such as timer etc
            static Audio::AudioSystem* pAudioSystem;
            static FileSystem* mspFileSystem;

			static ScreenOrientation meDefaultOrientation;
			
			static std::vector<SystemSPtr> mSystems; //All systems in use by the application
            static std::vector<IUpdateable*> mUpdateableSystems; //All updateable systems in use by the application
            
            static ResourceManagerDispenser* mpResourceManagerDispenser;
            ComponentFactoryDispenser* mpComponentFactoryDispenser;
		
			static bool mbHasTouchInput;
            static bool mbUpdateSystems;
		private:
			
			std::vector<ResourceProvider*> mResourceProviders; //All resource provider systems available

			static SystemConfirmDialog::Delegate mActiveSysConfirmDelegate;
        
			static TimeIntervalSecs uddwCurrentAppTime;
            static Rendering::FontSPtr pDefaultFont;
            static Rendering::MeshSPtr pDefaultMesh;
            static Rendering::MaterialSPtr pDefaultMaterial;
			static f32 mfUpdateInterval;
            static f32 mfUpdateSpeed;
            
            static f32 s_updateIntervalRemainder;
            static bool s_shouldNotifyConnectionsResumeEvent;
            static bool s_isFirstFrame;
            static bool s_isSuspending;
		};
	}
}

#endif
