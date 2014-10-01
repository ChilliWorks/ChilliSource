//
//  Window.cpp
//  Chilli Source
//  Created by Scott Downie on 21/04/2011.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2011 Tag Games Limited
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//

#include <ChilliSource/GUI/Base/Window.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Delegate/MakeDelegate.h>
#include <ChilliSource/Core/Base/Screen.h>

#include <ChilliSource/Input/Pointer/PointerSystem.h>

#ifdef CS_ENABLE_DEBUGSTATS
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
			SetPosition(Core::UnifiedVector2(Core::Vector2::k_zero, vAbsSize * 0.5f));
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
        	for (const Input::Pointer& pointer : m_pointerSystem->GetPointers())
        	{
        		for (const Input::Pointer::InputType& type : pointer.GetActiveInputs())
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
		void Window::_OnPointerDown(const Input::Pointer& in_pointer, f64 in_timestamp, Input::Pointer::InputType in_inputType)
		{
			if (in_inputType == Input::Pointer::GetDefaultInputType())
            {
                if (!UserInteraction)
                    return;
                
                mSubviewsCopy = mSubviews;
                
                mInputEvents.OnPointerDown(this, in_pointer, true);
                
                //We need to notify any subviews they get first dibs
                for(GUIView::Subviews::reverse_iterator it = mSubviewsCopy.rbegin(); it != mSubviewsCopy.rend(); ++it)
                {
                    if(((*it)->IsAcceptTouchesOutsideOfBoundsEnabled() || (*it)->Contains(in_pointer.GetPosition())))
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
		void Window::_OnPointerMoved(const Input::Pointer& in_pointer, f64 in_timestamp)
		{
            if (in_pointer.IsInputDown(Input::Pointer::GetDefaultInputType()) == true)
            {
                if (!UserInteraction)
                    return;
                
                mSubviewsCopy = mSubviews;
                
                mInputEvents.OnPointerMoved(this, in_pointer);
                
                //We need to notify any subviews they get first dibs
                for(GUIView::Subviews::reverse_iterator it = mSubviewsCopy.rbegin(); it != mSubviewsCopy.rend(); ++it)
                {
                    (*it)->OnPointerMoved(in_pointer, in_timestamp);
                }
                
                mSubviewsCopy.clear();
                m_pointerMovedEvent.NotifyConnections(in_pointer, in_timestamp);
            }
		}
		//-----------------------------------------------------------
		//-----------------------------------------------------------
		void Window::_OnPointerUp(const Input::Pointer& in_pointer, f64 in_timestamp, Input::Pointer::InputType in_inputType)
		{
            if (in_inputType == Input::Pointer::GetDefaultInputType())
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
                
                mSubviewsCopy.clear();
                m_pointerUpEvent.NotifyConnections(in_pointer, in_timestamp, in_inputType);
            }
		}
        //-----------------------------------------------------------
        //-----------------------------------------------------------
        bool Window::OnPointerDown(const Input::Pointer& in_pointer, f64 in_timestamp, Input::Pointer::InputType in_inputType)
        {
            return false;
        }
        //-----------------------------------------------------------
        //-----------------------------------------------------------
        void Window::OnPointerMoved(const Input::Pointer& in_pointer, f64 in_timestamp)
        {
        }
        //-----------------------------------------------------------
        //-----------------------------------------------------------
        void Window::OnPointerUp(const Input::Pointer& in_pointer, f64 in_timestamp, Input::Pointer::InputType in_inputType)
        {
        }
#ifdef CS_ENABLE_DEBUGSTATS
        //-----------------------------------------------------
        //-----------------------------------------------------
        void Window::Draw(Rendering::CanvasRenderer * inpCanvas)
        {
            GUIView::Draw(inpCanvas);
            Core::Application::Get()->GetDebugStats()->DrawStats(inpCanvas, this);
        }
#endif
		//-----------------------------------------------
		//-----------------------------------------------
		Window::~Window()
		{
			//Deregister for touch delegates
			//The window is responsible for receiving input for this scene
			StopListeningForPointerInput();
		}
	}
}
