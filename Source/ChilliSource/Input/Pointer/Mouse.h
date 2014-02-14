
#ifndef _MO_FLO_INPUT_MOUSE_H_
#define _MO_FLO_INPUT_MOUSE_H_

#include <ChilliSource/Input/ForwardDeclarations.h>
#include <ChilliSource/Input/Base/InputDevice.h>

#include <ChilliSource/Core/Event/GenericEvent.h>

namespace moFlo
{
	namespace Input
	{
		enum MouseInputType
		{
			MOUSE_LEFT_BUTTON,
			MOUSE_RIGHT_BUTTON,
			MOUSE_MIDDLE_BUTTON,
			MOUSE_NUM_BUTTON,
		};

		typedef fastdelegate::FastDelegate1<const IMouse*> MouseEventDelegate;

		class IMouse : public IInputDevice
		{
		public:

			DECLARE_NAMED_INTERFACE(IMouse);
			IMouse();
			IMouse(ITouchScreen* inpTouchProxy);
			virtual ~IMouse(){}

			//------------------------------------------------------
			/// Is Button Down
			///
			/// @param Mouse button type
			/// @return Whether the mouse button is down
			//------------------------------------------------------
			bool IsButtonDown(MouseInputType ineButton) const;
			//------------------------------------------------------
			/// Get Position
			/// 
			/// @return Position of cursor on screen
			//------------------------------------------------------
			virtual moFlo::Core::CVector2 GetPosition() const = 0;
			//------------------------------------------------------
			/// Get Mouse Pressed Event
			///
			/// @return Event triggered on mouse button down
			//------------------------------------------------------
			IEvent<MouseEventDelegate> & GetMousePressedEvent();
			//------------------------------------------------------
			/// Get Mouse Moved Event
			///
			/// @return Event triggered on mouse moved
			//------------------------------------------------------
			IEvent<MouseEventDelegate> & GetMouseMovedEvent();
			//------------------------------------------------------
			/// Get Mouse Released Event
			///
			/// @return Event triggered on mouse button up
			//------------------------------------------------------
			IEvent<MouseEventDelegate> & GetMouseReleasedEvent();

		protected:

			CEvent1<MouseEventDelegate> mOnMouseMovedEvent;
			CEvent1<MouseEventDelegate> mOnMousePressedEvent;
			CEvent1<MouseEventDelegate> mOnMouseReleasedEvent;

			//Tracks the state of the mouse buttons
			bool mbaButtonsDown[MOUSE_NUM_BUTTON];

			ITouchScreen* mpTouchProxy;
		};
	}
}

#endif