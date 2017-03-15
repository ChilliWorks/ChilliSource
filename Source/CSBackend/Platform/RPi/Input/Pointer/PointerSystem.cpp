//
//  The MIT License (MIT)
//
//  Copyright (c) 2017 Tag Games Limited
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

#ifdef CS_TARGETPLATFORM_RPI

#include <CSBackend/Platform/RPi/Input/Pointer/PointerSystem.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>
#include <ChilliSource/Core/Delegate/MakeDelegate.h>

namespace CSBackend
{
	namespace RPi
	{
		namespace
		{
			/// @param button
			///		The X11 button ID.
			///
			/// @return The equivelent Press Type for the button Id
			///
			ChilliSource::Pointer::InputType ButtonIdToInputType(u32 button)
			{
				switch (button)
				{
				case 1:
					return ChilliSource::Pointer::InputType::k_leftMouseButton;
				case 2:
					return ChilliSource::Pointer::InputType::k_middleMouseButton;
				case 3:
					return ChilliSource::Pointer::InputType::k_rightMouseButton;
				default:
					return ChilliSource::Pointer::InputType::k_none;
				}

				return ChilliSource::Pointer::InputType::k_none;
			}
		}

		CS_DEFINE_NAMEDTYPE(PointerSystem);

		//---------------------------------------------------------------------------------
		bool PointerSystem::IsA(ChilliSource::InterfaceIDType interfaceId) const noexcept
		{
			return (ChilliSource::PointerSystem::InterfaceID == interfaceId || PointerSystem::InterfaceID == interfaceId);
		}

		//---------------------------------------------------------------------------------
		void PointerSystem::OnInit() noexcept
		{
			m_screen = ChilliSource::Application::Get()->GetSystem<ChilliSource::Screen>();
			CS_ASSERT(m_screen != nullptr, "Cannot find system required by PointerSystem: Screen.");

			auto screenResolution = m_screen->GetResolution();

			//TODO:check for mouse and only create when one is found.

			DispmanWindow::Get()->SetMouseDelegates(ChilliSource::MakeDelegate(this, &PointerSystem::OnMouseButtonEvent), ChilliSource::MakeDelegate(this, &PointerSystem::OnMouseMoved));

			//create the mouse pointer
			ChilliSource::Integer2 mousePosi = DispmanWindow::Get()->GetMousePosition();
			ChilliSource::Vector2 mousePos((f32)mousePosi.x, screenResolution.y - (f32)mousePosi.y);

			m_pointerId = AddPointerCreateEvent(mousePos);
		}

		//---------------------------------------------------------------------------------
		void PointerSystem::OnMouseButtonEvent(u32 button, DispmanWindow::MouseButtonEvent event) noexcept
		{
			//In X11 the scroll wheel acts like a button getting pressed events for each tick
			switch(button)
			{
				case 4:
					AddPointerScrollEvent(m_pointerId, ChilliSource::Vector2(0.0f, 1.0f));
					return;
				case 5:
					AddPointerScrollEvent(m_pointerId, ChilliSource::Vector2(0.0f, -1.0f));
					return;
			}

			//If the button isn't one the scroll buttons then we are into regular button logic
			ChilliSource::Pointer::InputType type = ButtonIdToInputType(button);
			if (type == ChilliSource::Pointer::InputType::k_none)
			{
				CS_LOG_WARNING_FMT("Unknown X11 mouse button: %d\n", button);
				return;
			}

			switch (event)
			{
			case DispmanWindow::MouseButtonEvent::k_pressed:
				AddPointerDownEvent(m_pointerId, type);
				break;
			case DispmanWindow::MouseButtonEvent::k_released:
				AddPointerUpEvent(m_pointerId, type);
				break;
			}
		}

		//---------------------------------------------------------------------------------
		void PointerSystem::OnMouseMoved(s32 xPos, s32 yPos) noexcept
		{
			ChilliSource::Vector2 touchLocation((f32)xPos, m_screen->GetResolution().y - (f32)yPos);
			AddPointerMovedEvent(m_pointerId, touchLocation);
		}

		//---------------------------------------------------------------------------------
		void PointerSystem::OnDestroy() noexcept
		{
			AddPointerRemoveEvent(m_pointerId);

			DispmanWindow::Get()->RemoveMouseDelegates();

			m_screen = nullptr;
		}
	}
}

#endif
