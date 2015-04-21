//
//  JavaClass.cpp
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

#include <CSBackend/Platform/Android/Main/JNI/Core/Java/JavaClass.h>

namespace CSBackend
{
	namespace Android
	{
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        JavaClass::JavaClass(const JavaClassDef& in_javaClassDef)
        {
            auto environment = JavaVirtualMachine::Get()->GetJNIEnvironment();

            m_className = in_javaClassDef.GetClassName();
            jclass jClass = environment->FindClass(m_className.c_str());
            CS_ASSERT(jClass != nullptr, "Could not find Java class: '" + m_className + "'");

            jmethodID jConstructor = environment->GetMethodID(jClass, "<init>", "()V");
            jobject jClassInstance = environment->NewObject(jClass, jConstructor);
            CheckJavaExceptions("A java exception occurred during construction of Java class: '" + m_className + "'");
            CS_ASSERT(jClassInstance != nullptr, "Could not create instance of Java class: '" + m_className + "'");

            m_javaObject = environment->NewGlobalRef(jClassInstance);
            environment->DeleteLocalRef(jClassInstance);

            for (const auto& method : in_javaClassDef.GetMethods())
            {
                CS_ASSERT(m_methods.find(method.first) == m_methods.end(), "Method '" + method.first + "' has already been added to Java class '" + m_className + "'");

                MethodInfo info;
                info.m_methodType = CalcMethodType(method.second);
                info.m_numArguments = CalcNumArguments(method.second);
                info.m_methodId = environment->GetMethodID(jClass, method.first.c_str(), method.second.c_str());
                CS_ASSERT(info.m_methodId != nullptr, "Could not find method '" + method.first + "' in Java Class '" + m_className + "'");

                m_methods.emplace(method.first, info);
            }

            environment->DeleteLocalRef(jClass);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void JavaClass::CheckJavaExceptions(const std::string& in_errorMessage) const
        {
            auto environment = JavaVirtualMachine::Get()->GetJNIEnvironment();

            jthrowable exception = environment->ExceptionOccurred();
            if (exception != nullptr)
            {
                environment->ExceptionDescribe();
                environment->ExceptionClear();

                CS_LOG_FATAL(in_errorMessage);
            }
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        JavaClass::MethodType JavaClass::CalcMethodType(const std::string& in_methodSignature) const
        {
            //TODO:
            return MethodType::k_void;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        u32 JavaClass::CalcNumArguments(const std::string& in_methodSignature) const
        {
            //TODO:
            return 0;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        JavaClass::~JavaClass()
        {
            //TODO: Clean up!
        }
    }
}

#endif
