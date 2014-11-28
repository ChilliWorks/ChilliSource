//
//  Canvas.cpp
//  Chilli Source
//  Created by Scott Downie on 21/04/2014.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2014 Tag Games Limited
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

#include <ChilliSource/UI/Base/Canvas.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/Screen.h>
#include <ChilliSource/Core/Delegate/MakeDelegate.h>
#include <ChilliSource/Core/Event/IConnectableEvent.h>
#include <ChilliSource/Input/Pointer/PointerSystem.h>
#include <ChilliSource/UI/Base/WidgetFactory.h>

namespace ChilliSource
{
	namespace UI
	{
        CS_DEFINE_NAMEDTYPE(Canvas);
        //----------------------------------------------------
        //----------------------------------------------------
        CanvasUPtr Canvas::Create()
        {
            return CanvasUPtr(new Canvas());
        }
        //----------------------------------------------------
        //----------------------------------------------------
        bool Canvas::IsA(Core::InterfaceIDType in_interfaceId) const
        {
            return in_interfaceId == Canvas::InterfaceID;
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        void Canvas::OnInit()
        {
            m_screen = Core::Application::Get()->GetSystem<Core::Screen>();
            CS_ASSERT(m_screen != nullptr, "Canvas must have access to screen");
            
            WidgetFactory* widgetFactory = Core::Application::Get()->GetWidgetFactory();
            
            m_canvas = widgetFactory->CreateWidget();
            m_canvas->SetName("Canvas");
            m_canvas->SetAbsoluteSize(GetSize());
            m_canvas->SetCanvas(m_canvas.get());
            m_canvas->SetAbsolutePosition(GetSize() * 0.5f);
            m_canvas->SetInputEnabled(true);
            m_canvas->SetInputConsumeEnabled(false);
            
            m_screenResizedConnection = m_screen->GetResolutionChangedEvent().OpenConnection(Core::MakeDelegate(this, &Canvas::OnScreenResolutionChanged));
            
            auto pointerSystem = Core::Application::Get()->GetSystem<Input::PointerSystem>();
            m_pointerDownConnection = pointerSystem->GetPointerDownEventInternal().OpenConnection(Core::MakeDelegate(this, &Canvas::OnPointerDown));
            m_pointerMovedConnection = pointerSystem->GetPointerMovedEvent().OpenConnection(Core::MakeDelegate(this, &Canvas::OnPointerMoved));
            m_pointerUpConnection = pointerSystem->GetPointerUpEvent().OpenConnection(Core::MakeDelegate(this, &Canvas::OnPointerUp));
        }
        //-----------------------------------------------------------
        /// UI can filter input events to prevent them from being
        /// forwarded to the external app.
        //-----------------------------------------------------------
        void Canvas::OnPointerDown(const Input::Pointer& in_pointer, f64 in_timestamp, Input::Pointer::InputType in_inputType, Input::Filter& in_filter)
        {
            m_canvas->OnPointerDown(in_pointer, in_timestamp, in_inputType, in_filter);
        }
        //-----------------------------------------------------------
        //-----------------------------------------------------------
        void Canvas::OnPointerMoved(const Input::Pointer& in_pointer, f64 in_timestamp)
        {
            m_canvas->OnPointerMoved(in_pointer, in_timestamp);
        }
        //-----------------------------------------------------------
        //-----------------------------------------------------------
        void Canvas::OnPointerUp(const Input::Pointer& in_pointer, f64 in_timestamp, Input::Pointer::InputType in_inputType)
        {
            m_canvas->OnPointerUp(in_pointer, in_timestamp, in_inputType);
        }
        //--------------------------------------------------------
        //--------------------------------------------------------
        void Canvas::OnScreenResolutionChanged(const Core::Vector2& in_resolution)
        {
			m_canvas->SetAbsoluteSize(in_resolution);
			m_canvas->SetAbsolutePosition(in_resolution * 0.5f);
        }
        //----------------------------------------------------
        //----------------------------------------------------
        const Core::Vector2& Canvas::GetSize() const
        {
            return m_screen->GetResolution();
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        void Canvas::OnResume()
        {
            m_canvas->OnResume();
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        void Canvas::OnForeground()
        {
            m_canvas->OnForeground();
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        void Canvas::OnUpdate(f32 in_timeSinceLastUpdate)
        {
            m_canvas->OnUpdate(in_timeSinceLastUpdate);
        }
        //----------------------------------------------------
        //----------------------------------------------------
        void Canvas::Draw(Rendering::CanvasRenderer* in_renderer) const
        {
            m_canvas->OnDraw(in_renderer);
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        void Canvas::OnBackground()
        {
            m_canvas->OnBackground();
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        void Canvas::OnSuspend()
        {
            m_canvas->OnSuspend();
        }
        //----------------------------------------------------
        //----------------------------------------------------
        void Canvas::AddWidget(const WidgetSPtr& in_widget)
        {
            m_canvas->AddWidget(in_widget);
        }
        //----------------------------------------------------
        //----------------------------------------------------
        void Canvas::RemoveWidget(Widget* in_widget)
        {
            m_canvas->RemoveWidget(in_widget);
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        void Canvas::OnDestroy()
        {
            m_canvas.reset();
            m_screenResizedConnection.reset();
            m_pointerDownConnection.reset();
            m_pointerMovedConnection.reset();
            m_pointerUpConnection.reset();
        }
	}
}
