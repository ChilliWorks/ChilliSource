//
//  UIComponentFactory.cpp
//  Chilli Source
//  Created by Ian Copland on 14/11/2014.
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

#include <ChilliSource/UI/Base/UIComponentFactory.h>

#include <ChilliSource/UI/Button/HighlightComponent.h>
#include <ChilliSource/UI/Button/ToggleHighlightComponent.h>
#include <ChilliSource/UI/Drawable/DrawableComponent.h>
#include <ChilliSource/UI/Layout/LayoutComponent.h>
#include <ChilliSource/UI/ProgressBar/ProgressBarComponent.h>
#include <ChilliSource/UI/Slider/SliderComponent.h>
#include <ChilliSource/UI/Text/TextComponent.h>

namespace ChilliSource
{
    CS_DEFINE_NAMEDTYPE(UIComponentFactory);
    //-----------------------------------------------------------------
    //-----------------------------------------------------------------
    UIComponentFactoryUPtr UIComponentFactory::Create()
    {
        return UIComponentFactoryUPtr(new UIComponentFactory());
    }
    //-----------------------------------------------------------------
    //-----------------------------------------------------------------
    bool UIComponentFactory::IsA(InterfaceIDType in_interfaceId) const
    {
        return (UIComponentFactory::InterfaceID == in_interfaceId);
    }
    //-----------------------------------------------------------------
    //-----------------------------------------------------------------
    std::vector<PropertyMap::PropertyDesc> UIComponentFactory::GetPropertyDescs(const std::string& in_componentTypeName) const
    {
        auto descsIt = m_descsMap.find(in_componentTypeName);
        CS_ASSERT(descsIt != m_descsMap.end(), "Could not get property descs for component with name: " + in_componentTypeName);
        
        return descsIt->second;
    }
    //-----------------------------------------------------------------
    //-----------------------------------------------------------------
    UIComponentUPtr UIComponentFactory::CreateComponent(const std::string& in_componentTypeName, const std::string& in_name, const PropertyMap& in_propertyMap) const
    {
        auto delegateIt = m_creatorDelegateMap.find(in_componentTypeName);
        CS_ASSERT(delegateIt != m_creatorDelegateMap.end(), "Could not create component with name: " + in_componentTypeName);
        
        return delegateIt->second(in_name, in_propertyMap);
    }
    //-----------------------------------------------------------------
    //-----------------------------------------------------------------
    void UIComponentFactory::OnInit()
    {
        Register<DrawableComponent>("UIDrawable");
        Register<LayoutComponent>("Layout");
        Register<HighlightComponent>("Highlight");
        Register<ToggleHighlightComponent>("ToggleHighlight");
        Register<TextComponent>("Text");
        Register<ProgressBarComponent>("ProgressBar");
        Register<SliderComponent>("Slider");
    }
    //-----------------------------------------------------------------
    //-----------------------------------------------------------------
    void UIComponentFactory::OnDestroy()
    {
        m_creatorDelegateMap.clear();
    }
}