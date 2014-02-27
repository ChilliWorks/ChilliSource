//
//  GUIWindow.cpp
//  moFlo
//
//  Created by Scott Downie on 21/04/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#include <ChilliSource/GUI/Base/Window.h>

#include <ChilliSource/Core/Base/ApplicationEvents.h>
#include <ChilliSource/Core/Base/MakeDelegate.h>

#include <ChilliSource/Input/Base/InputSystem.h>

#ifdef DEBUG_STATS
#include <ChilliSource/GUI/Debug/DebugStatsView.h>
#include <ChilliSource/Debugging/Base/DebugStats.h>
#endif

namespace ChilliSource
{
	namespace GUI
	{
		//-----------------------------------------------
		/// Constructor
		///
		/// Creates the window with an absolute size
		//-----------------------------------------------
		Window::Window():mbListeningForTouches(false)
		{
			//We are the root window of the view hierarchy
			//Each hierarchy can only have on window
			mpRootWindow = this;
            
			Core::CVector2 vAbsSize = Core::CScreen::GetOrientedDimensions();
            
			//The window is fullscreen and centred
			SetSize(Core::UnifiedVector2(Core::CVector2(1.0f, 1.0f), vAbsSize));
			SetPosition(Core::UnifiedVector2(Core::CVector2::ZERO, vAbsSize * 0.5f));
			SetName("RootWindow");
            
			//Register for screen rotation events
			Core::CApplicationEvents::GetScreenOrientationChangedEvent() += Core::MakeDelegate(this, &Window::OnScreenOrientationChanged);
			Core::CApplicationEvents::GetScreenResizedEvent() += Core::MakeDelegate(this, &Window::OnScreenResized);
		}
		//-----------------------------------------------------
		/// Set Input System
		///
		/// @param pointer to the input system
		//-----------------------------------------------------
		void Window::SetInputSystem(Input::InputSystem* inpInputSystem)
		{
			mpInputSystem = inpInputSystem;
            
			//The window is responsible for receiving input for this scene
            ListenForTouches();
		}
        //-----------------------------------------------------
        /// Get Input System
        ///
        /// @return pointer to the input system
        //-----------------------------------------------------
        Input::InputSystem* Window::GetInputSystem()
        {
            return mpInputSystem;
        }
        void Window::ListenForTouches()
        {
            if(mpInputSystem && mpInputSystem->GetTouchScreenPtr() && !mbListeningForTouches)
			{
				mpInputSystem->GetTouchScreenPtr()->GetTouchBeganEvent() += Core::MakeDelegate(this, &Window::_OnTouchBegan);
				mpInputSystem->GetTouchScreenPtr()->GetTouchMovedEvent() += Core::MakeDelegate(this, &Window::_OnTouchMoved);
				mpInputSystem->GetTouchScreenPtr()->GetTouchEndEvent() += Core::MakeDelegate(this, &Window::_OnTouchEnded);
                mbListeningForTouches=true;
			}
        }
        
        void Window::UnlistenFromTouches()
        {
            if(mpInputSystem && mpInputSystem->GetTouchScreenPtr() && mbListeningForTouches)
			{
				mpInputSystem->GetTouchScreenPtr()->GetTouchBeganEvent() -= Core::MakeDelegate(this, &Window::_OnTouchBegan);
				mpInputSystem->GetTouchScreenPtr()->GetTouchMovedEvent() -= Core::MakeDelegate(this, &Window::_OnTouchMoved);
				mpInputSystem->GetTouchScreenPtr()->GetTouchEndEvent() -= Core::MakeDelegate(this, &Window::_OnTouchEnded);
                mbListeningForTouches=false;
			}
        }
		//-----------------------------------------------------------
		/// On Screen Orientation Changed
		///
		/// Triggered if the screen orientation changes so we can
		/// resize ourself
		//-----------------------------------------------------------
		void Window::OnScreenOrientationChanged(Core::ScreenOrientation ineOrientation)
		{
			Core::CVector2 vAbsSize = Core::CScreen::GetOrientedDimensions();
            
			//The window is fullscreen and centred
			SetSize(Core::UnifiedVector2(Core::CVector2(1.0f, 1.0f), vAbsSize));
			SetPosition(Core::UnifiedVector2(Core::CVector2(0.0f, 0.0f), vAbsSize * 0.5f));
            
			//Notify all subviews and they can decide what to do
			GUIView::OnScreenOrientationChanged();
		}
		//-----------------------------------------------------------
		/// On Screen Resized
		///
		/// Triggered if the screen resizes
		//-----------------------------------------------------------
		void Window::OnScreenResized(u32 inudwWidth, u32 inudwHeight)
		{
			Core::CVector2 vAbsSize((f32)inudwWidth, (f32)inudwHeight);
            
			//The window is fullscreen and centred
			SetSize(Core::UnifiedVector2(Core::CVector2(1.0f, 1.0f), vAbsSize));
			SetPosition(Core::UnifiedVector2(Core::CVector2(0.0f, 0.0f), vAbsSize * 0.5f));
            
			//Notify all subviews and they can decide what to do
			GUIView::OnScreenOrientationChanged();
		}
		//-----------------------------------------------------------
		/// On Touch Began
		///
		/// Called when the window receives cursor/touch input
		///
		/// @param Touch data
		//-----------------------------------------------------------
		void Window::_OnTouchBegan(const Input::TouchInfo & insTouchInfo)
		{
			if (!UserInteraction)
				return;
			
			mSubviewsCopy = mSubviews;
            
			mInputEvents.OnTouchBegan(this, insTouchInfo);
			
			//We need to notify any subviews they get first dibs
			for(GUIView::Subviews::reverse_iterator it = mSubviewsCopy.rbegin(); it != mSubviewsCopy.rend(); ++it)
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
		void Window::_OnTouchMoved(const Input::TouchInfo & insTouchInfo)
		{
			if (!UserInteraction)
				return;
			
			mSubviewsCopy = mSubviews;
            
			mInputEvents.OnTouchMoved(this, insTouchInfo);
			
			//We need to notify any subviews they get first dibs
			for(GUIView::Subviews::reverse_iterator it = mSubviewsCopy.rbegin(); it != mSubviewsCopy.rend(); ++it)
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
		void Window::_OnTouchEnded(const Input::TouchInfo & insTouchInfo)
		{
			if (!UserInteraction)
				return;
			
			mSubviewsCopy = mSubviews;
            
			mInputEvents.OnTouchEnded(this, insTouchInfo);
			
			//We need to notify any subviews they get first dibs
			for(GUIView::Subviews::reverse_iterator it = mSubviewsCopy.rbegin(); it != mSubviewsCopy.rend(); ++it)
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
        bool Window::OnTouchBegan(const Input::TouchInfo & insTouchInfo)
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
        bool Window::OnTouchMoved(const Input::TouchInfo & insTouchInfo)
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
        void Window::OnTouchEnded(const Input::TouchInfo & insTouchInfo)
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
        void Window::Draw(CCanvasRenderer * inpCanvas)
        {
            GUIView::Draw(inpCanvas);
            
            if(DebugStats::IsEnabled())
            {
                DebugStatsView::GetSingletonPtr()->SetParentView(this);
                DebugStatsView::GetSingletonPtr()->SetRootWindow(this);
                DebugStatsView::GetSingletonPtr()->RefreshStats();
                DebugStatsView::GetSingletonPtr()->Draw(inpCanvas);
            }
        }
#endif
		//-----------------------------------------------
		/// Destructor
		///
		//-----------------------------------------------
		Window::~Window()
		{
			//Deregister for touch delegates
			//The window is responsible for receiving input for this scene
			UnlistenFromTouches();
            
			Core::CApplicationEvents::GetScreenOrientationChangedEvent() -= Core::MakeDelegate(this, &Window::OnScreenOrientationChanged);
			Core::CApplicationEvents::GetScreenResizedEvent() -= Core::MakeDelegate(this, &Window::OnScreenResized);
		}
	}
}