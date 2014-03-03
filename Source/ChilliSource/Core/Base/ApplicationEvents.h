//
//  ApplicationEvents.h
//  moFloTest
//
//  Created by Scott Downie on 13/05/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#ifndef _MO_FLO_CORE_APPLICATION_EVENTS_H_
#define _MO_FLO_CORE_APPLICATION_EVENTS_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/Screen.h>
#include <ChilliSource/Core/Event/Event.h>

#include <functional>

namespace ChilliSource
{
    namespace Core
    {
        typedef std::function<void()> ApplicationSystemDelegate;
        typedef std::function<void(ScreenOrientation)> ApplicationScreenOrientationDelegate;
		typedef std::function<void(u32, u32)> ApplicationScreenResizeDelegate;
        typedef std::function<void(const std::string&)> ApplicationReceivesURLDelegate;
		
        class ApplicationEvents
        {
        public:
            
            //-----------------------------------------------------------------
            /// Get Resume Event
            ///
            /// @return Event that is triggered when the application resumes
            //-----------------------------------------------------------------
            static Event<ApplicationSystemDelegate>& GetResumeEvent();
            //-----------------------------------------------------------------
            /// Get Suspend Event
            ///
            /// @return Event that is triggered on application suspension
            //-----------------------------------------------------------------
            static Event<ApplicationSystemDelegate>& GetSuspendEvent();
			//-----------------------------------------------------------------
            /// Get Late Suspend Event
            ///
            /// @return Event that is triggered on application suspension after GetSuspendEvent
            //-----------------------------------------------------------------
            static Event<ApplicationSystemDelegate>& GetLateSuspendEvent();
            //-----------------------------------------------------------------
            /// Get Low Memory Event
            ///
            /// @return Event that is triggered when the application 
            /// receives a memory warning
            //-----------------------------------------------------------------
            static Event<ApplicationSystemDelegate>& GetLowMemoryEvent();
            //-----------------------------------------------------------------
            /// Get Screen Orientation Changed Event
            ///
            /// @return Event that is triggered when screen rotates
            //-----------------------------------------------------------------
            static Event<ApplicationScreenOrientationDelegate>& GetScreenOrientationChangedEvent();
            //-----------------------------------------------------------------
            /// Get Back Event
            ///
            /// @return Event that is triggered when back is pressed
            //-----------------------------------------------------------------
            static Event<ApplicationSystemDelegate>& GetGoBackEvent();
			//-----------------------------------------------------------------
            /// Get Screen Resized Event
            ///
            /// @return Event that is triggered when screen resizes
            //-----------------------------------------------------------------
            static Event<ApplicationScreenResizeDelegate>& GetScreenResizedEvent();
			//-----------------------------------------------------------------
            /// GetReceivesURLEvent
            ///
            /// @return Event that is triggered when the app receives a URL string
            //-----------------------------------------------------------------
            static Event<ApplicationReceivesURLDelegate>& GetReceivesURLEvent();
        private:
            
            static Event<ApplicationSystemDelegate> mResumeEvent;
            static Event<ApplicationSystemDelegate> mSuspendEvent;
			static Event<ApplicationSystemDelegate> mLateSuspendEvent;
            static Event<ApplicationSystemDelegate> mLowMemoryEvent;
            
            static Event<ApplicationScreenOrientationDelegate> mScreenOrientationChangedEvent;
            static Event<ApplicationSystemDelegate> mGoBackEvent;
			static Event<ApplicationScreenResizeDelegate> mScreenResizedEvent;
			static Event<ApplicationReceivesURLDelegate> mReceivesURLEvent;
        };
    }
}

#endif
