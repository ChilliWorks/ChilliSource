//
//  GUIWindow.cpp
//  moFlo
//
//  Created by Scott Downie on 21/04/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#include <ChilliSource/GUI/Base/Window.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/ApplicationEvents.h>
#include <ChilliSource/Core/Base/MakeDelegate.h>
#include <ChilliSource/Core/Base/Screen.h>

#include <ChilliSource/Input/Pointer/PointerSystem.h>

#ifdef CS_ENABLE_DEBUGSTATS
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
            
            m_pointerSystem = Core::Application::Get()->GetSystem<Input::PointerSystem>();
            
			Core::Vector2 vAbsSize = GetScreen()->GetResolution();
            
			//The window is fullscreen and centred
			SetSize(Core::UnifiedVector2(Core::Vector2(1.0f, 1.0f), vAbsSize));
			SetPosition(Core::UnifiedVector2(Core::Vector2::ZERO, vAbsSize * 0.5f));
			SetName("RootWindow");
            
			//Register for screen rotation events
			m_screenResizedConnection = GetScreen()->GetResolutionChangedEvent().OpenConnection(Core::MakeDelegate(this, &Window::OnScreenResolutionChanged));
		}
        void Window::StartListeningForPointerInput()
        {
            if(m_pointerSystem && !mbListeningForTouches)
			{
				m_pointerDownConnection = m_pointerSystem->GetPointerDownEvent().OpenConnection(Core::MakeDelegate(this, &Window::_OnPointerDown));
				m_pointerMovedConnection = m_pointerSystem->GetPointerMovedEvent().OpenConnection(Core::MakeDelegate(this, &Window::_OnPointerMoved));
				m_pointerUpConnection = m_pointerSystem->GetPointerUpEvent().OpenConnection(Core::MakeDelegate(this, &Window::_OnPointerUp));
                mbListeningForTouches=true;
			}
        }
        
        void Window::StopListeningForPointerInput()
        {
        	//send up event for all currently active pointers
        	for (const Input::PointerSystem::Pointer& pointer : m_pointerSystem->GetPointers())
        	{
        		for (const Input::PointerSystem::InputType& type : pointer.m_activeInput)
        		{
        			_OnPointerUp(pointer, ((f64)Core::Application::Get()->GetSystemTimeInMilliseconds()) / 1000.0, type);
        		}
        	}

            m_pointerDownConnection.reset();
            m_pointerMovedConnection.reset();
            m_pointerUpConnection.reset();
            mbListeningForTouches = false;
        }
		//-----------------------------------------------------------
		//-----------------------------------------------------------
		void Window::OnScreenResolutionChanged(const Core::Vector2& in_resolution)
		{
			Core::Vector2 vAbsSize((f32)in_resolution.x, (f32)in_resolution.y);
            
			//The window is fullscreen and centred
			SetSize(Core::UnifiedVector2(Core::Vector2(1.0f, 1.0f), vAbsSize));
			SetPosition(Core::UnifiedVector2(Core::Vector2(0.0f, 0.0f), vAbsSize * 0.5f));
            
			//Notify all subviews and they can decide what to do
			GUIView::OnScreenResolutionChanged();
		}
		//-----------------------------------------------------------
		//-----------------------------------------------------------
		void Window::_OnPointerDown(const Input::PointerSystem::Pointer& in_pointer, f64 in_timestamp, Input::PointerSystem::InputType in_inputType)
		{
			if (in_inputType == Input::PointerSystem::GetDefaultInputType())
            {
                if (!UserInteraction)
                    return;
                
                mSubviewsCopy = mSubviews;
                
                mInputEvents.OnPointerDown(this, in_pointer, true);
                
                //We need to notify any subviews they get first dibs
                for(GUIView::Subviews::reverse_iterator it = mSubviewsCopy.rbegin(); it != mSubviewsCopy.rend(); ++it)
                {
                    if(((*it)->IsAcceptTouchesOutsideOfBoundsEnabled() || (*it)->Contains(in_pointer.m_location)))
                    {
                        if((*it)->OnPointerDown(in_pointer, in_timestamp, in_inputType))
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
                m_pointerDownEvent.NotifyConnections(in_pointer, in_timestamp, in_inputType);
            }
		}
		//-----------------------------------------------------------
		//-----------------------------------------------------------
		void Window::_OnPointerMoved(const Input::PointerSystem::Pointer& in_pointer, f64 in_timestamp)
		{
            if (in_pointer.m_activeInput.find(Input::PointerSystem::GetDefaultInputType()) != in_pointer.m_activeInput.end())
            {
                if (!UserInteraction)
                    return;
                
                mSubviewsCopy = mSubviews;
                
                mInputEvents.OnPointerMoved(this, in_pointer);
                
                //We need to notify any subviews they get first dibs
                for(GUIView::Subviews::reverse_iterator it = mSubviewsCopy.rbegin(); it != mSubviewsCopy.rend(); ++it)
                {
                    if((*it)->OnPointerMoved(in_pointer, in_timestamp))
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
                m_pointerMovedEvent.NotifyConnections(in_pointer, in_timestamp);
            }
		}
		//-----------------------------------------------------------
		//-----------------------------------------------------------
		void Window::_OnPointerUp(const Input::PointerSystem::Pointer& in_pointer, f64 in_timestamp, Input::PointerSystem::InputType in_inputType)
		{
            if (in_inputType == Input::PointerSystem::GetDefaultInputType())
            {
                if (!UserInteraction)
                    return;
                
                mSubviewsCopy = mSubviews;
                
                mInputEvents.OnPointerUp(this, in_pointer);
                
                //We need to notify any subviews they get first dibs
                for(GUIView::Subviews::reverse_iterator it = mSubviewsCopy.rbegin(); it != mSubviewsCopy.rend(); ++it)
                {
                    (*it)->OnPointerUp(in_pointer, in_timestamp, in_inputType);
                }
                
                //If the touch has not been consumed we then notify
                //the outside world
                mSubviewsCopy.clear();
                m_pointerUpEvent.NotifyConnections(in_pointer, in_timestamp, in_inputType);
            }
		}
        //-----------------------------------------------------------
        //-----------------------------------------------------------
        bool Window::OnPointerDown(const Input::PointerSystem::Pointer& in_pointer, f64 in_timestamp, Input::PointerSystem::InputType in_inputType)
        {
            return false;
        }
        //-----------------------------------------------------------
        //-----------------------------------------------------------
        bool Window::OnPointerMoved(const Input::PointerSystem::Pointer& in_pointer, f64 in_timestamp)
        {
            return false;
        }
        //-----------------------------------------------------------
        //-----------------------------------------------------------
        void Window::OnPointerUp(const Input::PointerSystem::Pointer& in_pointer, f64 in_timestamp, Input::PointerSystem::InputType in_inputType)
        {
        }
#ifdef CS_ENABLE_DEBUGSTATS
        //-----------------------------------------------------
        /// Draw
        ///
        /// Override this implementation to draw the GUI
        /// using the primitves within canvas renderer i.e.
        /// DrawBox, DrawString etc
        ///
        /// @param Canvas renderer
        //-----------------------------------------------------
        void Window::Draw(Rendering::CanvasRenderer * inpCanvas)
        {
            GUIView::Draw(inpCanvas);
            
            if(Debugging::DebugStats::IsEnabled())
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
			StopListeningForPointerInput();
		}
	}
}
