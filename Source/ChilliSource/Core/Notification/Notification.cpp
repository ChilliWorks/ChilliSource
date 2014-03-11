//
//  Notification.cpp
//  Chilli Source
//
//  Created by Ian Copland on 10/03/2014.
//  Copyright (c) 2014 Tag Games Ltd. All rights reserved.
//

#include <ChilliSource/Core/Notification/Notification.h>

namespace ChilliSource
{
    namespace Core
    {
        //----------------------------------------------------
        //----------------------------------------------------
        Notification::Notification(ID in_id, const ParamDictionary& in_params, Priority in_priority)
            : m_id(in_id), m_params(in_params), m_priority(in_priority)
        {
        }
        //----------------------------------------------------
        //----------------------------------------------------
        Notification::ID Notification::GetID() const
        {
            return m_id;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        const ParamDictionary& Notification::GetParams() const
        {
            return m_params;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        Notification::Priority Notification::GetPriority() const
        {
            return m_priority;
        }
    }
}
