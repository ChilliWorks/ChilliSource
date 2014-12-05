//
//  ComponentDesc.cpp
//  Chilli Source
//  Created by Ian Copland on 19/11/2014.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2014 Tag Games Limited
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

#include <ChilliSource/UI/Base/ComponentDesc.h>

namespace ChilliSource
{
    namespace UI
    {
        //------------------------------------------------------------------
        //------------------------------------------------------------------
        ComponentDesc::ComponentDesc(const std::string& in_type, const std::string& in_name, const Core::PropertyMap& in_propertyMap)
            : m_type(in_type), m_name(in_name), m_propertyMap(in_propertyMap)
        {
        }
        //------------------------------------------------------------------
        //------------------------------------------------------------------
        const std::string& ComponentDesc::GetType() const
        {
            return m_type;
        }
        //------------------------------------------------------------------
        //------------------------------------------------------------------
        const std::string& ComponentDesc::GetName() const
        {
            return m_name;
        }
        //------------------------------------------------------------------
        //------------------------------------------------------------------
        const Core::PropertyMap& ComponentDesc::GetProperties() const
        {
            return m_propertyMap;
        }
    }
}
