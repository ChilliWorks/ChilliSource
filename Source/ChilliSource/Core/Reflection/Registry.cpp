//
//  Registry.cpp
//  Chilli Source
//  Created by Robert Henning on 30/11/2011.
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

#include <ChilliSource/Core/Cryptographic/HashCRC32.h>
#include <ChilliSource/Core/Reflection/Registry.h>
#include <ChilliSource/Core/Reflection/MetaClass.h>

#include <algorithm>

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

