//
//  InputSystem.h
//  ChilliSource
//
//  Created by Stuart McGaw on 22/11/2010.
//  Copyright 2010 Tag Games. All rights reserved.
//

#ifndef _CHILLISOURCE_INPUT_BASE_INPUTSYSTEM_H_
#define _CHILLISOURCE_INPUT_BASE_INPUTSYSTEM_H_

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
            
            CS_DECLARE_NAMEDTYPE(InputSystem);
            
            //-------------------------------------------------------
            /// Create the platform dependent backend
            ///
            /// @author S Downie
            ///
            /// @return New backend instance
            //-------------------------------------------------------
            static InputSystemUPtr Create();
            //-------------------------------------------------------
            /// Destructor
            ///
            /// @author S Downie
            //-------------------------------------------------------
			virtual ~InputSystem(){};
			
            //-------------------------------------------------------
            /// @author S Downie
            ///
            /// @param Interface ID of input device
            ///
            /// @return Whether input device of given type can be
            /// created
            //-------------------------------------------------------
			virtual bool CanCreateDeviceWithInterface(Core::InterfaceIDType in_interfaceID) const = 0;
            //-------------------------------------------------------
            /// @author S Downie
            ///
            /// @param Interface ID of input device
            ///
            /// @return Input device of given type or null if none
            /// exists
            //-------------------------------------------------------
			virtual InputDevice * GetDeviceWithInterface(Core::InterfaceIDType in_interfaceID) = 0;
            //-------------------------------------------------------
            /// @author S Downie
            ///
            /// @return Touchscreen device or null if none exists
            //-------------------------------------------------------
			virtual TouchScreen* GetTouchScreen() {return nullptr;}
            //-------------------------------------------------------
            /// @author S Downie
            ///
            /// @return Virtual keyboard device or null if none exists
            //-------------------------------------------------------
			virtual VirtualKeyboard* GetVirtualKeyboard() {return nullptr;}
            //-------------------------------------------------------
            /// @author S Downie
            ///
            /// @return Mouse device or null if none exists
            //-------------------------------------------------------
			virtual Mouse* GetMouse() {return nullptr;}
            //-------------------------------------------------------
            /// Flush the buffered input events to the application
            ///
            /// @author S Downie
            //-------------------------------------------------------
            virtual void FlushBufferedInput() {};
            
        protected:
            
            //-------------------------------------------------------
            /// Private constructor to force use of factory method
            ///
            /// @author S Downie
            //-------------------------------------------------------
            InputSystem(){}
            
        protected:
            
            f64 m_timeStamp = 0.0;
		};
	}
}

#endif
