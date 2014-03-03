/**
 * HttpConnectionJavaInterface.h
 * moFlow
 *
 * Created by Ian Copland on 09/11/2011
 * Copyright �2010 Tag Games Limited - All rights reserved
 */

#ifndef _MOFLO_PLATFORM_ANDROID_INPUT_HTTPCONNECTION_INTERFACE_JAVA_INTERFACE_
#define _MOFLO_PLATFORM_ANDROID_INPUT_HTTPCONNECTION_INTERFACE_JAVA_INTERFACE_

#include <ChilliSource/Backend/Platform/Android/Core/JNI/_JavaInterface.h>
#include <ChilliSource/Core/Container/ParamDictionary.h>

#include <jni.h>

#include <map>

namespace ChilliSource
{
	namespace Android
	{
		//===========================================================
		/// Http Request Error Code
		///
		/// An enum for the different response codes from the android
		/// HttpRequest.
		//===========================================================
		enum class HttpRequestResultCode
		{
			k_success,
			k_couldNotConnect,
			k_timeout,
			k_couldNotMakeRequest
		};

		//===========================================================
		/// Http Request Type
		///
		/// An enum for the types of http request
		//===========================================================
		enum class HttpRequestType
		{
			k_get,
			k_post
		};

		class SCHttpConnectionJavaInterface : public _IJavaInterface
		{
		public:
			//--------------------------------------------------------------------------------------
			/// Setup Java Interface
			///
			/// Sets up the java interface: stores the Virtual Machine, and Initialises any java
			/// methods callable from native.
			///
			/// @param a pointer to the virtual machine
			//--------------------------------------------------------------------------------------
			static void SetupJavaInterface(JavaVM* inpJavaVM);
			//--------------------------------------------------------------------------------------
			/// Http Request
			///
			/// Sends a HTTP request using java "HttpURLConnection" system. This handles persistant
			/// connections and SSL.
			///
			/// @param the url
			/// @param the request type
			/// @param The post body. Not used if inbIsPost is false
			/// @param Output parameter containing the response string.
			/// @return The response code.
			//--------------------------------------------------------------------------------------
			static HttpRequestResultCode HttpRequest(const std::string& instrUrl, HttpRequestType ineRequestType,
														const std::string& instrBody, std::string& outstrResponse, std::string& outstrRedirect, s32& outdwResponseCode);
			//--------------------------------------------------------------------------------------
			/// Http Request
			///
			/// Sends a HTTP request using java "HttpURLConnection" system. This handles persistant
			/// connections and SSL.
			///
			/// @param The url
			/// @param The request type
			/// @param The headers as key/value pair
			/// @param The post body. Not used if inbIsPost is false
			/// @param Output parameter containing the response string.
			/// @param Whether or not to use a persistent connection
			/// @return The response code.
			//--------------------------------------------------------------------------------------
			static HttpRequestResultCode HttpRequest(const std::string& instrUrl, HttpRequestType ineRequestType,
														Core::ParamDictionary insHeaders, const std::string& instrBody,
														std::string& outstrResponse, std::string& outstrRedirect, s32& outdwResponseCode,
														bool inbKeepAlive);
			//--------------------------------------------------------------------------------------
			/// Is Connected
			///
			/// Calls into java to find whether or not the device is currently connected to the
			/// internet
			///
			/// @return whether or not it's connected.
			//--------------------------------------------------------------------------------------
			static bool IsConnected();
		private:

		};
	}
}

#endif
