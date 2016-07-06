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

#include <CSBackend/Platform/Android/Main/JNI/Core/Java/JavaVirtualMachine.h>
#include <CSBackend/Platform/Android/Main/JNI/Core/Java/JavaUtils.h>

namespace CSBackend
{
	namespace Android
	{
        //------------------------------------------------------------------------------
        JavaVirtualMachine::JavaVirtualMachine(JavaVM* in_javaVirtualMachine) noexcept
            : m_javaVirtualMachine(in_javaVirtualMachine)
        {
            constexpr char k_activityClassName[] = "com/chilliworks/chillisource/core/CSActivity";

            CS_ASSERT(m_javaVirtualMachine != nullptr, "The java virtual machine cannot be null.");

            auto environment = GetJNIEnvironment();

            auto activityClass = environment->FindClass(k_activityClassName);
            auto activityClassClass = environment->GetObjectClass(activityClass);
            auto getClassLoaderMethod = environment->GetMethodID(activityClassClass, "getClassLoader", "()Ljava/lang/ClassLoader;");
            auto classLoader = environment->CallObjectMethod(activityClass, getClassLoaderMethod);
            m_classLoader = environment->NewGlobalRef(classLoader);

            auto classLoaderClass = environment->GetObjectClass(m_classLoader);
            m_findClassMethod = environment->GetMethodID(classLoaderClass, "findClass", "(Ljava/lang/String;)Ljava/lang/Class;");

            environment->DeleteLocalRef(activityClass);
            environment->DeleteLocalRef(activityClassClass);
            environment->DeleteLocalRef(classLoader);
            environment->DeleteLocalRef(classLoaderClass);
        }

        //------------------------------------------------------------------------------
        void JavaVirtualMachine::AttachCurrentThread() noexcept
        {
            JNIEnv* environment = nullptr;
            m_javaVirtualMachine->AttachCurrentThread(&environment, nullptr);
        }

        //------------------------------------------------------------------------------
        JNIEnv* JavaVirtualMachine::GetJNIEnvironment() noexcept
        {
            CS_ASSERT(m_javaVirtualMachine != nullptr, "The java virtual machine cannot be null.");

            JNIEnv* environment = nullptr;
        	m_javaVirtualMachine->AttachCurrentThread(&environment, nullptr);
        	CS_ASSERT(environment != nullptr, "The jni environment cannot be null.");

        	return environment;
        }

        //------------------------------------------------------------------------------
        void JavaVirtualMachine::DetachCurrentThread() noexcept
        {
            m_javaVirtualMachine->DetachCurrentThread();
        }

        //------------------------------------------------------------------------------
        jclass JavaVirtualMachine::FindClass(const std::string& className) noexcept
        {
            auto environment = GetJNIEnvironment();

            auto javaClassName = JavaUtils::CreateJStringFromSTDString(className);
            auto outputClass = environment->CallObjectMethod(m_classLoader, m_findClassMethod, javaClassName);

            JavaUtils::CheckJavaExceptions("An exception was thrown while finding Java class '" + className + "'.");

            environment->DeleteLocalRef(javaClassName);

            return static_cast<jclass>(outputClass);
        }
	}
}

#endif
