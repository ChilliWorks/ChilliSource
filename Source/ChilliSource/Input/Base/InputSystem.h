/** 
 * File: InputSystem.h
 * Date: 5 Oct 2010
 * Description: Interface for a moFlo input system
 */

/** 
 *
 * Author Stuart 
 * Version 1.0 - moFlo
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#ifndef _MOFLO_INPUT_INPUTSYSTEM_H_
#define _MOFLO_INPUT_INPUTSYSTEM_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/System/System.h>
#include <ChilliSource/Core/System/SystemConcepts.h>

namespace ChilliSource
{

	namespace Input
	{
		class InputSystem : public Core::System, public Core::IUpdateable
		{
		public:
			virtual ~InputSystem(){};
			DECLARE_NAMED_INTERFACE(InputSystem);
			
			virtual bool CanCreateDeviceWithInterface(Core::InterfaceIDType inInterfaceID) const = 0;
			virtual InputDevice * GetDeviceWithInterface(Core::InterfaceIDType inInterfaceID) = 0;
			
			virtual TouchScreen * GetTouchScreenPtr(){return nullptr;}
			virtual Accelerometer * GetAccelerometerPtr(){return nullptr;}
			virtual VirtualKeyboard* GetVirtualKeyboardPtr(){return nullptr;}
			virtual Mouse* GetMousePtr(){return nullptr;}
            
            virtual void FlushBufferedInput(){};
            
        protected:
            
            f64 mffTimeStamp;
		};
	}
	
}

#endif
