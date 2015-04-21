//
//  JavaClass.h
//  Chilli Source
//  Created by Ian Copland on 21/04/2015.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2015 Tag Games Limited
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

#ifndef _CSBACKEND_PLATFORM_ANDROID_MAIN_JNI_CORE_JNI_JAVACLASS_H_
#define _CSBACKEND_PLATFORM_ANDROID_MAIN_JNI_CORE_JNI_JAVACLASS_H_

#include <ChilliSource/ChilliSource.h>
#include <CSBackend/Platform/Android/Main/JNI/ForwardDeclarations.h>
#include <CSBackend/Platform/Android/Main/JNI/Core/JNI/JavaClassDef.h>
#include <CSBackend/Platform/Android/Main/JNI/Core/JNI/JavaVirtualMachine.h>

#include <jni.h>

namespace CSBackend
{
	namespace Android
	{
		//------------------------------------------------------------------------------
		/// Provides a means to create an instance of a class in java and call its
		/// methods.
		///
		/// TODO: Further usage description
		///
		/// Use of JavaClass is preferred over the older JavaInterface method of
		/// communication with Java classes.
		///
		/// JavaClass is immutable after construction.
		///
		/// @author Ian Copland
		//------------------------------------------------------------------------------
		class JavaClass final
		{
		public:
            //------------------------------------------------------------------------------
            /// Constructor. Instantiates the defined java class and prepares the methods
            /// so that they can be called.
            ///
            /// The class and all methods will be checked to ensure they exist.
            ///
            /// @author Ian Copland
            ///
            /// @param in_javaClassDef - The definition for this java class.
            //------------------------------------------------------------------------------
            JavaClass(const JavaClassDef& in_javaClassDef);
            //------------------------------------------------------------------------------
            /// Calls a void java method.
            ///
            /// The method will be checked to ensure the method type is correct. Exceptions
            /// thrown from Java are treated as fatal errors and the application will be
            /// terminated.
            ///
            /// @author Ian Copland
            ///
            /// @param in_methodName - The name of the method to call.
            /// @param ... - The arguments to the method.
            //------------------------------------------------------------------------------
            template <typename... TArgs> void CallVoidMethod(const std::string& in_methodName, TArgs&&... in_args) const;

        private:
            //------------------------------------------------------------------------------
            /// An enum describing the various method types which can be called through
            /// JavaClass.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            enum class MethodType
            {
                k_void
            };
            //------------------------------------------------------------------------------
            /// A struct containing information on a single Java method.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            struct MethodInfo
            {
                jmethodID m_methodId = nullptr;
                MethodType m_methodType = MethodType::k_void;
                u32 m_numArguments = 0;
            };
            //------------------------------------------------------------------------------
            /// Checks for java exceptions. If an exception has occurred it is treated as
            /// a fatal error and the application will be terminated.
            ///
            /// @author Ian Copland
            ///
            /// @param in_errorMessage - The error message to print if an exception occurred.
            //------------------------------------------------------------------------------
            void CheckJavaExceptions(const std::string& in_errorMessage);
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The method type for the given method signature.
            //------------------------------------------------------------------------------
            MethodType CalcMethodType(const std::string& in_methodSignature);
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The number of parameters the method described by the given signature
            /// takes.
            //------------------------------------------------------------------------------
            u32 CalcNumArguments(const std::string& in_methodSignature);

            std::string m_className;
            jobject m_javaObject = nullptr;
            std::map<std::string, MethodInfo> m_methods;
		};
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename... TArgs> void JavaClass::CallVoidMethod(const std::string& in_methodName, TArgs&&... in_args) const
        {
            auto environment = JavaVirtualMachine::Get()->GetJNIEnvironment();

            auto methodInfoIt = m_methods.find(in_methodName);
            CS_ASSERT(methodInfoIt != m_methods.end(), "Could not find method '" + in_methodName + "' in Java class '" + m_className + "'");
            CS_ASSERT(methodInfoIt->m_methodType == MethodType::k_void, "Cannot call method '" + in_methodName + "' in Java class '" + m_className + "' as it is not a void method.");
            CS_ASSERT(methodInfoIt->m_numArguments == sizeof...(in_args), "Cannot call method '" + in_methodName + "' in Java class '" + m_className + "' because too few arguments were supplied.");
        }
	}
}

#endif

#endif
