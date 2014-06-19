/*
 *  JavaInterfaceUtils.h
 *  moFlow
 *
 *  Created by Ian Copland on 09/08/2012.
 *  Copyright 2012 Tag Games. All rights reserved.
 *
 */

#ifdef CS_TARGETPLATFORM_ANDROID

#ifndef _CHILLISOURCE_PLATFORM_ANDROID_JAVAINTERFACE_JAVAINTERFACEUTILS_H_
#define _CHILLISOURCE_PLATFORM_ANDROID_JAVAINTERFACE_JAVAINTERFACEUTILS_H_

#include <ChilliSource/ChilliSource.h>

#include <jni.h>

#include <string>

namespace CSBackend
{
	namespace Android
	{
		namespace JavaInterfaceUtils
		{
			//--------------------------------------------------------------------------------------
			/// Create JString From STDString
			///
			/// Converts a std string into a JString.
			///
			/// @param the std::string
			/// @return the new java string
			//--------------------------------------------------------------------------------------
			jstring CreateJStringFromSTDString(const std::string& instrString);
			//--------------------------------------------------------------------------------------
			/// Create STDString From JString
			///
			/// Takes a java string and converts it to a std::string
			///
			/// @param the java string
			/// @return the new std::string
			//--------------------------------------------------------------------------------------
			std::string CreateSTDStringFromJString(jstring injstrString);
			//--------------------------------------------------------------------------------------
			/// Create STDString From JByteArray
			///
			/// Takes a java byte array and converts it to a std::string
			///
			/// @param the java byte array
			/// @return the new std::string
			//--------------------------------------------------------------------------------------
			std::string CreateSTDStringFromJByteArray(jbyteArray injabyArray, int indwLength);
			//--------------------------------------------------------------------------------------
			/// Get Int Element From JArray
			///
			/// Returns the value at the requested index in the j-array
			///
			/// @param the java array.
			/// @param the index.
			/// @return the integer value.
			//--------------------------------------------------------------------------------------
			s32 GetIntElementFromJArray(jintArray injadwArray, u32 inudwIndex);
		}
	}
}

#endif

#endif