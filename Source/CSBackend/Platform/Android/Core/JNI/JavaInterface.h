/*
 *  JavaInterface.h
 *  moFlow
 *
 *  Created by Ian Copland on 09/08/2012.
 *  Copyright 2012 Tag Games. All rights reserved.
 *
 */

#ifdef CS_TARGETPLATFORM_ANDROID

#ifndef _CHILLISOURCE_PLATFORM_ANDROID_JAVAINTERFACE_JAVAINTERFACE_H_
#define _CHILLISOURCE_PLATFORM_ANDROID_JAVAINTERFACE_JAVAINTERFACE_H_

#include <CSBackend/Platform/Android/ForwardDeclarations.h>
#include <ChilliSource/Core/Base/QueryableInterface.h>

#include <jni.h>

#include <map>

namespace CSBackend
{
	namespace Android
	{
		//========================================================
		/// Java Interface Manager
		///
		/// Handles all of the java interfaces and provides an
		/// interface to access them.
		//========================================================
		class IJavaInterface : public CSCore::QueryableInterface
		{
		public:
			//--------------------------------------------------------
			/// Constructor
			//--------------------------------------------------------
			IJavaInterface();
			//--------------------------------------------------------
			/// Destructor
			//--------------------------------------------------------
			virtual ~IJavaInterface();
		protected:
			//--------------------------------------------------------
			/// Create Native Interface
			///
			/// Creates the Native Interface on the Java side.
			///
			/// @param the name of the native interface.
			//--------------------------------------------------------
			void CreateNativeInterface(const std::string& instrInterfaceName);
			//--------------------------------------------------------
			/// Create Method Reference
			///
			/// Creates a reference to a method in the created native
			/// interface.
			///
			/// @param the name of the native interface.
			//--------------------------------------------------------
			void CreateMethodReference(const std::string& instrMethodName, const std::string& instrMethodSignature);
			//--------------------------------------------------------
			/// Get Java Object
			///
			/// @return the Java Object
			//--------------------------------------------------------
			jobject GetJavaObject() const;
			//--------------------------------------------------------
			/// Get Method ID
			///
			/// @param the method name
			/// @return the method id.
			//--------------------------------------------------------
			jmethodID GetMethodID(const std::string& instrMethodName) const;
		private:

			jobject mpJavaObject;
			std::map<std::string, jmethodID> mMethodReferenceMap;
		};
	}
}

#endif

#endif