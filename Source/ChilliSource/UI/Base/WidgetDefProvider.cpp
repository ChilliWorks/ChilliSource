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
#include <ChilliSource/Scripting/Lua/LuaSource.h>
#include <ChilliSource/UI/Base/PropertyMap.h>
#include <ChilliSource/UI/Base/PropertyType.h>
#include <ChilliSource/UI/Base/Widget.h>
#include <ChilliSource/UI/Base/WidgetDef.h>
#include <ChilliSource/UI/Base/WidgetParserUtils.h>
#include <ChilliSource/UI/Base/WidgetTemplate.h>
#include <ChilliSource/UI/Base/WidgetTemplateProvider.h>

#include <json/json.h>

namespace ChilliSource
{
    namespace UI
    {
        namespace
        {
            const std::string k_extension("csuidef");

            //-------------------------------------------------------
            /// From the given JSON value parse the hierarchy and
            /// create definitions for all child widgets. Some of the properties
            /// require conversion from relative to absolute paths
            /// hence the definition path info.
            ///
            /// @author S Downie
            ///
            /// @param Json hierarchy
            /// @param Json widgets
            /// @param Definition location
            /// @param Defintion path (no file name)
            ///
            /// @return The child widget descriptions.
            //-------------------------------------------------------
            std::vector<WidgetDesc> ParseChildWidgets(const Json::Value& in_hierarchy, const Json::Value& in_widgets, Core::StorageLocation in_definitionLocation, const std::string& in_definitionPath)
            {
                std::vector<WidgetDesc> output;
                
                for(u32 i=0; i<in_hierarchy.size(); ++i)
                {
                    const Json::Value& hierarchyItem = in_hierarchy[i];
                    std::string name = hierarchyItem["Name"].asString();
                    const Json::Value& widget = in_widgets[name];
                    const Json::Value& childrenHierarchy = hierarchyItem["Children"];
                    WidgetDesc childDesc = WidgetParserUtils::ParseWidget(widget, name, in_widgets, childrenHierarchy, in_definitionLocation, in_definitionPath);
                    output.push_back(childDesc);
                }
                
                return output;
            }
            
            WidgetDesc GetDescWithName(const std::vector<WidgetDesc>& in_widgets, const std::string& in_name)
            {
                for (const auto& desc : in_widgets)
                {
                    if (desc.GetProperties().GetPropertyOrDefault("Name", "") == in_name)
                    {
                        return desc;
                    }
                }
                
                CS_LOG_FATAL("Could not find widget desc with name: " + in_name);
                return WidgetDesc("", PropertyMap(), std::vector<WidgetDesc>());
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
            PropertyMap BuildPropertyMap(const Json::Value& in_properties, const std::vector<WidgetDesc>& in_children, const std::vector<WidgetDef::ChildPropertyLink>& in_childPropertyLinks)
            {
                //define the properties.
                std::vector<PropertyMap::PropertyDesc> descs = Widget::GetPropertyDescs();
                
                //add linked properties
                for (auto& link : in_childPropertyLinks)
                {
                    auto widgetDesc = GetDescWithName(in_children, link.m_childName);
                    PropertyMap::PropertyDesc desc;
                    desc.m_type = widgetDesc.GetProperties().GetType(link.m_propertyName);
                    desc.m_name = link.m_linkName;
                    descs.push_back(desc);
                }
                
                //add custom properties.
                //TODO: This should be removed once Lua functionality is a component.
                for(auto it = in_properties.begin(); it != in_properties.end(); ++it)
                {
                    CS_ASSERT((*it).isString() == true, "WidgetDefProvider: Properties values in file must be strings: " + std::string(it.memberName()));
					PropertyMap::PropertyDesc desc;
					desc.m_type = ParsePropertyType((*it).asString());
					desc.m_name = it.memberName();
					descs.push_back(desc);
                }
                
                //build the property map
                PropertyMap output(descs);
                
                //initialise the values in the custom properties
                //TODO: This should be removed once Lua functionality is a component.
                for(auto it = in_properties.begin(); it != in_properties.end(); ++it)
                {
                    PropertyType type = ParsePropertyType((*it).asString());
                    output.SetProperty(type, it.memberName(), GetDefaultPropertyTypeValue(type));
                }
                
                return output;
            }
            //-------------------------------------------------------
            /// From the given JSON value parse the values of the property
            /// types into the given container. Some of the properties
            /// require conversion from relative to absolute paths
            /// hence the definition path info.
            ///
            /// @author S Downie
            ///
            /// @param Json defaults
            /// @param Definition location
            /// @param Defintion path (no file name)
            /// @param [Out] Default property values
            /// @param [Out] Custom property values
            //-------------------------------------------------------
            void ParseDefaultValues(const Json::Value& in_defaults, Core::StorageLocation in_definitionLocation, const std::string& in_definitionPath, PropertyMap& out_properties)
            {
                for(auto it = in_defaults.begin(); it != in_defaults.end(); ++it)
                {
                    if(out_properties.HasKey(it.memberName()) == true)
                    {
                        if(strcmp(it.memberName(), "Drawable") == 0)
                        {
                            //Special case for drawable
                            CS_ASSERT((*it).isObject(), "Value can only be specified as object: " + std::string(it.memberName()));
                            out_properties.SetProperty(it.memberName(), WidgetParserUtils::ParseDrawableValues(*it, in_definitionLocation, in_definitionPath));
                        }
                        else if(strcmp(it.memberName(), "Layout") == 0)
                        {
                            //Special case for drawable
                            CS_ASSERT((*it).isObject(), "Value can only be specified as object: " + std::string(it.memberName()));
                            out_properties.SetProperty(it.memberName(), WidgetParserUtils::ParseLayoutValues(*it));
                        }
                        else
                        {
                            CS_ASSERT((*it).isString(), "Value can only be specified as string: " + std::string(it.memberName()));
                            out_properties.SetProperty(out_properties.GetType(it.memberName()), it.memberName(), (*it).asString());
                        }
                    }
                    else
                    {
                        CS_LOG_FATAL("Property with name does not exist: " + std::string(it.memberName()));
                    }
                }
            }
            //-------------------------------------------------------
            /// Parses and builds the links for a parent property
            /// that directly affects a child property
            ///
            /// @author S Downie
            ///
            /// @param JSON object containing all exposed properties
            ///
            /// @return The list of child links.
            //-------------------------------------------------------
            std::vector<WidgetDef::ChildPropertyLink> ParseLinkedChildProperties(const Json::Value& in_properties)
            {
                std::vector<WidgetDef::ChildPropertyLink> links;
                
                for(auto it = in_properties.begin(); it != in_properties.end(); ++it)
                {
                    WidgetDef::ChildPropertyLink link;
                    link.m_linkName = it.memberName();
                    link.m_childName = (*it)["Child"].asString();
                    link.m_propertyName = (*it)["Property"].asString();
                    links.push_back(link);
                }
                
                return links;
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
                
                CS_ASSERT(root.isMember("Type"), "Widget def must have Type");
                std::string typeName = root["Type"].asString();
                
                std::string definitionFileName;
                std::string pathToDefinition;
                Core::StringUtils::SplitFilename(in_filepath, definitionFileName, pathToDefinition);
            
                const Json::Value& hierarchy = root["Hierarchy"];
                const Json::Value& children = root["Children"];
                std::vector<WidgetDesc> childDescs;
                if(hierarchy.isNull() == false && hierarchy.isArray() == true && children.isNull() == false)
                {
                    childDescs = ParseChildWidgets(hierarchy, children, in_storageLocation, pathToDefinition);
                }
                
                std::vector<WidgetDef::ChildPropertyLink> childPropertyLinks;
                const Json::Value& childProperties = root["ChildProperties"];
                if(childProperties.isNull() == false)
                {
                    childPropertyLinks = ParseLinkedChildProperties(childProperties);
                }
                
                const Json::Value& customProperties = root["Properties"];
                PropertyMap defaultProperties = BuildPropertyMap(customProperties, childDescs, childPropertyLinks);
                
                const Json::Value& defaults = root["Defaults"];
                if(defaults.isNull() == false)
                {
                    ParseDefaultValues(defaults, in_storageLocation, pathToDefinition, defaultProperties);
                }
                
                const Json::Value& behaviour = root["Behaviour"];
                
                if(behaviour.isNull() == false)
                {
                    bool relativePath = behaviour.isMember("Location") == false;
                    Core::StorageLocation behaviourLocation = in_storageLocation;
                    std::string behaviourPath = behaviour["FilePath"].asString();
                    
                    if(relativePath == false)
                    {
                        behaviourLocation = Core::ParseStorageLocation(behaviour["Location"].asString());
                    }
                    else
                    {
                        behaviourPath = Core::StringUtils::ResolveParentedDirectories(pathToDefinition + behaviourPath);
                    }
                    
                    if(in_delegate == nullptr)
                    {
                        auto luaSource = Core::Application::Get()->GetResourcePool()->LoadResource<Scripting::LuaSource>(behaviourLocation, behaviourPath);
                        widgetDef->Build(typeName, defaultProperties, childDescs, childPropertyLinks, luaSource);
                        out_resource->SetLoadState(luaSource->GetLoadState());
                    }
                    else
                    {
                        Core::Application::Get()->GetResourcePool()->LoadResourceAsync<Scripting::LuaSource>(behaviourLocation, behaviourPath, [=](const Core::ResourceCSPtr& in_resource)
                        {
                            auto luaSource = std::static_pointer_cast<const Scripting::LuaSource>(in_resource);
                            widgetDef->Build(typeName, defaultProperties, childDescs, childPropertyLinks, luaSource);
                            out_resource->SetLoadState(luaSource->GetLoadState());
                            CSCore::Application::Get()->GetTaskScheduler()->ScheduleMainThreadTask(std::bind(in_delegate, out_resource));
                        });
                    }
                }
                else
                {
                    widgetDef->Build(typeName, defaultProperties, childDescs, childPropertyLinks, nullptr);
                    
                    out_resource->SetLoadState(CSCore::Resource::LoadState::k_loaded);
                    if(in_delegate != nullptr)
                    {
                        CSCore::Application::Get()->GetTaskScheduler()->ScheduleMainThreadTask(std::bind(in_delegate, out_resource));
                    }
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
