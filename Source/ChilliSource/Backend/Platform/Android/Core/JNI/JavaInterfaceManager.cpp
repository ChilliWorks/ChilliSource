/*
 *  JavaInterfaceManager.cpp
 *  moFlow
 *
 *  Created by Ian Copland on 08/08/2012.
 *  Copyright 2012 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Backend/Platform/Android/Core/JNI/JavaInterfaceManager.h>

namespace ChilliSource
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
		IJavaInterfaceSPtr JavaInterfaceManager::GetJavaInterface(Core::InterfaceIDType inInterfaceID)
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
		void JavaInterfaceManager::GetJavaInterfaces(Core::InterfaceIDType inInterfaceID, std::vector<IJavaInterfaceSPtr>& outJavaInterfaces)
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
