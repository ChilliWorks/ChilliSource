/**
 * File: JavaInterface.h
 * Date: 1 Oct 2010
 * Description: Interface functions for calls to and from java.
 */

/**
 *
 * Author: Ian Copland
 * Version 1.0 - moFlo
 * Copyright �2010 Tag Games Limited - All rights reserved
 */

#ifndef _MOFLOW_PLATFORM_ANDROID_JAVAINTERFACE__JAVAINTERFACE_
#define _MOFLOW_PLATFORM_ANDROID_JAVAINTERFACE__JAVAINTERFACE_

#include <jni.h>
#include <map>
#include <ChilliSource/Core/String/UTF8String.h>

namespace ChilliSource
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
			/// Create Jstring From UTF8String
			///
			/// Converts a UTF8String into a JString.
			///
			/// @param the UTF8String
			/// @return the new java string
			//--------------------------------------------------------------------------------------
			static jstring CreateJStringFromUTF8String(UTF8String instrString);
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
			/// Create UTF8String From JString
			///
			/// Takes a java string and converts it to a UTF8String
			///
			/// @param the java string
			/// @return the new UTF8String
			//--------------------------------------------------------------------------------------
			static UTF8String CreateUTF8StringFromJString(jstring injstrString);
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
