//
//  SharedPreferencesJavaInterface.h
//  Chilli Source
//  Created by Ian Copland on 01/12/2011.
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

#ifndef _CHILLISOURCE_PLATFORM_ANDROID_INPUT_SHAREDPREFERENCES_INTERFACE_JAVA_INTERFACE_
#define _CHILLISOURCE_PLATFORM_ANDROID_INPUT_SHAREDPREFERENCES_INTERFACE_JAVA_INTERFACE_

#include <CSBackend/Platform/Android/Core/JNI/_JavaInterface.h>

#include <jni.h>

namespace CSBackend
{
	namespace Android
	{
		class SharedPreferencesJavaInterface : public _IJavaInterface
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
			//-------------------------------------------------------------------
			/// Key Exists
			///
			/// @param the shared preferences name.
			/// @param the key.
			/// @return whether or not the given key is available in the specified
			//			shared preferences file.
			//-------------------------------------------------------------------
			static bool KeyExists(std::string instrName, std::string instrKey);
			//-------------------------------------------------------------------
			/// Get String
			///
			/// @param the shared preferences name.
			/// @param the key.
			/// @param default value to return if the key doesnt exist.
			/// @return the value for the given key.
			//-------------------------------------------------------------------
			static std::string GetString(std::string instrName, std::string instrKey, std::string instrDefaultValue);
			//-------------------------------------------------------------------
			/// Set String
			///
			/// @param the shared preferences name.
			/// @param the key.
			/// @param The value to write
			//-------------------------------------------------------------------
			static void SetString(std::string instrName, std::string instrKey, std::string instrValue);
			//-------------------------------------------------------------------
			/// Get Boolean
			///
			/// @param the shared preferences name.
			/// @param the key.
			/// @param default value to return if the key doesnt exist.
			/// @return the value for the given key.
			//-------------------------------------------------------------------
			static bool GetBoolean(std::string instrName, std::string instrKey, bool inbDefaultValue);
			//-------------------------------------------------------------------
			/// Set Boolean
			///
			/// @param the shared preferences name.
			/// @param the key.
			/// @param The value to write
			//-------------------------------------------------------------------
			static void SetBoolean(std::string instrName, std::string instrKey, bool inbValue);
		private:

		};
	}
}

#endif

#endif
