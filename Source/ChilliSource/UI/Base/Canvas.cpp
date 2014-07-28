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
            
			m_canvas = WidgetUPtr(new Widget(WidgetDesc::DefaultPropertiesDesc(), std::vector<WidgetDesc::CustomPropertyDesc>()));
            m_canvas->SetName("Canvas");
            m_canvas->SetAbsoluteSize(GetSize());
            m_canvas->SetCanvas(m_canvas.get());
            m_canvas->SetAbsolutePosition(GetSize() * 0.5f);
            
            m_screenResizedConnection = m_screen->GetResolutionChangedEvent().OpenConnection(Core::MakeDelegate(this, &Canvas::OnScreenResolutionChanged));
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
        //----------------------------------------------------
        //----------------------------------------------------
        void Canvas::Draw(Rendering::CanvasRenderer* in_renderer) const
        {
            m_canvas->Draw(in_renderer);
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
        }
	}
}
