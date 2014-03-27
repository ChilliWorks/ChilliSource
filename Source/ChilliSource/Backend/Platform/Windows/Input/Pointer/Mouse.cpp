//
//  Mouse.cpp
//  Chilli Source
//
//  Created by Scott Downie on 24/11/2011.
//  Copyright (c) 2011 Tag Games Ltd. All rights reserved.
//

#include <ChilliSource/Backend/Platform/Windows/Input/Pointer/Mouse.h>

#include <ChilliSource/Backend/Platform/Windows/GLFW/Base/GLFWManager.h>
#include <ChilliSource/Core/Base/Screen.h>
#include <ChilliSource/Input/Pointer/TouchScreen.h>

namespace ChilliSource
{
	namespace Windows
	{
		Mouse* gpMouseInstance = nullptr;

		//----------------------------------------------------
		//----------------------------------------------------
		Mouse::Mouse()
		: m_currentTouchID(0)
		{
			gpMouseInstance = this;

			//Register for glfw mouse callbacks
			GLFWManager::Get()->SetCursorPosDelegate(&Mouse::OnMouseMoved);
			GLFWManager::Get()->SetMouseButtonDelegate(&Mouse::OnMouseButtonPressed);
		}
		//------------------------------------------------------
		//------------------------------------------------------
		Mouse::Mouse(Input::TouchScreen* in_touchProxy)
		: Input::Mouse(in_touchProxy)
		, m_currentTouchID(0)
		{
			gpMouseInstance = this;

			//Register for glfw mouse callbacks
			GLFWManager::Get()->SetCursorPosDelegate(&Mouse::OnMouseMoved);
			GLFWManager::Get()->SetMouseButtonDelegate(&Mouse::OnMouseButtonPressed);
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		bool Mouse::IsA(Core::InterfaceIDType in_interfaceID) const
		{
			return in_interfaceID == Input::Mouse::InterfaceID;
		}
		//------------------------------------------------------
		//------------------------------------------------------
		Core::Vector2 Mouse::GetPosition() const
		{
			f64 xPos, yPos = 0.0;
			GLFWManager::Get()->GetCursorPos(&xPos, &yPos);

			yPos = Core::Screen::GetOrientedHeight() - yPos;
			return Core::Vector2((f32)xPos, (f32)yPos);
		}
		//----------------------------------------------
		//----------------------------------------------
		void Mouse::OnMouseMoved(GLFWwindow* in_window, f64 in_xPos, f64 in_yPos)
		{
			if(gpMouseInstance)
			{
				gpMouseInstance->mOnMouseMovedEvent.NotifyConnections(gpMouseInstance);
				
				//We may want to fake touch input
				if(gpMouseInstance->mpTouchProxy && gpMouseInstance->mbaButtonsDown[(s32)Input::MouseInputType::k_leftButton])
				{
					gpMouseInstance->mpTouchProxy->MoveTouch(gpMouseInstance->m_currentTouchID, Core::Vector2((f32)in_xPos, (f32)in_yPos), gpMouseInstance->mpTouchProxy->GetLastTimeStamp());
				}
			}
		}
		//----------------------------------------------
		//----------------------------------------------
		void Mouse::OnMouseButtonPressed(GLFWwindow* in_window, s32 in_buttonID, s32 in_buttonAction, s32 in_modifierKeys)
		{
			if(gpMouseInstance)
			{
				switch (GLFWManager::MouseButtonAction(in_buttonAction))
				{
				case GLFWManager::MouseButtonAction::k_press :
					gpMouseInstance->mbaButtonsDown[in_buttonID] = true;
					gpMouseInstance->mOnMousePressedEvent.NotifyConnections(gpMouseInstance);
					
					//We may want to fake touch input
					if(gpMouseInstance->mpTouchProxy)
					{
						f64 xPos, yPos = 0.0;
						GLFWManager::Get()->GetCursorPos(&xPos, &yPos);
						gpMouseInstance->m_currentTouchID = gpMouseInstance->mpTouchProxy->StartTouch(Core::Vector2((f32)xPos, (f32)yPos), gpMouseInstance->mpTouchProxy->GetLastTimeStamp());
					}
					break;
				case GLFWManager::MouseButtonAction::k_release:
					gpMouseInstance->mbaButtonsDown[in_buttonID] = false;
					gpMouseInstance->mOnMouseReleasedEvent.NotifyConnections(gpMouseInstance);

					//We may want to fake touch input
					if(gpMouseInstance->mpTouchProxy)
					{
						gpMouseInstance->mpTouchProxy->EndTouch(gpMouseInstance->m_currentTouchID, gpMouseInstance->mpTouchProxy->GetLastTimeStamp());
					}
					break;
				default:
					break;
				};
			}
		}
		//----------------------------------------------------
		//----------------------------------------------------
		Mouse::~Mouse()
		{
			//Remove as listener
			GLFWManager::Get()->SetCursorPosDelegate(nullptr);
			GLFWManager::Get()->SetMouseButtonDelegate(nullptr);

			if(gpMouseInstance ==  this)
			{
				gpMouseInstance = nullptr;
			}
		}
	}
}
