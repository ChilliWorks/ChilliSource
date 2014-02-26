
#include <ChilliSource/Input/Pointer/Mouse.h>

namespace ChilliSource
{
	namespace Input
	{
		DEFINE_NAMED_INTERFACE(Mouse);

		//------------------------------------------------------
		/// Constructor
		///
		/// Default
		//------------------------------------------------------
		Mouse::Mouse() : mpTouchProxy(nullptr)
		{
			memset(mbaButtonsDown, false, sizeof(bool) * (u32)MouseInputType::k_total);
		}
		//------------------------------------------------------
		/// Constructor
		///
		/// Takes a touch screen proxy to fake touch input
		///
		/// @param Touch screen proxy
		//------------------------------------------------------
		Mouse::Mouse(TouchScreen* inpTouchProxy) : mpTouchProxy(inpTouchProxy)
		{
			memset(mbaButtonsDown, false, sizeof(bool) * (u32)MouseInputType::k_total);
		}
		//------------------------------------------------------
		/// Is Button Down
		///
		/// @param Mouse button type
		/// @return Whether the mouse button is down
		//------------------------------------------------------
		bool Mouse::IsButtonDown(MouseInputType ineButton) const
		{
			return mbaButtonsDown[(u32)ineButton];
		}
		//------------------------------------------------------
		/// Get Mouse Pressed Event
		///
		/// @return Event triggered on mouse button down
		//------------------------------------------------------
		Core::IEvent<MouseEventDelegate> & Mouse::GetMousePressedEvent()
		{
			return mOnMousePressedEvent;
		}
		//------------------------------------------------------
		/// Get Mouse Moved Event
		///
		/// @return Event triggered on mouse moved
		//------------------------------------------------------
		Core::IEvent<MouseEventDelegate> & Mouse::GetMouseMovedEvent()
		{
			return mOnMouseMovedEvent;
		}
		//------------------------------------------------------
		/// Get Mouse Released Event
		///
		/// @return Event triggered on mouse button up
		//------------------------------------------------------
		Core::IEvent<MouseEventDelegate> & Mouse::GetMouseReleasedEvent()
		{
			return mOnMouseReleasedEvent;
		}
	}
}