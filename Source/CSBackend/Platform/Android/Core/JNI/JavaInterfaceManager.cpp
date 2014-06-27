//
//  JavaInterfaceManager.cpp
//  Chilli Source
//  Created by Ian Copland on 08/08/2012.
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

#include <CSBackend/Platform/Android/Core/JNI/JavaInterfaceManager.h>

namespace CSBackend
{
	namespace Android
	{
		JavaInterfaceManager* JavaInterfaceManager::mpJavaInterfaceManagerSingleton = nullptr;

		//-------------------------------------------------------
		/// Get Singleton Ptr
		//-------------------------------------------------------
		JavaInterfaceManager* JavaInterfaceManager::GetSingletonPtr()
		{
			if (mpJavaInterfaceManagerSingleton == nullptr)
			{
				mpJavaInterfaceManagerSingleton = new JavaInterfaceManager();
			}

			return mpJavaInterfaceManagerSingleton;
		}
		//--------------------------------------------------------
		/// Get Singleton
		//--------------------------------------------------------
		JavaInterfaceManager& JavaInterfaceManager::GetSingleton()
		{
			return *GetSingletonPtr();
		}
		//--------------------------------------------------------
		/// Constructor
		//--------------------------------------------------------
		JavaInterfaceManager::JavaInterfaceManager()
		 : mpJavaVM(nullptr)
		{
		}
		//--------------------------------------------------------
		/// Initialise
		//--------------------------------------------------------
		void JavaInterfaceManager::Initialise(JavaVM* inpJavaVM)
		{
			mpJavaVM = inpJavaVM;
		}
		//--------------------------------------------------------
		/// Get Java VM
		//--------------------------------------------------------
		JavaVM* JavaInterfaceManager::GetJavaVM()
		{
			return mpJavaVM;
		}
		//--------------------------------------------------------
		/// Get JNI Environment Ptr
		//--------------------------------------------------------
		JNIEnv* JavaInterfaceManager::GetJNIEnvironmentPtr()
		{
			if (mpJavaVM != nullptr)
			{
				JNIEnv* env;
				mpJavaVM->AttachCurrentThread(&env, nullptr);
				return env;
			}

			return nullptr;
		}
		//--------------------------------------------------------
		/// Add Java Interface
		//--------------------------------------------------------
		void JavaInterfaceManager::AddJavaInterface(IJavaInterfaceSPtr inpJavaInterface)
		{
			mJavaInterfaces.push_back(inpJavaInterface);
		}
		//--------------------------------------------------------
		/// Get Java Interface
		//--------------------------------------------------------
		IJavaInterfaceSPtr JavaInterfaceManager::GetJavaInterface(CSCore::InterfaceIDType inInterfaceID)
		{
			for (std::vector<IJavaInterfaceSPtr>::const_iterator it = mJavaInterfaces.begin(); it != mJavaInterfaces.end(); ++it)
			{
				if ((*it)->IsA(inInterfaceID))
				{
					return (*it);
				}
			}

			CS_LOG_WARNING("JavaInterfaceManager cannot find a JavaInterface with the requested interface.");
			return IJavaInterfaceSPtr();
		}
		//--------------------------------------------------------
		/// Get Java Interfaces
		//--------------------------------------------------------
		void JavaInterfaceManager::GetJavaInterfaces(CSCore::InterfaceIDType inInterfaceID, std::vector<IJavaInterfaceSPtr>& outJavaInterfaces)
		{
			outJavaInterfaces.clear();
			for (std::vector<IJavaInterfaceSPtr>::const_iterator it = mJavaInterfaces.begin(); it != mJavaInterfaces.end(); ++it)
			{
				if ((*it)->IsA(inInterfaceID))
				{
					outJavaInterfaces.push_back((*it));
				}
			}
		}
		//--------------------------------------------------------
		/// Destructor
		//--------------------------------------------------------
		JavaInterfaceManager::~JavaInterfaceManager()
		{
			mJavaInterfaces.clear();
		}
	}
}

#endif
