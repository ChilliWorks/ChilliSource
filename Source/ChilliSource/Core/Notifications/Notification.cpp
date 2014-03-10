//
//  Notification.cpp
//  Chilli Source
//
//  Created by Ian Copland on 10/03/2014.
//  Copyright (c) 2014 Tag Games Ltd. All rights reserved.
//

#include <ChilliSource/Core/Notifications/Notification.h>

namespace ChilliSource
{
    namespace Core
    {
        //----------------------------------------------------
        //----------------------------------------------------
        Notification::Notification(ID in_id, const ParamDictionary& in_params)
            : m_id(in_id), m_params(in_params)
        {
        }
        //----------------------------------------------------
        //----------------------------------------------------
        IConnectableEvent<Notification::PresentedDelegate>& Notification::GetPresentedEvent()
        {
            return m_presentedEvent;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        IConnectableEvent<Notification::DismissedDelegate>& Notification::GetDismissedEvent()
        {
            return m_dismissedEvent;
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
        void Notification::Dismiss()
        {
            m_dismissed = true;
            
            m_dismissedEvent.NotifyConnections();
            m_dismissedEvent.CloseAllConnections();
        }
    }
}
