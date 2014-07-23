//
//  _JavaInterface.h
//  Chilli Source
//  Created by Ian Copland on 17/03/2011.
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

#ifdef CS_TARGETPLATFORM_ANDROID

#ifndef _CHILLISOURCE_PLATFORM_ANDROID_JAVAINTERFACE__JAVAINTERFACE_
#define _CHILLISOURCE_PLATFORM_ANDROID_JAVAINTERFACE__JAVAINTERFACE_

#include <ChilliSource/ChilliSource.h>

#include <jni.h>
#include <map>

namespace CSBackend
{
	namespace Android
	{
		struct MethodReference
		{
			jmethodID mMethodID;
			jclass mClassID;
		};

		class _IJavaInterface
		{
			_IJavaInterface();
		public:
			virtual ~_IJavaInterface();

			//--------------------------------------------------------------------------------------
			/// Get JNI Environment Ptr
			///
			///	returns the jni enviroment pointer for the current thread. The value returned from
			/// this should NOT be stored as global or member data, as this will break thread safety
			///
			/// @return JNIEnv* returns a pointer to the jni environment.
			//--------------------------------------------------------------------------------------
			static JNIEnv* GetJNIEnvironmentPtr();
			//--------------------------------------------------------------------------------------
			/// Get Java VM
			///
			/// @return VM
			//--------------------------------------------------------------------------------------
			static JavaVM* GetJavaVM();
			//--------------------------------------------------------------------------------------
			/// Get Static Method Reference
			///
			/// Returns the method id for the specfied java method name
			///
			/// @param std::string method name.
			/// @return the method reference.
			//--------------------------------------------------------------------------------------
			static MethodReference GetStaticMethodReference(std::string instrMethodName);
			//--------------------------------------------------------------------------------------
			/// Create JString From STDString
			///
			/// Converts a std string into a JString.
			///
			/// @param the std::string
			/// @return the new java string
			//--------------------------------------------------------------------------------------
			static jstring CreateJStringFromSTDString(std::string instrString);
			//--------------------------------------------------------------------------------------
			/// Create STDString From JString
			///
			/// Takes a java string and converts it to a std::string
			///
			/// @param the java string
			/// @return the new std::string
			//--------------------------------------------------------------------------------------
			static std::string CreateSTDStringFromJString(jstring injstrString);
			//--------------------------------------------------------------------------------------
			/// Create STDString From JByteArray
			///
			/// Takes a java byte array and converts it to a std::string
			///
			/// @param the java byte array
			/// @return the new std::string
			//--------------------------------------------------------------------------------------
			static std::string CreateSTDStringFromJByteArray(jbyteArray injabyArray, int indwLength);
			//--------------------------------------------------------------------------------------
			/// Get Int Element From JArray
			///
			/// Returns the value at the requested index in the j-array
			///
			/// @param the java array.
			/// @param the index.
			/// @return the integer value.
			//--------------------------------------------------------------------------------------
			static s32 GetIntElementFromJArray(jintArray injadwArray, u32 inudwIndex);
		protected:
			//--------------------------------------------------------------------------------------
			/// Init Callable Static Method
			///
			/// Initialises a method such that it can be called easier later on. This also means
			/// That the performance hit from getting method IDs is not potentially taken at
			///	run-time.
			///
			/// @param std::string class name.
			/// @param std::string method name.
			/// @param std::string the method signature
			//--------------------------------------------------------------------------------------
			static void InitCallableStaticMethod(std::string instrClassName, std::string instrMethodName, std::string instrSignature);

			static std::map<std::string, MethodReference> mStaticMethodRefMap;
			static JavaVM* mspJavaVM;
		};
	}
}

#endif

#endif
