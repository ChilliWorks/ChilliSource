//
//  JavaVirtualMachine.h
//  ChilliSource
//  Created by Ian Copland on 21/04/2015.
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

#ifndef _CSBACKEND_PLATFORM_ANDROID_MAIN_JNI_CORE_JAVA_JAVAVIRTUALMACHINE_H_
#define _CSBACKEND_PLATFORM_ANDROID_MAIN_JNI_CORE_JAVA_JAVAVIRTUALMACHINE_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/Singleton.h>

#include <CSBackend/Platform/Android/Main/JNI/ForwardDeclarations.h>

#include <jni.h>

namespace CSBackend
{
	namespace Android
	{
		//------------------------------------------------------------------------------
		/// A singleton which provides access to the java virtual machine. This is used
		/// to get the JNI Environment for the current thread.
		///
		/// @author Ian Copland
		//------------------------------------------------------------------------------
		class JavaVirtualMachine final : public ChilliSource::Singleton<JavaVirtualMachine>
		{
		public:
			//------------------------------------------------------------------------------
			/// Attached the current thread to the JVM. If a background thread created in
			/// native might be calling up to Java this should be the first thing it calls.
			/// If this is called, DetachCurrentThread() must be called before the thread
			/// finishes or the JVM will crash.
			///
			/// @author Ian Copland
			//------------------------------------------------------------------------------
			void AttachCurrentThread();
			//------------------------------------------------------------------------------
			/// @author Ian Copland
			///
			/// @return The pointer to the JNI Environment for the current thread. The
			/// thread will be attached to the JVM if it isn't already. This means that
			/// DetachCurrentThread will need to be called prior to the thread exiting if
			/// the thread was created in native.
			//------------------------------------------------------------------------------
			JNIEnv* GetJNIEnvironment();
			//------------------------------------------------------------------------------
			/// Detaches the current thread from the JVM. This must be called for all
			/// natively created threads that have been attached to the JVM. This must not
			/// be called for threads which were created in Java.
			///
			/// @author Ian Copland
			//------------------------------------------------------------------------------
			void DetachCurrentThread();
		private:
		    friend class ChilliSource::Singleton<JavaVirtualMachine>;
			//------------------------------------------------------------------------------
			/// Constructor.
			///
			/// @author Ian Copland
			///
			/// @param in_javaVirtualMachine - The pointer to the java virtual machine.
			//------------------------------------------------------------------------------
			JavaVirtualMachine(JavaVM* in_javaVirtualMachine);

			JavaVM* m_javaVirtualMachine;
		};
	}
}

#endif

#endif
