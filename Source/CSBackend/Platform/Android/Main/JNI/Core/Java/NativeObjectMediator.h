//
//  NativeObjectMediator
//  ChilliSource
//  Created by HMcLaughlin on 31/08/2015.
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

#ifndef _CSBACKEND_PLATFORM_ANDROID_MAIN_JNI_CORE_JAVA_NATIVEOBJECTMEDIATOR_H_
#define _CSBACKEND_PLATFORM_ANDROID_MAIN_JNI_CORE_JAVA_NATIVEOBJECTMEDIATOR_H_

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
        namespace NativeObjectMediator
        {
            //------------------------------------------------------------------------------
            /// Gets the definition for a native pointer java class
            ///
            /// @author HMcLaughlin
            ///
            /// @return Native pointer def
            //------------------------------------------------------------------------------
            JavaClassDef GetNativePointerClassDef()
            {
                JavaClassDef javaNativePointerClassDef("com/chilliworks/chillisource/core/NativePointer", "(JJ)V");
                javaNativePointerClassDef.AddMethod("getPointerAddress", "()J");
                javaNativePointerClassDef.AddMethod("getTypeHash", "()J");
                
                return javaNativePointerClassDef;
            }
            //------------------------------------------------------------------------------
            /// Generates a unique number for a templated type
            ///
            /// @author HMcLaughlin
            ///
            /// @return Unique ID
            //------------------------------------------------------------------------------
            template <typename TType> u64 GenerateId()
            {
                auto typeIndex = std::type_index(typeid(TType));
                return static_cast<u64>(typeIndex.hash_code());
            }
            //------------------------------------------------------------------------------
            /// Casts a native pointer to a Java Native Pointer container
            ///
            /// @author HMcLaughlin
            ///
            /// @param Object pointer
            ///
            /// @return Java Object containing pointer info
            //------------------------------------------------------------------------------
            template <typename TType> JavaClassSPtr NativeToJavaPointer(TType* in_type)
            {
                //Create a java NativePointer for this instance
                JavaClassDef javaNativePointerClassDef = GetNativePointerClassDef();
                
                s64 pointerAddress = (s64)in_type;
                s64 pointerTypeHash = GenerateId<TType>();

                JavaClassSPtr javaNativePointerClass = JavaClassSPtr(new JavaClass(javaNativePointerClassDef, pointerAddress, pointerTypeHash));
                return javaNativePointerClass;
            }
            //------------------------------------------------------------------------------
            /// Casts a java pointer container back to a c++ pointer of the original type
            ///
            /// @author HMcLaughlin
            ///
            /// @param Java Object containing pointer info
            ///
            /// @return Native pointer
            //------------------------------------------------------------------------------
            template <typename TType> TType* JavaToNativePointer(const JavaClassSPtr& in_javaContainer)
            {
                CS_ASSERT(in_javaContainer->GetClassName() == GetNativePointerClassDef().GetClassName(), "Java object is not of type NativePointer! " + in_javaContainer->GetClassName());

                u64 pointerAddress = in_javaContainer->CallLongMethod("getPointerAddress");
                u64 pointerTypeHash = in_javaContainer->CallLongMethod("getTypeHash");
                
                CS_ASSERT(pointerTypeHash == GenerateId<TType>(), "Failed to cast Java pointer back to c pointer, type mismatch!");
                
                return reinterpret_cast<TType*>(pointerAddress);
            }
        }
    }
}

#endif

#endif
