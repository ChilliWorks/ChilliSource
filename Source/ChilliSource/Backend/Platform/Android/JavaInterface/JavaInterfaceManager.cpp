/*
 *  JavaInterfaceManager.cpp
 *  moFlow
 *
 *  Created by Ian Copland on 08/08/2012.
 *  Copyright 2012 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Backend/Platform/Android/JavaInterface/JavaInterfaceManager.h>

namespace ChilliSource
{
	namespace Android
	{
		CJavaInterfaceManager* CJavaInterfaceManager::mpJavaInterfaceManagerSingleton = NULL;

		//-------------------------------------------------------
		/// Get Singleton Ptr
		//-------------------------------------------------------
		CJavaInterfaceManager* CJavaInterfaceManager::GetSingletonPtr()
		{
			if (mpJavaInterfaceManagerSingleton == NULL)
			{
				mpJavaInterfaceManagerSingleton = new CJavaInterfaceManager();
			}

			return mpJavaInterfaceManagerSingleton;
		}
		//--------------------------------------------------------
		/// Get Singleton
		//--------------------------------------------------------
		CJavaInterfaceManager& CJavaInterfaceManager::GetSingleton()
		{
			return *GetSingletonPtr();
		}
		//--------------------------------------------------------
		/// Constructor
		//--------------------------------------------------------
		CJavaInterfaceManager::CJavaInterfaceManager()
		 : mpJavaVM(NULL)
		{
		}
		//--------------------------------------------------------
		/// Initialise
		//--------------------------------------------------------
		void CJavaInterfaceManager::Initialise(JavaVM* inpJavaVM)
		{
			mpJavaVM = inpJavaVM;
		}
		//--------------------------------------------------------
		/// Get Java VM
		//--------------------------------------------------------
		JavaVM* CJavaInterfaceManager::GetJavaVM()
		{
			return mpJavaVM;
		}
		//--------------------------------------------------------
		/// Get JNI Environment Ptr
		//--------------------------------------------------------
		JNIEnv* CJavaInterfaceManager::GetJNIEnvironmentPtr()
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
		void CJavaInterfaceManager::AddJavaInterface(JavaInterfacePtr inpJavaInterface)
		{
			mJavaInterfaces.push_back(inpJavaInterface);
		}
		//--------------------------------------------------------
		/// Get Java Interface
		//--------------------------------------------------------
		JavaInterfacePtr CJavaInterfaceManager::GetJavaInterface(Core::InterfaceIDType inInterfaceID)
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
		void CJavaInterfaceManager::GetJavaInterfaces(Core::InterfaceIDType inInterfaceID, std::vector<JavaInterfacePtr>& outJavaInterfaces)
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
		CJavaInterfaceManager::~CJavaInterfaceManager()
		{
			mJavaInterfaces.clear();
		}
	}
}
