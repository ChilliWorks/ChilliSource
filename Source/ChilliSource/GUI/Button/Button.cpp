//
//  Button.cpp
//  moFlo
//
//  Created by Scott Downie on 26/04/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#include <algorithm>

#include <ChilliSource/GUI/Button/Button.h>

namespace ChilliSource
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
        Core::IEvent<ButtonEventDelegate>& IButton::GetActivatedEvent()
        {
            return mOnButtonActivatedEvent;
        }
    }
}
