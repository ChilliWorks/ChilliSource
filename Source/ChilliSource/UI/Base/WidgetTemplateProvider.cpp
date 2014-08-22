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
#include <ChilliSource/UI/Base/WidgetParserUtils.h>
#include <ChilliSource/UI/Base/WidgetTemplate.h>

#include <json/json.h>

namespace ChilliSource
{
    namespace UI
    {
        namespace
        {
            const std::string k_extension("csui");
            
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
                
                std::string definitionFileName;
                std::string pathToDefinition;
                Core::StringUtils::SplitFilename(in_filepath, definitionFileName, pathToDefinition);
                
                WidgetTemplateProvider::ParseTemplate(root, in_storageLocation, pathToDefinition, hierarchyDesc);
                
                widgetTemplate->Build(hierarchyDesc);
                
                out_resource->SetLoadState(CSCore::Resource::LoadState::k_loaded);
                if(in_delegate != nullptr)
                {
                    CSCore::Application::Get()->GetTaskScheduler()->ScheduleMainThreadTask(std::bind(in_delegate, out_resource));
                }
            }
            //-------------------------------------------------------
            /// Convenience method for setting a property in a prop
            /// map from JSON. This will handle the special cases
            /// of object types layout and drawable
            ///
            /// @author S Downie
            ///
            /// @param Property name
            /// @param JSON to parse and assign into map
            /// @param Location of file for relative pathing
            /// @param Base path of file for relative pathing
            /// @param [Out] Property map to assign to
            //-------------------------------------------------------
            void SetProperty(const std::string& in_propName, const Json::Value& in_json, Core::StorageLocation in_templateLocation, const std::string& in_templatePath, PropertyMap& in_propMap)
            {
                if(in_propName == "Drawable")
                {
                    //Special case for drawable
                    CS_ASSERT(in_json.isObject(), "Value can only be specified as object: " + in_propName);
                    in_propMap.SetProperty(in_propName, WidgetParserUtils::ParseDrawableValues(in_json, in_templateLocation, in_templatePath));
                }
                else if(in_propName == "Layout")
                {
                    //Special case for drawable
                    CS_ASSERT(in_json.isObject(), "Value can only be specified as object: " + in_propName);
                    in_propMap.SetProperty(in_propName, WidgetParserUtils::ParseLayoutValues(in_json));
                }
                else
                {
                    CS_ASSERT(in_json.isString(), "Value can only be specified as string: " + in_propName);
                    in_propMap.SetProperty(in_propMap.GetType(in_propName), in_propName, in_json.asString());
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
        void WidgetTemplateProvider::ParseTemplate(const Json::Value& in_template, Core::StorageLocation in_templateLocation, const std::string& in_templatePath, WidgetHierarchyDesc& out_hierarchyDesc)
        {
            CS_ASSERT(in_template.isMember("Type") == true, "Widget template must have type");
            std::string type = in_template["Type"].asString();
            
            if(type == "Template")
            {
                //This type is a special case in which the property values are read from a separate template file
                CS_ASSERT(in_template.isMember("TemplatePath"), "Link to template file must have TemplatePath");
                
                bool relativePath = in_template.isMember("TemplateLocation") == false;
                Core::StorageLocation location = in_templateLocation;
                std::string path = in_template["TemplatePath"].asString();
                
                if(relativePath == false)
                {
                    location = Core::ParseStorageLocation(in_template["TemplateLocation"].asString());
                }
                else
                {
                    path = in_templatePath + path;
                }
                
                auto resPool = Core::Application::Get()->GetResourcePool();
                WidgetTemplateCSPtr widgetTemplate = resPool->LoadResource<WidgetTemplate>(location, path);
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
                std::string propertyName = it.memberName();
                
                if(propertyName == "Type")
                {
                    CS_ASSERT((*it).isString(), "Value can only be specified as string: " + std::string(it.memberName()));
                    if(type != "Template")
                    {
                        out_hierarchyDesc.m_type = (*it).asString();
                    }
                }
                else if(out_hierarchyDesc.m_defaultProperties.HasProperty(it.memberName()) == true)
                {
                    SetProperty(propertyName, *it, in_templateLocation, in_templatePath, out_hierarchyDesc.m_defaultProperties);
                }
                else if(out_hierarchyDesc.m_customProperties.HasProperty(it.memberName()) == true)
                {
                    SetProperty(propertyName, *it, in_templateLocation, in_templatePath, out_hierarchyDesc.m_customProperties);
                }
                else if(propertyName == "TemplateLocation" || propertyName == "TemplatePath" || propertyName == "Children" || propertyName == "Hierarchy")
                {
                    //Ignore these as they are handled elsewhere but we do not want them to be included
                    //in the custom properties list
                }
                else
                {
                    auto itLink = std::find_if(out_hierarchyDesc.m_links.begin(), out_hierarchyDesc.m_links.end(), [propertyName](const WidgetHierarchyDesc::WidgetPropertyLink& in_link)
                    {
                        return in_link.m_linkName == propertyName;
                    });
                    
                    if(itLink != out_hierarchyDesc.m_links.end())
                    {
                        for(auto& child : out_hierarchyDesc.m_children)
                        {
                            if(child.m_defaultProperties.GetProperty<std::string>("Name") == itLink->m_widgetName)
                            {
                                if(child.m_defaultProperties.HasProperty(itLink->m_propertyName) == true)
                                {
                                    SetProperty(itLink->m_propertyName, *it, in_templateLocation, in_templatePath, child.m_defaultProperties);
                                    break;
                                }
                                if(child.m_customProperties.HasProperty(itLink->m_propertyName) == true)
                                {
                                    SetProperty(itLink->m_propertyName, *it, in_templateLocation, in_templatePath, child.m_customProperties);
                                    break;
                                }
                            }
                        }
                    }
                    else
                    {
                        CS_LOG_FATAL("Property with name does not exist: " + std::string(propertyName));
                    }
                }
            }
            
            out_hierarchyDesc.m_access = WidgetHierarchyDesc::Access::k_external;
            
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
                    ParseTemplate(widget, in_templateLocation, in_templatePath, childDesc);
                    out_hierarchyDesc.m_children.push_back(childDesc);
                }
            }
        }
    }
}
