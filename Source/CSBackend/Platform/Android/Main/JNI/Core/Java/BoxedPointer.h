//
//  BoxedPointer
//  ChilliSource
//  Created by HMcLaughlin on 31/07/2015.
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

#ifndef _CSBACKEND_PLATFORM_ANDROID_MAIN_JNI_CORE_JAVA_BOXEDPOINTER_H_
#define _CSBACKEND_PLATFORM_ANDROID_MAIN_JNI_CORE_JAVA_BOXEDPOINTER_H_

#include <ChilliSource/ChilliSource.h>

#include <CSBackend/Platform/Android/Main/JNI/ForwardDeclarations.h>
#include <CSBackend/Platform/Android/Main/JNI/Core/Java/JavaClass.h>
#include <CSBackend/Platform/Android/Main/JNI/Core/Java/JavaClassDef.h>
#include <CSBackend/Platform/Android/Main/JNI/Core/Java/JavaMethodSignature.h>
#include <CSBackend/Platform/Android/Main/JNI/Core/Java/JavaUtils.h>
#include <CSBackend/Platform/Android/Main/JNI/Core/Java/JavaVirtualMachine.h>

#include <jni.h>

#include <typeindex>
#include <unordered_map>

namespace CSBackend
{
	namespace Android
	{
        namespace BoxedPointer
        {
            //------------------------------------------------------------------------------
            /// @author HMcLaughlin
            ///
            /// @return Definition for a java BoxedPointer class
            //------------------------------------------------------------------------------
            JavaClassDef GetBoxedPointerClassDef();
            //------------------------------------------------------------------------------
            /// Generates a unique number for a templated type
            ///
            /// @author HMcLaughlin
            ///
            /// @return Unique ID
            //------------------------------------------------------------------------------
            template <typename TType> s64 GenerateTypeHash();
            //------------------------------------------------------------------------------
            /// Wraps a native pointer in a java BoxedPointer container
            ///
            /// @author HMcLaughlin
            ///
            /// @param in_type - Object pointer
            ///
            /// @return Java Boxed Pointer containing pointer info
            //------------------------------------------------------------------------------
            template <typename TType> JavaClassUPtr Box(TType* in_type);
            //------------------------------------------------------------------------------
            /// Casts a java BoxedPointer back to a c++ pointer of the original type
            ///
            /// @author HMcLaughlin
            ///
            /// @param in_javaContainer - Java Object containing pointer info
            ///
            /// @return Native pointer
            //------------------------------------------------------------------------------
            template <typename TType> TType* Unbox(JavaClass* in_javaBoxedPointer);
        }

        namespace BoxedPointer
        {
            //------------------------------------------------------------------------------
            //------------------------------------------------------------------------------
            template <typename TType> s64 GenerateTypeHash()
            {
                auto typeIndex = std::type_index(typeid(TType));
                return static_cast<s64>(typeIndex.hash_code());
            }
            //------------------------------------------------------------------------------
            //------------------------------------------------------------------------------
            template <typename TType> JavaClassUPtr Box(TType* in_type)
            {
                //Create a java NativePointer for this instance
                JavaClassDef javaBoxedPointerClassDef = GetBoxedPointerClassDef();

                s64 pointerAddress = (s64)in_type;
                s64 pointerTypeHash = GenerateTypeHash<TType>();

                JavaClassUPtr javaBoxedPointerClass = JavaClassUPtr(new JavaClass(javaBoxedPointerClassDef, pointerAddress, pointerTypeHash));
                return javaBoxedPointerClass;
            }
            //------------------------------------------------------------------------------
            //------------------------------------------------------------------------------
            template <typename TType> TType* Unbox(JavaClass* in_javaBoxedPointer)
            {
                CS_ASSERT(in_javaBoxedPointer->GetClassName() == GetBoxedPointerClassDef().GetClassName(), "Cannot convert '" + in_javaBoxedPointer->GetClassName() + "', only BoxedPointer");

                s64 pointerAddress = in_javaBoxedPointer->CallLongMethod("getPointerAddress");

                CS_ASSERT(in_javaBoxedPointer->CallLongMethod("getTypeHash") == GenerateTypeHash<TType>(), "Failed to cast Java pointer back to c pointer, type mismatch!");

                return reinterpret_cast<TType*>(pointerAddress);
            }
        }
    }
}

#endif

#endif
