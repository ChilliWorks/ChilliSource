//
//  Registry.hpp
//  MoFlowSkeleton
//
//  Created by Robert Henning on 30/11/2011.
//  Copyright (c) 2011 Tag Games. All rights reserved.
//

#include <ChilliSource/Core/Reflection/Registry.h>
#include <ChilliSource/Core/Reflection/MetaClass.h>

#include <ChilliSource/Core/Cryptographic/HashCRC32.h>

namespace ChilliSource
{
    namespace Core
    {
        namespace Reflect
        {
            std::vector<CMetaClass*> CRegistry::MetaClasses;

            //------------------------------------------------------------
            /// Add Class
            ///
            /// @param Meta-class
            //------------------------------------------------------------
            void CRegistry::AddClass(CMetaClass* inpClass)
            {
                MetaClasses.push_back(inpClass);
            }
            //------------------------------------------------------------
            /// Remove Class
            ///
            /// @param Meta-class
            //------------------------------------------------------------
            void CRegistry::RemoveClass(CMetaClass* inpClass)
            {
                std::vector<CMetaClass*>::iterator it = std::find(MetaClasses.begin(), MetaClasses.end(), inpClass);
                if(it != MetaClasses.end())
                {
                    MetaClasses.erase(it);
                }
            }
            //------------------------------------------------------------
            /// Get Class
            ///
            /// Get the meta-class with the given name ID
            ///
            /// @param Class name
            /// @return Meta class
            //------------------------------------------------------------
            CMetaClass* CRegistry::GetClass(const std::string& instrName)
            {
                u32 udwNameHash = HashCRC32::GenerateHashCode(instrName);
                for(std::vector<CMetaClass*>::iterator it = MetaClasses.begin(); it != MetaClasses.end(); ++it)
                {
                    if((*it)->GetNameHash() == udwNameHash)
                    {
                        return (*it);
                    }
                }

                return nullptr;
            }
            //------------------------------------------------------------
            /// Get Classes
            ///
            /// Get all the meta-classes
            ///
            /// @return Array of meta classes
            //------------------------------------------------------------
            const std::vector<CMetaClass*>& CRegistry::GetClasses()
            {
                return MetaClasses;
            }
            //-------------------------------------------------------------
            /// Destructor
            //-------------------------------------------------------------
            CRegistry::~CRegistry()
            {
                for(std::vector<CMetaClass*>::iterator it = MetaClasses.begin(); it != MetaClasses.end(); ++it)
                {
                    delete (*it);
                }
            }
        }
    }
}

