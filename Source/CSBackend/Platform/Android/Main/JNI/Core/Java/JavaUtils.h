//
//  JavaUtils.h
//  ChilliSource
//  Created by Ian Copland on 09/08/2012.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2012 Tag Games Limited
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

#ifndef _CSBACKEND_PLATFORM_ANDROID_MAIN_JNI_CORE_JAVA_JAVAUTILS_H_
#define _CSBACKEND_PLATFORM_ANDROID_MAIN_JNI_CORE_JAVA_JAVAUTILS_H_

#include <ChilliSource/ChilliSource.h>

#include <CSBackend/Platform/Android/Main/JNI/ForwardDeclarations.h>

#include <jni.h>

#include <string>

namespace CSBackend
{
	namespace Android
	{
	    //------------------------------------------------------------------------------
	    /// A series of convenience functions for working with Java from native.
	    ///
	    /// @author Ian Copland
	    //------------------------------------------------------------------------------
		namespace JavaUtils
		{
			//------------------------------------------------------------------------------
			/// Converts a std string into a JString. The JString will need to be manually
			/// released with ReleaseLocalRef() once it has been finished with.
			///
			/// @author Ian Copland
			///
			/// @param in_string - the std::string
			///
			/// @return The new java string
			//------------------------------------------------------------------------------
			jstring CreateJStringFromSTDString(const std::string& in_string);
			//------------------------------------------------------------------------------
			/// Takes a java string and converts it to a std::string. If a null string is
			/// passed in, an empty string will be returned.
			///
			/// @author Ian Copland
			///
			/// @param in_jString - The java string
			///
			/// @return The new std::string
			//------------------------------------------------------------------------------
			std::string CreateSTDStringFromJString(jstring in_jString);
			//------------------------------------------------------------------------------
			/// Takes a java byte array and converts it to a std::string. If a null array
			/// or an array of size 0 is passed in, an empty string is returned.
			///
			/// @author Ian Copland
			///
			/// @param in_array - the java byte array
			/// @param in_length - The length of the java byte array.
			///
			/// @return The new std::string
			//------------------------------------------------------------------------------
			std::string CreateSTDStringFromJByteArray(jbyteArray in_array, u32 in_length);
			//------------------------------------------------------------------------------
			/// Returns the value at the requested index in the j-array
			///
			/// @author Ian Copland
			///
			/// @param in_array - the java array.
			/// @param in_index - the index.
			///
			/// @return The integer value.
			//------------------------------------------------------------------------------
			s32 GetIntElementFromJArray(jintArray in_array, u32 in_index);
			//------------------------------------------------------------------------------
			/// Checks for java exceptions. If an exception has occurred it is treated as
			/// a fatal error and the application will be terminated.
			///
			/// This rarely needs to be called manually as JavaClass and JavaStaticClass
			/// both call it internally immediately after every java call.
			///
			/// @author Ian Copland
			///
			/// @param in_errorMessage - The error message to print if an exception occurred.
			//------------------------------------------------------------------------------
			void CheckJavaExceptions(const std::string& in_errorMessage);
			//------------------------------------------------------------------------------
			/// Deletes a local reference. This is provided for convenience to save having
			/// to get the java environment when a reference was created through the other
			/// convenience methods provided here.
			///
			/// @author Ian Copland
			///
			/// @param in_localReference - The local java reference which should be deleted.
			//------------------------------------------------------------------------------
			void DeleteLocalRef(jobject in_localReference);
		}
	}
}

#endif

#endif
