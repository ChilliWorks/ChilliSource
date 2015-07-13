//
//  JavaStaticClassDef.cpp
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

#include <CSBackend/Platform/Android/Main/JNI/Core/Java/JavaStaticClassDef.h>

namespace CSBackend
{
	namespace Android
	{
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        JavaStaticClassDef::JavaStaticClassDef(const std::string& in_className)
            : m_className(in_className)
        {
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void JavaStaticClassDef::AddMethod(const std::string& in_methodName, const std::string& in_methodSignature)
        {
            CS_ASSERT((m_methods.find(in_methodName) == m_methods.end()), "Cannot add method '" + in_methodName + "' to static class definition '" + m_className + "' because the method name already exists.");

            m_methods.emplace(in_methodName, in_methodSignature);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        const std::string& JavaStaticClassDef::GetClassName() const
        {
            return m_className;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        const JavaStaticClassDef::MethodMap& JavaStaticClassDef::GetMethods() const
        {
            return m_methods;
        }
	}
}

#endif
