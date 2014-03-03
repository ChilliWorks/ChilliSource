
#include <ChilliSource/Backend/Platform/Windows/Input/Pointer/TouchScreen.h>

namespace ChilliSource
{
	namespace Windows
	{
		//-------------------------------------------------------
		/// Is A
		//-------------------------------------------------------
		bool CTouchScreen::IsA(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == Input::TouchScreen::InterfaceID;
		}
	}
}