
#ifndef _MO_FLO_WINDOWSPLATFORM_INPUT_MOUSE_H_
#define _MO_FLO_WINDOWSPLATFORM_INPUT_MOUSE_H_

#include <ChilliSource/Input/ForwardDeclarations.h>
#include <ChilliSource/Input/Mouse.h>

namespace moFlo
{
	namespace WindowsPlatform
	{
		class CMouse : public Input::IMouse
		{
		public:
			CMouse();
			CMouse(Input::ITouchScreen* inpTouchProxy);
			virtual ~CMouse();

			//-------------------------------------------------------
			/// Is A
			//-------------------------------------------------------
			bool IsA(Core::InterfaceIDType inInterfaceID) const;
			//------------------------------------------------------
			/// Get Position
			/// 
			/// @return Position of cursor on screen
			//------------------------------------------------------
			moFlo::Core::CVector2 GetPosition() const;

		private:

			//---GLFW Mouse Delegates
			//----------------------------------------------
			/// On Mouse Moved (GLFW)
			///
			/// @param Cursor X Pos
			/// @param Cursor Y Pos
			//----------------------------------------------
			static void OnMouseMoved(s32 indwPosX, s32 indwPosY);
			//----------------------------------------------
			/// On Mouse Button Pressed (GLFW)
			///
			/// @param Button ID
			/// @param Button status
			//----------------------------------------------
			static void OnMouseButtonPressed(s32 indwButtonID, s32 indwMouseButtonState);

		private:

			u32 mudwCurrentTouchID;
		};
	}
}

#endif

