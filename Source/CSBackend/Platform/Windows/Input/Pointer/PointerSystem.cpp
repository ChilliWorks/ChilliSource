//
//  PointerSystem.cpp
//  Chilli Source
//  Created by Ian Copland on 27/03/2014.
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

#ifdef CS_TARGETPLATFORM_WINDOWS

#include <CSBackend/Platform/Windows/Input/Pointer/PointerSystem.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/Screen.h>
#include <ChilliSource/Core/Delegate/MakeDelegate.h>

namespace CSBackend
{
	namespace Windows
	{
		namespace
		{
			//------------------------------------------------
			/// @author S Downie
			///
			/// @param The SFML button ID.
			///
			/// @return The equivelent Press Type for the
			/// button Id
			//------------------------------------------------
			ChilliSource::Pointer::InputType ButtonIdToInputType(sf::Mouse::Button in_button)
			{
				switch (in_button)
				{
				case sf::Mouse::Button::Left:
					return ChilliSource::Pointer::InputType::k_leftMouseButton;
				case sf::Mouse::Button::Middle:
					return ChilliSource::Pointer::InputType::k_middleMouseButton;
				case sf::Mouse::Button::Right:
					return ChilliSource::Pointer::InputType::k_rightMouseButton;
				default:
					return ChilliSource::Pointer::InputType::k_none;
				}

			}
		}
		CS_DEFINE_NAMEDTYPE(PointerSystem);
		//----------------------------------------------------
		//----------------------------------------------------
		bool PointerSystem::IsA(ChilliSource::InterfaceIDType in_interfaceId) const
		{
			return (ChilliSource::PointerSystem::InterfaceID == in_interfaceId || PointerSystem::InterfaceID == in_interfaceId);
		}
		//------------------------------------------------
		//------------------------------------------------
		void PointerSystem::OnInit()
		{
			m_screen = ChilliSource::Application::Get()->GetSystem<ChilliSource::Screen>();
			CS_ASSERT(m_screen != nullptr, "Cannot find system required by PointerSystem: Screen.");

			m_mouseButtonConnection = SFMLWindow::Get()->GetMouseButtonEvent().OpenConnection(ChilliSource::MakeDelegate(this, &PointerSystem::OnMouseButtonEvent));
			m_mouseMovedConnection = SFMLWindow::Get()->GetMouseMovedEvent().OpenConnection(ChilliSource::MakeDelegate(this, &PointerSystem::OnMouseMoved));
			m_mouseWheelConnection = SFMLWindow::Get()->GetMouseWheelEvent().OpenConnection(ChilliSource::MakeDelegate(this, &PointerSystem::OnMouseWheeled));

			//create the mouse pointer
			ChilliSource::Integer2 mousePosi = SFMLWindow::Get()->GetMousePosition();
			ChilliSource::Vector2 mousePos((f32)mousePosi.x, m_screen->GetResolution().y - (f32)mousePosi.y);

			m_pointerId = AddPointerCreateEvent(mousePos);
		}
		//----------------------------------------------------
		//----------------------------------------------------
		void PointerSystem::HideCursor()
		{
			SFMLWindow::Get()->HideCursor();
		}
		//----------------------------------------------------
		//----------------------------------------------------
		void PointerSystem::ShowCursor()
		{
			SFMLWindow::Get()->ShowCursor();
		}
		//----------------------------------------------
		//----------------------------------------------
		void PointerSystem::OnMouseButtonEvent(sf::Mouse::Button in_button, SFMLWindow::MouseButtonEvent in_event, s32 in_xPos, s32 in_yPos)
		{
			ChilliSource::Pointer::InputType type = ButtonIdToInputType(in_button);
			if (type == ChilliSource::Pointer::InputType::k_none)
			{
				return;
			}

			switch (in_event)
			{
			case SFMLWindow::MouseButtonEvent::k_pressed:
				AddPointerDownEvent(m_pointerId, type);
				break;
			case SFMLWindow::MouseButtonEvent::k_released:
				AddPointerUpEvent(m_pointerId, type);
				break;
			}
		}
		//----------------------------------------------
		//----------------------------------------------
		void PointerSystem::OnMouseMoved(s32 in_xPos, s32 in_yPos)
		{
			ChilliSource::Vector2 touchLocation((f32)in_xPos, m_screen->GetResolution().y - (f32)in_yPos);
			AddPointerMovedEvent(m_pointerId, touchLocation);
		}
		//----------------------------------------------
		//----------------------------------------------
		void PointerSystem::OnMouseWheeled(s32 in_delta)
		{
			ChilliSource::Vector2 delta(0.0f, (f32)in_delta);
			AddPointerScrollEvent(m_pointerId, delta);
		}
		//------------------------------------------------
		//------------------------------------------------
		void PointerSystem::OnDestroy()
		{
			AddPointerRemoveEvent(m_pointerId);

			m_mouseButtonConnection = nullptr;
			m_mouseMovedConnection = nullptr;

			m_screen = nullptr;
		}
	}
}

#endif
