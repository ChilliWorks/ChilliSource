//
//  PerformanceTimer.h
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

