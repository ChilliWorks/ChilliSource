
#ifndef _MO_FLO_WINDOWSPLATFORM_INPUT_MOUSE_H_
#define _MO_FLO_WINDOWSPLATFORM_INPUT_MOUSE_H_

#include <ChilliSource/Backend/Platform/Windows/ForwardDeclarations.h>
#include <ChilliSource/Input/ForwardDeclarations.h>
#include <ChilliSource/Input/Pointer/Mouse.h>

namespace ChilliSource
{
	namespace Windows
	{
		class Mouse : public Input::Mouse
		{
		public:
			Mouse();
			Mouse(Input::TouchScreen* inpTouchProxy);
			virtual ~Mouse();

			//-------------------------------------------------------
			/// Is A
			//-------------------------------------------------------
			bool IsA(Core::InterfaceIDType inInterfaceID) const;
			//------------------------------------------------------
			/// Get Position
			/// 
			/// @return Position of cursor on screen
			//------------------------------------------------------
			ChilliSource::Core::Vector2 GetPosition() const;

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

