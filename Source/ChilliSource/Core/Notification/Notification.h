//
//  Notification.h
//  Chilli Source
//
//  Created by I Copland on 10/03/2014.
//  Copyright (c) 2014 Tag Games Ltd. All rights reserved.
//

#ifndef _CHILLISOURCE_CORE_NOTIFICATIONS_NOTIFICATION_H_
#define _CHILLISOURCE_CORE_NOTIFICATIONS_NOTIFICATION_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Event/Event.h>
#include <ChilliSource/Core/Container/ParamDictionary.h>

namespace ChilliSource
{
    namespace Core
    {
        //------------------------------------------------------------
        /// A notification that can be used by any of the notification
        /// systems.
        //------------------------------------------------------------
        struct Notification
        {
            //----------------------------------------------------
            /// An enum describing the priority of a notification.
            //----------------------------------------------------
            enum class Priority
            {
                k_standard,
                k_high
            };
            //----------------------------------------------------
            /// Typedefs
            //----------------------------------------------------
            typedef u32 ID;

            ID m_id;
            ParamDictionary m_params;
            Priority m_priority;
        };
    }
}

#endif
