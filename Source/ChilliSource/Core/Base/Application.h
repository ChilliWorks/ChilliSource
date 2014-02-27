/*
 *  moFloApplication.h
 *  moFlo
 *
 *  Created by Scott Downie on 23/09/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#ifndef _MOFLO_APPLICATION_H_
#define _MOFLO_APPLICATION_H_

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
        
		//--------------------------------------------------------------------------------------------------
		/// Description:
		///
		/// Main hub of the framework. Handles initialisation
		/// and subsystems (including queries). Developers
		/// must override the initialisation methods to create
		/// the subsystems they require.
		//--------------------------------------------------------------------------------------------------
		class CApplication
		{
		public:
			CApplication();
			virtual ~CApplication();
			
			//--------------------------------------------------------------------------------------------------
			/// Run
			///
			/// Launch the application's setup code and cause it to begin it's update loop
			//--------------------------------------------------------------------------------------------------
			void Run();
			//--------------------------------------------------------------------------------------------------
			/// Suspend
			///
            /// Triggered on receiving a "application will suspend" message.
            /// This will notify active states to pause and tell the sub systems to stop
			//--------------------------------------------------------------------------------------------------
			static void Suspend();
			//--------------------------------------------------------------------------------------------------
			/// Resume
			///
			/// Resumes application from suspended state
			//--------------------------------------------------------------------------------------------------
			static void Resume();
			//--------------------------------------------------------------------------------------------------
			/// On Screen Changed Orientation
			///
			/// Triggered on receiving a "orientation changed" message. Used to tell the camera and input
			/// to rotate
			//--------------------------------------------------------------------------------------------------
			static void OnScreenChangedOrientation(ScreenOrientation ineOrientation);
			//--------------------------------------------------------------------------------------------------
			/// On Screen Resized
			///
			/// Triggered on receiving a "screen resized" message
			//--------------------------------------------------------------------------------------------------
			static void OnScreenResized(u32 inudwWidth, u32 inudwHeight);
			//--------------------------------------------------------------------------------------------------
			/// On Application Memory Warning
			///
			/// Triggered on receiving a "application memory warning" message.
			/// This will notify active resource managers to purge their caches
			//--------------------------------------------------------------------------------------------------
			static void OnApplicationMemoryWarning();
			//--------------------------------------------------------------------------------------------------
			/// On Go Back
			///
			/// Triggered on receiving a "go back" event. This is usually caused by a back button being pressed
			//--------------------------------------------------------------------------------------------------
			static void OnGoBack();
            //--------------------------------------------------------------------------------------------------
			/// On Frame Begin
			///
			/// Triggered on receiving a "frame started" message. This will then update the state manager
			/// and each updatable subsystem with the time since last frame
			//--------------------------------------------------------------------------------------------------
			static void OnFrameBegin(f32 infDt, TimeIntervalSecs inuddwTimestamp);
			//--------------------------------------------------------------------------------------------------
			/// Get System Implementing
			///
			/// Looks for a system that implements the given interface (i.e. a 2DRenderSystem etc)
			/// @param The type ID of the system you wish to implement
			/// @return System that implements the given interface or nullptr if no system
			//--------------------------------------------------------------------------------------------------
			static SystemPtr GetSystemImplementing(InterfaceIDType inInterfaceID);
			//--------------------------------------------------------------------------------------------------
			/// Get System Implementing
			///
			/// Looks for a system that implements the given interface (i.e. a 2DRenderSystem etc)
			/// @param The type ID of the system you wish to implement
			/// @return System that implements the given interface or nullptr if no system
			//--------------------------------------------------------------------------------------------------
			//Templated convenience version of the above saving getting the interface directly
			template <typename T> static T* GetSystemImplementing()
			{
				SystemPtr pSystem = GetSystemImplementing(T::InterfaceID);
				
				if (pSystem != nullptr) 
				{
					return static_cast<T*>(pSystem.get());
				}
				
				return nullptr;
			}
            //--------------------------------------------------------------------------------------------------
			/// Get System Implementing
			///
			/// Looks for a system that implements the given interface (i.e. a 2DRenderSystem etc)
			/// @param The type ID of the system you wish to implement
			/// @return System that implements the given interface or nullptr if no system
			//--------------------------------------------------------------------------------------------------
			//Templated convenience version of the above saving getting the interface directly
			template <typename T, typename U> static T* GetSystemImplementing()
			{
				SystemPtr pSystem = GetSystemImplementing(U::InterfaceID);
				
				if (pSystem != nullptr) 
				{
					return pSystem->GetInterface<T>();
				}
				
				return nullptr;
			}
			//--------------------------------------------------------------------------------------------------
			/// Get Systems Implementing
			///
			/// Looks for systems that implements the given interface (i.e. a 2DRenderSystem etc)
			/// and fills an array with them.
			/// @param The type ID of the system you wish to implement
			//--------------------------------------------------------------------------------------------------
			static void GetSystemsImplementing(InterfaceIDType inInterfaceID, std::vector<SystemPtr> & outSystems);
			//--------------------------------------------------------------------------------------------------
			/// Get Systems Implementing
			///
			/// Looks for systems that implements the given interface (i.e. a 2DRenderSystem etc)
			/// and fills an array with them.
			/// @param The type ID of the system you wish to implement
			//--------------------------------------------------------------------------------------------------
			//Templated convenience version of the above saving getting the interface directly
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
			//--------------------------------------------------------------------------------------------------
			/// Get Resource Provider Producing
			///
			/// Looks for a resource provider that can create a resource of type
			/// @param The type ID of the resource you wish to create (i.e. Model, Texture)
			/// @return Resource provider that loads the resource type
			//--------------------------------------------------------------------------------------------------
			IResourceProvider* GetResourceProviderProducing(InterfaceIDType inInterfaceID, const std::string & inExtension);
            //--------------------------------------------------------------------------------------------------
            /// Get MoFlow Version
            ///
            /// @return String containing the engine version number
            //--------------------------------------------------------------------------------------------------
            static std::string GetMoFlowVersion();
			//--------------------------------------------------------------------------------------------------
			/// Get App Version
			///
			/// @return String containing the application version number
			//--------------------------------------------------------------------------------------------------
			static std::string GetAppVersion();
            //--------------------------------------------------------------------------------------------------
            /// Make Toast
            ///
            /// Display a toast notification with the given text
            ///
            /// @param Text
            //--------------------------------------------------------------------------------------------------
            static void MakeToast(const UTF8String& instrText);
			//--------------------------------------------------------------------------------------------------
			/// Get App Elapsed Time
			///
			/// @return The time elapsed since the application began
			//--------------------------------------------------------------------------------------------------
			static TimeIntervalSecs GetAppElapsedTime();
			//--------------------------------------------------------------------------------------------------
			/// Set App Elapsed Time
			///
			/// @param The time elapsed since the application began
			//--------------------------------------------------------------------------------------------------
			static void SetAppElapsedTime(TimeIntervalSecs inElapsedTime);
            //--------------------------------------------------------------------------------------------------
            /// Get System Time
            ///
            /// @return System clock time in seconds since epoch
            //--------------------------------------------------------------------------------------------------
            static TimeIntervalSecs GetSystemTime();
			//--------------------------------------------------------------------------------------------------
            /// Get System Time In Milliseconds
            ///
            /// @return System clock time in milliseconds since epoch
            //--------------------------------------------------------------------------------------------------
            static TimeIntervalSecs GetSystemTimeInMilliseconds();
			//--------------------------------------------------------------------------------------------------
			/// Set Update Interval
			///
			/// Reset the time between update calls
			/// to adjust the frame rate.
			///
			/// @param Time between update calls
			//--------------------------------------------------------------------------------------------------
			static void SetUpdateInterval(f32 infUpdateInterval);
			//--------------------------------------------------------------------------------------------------
			/// Get Update Interval
			///
			/// @return Time between update calls
			//--------------------------------------------------------------------------------------------------
			static f32 GetUpdateInterval();
			//--------------------------------------------------------------------------------------------------
			/// Get Max Update Interval
			///
			/// @return Max time to be processed in a single frame.
			//--------------------------------------------------------------------------------------------------
			static f32 GetUpdateIntervalMax();
            //--------------------------------------------------------------------------------------------------
			/// Set Update Speed
            ///
			/// @param Scaler to speed up or slow down update time
			//--------------------------------------------------------------------------------------------------
			static void SetUpdateSpeed(f32 infSpeed);
			//--------------------------------------------------------------------------------------------------
			/// Get State Manager
			///
			/// @return Application state manager
			//--------------------------------------------------------------------------------------------------
			static CStateManager& GetStateManager();
			//--------------------------------------------------------------------------------------------------
			/// Get State Manager Pointer
			///
			/// @return Handle to application state manager
			//--------------------------------------------------------------------------------------------------
			static CStateManager* GetStateManagerPtr();
			//--------------------------------------------------------------------------------------------------
			/// Get Renderer Pointer
			///
			/// @return Handle application renderer
			//--------------------------------------------------------------------------------------------------
			static Rendering::CRenderer* GetRendererPtr() {return mpRenderer;}
			//--------------------------------------------------------------------------------------------------
			/// Get Render System Pointer
			///
			/// @return Handle to platfrom specific render system
			//--------------------------------------------------------------------------------------------------
			static Rendering::IRenderSystem* GetRenderSystemPtr() {return mpRenderSystem;}
            //--------------------------------------------------------------------------------------------------
			/// Get Platform System Ptr
			///
            /// @return Pointer to the platform system
            //--------------------------------------------------------------------------------------------------
			static IPlatformSystem* GetPlatformSystemPtr(){return pPlatformSystem;}
			//--------------------------------------------------------------------------------------------------
			/// Get Input System Ptr
			///
			/// @return Pointer to the input system
			//--------------------------------------------------------------------------------------------------
			static Input::InputSystem* GetInputSystemPtr(){return mpInputSystem;}
			//--------------------------------------------------------------------------------------------------
			/// Get Input System Ptr
			///
			/// @return Pointer to the input system
			//--------------------------------------------------------------------------------------------------
			static Audio::AudioSystem* GetAudioSystemPtr(){return pAudioSystem;}
			//--------------------------------------------------------------------------------------------------
			/// Get File System Ptr
			///
			/// @return Pointer to the file system
			//--------------------------------------------------------------------------------------------------
			static IFileSystem* GetFileSystemPtr(){return mspFileSystem;}
            //--------------------------------------------------------------------------------------------------
			/// Refresh Master Text
			///
            /// Call this after a DLC update to refresh the master text cache from file
            ///
            /// @param Storage location
            /// @param File path excluding name (i.e. if root then "")
            //--------------------------------------------------------------------------------------------------
            static void RefreshMasterText(StorageLocation ineStorageLocation, const std::string& instrDirectory);
            //--------------------------------------------------------------------------------------------------
			/// Get Default Font
			///
            /// @return Default font given to the system by the application delegate
            //--------------------------------------------------------------------------------------------------
            static const Rendering::FontPtr& GetDefaultFont();
            //--------------------------------------------------------------------------------------------------
			/// Get Default Mesh
			///
            /// @return Default mesh given to the system by the application delegate
            //--------------------------------------------------------------------------------------------------
            static const Rendering::MeshPtr& GetDefaultMesh();
            //--------------------------------------------------------------------------------------------------
			/// Get Default Material
			///
            /// @return Default material given to the system by the application delegate
            //--------------------------------------------------------------------------------------------------
            static const Rendering::MaterialPtr& GetDefaultMaterial();
            //--------------------------------------------------------------------------------------------------
            /// Quit
            ///
            /// Stop the application and exit 
            /// gracefully
            //--------------------------------------------------------------------------------------------------
            static void Quit();
            //--------------------------------------------------------------------------------------------------
            /// Show System Confirm Dialog
            ///
            /// Display a system confirmation dialog with the given ID and delegate
            ///
            /// @param ID
            /// @param SystemConfirmDialogDelegate
            /// @param Title text
            /// @param Message text
            /// @param Confirm text
            /// @param Cancel text
            //--------------------------------------------------------------------------------------------------
            static void ShowSystemConfirmDialog(u32 inudwID, const SystemConfirmDialog::Delegate& inDelegate, const UTF8String& instrTitle, const UTF8String& instrMessage, const UTF8String& instrConfirm, const UTF8String& instrCancel);
            //--------------------------------------------------------------------------------------------------
            /// Show System Dialog
            ///
            /// Display a system dialog with the given ID and delegate
            ///
            /// @param ID
            /// @param SystemConfirmDialogDelegate
            /// @param Title text
            /// @param Message text
            /// @param Confirm text
            //--------------------------------------------------------------------------------------------------
            static void ShowSystemDialog(u32 inudwID, const SystemConfirmDialog::Delegate& inDelegate, const UTF8String& instrTitle, const UTF8String& instrMessage, const UTF8String& instrConfirm);
            //--------------------------------------------------------------------------------------------------
            /// On System Confirm Dialog Result
            ///
            /// Triggered from a system dialog confirmation event
            ///
            /// @param ID
            /// @param Result
            //--------------------------------------------------------------------------------------------------
            static void OnSystemConfirmDialogResult(u32 inudwID, SystemConfirmDialog::Result ineResult);
            //--------------------------------------------------------------------------------------------------
			/// Set Render System
			///
			/// @param the system pointer.
			//--------------------------------------------------------------------------------------------------
			static void SetRenderSystem(Rendering::IRenderSystem* inpSystem);
            //--------------------------------------------------------------------------------------------------
			/// Set Input System
			///
			/// @param the system pointer.
			//--------------------------------------------------------------------------------------------------
			static void SetInputSystem(Input::InputSystem* inpSystem);
            //--------------------------------------------------------------------------------------------------
			/// Set Audio System
			///
			/// @param the system pointer.
			//--------------------------------------------------------------------------------------------------
			static void SetAudioSystem(Audio::AudioSystem* inpSystem);
            //--------------------------------------------------------------------------------------------------
			/// Set Renderer
			///
			/// @param the renderer
			//--------------------------------------------------------------------------------------------------
			static void SetRenderer(Rendering::CRenderer* inpSystem);
			//--------------------------------------------------------------------------------------------------
			/// Set File System
			///
			/// @param the file system
			//--------------------------------------------------------------------------------------------------
			static void SetFileSystem(IFileSystem* inpSystem);
			//--------------------------------------------------------------------------------------------------
			/// Set Has Touch Input
			///
			/// @param whether or not touch input is available.
			//--------------------------------------------------------------------------------------------------
			static void SetHasTouchInput(bool inbTouchInput);
			//--------------------------------------------------------------------------------------------------
			/// Has Touch Input
			///
			/// @return whether or not touch input is available.
			//--------------------------------------------------------------------------------------------------
			static bool HasTouchInput();
            //----------------------------------------------------------------------
			/// Set Orientation
			///
			/// Tell the active camera to roate its view and if we are using
			/// touch input we must rotate the input co-ordinates
			/// @param Screen orientation flag
			//----------------------------------------------------------------------
			static void SetOrientation(ScreenOrientation inOrientation);
			//--------------------------------------------------------------------------------------------------
			/// Enable System Updating
			///
			/// Enabled and disabled the updating of "updateable" systems.
			///
			/// @param whether or not to update.
			//--------------------------------------------------------------------------------------------------
			static void EnableSystemUpdating(bool inbEnable);
            //--------------------------------------------------------------------------------------------------
            /// Update
            ///
            /// A single update cycle that updates all updateables, timers and the active state
            /// This can be called multiple times per frame depending on fixed updates.
            ///
            /// @param Time between frames
            //--------------------------------------------------------------------------------------------------
			static void Update(f32 infDT);
			//--------------------------------------------------------------------------------------------------
			/// Push Initial State
			///
			/// Give the state manager the initial state. Overriden by application to add initial state
			//--------------------------------------------------------------------------------------------------
			virtual void PushInitialState() = 0;
		protected:
            
            //---App Delegates
            //--------------------------------------------------------------------------------------------------
            /// Set Resource Directories
            ///
            /// Give MoFlow the available resource directories and the info required to decide which directory
            /// the current device should use
            ///
            /// @param Out: Vector of directory info
            /// @param Out: The name of the directory to fall back on for resolution dependant assets
            /// @param Out: The name of the directory to use as the default (i.e. for shared assets)
            //--------------------------------------------------------------------------------------------------
            virtual void SetResourceDirectories(std::vector<ResourceDirectoryInfo>& outaResDependantDirectoryInfos, std::string& outstrResDefaultDirectory, std::string& outstrDefaultDirectory) = 0;
			//--------------------------------------------------------------------------------------------------
			/// Create Systems
			///
			/// Applications override this to create systems specific to the app or game
			//--------------------------------------------------------------------------------------------------
			virtual void CreateSystems() = 0;	
			//--------------------------------------------------------------------------------------------------
			/// Post Create Systems
			///
			/// Once the systems have been created they are then added to the pool and initialised
			//--------------------------------------------------------------------------------------------------
			void PostCreateSystems();
            //--------------------------------------------------------------------------------------------------
            /// Load Default Resources
            ///
            /// Open the app config file and load any of the specified resources to act as the defaults
            //--------------------------------------------------------------------------------------------------
            void LoadDefaultResources();
            //--------------------------------------------------------------------------------------------------
            /// Determine Resource Directories
            ///
            /// Depedending on the device decide which folders resources should be loaded from
            //--------------------------------------------------------------------------------------------------
            void DetermineResourceDirectories();
        
        private:
            //---------------------------------------------------
            /// Resumes the application. This will be called when
            /// at the start of the next update following Resume
            /// being called.
            ///
            /// @author Ian Copland
            //---------------------------------------------------
            static void OnApplicationResumed();
            
		protected: //---Members
			
			static CStateManager mStateMgr;		//Handles the state updating and transitioning
            
			static Rendering::CRenderer* mpRenderer;
			
            //---Systems
            static Rendering::IRenderSystem* mpRenderSystem;
			static Input::InputSystem * mpInputSystem;
            static IPlatformSystem* pPlatformSystem;	//Interface to platform specific API's such as timer etc
            static Audio::AudioSystem* pAudioSystem;
            static IFileSystem* mspFileSystem;

			static ScreenOrientation meDefaultOrientation;
			
			static std::vector<SystemPtr> mSystems; //All systems in use by the application
            static std::vector<IUpdateable*> mUpdateableSystems; //All updateable systems in use by the application
            
            static CResourceManagerDispenser* mpResourceManagerDispenser;
            CComponentFactoryDispenser* mpComponentFactoryDispenser;
		
			static bool mbHasTouchInput;
            static bool mbUpdateSystems;
		private:
			
			std::vector<IResourceProvider*> mResourceProviders; //All resource provider systems available

			static SystemConfirmDialog::Delegate mActiveSysConfirmDelegate;
        
			static TimeIntervalSecs uddwCurrentAppTime;
            static Rendering::FontPtr pDefaultFont;
            static Rendering::MeshPtr pDefaultMesh;
            static Rendering::MaterialPtr pDefaultMaterial;
			static f32 mfUpdateInterval;
            static f32 mfUpdateSpeed;
            
            static f32 s_updateIntervalRemainder;
            static bool s_shouldInvokeResumeEvent;
            static bool s_isFirstFrame;
            static bool s_isSuspending;
		};
	}
}

#endif
