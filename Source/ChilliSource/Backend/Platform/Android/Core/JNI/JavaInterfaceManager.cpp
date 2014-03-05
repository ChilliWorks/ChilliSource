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
		JavaInterfaceManager* JavaInterfaceManager::mpJavaInterfaceManagerSingleton = NULL;

		//-------------------------------------------------------
		/// Get Singleton Ptr
		//-------------------------------------------------------
		JavaInterfaceManager* JavaInterfaceManager::GetSingletonPtr()
		{
			if (mpJavaInterfaceManagerSingleton == NULL)
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
		 : mpJavaVM(NULL)
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
			if (mpJavaVM != NULL)
			{
				JNIEnv* env;
				mpJavaVM->AttachCurrentThread(&env, NULL);
				return env;
			}

			return NULL;
		}
		//--------------------------------------------------------
		/// Add Java Interface
		//--------------------------------------------------------
		void JavaInterfaceManager::AddJavaInterface(JavaInterfacePtr inpJavaInterface)
		{
			mJavaInterfaces.push_back(inpJavaInterface);
		}
		//--------------------------------------------------------
		/// Get Java Interface
		//--------------------------------------------------------
		JavaInterfacePtr JavaInterfaceManager::GetJavaInterface(Core::InterfaceIDType inInterfaceID)
		{
			for (std::vector<JavaInterfacePtr>::const_iterator it = mJavaInterfaces.begin(); it != mJavaInterfaces.end(); ++it)
			{
				if ((*it)->IsA(inInterfaceID))
				{
					return (*it);
				}
			}

			CS_LOG_WARNING("JavaInterfaceManager cannot find a JavaInterface with the requested interface.");
			return JavaInterfacePtr();
		}
		//--------------------------------------------------------
		/// Get Java Interfaces
		//--------------------------------------------------------
		void JavaInterfaceManager::GetJavaInterfaces(Core::InterfaceIDType inInterfaceID, std::vector<JavaInterfacePtr>& outJavaInterfaces)
		{
			outJavaInterfaces.clear();
			for (std::vector<JavaInterfacePtr>::const_iterator it = mJavaInterfaces.begin(); it != mJavaInterfaces.end(); ++it)
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
