/*
 *  moFloApplication.cpp
 *  moFlo
 *
 *  Created by Scott Downie on 23/09/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/ApplicationEvents.h>
#include <ChilliSource/Core/System/SystemConcepts.h>
#include <ChilliSource/Core/Resource/ResourceProvider.h>
#include <ChilliSource/Core/Entity/ComponentFactory.h>
#include <ChilliSource/Core/Time/CoreTimer.h>
#include <ChilliSource/Core/File/TweakableConstants.h>
#include <ChilliSource/Core/File/LocalDataStore.h>
#include <ChilliSource/Core/Localisation/LocalisedText.h>
#include <ChilliSource/Core/State/StateManager.h>
#include <ChilliSource/Core/Resource/ResourceGroupManager.h>
#include <ChilliSource/Core/Base/Screen.h>
#include <ChilliSource/Core/Base/Logging.h>
#include <ChilliSource/Core/Base/Device.h>
#include <ChilliSource/Core/System/System.h>
#include <ChilliSource/Core/Base/Utils.h>
#include <ChilliSource/Core/Resource/ResourceManager.h>
#include <ChilliSource/Core/Resource/ResourceManagerDispenser.h>
#include <ChilliSource/Core/Entity/ComponentFactoryDispenser.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>
#include <ChilliSource/Core/Base/PlatformSystem.h>
#include <ChilliSource/Core/Notifications/NotificationScheduler.h>
#include <ChilliSource/Core/Math/MathUtils.h>

#include <ChilliSource/Rendering/Material/Material.h>
#include <ChilliSource/Rendering/Font/Font.h>
#include <ChilliSource/Rendering/Model/Mesh.h>
#include <ChilliSource/Rendering/Base/Renderer.h>
#include <ChilliSource/Rendering/Base/RenderSystem.h>
#include <ChilliSource/Rendering/Camera/CameraComponent.h>

#include <ChilliSource/Input/Base/InputSystem.h>

#include <ctime>

namespace ChilliSource
{
	namespace Core
	{
        namespace
        {
            const f32 kfDefaultUpdateInterval = 1.0f/60.0f;
            const f32 kfUpdateClampThreshold = 0.33f;
            const f32 kfUpdateIntervalMax = kfUpdateClampThreshold;
            
            //----------------------------------------------------
            /// TODO:
            //----------------------------------------------------
            bool ResolutionSortPredicate(const ResourceDirectoryInfo& inLHS, const ResourceDirectoryInfo& inRHS)
            {
                return inLHS.udwMaxRes < inRHS.udwMaxRes;
            }
        }
        //----------------------------------------------------
        //----------------------------------------------------
        Application* Application::Get()
        {
            return nullptr;
        }
        //----------------------------------------------------
        //----------------------------------------------------
		Application::Application()
        : uddwCurrentAppTime(0), mbHasTouchInput(false), mbUpdateSystems(true), mfUpdateInterval(kfDefaultUpdateInterval), mfUpdateSpeed(1.0f), mpRenderSystem(nullptr),
            mpInputSystem(nullptr), pPlatformSystem(nullptr), pAudioSystem(nullptr), mpRenderer(nullptr), mspFileSystem(nullptr), meDefaultOrientation(ScreenOrientation::k_landscapeRight),
            mpResourceManagerDispenser(nullptr), s_updateIntervalRemainder(0.0f), s_shouldNotifyConnectionsResumeEvent(false), s_isFirstFrame(true), s_isSuspending(false)
		{
		}
        //----------------------------------------------------
        //----------------------------------------------------
		void Application::Run()
		{
            mpResourceManagerDispenser = new ResourceManagerDispenser(this);
            mpComponentFactoryDispenser = new ComponentFactoryDispenser(this);
            mStateMgr.SetOwningApplication(this);
            
#ifdef CS_TARGETPLATFORM_WINDOWS
			//Because windows by default is landscape, this needs to be flipped.
			meDefaultOrientation = Core::ScreenOrientation::k_portraitUp;
#endif
            
			//Initialise the platform specific API's
            pPlatformSystem = PlatformSystem::Create();
			pPlatformSystem->Init();
            
			//Set the screen helper classes dimensions
            Core::Screen::SetRawDimensions(pPlatformSystem->GetScreenDimensions());
            Core::Screen::SetOrientation(meDefaultOrientation);
            Core::Screen::SetDensity(pPlatformSystem->GetScreenDensity());
            
            DetermineResourceDirectories();

            //init tweakable constants and local data store.
			new TweakableConstants();
			new LocalDataStore();

            //Set up the device helper
            Device::Init(pPlatformSystem);

			//Set up the task scheduler
			TaskScheduler::Init(Core::Device::GetNumCPUCores() * 2);

			//System setup
			pPlatformSystem->CreateDefaultSystems(mSystems);
            
			CreateSystems();
			PostCreateSystems();
            LoadDefaultResources();
			OnScreenChangedOrientation(meDefaultOrientation);

            Initialise();
            
			if (mStateMgr.GetActiveScenePtr() == nullptr)
				PushInitialState();

			//Register for update events
			LocalDataStore::GetSingleton().SubscribeToApplicationSuspendEvent();

			//Begin the update loop
			pPlatformSystem->Run();
		}
        //----------------------------------------------------
        //----------------------------------------------------
		std::string Application::GetAppVersion()
		{
			return pPlatformSystem->GetAppVersion();
		}
        //----------------------------------------------------
        //----------------------------------------------------
		TimeIntervalSecs Application::GetAppElapsedTime()
		{
			return uddwCurrentAppTime;
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
			return pPlatformSystem->GetSystemTimeMS();
		}
        //----------------------------------------------------
        //----------------------------------------------------
		void Application::SetUpdateInterval(f32 infUpdateInterval)
		{
			mfUpdateInterval = infUpdateInterval;
		}
        //----------------------------------------------------
        //----------------------------------------------------
		f32 Application::GetUpdateInterval()
		{
			return mfUpdateInterval;
		}
        //----------------------------------------------------
        //----------------------------------------------------
		f32 Application::GetUpdateIntervalMax()
		{
			return kfUpdateIntervalMax;
		}
        //----------------------------------------------------
        //----------------------------------------------------
        void Application::SetUpdateSpeed(f32 infSpeed)
        {
            mfUpdateSpeed = infSpeed;
        }
        //----------------------------------------------------
        //----------------------------------------------------
		void Application::EnableSystemUpdating(bool inbEnable)
		{
			mbUpdateSystems = inbEnable;
		}
        //----------------------------------------------------
        //----------------------------------------------------
        void Application::Quit()
        {
            pPlatformSystem->TerminateUpdater();
        }
        //----------------------------------------------------
        //----------------------------------------------------
        const Rendering::FontSPtr& Application::GetDefaultFont()
        {
            return pDefaultFont;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        const Rendering::MeshSPtr& Application::GetDefaultMesh()
        {
            return pDefaultMesh;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        const Rendering::MaterialSPtr& Application::GetDefaultMaterial()
        {
            return pDefaultMaterial;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        StateManager* Application::GetStateManagerPtr()
        {
            return &mStateMgr;
        }
        //-----------------------------------------------------
        //-----------------------------------------------------
        Rendering::Renderer* Application::GetRendererPtr()
        {
            return mpRenderer;
        }
        //-----------------------------------------------------
        //-----------------------------------------------------
        Rendering::RenderSystem* Application::GetRenderSystemPtr()
        {
            return mpRenderSystem;
        }
        //-----------------------------------------------------
        //-----------------------------------------------------
        PlatformSystem* Application::GetPlatformSystemPtr()
        {
            return pPlatformSystem;
        }
        //-----------------------------------------------------
        //-----------------------------------------------------
        Input::InputSystem* Application::GetInputSystemPtr()
        {
            return mpInputSystem;
        }
        //-----------------------------------------------------
        //-----------------------------------------------------
        Audio::AudioSystem* Application::GetAudioSystemPtr()
        {
            return pAudioSystem;
        }
        //-----------------------------------------------------
        //-----------------------------------------------------
        FileSystem* Application::GetFileSystemPtr()
        {
            return mspFileSystem;
        }
        //----------------------------------------------------
        //----------------------------------------------------
		void Application::OnFrameBegin(f32 infDt, TimeIntervalSecs inuddwTimestamp)
		{
            if(s_shouldNotifyConnectionsResumeEvent == true)
			{
				s_shouldNotifyConnectionsResumeEvent = false;
				ResumeApplication();
			}
            
            if(s_isSuspending)
            {
                // Updating after told to suspend so early out
                return;
            }
            
#ifdef CS_ENABLE_DEBUGSTATS
            Debugging::DebugStats::RecordEvent("FrameTime", infDt);
			Debugging::DebugStats::RecordEvent("FPS", 1.0f/infDt);
#endif
            
			//Update the app time since start
			uddwCurrentAppTime = inuddwTimestamp;
            
			TaskScheduler::ExecuteMainThreadTasks();
            
            //We do not need to render as often as we update so this callback will be triggered
            //less freqenctly than the update frequency suggests. We must work out how many times to update based on the time since last frame
            //and our actual update frequency. We carry the remainder to the next frame until we have a full update cycle
            s_updateIntervalRemainder = MathUtils::Min(s_updateIntervalRemainder + infDt, GetUpdateIntervalMax());
            
			//Force the input system to distribute any buffered input
			if(mpInputSystem != nullptr)
			{
				mpInputSystem->FlushBufferedInput();
			}
            
            while((s_updateIntervalRemainder >= Application::GetUpdateInterval()) || s_isFirstFrame)
            {
                s_updateIntervalRemainder -=  Application::GetUpdateInterval();
                mStateMgr.FixedUpdate(Application::GetUpdateInterval());
                
                s_isFirstFrame = false;
            }
            
            //Tell the state manager to update the active state
            Update(infDt);
            
            //Render the scene
            mpRenderer->RenderToScreen(Core::Application::Get()->GetStateManagerPtr()->GetActiveScenePtr());
            
#ifdef CS_ENABLE_DEBUGSTATS
			DebugStats::Clear();
#endif
		}
        //----------------------------------------------------
        //----------------------------------------------------
		void Application::OnSuspend()
		{
            CS_LOG_DEBUG("App Suspending...");
            
			s_isSuspending = true;
            
			//Tell the active state to save it's data etc
			mStateMgr.Pause();
            
			//We must invalidate the application timer. This will stop sub-system updates
			pPlatformSystem->SetUpdaterActive(false);
            
			//We need to rebind or rebuild the context if it was stolen
			if(mpRenderSystem)
			{
				mpRenderSystem->Suspend();
			}
            
			ApplicationEvents::GetSuspendEvent().NotifyConnections();
			ApplicationEvents::GetLateSuspendEvent().NotifyConnections();
			
			CS_LOG_DEBUG("App Finished Suspending...");
		}
        //----------------------------------------------------
        //----------------------------------------------------
		void Application::OnResume()
		{
            s_shouldNotifyConnectionsResumeEvent = true;
            
			//We must restart the application timer. This will automatically restart system updates
			pPlatformSystem->SetUpdaterActive(true);
		}
        //----------------------------------------------------
        //----------------------------------------------------
		void Application::OnScreenChangedOrientation(ScreenOrientation ineOrientation)
		{
			Screen::SetOrientation(ineOrientation);
            
			if(mpRenderSystem)
			{
				mpRenderSystem->OnScreenOrientationChanged(Core::Screen::GetOrientedWidth(), Core::Screen::GetOrientedHeight());
			}
            
			//Flip the screen
			SetOrientation(ineOrientation);
			ApplicationEvents::GetScreenOrientationChangedEvent().NotifyConnections(ineOrientation);
            
			CS_LOG_DEBUG("Screen Oriented Notification");
            
		}
        //----------------------------------------------------
        //----------------------------------------------------
		void Application::OnScreenResized(u32 inudwWidth, u32 inudwHeight)
		{
			Screen::SetRawDimensions(Core::Vector2((f32)inudwWidth, (f32)inudwHeight));
            
			if(mpRenderSystem)
			{
				mpRenderSystem->OnScreenOrientationChanged(inudwWidth, inudwHeight);
			}
            
			if(HasTouchInput() == true)
			{
				Input::TouchScreen * pTouchScreen = GetSystemImplementing(Input::InputSystem::InterfaceID)->GetInterface<Input::InputSystem>()->GetTouchScreenPtr();
				pTouchScreen->SetScreenHeight(Screen::GetOrientedHeight());
			}
            
			ApplicationEvents::GetScreenResizedEvent().NotifyConnections(inudwWidth, inudwHeight);
            
			CS_LOG_DEBUG("Screen resized Notification");
		}
        //----------------------------------------------------
        //----------------------------------------------------
		void Application::OnApplicationMemoryWarning()
		{
			CS_LOG_DEBUG("Memory Warning. Clearing resource cache...");
			ResourceManagerDispenser::GetSingletonPtr()->FreeResourceCaches();
			ApplicationEvents::GetLowMemoryEvent().NotifyConnections();
		}
        //----------------------------------------------------
        //----------------------------------------------------
		void Application::OnGoBack()
		{
			CS_LOG_DEBUG("Go back event.");
			mStateMgr.GetActiveState()->OnGoBack();
			ApplicationEvents::GetGoBackEvent().NotifyConnections();
		}
        //----------------------------------------------------
        //----------------------------------------------------
		System* Application::GetSystemImplementing(InterfaceIDType inInterfaceID)
		{
			for (std::vector<SystemSPtr>::const_iterator it = mSystems.begin(); it != mSystems.end(); ++it)
			{
				if ((*it)->IsA(inInterfaceID))
				{
					return (*it).get();
				}
			}
			
			CS_LOG_WARNING("Application cannot find implementing systems");
			return nullptr;
		}
        //----------------------------------------------------
        //----------------------------------------------------
		void Application::PostCreateSystems()
		{
            //Loop round all the created systems and categorise them
			for(std::vector<SystemSPtr>::iterator it = mSystems.begin(); it != mSystems.end(); ++it) 
			{
				if ((*it)->IsA(IUpdateable::InterfaceID))
				{
					mUpdateableSystems.push_back((*it)->GetInterface<IUpdateable>());
				}
				if ((*it)->IsA(IComponentProducer::InterfaceID)) 
				{
                    IComponentProducer* pProducer = (*it)->GetInterface<IComponentProducer>();
                    u32 udwNumFactoriesInSystem = pProducer->GetNumComponentFactories();

                    for(u32 i=0; i<udwNumFactoriesInSystem; ++i)
                    {
                        mpComponentFactoryDispenser->RegisterComponentFactory(pProducer->GetComponentFactoryPtr(i));
                    }
				}
				if ((*it)->IsA(ResourceProvider::InterfaceID)) 
				{
					mResourceProviders.push_back((*it)->GetInterface<ResourceProvider>());
				}
			}

            //Give the resource managers their providers
            mpResourceManagerDispenser->SetResourceProviders(mResourceProviders);

            pPlatformSystem->PostCreateSystems();
		}
        //----------------------------------------------------
        //----------------------------------------------------
        void Application::LoadDefaultResources()
        {
            Json::Value jRoot;
            if(Utils::ReadJson(StorageLocation::k_package, "App.config", &jRoot) == true)
            {
                if(jRoot.isMember("MaxFPS"))
                {
                    u32 udwMaxFPS = jRoot["MaxFPS"].asUInt();
                    pPlatformSystem->SetMaxFPS(udwMaxFPS);
                }
                
                if(jRoot.isMember("MasterText"))
                {
                    StorageLocation eStorageLocation = GetStorageLocationFromString(jRoot["MasterText"].get("Location", "Package").asString());
                    std::string strPath = jRoot["MasterText"].get("Path", "").asString();
                    LocalisedText::RefreshMasterText(eStorageLocation, strPath);
                }
                
                if(jRoot.isMember("DefaultMesh"))
                {
                    StorageLocation eStorageLocation = GetStorageLocationFromString(jRoot["DefaultMesh"].get("Location", "Package").asString());
                    std::string strPath = jRoot["DefaultMesh"].get("Path", "").asString();
                    pDefaultMesh = LOAD_RESOURCE(Rendering::Mesh, eStorageLocation, strPath);
                }
                
                if(jRoot.isMember("DefaultFont"))
                {
                    StorageLocation eStorageLocation = GetStorageLocationFromString(jRoot["DefaultFont"].get("Location", "Package").asString());
                    std::string strPath = jRoot["DefaultFont"].get("Path", "").asString();
                    pDefaultFont = LOAD_RESOURCE(Rendering::Font, eStorageLocation, strPath);
                }
                
                if(jRoot.isMember("DefaultMaterial"))
                {
                    StorageLocation eStorageLocation = GetStorageLocationFromString(jRoot["DefaultMaterial"].get("Location", "Package").asString());
                    std::string strPath = jRoot["DefaultMaterial"].get("Path", "").asString();
                    pDefaultMaterial = LOAD_RESOURCE(Rendering::Material, eStorageLocation, strPath);
                }
            }
        }
        //----------------------------------------------------
        //----------------------------------------------------
        void Application::DetermineResourceDirectories()
        {
            //Get a list of the resource directories and determine which one this device should be
            //loading from based on it's screen
            std::vector<ResourceDirectoryInfo> aDirectoryInfos;
            std::string strDefaultDir, strDeviceDir, strDefaultDeviceDir;
            SetResourceDirectories(aDirectoryInfos, strDefaultDeviceDir, strDefaultDir);
            
            //Sort the info by resolution low to high
            std::sort(aDirectoryInfos.begin(), aDirectoryInfos.end(), ResolutionSortPredicate);
            
            u32 udwCurrentRes = Screen::GetOrientedWidth() * Screen::GetOrientedHeight();
            f32 fCurrenctDensity = Screen::GetDensity();
            f32 fAssetDensity = 1.0f;
            for(std::vector<ResourceDirectoryInfo>::const_iterator it = aDirectoryInfos.begin(); it != aDirectoryInfos.end(); ++it)
            {
                //The density and the resolution must both be under the maximum for the directory to be selected.
                if(udwCurrentRes <= it->udwMaxRes && fCurrenctDensity <= it->fMaxDensity)
                {
                    strDeviceDir = it->strDirectory;
                    fAssetDensity = it->fResourcesDensity;
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
		void Application::Update(f32 infDT)
		{
            infDT *= mfUpdateSpeed;
            
			CoreTimer::Update(infDT);
            
            NotificationScheduler::Update(infDT);
            
			//Update sub systems
            if (mbUpdateSystems == true)
            {
				for(std::vector<IUpdateable*>::iterator it = mUpdateableSystems.begin(); it != mUpdateableSystems.end(); ++it)
				{
					(*it)->Update(infDT);
				}
            }
			
			//Tell the state manager to update the active state
			mStateMgr.Update(infDT);
		}
        //----------------------------------------------------
        //----------------------------------------------------
		void Application::SetOrientation(ScreenOrientation inOrientation)
		{
			if(mpRenderer->GetActiveCameraPtr())
			{
				mpRenderer->GetActiveCameraPtr()->SetViewportOrientation(inOrientation);
			}
            
			if(HasTouchInput() == true)
			{
				Input::TouchScreen * pTouchScreen = GetSystemImplementing(Input::InputSystem::InterfaceID)->GetInterface<Input::InputSystem>()->GetTouchScreenPtr();
				pTouchScreen->SetScreenHeight(Screen::GetOrientedHeight());
			}
		}
        //----------------------------------------------------
        //----------------------------------------------------
		void Application::ResumeApplication()
		{
			CS_LOG_DEBUG("App Resuming...");
            
			if(mpRenderSystem != nullptr)
			{
				mpRenderSystem->Resume();
			}
            
			s_isSuspending = false;
			ApplicationEvents::GetResumeEvent().NotifyConnections();
            
			//Tell the active state to continue
			mStateMgr.Resume();
			
			CS_LOG_DEBUG("App Finished Resuming...");
		}
        //----------------------------------------------------
        //----------------------------------------------------
		void Application::SetHasTouchInput(bool inbTouchInput)
		{
			mbHasTouchInput = inbTouchInput;
		}
        //----------------------------------------------------
        //----------------------------------------------------
		bool Application::HasTouchInput()
		{
			return mbHasTouchInput;
		}
        //----------------------------------------------------
        //----------------------------------------------------
		Application::~Application()
		{
			mStateMgr.DestroyAll();

			pDefaultFont.reset();
			pDefaultMesh.reset();
			pDefaultMaterial.reset();

			CS_SAFEDELETE(pPlatformSystem);
            CS_SAFEDELETE(mpResourceManagerDispenser);
            CS_SAFEDELETE(mpComponentFactoryDispenser);

			//We have an issue with the order of destruction of systems.
			while(mSystems.empty() == false)
			{
				SystemSPtr pSystem = mSystems.back();
				mSystems.pop_back();
				pSystem.reset();
			}
		}
	}
}
