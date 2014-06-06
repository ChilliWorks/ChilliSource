/*
 *  JavaInterface.cpp
 *  moFlow
 *
 *  Created by Ian Copland on 09/08/2012.
 *  Copyright 2012 Tag Games. All rights reserved.
 *
 */

#ifdef CS_TARGETPLATFORM_ANDROID

#include <ChilliSource/Backend/Platform/Android/Core/JNI/JavaInterface.h>

#include <ChilliSource/Backend/Platform/Android/Core/JNI/JavaInterfaceManager.h>

namespace ChilliSource
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
					CS_LOG_FATAL("Cannot find native interface class '" + instrInterfaceName + "'");
				}

				//get the class instance
				jmethodID jConstructor = pEnv->GetMethodID(jClassType, "<init>", "()V");
				jobject jClassInstance = pEnv->NewObject(jClassType, jConstructor);
				if (jClassInstance == nullptr)
				{
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