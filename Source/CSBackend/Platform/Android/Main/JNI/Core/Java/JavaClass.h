//
//  JavaClass.h
//  ChilliSource
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
#include <CSBackend/Platform/Android/Main/JNI/Core/Java/JavaMethodSignature.h>
#include <CSBackend/Platform/Android/Main/JNI/Core/Java/JavaUtils.h>
#include <CSBackend/Platform/Android/Main/JNI/Core/Java/JavaVirtualMachine.h>

#include <jni.h>

#include <unordered_map>

namespace CSBackend
{
	namespace Android
	{
		//------------------------------------------------------------------------------
		/// Provides a means to create an instance of a class in java and call its
		/// methods.
		///
		/// This is essentially a wrapper around the JNI functions for calling methods
		/// on a class instance. Methods references are cached for the sake of
		/// performance and there are a number of sanity checks to assist debugging of
		/// issues including confirming that the correct number of parameters have
		/// been provided and that the method is being called with the correct return
		/// type. Exceptions are also checked for after every method call, avoiding
		/// difficult to debug issues where native execution continues until the next
		/// java call when an exception is thrown.
		///
		/// JavaClass is immutable after construction, meaning it can safely be called
		/// from multiple threads at the same time. Care still needs to be taken to
		/// ensure the Java and Native classes that are using it are thread-safe,
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
            /// @param ... - The arguments to the constructor.
            //------------------------------------------------------------------------------
            template <typename... TArgs> JavaClass(const JavaClassDef& in_javaClassDef, TArgs&&... in_args);
            //------------------------------------------------------------------------------
            /// Constructor. Creates the java class with an already
            /// instantiated object and prepares the methods so that they can be called.
            ///
            /// The class and all methods will be checked to ensure they exist.
            ///
            /// @author HMcLaughlin
            ///
            /// @param in_object - Object instance whose type is defined by in_javaClassDef
            /// @param in_javaClassDef - The definition for this java class.
            //------------------------------------------------------------------------------
            JavaClass(jobject in_object, const JavaClassDef& in_javaClassDef);
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
            /// Returns the java object for this
            ///
            /// @author HMcLaughlin
            ///
            /// @return Java object
            //------------------------------------------------------------------------------
            jobject GetJavaObject() const;
            //------------------------------------------------------------------------------
            /// Gets the class name
            ///
            /// @author HMcLaughlin
            ///
            /// @return Class Name of the java def
            //------------------------------------------------------------------------------
            const std::string& GetClassName() const;
            //------------------------------------------------------------------------------
            /// Destructor.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            ~JavaClass();

        private:
            //------------------------------------------------------------------------------
            /// A struct containing information on a single Java method.
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
            jobject m_javaObject = nullptr;
            std::unordered_map<std::string, MethodInfo> m_methods;
		};
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename... TArgs> JavaClass::JavaClass(const JavaClassDef& in_javaClassDef, TArgs&&... in_args)
        {
            auto jvm = JavaVirtualMachine::Get();
            auto environment = JavaVirtualMachine::Get()->GetJNIEnvironment();

            //Get the class
            m_className = in_javaClassDef.GetClassName();
            jclass jClass = jvm->FindClass(m_className);

            CS_ASSERT(jClass != nullptr, "Could not find Java class: '" + m_className + "'");

            //create an instance of the class
            CS_ASSERT(JavaMethodSignature::CalcReturnType(in_javaClassDef.GetConstructorSignature()) == JavaMethodSignature::ReturnType::k_void, "Cannot call constructor for Java class '" + m_className + "' because a non-void return type was specified.");
            CS_ASSERT(JavaMethodSignature::CalcNumArguments(in_javaClassDef.GetConstructorSignature()) == sizeof...(TArgs), "Cannot call constructor for Java class '" + m_className + "' because an incorrect number of arguments were supplied.");

            jmethodID jConstructor = environment->GetMethodID(jClass, "<init>", in_javaClassDef.GetConstructorSignature().c_str());
            CS_ASSERT(jConstructor != nullptr, "Could not find constructor with signature '" + in_javaClassDef.GetConstructorSignature()+ "' in java class '" + m_className + "'");
            jobject jClassInstance = environment->NewObject(jClass, jConstructor, std::forward<TArgs>(in_args)...);

            JavaUtils::CheckJavaExceptions("A java exception occurred during construction of Java class: '" + m_className + "'");
            CS_ASSERT(jClassInstance != nullptr, "Could not create instance of Java class: '" + m_className + "'");

            m_javaObject = environment->NewGlobalRef(jClassInstance);

            //setup the method references
            for (const auto& method : in_javaClassDef.GetMethods())
            {
                CS_ASSERT(m_methods.find(method.first) == m_methods.end(), "Method '" + method.first + "' has already been added to Java class '" + m_className + "'");

                MethodInfo info;
                info.m_returnType = JavaMethodSignature::CalcReturnType(method.second);
                info.m_numArguments = JavaMethodSignature::CalcNumArguments(method.second);
                info.m_methodId = environment->GetMethodID(jClass, method.first.c_str(), method.second.c_str());

                CS_ASSERT(info.m_methodId != nullptr, "Could not find method '" + method.first + "' in Java Class '" + m_className + "'");

                m_methods.emplace(method.first, info);
            }

            environment->DeleteLocalRef(jClassInstance);
            environment->DeleteLocalRef(jClass);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename... TArgs> void JavaClass::CallVoidMethod(const std::string& in_methodName, TArgs&&... in_args) const
        {
            auto environment = JavaVirtualMachine::Get()->GetJNIEnvironment();
            auto methodId = GetMethodId(in_methodName, JavaMethodSignature::ReturnType::k_void, sizeof...(TArgs));

            environment->CallVoidMethod(m_javaObject, methodId, std::forward<TArgs>(in_args)...);
            JavaUtils::CheckJavaExceptions("An exception was thrown while calling method '" + in_methodName + "' in Java class '" + m_className + "'.");
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename... TArgs> bool JavaClass::CallBoolMethod(const std::string& in_methodName, TArgs&&... in_args) const
        {
            auto environment = JavaVirtualMachine::Get()->GetJNIEnvironment();
            auto methodId = GetMethodId(in_methodName, JavaMethodSignature::ReturnType::k_bool, sizeof...(TArgs));

            auto output = environment->CallBooleanMethod(m_javaObject, methodId, std::forward<TArgs>(in_args)...);
            JavaUtils::CheckJavaExceptions("An exception was thrown while calling method '" + in_methodName + "' in Java class '" + m_className + "'.");

            return output;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename... TArgs> u8 JavaClass::CallByteMethod(const std::string& in_methodName, TArgs&&... in_args) const
        {
            auto environment = JavaVirtualMachine::Get()->GetJNIEnvironment();
            auto methodId = GetMethodId(in_methodName, JavaMethodSignature::ReturnType::k_byte, sizeof...(TArgs));

            auto output = environment->CallByteMethod(m_javaObject, methodId, std::forward<TArgs>(in_args)...);
            JavaUtils::CheckJavaExceptions("An exception was thrown while calling method '" + in_methodName + "' in Java class '" + m_className + "'.");

            return output;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename... TArgs> u16 JavaClass::CallCharMethod(const std::string& in_methodName, TArgs&&... in_args) const
        {
            auto environment = JavaVirtualMachine::Get()->GetJNIEnvironment();
            auto methodId = GetMethodId(in_methodName, JavaMethodSignature::ReturnType::k_char, sizeof...(TArgs));

            auto output = environment->CallCharMethod(m_javaObject, methodId, std::forward<TArgs>(in_args)...);
            JavaUtils::CheckJavaExceptions("An exception was thrown while calling method '" + in_methodName + "' in Java class '" + m_className + "'.");

            return output;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename... TArgs> s16 JavaClass::CallShortMethod(const std::string& in_methodName, TArgs&&... in_args) const
        {
            auto environment = JavaVirtualMachine::Get()->GetJNIEnvironment();
            auto methodId = GetMethodId(in_methodName, JavaMethodSignature::ReturnType::k_short, sizeof...(TArgs));

            auto output = environment->CallShortMethod(m_javaObject, methodId, std::forward<TArgs>(in_args)...);
            JavaUtils::CheckJavaExceptions("An exception was thrown while calling method '" + in_methodName + "' in Java class '" + m_className + "'.");

            return output;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename... TArgs> s32 JavaClass::CallIntMethod(const std::string& in_methodName, TArgs&&... in_args) const
        {
            auto environment = JavaVirtualMachine::Get()->GetJNIEnvironment();
            auto methodId = GetMethodId(in_methodName, JavaMethodSignature::ReturnType::k_int, sizeof...(TArgs));

            auto output = environment->CallIntMethod(m_javaObject, methodId, std::forward<TArgs>(in_args)...);
            JavaUtils::CheckJavaExceptions("An exception was thrown while calling method '" + in_methodName + "' in Java class '" + m_className + "'.");

            return output;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename... TArgs> s64 JavaClass::CallLongMethod(const std::string& in_methodName, TArgs&&... in_args) const
        {
            auto environment = JavaVirtualMachine::Get()->GetJNIEnvironment();
            auto methodId = GetMethodId(in_methodName, JavaMethodSignature::ReturnType::k_long, sizeof...(TArgs));

            auto output = environment->CallLongMethod(m_javaObject, methodId, std::forward<TArgs>(in_args)...);
            JavaUtils::CheckJavaExceptions("An exception was thrown while calling method '" + in_methodName + "' in Java class '" + m_className + "'.");

            return output;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename... TArgs> f32 JavaClass::CallFloatMethod(const std::string& in_methodName, TArgs&&... in_args) const
        {
            auto environment = JavaVirtualMachine::Get()->GetJNIEnvironment();
            auto methodId = GetMethodId(in_methodName, JavaMethodSignature::ReturnType::k_float, sizeof...(TArgs));

            f32 output = environment->CallFloatMethod(m_javaObject, methodId, std::forward<TArgs>(in_args)...);
            JavaUtils::CheckJavaExceptions("An exception was thrown while calling method '" + in_methodName + "' in Java class '" + m_className + "'.");

            return output;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename... TArgs> f64 JavaClass::CallDoubleMethod(const std::string& in_methodName, TArgs&&... in_args) const
        {
            auto environment = JavaVirtualMachine::Get()->GetJNIEnvironment();
            auto methodId = GetMethodId(in_methodName, JavaMethodSignature::ReturnType::k_double, sizeof...(TArgs));

            auto output = environment->CallDoubleMethod(m_javaObject, methodId, std::forward<TArgs>(in_args)...);
            JavaUtils::CheckJavaExceptions("An exception was thrown while calling method '" + in_methodName + "' in Java class '" + m_className + "'.");

            return output;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename... TArgs> jobject JavaClass::CallObjectMethod(const std::string& in_methodName, TArgs&&... in_args) const
        {
            auto environment = JavaVirtualMachine::Get()->GetJNIEnvironment();
            auto methodId = GetMethodId(in_methodName, JavaMethodSignature::ReturnType::k_object, sizeof...(TArgs));

            auto output = environment->CallObjectMethod(m_javaObject, methodId, std::forward<TArgs>(in_args)...);
            JavaUtils::CheckJavaExceptions("An exception was thrown while calling method '" + in_methodName + "' in Java class '" + m_className + "'.");

            return output;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename... TArgs> std::string JavaClass::CallStringMethod(const std::string& in_methodName, TArgs&&... in_args) const
        {
            auto environment = JavaVirtualMachine::Get()->GetJNIEnvironment();
            auto methodId = GetMethodId(in_methodName, JavaMethodSignature::ReturnType::k_string, sizeof...(TArgs));

            auto jString = static_cast<jstring>(environment->CallObjectMethod(m_javaObject, methodId, std::forward<TArgs>(in_args)...));
            JavaUtils::CheckJavaExceptions("An exception was thrown while calling method '" + in_methodName + "' in Java class '" + m_className + "'.");

            std::string output = JavaUtils::CreateSTDStringFromJString(jString);
            environment->DeleteLocalRef(jString);

            return output;
        }
	}
}

#endif

#endif
