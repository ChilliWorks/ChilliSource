/*
 *  Activity.h
 *  moFlow
 *
 *  Created by Stuart McGaw on 10/06/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#ifndef _MOFLO_CORE_ACTIVITY_H_
#define _MOFLO_CORE_ACTIVITY_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/QueryableInterface.h>

#include <functional>

namespace ChilliSource
{
    namespace Core
    {
        /*
         Activity represents a class offering UI to perform a specific task
        */
        class Activity : public Core::QueryableInterface
        {
        public:
            virtual ~Activity(){}
        };
        
        typedef std::function<void(Activity*)> ActivityDismissedEvent;
    }
}


#endif
