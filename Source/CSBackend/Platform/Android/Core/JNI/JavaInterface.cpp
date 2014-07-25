//
//  JavaInterface.cpp
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

#include <CSBackend/Platform/Android/Core/JNI/JavaInterface.h>

#include <CSBackend/Platform/Android/Core/JNI/JavaInterfaceManager.h>

namespace CSBackend
{
	namespace Android
	{
		//--------------------------------------------------------
		/// Constructor
		//--------------------------------------------------------
		IJavaInterface::IJavaInterface()
		:mpJavaObject(nullptr)
		{
		}
		//--------------------------------------------------------
		/// Destructor
		//--------------------------------------------------------
		IJavaInterface::~IJavaInterface()
		{
			if (mpJavaObject == nullptr)
			{
				JNIEnv* pEnv = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();

				pEnv->DeleteGlobalRef(mpJavaObject);
			}
		}
		//--------------------------------------------------------
		/// Create Native Interface
		//--------------------------------------------------------
		void IJavaInterface::CreateNativeInterface(const std::string& instrInterfaceName)
		{
			if (mpJavaObject == nullptr)
			{
				JNIEnv* pEnv = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();

				//get the class type
				jclass jClassType = pEnv->FindClass(instrInterfaceName.c_str());
				if (jClassType == nullptr)
				{
					jthrowable exception = pEnv->ExceptionOccurred();
					if (exception != nullptr)
					{
						pEnv->ExceptionDescribe();
						pEnv->ExceptionClear();
					}

					CS_LOG_FATAL("Cannot find native interface class '" + instrInterfaceName + "'");
				}

				//get the class instance
				jmethodID jConstructor = pEnv->GetMethodID(jClassType, "<init>", "()V");
				jobject jClassInstance = pEnv->NewObject(jClassType, jConstructor);
				if (jClassInstance == nullptr)
				{
					jthrowable exception = pEnv->ExceptionOccurred();
					if (exception != nullptr)
					{
						pEnv->ExceptionDescribe();
						pEnv->ExceptionClear();
					}

					CS_LOG_FATAL("Cannot create native interface instance '" + instrInterfaceName + "'");
				}

				//create a global reference to the instance
				mpJavaObject = pEnv->NewGlobalRef(jClassInstance);

				//delete all the local references
				pEnv->DeleteLocalRef(jClassType);
				pEnv->DeleteLocalRef(jClassInstance);
			}
			else
			{
				CS_LOG_FATAL("Cannot create native interface " + instrInterfaceName + ", as it has already been created!");
			}
		}
		//--------------------------------------------------------
		/// Create Method Reference
		//--------------------------------------------------------
		void IJavaInterface::CreateMethodReference(const std::string& instrMethodName, const std::string& instrMethodSignature)
		{
			if (mpJavaObject != nullptr)
			{
				JNIEnv* pEnv = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();

				//get the method reference
				jclass jClassType = pEnv->GetObjectClass(mpJavaObject);
				jmethodID jMethod = pEnv->GetMethodID(jClassType, instrMethodName.c_str(), instrMethodSignature.c_str());

				if (jMethod != nullptr)
				{
					//add it to the method list
					if (mMethodReferenceMap.find(instrMethodName) == mMethodReferenceMap.end())
					{
						mMethodReferenceMap[instrMethodName] = jMethod;
					}
					else
					{
						CS_LOG_FATAL("Could not add method reference '" + instrMethodName + "' - method name already exists.");
					}
				}
				else
				{
					CS_LOG_FATAL("Could not create method reference '" + instrMethodName + "'");
				}

				//delete local references
				pEnv->DeleteLocalRef(jClassType);
			}
			else
			{
				CS_LOG_FATAL("Cannot create method reference as the native interface has not been created!");
			}
		}
		//--------------------------------------------------------
		/// Get Java Object
		//--------------------------------------------------------
		jobject IJavaInterface::GetJavaObject() const
		{
			return mpJavaObject;
		}
		//--------------------------------------------------------
		/// Get Method ID
		//--------------------------------------------------------
		jmethodID IJavaInterface::GetMethodID(const std::string& instrMethodName) const
		{
			std::map<std::string, jmethodID>::const_iterator it = mMethodReferenceMap.find(instrMethodName);
			if (it != mMethodReferenceMap.end())
			{
				return it->second;
			}

			CS_LOG_FATAL("Could not find method reference '" + instrMethodName + "', was it created?");
			return nullptr;
		}
	}
}

#endif
