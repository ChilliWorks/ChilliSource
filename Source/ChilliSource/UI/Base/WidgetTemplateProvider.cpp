//
//  WidgetTemplateProvider.cpp
//  Chilli Source
//  Created by Scott Downie on 30/07/2014.
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

#include <ChilliSource/UI/Base/WidgetTemplateProvider.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/Utils.h>
#include <ChilliSource/Core/Resource/ResourcePool.h>
#include <ChilliSource/Core/String/StringParser.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>
#include <ChilliSource/UI/Base/PropertyMap.h>
#include <ChilliSource/UI/Base/PropertyType.h>
#include <ChilliSource/UI/Base/Widget.h>
#include <ChilliSource/UI/Base/WidgetFactory.h>
#include <ChilliSource/UI/Base/WidgetHierarchyDesc.h>
#include <ChilliSource/UI/Base/WidgetTemplate.h>
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
            const std::string k_extension("csui");
            
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
            PropertyMap ParseDrawableValues(const Json::Value& in_drawable)
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
            PropertyMap ParseLayoutValues(const Json::Value& in_layout)
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
                
                WidgetTemplate* widgetTemplate = (WidgetTemplate*)out_resource.get();
                
                WidgetHierarchyDesc hierarchyDesc;
                WidgetTemplateProvider::ParseTemplate(root, hierarchyDesc);
                
                widgetTemplate->Build(hierarchyDesc);
                
                out_resource->SetLoadState(CSCore::Resource::LoadState::k_loaded);
                if(in_delegate != nullptr)
                {
                    CSCore::Application::Get()->GetTaskScheduler()->ScheduleMainThreadTask(std::bind(in_delegate, out_resource));
                }
            }
        }
        
        CS_DEFINE_NAMEDTYPE(WidgetTemplateProvider);
        //-------------------------------------------------------
        //-------------------------------------------------------
        WidgetTemplateProviderUPtr WidgetTemplateProvider::Create()
        {
            return WidgetTemplateProviderUPtr(new WidgetTemplateProvider());
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        bool WidgetTemplateProvider::IsA(Core::InterfaceIDType in_interfaceId) const
        {
            return (in_interfaceId == Core::ResourceProvider::InterfaceID || in_interfaceId == WidgetTemplateProvider::InterfaceID);
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        Core::InterfaceIDType WidgetTemplateProvider::GetResourceType() const
        {
            return WidgetTemplate::InterfaceID;
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        bool WidgetTemplateProvider::CanCreateResourceWithFileExtension(const std::string& in_extension) const
        {
            return (in_extension == k_extension);
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
		void WidgetTemplateProvider::CreateResourceFromFile(Core::StorageLocation in_storageLocation, const std::string& in_filepath, const Core::IResourceOptionsBaseCSPtr& in_options, const Core::ResourceSPtr& out_resource)
        {
            LoadDesc(in_storageLocation, in_filepath, nullptr, out_resource);
        }
        //----------------------------------------------------
        //----------------------------------------------------
		void WidgetTemplateProvider::CreateResourceFromFileAsync(Core::StorageLocation in_storageLocation, const std::string& in_filepath, const Core::IResourceOptionsBaseCSPtr& in_options, const AsyncLoadDelegate& in_delegate, const Core::ResourceSPtr& out_resource)
        {
			auto task = std::bind(LoadDesc, in_storageLocation, in_filepath, in_delegate, out_resource);
            Core::Application::Get()->GetTaskScheduler()->ScheduleTask(task);
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        void WidgetTemplateProvider::ParseTemplate(const Json::Value& in_template, WidgetHierarchyDesc& out_hierarchyDesc)
        {
            CS_ASSERT(in_template.isMember("Type") == true, "Widget template must have type");
            std::string type = in_template["Type"].asString();
            
            if(type == "Template")
            {
                //This type is a special case in which the property values are read from a separate template file
                CS_ASSERT(in_template.isMember("TemplateLocation"), "Link to template file must have TemplateLocation");
                CS_ASSERT(in_template.isMember("TemplatePath"), "Link to template file must have TemplatePath");
                
                Core::StorageLocation location = Core::ParseStorageLocation(in_template["TemplateLocation"].asString());
                
                auto resPool = Core::Application::Get()->GetResourcePool();
                WidgetTemplateCSPtr widgetTemplate = resPool->LoadResource<WidgetTemplate>(location, in_template["TemplatePath"].asString());
                out_hierarchyDesc = widgetTemplate->GetHierarchyDesc();
            }
            else
            {
                auto widgetFactory = Core::Application::Get()->GetWidgetFactory();
                WidgetDefCSPtr widgetDef = widgetFactory->GetDefinition(type);
                out_hierarchyDesc = widgetDef->GetHierarchyDesc();
            }
            
            
            for(auto it = in_template.begin(); it != in_template.end(); ++it)
            {
                if(out_hierarchyDesc.m_defaultProperties.HasProperty(it.memberName()) == true)
                {
                    if(strcmp(it.memberName(), "Drawable") == 0)
                    {
                        //Special case for drawable
                        CS_ASSERT((*it).isObject(), "Value can only be specified as object: " + std::string(it.memberName()));
                        out_hierarchyDesc.m_defaultProperties.SetProperty(it.memberName(), ParseDrawableValues(*it));
                    }
                    else if(strcmp(it.memberName(), "Layout") == 0)
                    {
                        //Special case for drawable
                        CS_ASSERT((*it).isObject(), "Value can only be specified as object: " + std::string(it.memberName()));
                        out_hierarchyDesc.m_defaultProperties.SetProperty(it.memberName(), ParseLayoutValues(*it));
                    }
                    else
                    {
                        CS_ASSERT((*it).isString(), "Value can only be specified as string: " + std::string(it.memberName()));
                        out_hierarchyDesc.m_defaultProperties.SetProperty(out_hierarchyDesc.m_defaultProperties.GetType(it.memberName()), it.memberName(), (*it).asString());
                    }
                }
                else if(out_hierarchyDesc.m_customProperties.HasProperty(it.memberName()) == true)
                {
                    CS_ASSERT((*it).isString(), "Value can only be specified as string: " + std::string(it.memberName()));
                    out_hierarchyDesc.m_customProperties.SetProperty(out_hierarchyDesc.m_customProperties.GetType(it.memberName()), it.memberName(), (*it).asString());
                }
                else if(strcmp(it.memberName(), "TemplateLocation") == 0 || strcmp(it.memberName(), "TemplatePath") == 0 || strcmp(it.memberName(), "Children") == 0 || strcmp(it.memberName(), "Hierarchy") == 0)
                {
                    //Ignore these as they are handled elsewhere but we do not want them to be included
                    //in the custom properties list
                }
                else
                {
                    CS_LOG_FATAL("Property with name does not exist: " + std::string(it.memberName()));
                }
            }
            
            const Json::Value& hierarchy = in_template["Hierarchy"];
            const Json::Value& children = in_template["Children"];
            if(hierarchy.isNull() == false && hierarchy.isArray() == true && children.isNull() == false)
            {
                for(u32 i=0; i<hierarchy.size(); ++i)
                {
                    const Json::Value& hierarchyItem = hierarchy[i];
                    std::string name = hierarchyItem["Name"].asString();
                    const Json::Value& widget = children[name];
                    
                    WidgetHierarchyDesc childDesc;
                    ParseTemplate(widget, childDesc);
                    out_hierarchyDesc.m_children.push_back(childDesc);
                }
            }
        }
    }
}
