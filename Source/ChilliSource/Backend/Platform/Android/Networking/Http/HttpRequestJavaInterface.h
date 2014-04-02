//
//  HttpRequestJavaInterface.h
//  Chilli Source
//
//  Created by Ian Copland on 09/11/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#ifndef _CHILLISOURCE_BACKEND_PLATFORM_ANDROID_HTTP_HTTPREQUESTJAVAINTERFACE_H_
#define _CHILLISOURCE_BACKEND_PLATFORM_ANDROID_HTTP_HTTPREQUESTJAVAINTERFACE_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Backend/Platform/Android/Core/JNI/_JavaInterface.h>

#include <jni.h>

namespace ChilliSource
{
	namespace Android
	{
		//--------------------------------------------------------------------------------------
		/// An enum for the different response codes from the android
		/// HttpRequest.
		///
		/// @author I Copland
		//--------------------------------------------------------------------------------------
		enum class HttpRequestResultCode
		{
			k_success,
			k_couldNotConnect,
			k_timeout,
			k_couldNotMakeRequest
		};

		//--------------------------------------------------------------------------------------
		/// An enum for the types of http request
		///
		/// @author I Copland
		//--------------------------------------------------------------------------------------
		enum class HttpRequestType
		{
			k_get,
			k_post
		};
		//--------------------------------------------------------------------------------------
		/// Java interface for communication between the Http Request system and the Java
		/// HttpURLConnection library
		///
		/// @author I Copland
		//--------------------------------------------------------------------------------------
		class HttpRequestJavaInterface final : public _IJavaInterface
		{
		public:
			//--------------------------------------------------------------------------------------
			/// Sets up the java interface: stores the Virtual Machine, and initialises any java
			/// methods callable from native.
			///
			/// @author I Copland
			///
			/// @param a pointer to the virtual machine
			//--------------------------------------------------------------------------------------
			static void SetupJavaInterface(JavaVM* in_javaVM);
			//--------------------------------------------------------------------------------------
			/// Set the connection timeout time in seconds
			///
			/// @author S Downie
			///
			/// @param Timeout in seconds
			//--------------------------------------------------------------------------------------
			static void SetConnectionTimeout(u32 in_timeoutSecs);
			//--------------------------------------------------------------------------------------
			/// Sends a HTTP request using java "HttpURLConnection" system. This handles persistent
			/// connections and SSL.
			///
			/// @author I Copland
			///
			/// @param The url
			/// @param The request type
			/// @param Dictionary of key value headers
			/// @param The post body. Not used if type is not post
			/// @param [Out] Parameter containing the response data as a string.
			/// @param [Out] URL of any redirection
			/// @param [Out] Http response code
			///
			/// @return The result code.
			//--------------------------------------------------------------------------------------
			static HttpRequestResultCode MakeHttpRequest(const std::string& in_url, HttpRequestType in_type, const Core::ParamDictionary& in_headers, const std::string& in_body,
													std::string& out_response, std::string& out_redirectUrl, s32& out_reponseCode);
			//--------------------------------------------------------------------------------------
			/// Calls into java to find whether or not the device is currently connected to the
			/// internet
			///
			/// @author I Copland
			///
			/// @return whether or not it's connected.
			//--------------------------------------------------------------------------------------
			static bool IsConnected();
		};
	}
}

#endif
