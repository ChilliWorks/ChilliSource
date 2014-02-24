//
//  Property.hpp
//  MoFlowSkeleton
//
//  Created by Scott Downie on 29/11/2011.
//  Copyright (c) 2011 Tag Games. All rights reserved.
//

#ifndef MO_FLOW_CORE_REFLECTION_PROPERTY_HPP_
#define MO_FLOW_CORE_REFLECTION_PROPERTY_HPP_

#include <ChilliSource/Core/Reflection/ForwardDeclarations.h>
#include <ChilliSource/Core/Reflection/Instance.h>
#include <ChilliSource/Core/Reflection/MetaClass.h>

#include <ChilliSource/Core/Cryptographic/HashCRC32.h>

#include <ChilliSource/Core/Base/FastDelegate.h>

namespace ChilliSource
{
    namespace Core
    {
        namespace Reflect
        {
            template <typename PropType, typename ClassType> class CPropertyConcrete;
            
            class CProperty
            {
            public:
                virtual ~CProperty(){}
                //--------------------------------------------------------
                /// Create Property
                ///
                /// @param Property name
                /// @param Property variable
                /// @param Access (Read-write by default)
                /// @return Property belonging to this class type
                //--------------------------------------------------------
                template <typename ClassType, typename PropType> 
                static CProperty* Create(const std::string& instrName, PropType ClassType::* inpValue, const std::string& instrType, CMetaClass* inpMetaClass, PropAccess ineAccessType)
                {
                    CPropertyConcrete<PropType, ClassType>* pProp = new CPropertyConcrete<PropType, ClassType>();
                    pProp->mstrName = instrName;
                    pProp->meAccessType = ineAccessType;
                    pProp->mudwSize = sizeof(PropType);
                    pProp->mudwOffset = CPropertyConcrete<PropType, ClassType>::GetOffset(inpValue);
                    pProp->mpClass = inpMetaClass;
                    pProp->mpStaticValue = NULL;
                    pProp->mudwNameHash = CHashCRC32::GenerateHashCode(instrName);  
                    pProp->mstrType = instrType;
                    pProp->mudwTypeHash = CHashCRC32::GenerateHashCode(instrType);
                    inpMetaClass->AddProperty(pProp);
                    return pProp;
                }
                //--------------------------------------------------------
                /// Create Property (Static)
                ///
                /// @param Property name
                /// @param Property static variable
                /// @param Access (Read-write by default)
                /// @return Property belonging to this class type
                //--------------------------------------------------------
                template <typename ClassType, typename PropType> 
                static CProperty* Create(const std::string& instrName, PropType* inpValue, const std::string& instrType, CMetaClass* inpMetaClass, PropAccess ineAccessType)
                {
                    CPropertyConcrete<PropType, ClassType>* pProp = new CPropertyConcrete<PropType, ClassType>();
                    pProp->mstrName = instrName;
                    pProp->meAccessType = ineAccessType;
                    pProp->mudwSize = sizeof(PropType);
                    pProp->mpClass = inpMetaClass;
                    pProp->mpStaticValue = inpValue;
                    pProp->mudwNameHash = CHashCRC32::GenerateHashCode(instrName);
                    pProp->mstrType = instrType;
                    pProp->mudwTypeHash = CHashCRC32::GenerateHashCode(instrType);
                    inpMetaClass->AddProperty(pProp);
                    return pProp;
                }
                //--------------------------------------------------------
                /// Get Name Hash
                ///
                /// @return The hashed name identifier of this property
                //--------------------------------------------------------
                u32 GetNameHash() const;
                //--------------------------------------------------------
                /// Get Name
                ///
                /// @return The name identifier of this property
                //--------------------------------------------------------
                const std::string& GetName() const;
                //--------------------------------------------------------
                /// Get Type
                ///
                /// @return The name identifier of this property type
                //--------------------------------------------------------
                const std::string& GetType() const;
                //--------------------------------------------------------
                /// Get Type Hash
                ///
                /// @return The hashed type identifier of this property
                //--------------------------------------------------------
                u32 GetTypeHash() const;
                //--------------------------------------------------------
                /// Get Size
                ///
                /// @return Sizeof the property type
                //--------------------------------------------------------
                u32 GetSize() const;
                //--------------------------------------------------------
                /// Get Class
                ///
                /// @return The meta class for this instance
                //--------------------------------------------------------
                CMetaClass* GetClass() const;
                //--------------------------------------------------------
                /// Set Accessor (Setter)
                ///
                /// @param The setter delegate
                //--------------------------------------------------------
                template <typename PropType, typename ClassType> void SetAccessor(void (ClassType::* inpMethod)(PropType))
                {
                    typedef CPropertyConcrete<PropType, ClassType> RealProp;
                    static_cast<RealProp*>(this)->mpSetterMethodValue = inpMethod;
                }
                //--------------------------------------------------------
                /// Set Accessor (Setter)
                ///
                /// @param The setter delegate
                //--------------------------------------------------------
                template <typename PropType, typename ClassType> void SetAccessor(void (ClassType::* inpMethod)(const PropType&))
                {
                    typedef CPropertyConcrete<PropType, ClassType> RealProp;
                    static_cast<RealProp*>(this)->mpSetterMethodConstRef = inpMethod;
                }
                //--------------------------------------------------------
                /// Set Accessor (Getter)
                ///
                /// @param The getter delegate
                //--------------------------------------------------------
                template <typename PropType, typename ClassType> void SetAccessor(PropType (ClassType::* inpMethod)() const)
                {
                    typedef CPropertyConcrete<PropType, ClassType> RealProp;
                    static_cast<RealProp*>(this)->mpGetterMethodValueConst = inpMethod;
                }
                //--------------------------------------------------------
                /// Set Accessor (Getter)
                ///
                /// @param The getter delegate
                //--------------------------------------------------------
                template <typename PropType, typename ClassType> void SetAccessor(const PropType& (ClassType::* inpMethod)() const)
                {
                    typedef CPropertyConcrete<PropType, ClassType> RealProp;
                    static_cast<RealProp*>(this)->mpGetterMethodConstRefConst = inpMethod;
                }
                //--------------------------------------------------------
                /// Set Accessor (Getter)
                ///
                /// @param The getter delegate
                //--------------------------------------------------------
                template <typename PropType, typename ClassType> void SetAccessor(PropType (ClassType::* inpMethod)())
                {
                    typedef CPropertyConcrete<PropType, ClassType> RealProp;
                    static_cast<RealProp*>(this)->mpGetterMethodValue = inpMethod;
                }
                //--------------------------------------------------------
                /// Set Accessor (Getter)
                ///
                /// @param The getter delegate
                //--------------------------------------------------------
                template <typename PropType, typename ClassType> void SetAccessor(const PropType& (ClassType::* inpMethod)())
                {
                    typedef CPropertyConcrete<PropType, ClassType> RealProp;
                    static_cast<RealProp*>(this)->mpGetterMethodConstRef = inpMethod;
                }
                //--------------------------------------------------------
                /// Set 
                ///
                /// Used to restrict access to the property
                /// based on the access policy
                ///
                /// @param Class meta instance
                /// @param Value to assign
                //--------------------------------------------------------
                template <typename T> void Set(CInstance* inpInstance, const T& inValue)
                {
                    MOFLOW_ASSERT(meAccessType != PropAccess::k_readOnly, "This property is read only and cannot be assigned");

                    if(!CallSetterConstRef((void*)(&inValue), inpInstance))
                    {
                        if(!CallSetterValue((void*)(&inValue), inpInstance))
                        {
                            T* pMember = reinterpret_cast<T*>((static_cast<s8*>(inpInstance->GetInstance()) + mudwOffset));
                            (*pMember) = inValue;
                        }
                    }
                }
                //--------------------------------------------------------
                /// Get
                ///
                /// Used to restrict access to the property
                /// based on the access policy
                ///
                /// @param Class meta instance
                /// @param Out: Property value
                //--------------------------------------------------------
                template <typename T> void Get(CInstance* inpInstance, T& outValue) 
                {
                    MOFLOW_ASSERT(meAccessType != PropAccess::k_writeOnly, "This property is write only and cannot be read");

                    if(!CallGetterConstRefConst((void*)(&outValue), inpInstance))
                    {
                        if(!CallGetterConstRef((void*)(&outValue), inpInstance))
                        {
                            if(!CallGetterValueConst((void*)(&outValue), inpInstance))
                            {
                                if(!CallGetterValue((void*)(&outValue), inpInstance))
                                {
                                    T* pMember = reinterpret_cast<T*>((static_cast<s8*>(inpInstance->GetInstance()) + mudwOffset));
                                    outValue = (*pMember);
                                }
                            }
                        }
                    }
                }
                //--------------------------------------------------------
                /// Set (Static)
                ///
                /// Used to restrict access to the property
                /// based on the access policy
                ///
                /// @param Value to assign
                //--------------------------------------------------------
                template <typename T> void Set(const T& inValue)
                {
                    MOFLOW_ASSERT(meAccessType != PropAccess::k_readOnly, "This property is read only and cannot be assigned");

                    MOFLOW_ASSERT(mpStaticValue, "To access the property without an instance it must be static");
                    *(static_cast<T*>(mpStaticValue)) = inValue;
                }
                //--------------------------------------------------------
                /// Get (Static)
                ///
                /// Used to restrict access to the property
                /// based on the access policy
                ///
                /// @param Out: Property value
                //--------------------------------------------------------
                template <typename T> void Get(T& outValue) const
                {
                    MOFLOW_ASSERT(meAccessType != PropAccess::k_writeOnly, "This property is write only and cannot be read");

                    MOFLOW_ASSERT(mpStaticValue, "To access the property without an instance it must be static");
                    outValue = *(static_cast<T*>(mpStaticValue));
                }
                //--------------------------------------------------------
                /// Call Setter Value
                ///
                /// Call the concrete class's value setter
                ///
                /// @param Pointer to value
                /// @param Pointer to instance
                /// @return Whether the accessor was called
                //--------------------------------------------------------
                virtual bool CallSetterValue(void* inpValue, CInstance* inpInstance) = 0;
                //--------------------------------------------------------
                /// Call Setter ConstRef
                ///
                /// Call the concrete class's const reference setter
                ///
                /// @param Pointer to value
                /// @param Pointer to instance
                /// @return Whether the accessor was called
                //--------------------------------------------------------
                virtual bool CallSetterConstRef(void* inpValue, CInstance* inpInstance) = 0;
                //--------------------------------------------------------
                /// Call Getter Value
                ///
                /// Call the concrete class's value getter
                ///
                /// @param Out: Pointer to value
                /// @param Pointer to instance
                /// @return Whether the accessor was called
                //--------------------------------------------------------
                virtual bool CallGetterValue(void* inpValue, CInstance* inpInstance) = 0;
                //--------------------------------------------------------
                /// Call Getter Value Const
                ///
                /// Call the concrete class's const value getter
                ///
                /// @param Out: Pointer to value
                /// @param Pointer to instance
                /// @return Whether the accessor was called
                //--------------------------------------------------------
                virtual bool CallGetterValueConst(void* inpValue, CInstance* inpInstance) = 0;
                //--------------------------------------------------------
                /// Call Getter ConstRef
                ///
                /// Call the concrete class's const reference getter
                ///
                /// @param Out: Pointer to value
                /// @param Pointer to instance
                /// @return Whether the accessor was called
                //--------------------------------------------------------
                virtual bool CallGetterConstRef(void* inpValue, CInstance* inpInstance) = 0;
                //--------------------------------------------------------
                /// Call Getter ConstRef Const
                ///
                /// Call the concrete class's const const reference getter
                ///
                /// @param Out: Pointer to value
                /// @param Pointer to instance
                /// @return Whether the accessor was called
                //--------------------------------------------------------
                virtual bool CallGetterConstRefConst(void* inpValue, CInstance* inpInstance) = 0;
                //--------------------------------------------------------
                /// Is Type
                ///
                /// @return Whether the property is of the type
                //--------------------------------------------------------
                bool IsType(const std::string& instrType) const
                {
                    return mstrType == instrType;
                }
                //--------------------------------------------------------
                /// Is Static
                ///
                /// @return Whether the property is static or a member
                //--------------------------------------------------------
                bool IsStatic() const
                {
                    return (mpStaticValue != NULL);
                }

            protected:
                std::string mstrName;
                std::string mstrType;

                PropAccess meAccessType;

                u32 mudwSize;
                u32 mudwOffset;
                u32 mudwNameHash;
                u32 mudwTypeHash;

                CMetaClass* mpClass;

                void* mpStaticValue;
            };
            //===============================================================================
            /// Property (Generic)
            ///
            /// An exposed member variable acting as a property that can be edited by
            /// name. The access to this property can be restricted to read-only etc
            /// Holds meta information such as size etc.
            //===============================================================================
            template <typename PropType, typename ClassType> class CPropertyConcrete : public CProperty
            {
            public:
                typedef PropType Type;
                typedef ClassType Class;
                //--------------------------------------------------------
                /// Get Offset
                ///
                /// @param Pointer to member data
                /// @return Offset of that data within the class
                //--------------------------------------------------------
                inline static u32 GetOffset(Type Class::* inpProperty)
                {
                    //Dirty boy!
                    return ((u32)&(((Class*)NULL)->*inpProperty));
                }
                //--------------------------------------------------------
                /// Call Setter Value
                ///
                /// Call the concrete class's value setter
                ///
                /// @param Pointer to value
                /// @param Pointer to instance
                /// @return Whether the accessor was called
                //--------------------------------------------------------
                bool CallSetterValue(void* inpValue, CInstance* inpInstance)
                {
                    if(mpSetterMethodValue)
                    {
                        ClassType* pInstance = static_cast<ClassType*>(inpInstance->GetInstance());
                        Type* pValue = static_cast<Type*>(inpValue);
                        (pInstance->*mpSetterMethodValue)(*pValue);  
                        return true;
                    }

                    return false;
                }
                //--------------------------------------------------------
                /// Call Setter ConstRef
                ///
                /// Call the concrete class's const reference setter
                ///
                /// @param Pointer to value
                /// @param Pointer to instance
                /// @return Whether the accessor was called
                //--------------------------------------------------------
                bool CallSetterConstRef(void* inpValue, CInstance* inpInstance)
                {
                    if(mpSetterMethodConstRef)
                    {
                        ClassType* pInstance = static_cast<ClassType*>(inpInstance->GetInstance());
                        Type* pValue = static_cast<Type*>(inpValue);
                        (pInstance->*mpSetterMethodConstRef)(*pValue);  
                        return true;
                    }

                    return false;
                }
                //--------------------------------------------------------
                /// Call Getter Value
                ///
                /// Call the concrete class's value getter
                ///
                /// @param Out: Pointer to value
                /// @param Pointer to instance
                /// @return Whether the accessor was called
                //--------------------------------------------------------
                bool CallGetterValue(void* inpValue, CInstance* inpInstance)
                {
                    if(mpGetterMethodValue)
                    {
                        ClassType* pInstance = static_cast<ClassType*>(inpInstance->GetInstance());
                        Type* pValue = static_cast<Type*>(inpValue);
                        *pValue = (pInstance->*mpGetterMethodValue)();  
                        return true;
                    }

                    return false;
                }
                //--------------------------------------------------------
                /// Call Getter Value Const
                ///
                /// Call the concrete class's const value getter
                ///
                /// @param Out: Pointer to value
                /// @param Pointer to instance
                /// @return Whether the accessor was called
                //--------------------------------------------------------
                bool CallGetterValueConst(void* inpValue, CInstance* inpInstance)
                {
                    if(mpGetterMethodValueConst)
                    {
                        ClassType* pInstance = static_cast<ClassType*>(inpInstance->GetInstance());
                        Type* pValue = static_cast<Type*>(inpValue);
                        *pValue = (pInstance->*mpGetterMethodValueConst)();  
                        return true;
                    }

                    return false;
                }
                //--------------------------------------------------------
                /// Call Getter ConstRef
                ///
                /// Call the concrete class's const reference getter
                ///
                /// @param Out: Pointer to value
                /// @param Pointer to instance
                /// @return Whether the accessor was called
                //--------------------------------------------------------
                bool CallGetterConstRef(void* inpValue, CInstance* inpInstance)
                {
                    if(mpGetterMethodConstRef)
                    {
                        ClassType* pInstance = static_cast<ClassType*>(inpInstance->GetInstance());
                        Type* pValue = static_cast<Type*>(inpValue);
                        *pValue = (pInstance->*mpGetterMethodConstRef)();  
                        return true;
                    }

                    return false;
                }
                //--------------------------------------------------------
                /// Call Getter ConstRef Const
                ///
                /// Call the concrete class's const const reference getter
                ///
                /// @param Out: Pointer to value
                /// @param Pointer to instance
                /// @return Whether the accessor was called
                //--------------------------------------------------------
                bool CallGetterConstRefConst(void* inpValue, CInstance* inpInstance)
                {
                    if(mpGetterMethodConstRefConst)
                    {
                        ClassType* pInstance = static_cast<ClassType*>(inpInstance->GetInstance());
                        Type* pValue = static_cast<Type*>(inpValue);
                        *pValue = (pInstance->*mpGetterMethodConstRefConst)();  
                        return true;
                    }

                    return false;
                }
            private:

                //--------------------------------------------------------
                /// Constructor
                //--------------------------------------------------------
                CPropertyConcrete() 
                    : mpSetterMethodValue(NULL), mpSetterMethodConstRef(NULL), mpGetterMethodValueConst(NULL), 
                    mpGetterMethodConstRefConst(NULL), mpGetterMethodValue(NULL), mpGetterMethodConstRef(NULL)
                {}

            private:

                void (Class::* mpSetterMethodValue)(Type);
                void (Class::*mpSetterMethodConstRef)(const Type&);
                Type (Class::* mpGetterMethodValueConst)() const;
                const Type& (Class::* mpGetterMethodConstRefConst)() const;
                Type (Class::* mpGetterMethodValue)();
                const Type& (Class::* mpGetterMethodConstRef)();

                friend class CProperty;
            };
        }
    }
}

#endif
