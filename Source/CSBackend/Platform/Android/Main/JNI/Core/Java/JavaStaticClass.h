//
//  JavaStaticClass.h
//  ChilliSource
//  Created by Ian Copland on 08/06/2015.
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

#ifndef _CSBACKEND_PLATFORM_ANDROID_MAIN_JNI_CORE_JAVA_JAVASTATICCLASS_H_
#define _CSBACKEND_PLATFORM_ANDROID_MAIN_JNI_CORE_JAVA_JAVASTATICCLASS_H_

#include <ChilliSource/ChilliSource.h>

#include <CSBackend/Platform/Android/Main/JNI/ForwardDeclarations.h>
#include <CSBackend/Platform/Android/Main/JNI/Core/Java/JavaMethodSignature.h>
#include <CSBackend/Platform/Android/Main/JNI/Core/Java/JavaStaticClassDef.h>
#include <CSBackend/Platform/Android/Main/JNI/Core/Java/JavaUtils.h>
#include <CSBackend/Platform/Android/Main/JNI/Core/Java/JavaVirtualMachine.h>

#include <jni.h>

#include <unordered_map>

namespace CSBackend
{
	namespace Android
	{
		//------------------------------------------------------------------------------
		/// Provides a means to reference a static class in java and call its methods.
		///
		/// This is essentially a wrapper around the JNI functions for calling static
		/// methods java methods. Methods references are cached for the sake of
		/// performance and there are a number of sanity checks to assist debugging of
		/// issues including confirming that the correct number of parameters have
		/// been provided and that the method is being called with the correct return
		/// type. Exceptions are also checked for after every method call, avoiding
		/// difficult to debug issues where native execution continues until the next
		/// java call when an exception is thrown.
		///
		/// JavaStaticClass is immutable after construction, meaning it can safely be
		/// called from multiple threads at the same time. Care still needs to be taken
		/// to ensure the Java and Native classes that are using it are thread-safe,
		/// however.
		///
		/// @author Ian Copland
		//------------------------------------------------------------------------------
		class JavaStaticClass final
		{
		public:
            //------------------------------------------------------------------------------
            /// Constructor. Prepares the static methods so that they can be called.
            ///
            /// The class and all methods will be checked to ensure they exist.
            ///
            /// @author Ian Copland
            ///
            /// @param in_javaStaticClassDef - The definition for this java static class.
            //------------------------------------------------------------------------------
            JavaStaticClass(const JavaStaticClassDef& in_javaStaticClassDef);
            //------------------------------------------------------------------------------
            /// Calls a void java static method.
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
            /// Calls a boolean java static method.
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
            /// Calls a byte java static method.
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
            /// Calls a char java static method.
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
            /// Calls a short java static method.
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
            /// Calls an int java static method.
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
            /// Calls a long java static method.
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
            /// Calls a float java static method.
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
            /// Calls a double java static method.
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
            /// Calls an object java static method.
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
            /// Calls an string java static method. This is a special case of CallObjectMethod()
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
            ~JavaStaticClass();

        private:
            //------------------------------------------------------------------------------
            /// A struct containing information on a single Java static method.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            struct MethodInfo
            {
                jmethodID m_methodId = nullptr;
                JavaMethodSignature::ReturnType m_returnType = JavaMethodSignature::ReturnType::k_void;
                u32 m_numArguments = 0;
            };
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
            jmethodID GetMethodId(const std::string& in_methodName, JavaMethodSignature::ReturnType in_returnType, u32 in_numArguments) const;

            std::string m_className;
            jclass m_javaClass = nullptr;
            std::unordered_map<std::string, MethodInfo> m_methods;
		};
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename... TArgs> void JavaStaticClass::CallVoidMethod(const std::string& in_methodName, TArgs&&... in_args) const
        {
            auto environment = JavaVirtualMachine::Get()->GetJNIEnvironment();
            auto methodId = GetMethodId(in_methodName, JavaMethodSignature::ReturnType::k_void, sizeof...(TArgs));

            environment->CallStaticVoidMethod(m_javaClass, methodId, std::forward<TArgs>(in_args)...);
            JavaUtils::CheckJavaExceptions("An exception was thrown while calling method '" + in_methodName + "' in Java static class '" + m_className + "'.");
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename... TArgs> bool JavaStaticClass::CallBoolMethod(const std::string& in_methodName, TArgs&&... in_args) const
        {
            auto environment = JavaVirtualMachine::Get()->GetJNIEnvironment();
            auto methodId = GetMethodId(in_methodName, JavaMethodSignature::ReturnType::k_bool, sizeof...(TArgs));

            auto output = environment->CallStaticBooleanMethod(m_javaClass, methodId, std::forward<TArgs>(in_args)...);
            JavaUtils::CheckJavaExceptions("An exception was thrown while calling method '" + in_methodName + "' in Java static class '" + m_className + "'.");

            return output;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename... TArgs> u8 JavaStaticClass::CallByteMethod(const std::string& in_methodName, TArgs&&... in_args) const
        {
            auto environment = JavaVirtualMachine::Get()->GetJNIEnvironment();
            auto methodId = GetMethodId(in_methodName, JavaMethodSignature::ReturnType::k_byte, sizeof...(TArgs));

            auto output = environment->CallStaticByteMethod(m_javaClass, methodId, std::forward<TArgs>(in_args)...);
            JavaUtils::CheckJavaExceptions("An exception was thrown while calling method '" + in_methodName + "' in Java static class '" + m_className + "'.");

            return output;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename... TArgs> u16 JavaStaticClass::CallCharMethod(const std::string& in_methodName, TArgs&&... in_args) const
        {
            auto environment = JavaVirtualMachine::Get()->GetJNIEnvironment();
            auto methodId = GetMethodId(in_methodName, JavaMethodSignature::ReturnType::k_char, sizeof...(TArgs));

            auto output = environment->CallStaticCharMethod(m_javaClass, methodId, std::forward<TArgs>(in_args)...);
            JavaUtils::CheckJavaExceptions("An exception was thrown while calling method '" + in_methodName + "' in Java static class '" + m_className + "'.");

            return output;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename... TArgs> s16 JavaStaticClass::CallShortMethod(const std::string& in_methodName, TArgs&&... in_args) const
        {
            auto environment = JavaVirtualMachine::Get()->GetJNIEnvironment();
            auto methodId = GetMethodId(in_methodName, JavaMethodSignature::ReturnType::k_short, sizeof...(TArgs));

            auto output = environment->CallStaticShortMethod(m_javaClass, methodId, std::forward<TArgs>(in_args)...);
            JavaUtils::CheckJavaExceptions("An exception was thrown while calling method '" + in_methodName + "' in Java static class '" + m_className + "'.");

            return output;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename... TArgs> s32 JavaStaticClass::CallIntMethod(const std::string& in_methodName, TArgs&&... in_args) const
        {
            auto environment = JavaVirtualMachine::Get()->GetJNIEnvironment();
            auto methodId = GetMethodId(in_methodName, JavaMethodSignature::ReturnType::k_int, sizeof...(TArgs));

            auto output = environment->CallStaticIntMethod(m_javaClass, methodId, std::forward<TArgs>(in_args)...);
            JavaUtils::CheckJavaExceptions("An exception was thrown while calling method '" + in_methodName + "' in Java static class '" + m_className + "'.");

            return output;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename... TArgs> s64 JavaStaticClass::CallLongMethod(const std::string& in_methodName, TArgs&&... in_args) const
        {
            auto environment = JavaVirtualMachine::Get()->GetJNIEnvironment();
            auto methodId = GetMethodId(in_methodName, JavaMethodSignature::ReturnType::k_long, sizeof...(TArgs));

            auto output = environment->CallStaticLongMethod(m_javaClass, methodId, std::forward<TArgs>(in_args)...);
            JavaUtils::CheckJavaExceptions("An exception was thrown while calling method '" + in_methodName + "' in Java static class '" + m_className + "'.");

            return output;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename... TArgs> f32 JavaStaticClass::CallFloatMethod(const std::string& in_methodName, TArgs&&... in_args) const
        {
            auto environment = JavaVirtualMachine::Get()->GetJNIEnvironment();
            auto methodId = GetMethodId(in_methodName, JavaMethodSignature::ReturnType::k_float, sizeof...(TArgs));

            f32 output = environment->CallStaticFloatMethod(m_javaClass, methodId, std::forward<TArgs>(in_args)...);
            JavaUtils::CheckJavaExceptions("An exception was thrown while calling method '" + in_methodName + "' in Java static class '" + m_className + "'.");

            return output;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename... TArgs> f64 JavaStaticClass::CallDoubleMethod(const std::string& in_methodName, TArgs&&... in_args) const
        {
            auto environment = JavaVirtualMachine::Get()->GetJNIEnvironment();
            auto methodId = GetMethodId(in_methodName, JavaMethodSignature::ReturnType::k_double, sizeof...(TArgs));

            auto output = environment->CallStaticDoubleMethod(m_javaClass, methodId, std::forward<TArgs>(in_args)...);
            JavaUtils::CheckJavaExceptions("An exception was thrown while calling method '" + in_methodName + "' in Java static class '" + m_className + "'.");

            return output;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename... TArgs> jobject JavaStaticClass::CallObjectMethod(const std::string& in_methodName, TArgs&&... in_args) const
        {
            auto environment = JavaVirtualMachine::Get()->GetJNIEnvironment();
            auto methodId = GetMethodId(in_methodName, JavaMethodSignature::ReturnType::k_object, sizeof...(TArgs));

            auto output = environment->CallStaticObjectMethod(m_javaClass, methodId, std::forward<TArgs>(in_args)...);
            JavaUtils::CheckJavaExceptions("An exception was thrown while calling method '" + in_methodName + "' in Java static class '" + m_className + "'.");

            return output;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename... TArgs> std::string JavaStaticClass::CallStringMethod(const std::string& in_methodName, TArgs&&... in_args) const
        {
            auto environment = JavaVirtualMachine::Get()->GetJNIEnvironment();
            auto methodId = GetMethodId(in_methodName, JavaMethodSignature::ReturnType::k_string, sizeof...(TArgs));

            auto jString = static_cast<jstring>(environment->CallStaticObjectMethod(m_javaClass, methodId, std::forward<TArgs>(in_args)...));
            JavaUtils::CheckJavaExceptions("An exception was thrown while calling method '" + in_methodName + "' in Java static class '" + m_className + "'.");

            std::string output = JavaUtils::CreateSTDStringFromJString(jString);
            environment->DeleteLocalRef(jString);

            return output;
        }
	}
}

#endif

#endif
