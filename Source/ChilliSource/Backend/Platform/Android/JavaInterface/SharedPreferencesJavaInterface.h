/**
 * SharedPreferencesJavaInterface.h
 * moFlo
 *
 * Created by Ian Copland on 01/12/2011
 * Copyright �2011 Tag Games Limited - All rights reserved
 */

#ifndef _MOFLO_PLATFORM_ANDROID_INPUT_SHAREDPREFERENCES_INTERFACE_JAVA_INTERFACE_
#define _MOFLO_PLATFORM_ANDROID_INPUT_SHAREDPREFERENCES_INTERFACE_JAVA_INTERFACE_

#include <jni.h>
#include <ChilliSource/Platform/Android/JavaInterface/_JavaInterface.h>

namespace moFlo
{
	namespace AndroidPlatform
	{
		class SCSharedPreferencesJavaInterface : public _IJavaInterface
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
