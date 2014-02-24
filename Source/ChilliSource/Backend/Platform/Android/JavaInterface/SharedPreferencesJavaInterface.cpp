/**
 * SharedPreferencesJavaInterface.cpp
 * moFlo
 *
 * Created by Ian Copland on 01/12/2011
 * Copyright �2011 Tag Games Limited - All rights reserved
 */

#include <ChilliSource/Platform/Android/JavaInterface/SharedPreferencesJavaInterface.h>

namespace ChilliSource
{
	namespace Android
	{
		//create static member

		//--------------------------------------------------------------------------------------
		/// Setup Java Interface
		//--------------------------------------------------------------------------------------
		void SCSharedPreferencesJavaInterface::SetupJavaInterface(JavaVM* inpJavaVM)
		{
			mspJavaVM = inpJavaVM;

			//Setup Java calls
			InitCallableStaticMethod("com/taggames/moflow/nativeinterface/CSharedPreferencesNativeInterface","KeyExists", "(Ljava/lang/String;Ljava/lang/String;)Z");
			InitCallableStaticMethod("com/taggames/moflow/nativeinterface/CSharedPreferencesNativeInterface","GetString", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");
			InitCallableStaticMethod("com/taggames/moflow/nativeinterface/CSharedPreferencesNativeInterface","SetString", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
			InitCallableStaticMethod("com/taggames/moflow/nativeinterface/CSharedPreferencesNativeInterface","GetBoolean", "(Ljava/lang/String;Ljava/lang/String;Z)Z");
			InitCallableStaticMethod("com/taggames/moflow/nativeinterface/CSharedPreferencesNativeInterface","SetBoolean", "(Ljava/lang/String;Ljava/lang/String;Z)V");
		}
		//-------------------------------------------------------------------
		/// Key Exists
		//-------------------------------------------------------------------
		bool SCSharedPreferencesJavaInterface::KeyExists(std::string instrName, std::string instrKey)
		{
			bool strOutput = false;
			MethodReference sdMethodRef = GetStaticMethodReference("KeyExists");
			if (sdMethodRef.mMethodID != 0 && sdMethodRef.mClassID != 0)
			{
				//convert std::strings to jstrings
				jstring jstrName = CreateJStringFromSTDString(instrName);
				jstring jstrKey = CreateJStringFromSTDString(instrKey);

				//call the java method
				JNIEnv* env = GetJNIEnvironmentPtr();
				strOutput = static_cast<bool>(env->CallStaticBooleanMethod(sdMethodRef.mClassID, sdMethodRef.mMethodID, jstrName, jstrKey));

				//delete the local references
				env->DeleteLocalRef(jstrName);
				env->DeleteLocalRef(jstrKey);
			}
			return strOutput;
		}
		//-------------------------------------------------------------------
		/// Get String
		//-------------------------------------------------------------------
		std::string SCSharedPreferencesJavaInterface::GetString(std::string instrName, std::string instrKey, std::string instrDefaultValue)
		{
			std::string strOutput = instrDefaultValue;
			MethodReference sdMethodRef = GetStaticMethodReference("GetString");
			if (sdMethodRef.mMethodID != 0 && sdMethodRef.mClassID != 0)
			{
				//convert std::strings to jstrings
				jstring jstrName = CreateJStringFromSTDString(instrName);
				jstring jstrKey = CreateJStringFromSTDString(instrKey);
				jstring jstrDefaultValue = CreateJStringFromSTDString(instrDefaultValue);

				//call the java method
				JNIEnv* env = GetJNIEnvironmentPtr();
				jstring jstrOutput = static_cast<jstring>(env->CallStaticObjectMethod(sdMethodRef.mClassID, sdMethodRef.mMethodID, jstrName, jstrKey, jstrDefaultValue));

				//convert the output to a std::string
				strOutput = CreateSTDStringFromJString(jstrOutput);

				//delete the local references
				env->DeleteLocalRef(jstrOutput);
				env->DeleteLocalRef(jstrName);
				env->DeleteLocalRef(jstrKey);
				env->DeleteLocalRef(jstrDefaultValue);
			}
			return strOutput;
		}
		//-------------------------------------------------------------------
		/// Set String
		//-------------------------------------------------------------------
		void SCSharedPreferencesJavaInterface::SetString(std::string instrName, std::string instrKey, std::string instrValue)
		{
			MethodReference sdMethodRef = GetStaticMethodReference("SetString");
			if (sdMethodRef.mMethodID != 0 && sdMethodRef.mClassID != 0)
			{
				//convert std::strings to jstrings
				jstring jstrName = CreateJStringFromSTDString(instrName);
				jstring jstrKey = CreateJStringFromSTDString(instrKey);
				jstring jstrValue = CreateJStringFromSTDString(instrValue);

				//call the java method
				JNIEnv* env = GetJNIEnvironmentPtr();
				env->CallStaticVoidMethod(sdMethodRef.mClassID, sdMethodRef.mMethodID, jstrName, jstrKey, jstrValue);

				//delete the local references
				env->DeleteLocalRef(jstrName);
				env->DeleteLocalRef(jstrKey);
				env->DeleteLocalRef(jstrValue);
			}
		}
		//-------------------------------------------------------------------
		/// Get Boolean
		//-------------------------------------------------------------------
		bool SCSharedPreferencesJavaInterface::GetBoolean(std::string instrName, std::string instrKey, bool inbDefaultValue)
		{
			bool bOutput = inbDefaultValue;
			MethodReference sdMethodRef = GetStaticMethodReference("GetBoolean");
			if (sdMethodRef.mMethodID != 0 && sdMethodRef.mClassID != 0)
			{
				//convert std::strings to jstrings
				jstring jstrName = CreateJStringFromSTDString(instrName);
				jstring jstrKey = CreateJStringFromSTDString(instrKey);

				//call the java method
				JNIEnv* env = GetJNIEnvironmentPtr();
				bOutput = static_cast<bool>(env->CallStaticBooleanMethod(sdMethodRef.mClassID, sdMethodRef.mMethodID, jstrName, jstrKey, inbDefaultValue));

				//delete the local references
				env->DeleteLocalRef(jstrName);
				env->DeleteLocalRef(jstrKey);
			}
			return bOutput;
		}
		//-------------------------------------------------------------------
		/// Set Boolean
		//-------------------------------------------------------------------
		void SCSharedPreferencesJavaInterface::SetBoolean(std::string instrName, std::string instrKey, bool inbValue)
		{
			MethodReference sdMethodRef = GetStaticMethodReference("SetBoolean");
			if (sdMethodRef.mMethodID != 0 && sdMethodRef.mClassID != 0)
			{
				//convert std::strings to jstrings
				jstring jstrName = CreateJStringFromSTDString(instrName);
				jstring jstrKey = CreateJStringFromSTDString(instrKey);

				//call the java method
				JNIEnv* env = GetJNIEnvironmentPtr();
				env->CallStaticVoidMethod(sdMethodRef.mClassID, sdMethodRef.mMethodID, jstrName, jstrKey, inbValue);

				//delete the local references
				env->DeleteLocalRef(jstrName);
				env->DeleteLocalRef(jstrKey);
			}
		}

	}
}
