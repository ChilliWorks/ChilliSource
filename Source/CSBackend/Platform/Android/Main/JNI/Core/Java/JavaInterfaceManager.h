//
//  JavaInterfaceManager.h
//  ChilliSource
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

#ifndef _CHILLISOURCE_PLATFORM_ANDROID_JAVAINTERFACE_JAVAINTERFACEMANAGER_H_
#define _CHILLISOURCE_PLATFORM_ANDROID_JAVAINTERFACE_JAVAINTERFACEMANAGER_H_

#include <CSBackend/Platform/Android/Main/JNI/ForwardDeclarations.h>
#include <CSBackend/Platform/Android/Main/JNI/Core/Java/JavaInterface.h>

#include <jni.h>

namespace CSBackend
{
	namespace Android
	{
		//========================================================
		/// Java Interface Manager
		///
		/// Handles all of the java interfaces and provides an
		/// interface to access them.
		///
		/// This is deprecated in favour of JavaClass and
		/// JavaVirtualMachine
		//========================================================
		class JavaInterfaceManager
		{
		public:
			//--------------------------------------------------------
			/// Get Singleton Ptr
			///
			/// @return a pointer to the singleton instance.
			//--------------------------------------------------------
			static JavaInterfaceManager* GetSingletonPtr();
			//--------------------------------------------------------
			/// Get Singleton
			///
			/// @return a reference to the singleton instance.
			//--------------------------------------------------------
			static JavaInterfaceManager& GetSingleton();
			//--------------------------------------------------------
			/// Initialise
			///
			/// Initialises the Java Interface Manager
			///
			/// @param The JVM.
			//--------------------------------------------------------
			void Initialise(JavaVM* inpJavaVM);
			//--------------------------------------------------------
			/// Get Java VM
			///
			/// @return The Java VM.
			//--------------------------------------------------------
			JavaVM* GetJavaVM();
			//--------------------------------------------------------
			/// Get JNI Environment Ptr
			///
			///	returns the jni enviroment pointer for the current
			/// thread. The value returned from this should NOT be stored
			/// as global or member data, as this will break thread safety
			///
			/// @return returns a pointer to the jni environment.
			//--------------------------------------------------------
			JNIEnv* GetJNIEnvironmentPtr();
			//--------------------------------------------------------
			/// Add Java Interface
			///
			/// Adds a new Java Interface. This should be called on
			/// creation of the system that uses the java interface.
			///
			/// @param The java interface.
			//--------------------------------------------------------
			void AddJavaInterface(IJavaInterfaceSPtr inpJavaInterface);
			//--------------------------------------------------------
			/// Get Java Interface
			///
			/// Looks for a java interface that implements the given interface.
			///
			/// @param The type ID of the java interface you wish to implement
			/// @return Java Interface that implements the given interface or nullptr if none
			//--------------------------------------------------------
			IJavaInterfaceSPtr GetJavaInterface(ChilliSource::InterfaceIDType inInterfaceID);
			//--------------------------------------------------------
			/// Get Java Interface
			///
			/// Looks for a java interface that implements the given interface.
			///
			/// @return Java Interface that implements the given interface or nullptr if none
			//--------------------------------------------------------
			template <typename T> std::shared_ptr<T> GetJavaInterface();
			//--------------------------------------------------------
			/// Get Java Interface
			///
			/// Looks for a java interface that implements the given interface.
			///
			/// @return Java Interface that implements the given interface or nullptr if none
			//--------------------------------------------------------
			template <typename T, typename U> std::shared_ptr<T> GetJavaInterface();
			//--------------------------------------------------------
			/// Get Java Interfaces
			///
			/// Looks for Java Interfaces that implement the given interface
			/// and fills an array with them.
			///
			/// @param The type ID of the Java Interface you wish to implement
			/// @param OUT: The array of java interfaces.
			//--------------------------------------------------------
			void GetJavaInterfaces(ChilliSource::InterfaceIDType inInterfaceID, std::vector<IJavaInterfaceSPtr>& outJavaInterfaces);
			//--------------------------------------------------------
			/// Get Java Interfaces
			///
			/// Looks for Java Interfaces that implement the given interface
			/// and fills an array with them.
			///
			/// @param OUT: The array of java interfaces.
			//--------------------------------------------------------
			template <typename T> void GetJavaInterfaces(std::vector<std::shared_ptr<T> >& outJavaInterfaces);
			//--------------------------------------------------------
			/// Destructor
			//--------------------------------------------------------
			~JavaInterfaceManager();
		private:
			//--------------------------------------------------------
			/// Constructor
			//--------------------------------------------------------
			JavaInterfaceManager();

			static JavaInterfaceManager* mpJavaInterfaceManagerSingleton;
			std::vector<IJavaInterfaceSPtr> mJavaInterfaces;
			JavaVM* mpJavaVM;
		};
		//========================================================
		/// Templated Definitions
		//========================================================
		//--------------------------------------------------------
		/// Get Java Interface
		//--------------------------------------------------------
		template <typename T> std::shared_ptr<T> JavaInterfaceManager::GetJavaInterface()
		{
			IJavaInterfaceSPtr pJavaInterface = GetJavaInterface(T::InterfaceID);

			if (pJavaInterface != nullptr)
			{
				return std::static_pointer_cast<T>(pJavaInterface);
			}

			return std::shared_ptr<T>();
		}
		//--------------------------------------------------------
		/// Get Java Interface
		//--------------------------------------------------------
		template <typename T, typename U> std::shared_ptr<T> JavaInterfaceManager::GetJavaInterface()
		{
			IJavaInterfaceSPtr pJavaInterface = GetJavaInterface(U::InterfaceID);

			if (pJavaInterface != nullptr)
			{
				return std::static_pointer_cast<T>(pJavaInterface);
			}

			return std::shared_ptr<T>();
		}
		//--------------------------------------------------------
		/// Get Java Interfaces
		//--------------------------------------------------------
		template <typename T> void JavaInterfaceManager::GetJavaInterfaces(std::vector<std::shared_ptr<T> >& outJavaInterfaces)
		{
			for (size_t nJavaInterface = 0; nJavaInterface < mJavaInterfaces.size(); nJavaInterface++)
			{
				if (mJavaInterfaces[nJavaInterface]->IsA(T::InterfaceID))
				{
					outJavaInterfaces.push_back(std::static_pointer_cast<T>(mJavaInterfaces[nJavaInterface]));
				}
			}
		}
	}
}

#endif

#endif
