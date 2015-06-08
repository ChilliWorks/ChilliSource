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

#include <CSBackend/Platform/Android/Main/JNI/Core/Java/JavaStaticClass.h>

#include <ChilliSource/Core/String/StringUtils.h>

#include <regex>

namespace CSBackend
{
	namespace Android
	{
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        JavaStaticClass::JavaStaticClass(const JavaStaticClassDef& in_javaStaticClassDef)
        {
            auto environment = JavaVirtualMachine::Get()->GetJNIEnvironment();

            //Get the class
            m_className = in_javaStaticClassDef.GetClassName();
            jclass jClass = environment->FindClass(m_className.c_str());
            CS_ASSERT(jClass != nullptr, "Could not find Java class: '" + m_className + "'");

            m_javaClass = static_cast<jclass>(environment->NewGlobalRef(jClass));
            environment->DeleteLocalRef(jClass);

            //setup the method references
            for (const auto& method : in_javaStaticClassDef.GetMethods())
            {
                CS_ASSERT(m_methods.find(method.first) == m_methods.end(), "Method '" + method.first + "' has already been added to Java static class '" + m_className + "'");

                MethodInfo info;
                info.m_returnType = CalcReturnType(method.second);
                info.m_numArguments = CalcNumArguments(method.second);
                info.m_methodId = environment->GetStaticMethodID(m_javaClass, method.first.c_str(), method.second.c_str());

                CS_ASSERT(info.m_methodId != nullptr, "Could not find method '" + method.first + "' in Java static class '" + m_className + "'");

                m_methods.emplace(method.first, info);
            }
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void JavaStaticClass::CheckJavaExceptions(const std::string& in_errorMessage) const
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
        JavaStaticClass::ReturnType JavaStaticClass::CalcReturnType(const std::string& in_methodSignature) const
        {
            std::regex argsExpression("\\([a-zA-Z0-9/;\\[]*\\)");
            auto outputType = std::regex_replace(in_methodSignature, argsExpression, "");

            if (outputType == "V")
            {
                return ReturnType::k_void;
            }
            else if (outputType == "Z")
            {
                return ReturnType::k_bool;
            }
            else if (outputType == "B")
            {
                return ReturnType::k_byte;
            }
            else if (outputType == "C")
            {
                return ReturnType::k_char;
            }
            else if (outputType == "S")
            {
                return ReturnType::k_short;
            }
            else if (outputType == "I")
            {
                return ReturnType::k_int;
            }
            else if (outputType == "J")
            {
                return ReturnType::k_long;
            }
            else if (outputType == "F")
            {
                return ReturnType::k_float;
            }
            else if (outputType == "D")
            {
                return ReturnType::k_double;
            }
            else if (outputType == "Ljava/lang/String;")
            {
                return ReturnType::k_string;
            }
            else
            {
                std::regex objectExpression("L[a-zA-Z0-9/]*;");
                std::regex primitiveArrayExpression("\\[[ZBCSIJFD]");
                std::regex objectArrayExpression("\\[L[a-zA-Z0-9/]*;");

                if (std::regex_match(outputType, objectExpression) == true || std::regex_match(outputType, primitiveArrayExpression) == true ||
                    std::regex_match(outputType, objectArrayExpression) == true)
                {
                    return ReturnType::k_object;
                }
            }

            CS_LOG_FATAL("Could not determine return type for JavaStaticClass method signature: '" + in_methodSignature + "'");
            return ReturnType::k_void;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        u32 JavaStaticClass::CalcNumArguments(const std::string& in_methodSignature) const
        {
            std::smatch match;
            std::regex argsExpression("\\([a-zA-Z0-9/;\\[]*\\)");
            std::regex_search(in_methodSignature, match, argsExpression);

            std::regex bracketsExpression("\\(|\\)");
            auto arguments = std::regex_replace(match.str(), bracketsExpression, "");

            u32 count = 0;
            bool isClassName = false;
            for (auto character : arguments)
            {
                if (isClassName == false)
                {
                    if (character == 'Z' || character == 'B' || character == 'C' || character == 'S' || character == 'I' || character == 'J' || character == 'F' || character == 'D')
                    {
                        ++count;
                    }
                    else if (character == 'L')
                    {
                        isClassName = true;
                    }
                    else if (character != '[')
                    {
                        CS_LOG_FATAL("Could not calculate number of arguments for JavaStaticClass method signature '" + in_methodSignature + "' due to invalid character: '" + character + "'");
                    }
                }
                else if (character == ';')
                {
                    isClassName = false;
                    ++count;
                }
            }

            return count;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        jmethodID JavaStaticClass::GetMethodId(const std::string& in_methodName, ReturnType in_returnType, u32 in_numArguments) const
        {
            auto methodInfoIt = m_methods.find(in_methodName);
            if (methodInfoIt == m_methods.end())
            {
                CS_LOG_FATAL("Could not find method '" + in_methodName + "' in Java static class '" + m_className + "'");
            }

            CS_ASSERT(methodInfoIt->second.m_returnType == in_returnType, "Cannot call method '" + in_methodName + "' in Java static class '" + m_className + "' because the wrong return type was specified.");
            CS_ASSERT(methodInfoIt->second.m_numArguments == in_numArguments, "Cannot call method '" + in_methodName + "' in Java static class '" + m_className + "' because an incorrect number of arguments were supplied.");

            return methodInfoIt->second.m_methodId;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        JavaStaticClass::~JavaStaticClass()
        {
            auto environment = JavaVirtualMachine::Get()->GetJNIEnvironment();
            environment->DeleteGlobalRef(m_javaClass);
            m_javaClass = nullptr;
        }
    }
}

#endif
