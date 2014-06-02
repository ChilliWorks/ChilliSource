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

#include <ChilliSource/Backend/Platform/Windows/Input/Pointer/PointerSystem.h>

#include <ChilliSource/Backend/Platform/Windows/GLFW/Base/GLFWManager.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/Screen.h>

//This needs to be included after windows.h
#include <Platform/Windows/glfw3.h>

namespace ChilliSource
{
	namespace Windows
	{
		namespace
		{
			PointerSystem* g_pointerSystem = nullptr;

			//------------------------------------------------
			/// @author I Copland
			///
			/// @return The current mouse position.
			//------------------------------------------------
			Core::Vector2 GetMousePosition()
			{
				f64 x = 0.0;
				f64 y = 0.0;

				GLFWManager::Get()->GetCursorPos(&x, &y);

				return Core::Vector2((f32)x, (f32)y);
			}
			//------------------------------------------------
			/// @author I Copland
			///
			/// @param The button ID.
			///
			/// @return The equivelent Press Type for the
			/// button Id
			//------------------------------------------------
			PointerSystem::InputType ButtonIdToInputType(s32 in_buttonId)
			{
				switch (in_buttonId)
				{
				case GLFW_MOUSE_BUTTON_LEFT:
					return PointerSystem::InputType::k_leftMouseButton;
				case GLFW_MOUSE_BUTTON_MIDDLE:
					return PointerSystem::InputType::k_middleMouseButton;
				case GLFW_MOUSE_BUTTON_RIGHT:
					return PointerSystem::InputType::k_rightMouseButton;
				default:
					return PointerSystem::InputType::k_none;
				}
				
			}
		}
		CS_DEFINE_NAMEDTYPE(PointerSystem);
		//----------------------------------------------------
		//----------------------------------------------------
		bool PointerSystem::IsA(Core::InterfaceIDType in_interfaceId) const
		{
			return (Input::PointerSystem::InterfaceID == in_interfaceId || PointerSystem::InterfaceID == in_interfaceId);
		}
		//----------------------------------------------
		//----------------------------------------------
		void PointerSystem::OnMouseMoved(GLFWwindow* in_window, f64 in_xPos, f64 in_yPos)
		{
			CS_ASSERT(g_pointerSystem, "OnMouseMoved callback requires a pointer system.");

			Core::Vector2 touchLocation((f32)in_xPos, g_pointerSystem->m_screen->GetResolution().y - ((f32)in_yPos));
			g_pointerSystem->AddPointerMovedEvent(g_pointerSystem->m_pointerId, touchLocation);
		}
		//----------------------------------------------
		//----------------------------------------------
		void PointerSystem::OnMouseButtonPressed(GLFWwindow* in_window, s32 in_buttonID, s32 in_buttonAction, s32 in_modifierKeys)
		{
			CS_ASSERT(g_pointerSystem, "On Mouse Button Pressed callback requires a pointer system.");

			switch (GLFWManager::MouseButtonAction(in_buttonAction))
			{
				case GLFWManager::MouseButtonAction::k_press:
				{
					InputType type = ButtonIdToInputType(in_buttonID);
					if (type != InputType::k_none)
					{
						g_pointerSystem->AddPointerDownEvent(g_pointerSystem->m_pointerId, type);
					}
					break;
				}
				case GLFWManager::MouseButtonAction::k_release:
				{
					InputType type = ButtonIdToInputType(in_buttonID);
					if (type != InputType::k_none)
					{
						g_pointerSystem->AddPointerUpEvent(g_pointerSystem->m_pointerId, type);
					}
					break;
				}
				default:
				{
					break;
				}
			};
		}
		//------------------------------------------------
		//------------------------------------------------
		void PointerSystem::OnInit()
		{
			m_screen = Core::Application::Get()->GetSystem<Core::Screen>();
			CS_ASSERT(m_screen != nullptr, "Cannot find system required by PointerSystem: Screen.");
			
			g_pointerSystem = this;

			//Register for glfw mouse callbacks
			GLFWManager::Get()->SetCursorPosDelegate(&PointerSystem::OnMouseMoved);
			GLFWManager::Get()->SetMouseButtonDelegate(&PointerSystem::OnMouseButtonPressed);

			//create the mouse pointer
			Core::Vector2 mousePos = GetMousePosition();
			mousePos.y = m_screen->GetResolution().y - mousePos.y;

			m_pointerId = AddPointerCreateEvent(mousePos);
		}
		//------------------------------------------------
		//------------------------------------------------
		void PointerSystem::OnDestroy()
		{
			AddPointerRemoveEvent(m_pointerId);

			GLFWManager::Get()->SetCursorPosDelegate(nullptr);
			GLFWManager::Get()->SetMouseButtonDelegate(nullptr);

			g_pointerSystem = nullptr;

			m_screen = nullptr;
		}
	}
}

#endif
