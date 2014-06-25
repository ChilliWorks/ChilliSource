//
//  JavaInterface.h
//  Chilli Source
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
