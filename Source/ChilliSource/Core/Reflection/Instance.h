//
//  Instance.h
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

#ifndef _CHILLISOURCE_CORE_REFLECTION_INSTANCE_HPP_
#define _CHILLISOURCE_CORE_REFLECTION_INSTANCE_HPP_

#include <ChilliSource/ChilliSource.h>

namespace ChilliSource
{
    namespace Core
    {
        namespace Reflect
        {
            //===============================================================================
            /// Instance
            ///
            /// An instance of a class object. This holds meta information relating to the
            /// class.  
            //===============================================================================
            class CInstance
            {
            public:
                ~CInstance();
                //--------------------------------------------------------
                /// Create 
                ///
                /// Create a new instance of the class represented by
                /// the given meta class
                ///
                /// @param Instance name
                /// @param Meta class
                /// @return Instance of class
                //--------------------------------------------------------
                static CInstance* Create(const std::string& instrName, CMetaClass* inpMetaClass);
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
                static CInstance* Create(void* inpInstance, const std::string& instrName, CMetaClass* inpMetaClass);
                //--------------------------------------------------------
                /// Destroy 
                ///
                /// Delete this instance of the class represented by
                /// the given meta class
                //--------------------------------------------------------
                void Destroy();
                //--------------------------------------------------------
                /// Create 
                ///
                /// @return Instance of class
                //--------------------------------------------------------
                template <typename T> static void* Create(CInstance* inpInstance)
                {
                    T* pInstance = new T();
                    pInstance->SetMetaInstance(inpInstance);
                    return pInstance;
                }
                //--------------------------------------------------------
                /// Destroy 
                ///
                /// Destruct the instance
                /// @param Instance
                //--------------------------------------------------------
                template <typename T> static void Destroy(void* inpInstance)
                {
                    delete static_cast<T*>(inpInstance);
                } 
                //--------------------------------------------------------
                /// Get Name Hash
                ///
                /// @return The name identifier of this instance
                //--------------------------------------------------------
                u32 GetNameHash() const;
                //--------------------------------------------------------
                /// Get Name
                ///
                /// @return The name identifier of this instance
                //--------------------------------------------------------
                const std::string& GetName() const;
                //--------------------------------------------------------
                /// Set Name
                ///
                /// @param The name identifier for this instance
                //--------------------------------------------------------
                void SetName(const std::string& instrName);
                //--------------------------------------------------------
                /// Get Class
                ///
                /// @return The meta class for this instance
                //--------------------------------------------------------
                CMetaClass* GetClass() const;
                //--------------------------------------------------------
                /// Set Instance
                ///
                /// @param The concrete instance wrapped by the meta-type
                //--------------------------------------------------------
                void SetInstance(void* inpInstance);
                //--------------------------------------------------------
                /// Get Instance
                ///
                /// @return The concrete instance wrapped by the meta-type
                //--------------------------------------------------------
                void* GetInstance() const;
                
            private:
                
                //--------------------------------------------------------
                /// Constructor
                ///
                /// @param Instance name
                /// @param Meta-class of which this is an instance
                //--------------------------------------------------------
                CInstance(const std::string& instrName, CMetaClass* inpClass);
                
            private:
                
                std::string mstrName;
                
                CMetaClass* mpClass;
                
                void* mpInstance;
                
                u32 mudwNameHash;
            };
        }
    }
}

#endif
