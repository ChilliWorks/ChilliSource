//
//  PerformanceTimer.cpp
//  MoshiMonsters
//
//  Created by Scott Downie on 04/09/2012.
//  Copyright (c) 2012 Tag Games. All rights reserved.
//

#include <ChilliSource/Core/Time/PerformanceTimer.h>

#ifdef CS_TARGETPLATFORM_WINDOWS
#include <windows.h>
#endif

namespace ChilliSource
{
    namespace Core
    {
		PerformanceTimer::PerformanceTimer()
			: m_lastDurationMicroS(0.0)
		{
#ifdef CS_TARGETPLATFORM_WINDOWS
			LARGE_INTEGER frequency;
			QueryPerformanceFrequency(&frequency);
			m_frequency = frequency.QuadPart;
			m_startTime = 0;
#endif
		}

        void PerformanceTimer::Start()
        {
#if defined CS_TARGETPLATFORM_IOS || defined CS_TARGETPLATFORM_ANDROID     
			gettimeofday(&m_startTime, 0);
#elif defined CS_TARGETPLATFORM_WINDOWS
			LARGE_INTEGER startTime;
			QueryPerformanceCounter(&startTime);
			m_startTime = startTime.QuadPart;
#endif
        }
        
        void PerformanceTimer::Stop()
        {
#if defined CS_TARGETPLATFORM_IOS || defined CS_TARGETPLATFORM_ANDROID    
            TimeValue stopTime;
			gettimeofday(&stopTime, 0);
			f64 startTimeMicro = (m_startTime.tv_sec * 1000000.0) + m_startTime.tv_usec;
			f64 stopTimeMicro = (stopTime.tv_sec * 1000000.0) + stopTime.tv_usec;
			m_lastDurationMicroS = stopTimeMicro - startTimeMicro;
#elif defined CS_TARGETPLATFORM_WINDOWS
			LARGE_INTEGER end;
			QueryPerformanceCounter(&end);
			m_lastDurationMicroS = (static_cast<f64>(end.QuadPart - m_startTime) / m_frequency) * 1000000.0;
#endif
        }
        
        f64 PerformanceTimer::GetTimeTakenS() const
        {
            return m_lastDurationMicroS * 0.000001;
        }
        
        f64 PerformanceTimer::GetTimeTakenMS() const
        {
			return m_lastDurationMicroS * 0.001;
        }
        
        f64 PerformanceTimer::GetTimeTakenMicroS() const
        {
			return m_lastDurationMicroS;
        }
    }
}