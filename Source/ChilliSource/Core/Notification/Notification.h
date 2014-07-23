//
//  Notification.h
//  Chilli Source
//  Created by Ian Copland on 10/03/2014.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2014 Tag Games Limited
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//

#ifndef _CHILLISOURCE_CORE_NOTIFICATION_NOTIFICATION_H_
#define _CHILLISOURCE_CORE_NOTIFICATION_NOTIFICATION_H_

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
