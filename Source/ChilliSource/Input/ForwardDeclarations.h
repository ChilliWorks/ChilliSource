/** 
 * File: ForwardDeclarations.h
 * Date: 5 Oct 2010
 * Description: Forward declarations for classes in the moFlo::Input namespace;
 */

/** 
 *
 * Author Stuart 
 * Version 1.0 - moFlo
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#ifndef _MOFLO_INPUT_FORWARDDECLARATIONS_H_
#define _MOFLO_INPUT_FORWARDDECLARATIONS_H_

namespace moFlo 
{
	namespace Input 
	{
		class IInputSystem;
		
		class IInputDeviceFactory;
		class IInputDevice;
		
		class ITouchScreen;
		class CGesture;
		class CTapGesture;
		class CSwipeGesture;
		class CPinchGesture;
		class CDragGesture;
		class CRotationGesture;
		
		struct TouchInfo;
		
		class IAccelerometer;
		class IMouse;
		class IKeyboard;
		
		//Virtual input devices
		class CVirtualAnalogStick;
		class IVirtualKeyboard;
	}
}

#endif