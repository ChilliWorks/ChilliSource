//
//  MetaClass.hpp
//  MoFlowSkeleton
//
//  Created by Scott Downie on 29/11/2011.
//  Copyright (c) 2011 Tag Games. All rights reserved.
//

#include <ChilliSource/Core/Reflection/MetaClass.h>
#include <ChilliSource/Core/Reflection/Property.h>
#include <ChilliSource/Core/Reflection/Instance.h>

#include <ChilliSource/Core/Cryptographic/HashCRC32.h>

namespace ChilliSource
{
    namespace Core
    {
        namespace Reflect
        {
            CMetaClass::MapMetaClassToInstanceCreateDelegate CMetaClass::mapClassToInstanceCreate;
            CMetaClass::MapMetaClassToInstanceDestroyDelegate CMetaClass::mapClassToInstanceDestroy;
            
            //--------------------------------------------------------
            /// Constructor
            ///
            /// @param Name
            //--------------------------------------------------------
            CMetaClass::CMetaClass(const std::string& instrName, u32 inudwSize) : mstrName(instrName), mudwSize(inudwSize), mpBaseClass(NULL)
            {
                mudwNameHash = CHashCRC32::GenerateHashCode(instrName);
            }
            //--------------------------------------------------------
            /// Register Instance Create Delegate
            ///
            /// @param Instance creation delegate
            //--------------------------------------------------------
            void CMetaClass::RegisterInstanceCreateDelegate(const InstanceCreateDelegate& inDelegate)
            {
                mapClassToInstanceCreate.insert(std::make_pair(mudwNameHash, inDelegate));
            }
            //--------------------------------------------------------
            /// Register Instance Destroy Delegate
            ///
            /// @param Instance destroy delegate
            //--------------------------------------------------------
            void CMetaClass::RegisterInstanceDestroyDelegate(const InstanceDestroyDelegate& inDelegate)
            {
                mapClassToInstanceDestroy.insert(std::make_pair(mudwNameHash, inDelegate));
            }
            //--------------------------------------------------------
            /// Instantiate
            ///
            /// Instantiate a concrete instance of this class type
            /// 
            /// @return New class instance
            //--------------------------------------------------------
            void* CMetaClass::Instantiate(CInstance* inpInstance) const
            {
                MapMetaClassToInstanceCreateDelegate::const_iterator it = mapClassToInstanceCreate.find(mudwNameHash);
                if(it != mapClassToInstanceCreate.end())
                {
                    return (it->second)(inpInstance);
                }
                
                return NULL;
            }
            //--------------------------------------------------------
            /// Destroy Instance
            ///
            /// Delete the concrete instance of the meta instance
            /// 
            /// @param Instance 
            //--------------------------------------------------------
            void CMetaClass::DestroyInstance(void* inpInstance) const
            {
                MapMetaClassToInstanceDestroyDelegate::const_iterator it = mapClassToInstanceDestroy.find(mudwNameHash);
                if(it != mapClassToInstanceDestroy.end())
                {
                    (it->second)(inpInstance);
                }
            }
            //--------------------------------------------------------
            /// Add Property
            ///
            /// @param Property 
            //--------------------------------------------------------
            void CMetaClass::AddProperty(CProperty* inpProp)
            {
                mProperties.push_back(inpProp);
            }
            //--------------------------------------------------------
            /// Add Instance
            ///
            /// @param Instance 
            //--------------------------------------------------------
            void CMetaClass::AddInstance(CInstance* inpInstance)
            {
                mInstances.push_back(inpInstance);
            }
            //--------------------------------------------------------
            /// Remove Instance
            ///
            /// @param Instance 
            //--------------------------------------------------------
            void CMetaClass::RemoveInstance(CInstance* inpInstance)
            {
                for(DYNAMIC_ARRAY<CInstance*>::iterator it = mInstances.begin(); it != mInstances.end(); ++it)
                {
                    if(inpInstance == (*it))
                    {
                        mInstances.erase(it);
                        return;
                    }
                }
            }
            //--------------------------------------------------------
            /// Get Properties
            ///
            /// @param Vector of properties for this meta-class
            //--------------------------------------------------------
            const DYNAMIC_ARRAY<CProperty*>& CMetaClass::GetProperties() const
            {
                return mProperties;
            }
            //--------------------------------------------------------
            /// Get Property
            ///
            /// @param Propery name identifier
            /// @return The property pointer of the this class with 
            /// the given name
            //--------------------------------------------------------
            CProperty* CMetaClass::GetProperty(const std::string& instrPropName) const
            {
                u32 udwNameHash = CHashCRC32::GenerateHashCode(instrPropName);
                
                for(DYNAMIC_ARRAY<CProperty*>::const_iterator it = mProperties.begin(); it != mProperties.end(); ++it)
                {
                    if(udwNameHash == (*it)->GetNameHash())
                    {
                        return (*it);
                    }
                }

                return (mpBaseClass ? mpBaseClass->GetProperty(instrPropName) : NULL);
            }
            //--------------------------------------------------------
            /// Get Instance
            ///
            /// @param Instance name identifier
            /// @return The instance pointer of the this class with 
            /// the given name
            //--------------------------------------------------------
            CInstance* CMetaClass::GetInstance(const std::string& instrName) const
            {
                u32 udwNameHash = CHashCRC32::GenerateHashCode(instrName);
                
                for(DYNAMIC_ARRAY<CInstance*>::const_iterator it = mInstances.begin(); it != mInstances.end(); ++it)
                {
                    if(udwNameHash == (*it)->GetNameHash())
                    {
                        return (*it);
                    }
                }
                
                return NULL;
            }
            //--------------------------------------------------------
            /// Get Instance
            ///
            /// @param Instance pointer
            /// @return The instance pointer of the this class with 
            /// the given name
            //--------------------------------------------------------
            CInstance* CMetaClass::GetInstance(void* inpInstance) const
            {
                for(DYNAMIC_ARRAY<CInstance*>::const_iterator it = mInstances.begin(); it != mInstances.end(); ++it)
                {
                    if(inpInstance == (*it)->GetInstance())
                    {
                        return (*it);
                    }
                }

                return NULL;
            }
            //--------------------------------------------------------
            /// Get Base Class
            ///
            /// @return The meta class of the class from which this
            /// object derives
            //--------------------------------------------------------
            CMetaClass* CMetaClass::GetBaseClass() const
            {
                return mpBaseClass;
            }
            //--------------------------------------------------------
            /// Set Base Class
            ///
            /// @param The meta class of the class from which this
            /// object derives
            //--------------------------------------------------------
            void CMetaClass::SetBaseClass(CMetaClass* inpClass)
            {
                mpBaseClass = inpClass;
            }
            //--------------------------------------------------------
            /// Get Name
            ///
            /// @return The name identifier of this property
            //--------------------------------------------------------
            const std::string& CMetaClass::GetName() const
            {
                return mstrName;
            }
            //--------------------------------------------------------
            /// Get Name Hash
            ///
            /// @return The hashed name identifier of this property
            //--------------------------------------------------------
            u32 CMetaClass::GetNameHash() const
            {
                return mudwNameHash;
            }
            //--------------------------------------------------------
            /// Get Size
            ///
            /// @return Sizeof the actual class
            //--------------------------------------------------------
            u32 CMetaClass::GetSize() const
            {
                return mudwSize;
            }
            //--------------------------------------------------------
            /// Destructor
            //--------------------------------------------------------
            CMetaClass::~CMetaClass()
            {
                for(DYNAMIC_ARRAY<CProperty*>::iterator it = mProperties.begin(); it != mProperties.end(); ++it)
                {
                    delete (*it);
                }
                for(DYNAMIC_ARRAY<CInstance*>::iterator it = mInstances.begin(); it != mInstances.end(); ++it)
                {
                    delete (*it);
                }
            }
        }
    }
}

