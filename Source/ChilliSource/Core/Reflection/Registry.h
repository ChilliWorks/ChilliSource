//
//  Registry.h
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

#ifndef MO_FLOW_CORE_REFLECTION_REGISTRY_HPP_
#define MO_FLOW_CORE_REFLECTION_REGISTRY_HPP_

#include <ChilliSource/ChilliSource.h>

#include <vector>

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
                static const std::vector<CMetaClass*>& GetClasses();
                
            private:
                
                static std::vector<CMetaClass*> MetaClasses;
            };
        }
    }
}

#endif
