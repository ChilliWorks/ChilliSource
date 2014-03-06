/** 
 * File: Touchscreen.h
 * Date: 5 Oct 2010
 * Description: Interface for a TouchScreen input device
 */

/** 
 *
 * Author Stuart 
 * Version 1.0 - moFlo
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#ifndef _MOFLO_PLATFORM_WINDOWS_TOUCHSCREEN_H_
#define _MOFLO_PLATFORM_WINDOWS_TOUCHSCREEN_H_

#include <ChilliSource/Backend/Platform/Windows/ForwardDeclarations.h>
#include <ChilliSource/Input/Pointer/TouchScreen.h>

namespace ChilliSource
{
	namespace Windows
	{
		class TouchScreen : public Input::TouchScreen
		{
		public:
			TouchScreen(){}
			virtual ~TouchScreen(){}

			//-------------------------------------------------------
			/// Is A
			//-------------------------------------------------------
			bool IsA(Core::InterfaceIDType inInterfaceID) const override;
		};
	}
}

#endif