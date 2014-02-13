/**
 * HttpConnectionJavaInterface.h
 * moFlow
 *
 * Created by Ian Copland on 17/03/2011
 * Copyright �2010 Tag Games Limited - All rights reserved
 */

//includes
#include <moFlo/Networking/HttpConnectionSystem.h>
#include <moFlo/Platform/Android/JavaInterface/HttpConnectionJavaInterface.h>
#include <moFlo/Platform/Android/JavaInterface/CoreJavaInterface.h>

namespace moFlo
{
	namespace AndroidPlatform
	{
		//--------------------------------------------------------------------------------------
		/// Setup Java Interface
		//--------------------------------------------------------------------------------------
		void SCHttpConnectionJavaInterface::SetupJavaInterface(JavaVM* inpJavaVM)
		{
			mspJavaVM = inpJavaVM;

			//Setup Java calls
			InitCallableStaticMethod("com/taggames/moflow/nativeinterface/CHttpConnectionNativeInterface","HttpRequest", "(Ljava/lang/String;ZLjava/lang/String;[I[Ljava/lang/String;[I[I)[B");
			InitCallableStaticMethod("com/taggames/moflow/nativeinterface/CHttpConnectionNativeInterface","HttpRequestWithHeaders", "(Ljava/lang/String;Z[Ljava/lang/String;[Ljava/lang/String;Ljava/lang/String;[I[Ljava/lang/String;[I[IZ)[B");
			InitCallableStaticMethod("com/taggames/moflow/nativeinterface/CHttpConnectionNativeInterface","IsConnected", "()Z");
		}
		//--------------------------------------------------------------------------------------
		/// Http Request
		//--------------------------------------------------------------------------------------
		HTTP_REQUEST_RESULT_CODE SCHttpConnectionJavaInterface::HttpRequest(const std::string& instrUrl, HTTP_REQUEST_TYPE ineRequestType,
				const std::string& instrBody, std::string& outstrResponse, std::string& outstrRedirect, s32& outdwResponseCode)
		{
			return HttpRequest(instrUrl, ineRequestType, Core::ParamDictionary(), instrBody, outstrResponse,outstrRedirect, outdwResponseCode, false);
		}
		//--------------------------------------------------------------------------------------
		/// Http Request
		//--------------------------------------------------------------------------------------
		HTTP_REQUEST_RESULT_CODE SCHttpConnectionJavaInterface::HttpRequest(const std::string& instrUrl, HTTP_REQUEST_TYPE ineRequestType,
				Core::ParamDictionary insHeaders, const std::string& instrBody,
				std::string& outstrResponse, std::string& outstrRedirect, s32& outdwResponseCode,
				bool inbKeepAlive)
		{
			HTTP_REQUEST_RESULT_CODE eOutput = HRRC_COULDNOTMAKEREQUEST;
			MethodReference sdMethodRef = GetStaticMethodReference("HttpRequestWithHeaders");
			if (sdMethodRef.mMethodID != 0 && sdMethodRef.mClassID != 0)
			{
				JNIEnv* env = GetJNIEnvironmentPtr();
				bool bIsPost = false;
				if (ineRequestType == HRT_POST)
					bIsPost = true;

				//Create an array out headers ParamDictionary as this is easier to pass to Java
				jstring jstrEmptyString = env->NewStringUTF("");
				jclass jStringClass = env->FindClass("java/lang/String");
				jobjectArray ajstrKey = env->NewObjectArray(insHeaders.size(), jStringClass, jstrEmptyString);
				jobjectArray ajstrValue = env->NewObjectArray(insHeaders.size(), jStringClass, jstrEmptyString);

				u32 udwParamCount = 0;
				for(Core::StringToStringMap::const_iterator it = insHeaders.begin(); it != insHeaders.end(); ++it)
				{
					jstring jstrFirst = CreateJStringFromSTDString(it->first);
					jstring jstrSecond = CreateJStringFromSTDString(it->second);
					env->SetObjectArrayElement(ajstrKey, udwParamCount, jstrFirst);
					env->SetObjectArrayElement(ajstrValue, udwParamCount, jstrSecond);
					env->DeleteLocalRef(jstrFirst);
					env->DeleteLocalRef(jstrSecond);
					udwParamCount++;
				}

				//create the strings
				jstring jstrUrl = CreateJStringFromSTDString(instrUrl);
				jstring jstrBody = CreateJStringFromSTDString(instrBody);
				jobjectArray jastrRedirect = env->NewObjectArray(1, jStringClass, jstrEmptyString);
				jintArray jadwResultLength = env->NewIntArray(1);
				jintArray jadwHttpResponseCode = env->NewIntArray(1);
				jintArray jadwResultCode = env->NewIntArray(1);

				//send the request
				jbyteArray jabyData = static_cast<jbyteArray>(env->CallStaticObjectMethod(sdMethodRef.mClassID, sdMethodRef.mMethodID,
						jstrUrl, bIsPost, ajstrKey, ajstrValue, jstrBody,
						jadwResultLength, jastrRedirect, jadwResultCode, jadwHttpResponseCode,
						inbKeepAlive));

				//get the result codes
				eOutput = (HTTP_REQUEST_RESULT_CODE)GetIntElementFromJArray(jadwResultCode, 0);
				outdwResponseCode = GetIntElementFromJArray(jadwHttpResponseCode, 0);

				//if successful, get the response
				if (eOutput == HRRC_SUCCESS)
				{
					int dwLength = GetIntElementFromJArray(jadwResultLength, 0);
					std::string strResponse = CreateSTDStringFromJByteArray(jabyData, dwLength);
					outstrResponse.append(strResponse);

					if(outdwResponseCode == moNetworking::kHTTPMovedTemporarily)
					{
						jstring jstrRedirect =(jstring)env->GetObjectArrayElement(jastrRedirect, 0);
						outstrRedirect = CreateSTDStringFromJString(jstrRedirect);
					}
				}

				//delete all local references
				env->DeleteLocalRef(jstrUrl);
				env->DeleteLocalRef(jstrBody);
				env->DeleteLocalRef(jastrRedirect);
				env->DeleteLocalRef(jadwResultLength);
				env->DeleteLocalRef(jadwHttpResponseCode);
				env->DeleteLocalRef(jadwResultCode);
				env->DeleteLocalRef(jabyData);
				env->DeleteLocalRef(ajstrKey);
				env->DeleteLocalRef(ajstrValue);
				env->DeleteLocalRef(jstrEmptyString);
				env->DeleteLocalRef(jStringClass);
			}

			return eOutput;
		}
		//--------------------------------------------------------------------------------------
		/// Is Connected
		//--------------------------------------------------------------------------------------
		bool SCHttpConnectionJavaInterface::IsConnected()
		{
			bool output = false;

			MethodReference sdMethodRef = GetStaticMethodReference("IsConnected");
			if (sdMethodRef.mMethodID != 0 && sdMethodRef.mClassID != 0)
			{
				JNIEnv* env = GetJNIEnvironmentPtr();

				//send the request
				output = static_cast<bool>(env->CallStaticBooleanMethod(sdMethodRef.mClassID, sdMethodRef.mMethodID));
			}

			return output;
		}
	}
}
