//
//  PerformanceTimer.cpp
//  Chilli Source
//  Created by Scott Downie on 04/09/2012.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2012 Tag Games Limited
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
            timeval stopTime;
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