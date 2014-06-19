//
//  PerformanceTimer.h
//  MoshiMonsters
//
//  Created by Scott Downie on 04/09/2012.
//  Copyright (c) 2012 Tag Games. All rights reserved.
//

#ifndef _CHILLISOURCE_CORE_PERFORMANCE_TIMER_H_
#define _CHILLISOURCE_CORE_PERFORMANCE_TIMER_H_

#include <ChilliSource/ChilliSource.h>

#if defined CS_TARGETPLATFORM_IOS || defined CS_TARGETPLATFORM_ANDROID   
#include <sys/time.h>
#endif

namespace ChilliSource
{
    namespace Core
    {
		class PerformanceTimer final
		{
		public:
			PerformanceTimer();
			void Start();
			void Stop();
			f64 GetTimeTakenS() const;
			f64 GetTimeTakenMS() const;
			f64 GetTimeTakenMicroS() const;
        private:
			f64 m_lastDurationMicroS;

#if defined CS_TARGETPLATFORM_IOS || defined CS_TARGETPLATFORM_ANDROID
			timeval m_startTime;
#elif defined CS_TARGETPLATFORM_WINDOWS
			s64 m_frequency;
			s64 m_startTime;
#endif
        };
    }
}

#endif

