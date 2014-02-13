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

#include <moFlo/Input/TouchScreen.h>

namespace moFlo
{
	namespace WindowsPlatform
	{
		class CTouchScreen : public Input::ITouchScreen
		{
		public:
			CTouchScreen(){}
			virtual ~CTouchScreen(){}

			//-------------------------------------------------------
			/// Is A
			//-------------------------------------------------------
			bool IsA(Core::InterfaceIDType inInterfaceID) const;
		};
	}
}

#endif