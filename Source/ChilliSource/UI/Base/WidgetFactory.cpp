//
//  WidgetFactory.cpp
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


#include <ChilliSource/UI/Base/WidgetFactory.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Delegate/MakeDelegate.h>
#include <ChilliSource/Core/Resource/ResourcePool.h>
#include <ChilliSource/UI/Base/Widget.h>
#include <ChilliSource/UI/Base/WidgetDef.h>
#include <ChilliSource/UI/Base/WidgetTemplate.h>
#include <ChilliSource/UI/Drawable/DrawableType.h>
#include <ChilliSource/UI/Drawable/NinePatchDrawable.h>
#include <ChilliSource/UI/Drawable/TextureDrawable.h>
#include <ChilliSource/UI/Drawable/ThreePatchDrawable.h>
#include <ChilliSource/UI/Layout/GridLayout.h>
#include <ChilliSource/UI/Layout/HListLayout.h>
#include <ChilliSource/UI/Layout/LayoutType.h>
#include <ChilliSource/UI/Layout/VListLayout.h>

namespace ChilliSource
{
	namespace UI
	{
		CS_DEFINE_NAMEDTYPE(WidgetFactory);
        
        namespace
        {
            const std::string k_widgetKey = "Widget";
            const std::string k_highlightButtonKey = "HighlightButton";
            
            //---------------------------------------------------------------------------
            /// Create the layout class based on the given description
            ///
            /// @author S Downie
            ///
            /// @param Layout desc
            ///
            /// @return Layout or nullptr
            //---------------------------------------------------------------------------
            ILayoutUPtr CreateLayout(const PropertyMap& in_properties)
            {
                LayoutType type = ParseLayoutType(in_properties.GetProperty<std::string>("Type"));
                
                switch(type)
                {
                    case LayoutType::k_none:
                        return nullptr;
                    case LayoutType::k_grid:
                        return ILayoutUPtr(new GridLayout(in_properties));
                    case LayoutType::k_hList:
                        return ILayoutUPtr(new HListLayout(in_properties));
                    case LayoutType::k_vList:
                        return ILayoutUPtr(new VListLayout(in_properties));
                }
                
                return nullptr;
            }
            //---------------------------------------------------------------------------
            /// Create the drawable class based on the given description
            ///
            /// @author S Downie
            ///
            /// @param Drawable desc
            ///
            /// @return Drawable or nullptr
            //---------------------------------------------------------------------------
            IDrawableUPtr CreateDrawable(const PropertyMap& in_properties)
            {
                DrawableType type = ParseDrawableType(in_properties.GetProperty<std::string>("Type"));
                
                switch(type)
                {
                    case DrawableType::k_none:
                        return nullptr;
                    case DrawableType::k_texture:
                        return IDrawableUPtr(new TextureDrawable(in_properties));
                    case DrawableType::k_ninePatch:
                        return IDrawableUPtr(new NinePatchDrawable(in_properties));
                    case DrawableType::k_threePatch:
                        return IDrawableUPtr(new ThreePatchDrawable(in_properties));
                }
                
                return nullptr;
            }
        }
        
        //---------------------------------------------------------------------------
        //---------------------------------------------------------------------------
        WidgetFactoryUPtr WidgetFactory::Create()
        {
            return WidgetFactoryUPtr(new WidgetFactory());
        }
        //---------------------------------------------------------------------------
        //---------------------------------------------------------------------------
		bool WidgetFactory::IsA(Core::InterfaceIDType in_interfaceId) const
        {
			return in_interfaceId == WidgetFactory::InterfaceID;
		}
        //---------------------------------------------------------------------------
        //---------------------------------------------------------------------------
        void WidgetFactory::OnInit()
        {
            auto resPool = Core::Application::Get()->GetResourcePool();

            WidgetDefCSPtr widgetDef = resPool->LoadResource<WidgetDef>(Core::StorageLocation::k_chilliSource, "Widgets/Widget.csuidef");
            RegisterDefinition(widgetDef);
            
            WidgetDefCSPtr highlightButtonDef = resPool->LoadResource<WidgetDef>(Core::StorageLocation::k_chilliSource, "Widgets/HighlightButton.csuidef");
            RegisterDefinition(highlightButtonDef);
            
            WidgetDefCSPtr horizontalSliderDef = resPool->LoadResource<WidgetDef>(Core::StorageLocation::k_chilliSource, "Widgets/HorizontalSliderBar.csuidef");
            RegisterDefinition(horizontalSliderDef);
        }
        //---------------------------------------------------------------------------
        //---------------------------------------------------------------------------
        void WidgetFactory::RegisterDefinition(const WidgetDefCSPtr& in_def)
        {
            m_widgetDefNameMap.insert(std::make_pair(in_def->GetHierarchyDesc().m_type, in_def));
        }
        //---------------------------------------------------------------------------
        //---------------------------------------------------------------------------
        WidgetDefCSPtr WidgetFactory::GetDefinition(const std::string& in_nameKey) const
        {
            auto it = m_widgetDefNameMap.find(in_nameKey);
            CS_ASSERT(it != m_widgetDefNameMap.end(), "No such widget def with name: " + in_nameKey);
            return it->second;
        }
        //---------------------------------------------------------------------------
        //---------------------------------------------------------------------------
        WidgetUPtr WidgetFactory::Create(const WidgetDefCSPtr& in_def) const
        {
            return CreateRecursive(in_def->GetHierarchyDesc());
        }
        //---------------------------------------------------------------------------
        //---------------------------------------------------------------------------
        WidgetUPtr WidgetFactory::Create(const WidgetTemplateCSPtr& in_template) const
        {
            return CreateRecursive(in_template->GetHierarchyDesc());
        }
        //---------------------------------------------------------------------------
        //---------------------------------------------------------------------------
        WidgetUPtr WidgetFactory::CreateHighlightButton() const
        {
            return Create(m_widgetDefNameMap.find(k_highlightButtonKey)->second);
        }
        //---------------------------------------------------------------------------
        //---------------------------------------------------------------------------
        WidgetUPtr WidgetFactory::CreateWidget() const
        {
            return Create(m_widgetDefNameMap.find(k_widgetKey)->second);
        }
        //---------------------------------------------------------------------------
        //---------------------------------------------------------------------------
        WidgetUPtr WidgetFactory::CreateRecursive(const WidgetHierarchyDesc& in_hierarchyDesc) const
        {
            WidgetUPtr widget(new Widget(in_hierarchyDesc.m_defaultProperties, in_hierarchyDesc.m_customProperties));
            
            if(in_hierarchyDesc.m_defaultProperties.HasProperty("Layout") == true)
            {
                switch (in_hierarchyDesc.m_access)
                {
                    case WidgetHierarchyDesc::Access::k_internal:
                        widget->SetInternalLayout(CreateLayout(in_hierarchyDesc.m_defaultProperties.GetProperty<PropertyMap>("Layout")));
                        break;
                    case WidgetHierarchyDesc::Access::k_external:
                        widget->SetLayout(CreateLayout(in_hierarchyDesc.m_defaultProperties.GetProperty<PropertyMap>("Layout")));
                        break;
                }
            }
            if(in_hierarchyDesc.m_defaultProperties.HasProperty("Drawable") == true)
            {
                widget->SetDrawable(CreateDrawable(in_hierarchyDesc.m_defaultProperties.GetProperty<PropertyMap>("Drawable")));
            }
            
            for(const auto& childHierarchyDesc : in_hierarchyDesc.m_children)
            {
                WidgetSPtr childWidget = CreateRecursive(childHierarchyDesc);
                switch (childHierarchyDesc.m_access)
                {
                    case WidgetHierarchyDesc::Access::k_internal:
                        widget->AddInternalWidget(childWidget);
                        break;
                    case WidgetHierarchyDesc::Access::k_external:
                        widget->AddWidget(childWidget);
                        break;
                }
            }
            
            //Hook up any links to our childrens properties
            std::unordered_map<std::string, std::pair<void*, void*>> defaultPropertyLinks;
            std::unordered_map<std::string, std::pair<Widget*, std::string>> customPropertyLinks;
            
            for(const auto& link : in_hierarchyDesc.m_links)
            {
                Widget* childWidget = widget->GetInternalWidget(link.m_widgetName);
                if(childWidget == nullptr)
                {
                    childWidget = widget->GetWidget(link.m_widgetName);
                }
                CS_ASSERT(childWidget != nullptr, "Cannot link to missing widget: " + link.m_widgetName);
                
                std::string lowerCasePropName(link.m_propertyName);
                Core::StringUtils::ToLowerCase(lowerCasePropName);

                if(lowerCasePropName == "name")
                {
                    auto setter = new std::function<void(const std::string&)>(Core::MakeDelegate(childWidget, &Widget::SetName));
                    auto getter = new std::function<const std::string&()>(Core::MakeDelegate(childWidget, &Widget::GetName));
                    defaultPropertyLinks.emplace(link.m_linkName, std::make_pair((void*)setter, (void*)getter));
                }
                else if(lowerCasePropName == "relposition")
                {
                    auto setter = new std::function<void(const Core::Vector2&)>(Core::MakeDelegate(childWidget, &Widget::SetRelativePosition));
                    auto getter = new std::function<Core::Vector2()>(Core::MakeDelegate(childWidget, &Widget::GetRelativePosition));
                    defaultPropertyLinks.emplace(link.m_linkName, std::make_pair((void*)setter, (void*)getter));
                }
                else if(lowerCasePropName == "absposition")
                {
                    auto setter = new std::function<void(const Core::Vector2&)>(Core::MakeDelegate(childWidget, &Widget::SetAbsolutePosition));
                    auto getter = new std::function<Core::Vector2()>(Core::MakeDelegate(childWidget, &Widget::GetAbsolutePosition));
                    defaultPropertyLinks.emplace(link.m_linkName, std::make_pair((void*)setter, (void*)getter));
                }
                else if(lowerCasePropName == "relsize")
                {
                    auto setter = new std::function<void(const Core::Vector2&)>(Core::MakeDelegate(childWidget, &Widget::SetRelativeSize));
                    auto getter = new std::function<Core::Vector2()>(Core::MakeDelegate(childWidget, &Widget::GetRelativeSize));
                    defaultPropertyLinks.emplace(link.m_linkName, std::make_pair((void*)setter, (void*)getter));
                }
                else if(lowerCasePropName == "abssize")
                {
                    auto setter = new std::function<void(const Core::Vector2&)>(Core::MakeDelegate(childWidget, &Widget::SetAbsoluteSize));
                    auto getter = new std::function<Core::Vector2()>(Core::MakeDelegate(childWidget, &Widget::GetAbsoluteSize));
                    defaultPropertyLinks.emplace(link.m_linkName, std::make_pair((void*)setter, (void*)getter));
                }
                else if(lowerCasePropName == "preferredsize")
                {
                    auto setter = new std::function<void(const Core::Vector2&)>(Core::MakeDelegate(childWidget, &Widget::SetDefaultPreferredSize));
                    auto getter = new std::function<Core::Vector2()>(Core::MakeDelegate(childWidget, &Widget::GetPreferredSize));
                    defaultPropertyLinks.emplace(link.m_linkName, std::make_pair((void*)setter, (void*)getter));
                }
                else if(lowerCasePropName == "scale")
                {
                    auto setter = new std::function<void(const Core::Vector2&)>(Core::MakeDelegate(childWidget, &Widget::ScaleTo));
                    auto getter = new std::function<Core::Vector2()>(Core::MakeDelegate(childWidget, &Widget::GetLocalScale));
                    defaultPropertyLinks.emplace(link.m_linkName, std::make_pair((void*)setter, (void*)getter));
                }
                else if(lowerCasePropName == "colour")
                {
                    auto setter = new std::function<void(const Core::Colour&)>(Core::MakeDelegate(childWidget, &Widget::SetColour));
                    auto getter = new std::function<Core::Colour()>(Core::MakeDelegate(childWidget, &Widget::GetLocalColour));
                    defaultPropertyLinks.emplace(link.m_linkName, std::make_pair((void*)setter, (void*)getter));
                }
                else if(lowerCasePropName == "rotation")
                {
                    auto setter = new std::function<void(f32)>(Core::MakeDelegate(childWidget, &Widget::RotateTo));
                    auto getter = new std::function<f32()>(Core::MakeDelegate(childWidget, &Widget::GetLocalRotation));
                    defaultPropertyLinks.emplace(link.m_linkName, std::make_pair((void*)setter, (void*)getter));
                }
                else if(lowerCasePropName == "originanchor")
                {
                    auto setter = new std::function<void(Rendering::AlignmentAnchor)>(Core::MakeDelegate(childWidget, &Widget::SetOriginAnchor));
                    auto getter = new std::function<Rendering::AlignmentAnchor()>(Core::MakeDelegate(childWidget, &Widget::GetOriginAnchor));
                    defaultPropertyLinks.emplace(link.m_linkName, std::make_pair((void*)setter, (void*)getter));
                }
                else if(lowerCasePropName == "parentalanchor")
                {
                    auto setter = new std::function<void(Rendering::AlignmentAnchor)>(Core::MakeDelegate(childWidget, &Widget::SetParentalAnchor));
                    auto getter = new std::function<Rendering::AlignmentAnchor()>(Core::MakeDelegate(childWidget, &Widget::GetParentalAnchor));
                    defaultPropertyLinks.emplace(link.m_linkName, std::make_pair((void*)setter, (void*)getter));
                }
                else if(lowerCasePropName == "visible")
                {
                    auto setter = new std::function<void(bool)>(Core::MakeDelegate(childWidget, &Widget::SetVisible));
                    auto getter = new std::function<bool()>(Core::MakeDelegate(childWidget, &Widget::IsVisible));
                    defaultPropertyLinks.emplace(link.m_linkName, std::make_pair((void*)setter, (void*)getter));
                }
                else if(lowerCasePropName == "clipchildren")
                {
                    auto setter = new std::function<void(bool)>(Core::MakeDelegate(childWidget, &Widget::SetClippingEnabled));
                    auto getter = new std::function<bool()>(Core::MakeDelegate(childWidget, &Widget::IsClippingEnabled));
                    defaultPropertyLinks.emplace(link.m_linkName, std::make_pair((void*)setter, (void*)getter));
                }
                else if(lowerCasePropName == "sizepolicy")
                {
                    auto setter = new std::function<void(SizePolicy)>(Core::MakeDelegate(childWidget, &Widget::SetSizePolicy));
                    auto getter = new std::function<SizePolicy()>(Core::MakeDelegate(childWidget, &Widget::GetSizePolicy));
                    defaultPropertyLinks.emplace(link.m_linkName, std::make_pair((void*)setter, (void*)getter));
                }
                else
                {
                    customPropertyLinks.emplace(link.m_linkName, std::make_pair(childWidget, link.m_propertyName));
                }
            }
            
            widget->SetPropertyLinks(std::move(defaultPropertyLinks), std::move(customPropertyLinks));
            
            return widget;
        }
        //---------------------------------------------------------------------------
        //---------------------------------------------------------------------------
        void WidgetFactory::OnDestroy()
        {
            m_widgetDefNameMap.clear();
        }
	}
}
