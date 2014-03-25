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
        CS_FORWARDDECLARE_CLASS(Accelerometer);
        //--------------------------------------------------
        /// Base
        //--------------------------------------------------
        CS_FORWARDDECLARE_CLASS(InputDevice);
        CS_FORWARDDECLARE_CLASS(InputDeviceFactory);
        CS_FORWARDDECLARE_CLASS(InputSystem);
        //--------------------------------------------------
        /// Gestures
        //--------------------------------------------------
        CS_FORWARDDECLARE_CLASS(Gesture);
        CS_FORWARDDECLARE_CLASS(TapCSwipeGestureGesture);
        CS_FORWARDDECLARE_CLASS(SwipeGesture);
        CS_FORWARDDECLARE_CLASS(PinchGesture);
        CS_FORWARDDECLARE_CLASS(DragGesture);
        CS_FORWARDDECLARE_CLASS(RotationGesture);
        //--------------------------------------------------
        /// Keyboard
        //--------------------------------------------------
        CS_FORWARDDECLARE_CLASS(Keyboard);
        //--------------------------------------------------
        /// Pointer
        //--------------------------------------------------
        CS_FORWARDDECLARE_CLASS(Mouse);
        CS_FORWARDDECLARE_CLASS(TouchScreen);
	}
}

#endif