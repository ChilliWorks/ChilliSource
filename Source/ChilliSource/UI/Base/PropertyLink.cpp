//
//  PropertyLink.cpp
//  ChilliSource
//  Created by Ian Copland on 24/11/2014.
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

#include <ChilliSource/UI/Base/PropertyLink.h>

namespace ChilliSource
{
    //------------------------------------------------------------------
    //------------------------------------------------------------------
    PropertyLink::PropertyLink(const std::string& in_linkName, const std::string& in_linkedOwner, const std::string& in_linkedProperty)
        : m_linkName(in_linkName), m_linkedOwner(in_linkedOwner), m_linkedProperty(in_linkedProperty)
    {
    }
    //------------------------------------------------------------------
    //------------------------------------------------------------------
    const std::string& PropertyLink::GetLinkName() const
    {
        return m_linkName;
    }
    //------------------------------------------------------------------
    //------------------------------------------------------------------
    const std::string& PropertyLink::GetLinkedOwner() const
    {
        return m_linkedOwner;
    }
    //------------------------------------------------------------------
    //------------------------------------------------------------------
    const std::string& PropertyLink::GetLinkedProperty() const
    {
        return m_linkedProperty;
    }
}
