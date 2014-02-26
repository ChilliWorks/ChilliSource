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

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/QueryableInterface.h>

namespace ChilliSource
{
	namespace Input
	{
		class InputDevice : public Core::IQueryableInterface
		{
		public:
			InputDevice(){}
			virtual ~InputDevice(){}
		};
		
		typedef std::shared_ptr<InputDevice> InputDevicePtr;
	}
}

#endif