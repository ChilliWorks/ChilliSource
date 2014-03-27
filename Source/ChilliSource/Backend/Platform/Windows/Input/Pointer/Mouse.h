//
//  Mouse.h
//  Chilli Source
//
//  Created by Scott Downie on 24/11/2011.
//  Copyright (c) 2011 Tag Games Ltd. All rights reserved.
//

#ifndef _CHILLISOURCE_BACKEND_PLATFORM_WINDOWS_INPUT_POINTER_MOUSE_H_
#define _CHILLISOURCE_BACKEND_PLATFORM_WINDOWS_INPUT_POINTER_MOUSE_H_

#include <ChilliSource/Backend/Platform/Windows/ForwardDeclarations.h>
#include <ChilliSource/Input/ForwardDeclarations.h>
#include <ChilliSource/Input/Pointer/Mouse.h>

struct GLFWwindow;

namespace ChilliSource
{
	namespace Windows
	{
		class Mouse : public Input::Mouse
		{
		public:
			//-------------------------------------------------------
			/// Constructor
			///
			/// @author S Downie
			//-------------------------------------------------------
			Mouse();
			//-------------------------------------------------------
			/// Constructor
			///
			/// @author S Downie
			///
			/// @param Touch screen to proxy mouse touches to
			//-------------------------------------------------------
			Mouse(Input::TouchScreen* in_touchProxy);
			//-------------------------------------------------------
			/// Destructor
			///
			/// @author S Downie
			//-------------------------------------------------------
			~Mouse();
			//-------------------------------------------------------
			/// @author S Downie
			///
			/// @param Interface ID
			///
			/// @return Whether class implements interface with ID
			//-------------------------------------------------------
			bool IsA(Core::InterfaceIDType in_interfaceID) const override;
			//------------------------------------------------------
			/// Get Position
			///
			/// @author S Downie
			///
			/// @return Position of cursor on screen
			//------------------------------------------------------
			Core::Vector2 GetPosition() const override;

		private:

			//---GLFW Mouse Delegates
			//----------------------------------------------
			/// Triggered by GLFW when the cursor moves
			///
			/// @author S Downie
			///
			/// @param Window that received input
			/// @param Cursor X Pos
			/// @param Cursor Y Pos
			//----------------------------------------------
			static void OnMouseMoved(GLFWwindow* in_window, f64 in_xPos, f64 in_yPos);
			//----------------------------------------------
			/// Triggered by GLFW when a mouse button is 
			/// pressed
			///
			/// @author S Downie
			///
			/// @param Window that received input
			/// @param Button ID
			/// @param Button action (Press/Release)
			/// @param Bit field describing modifier keys
			//----------------------------------------------
			static void OnMouseButtonPressed(GLFWwindow* in_window, s32 in_buttonID, s32 in_buttonAction, s32 in_modifierKeys);

		private:

			u32 m_currentTouchID;
		};
	}
}

#endif

