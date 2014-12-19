//
//  HttpRequestSystem.cpp
//  Chilli Source
//  Created by Scott Downie on 23/05/2011.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2011 Tag Games Limited
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

#include <ChilliSource/Networking/Http/HttpRequestSystem.h>

#ifdef CS_TARGETPLATFORM_IOS
#include <CSBackend/Platform/iOS/Networking/Http/HttpRequestSystem.h>
#endif

#ifdef CS_TARGETPLATFORM_ANDROID
#include <CSBackend/Platform/Android/Networking/Http/HttpRequestSystem.h>
#endif

#ifdef CS_TARGETPLATFORM_WINDOWS
#include <CSBackend/Platform/Windows/Networking/Http/HttpRequestSystem.h>
#endif

namespace ChilliSource
{
	namespace Networking
    {
		CS_DEFINE_NAMEDTYPE(HttpRequestSystem);
        
        //-------------------------------------------------------
        //-------------------------------------------------------
        HttpRequestSystemUPtr HttpRequestSystem::Create()
        {
#ifdef CS_TARGETPLATFORM_IOS
            return HttpRequestSystemUPtr(new CSBackend::iOS::HttpRequestSystem());
#endif
#ifdef CS_TARGETPLATFORM_ANDROID
            return HttpRequestSystemUPtr(new CSBackend::Android::HttpRequestSystem());
#endif
#ifdef CS_TARGETPLATFORM_WINDOWS
            return HttpRequestSystemUPtr(new CSBackend::Windows::HttpRequestSystem());
#endif
            return nullptr;
        }
        //--------------------------------------------------------------------------------------------------
        /// @author S Downie
        ///
        /// @param The number of bytes read before the buffer is flushed (0 is infinite)
        //--------------------------------------------------------------------------------------------------
        void HttpRequestSystem::SetMaxBufferSize(u32 in_sizeInBytes)
        {
            m_maxBufferSize = in_sizeInBytes;
        }
        //--------------------------------------------------------------------------------------------------
        //--------------------------------------------------------------------------------------------------
        u32 HttpRequestSystem::GetMaxBufferSize() const
        {
            return m_maxBufferSize;
        }
	}
}
