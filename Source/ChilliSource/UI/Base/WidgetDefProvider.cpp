//
//  WidgetDefProvider.cpp
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

#include <ChilliSource/UI/Base/WidgetDefProvider.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/Utils.h>
#include <ChilliSource/Core/Resource/ResourcePool.h>
#include <ChilliSource/Core/String/StringParser.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>
#include <ChilliSource/UI/Base/PropertyMap.h>
#include <ChilliSource/UI/Base/PropertyType.h>
#include <ChilliSource/UI/Base/Widget.h>
#include <ChilliSource/UI/Base/WidgetDef.h>
#include <ChilliSource/UI/Base/WidgetHierarchyDesc.h>
#include <ChilliSource/UI/Base/WidgetTemplate.h>
#include <ChilliSource/UI/Base/WidgetTemplateProvider.h>
#include <ChilliSource/UI/Drawable/IDrawable.h>
#include <ChilliSource/UI/Drawable/DrawableType.h>
#include <ChilliSource/UI/Layout/ILayout.h>
#include <ChilliSource/UI/Layout/LayoutType.h>

#include <json/json.h>

namespace ChilliSource
{
    namespace UI
    {
        namespace
        {
            const std::string k_extension("csuidef");
            
            //-------------------------------------------------------
            /// From the given JSON value parse the values of the property
            /// types into the given container
            ///
            /// @author S Downie
            ///
            /// @param Json defaults
            /// @param [Out] Default property values
            /// @param [Out] Custom property values
            //-------------------------------------------------------
            void ParseDefaultValues(const Json::Value& in_defaults, PropertyMap& out_defaultProperties, PropertyMap& out_customProperties);

            //-------------------------------------------------------
            /// From the given JSON value parse the hierarchy and
            /// create definitions for all child widgets
            ///
            /// @author S Downie
            ///
            /// @param Json hierarchy
            /// @param Json widgets
            /// @param [Out] Children descriptions
            //-------------------------------------------------------
            void ParseChildWidgets(const Json::Value& in_hierarchy, const Json::Value& in_widgets, std::vector<WidgetHierarchyDesc>& out_children)
            {
                for(u32 i=0; i<in_hierarchy.size(); ++i)
                {
                    const Json::Value& hierarchyItem = in_hierarchy[i];
                    std::string name = hierarchyItem["Name"].asString();
                    const Json::Value& widget = in_widgets[name];
                    
                    WidgetHierarchyDesc childDesc;
                    WidgetTemplateProvider::ParseTemplate(widget, childDesc);
                    childDesc.m_defaultProperties.SetProperty("Name", name);
                    
                    const Json::Value& children = hierarchyItem["Children"];
                    if(children.isNull() == false)
                    {
                        ParseChildWidgets(children, in_widgets, childDesc.m_children);
                    }
                    
                    out_children.push_back(childDesc);
                }
            }
            //-------------------------------------------------------
            /// From the given JSON value parse the custom property
            /// types, names and values into the given container
            ///
            /// @author S Downie
            ///
            /// @param Json properties
            /// @param [Out] Custom properties
            //-------------------------------------------------------
            void ParseCustomProperties(const Json::Value& in_properties, PropertyMap& out_customProperties)
            {
                std::vector<PropertyMap::PropertyDesc> descs;
                descs.reserve(in_properties.size());
                for(auto it = in_properties.begin(); it != in_properties.end(); ++it)
                {
                    CS_ASSERT((*it).isString() == true, "WidgetDefProvider: Properties values in file must be strings: " + std::string(it.memberName()));
                    descs.push_back({ParsePropertyType((*it).asString()), it.memberName()});
                }
                
                out_customProperties.AllocateKeys(descs);
            }
            //-------------------------------------------------------
            /// From the given JSON value parse the values of the drawable
            /// types into a property map
            ///
            /// @author S Downie
            ///
            /// @param Json drawable
            ///
            /// @return Property values
            //-------------------------------------------------------
            PropertyMap ParseDrawableDefaultValues(const Json::Value& in_drawable)
            {
                DrawableType type = ParseDrawableType(in_drawable["Type"].asString());
                auto supportedProperties = IDrawable::GetPropertyDescs(type);
                PropertyMap result(supportedProperties);
                
                for(const auto& propDesc : supportedProperties)
                {
                    if(in_drawable.isMember(propDesc.m_name) == true)
                    {
                        result.SetProperty(propDesc.m_type, propDesc.m_name, in_drawable[propDesc.m_name].asString());
                    }
                }
                
                return result;
            }
            //-------------------------------------------------------
            /// From the given JSON value parse the values of the layout
            /// types into a property map
            ///
            /// @author S Downie
            ///
            /// @param Json layout
            ///
            /// @return Property values
            //-------------------------------------------------------
            PropertyMap ParseLayoutDefaultValues(const Json::Value& in_layout)
            {
                LayoutType type = ParseLayoutType(in_layout["Type"].asString());
                auto supportedProperties = ILayout::GetPropertyDescs(type);
                PropertyMap result(supportedProperties);
                
                for(const auto& propDesc : supportedProperties)
                {
                    if(in_layout.isMember(propDesc.m_name) == true)
                    {
                        result.SetProperty(propDesc.m_type, propDesc.m_name, in_layout[propDesc.m_name].asString());
                    }
                }
                
                return result;
            }
            //-------------------------------------------------------
            //-------------------------------------------------------
            void ParseDefaultValues(const Json::Value& in_defaults, PropertyMap& out_defaultProperties, PropertyMap& out_customProperties)
            {
                out_defaultProperties.AllocateKeys(Widget::GetPropertyDescs());
        
                for(auto it = in_defaults.begin(); it != in_defaults.end(); ++it)
                {
                    if(out_defaultProperties.HasProperty(it.memberName()) == true)
                    {
                        if(strcmp(it.memberName(), "Drawable") == 0)
                        {
                            //Special case for drawable
                            CS_ASSERT((*it).isObject(), "Value can only be specified as object: " + std::string(it.memberName()));
                            out_defaultProperties.SetProperty(it.memberName(), ParseDrawableDefaultValues(*it));
                        }
                        else if(strcmp(it.memberName(), "Layout") == 0)
                        {
                            //Special case for drawable
                            CS_ASSERT((*it).isObject(), "Value can only be specified as object: " + std::string(it.memberName()));
                            out_defaultProperties.SetProperty(it.memberName(), ParseLayoutDefaultValues(*it));
                        }
                        else
                        {
                            CS_ASSERT((*it).isString(), "Value can only be specified as string: " + std::string(it.memberName()));
                            out_defaultProperties.SetProperty(out_defaultProperties.GetType(it.memberName()), it.memberName(), (*it).asString());
                        }
                    }
                    else if(out_customProperties.HasProperty(it.memberName()) == true)
                    {
                        CS_ASSERT((*it).isString(), "Value can only be specified as string: " + std::string(it.memberName()));
                        out_customProperties.SetProperty(out_customProperties.GetType(it.memberName()), it.memberName(), (*it).asString());
                    }
                    else
                    {
                        CS_LOG_FATAL("Property with name does not exist: " + std::string(it.memberName()));
                    }
                }
            }
            //-------------------------------------------------------
            /// Performs the heavy lifting for loading a UI
            /// widget description from file
            ///
            /// @author S Downie
            ///
            /// @param Storage location
            /// @param File path
            /// @param Async load delegate
            /// @param [Out] Resource
            //-------------------------------------------------------
            void LoadDesc(Core::StorageLocation in_storageLocation, const std::string& in_filepath, const Core::ResourceProvider::AsyncLoadDelegate& in_delegate, const Core::ResourceSPtr& out_resource)
            {
                Json::Value root;
                Core::Utils::ReadJson(in_storageLocation, in_filepath, &root);
                
                WidgetDef* widgetDef = (WidgetDef*)out_resource.get();
                
                WidgetHierarchyDesc hierarchyDesc;
            
                const Json::Value& hierarchy = root["Hierarchy"];
                const Json::Value& children = root["Children"];
                if(hierarchy.isNull() == false && hierarchy.isArray() == true && children.isNull() == false)
                {
                    ParseChildWidgets(hierarchy, children, hierarchyDesc.m_children);
                }
                
                const Json::Value& customProperties = root["Properties"];
                if(customProperties.isNull() == false)
                {
                    ParseCustomProperties(customProperties, hierarchyDesc.m_customProperties);
                }
                
                const Json::Value& defaults = root["Defaults"];
                if(defaults.isNull() == false)
                {
                    ParseDefaultValues(defaults, hierarchyDesc.m_defaultProperties, hierarchyDesc.m_customProperties);
                }
                
                widgetDef->Build(hierarchyDesc);
                
                out_resource->SetLoadState(CSCore::Resource::LoadState::k_loaded);
                if(in_delegate != nullptr)
                {
                    CSCore::Application::Get()->GetTaskScheduler()->ScheduleMainThreadTask(std::bind(in_delegate, out_resource));
                }
            }
        }
        
        CS_DEFINE_NAMEDTYPE(WidgetDefProvider);
        //-------------------------------------------------------
        //-------------------------------------------------------
        WidgetDefProviderUPtr WidgetDefProvider::Create()
        {
            return WidgetDefProviderUPtr(new WidgetDefProvider());
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        bool WidgetDefProvider::IsA(Core::InterfaceIDType in_interfaceId) const
        {
            return (in_interfaceId == Core::ResourceProvider::InterfaceID || in_interfaceId == WidgetDefProvider::InterfaceID);
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        Core::InterfaceIDType WidgetDefProvider::GetResourceType() const
        {
            return WidgetDef::InterfaceID;
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        bool WidgetDefProvider::CanCreateResourceWithFileExtension(const std::string& in_extension) const
        {
            return (in_extension == k_extension);
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
		void WidgetDefProvider::CreateResourceFromFile(Core::StorageLocation in_storageLocation, const std::string& in_filepath, const Core::IResourceOptionsBaseCSPtr& in_options, const Core::ResourceSPtr& out_resource)
        {
            LoadDesc(in_storageLocation, in_filepath, nullptr, out_resource);
        }
        //----------------------------------------------------
        //----------------------------------------------------
		void WidgetDefProvider::CreateResourceFromFileAsync(Core::StorageLocation in_storageLocation, const std::string& in_filepath, const Core::IResourceOptionsBaseCSPtr& in_options, const AsyncLoadDelegate& in_delegate, const Core::ResourceSPtr& out_resource)
        {
			auto task = std::bind(LoadDesc, in_storageLocation, in_filepath, in_delegate, out_resource);
            Core::Application::Get()->GetTaskScheduler()->ScheduleTask(task);
        }
    }
}
