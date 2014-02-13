
#include <ChilliSource/Platform/Windows/Input/MouseWindows.h>

#include <ChilliSource/Platform/Windows/OpenGL/glfw.h>

#include <ChilliSource/Input/TouchScreen.h>

#include <ChilliSource/Core/Screen.h>

namespace moFlo
{
	namespace WindowsPlatform
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
		CMouse::CMouse(Input::ITouchScreen* inpTouchProxy) : IMouse(inpTouchProxy), mudwCurrentTouchID(0)
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
			return inInterfaceID == Input::IMouse::InterfaceID;
		}
		//------------------------------------------------------
		/// Get Position
		/// 
		/// @return Position of cursor on screen
		//------------------------------------------------------
		moFlo::Core::CVector2 CMouse::GetPosition() const
		{
			s32 dwX, dwY = 0;
			glfwGetMousePos(&dwX, &dwY);

			dwY = Core::CScreen::GetOrientedHeight() - dwY; 
			return moFlo::Core::CVector2((f32)dwX, (f32)dwY);
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
				gpMouseInstance->mOnMouseMovedEvent.Invoke(gpMouseInstance);
				
				//We may want to fake touch input
				if(gpMouseInstance->mpTouchProxy && gpMouseInstance->mbaButtonsDown[Input::MOUSE_LEFT_BUTTON])
				{
					gpMouseInstance->mpTouchProxy->MoveTouch(gpMouseInstance->mudwCurrentTouchID, Core::CVector2((f32)indwPosX, (f32)indwPosY), gpMouseInstance->mpTouchProxy->GetLastTimeStamp());
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
					gpMouseInstance->mOnMousePressedEvent.Invoke(gpMouseInstance);
					
					//We may want to fake touch input
					if(gpMouseInstance->mpTouchProxy)
					{
						s32 dwXPos, dwYPos = 0;
						glfwGetMousePos(&dwXPos, &dwYPos);
						gpMouseInstance->mudwCurrentTouchID = gpMouseInstance->mpTouchProxy->StartTouch(Core::CVector2((f32)dwXPos, (f32)dwYPos), gpMouseInstance->mpTouchProxy->GetLastTimeStamp());
					}
					break;
				case GLFW_RELEASE:
					gpMouseInstance->mbaButtonsDown[indwButtonID] = false;
					gpMouseInstance->mOnMouseReleasedEvent.Invoke(gpMouseInstance);

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
