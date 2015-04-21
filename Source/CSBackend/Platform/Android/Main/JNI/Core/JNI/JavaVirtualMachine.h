//
//  JavaVirtualMachine.h
//  Chilli Source
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

#ifndef _CSBACKEND_PLATFORM_ANDROID_MAIN_JNI_CORE_JNI_JAVAVIRTUALMACHINE_H_
#define _CSBACKEND_PLATFORM_ANDROID_MAIN_JNI_CORE_JNI_JAVAVIRTUALMACHINE_H_

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
		class JavaVirtualMachine final : public CSCore::Singleton<JavaVirtualMachine>
		{
		public:
			//------------------------------------------------------------------------------
			/// @author Ian Copland
			///
			/// @return The pointer to the JNI Environment for the current thread.
			//------------------------------------------------------------------------------
			JNIEnv* GetJNIEnvironment() const;

		private:
		    friend class CSCore::Singleton<JavaVirtualMachine>;
			//------------------------------------------------------------------------------
			/// Constructor.
			///
			/// @author Ian Copland
			///
			/// @param in_javaVirtualMachine - The pointer to the java virtual machine.
			//------------------------------------------------------------------------------
			JavaVirtualMachine(JavaVM* in_javaVirtualMachine);

			const JavaVM* m_javaVirtualMachine;
		};
	}
}

#endif

#endif
