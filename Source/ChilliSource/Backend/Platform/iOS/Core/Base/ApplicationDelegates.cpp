/*
 *  ApplicationDelegates.cpp
 *  MoFlow
 *
 *  Created by Ian Copland on 17/01/2012.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Backend/Platform/iOS/Core/Base/ApplicationDelegates.h>
#include <ChilliSource/Input/Base/InputSystem.h>
#include <ChilliSource/Input/Pointer/TouchScreen.h>
#include <ChilliSource/Core/Math/MathUtils.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/ApplicationEvents.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>
#include <ChilliSource/Core/Resource/ResourceManagerDispenser.h>
#include <ChilliSource/Core/Base/Screen.h>
#include <ChilliSource/Core/State/StateManager.h>
#include <ChilliSource/Core/Base/PlatformSystem.h>
#include <ChilliSource/Rendering/Base/RenderSystem.h>
#include <ChilliSource/Rendering/Base/Renderer.h>
#include <ChilliSource/Rendering/Camera/CameraComponent.h>

#ifdef DEBUG_STATS
#include <ChilliSource/Debugging/Base/DebugStats.h>
#endif

namespace ChilliSource
{
    DEFINE_CREATABLE(IApplicationDelegates, iOS::CApplicationDelegates);
    
	namespace iOS
	{
		//----------------------------------------------
		/// Constructor
		//----------------------------------------------
		CApplicationDelegates::CApplicationDelegates()
        : mfUpdateIntervalRemainder(0.0f), mbShouldInvokeResumeEvent(false), mbIsFirstFrame(true),  mbIsSuspending(false)
		{
            
		}
		//----------------------------------------------
		/// SetInvokeResumeEvent
		//----------------------------------------------
		void CApplicationDelegates::SetInvokeResumeEvent(bool inbResume)
		{
			mbShouldInvokeResumeEvent = inbResume;
		}
		//----------------------------------------------
		/// On Application Resumed
		//----------------------------------------------
		void CApplicationDelegates::OnApplicationResumed()
		{
			DEBUG_LOG("App Resuming...");
            
			if(Core::CApplication::GetRenderSystemPtr() != NULL)
			{
				Core::CApplication::GetRenderSystemPtr()->Resume();
			}
            
			mbIsSuspending = false;
			Core::CApplicationEvents::GetResumeEvent().Invoke();
            
			//Tell the active state to continue
			Core::CApplication::GetStateManagerPtr()->Resume();
			
			DEBUG_LOG("App Finished Resuming...");
		}
		//----------------------------------------------
		/// On Application Suspended
		//----------------------------------------------
		void CApplicationDelegates::OnApplicationSuspended()
		{
			DEBUG_LOG("App Suspending...");
			mbIsSuspending = true;
			//Tell the active state to save it's data etc
			Core::CApplication::GetStateManagerPtr()->Pause();
            
			//We must invalidate the application timer. This will stop sub-system updates
			Core::CApplication::GetPlatformSystemPtr()->SetUpdaterActive(false);
            
			//We need to rebind or rebuild the context if it was stolen
			if(Core::CApplication::GetRenderSystemPtr())
			{
				Core::CApplication::GetRenderSystemPtr()->Suspend();
			}
            
			Core::CApplicationEvents::GetSuspendEvent().Invoke();
			Core::CApplicationEvents::GetLateSuspendEvent().Invoke();
			
			DEBUG_LOG("App Finished Suspending...");
		}
		//----------------------------------------------
		/// On Frame Begin
		//----------------------------------------------
		void CApplicationDelegates::OnFrameBegin(f32 infDt, TimeIntervalSecs inuddwTimestamp)
		{
			if(mbShouldInvokeResumeEvent)
			{
				mbShouldInvokeResumeEvent = false;
				OnApplicationResumed();
			}
            
            if(mbIsSuspending)
            {
                // Updating after told to suspend so early out
                return;
            }
            
#ifdef DEBUG_STATS
			CDebugStats::RecordEvent("FrameTime", infDt);
			CDebugStats::RecordEvent("FPS", 1.0f/infDt);
#endif
            
			//Update the app time since start
			Core::CApplication::SetAppElapsedTime(inuddwTimestamp);
            
			CTaskScheduler::ExecuteMainThreadTasks();
            
            //We do not need to render as often as we update so this callback will be triggered
            //less freqenctly than the update frequency suggests. We must work out how many times to update based on the time since last frame
            //and our actual update frequency. We carry the remainder to the next frame until we have a full update cycle
            mfUpdateIntervalRemainder = Core::CMathUtils::Min(mfUpdateIntervalRemainder + infDt, Core::CApplication::GetUpdateIntervalMax());
            
			//Force the input system to distribute any buffered input
			if(Core::CApplication::GetInputSystemPtr() != NULL)
			{
				Core::CApplication::GetInputSystemPtr()->FlushBufferedInput();
			}
            
            while((mfUpdateIntervalRemainder >= Core::CApplication::GetUpdateInterval()) || mbIsFirstFrame)
            {
                mfUpdateIntervalRemainder -=  Core::CApplication::GetUpdateInterval();
                Core::CApplication::GetStateManagerPtr()->FixedUpdate(Core::CApplication::GetUpdateInterval());
                
                mbIsFirstFrame = false;
            }
        
            //Tell the state manager to update the active state
            Core::CApplication::Update(infDt);
            
            //Render the scene
            Core::CApplication::GetRendererPtr()->RenderToScreen(Core::CApplication::GetStateManagerPtr()->GetActiveScenePtr());

#ifdef DEBUG_STATS
			CDebugStats::Clear();
#endif
		}
		//----------------------------------------------
		/// On Screen Changed Orientation
		//----------------------------------------------
		void CApplicationDelegates::OnScreenChangedOrientation(Core::ScreenOrientation ineOrientation)
		{
			Core::CScreen::SetOrientation(ineOrientation);

			if(Core::CApplication::GetRenderSystemPtr())
			{
				Core::CApplication::GetRenderSystemPtr()->OnScreenOrientationChanged(Core::CScreen::GetOrientedWidth(), Core::CScreen::GetOrientedHeight());
			}
            
			//Flip the screen
			Core::CApplication::SetOrientation(ineOrientation);
			ChilliSource::Core::CApplicationEvents::GetScreenOrientationChangedEvent().Invoke(ineOrientation);
            
			DEBUG_LOG("Screen Oriented Notification");
		}
		//----------------------------------------------
		/// On Screen Resized
		//----------------------------------------------
		void CApplicationDelegates::OnScreenResized(u32 inudwWidth, u32 inudwHeight)
		{
			Core::CScreen::SetRawDimensions(Core::CVector2((f32)inudwWidth, (f32)inudwHeight));

			if(Core::CApplication::GetRenderSystemPtr())
			{
				Core::CApplication::GetRenderSystemPtr()->OnScreenOrientationChanged(inudwWidth, inudwHeight);
			}

			if(Core::CApplication::HasTouchInput() == true)
			{
				Input::ITouchScreen * pTouchScreen = Core::CApplication::GetSystemImplementing(Input::IInputSystem::InterfaceID)->GetInterface<Input::IInputSystem>()->GetTouchScreenPtr();
				pTouchScreen->SetScreenHeight(Core::CScreen::GetOrientedHeight());
			}

			Core::CApplicationEvents::GetScreenResizedEvent().Invoke(inudwWidth, inudwHeight);
            
			DEBUG_LOG("Screen resized Notification");
		}
		//----------------------------------------------
		/// On Application Memory Warning
		//----------------------------------------------
		void CApplicationDelegates::OnApplicationMemoryWarning()
		{
			DEBUG_LOG("Memory Warning. Clearing resource cache...");
			Core::CResourceManagerDispenser::GetSingletonPtr()->FreeResourceCaches();
			ChilliSource::Core::CApplicationEvents::GetLowMemoryEvent().Invoke();
		}
		//----------------------------------------------
		/// On Go Back
		//----------------------------------------------
		void CApplicationDelegates::OnGoBack()
		{
			DEBUG_LOG("Go back event.");
			Core::CApplication::GetStateManagerPtr()->GetActiveState()->OnGoBack();
			ChilliSource::Core::CApplicationEvents::GetGoBackEvent().Invoke();
		}
		//----------------------------------------------
		/// On Set Orientiation
		//----------------------------------------------
		void CApplicationDelegates::OnSetOrientation(Core::ScreenOrientation inOrientation)
		{
			if(Core::CApplication::GetRendererPtr()->GetActiveCameraPtr())
			{
				Core::CApplication::GetRendererPtr()->GetActiveCameraPtr()->SetViewportOrientation(inOrientation);
			}
            
			if(Core::CApplication::HasTouchInput() == true)
			{
				Input::ITouchScreen * pTouchScreen = Core::CApplication::GetSystemImplementing(Input::IInputSystem::InterfaceID)->GetInterface<Input::IInputSystem>()->GetTouchScreenPtr();
				pTouchScreen->SetScreenHeight(Core::CScreen::GetOrientedHeight());
			}
		}
		//----------------------------------------------
		/// Destructor
		//----------------------------------------------
		CApplicationDelegates::~CApplicationDelegates()
		{
            
		}
	}
}