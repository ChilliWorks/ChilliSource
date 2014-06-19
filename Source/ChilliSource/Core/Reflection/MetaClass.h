//
//  MetaClass.h
//  Chilli Source
//  Created by Scott Downie on 29/11/2011.
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

#ifndef MO_FLOW_CORE_REFLECTION_META_CLASS_HPP_
#define MO_FLOW_CORE_REFLECTION_META_CLASS_HPP_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Reflection/Instance.h>
#include <ChilliSource/Core/Reflection/Registry.h>

#include <functional>
#include <unordered_map>
#include <vector>

namespace ChilliSource
{
    namespace Core
    {
        namespace Reflect
        {
            typedef std::function<void*(CInstance*)> InstanceCreateDelegate;
            typedef std::function<void(void*)> InstanceDestroyDelegate;
            
            //===============================================================================
            /// Meta Class
            ///
            /// Meta data relating to a class. Allows the class to be accessed and created
            /// by name. Now two meta class instances should have the same name and hence
            /// the name is part of the class template 
            //===============================================================================
            class CMetaClass
            {
            public:
                ~CMetaClass();
                //------------------------------------------------------------
                /// Create
                ///
                /// Helper function to create and setup the meta class for
                /// the given class type T. 
                ///
                /// @param Class name
                /// @return Meta class for T
                //------------------------------------------------------------
                template <typename T> static CMetaClass* Create(const std::string& instrName)
                {
                    CMetaClass* pClass = new CMetaClass(instrName, sizeof(T));
                    pClass->RegisterInstanceCreateDelegate(InstanceCreateDelegate(&CInstance::Create<T>));
                    pClass->RegisterInstanceDestroyDelegate(InstanceDestroyDelegate(&CInstance::Destroy<T>));
                    CRegistry::AddClass(pClass);
                    return pClass;
                }
                //--------------------------------------------------------
                /// Instantiate
                ///
                /// Instantiate a concrete instance of this class type
                /// 
                /// @return New class instance
                //--------------------------------------------------------
                void* Instantiate(CInstance* inpInstance) const;
                //--------------------------------------------------------
                /// Destroy Instance
                ///
                /// Delete the concrete instance of the meta instance
                /// 
                /// @param Instance 
                //--------------------------------------------------------
                void DestroyInstance(void* inpInstance) const;
                //--------------------------------------------------------
                /// Add Property
                ///
                /// @param Property 
                //--------------------------------------------------------
                void AddProperty(CProperty* inpProp);
                //--------------------------------------------------------
                /// Add Instance
                ///
                /// @param Instance 
                //--------------------------------------------------------
                void AddInstance(CInstance* inpInstance);
                //--------------------------------------------------------
                /// Remove Instance
                ///
                /// @param Instance 
                //--------------------------------------------------------
                void RemoveInstance(CInstance* inpInstance);
                //--------------------------------------------------------
                /// Get Properties
                ///
                /// @param Vector of properties for this meta-class
                //--------------------------------------------------------
                const std::vector<CProperty*>& GetProperties() const;
                //--------------------------------------------------------
                /// Get Property
                ///
                /// @param Propery name identifier
                /// @return The property pointer of the this class with 
                /// the given name
                //--------------------------------------------------------
                CProperty* GetProperty(const std::string& instrPropName) const;
                //--------------------------------------------------------
                /// Get Instance
                ///
                /// @param Instance name identifier
                /// @return The instance pointer of the this class with 
                /// the given name
                //--------------------------------------------------------
                CInstance* GetInstance(const std::string& instrName) const;
                //--------------------------------------------------------
                /// Get Instance
                ///
                /// @param Instance pointer
                /// @return The instance pointer of the this class with 
                /// the given name
                //--------------------------------------------------------
                CInstance* GetInstance(void* inpInstance) const;
                //--------------------------------------------------------
                /// Get Base Class
                ///
                /// @return The meta class of the class from which this
                /// object derives
                //--------------------------------------------------------
                CMetaClass* GetBaseClass() const;
                //--------------------------------------------------------
                /// Set Base Class
                ///
                /// @param The meta class of the class from which this
                /// object derives
                //--------------------------------------------------------
                void SetBaseClass(CMetaClass* inpClass);
                //--------------------------------------------------------
                /// Get Name
                ///
                /// @return The name identifier of this property
                //--------------------------------------------------------
                const std::string& GetName() const;
                //--------------------------------------------------------
                /// Get Name Hash
                ///
                /// @return The hashed name identifier of this property
                //--------------------------------------------------------
                u32 GetNameHash() const;
                //--------------------------------------------------------
                /// Get Size
                ///
                /// @return Sizeof the actual class
                //--------------------------------------------------------
                u32 GetSize() const;
                //--------------------------------------------------------
                /// Register Instance Create Delegate
                ///
                /// @param Instance creation delegate
                //--------------------------------------------------------
                void RegisterInstanceCreateDelegate(const InstanceCreateDelegate& inDelegate);
                //--------------------------------------------------------
                /// Register Instance Destroy Delegate
                ///
                /// @param Instance destroy delegate
                //--------------------------------------------------------
                void RegisterInstanceDestroyDelegate(const InstanceDestroyDelegate& inDelegate);

            private:

                //--------------------------------------------------------
                /// Constructor
                ///
                /// @param Name
                //--------------------------------------------------------
                CMetaClass(const std::string& instrName, u32 inudwSize);
                   
            private:
                
                std::vector<CProperty*> mProperties;
                std::vector<CInstance*> mInstances;

                CMetaClass* mpBaseClass;
                
                std::string mstrName;
                
                u32 mudwSize;
                u32 mudwNameHash;
                
                typedef std::unordered_map<u32, InstanceCreateDelegate> MapMetaClassToInstanceCreateDelegate;
                static MapMetaClassToInstanceCreateDelegate mapClassToInstanceCreate;
                
                typedef std::unordered_map<u32, InstanceDestroyDelegate> MapMetaClassToInstanceDestroyDelegate;
                static MapMetaClassToInstanceDestroyDelegate mapClassToInstanceDestroy;
            };
        }
    }
}

#endif
