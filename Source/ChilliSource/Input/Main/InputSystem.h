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

#include <ChilliSource/Input/ForwardDeclarations.h>
#include <ChilliSource/Core/System.h>
#include <ChilliSource/Core/SystemConcepts.h>

namespace moFlo
{

	namespace Input
	{
		class IInputSystem : public Core::ISystem, public Core::IUpdateable
		{
		public:
			virtual ~IInputSystem(){};
			DECLARE_NAMED_INTERFACE(IInputSystem);
			
			virtual bool CanCreateDeviceWithInterface(Core::InterfaceIDType inInterfaceID) const = 0;
			virtual IInputDevice * GetDeviceWithInterface(Core::InterfaceIDType inInterfaceID) = 0;
			
			virtual ITouchScreen * GetTouchScreenPtr(){return NULL;}
			virtual IAccelerometer * GetAccelerometerPtr(){return NULL;}
			virtual IVirtualKeyboard* GetVirtualKeyboardPtr(){return NULL;}
			virtual IMouse* GetMousePtr(){return NULL;}
            
            virtual void FlushBufferedInput(){};
            
        protected:
            
            f64 mffTimeStamp;
		};
	}
	
}

#endif
