//
//  Instance.cpp
//  Chilli Source
//  Created by Scott Downie on 30/11/2011.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2011 Tag Games Limited
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

#include <ChilliSource/Core/Reflection/Instance.h>
#include <ChilliSource/Core/Reflection/MetaClass.h>

#include <ChilliSource/Core/Cryptographic/HashCRC32.h>

namespace ChilliSource
{
    namespace Core
    {
        namespace Reflect
        {
            //--------------------------------------------------------
            /// Constructor
            ///
            /// @param Instance name
            /// @param Meta-class of which this is an instance
            //--------------------------------------------------------
            CInstance::CInstance(const std::string& instrName, CMetaClass* inpClass)
            :mstrName(instrName), mpClass(inpClass), mpInstance(nullptr)
            {
                mudwNameHash = HashCRC32::GenerateHashCode(instrName);
            }
            //--------------------------------------------------------
            /// Create 
            ///
            /// Create a new instance of the class represented by
            /// this meta class
            ///
            /// @param Instance name
            /// @param Meta class
            /// @return Instance of class
            //--------------------------------------------------------
            CInstance* CInstance::Create(const std::string& instrName, CMetaClass* inpMetaClass)
            {
                CInstance* pInstance = new CInstance(instrName, inpMetaClass);
                pInstance->SetInstance(inpMetaClass->Instantiate(pInstance));
                inpMetaClass->AddInstance(pInstance);
                return pInstance;
            } 
            //--------------------------------------------------------
            /// Create 
            ///
            /// Create a new meta instance of the class represented by
            /// the given meta class with the given instance
            ///
            /// @param Instance pointer
            /// @param Instance name
            /// @param Meta class
            /// @return Instance of class
            //--------------------------------------------------------
            CInstance* CInstance::Create(void* inpInstance, const std::string& instrName, CMetaClass* inpMetaClass)
            {
                CInstance* pInstance = new CInstance(instrName, inpMetaClass);
                pInstance->SetInstance(inpInstance);
                inpMetaClass->AddInstance(pInstance);
                return pInstance;
            }
            //--------------------------------------------------------
            /// Destroy 
            ///
            /// Delete this instance of the class represented by
            /// the given meta class
            //--------------------------------------------------------
            void CInstance::Destroy()
            {
                mpClass->RemoveInstance(this);
                mpClass->DestroyInstance(mpInstance);
            }
            //--------------------------------------------------------
            /// Get Name Hash
            ///
            /// @return The name identifier of this instance
            //--------------------------------------------------------
            u32 CInstance::GetNameHash() const
            {
                return mudwNameHash;
            }
            //--------------------------------------------------------
            /// Get Name
            ///
            /// @return The name identifier of this instance
            //--------------------------------------------------------
            const std::string& CInstance::GetName() const
            {
                return mstrName;
            }
            //--------------------------------------------------------
            /// Set Name
            ///
            /// @param The name identifier for this instance
            //--------------------------------------------------------
            void CInstance::SetName(const std::string& instrName)
            {
                mstrName = instrName;
                mudwNameHash = HashCRC32::GenerateHashCode(instrName);
            }
            //--------------------------------------------------------
            /// Get Class
            ///
            /// @return The meta class for this instance
            //--------------------------------------------------------
            CMetaClass* CInstance::GetClass() const
            {
                return mpClass;
            }
            //--------------------------------------------------------
            /// Set Instance
            ///
            /// @param The concrete instance wrapped by the meta-type
            //--------------------------------------------------------
            void CInstance::SetInstance(void* inpInstance)
            {
                mpInstance = inpInstance;
            }
            //--------------------------------------------------------
            /// Get Instance
            ///
            /// @return The concrete instance wrapped by the meta-type
            //--------------------------------------------------------
            void* CInstance::GetInstance() const
            {
                return mpInstance;
            }
            //--------------------------------------------------------
            /// Destructor
            //--------------------------------------------------------
            CInstance::~CInstance()
            {
                mpClass->DestroyInstance(mpInstance);
            }
        }
    }
}

