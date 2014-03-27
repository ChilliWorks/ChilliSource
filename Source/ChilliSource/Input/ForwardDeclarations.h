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
        /// Gestures
        //--------------------------------------------------
        CS_FORWARDDECLARE_CLASS(Gesture);
        CS_FORWARDDECLARE_CLASS(TapGesture);
        CS_FORWARDDECLARE_CLASS(SwipeGesture);
        CS_FORWARDDECLARE_CLASS(PinchGesture);
        CS_FORWARDDECLARE_CLASS(DragGesture);
        CS_FORWARDDECLARE_CLASS(HoldGesture);
        //--------------------------------------------------
        /// Keyboard
        //--------------------------------------------------
        CS_FORWARDDECLARE_CLASS(Keyboard);
        //--------------------------------------------------
        /// Pointer
        //--------------------------------------------------
        CS_FORWARDDECLARE_CLASS(PointerSystem);
	}
}

#endif
