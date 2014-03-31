//
//  PointerSystem.h
//  Chilli Source
//
//  Created by Ian Copland on 27/03/2014.
//  Copyright (c) 2014 Tag Games Ltd. All rights reserved.
//

#ifndef _CHILLISOURCE_BACKEND_PLATFORM_WINDOWS_INPUT_POINTER_POINTERSYSTEM_H_
#define _CHILLISOURCE_BACKEND_PLATFORM_WINDOWS_INPUT_POINTER_POINTERSYSTEM_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Backend/Platform/Windows/ForwardDeclarations.h>
#include <ChilliSource/Input/Pointer/PointerSystem.h>

struct GLFWwindow;

namespace ChilliSource
{
	namespace Windows
	{
		//------------------------------------------------------------
		/// The iOS backend for the pointer system. This provides
		/// access to touch events.
		///
		/// @author I Copland
		//------------------------------------------------------------
		class PointerSystem final : public Input::PointerSystem
		{
		public:
			CS_DECLARE_NAMEDTYPE(PointerSystem);
			//----------------------------------------------------
			/// Queries whether or not this system implements the
			/// interface with the given ID.
			///
			/// @author I Copland
			///
			/// @param The interface Id.
			///
			/// @return Whether or not the interface is implemented.
			//----------------------------------------------------
			bool IsA(Core::InterfaceIDType in_interfaceId) const override;
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
			friend Input::PointerSystemUPtr Input::PointerSystem::Create();

			//------------------------------------------------
			/// Default constructor. Declared private to force
			/// the use of the factory method.
			///
			/// @author I Copland
			//------------------------------------------------
			PointerSystem() = default;
			//------------------------------------------------
			/// Initialisation method called at a time when
			/// all App Systems have been created. System
			/// initialisation occurs in the order they were
			/// created.
			///
			/// @author I Copland
			//------------------------------------------------
			void OnInit() override;
			//------------------------------------------------
			/// Called when the application is being destroyed.
			/// This should be used to cleanup memory and
			/// references to other systems. System destruction
			/// occurs in the reverse order to which they
			/// were created
			///
			/// @author I Copland
			//------------------------------------------------
			void OnDestroy() override;

			u64 m_pointerId;
		};
	}
}

#endif
