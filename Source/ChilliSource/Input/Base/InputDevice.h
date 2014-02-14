/** 
 * File: InputDevice.h
 * Date: 5 Oct 2010
 * Description: Interface for a InputDevice
 */

/** 
 *
 * Author Stuart 
 * Version 1.0 - moFlo
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#ifndef _MOFLO_INPUT_INPUTDEVICE_H_
#define _MOFLO_INPUT_INPUTDEVICE_H_

#include <ChilliSource/Core/Base/QueryableInterface.h>

namespace moFlo
{
	namespace Input
	{
		class IInputDevice : public Core::IQueryableInterface
		{
		public:
			IInputDevice(){}
			virtual ~IInputDevice(){}
		};
		
		typedef SHARED_PTR<IInputDevice> InputDevicePtr;
	}
}

#endif