//
//  JavaSystem.h
//  ChilliSource
//  Created by Ian Copland on 23/04/2015.
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

#ifndef _CSBACKEND_PLATFORM_ANDROID_MAIN_JNI_CORE_JAVA_JAVASYSTEM_H_
#define _CSBACKEND_PLATFORM_ANDROID_MAIN_JNI_CORE_JAVA_JAVASYSTEM_H_

#include <ChilliSource/ChilliSource.h>

#include <CSBackend/Platform/Android/Main/JNI/ForwardDeclarations.h>
#include <CSBackend/Platform/Android/Main/JNI/Core/Java/JavaClass.h>

#include <jni.h>

namespace CSBackend
{
	namespace Android
	{
		//------------------------------------------------------------------------------
		/// A specialised version of JavaClass, specifically for accessing classes which
		/// extend System. This ensures the init() and destroy() methods will be called
		/// as appropriate for the system.
		///
		/// @author Ian Copland
		//------------------------------------------------------------------------------
		class JavaSystem final
		{
		public:
            //------------------------------------------------------------------------------
            /// Constructor. Instantiates the underlying JavaSystem and calls the init()
            /// method.
            /// @author Ian Copland
            ///
            /// @param in_javaClassDef - The definition for this java system.
            //------------------------------------------------------------------------------
            template <typename... TArgs> JavaSystem(const JavaClassDef& in_javaClassDef, TArgs&&... in_args);
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
            ~JavaSystem();

        private:

            JavaClassUPtr m_javaClass;
		};
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename... TArgs> JavaSystem::JavaSystem(const JavaClassDef& in_javaClassDef, TArgs&&... in_args)
        {
            JavaClassDef updatedDef(in_javaClassDef.GetClassName(), in_javaClassDef.GetConstructorSignature());
            updatedDef.AddMethod("init", "()V");
            updatedDef.AddMethod("destroy", "()V");
            for (const auto& methodInfo : in_javaClassDef.GetMethods())
            {
                updatedDef.AddMethod(methodInfo.first, methodInfo.second);
            }
            m_javaClass = JavaClassUPtr(new JavaClass(updatedDef, std::forward<TArgs>(in_args)...));

            m_javaClass->CallVoidMethod("init");
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename... TArgs> void JavaSystem::CallVoidMethod(const std::string& in_methodName, TArgs&&... in_args) const
        {
            m_javaClass->CallVoidMethod(in_methodName, std::forward<TArgs>(in_args)...);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename... TArgs> bool JavaSystem::CallBoolMethod(const std::string& in_methodName, TArgs&&... in_args) const
        {
            return m_javaClass->CallBoolMethod(in_methodName, std::forward<TArgs>(in_args)...);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename... TArgs> u8 JavaSystem::CallByteMethod(const std::string& in_methodName, TArgs&&... in_args) const
        {
            return m_javaClass->CallByteMethod(in_methodName, std::forward<TArgs>(in_args)...);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename... TArgs> u16 JavaSystem::CallCharMethod(const std::string& in_methodName, TArgs&&... in_args) const
        {
            return m_javaClass->CallCharMethod(in_methodName, std::forward<TArgs>(in_args)...);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename... TArgs> s16 JavaSystem::CallShortMethod(const std::string& in_methodName, TArgs&&... in_args) const
        {
            return m_javaClass->CallShortMethod(in_methodName, std::forward<TArgs>(in_args)...);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename... TArgs> s32 JavaSystem::CallIntMethod(const std::string& in_methodName, TArgs&&... in_args) const
        {
            return m_javaClass->CallIntMethod(in_methodName, std::forward<TArgs>(in_args)...);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename... TArgs> s64 JavaSystem::CallLongMethod(const std::string& in_methodName, TArgs&&... in_args) const
        {
            return m_javaClass->CallLongMethod(in_methodName, std::forward<TArgs>(in_args)...);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename... TArgs> f32 JavaSystem::CallFloatMethod(const std::string& in_methodName, TArgs&&... in_args) const
        {
            return m_javaClass->CallFloatMethod(in_methodName, std::forward<TArgs>(in_args)...);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename... TArgs> f64 JavaSystem::CallDoubleMethod(const std::string& in_methodName, TArgs&&... in_args) const
        {
            return m_javaClass->CallDoubleMethod(in_methodName, std::forward<TArgs>(in_args)...);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename... TArgs> jobject JavaSystem::CallObjectMethod(const std::string& in_methodName, TArgs&&... in_args) const
        {
            return m_javaClass->CallObjectMethod(in_methodName, std::forward<TArgs>(in_args)...);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename... TArgs> std::string JavaSystem::CallStringMethod(const std::string& in_methodName, TArgs&&... in_args) const
        {
            return m_javaClass->CallStringMethod(in_methodName, std::forward<TArgs>(in_args)...);
        }
	}
}

#endif

#endif
