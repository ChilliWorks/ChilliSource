//
//  MainThreadId.h
//  ChilliSource
//  Created by Nicolas Tanda on 07/08/2015.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2015 Tag Games Limited
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

#ifdef CS_TARGETPLATFORM_ANDROID

#ifndef _CSBACKEND_PLATFORM_ANDROID_MAIN_JNI_CORE_THREADING_MAINTHREADID_H_
#define _CSBACKEND_PLATFORM_ANDROID_MAIN_JNI_CORE_THREADING_MAINTHREADID_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/Singleton.h>

#include <CSBackend/Platform/Android/Main/JNI/ForwardDeclarations.h>

#include <atomic>
#include <thread>

namespace CSBackend
{
	namespace Android
	{
		//------------------------------------------------------------------------------
		/// A container for the main thread id
		///
		/// @author Nicolas Tanda
		//------------------------------------------------------------------------------
		class MainThreadId final : public ChilliSource::Singleton<MainThreadId>
		{
		public:
			//------------------------------------------------------------------------------
			/// @author Nicolas Tanda
			///
			/// @return The Id of the main thread
			//------------------------------------------------------------------------------
			std::thread::id GetId() const;
			//------------------------------------------------------------------------------
			/// Set the main thread id to the current thread id
			///
			/// @author Nicolas Tanda
			//------------------------------------------------------------------------------
			void SetCurrentThread();

		private:
		    friend class ChilliSource::Singleton<MainThreadId>;
			//------------------------------------------------------------------------------
			/// Constructor.
			///
			/// @author Nicolas Tanda
			//------------------------------------------------------------------------------
			MainThreadId();

        private:
			std::atomic<std::thread::id> m_mainThreadId;
		};
	}
}

#endif

#endif
