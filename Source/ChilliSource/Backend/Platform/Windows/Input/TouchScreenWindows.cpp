
#include <ChilliSource/Platform/Windows/Input/TouchscreenWindows.h>

namespace ChilliSource
{
	namespace Windows
	{
		//-------------------------------------------------------
		/// Is A
		//-------------------------------------------------------
		bool CTouchScreen::IsA(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == Input::ITouchScreen::InterfaceID;
		}
	}
}