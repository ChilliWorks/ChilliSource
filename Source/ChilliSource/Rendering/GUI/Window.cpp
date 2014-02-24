//
//  GUIWindow.cpp
//  moFlo
//
//  Created by Scott Downie on 21/04/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#include <ChilliSource/Rendering/GUI/Window.h>

#include <ChilliSource/Core/Base/ApplicationEvents.h>

#include <ChilliSource/Input/Base/InputSystem.h>

#ifdef DEBUG_STATS
#include <ChilliSource/Rendering/GUI/Defaults/DebugStatsView.h>
#include <ChilliSource/Debugging/Base/DebugStats.h>
#endif

namespace ChilliSource
{
    DEFINE_CREATABLE(ISurface, GUI::CWindow);
    
	namespace Rendering
	{
		//-----------------------------------------------
		/// Constructor
		///
		/// Creates the window with an absolute size
		//-----------------------------------------------
		CWindow::CWindow():mbListeningForTouches(false)
		{
			//We are the root window of the view hierarchy
			//Each hierarchy can only have on window
			mpRootWindow = this;
            
			Core::CVector2 vAbsSize = Core::CScreen::GetOrientedDimensions();
            
			//The window is fullscreen and centred
			SetSize(UnifiedVector2(Core::CVector2(1.0f, 1.0f), vAbsSize));
			SetPosition(UnifiedVector2(Core::CVector2::ZERO, vAbsSize * 0.5f));
			SetName("RootWindow");
            
			//Register for screen rotation events
			Core::CApplicationEvents::GetScreenOrientationChangedEvent() += Core::ApplicationScreenOrientationDelegate(this, &CWindow::OnScreenOrientationChanged);
			Core::CApplicationEvents::GetScreenResizedEvent() += Core::ApplicationScreenResizeDelegate(this, &CWindow::OnScreenResized);
		}
		//-----------------------------------------------------
		/// Set Input System
		///
		/// @param pointer to the input system
		//-----------------------------------------------------
		void CWindow::SetInputSystem(Input::IInputSystem* inpInputSystem)
		{
			mpInputSystem = inpInputSystem;
            
			//The window is responsible for receiving input for this scene
            ListenForTouches();
		}
        
        void CWindow::ListenForTouches()
        {
            if(mpInputSystem && mpInputSystem->GetTouchScreenPtr() && !mbListeningForTouches)
			{
				mpInputSystem->GetTouchScreenPtr()->GetTouchBeganEvent() += Input::TouchEventDelegate(this, &CWindow::_OnTouchBegan);
				mpInputSystem->GetTouchScreenPtr()->GetTouchMovedEvent() += Input::TouchEventDelegate(this, &CWindow::_OnTouchMoved);
				mpInputSystem->GetTouchScreenPtr()->GetTouchEndEvent() += Input::TouchEventDelegate(this, &CWindow::_OnTouchEnded);
                mbListeningForTouches=true;
			}
        }
        
        void CWindow::UnlistenFromTouches()
        {
            if(mpInputSystem && mpInputSystem->GetTouchScreenPtr() && mbListeningForTouches)
			{
				mpInputSystem->GetTouchScreenPtr()->GetTouchBeganEvent() -= Input::TouchEventDelegate(this, &CWindow::_OnTouchBegan);
				mpInputSystem->GetTouchScreenPtr()->GetTouchMovedEvent() -= Input::TouchEventDelegate(this, &CWindow::_OnTouchMoved);
				mpInputSystem->GetTouchScreenPtr()->GetTouchEndEvent() -= Input::TouchEventDelegate(this, &CWindow::_OnTouchEnded);
                mbListeningForTouches=false;
			}
        }
        
		//-----------------------------------------------------------
		/// On Screen Orientation Changed
		///
		/// Triggered if the screen orientation changes so we can
		/// resize ourself
		//-----------------------------------------------------------
		void CWindow::OnScreenOrientationChanged(Core::ScreenOrientation ineOrientation)
		{
			Core::CVector2 vAbsSize = Core::CScreen::GetOrientedDimensions();
            
			//The window is fullscreen and centred
			SetSize(UnifiedVector2(Core::CVector2(1.0f, 1.0f), vAbsSize));
			SetPosition(UnifiedVector2(Core::CVector2(0.0f, 0.0f), vAbsSize * 0.5f));
            
			//Notify all subviews and they can decide what to do
			CGUIView::OnScreenOrientationChanged();
		}
		//-----------------------------------------------------------
		/// On Screen Resized
		///
		/// Triggered if the screen resizes
		//-----------------------------------------------------------
		void CWindow::OnScreenResized(u32 inudwWidth, u32 inudwHeight)
		{
			Core::CVector2 vAbsSize((f32)inudwWidth, (f32)inudwHeight);
            
			//The window is fullscreen and centred
			SetSize(UnifiedVector2(Core::CVector2(1.0f, 1.0f), vAbsSize));
			SetPosition(UnifiedVector2(Core::CVector2(0.0f, 0.0f), vAbsSize * 0.5f));
            
			//Notify all subviews and they can decide what to do
			CGUIView::OnScreenOrientationChanged();
		}
		//-----------------------------------------------------------
		/// On Touch Began
		///
		/// Called when the window receives cursor/touch input
		///
		/// @param Touch data
		//-----------------------------------------------------------
		void CWindow::_OnTouchBegan(const Input::TouchInfo & insTouchInfo)
		{
			if (!UserInteraction)
				return;
			
			mSubviewsCopy = mSubviews;
            
			mInputEvents.OnTouchBegan(this, insTouchInfo);
			
			//We need to notify any subviews they get first dibs
			for(CGUIView::Subviews::reverse_iterator it = mSubviewsCopy.rbegin(); it != mSubviewsCopy.rend(); ++it)
			{
				if(((*it)->IsAcceptTouchesOutsideOfBoundsEnabled() || (*it)->Contains(insTouchInfo.vLocation)))
				{
					if((*it)->OnTouchBegan(insTouchInfo))
					{
						//This means the touch has been consumed
						//and we should not notify anyone else
                        mSubviewsCopy.clear();
						return;
					}
				}
			}
            
			//If the touch has not been consumed we then notify
			//the outside world
            mSubviewsCopy.clear();
            mTouchBeganEvent.Invoke(insTouchInfo);
		}
		//-----------------------------------------------------------
		/// On Touch Moved
		///
		/// Called when the window receives cursor/touch input
		///
		/// @param Touch data
		//-----------------------------------------------------------
		void CWindow::_OnTouchMoved(const Input::TouchInfo & insTouchInfo)
		{
			if (!UserInteraction)
				return;
			
			mSubviewsCopy = mSubviews;
            
			mInputEvents.OnTouchMoved(this, insTouchInfo);
			
			//We need to notify any subviews they get first dibs
			for(CGUIView::Subviews::reverse_iterator it = mSubviewsCopy.rbegin(); it != mSubviewsCopy.rend(); ++it)
			{
				if((*it)->OnTouchMoved(insTouchInfo))
				{
					//This means the touch has been consumed
					//and we should not notify anyone else
                    mSubviewsCopy.clear();
					return;
				}
			}
            
			//If the touch has not been consumed we then notify
			//the outside world
            mSubviewsCopy.clear();
            mTouchMovedEvent.Invoke(insTouchInfo);
		}
		//-----------------------------------------------------------
		/// On Touch Ended
		///
		/// Called when the window stops receiving cursor/touch input
		///
		/// @param Touch data
		//-----------------------------------------------------------
		void CWindow::_OnTouchEnded(const Input::TouchInfo & insTouchInfo)
		{
			if (!UserInteraction)
				return;
			
			mSubviewsCopy = mSubviews;
            
			mInputEvents.OnTouchEnded(this, insTouchInfo);
			
			//We need to notify any subviews they get first dibs
			for(CGUIView::Subviews::reverse_iterator it = mSubviewsCopy.rbegin(); it != mSubviewsCopy.rend(); ++it)
			{
				(*it)->OnTouchEnded(insTouchInfo);
			}
            
			//If the touch has not been consumed we then notify
			//the outside world
            mSubviewsCopy.clear();
            mTouchEndedEvent.Invoke(insTouchInfo);
		}
        //-----------------------------------------------------------
        /// On Touch Began
        ///
        /// Called when the window receives cursor/touch input
        ///
        /// @param Touch data
        //-----------------------------------------------------------
        bool CWindow::OnTouchBegan(const Input::TouchInfo & insTouchInfo)
        {
            return false;
        }
        //-----------------------------------------------------------
        /// On Touch Moved
        ///
        /// Called when the window receives cursor/touch input
        ///
        /// @param Touch data
        //-----------------------------------------------------------
        bool CWindow::OnTouchMoved(const Input::TouchInfo & insTouchInfo)
        {
            return false;
        }
        //-----------------------------------------------------------
        /// On Touch Ended
        ///
        /// Called when the window stops receiving cursor/touch input
        ///
        /// @param Touch data
        //-----------------------------------------------------------
        void CWindow::OnTouchEnded(const Input::TouchInfo & insTouchInfo)
        {
            
        }
#ifdef DEBUG_STATS
        //-----------------------------------------------------
        /// Draw
        ///
        /// Override this implementation to draw the GUI
        /// using the primitves within canvas renderer i.e.
        /// DrawBox, DrawString etc
        ///
        /// @param Canvas renderer
        //-----------------------------------------------------
        void CWindow::Draw(Rendering::CCanvasRenderer * inpCanvas)
        {
            CGUIView::Draw(inpCanvas);
            
            if(CDebugStats::IsEnabled())
            {
                CDebugStatsView::GetSingletonPtr()->SetParentView(this);
                CDebugStatsView::GetSingletonPtr()->SetRootWindow(this);
                CDebugStatsView::GetSingletonPtr()->RefreshStats();
                CDebugStatsView::GetSingletonPtr()->Draw(inpCanvas);
            }
        }
#endif
		//-----------------------------------------------
		/// Destructor
		///
		//-----------------------------------------------
		CWindow::~CWindow()
		{
			//Deregister for touch delegates
			//The window is responsible for receiving input for this scene
			UnlistenFromTouches();
            
			Core::CApplicationEvents::GetScreenOrientationChangedEvent() -= Core::ApplicationScreenOrientationDelegate(this, &CWindow::OnScreenOrientationChanged);
			Core::CApplicationEvents::GetScreenResizedEvent() -= Core::ApplicationScreenResizeDelegate(this, &CWindow::OnScreenResized);
		}
	}
}