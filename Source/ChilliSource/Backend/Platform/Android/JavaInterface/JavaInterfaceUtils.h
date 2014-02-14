/*
 *  JavaInterfaceUtils.h
 *  moFlow
 *
 *  Created by Ian Copland on 09/08/2012.
 *  Copyright 2012 Tag Games. All rights reserved.
 *
 */

#ifndef _MOFLOW_PLATFORM_ANDROID_JAVAINTERFACE_JAVAINTERFACEUTILS_H_
#define _MOFLOW_PLATFORM_ANDROID_JAVAINTERFACE_JAVAINTERFACEUTILS_H_

#include <jni.h>
#include <ChilliSource/Core/String/UTF8String.h>

namespace moFlo
{
	namespace AndroidPlatform
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
			/// Create Jstring From UTF8String
			///
			/// Converts a UTF8String into a JString.
			///
			/// @param the UTF8String
			/// @return the new java string
			//--------------------------------------------------------------------------------------
			jstring CreateJStringFromUTF8String(const UTF8String& instrString);
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
			/// Create UTF8String From JString
			///
			/// Takes a java string and converts it to a UTF8String
			///
			/// @param the java string
			/// @return the new UTF8String
			//--------------------------------------------------------------------------------------
			UTF8String CreateUTF8StringFromJString(jstring injstrString);
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
