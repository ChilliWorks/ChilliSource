
#ifndef _MO_FLO_INPUT_MOUSE_H_
#define _MO_FLO_INPUT_MOUSE_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Input/Base/InputDevice.h>
#include <ChilliSource/Core/Event/GenericEvent.h>

namespace ChilliSource
{
	namespace Input
	{
		enum class MouseInputType
		{
            k_leftButton,
            k_rightButton,
            k_middleButton,
            k_total
		};

		typedef std::function<void(const Mouse*)> MouseEventDelegate;

		class Mouse : public InputDevice
		{
		public:

			DECLARE_NAMED_INTERFACE(Mouse);
			Mouse();
			Mouse(TouchScreen* inpTouchProxy);
			virtual ~Mouse(){}

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
			virtual ChilliSource::Core::CVector2 GetPosition() const = 0;
			//------------------------------------------------------
			/// Get Mouse Pressed Event
			///
			/// @return Event triggered on mouse button down
			//------------------------------------------------------
			Core::IEvent<MouseEventDelegate> & GetMousePressedEvent();
			//------------------------------------------------------
			/// Get Mouse Moved Event
			///
			/// @return Event triggered on mouse moved
			//------------------------------------------------------
			Core::IEvent<MouseEventDelegate> & GetMouseMovedEvent();
			//------------------------------------------------------
			/// Get Mouse Released Event
			///
			/// @return Event triggered on mouse button up
			//------------------------------------------------------
			Core::IEvent<MouseEventDelegate> & GetMouseReleasedEvent();

		protected:

			Core::CEvent1<MouseEventDelegate> mOnMouseMovedEvent;
			Core::CEvent1<MouseEventDelegate> mOnMousePressedEvent;
			Core::CEvent1<MouseEventDelegate> mOnMouseReleasedEvent;

			//Tracks the state of the mouse buttons
			bool mbaButtonsDown[(u32)MouseInputType::k_total];

			TouchScreen* mpTouchProxy;
		};
	}
}

#endif