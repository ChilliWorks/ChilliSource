//
//  PerformanceTimer.cpp
//  MoshiMonsters
//
//  Created by Scott Downie on 04/09/2012.
//  Copyright (c) 2012 Tag Games. All rights reserved.
//

#include <ChilliSource/Core/Time/PerformanceTimer.h>

namespace ChilliSource
{
    namespace Core
    {
        void CPerformanceTimer::Start()
        {
#ifdef TARGET_WINDOWS
            QueryPerformanceFrequency(&mFrequency);
            QueryPerformanceCounter(&mStartTime);
#else
            gettimeofday(&mStartTime, NULL);
#endif
        }
        
        void CPerformanceTimer::Stop()
        {
            TimeValue StopTime;
#ifdef TARGET_WINDOWS
            QueryPerformanceCounter(&StopTime);
#else
            gettimeofday(&StopTime, NULL);
#endif
            mffLastDurationMicroS = GetTimeDurationMicroS(mStartTime, StopTime);
        }
        
        f64 CPerformanceTimer::GetTimeTakenS() const
        {
            return mffLastDurationMicroS * 0.000001;
        }
        
        f64 CPerformanceTimer::GetTimeTakenMS() const
        {
            return mffLastDurationMicroS * 0.001;
        }
        
        f64 CPerformanceTimer::GetTimeTakenMicroS() const
        {
            return mffLastDurationMicroS;
        }
        
        f64 CPerformanceTimer::GetTimeDurationMicroS(const TimeValue& inStart, const TimeValue& inEnd)
        {
            f64 ffStartTimeMicro, ffStopTimeMicro = 0.0;
            
#ifdef TARGET_WINDOWS
            //ffStartTimeMicro = inStart.QuadPart * (1000000.0 / mFrequency.QuadPart);
            //ffStopTimeMicro = inEnd.QuadPart * (1000000.0 / mFrequency.QuadPart);
#else
            ffStartTimeMicro = (inStart.tv_sec * 1000000.0) + inStart.tv_usec;
            ffStopTimeMicro = (inEnd.tv_sec * 1000000.0) + inEnd.tv_usec;
#endif
            
            return ffStopTimeMicro - ffStartTimeMicro;
        }
    }
}