//
//  JavaSystem.cpp
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

#include <CSBackend/Platform/Android/Main/JNI/Core/Java/JavaSystem.h>

#include <CSBackend/Platform/Android/Main/JNI/Core/Java/JavaClassDef.h>

namespace CSBackend
{
	namespace Android
	{
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        JavaSystem::~JavaSystem()
        {
            m_javaClass->CallVoidMethod("destroy");
            m_javaClass.reset();
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        jobject JavaSystem::GetJavaObject() const
        {
            return m_javaClass->GetJavaObject();
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        const std::string& JavaSystem::GetClassName() const
        {
            return m_javaClass->GetClassName();
        }
    }
}

#endif
