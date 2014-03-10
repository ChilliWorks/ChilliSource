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
        class Notification
        {
        public:
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
            //----------------------------------------------------
            /// Constructor
            ///
            /// @author I Copland
            ///
            /// @param The ID.
            /// @param The param dictionary.
            /// @param The notification priority.
            //----------------------------------------------------
            Notification(ID in_id, const ParamDictionary& in_params, Priority in_priority);
            //----------------------------------------------------
            /// @author I Copland
            ///
            /// @return The notifications ID.
            //----------------------------------------------------
            ID GetID() const;
            //----------------------------------------------------
            /// @author I Copland
            ///
            /// @return The params associated with this notification.
            //----------------------------------------------------
            const ParamDictionary& GetParams() const;
            //----------------------------------------------------
            /// @author I Copland
            ///
            /// @return The priority of the notification.
            //----------------------------------------------------
            Priority GetPriority() const;
        private:
            ID m_id;
            ParamDictionary m_params;
            Priority m_priority;
        };
    }
}

#endif
