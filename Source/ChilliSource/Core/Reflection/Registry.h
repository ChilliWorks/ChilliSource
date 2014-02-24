//
//  Registry.hpp
//  MoFlowSkeleton
//
//  Created by Scott Downie on 30/11/2011.
//  Copyright (c) 2011 Tag Games. All rights reserved.
//

#ifndef MO_FLOW_CORE_REFLECTION_REGISTRY_HPP_
#define MO_FLOW_CORE_REFLECTION_REGISTRY_HPP_

#include <ChilliSource/Core/Reflection/ForwardDeclarations.h>

namespace ChilliSource
{
    namespace Core
    {
        namespace Reflect
        {
            class CRegistry
            {
            public:
                ~CRegistry();
                //------------------------------------------------------------
                /// Add Class
                ///
                /// @param Meta-class
                //------------------------------------------------------------
                static void AddClass(CMetaClass* inpClass);
                //------------------------------------------------------------
                /// Remove Class
                ///
                /// @param Meta-class
                //------------------------------------------------------------
                static void RemoveClass(CMetaClass* inpClass);
                //------------------------------------------------------------
                /// Get Class
                ///
                /// Get the meta-class with the given name ID
                ///
                /// @param Class name
                /// @return Meta class
                //------------------------------------------------------------
                static CMetaClass* GetClass(const std::string& instrName);
                //------------------------------------------------------------
                /// Get Classes
                ///
                /// Get all the meta-classes
                ///
                /// @return Array of meta classes
                //------------------------------------------------------------
                static const DYNAMIC_ARRAY<CMetaClass*>& GetClasses();
                
            private:
                
                static DYNAMIC_ARRAY<CMetaClass*> MetaClasses;
            };
        }
    }
}

#endif
