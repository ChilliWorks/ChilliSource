
#include <ChilliSource/Backend/Platform/Windows/Input/Pointer/Mouse.h>

#include <ChilliSource/Core/Base/Screen.h>
#include <ChilliSource/Input/Pointer/TouchScreen.h>

#include <Platform/Windows/glfw.h>

namespace ChilliSource
{
	namespace Windows
	{
		CMouse* gpMouseInstance = NULL;
		//----------------------------------------------------
		/// Constructor
		///
		/// Default
		//----------------------------------------------------
		CMouse::CMouse() : mudwCurrentTouchID(0)
		{
			//Register for glfw mouse callbacks
			glfwSetMousePosCallback((GLFWmouseposfun)&CMouse::OnMouseMoved);
			glfwSetMouseButtonCallback((GLFWmousebuttonfun)&CMouse::OnMouseButtonPressed);

			gpMouseInstance = this;
		}
		//------------------------------------------------------
		/// Constructor
		///
		/// Takes a touch screen proxy to fake touch input
		///
		/// @param Touch screen proxy
		//------------------------------------------------------
		CMouse::CMouse(Input::TouchScreen* inpTouchProxy) : Mouse(inpTouchProxy), mudwCurrentTouchID(0)
		{
			//Register for glfw mouse callbacks
			glfwSetMousePosCallback((GLFWmouseposfun)&CMouse::OnMouseMoved);
			glfwSetMouseButtonCallback((GLFWmousebuttonfun)&CMouse::OnMouseButtonPressed);

			gpMouseInstance = this;
		}
		//-------------------------------------------------------
		/// Is A
		//-------------------------------------------------------
		bool CMouse::IsA(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == Input::Mouse::InterfaceID;
		}
		//------------------------------------------------------
		/// Get Position
		/// 
		/// @return Position of cursor on screen
		//------------------------------------------------------
		ChilliSource::Core::Vector2 CMouse::GetPosition() const
		{
			s32 dwX, dwY = 0;
			glfwGetMousePos(&dwX, &dwY);

			dwY = Core::Screen::GetOrientedHeight() - dwY; 
			return ChilliSource::Core::Vector2((f32)dwX, (f32)dwY);
		}
		//---GLFW Mouse Delegates
		//----------------------------------------------
		/// On Mouse Moved (GLFW)
		///
		/// @param Cursor X Pos
		/// @param Cursor Y Pos
		//----------------------------------------------
		void CMouse::OnMouseMoved(s32 indwPosX, s32 indwPosY)
		{
			if(gpMouseInstance)
			{
				gpMouseInstance->mOnMouseMovedEvent.NotifyConnections(gpMouseInstance);
				
				//We may want to fake touch input
				if(gpMouseInstance->mpTouchProxy && gpMouseInstance->mbaButtonsDown[(s32)Input::MouseInputType::k_leftButton])
				{
					gpMouseInstance->mpTouchProxy->MoveTouch(gpMouseInstance->mudwCurrentTouchID, Core::Vector2((f32)indwPosX, (f32)indwPosY), gpMouseInstance->mpTouchProxy->GetLastTimeStamp());
				}
			}
		}
		//----------------------------------------------
		/// On Mouse Button Pressed (GLFW)
		///
		/// @param Button ID
		/// @param Button status
		//----------------------------------------------
		void CMouse::OnMouseButtonPressed(s32 indwButtonID, s32 indwMouseButtonState)
		{
			if(gpMouseInstance)
			{
				switch(indwMouseButtonState)
				{
				case GLFW_PRESS:
					gpMouseInstance->mbaButtonsDown[indwButtonID] = true;
					gpMouseInstance->mOnMousePressedEvent.NotifyConnections(gpMouseInstance);
					
					//We may want to fake touch input
					if(gpMouseInstance->mpTouchProxy)
					{
						s32 dwXPos, dwYPos = 0;
						glfwGetMousePos(&dwXPos, &dwYPos);
						gpMouseInstance->mudwCurrentTouchID = gpMouseInstance->mpTouchProxy->StartTouch(Core::Vector2((f32)dwXPos, (f32)dwYPos), gpMouseInstance->mpTouchProxy->GetLastTimeStamp());
					}
					break;
				case GLFW_RELEASE:
					gpMouseInstance->mbaButtonsDown[indwButtonID] = false;
					gpMouseInstance->mOnMouseReleasedEvent.NotifyConnections(gpMouseInstance);

					//We may want to fake touch input
					if(gpMouseInstance->mpTouchProxy)
					{
						gpMouseInstance->mpTouchProxy->EndTouch(gpMouseInstance->mudwCurrentTouchID, gpMouseInstance->mpTouchProxy->GetLastTimeStamp());
					}
					break;
				default:
					break;
				};
			}
		}
		//----------------------------------------------------
		/// Destructor
		//----------------------------------------------------
		CMouse::~CMouse()
		{
			//Remove as listener
			glfwSetMousePosCallback(NULL);
			glfwSetMouseButtonCallback(NULL);

			if(gpMouseInstance ==  this)
			{
				gpMouseInstance = NULL;
			}
		}
	}
}
