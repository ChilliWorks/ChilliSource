//
//  CoreTimer.h
//  ChilliSource
//  Created by Scott Downie on 09/11/2010.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2010 Tag Games Limited
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

#ifndef _CHILLISOURCE_CORE_CORE_TIMER_H_
#define _CHILLISOURCE_CORE_CORE_TIMER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Event/Event.h>

#include <functional>

namespace ChilliSource
{
    typedef std::function<void(const f32)> TimeEventDelegate;
    
    class CoreTimer
    {
    public:
        
        //----------------------------------------------------
        /// Get Timer Update Event
        ///
        /// @return The event
        //----------------------------------------------------
        static IConnectableEvent<TimeEventDelegate>& GetTimerUpdateEvent();

    private:
        
        //----------------------------------------------------
        /// Update
        ///
        /// @param Time since last frame
        //----------------------------------------------------
        static void Update(f32 infDT);
        
    private:
        
        static Event<TimeEventDelegate> mOnTimerUpdateEvent;
        
        friend class Application; //Only application can invoke the update
    };
}

#endif
