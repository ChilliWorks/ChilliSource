//
//  Button.cpp
//  moFlo
//
//  Created by Scott Downie on 26/04/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#include <algorithm>

#include <ChilliSource/Rendering/GUI/Button.h>

namespace moFlo
{
    namespace GUI
    {
		DEFINE_META_CLASS(IButton)

        //--------------------------------------------------
        /// Get Activated Event
        ///
        /// @return Event triggered when the button is 
        /// activated
        //--------------------------------------------------
        IEvent<ButtonEventDelegate>& IButton::GetActivatedEvent()
        {
            return mOnButtonActivatedEvent;
        }
    }
}
