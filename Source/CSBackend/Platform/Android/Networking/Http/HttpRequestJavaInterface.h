//
//  HttpRequestJavaInterface.h
//  Chilli Source
//  Created by Ian Copland on 09/11/2011.
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

#ifndef _CSBACKEND_PLATFORM_ANDROID_HTTP_HTTPREQUESTJAVAINTERFACE_H_
#define _CSBACKEND_PLATFORM_ANDROID_HTTP_HTTPREQUESTJAVAINTERFACE_H_

#include <ChilliSource/ChilliSource.h>
#include <CSBackend/Platform/Android/ForwardDeclarations.h>
#include <CSBackend/Platform/Android/Core/JNI/_JavaInterface.h>

#include <jni.h>

#include <mutex>
#include <unordered_map>

namespace CSBackend
{
	namespace Android
	{
		//--------------------------------------------------------------------------------------
		/// Java interface for communication between the Http Request system and the Java
		/// HttpURLConnection library
		///
		/// @author Ian Copland
		//--------------------------------------------------------------------------------------
		class HttpRequestJavaInterface final : public _IJavaInterface
		{
		public:
			//--------------------------------------------------------------------------------------
			/// An enum for the different response codes from the android
			/// HttpRequest.
			///
			/// @author Ian Copland
			//--------------------------------------------------------------------------------------
			enum class RequestResultCode
			{
				k_success,
				k_couldNotConnect,
				k_timeout,
				k_couldNotMakeRequest
			};

			//--------------------------------------------------------------------------------------
			/// An enum for the types of http request
			///
			/// @author Ian Copland
			//--------------------------------------------------------------------------------------
			enum class RequestType
			{
				k_get,
				k_post
			};
			//--------------------------------------------------------------------------------------
			/// Sets up the java interface: stores the Virtual Machine, and initialises any java
			/// methods callable from native.
			///
			/// @author Ian Copland
			///
			/// @param a pointer to the virtual machine
			//--------------------------------------------------------------------------------------
			static void SetupJavaInterface(JavaVM* in_javaVM);
			//--------------------------------------------------------------------------------------
			/// Sends a HTTP request using java "HttpURLConnection" system. This handles persistent
			/// connections and SSL.
			///
			/// @author Ian Copland
			///
			/// @param The request object used for callbacks
			/// @param The url
			/// @param The request type
			/// @param Dictionary of key value headers
			/// @param The post body. Not used if type is not post
			/// @param Connection timeout in seconds
			/// @param Max buffer size in bytes
			/// @param [Out] Parameter containing the response data as a string.
			/// @param [Out] Http response code
			///
			/// @return The result code.
			//--------------------------------------------------------------------------------------
			static RequestResultCode MakeHttpRequest(HttpRequest* in_request, const std::string& in_url, RequestType in_type, const CSCore::ParamDictionary& in_headers, const std::string& in_body,
					s32 in_timeout, s32 in_maxBufferSize,
					std::string& out_response, s32& out_reponseCode);
			//--------------------------------------------------------------------------------------
			/// Calls into java to find whether or not the device is currently connected to the
			/// internet
			///
			/// @author Ian Copland
			///
			/// @return whether or not it's connected.
			//--------------------------------------------------------------------------------------
			static bool IsConnected();
			//--------------------------------------------------------------------------------------
			/// Called by Java when the request contents exceed the max buffer size and are flushed
			///
			/// @author S Downie
			///
			/// @param Partial data
			/// @param Response code
			/// @param Request Id of request to notify
			//--------------------------------------------------------------------------------------
			static void OnFlushed(const std::string& in_data, u32 in_responseCode, s32 in_requestId);

			static s32 s_requestIdCounter;
			static std::unordered_map<s32, HttpRequest*> s_requestMap;
			static std::mutex s_requestMutex;
		};
	}
}

#endif

#endif
