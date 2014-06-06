/**
 * Author: Ian Copland
 * Date: 17/03/2011
 * Description: A interface class for
 */

#ifdef CS_TARGETPLATFORM_ANDROID

//includes
#include <CSBackend/Platform/Android/Core/JNI/_JavaInterface.h>

namespace ChilliSource
{
	namespace Android
	{
		//declare private statics
		std::map<std::string, MethodReference> _IJavaInterface::mStaticMethodRefMap;
		JavaVM* _IJavaInterface::mspJavaVM = nullptr;

		//--------------------------------------------------------------------------------------
		/// Get JNI Environment Ptr
		//--------------------------------------------------------------------------------------
		JNIEnv * _IJavaInterface::GetJNIEnvironmentPtr()
		{
			JNIEnv* env;
			mspJavaVM->AttachCurrentThread(&env, nullptr);
			return env;
		}
		//--------------------------------------------------------------------------------------
		/// Get Java VM
		///
		/// @return VM
		//--------------------------------------------------------------------------------------
		JavaVM* _IJavaInterface::GetJavaVM()
		{
			return mspJavaVM;
		}
		//--------------------------------------------------------------------------------------
		/// Init Callable Static Method
		//--------------------------------------------------------------------------------------
		void _IJavaInterface::InitCallableStaticMethod(std::string instrClassName, std::string instrMethodName, std::string instrSignature)
		{
			JNIEnv* pEnv = GetJNIEnvironmentPtr();

			MethodReference ref;
			jclass javaInterfaceClass = pEnv->FindClass(instrClassName.c_str());
			ref.mClassID = static_cast<jclass>(pEnv->NewGlobalRef(javaInterfaceClass));
			ref.mMethodID = pEnv->GetStaticMethodID(ref.mClassID, instrMethodName.c_str(), instrSignature.c_str());
			pEnv->DeleteLocalRef(javaInterfaceClass);

			if (ref.mMethodID == 0 || ref.mClassID == 0)
				CS_LOG_ERROR("Could not get method and class ID\nClassname \""+instrClassName+"\"\nMethodname \""+instrMethodName+"\"\nSigniture \""+instrSignature+"\"");

			if (mStaticMethodRefMap.find(instrMethodName) == mStaticMethodRefMap.end())
				mStaticMethodRefMap[instrMethodName] = ref;
			else
				CS_LOG_ERROR("Could not add method reference - method name already exists.");
		}

		//--------------------------------------------------------------------------------------
		/// Get Static Method Reference
		//--------------------------------------------------------------------------------------
		MethodReference _IJavaInterface::GetStaticMethodReference(std::string instrMethodName)
		{
			MethodReference ref;
			ref.mClassID = 0;
			ref.mMethodID = 0;

			if (mStaticMethodRefMap.find(instrMethodName) != mStaticMethodRefMap.end())
				ref = mStaticMethodRefMap[instrMethodName];
			else
				CS_LOG_ERROR("Could not find method reference. Have you initialised it in the Java Interface?");

			return ref;
		}
		//--------------------------------------------------------------------------------------
		/// Create JString From STDString
		//--------------------------------------------------------------------------------------
		jstring _IJavaInterface::CreateJStringFromSTDString(std::string instrString)
		{
			JNIEnv* pEnv = GetJNIEnvironmentPtr();
			if (pEnv != nullptr)
			{
				jstring jstrNewString = pEnv->NewStringUTF(instrString.c_str());
				return jstrNewString;
			}
			return nullptr;
		}
		//--------------------------------------------------------------------------------------
		/// Create STDString From JString
		//--------------------------------------------------------------------------------------
		std::string _IJavaInterface::CreateSTDStringFromJString(jstring injstrString)
		{
			JNIEnv* pEnv = GetJNIEnvironmentPtr();
			if (pEnv != nullptr && injstrString != nullptr)
			{
				const char * cString = pEnv->GetStringUTFChars(injstrString, JNI_FALSE);
				if (cString != nullptr)
				{
					std::string stdString = std::string(cString);
					pEnv->ReleaseStringUTFChars(injstrString, cString);
					return stdString;
				}
			}
			return "";
		}
		//--------------------------------------------------------------------------------------
		/// Create STDString From JByteArray
		//--------------------------------------------------------------------------------------
		std::string _IJavaInterface::CreateSTDStringFromJByteArray(jbyteArray injabyArray, int indwLength)
		{
			if (injabyArray != nullptr && indwLength > 0)
			{
				JNIEnv* pEnv = GetJNIEnvironmentPtr();
				if (pEnv != nullptr)
				{
					jbyte* pBytes = pEnv->GetByteArrayElements(injabyArray, JNI_FALSE);
					if (pBytes != nullptr)
					{
						std::string strOutput((const char*)pBytes, indwLength);
						pEnv->ReleaseByteArrayElements(injabyArray, pBytes, 0);
						return strOutput;
					}
				}
			}

			return "";
		}
		//--------------------------------------------------------------------------------------
		/// Get Int Element From JArray
		//--------------------------------------------------------------------------------------
		s32 _IJavaInterface::GetIntElementFromJArray(jintArray injadwArray, u32 inudwIndex)
		{
			JNIEnv* pEnv = GetJNIEnvironmentPtr();
			if (pEnv != nullptr && injadwArray != nullptr)
			{
				jint* pIntegers = pEnv->GetIntArrayElements(injadwArray, JNI_FALSE);
				if (pIntegers != nullptr)
				{
					s32 dwOutput = pIntegers[inudwIndex];
					pEnv->ReleaseIntArrayElements(injadwArray, pIntegers, 0);
					return dwOutput;
				}
			}
			return -1;
		}
	}
}

#endif
