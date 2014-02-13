
#include <moFlo/Platform/Windows/Input/TouchscreenWindows.h>

namespace moFlo
{
	namespace WindowsPlatform
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