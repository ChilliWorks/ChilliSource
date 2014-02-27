//
//  ForwardDeclarations.h
//  Chilli Source
//
//  Created by Ian Copland on 26/02/2014.
//  Copyright 2014 Tag Games. All rights reserved.
//

#ifndef _CHILLISOURCE_INPUT_FORWARDDECLARATIONS_H_
#define _CHILLISOURCE_INPUT_FORWARDDECLARATIONS_H_

#include <ChilliSource/Core/Base/StandardMacros.h>

#include <memory>

namespace ChilliSource 
{
	namespace Input 
	{
        //--------------------------------------------------
        /// Accelerometer
        //--------------------------------------------------
        CS_FORWARD_DECLARE_CLASS(Accelerometer);
        //--------------------------------------------------
        /// Base
        //--------------------------------------------------
        CS_FORWARD_DECLARE_CLASS(InputDevice);
        CS_FORWARD_DECLARE_CLASS(InputDeviceFactory);
        CS_FORWARD_DECLARE_CLASS(InputSystem);
        //--------------------------------------------------
        /// Gestures
        //--------------------------------------------------
        CS_FORWARD_DECLARE_CLASS(Gesture);
        CS_FORWARD_DECLARE_CLASS(TapCSwipeGestureGesture);
        CS_FORWARD_DECLARE_CLASS(SwipeGesture);
        CS_FORWARD_DECLARE_CLASS(PinchGesture);
        CS_FORWARD_DECLARE_CLASS(DragGesture);
        CS_FORWARD_DECLARE_CLASS(RotationGesture);
        //--------------------------------------------------
        /// Keyboard
        //--------------------------------------------------
        CS_FORWARD_DECLARE_CLASS(VirtualKeyboard);
        //--------------------------------------------------
        /// Pointer
        //--------------------------------------------------
        CS_FORWARD_DECLARE_CLASS(Mouse);
        CS_FORWARD_DECLARE_CLASS(TouchScreen);
	}
}

#endif