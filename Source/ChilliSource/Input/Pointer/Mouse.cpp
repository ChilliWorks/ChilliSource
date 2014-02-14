
#include <ChilliSource/Input/Pointer/Mouse.h>

namespace moFlo
{
	namespace Input
	{
		DEFINE_NAMED_INTERFACE(IMouse);

		//------------------------------------------------------
		/// Constructor
		///
		/// Default
		//------------------------------------------------------
		IMouse::IMouse() : mpTouchProxy(NULL)
		{
			memset(mbaButtonsDown, false, sizeof(bool) * MOUSE_NUM_BUTTON);
		}
		//------------------------------------------------------
		/// Constructor
		///
		/// Takes a touch screen proxy to fake touch input
		///
		/// @param Touch screen proxy
		//------------------------------------------------------
		IMouse::IMouse(ITouchScreen* inpTouchProxy) : mpTouchProxy(inpTouchProxy)
		{
			memset(mbaButtonsDown, false, sizeof(bool) * MOUSE_NUM_BUTTON);
		}
		//------------------------------------------------------
		/// Is Button Down
		///
		/// @param Mouse button type
		/// @return Whether the mouse button is down
		//------------------------------------------------------
		bool IMouse::IsButtonDown(MouseInputType ineButton) const
		{
			return mbaButtonsDown[ineButton];
		}
		//------------------------------------------------------
		/// Get Mouse Pressed Event
		///
		/// @return Event triggered on mouse button down
		//------------------------------------------------------
		IEvent<MouseEventDelegate> & IMouse::GetMousePressedEvent()
		{
			return mOnMousePressedEvent;
		}
		//------------------------------------------------------
		/// Get Mouse Moved Event
		///
		/// @return Event triggered on mouse moved
		//------------------------------------------------------
		IEvent<MouseEventDelegate> & IMouse::GetMouseMovedEvent()
		{
			return mOnMouseMovedEvent;
		}
		//------------------------------------------------------
		/// Get Mouse Released Event
		///
		/// @return Event triggered on mouse button up
		//------------------------------------------------------
		IEvent<MouseEventDelegate> & IMouse::GetMouseReleasedEvent()
		{
			return mOnMouseReleasedEvent;
		}
	}
}