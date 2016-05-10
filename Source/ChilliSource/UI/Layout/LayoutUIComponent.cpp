//
//  LayoutUIComponent.cpp
//  Chilli Source
//  Created by Ian Copland on 07/01/2015.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2015 Tag Games Limited
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

#include <ChilliSource/UI/Layout/LayoutUIComponent.h>

#include <ChilliSource/Core/Delegate/MakeDelegate.h>
#include <ChilliSource/UI/Base/PropertyTypes.h>
#include <ChilliSource/UI/Base/Widget.h>
#include <ChilliSource/UI/Layout/UILayout.h>
#include <ChilliSource/UI/Layout/UILayoutDef.h>

namespace ChilliSource
{
    namespace
    {
        const char k_layoutKey[] = "Layout";
        
        const std::vector<PropertyMap::PropertyDesc> k_propertyDescs =
        {
            {PropertyTypes::UILayoutDef(), k_layoutKey},
        };
    }
    
    CS_DEFINE_NAMEDTYPE(LayoutUIComponent);
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    const std::vector<PropertyMap::PropertyDesc>& LayoutUIComponent::GetPropertyDescs()
    {
        return k_propertyDescs;
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    LayoutUIComponent::LayoutUIComponent(const std::string& in_componentName, const PropertyMap& in_properties)
        : UIComponent(in_componentName)
    {
        RegisterProperty<UILayoutDefCSPtr>(PropertyTypes::UILayoutDef(), k_layoutKey, MakeDelegate(this, &LayoutUIComponent::GetLayoutDef), MakeDelegate(this, &LayoutUIComponent::ApplyLayoutDef));
        ApplyRegisteredProperties(in_properties);
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    bool LayoutUIComponent::IsA(InterfaceIDType in_interfaceId) const
    {
        return (LayoutUIComponent::InterfaceID == in_interfaceId);
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    UILayout* LayoutUIComponent::GetLayout()
    {
        return m_layout.get();
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    const UILayout* LayoutUIComponent::GetLayout() const
    {
        return m_layout.get();
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    void LayoutUIComponent::ApplyLayoutDef(const UILayoutDefCSPtr& in_layoutDef)
    {
        CS_ASSERT(in_layoutDef != nullptr, "Cannot set null layout def on a layout component.");
        
        m_layoutDef = in_layoutDef;
        m_layout = m_layoutDef->CreateLayout(this);
        
        OnLayoutChanged();
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    void LayoutUIComponent::BuildLayout()
    {
        m_layout->BuildLayout();
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    Vector2 LayoutUIComponent::GetSizeForIndex(u32 in_index) const
    {
        return m_layout->GetSizeForIndex(in_index);
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    Vector2 LayoutUIComponent::GetPositionForIndex(u32 in_index) const
    {
        return m_layout->GetPositionForIndex(in_index);
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    const UILayoutDefCSPtr& LayoutUIComponent::GetLayoutDef() const
    {
        return m_layoutDef;
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    void LayoutUIComponent::OnLayoutChanged()
    {
        GetWidget()->ForceLayout();
    }
}