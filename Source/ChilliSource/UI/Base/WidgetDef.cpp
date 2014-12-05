//
//  WidgetDef.cpp
//  Chilli Source
//  Created by Scott Downie on 25/07/2014.
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

#include <ChilliSource/UI/Base/WidgetDef.h>

namespace ChilliSource
{
    namespace UI
    {
        CS_DEFINE_NAMEDTYPE(WidgetDef);
        //-------------------------------------------------------
        //-------------------------------------------------------
        WidgetDefUPtr WidgetDef::Create()
        {
            return WidgetDefUPtr(new WidgetDef());
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        bool WidgetDef::IsA(Core::InterfaceIDType in_interfaceId) const
        {
            return in_interfaceId == WidgetDef::InterfaceID;
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        void WidgetDef::Build(const std::string& in_typeName, const Core::PropertyMap& in_defaultProperties, const std::vector<ComponentDesc>& in_componentDescs, const std::vector<PropertyLink>& in_componentPropertyLinks,
                              const std::vector<WidgetDesc>& in_childDescs, const std::vector<PropertyLink>& in_childPropertyLinks)
        {
            CS_ASSERT(GetLoadState() == LoadState::k_loading, "A WidgetDef can only be built while in the loading state.");
            
            m_typeName = in_typeName;
            m_defaultProperties = in_defaultProperties;
            m_childDescs = in_childDescs;
            m_childPropertyLinks = in_childPropertyLinks;
            m_componentDescs = in_componentDescs;
            m_componentPropertyLinks = in_componentPropertyLinks;
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        const std::string& WidgetDef::GetTypeName() const
        {
            return m_typeName;
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        const Core::PropertyMap& WidgetDef::GetDefaultProperties() const
        {
            return m_defaultProperties;
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        const std::vector<ComponentDesc>& WidgetDef::GetComponentDescs() const
        {
            return m_componentDescs;
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        const std::vector<PropertyLink>& WidgetDef::GetComponentPropertyLinks() const
        {
            return m_componentPropertyLinks;
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        const std::vector<WidgetDesc>& WidgetDef::GetChildDescs() const
        {
            return m_childDescs;
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        const std::vector<PropertyLink>& WidgetDef::GetChildPropertyLinks() const
        {
            return m_childPropertyLinks;
        }
    }
}
