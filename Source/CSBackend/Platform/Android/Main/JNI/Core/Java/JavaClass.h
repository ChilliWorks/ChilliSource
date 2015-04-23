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

#ifndef _CSBACKEND_PLATFORM_ANDROID_MAIN_JNI_CORE_JAVA_JAVACLASS_H_
#define _CSBACKEND_PLATFORM_ANDROID_MAIN_JNI_CORE_JAVA_JAVACLASS_H_

#include <ChilliSource/ChilliSource.h>

#include <CSBackend/Platform/Android/Main/JNI/ForwardDeclarations.h>
#include <CSBackend/Platform/Android/Main/JNI/Core/Java/JavaClassDef.h>
#include <CSBackend/Platform/Android/Main/JNI/Core/Java/JavaUtils.h>
#include <CSBackend/Platform/Android/Main/JNI/Core/Java/JavaVirtualMachine.h>

#include <jni.h>

#include <unordered_map>

//REMOVE ME!
#include <typeinfo>
//////

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
		/// JavaClass is immutable after construction, meaning it can safely be called
		/// from multiple threads at the same time. Care still needs to be taken to
		/// ensure the Java and Native classes that are using this are thread-safe,
		/// however.
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
            //------------------------------------------------------------------------------
            /// Calls a boolean java method.
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
            template <typename... TArgs> bool CallBoolMethod(const std::string& in_methodName, TArgs&&... in_args) const;
            //------------------------------------------------------------------------------
            /// Calls a byte java method.
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
            template <typename... TArgs> u8 CallByteMethod(const std::string& in_methodName, TArgs&&... in_args) const;
            //------------------------------------------------------------------------------
            /// Calls a char java method.
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
            template <typename... TArgs> u16 CallCharMethod(const std::string& in_methodName, TArgs&&... in_args) const;
            //------------------------------------------------------------------------------
            /// Calls a short java method.
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
            template <typename... TArgs> s16 CallShortMethod(const std::string& in_methodName, TArgs&&... in_args) const;
            //------------------------------------------------------------------------------
            /// Calls an int java method.
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
            template <typename... TArgs> s32 CallIntMethod(const std::string& in_methodName, TArgs&&... in_args) const;
            //------------------------------------------------------------------------------
            /// Calls a long java method.
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
            template <typename... TArgs> s64 CallLongMethod(const std::string& in_methodName, TArgs&&... in_args) const;
            //------------------------------------------------------------------------------
            /// Calls a float java method.
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
            template <typename... TArgs> f32 CallFloatMethod(const std::string& in_methodName, TArgs&&... in_args) const;
            //------------------------------------------------------------------------------
            /// Calls a double java method.
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
            template <typename... TArgs> f64 CallDoubleMethod(const std::string& in_methodName, TArgs&&... in_args) const;
            //------------------------------------------------------------------------------
            /// Calls an object java method.
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
            template <typename... TArgs> jobject CallObjectMethod(const std::string& in_methodName, TArgs&&... in_args) const;
            //------------------------------------------------------------------------------
            /// Calls an string java method. This is a special case of CallObjectMethod()
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
            template <typename... TArgs> std::string CallStringMethod(const std::string& in_methodName, TArgs&&... in_args) const;
            //------------------------------------------------------------------------------
            /// Destructor.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            ~JavaClass();

        private:
            //------------------------------------------------------------------------------
            /// An enum describing the various return types for methods  which can be called
            /// through JavaClass.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            enum class ReturnType
            {
                k_void,
                k_bool,
                k_byte,
                k_char,
                k_short,
                k_int,
                k_long,
                k_float,
                k_double,
                k_object,
                k_string
            };
            //------------------------------------------------------------------------------
            /// A struct containing information on a single Java method.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            struct MethodInfo
            {
                jmethodID m_methodId = nullptr;
                ReturnType m_returnType = ReturnType::k_void;
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
            void CheckJavaExceptions(const std::string& in_errorMessage) const;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The return type for the given method signature.
            //------------------------------------------------------------------------------
            ReturnType CalcReturnType(const std::string& in_methodSignature) const;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The number of parameters the method described by the given signature
            /// takes.
            //------------------------------------------------------------------------------
            u32 CalcNumArguments(const std::string& in_methodSignature) const;
            //------------------------------------------------------------------------------
            /// Finds the method with the given name and performs some checks to mitigate
            /// mistakes when calling the JNI. This includes that the method has the
            /// correct return type and has the correct number of arguments.
            ///
            /// @author Ian Copland
            ///
            /// @return The method info for the given name, type and number of arguments.
            ///
            /// @param in_methodName - The name of the method.
            /// @param in_returnType - The type of the method.
            /// @param in_numArguments - The number of arguments the method should have.
            ///
            /// @return The method info.
            //------------------------------------------------------------------------------
            jmethodID GetMethodId(const std::string& in_methodName, ReturnType in_returnType, u32 in_numArguments) const;

            std::string m_className;
            jobject m_javaObject = nullptr;
            std::unordered_map<std::string, MethodInfo> m_methods;
		};
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename... TArgs> void JavaClass::CallVoidMethod(const std::string& in_methodName, TArgs&&... in_args) const
        {
            auto environment = JavaVirtualMachine::Get()->GetJNIEnvironment();
            auto methodId = GetMethodId(in_methodName, ReturnType::k_void, sizeof...(TArgs));

            environment->CallVoidMethod(m_javaObject, methodId, std::forward<TArgs>(in_args)...);
            CheckJavaExceptions("An exception was thrown while calling method '" + in_methodName + "' in Java class '" + m_className + "'.");
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename... TArgs> bool JavaClass::CallBoolMethod(const std::string& in_methodName, TArgs&&... in_args) const
        {
            auto environment = JavaVirtualMachine::Get()->GetJNIEnvironment();
            auto methodId = GetMethodId(in_methodName, ReturnType::k_bool, sizeof...(TArgs));

            auto output = environment->CallBooleanMethod(m_javaObject, methodId, std::forward<TArgs>(in_args)...);
            CheckJavaExceptions("An exception was thrown while calling method '" + in_methodName + "' in Java class '" + m_className + "'.");

            return output;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename... TArgs> u8 JavaClass::CallByteMethod(const std::string& in_methodName, TArgs&&... in_args) const
        {
            auto environment = JavaVirtualMachine::Get()->GetJNIEnvironment();
            auto methodId = GetMethodId(in_methodName, ReturnType::k_byte, sizeof...(TArgs));

            auto output = environment->CallByteMethod(m_javaObject, methodId, std::forward<TArgs>(in_args)...);
            CheckJavaExceptions("An exception was thrown while calling method '" + in_methodName + "' in Java class '" + m_className + "'.");

            return output;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename... TArgs> u16 JavaClass::CallCharMethod(const std::string& in_methodName, TArgs&&... in_args) const
        {
            auto environment = JavaVirtualMachine::Get()->GetJNIEnvironment();
            auto methodId = GetMethodId(in_methodName, ReturnType::k_char, sizeof...(TArgs));

            auto output = environment->CallCharMethod(m_javaObject, methodId, std::forward<TArgs>(in_args)...);
            CheckJavaExceptions("An exception was thrown while calling method '" + in_methodName + "' in Java class '" + m_className + "'.");

            return output;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename... TArgs> s16 JavaClass::CallShortMethod(const std::string& in_methodName, TArgs&&... in_args) const
        {
            auto environment = JavaVirtualMachine::Get()->GetJNIEnvironment();
            auto methodId = GetMethodId(in_methodName, ReturnType::k_short, sizeof...(TArgs));

            auto output = environment->CallShortMethod(m_javaObject, methodId, std::forward<TArgs>(in_args)...);
            CheckJavaExceptions("An exception was thrown while calling method '" + in_methodName + "' in Java class '" + m_className + "'.");

            return output;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename... TArgs> s32 JavaClass::CallIntMethod(const std::string& in_methodName, TArgs&&... in_args) const
        {
            auto environment = JavaVirtualMachine::Get()->GetJNIEnvironment();
            auto methodId = GetMethodId(in_methodName, ReturnType::k_int, sizeof...(TArgs));

            auto output = environment->CallIntMethod(m_javaObject, methodId, std::forward<TArgs>(in_args)...);
            CheckJavaExceptions("An exception was thrown while calling method '" + in_methodName + "' in Java class '" + m_className + "'.");

            return output;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename... TArgs> s64 JavaClass::CallLongMethod(const std::string& in_methodName, TArgs&&... in_args) const
        {
            auto environment = JavaVirtualMachine::Get()->GetJNIEnvironment();
            auto methodId = GetMethodId(in_methodName, ReturnType::k_long, sizeof...(TArgs));

            auto output = environment->CallLongMethod(m_javaObject, methodId, std::forward<TArgs>(in_args)...);
            CheckJavaExceptions("An exception was thrown while calling method '" + in_methodName + "' in Java class '" + m_className + "'.");

            return output;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename... TArgs> f32 JavaClass::CallFloatMethod(const std::string& in_methodName, TArgs&&... in_args) const
        {
            auto environment = JavaVirtualMachine::Get()->GetJNIEnvironment();
            auto methodId = GetMethodId(in_methodName, ReturnType::k_float, sizeof...(TArgs));

            f32 output = environment->CallFloatMethod(m_javaObject, methodId, std::forward<TArgs>(in_args)...);
            CheckJavaExceptions("An exception was thrown while calling method '" + in_methodName + "' in Java class '" + m_className + "'.");

            return output;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename... TArgs> f64 JavaClass::CallDoubleMethod(const std::string& in_methodName, TArgs&&... in_args) const
        {
            auto environment = JavaVirtualMachine::Get()->GetJNIEnvironment();
            auto methodId = GetMethodId(in_methodName, ReturnType::k_double, sizeof...(TArgs));

            auto output = environment->CallDoubleMethod(m_javaObject, methodId, std::forward<TArgs>(in_args)...);
            CheckJavaExceptions("An exception was thrown while calling method '" + in_methodName + "' in Java class '" + m_className + "'.");

            return output;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename... TArgs> jobject JavaClass::CallObjectMethod(const std::string& in_methodName, TArgs&&... in_args) const
        {
            auto environment = JavaVirtualMachine::Get()->GetJNIEnvironment();
            auto methodId = GetMethodId(in_methodName, ReturnType::k_object, sizeof...(TArgs));

            auto output = environment->CallObjectMethod(m_javaObject, methodId, std::forward<TArgs>(in_args)...);
            CheckJavaExceptions("An exception was thrown while calling method '" + in_methodName + "' in Java class '" + m_className + "'.");

            return output;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename... TArgs> std::string JavaClass::CallStringMethod(const std::string& in_methodName, TArgs&&... in_args) const
        {
            auto environment = JavaVirtualMachine::Get()->GetJNIEnvironment();
            auto methodId = GetMethodId(in_methodName, ReturnType::k_string, sizeof...(TArgs));

            auto jString = static_cast<jstring>(environment->CallObjectMethod(m_javaObject, methodId, std::forward<TArgs>(in_args)...));
            CheckJavaExceptions("An exception was thrown while calling method '" + in_methodName + "' in Java class '" + m_className + "'.");

            std::string output = JavaUtils::CreateSTDStringFromJString(jString);
            environment->DeleteLocalRef(jString);

            return output;
        }
	}
}

#endif

#endif
