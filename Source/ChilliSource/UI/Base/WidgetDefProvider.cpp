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
#include <ChilliSource/Core/Container/Property/PropertyMap.h>
#include <ChilliSource/Core/Json/JsonUtils.h>
#include <ChilliSource/Core/Resource/ResourcePool.h>
#include <ChilliSource/Core/String/StringParser.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>
#include <ChilliSource/UI/Base/ComponentFactory.h>
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
            
            const char k_widgetNameKey[] = "Name";
            
            const char k_componentTypeKey[] = "Type";
            const char k_componentNameKey[] = "Name";

            //-------------------------------------------------------
            /// Searches the given widget desc list for a widget
            /// description with the given name. This is recursive and
            /// will navigate down the widget desc tree to find the
            /// widget. Recursion is breadth first. If one cannot
            /// be found the app is considered to be in an
            /// irrecoverable state and will terminate.
            ///
            /// @author Ian Copland
            ///
            /// @param The list of widget descriptions.
            /// @param The name to look for.
            /// @param [Out] The widget description with the given
            /// name.
            //-------------------------------------------------------
            bool GetWidgetDescWithName(const std::vector<WidgetDesc>& in_widgets, const std::string& in_name, WidgetDesc& out_widgetDesc)
            {
                for (const auto& desc : in_widgets)
                {
                    if (desc.GetProperties().GetPropertyOrDefault(k_widgetNameKey, "") == in_name)
                    {
                        out_widgetDesc = desc;
                        return true;
                    }
                }
                
                for (const auto& desc : in_widgets)
                {
                    if (GetWidgetDescWithName(desc.GetChildDescs(), in_name, out_widgetDesc) == true)
                    {
                        return true;
                    }
                }
                
                return false;
            }
            //-------------------------------------------------------
            /// Searches the given component desc list for a component
            /// description with the given name. If one cannot be
            /// found the app is considered to be in an irrecoverable
            /// state and will terminate.
            ///
            /// @author Ian Copland
            ///
            /// @param The list of component descriptions.
            /// @param The name to look for.
            ///
            /// @return The widget description with the given name.
            //-------------------------------------------------------
            ComponentDesc GetComponentDescWithName(const std::vector<ComponentDesc>& in_componentDescs, const std::string& in_name)
            {
                for (const auto& desc : in_componentDescs)
                {
                    if (desc.GetName() == in_name)
                    {
                        return desc;
                    }
                }
                
                CS_LOG_FATAL("Could not find component description with name: " + in_name);
                return ComponentDesc();
            }
            //-------------------------------------------------------
            /// Parses the given json to create a single component
            /// object.
            ///
            /// A component may contain file path properties which may
            /// be relative to the widget def if a storage location
            /// is not supplied.
            ///
            /// @author Ian Copland
            ///
            /// @param The component json object.
            /// @param The storage location of the definition.
            /// @param The directory path of the definition.
            ///
            /// @return The output component description.
            //-------------------------------------------------------
            ComponentDesc ParseComponent(const Json::Value& in_componentJson, Core::StorageLocation in_definitionLocation, const std::string& in_definitionPath, ComponentFactory* in_componentFactory)
            {
                CS_ASSERT(in_componentJson.isNull() == false, "Cannot parse null component json.");
                CS_ASSERT(in_componentJson.isObject() == true, "Component json must be an object.");
                CS_ASSERT(in_componentJson.isMember(k_componentTypeKey) == true, "Component json must contain a '" + std::string(k_componentTypeKey) + "' key.");
                CS_ASSERT(in_componentJson.isMember(k_componentNameKey) == true, "Component json must contain a '" + std::string(k_componentNameKey) + "' key.");
                
                Json::Value typeJson = in_componentJson.get(k_componentTypeKey, Json::nullValue);
                Json::Value nameJson = in_componentJson.get(k_componentNameKey, Json::nullValue);
                CS_ASSERT(typeJson.isString() == true, "The '" + std::string(k_componentTypeKey) + "' in component json must be a string.");
                CS_ASSERT(nameJson.isString() == true, "The '" + std::string(k_componentNameKey) + "' in component json must be a string.");
                
                std::string type = typeJson.asString();
                std::string name = nameJson.asString();
                Core::PropertyMap propertyMap(in_componentFactory->GetPropertyDescs(type));
                
                for(auto it = in_componentJson.begin(); it != in_componentJson.end(); ++it)
                {
                    std::string propertyName = it.memberName();
                    
                    //ignore the type and name key since they're already handled
                    if (propertyName != k_componentTypeKey && propertyName != k_componentNameKey)
                    {
                        if (propertyMap.HasKey(propertyName) == true)
                        {
                            WidgetParserUtils::SetProperty(propertyName, (*it), in_definitionLocation, in_definitionPath, propertyMap);
                        }
                        else
                        {
                            CS_LOG_FATAL("Property '" + propertyName + "' in component '" + name + "' of type '" + type + "' does not exist.");
                        }
                    }
                    
                }
                
                return ComponentDesc(type, name, propertyMap);
            }
            //-------------------------------------------------------
            /// Parses the given json to create a list of component
            /// descriptions.
            ///
            /// Components may contain file path properties which may
            /// be relative to the widget def if a storage location
            /// is not supplied.
            ///
            /// @author Ian Copland
            ///
            /// @param The components json object.
            /// @param The storage location of the definition.
            /// @param The directory path of the definition.
            ///
            /// @return The output component descriptions.
            //-------------------------------------------------------
            std::vector<ComponentDesc> ParseComponents(const Json::Value& in_componentsJson, Core::StorageLocation in_definitionLocation, const std::string& in_definitionPath, ComponentFactory* in_componentFactory)
            {
                CS_ASSERT(in_componentsJson.isNull() == false, "Cannot parse null components json.");
                CS_ASSERT(in_componentsJson.isArray() == true, "Components json must be an array.");
                
                std::vector<ComponentDesc> output;
                for(auto& componentJson : in_componentsJson)
                {
                    output.push_back(ParseComponent(componentJson, in_definitionLocation, in_definitionPath, in_componentFactory));
                }
                
                return output;
            }
            //-------------------------------------------------------
            /// From the given JSON value parse the hierarchy and
            /// create definitions for all child widgets. Some of the
            /// properties require conversion from relative to absolute
            /// paths hence the definition path info.
            ///
            /// @author S Downie
            ///
            /// @param Json children.
            /// @param Definition location
            /// @param Defintion path (no file name)
            ///
            /// @return The child widget descriptions.
            //-------------------------------------------------------
            std::vector<WidgetDesc> ParseChildWidgets(const Json::Value& in_children, Core::StorageLocation in_definitionLocation, const std::string& in_definitionPath)
            {
                std::vector<WidgetDesc> output;
                
                for(u32 i = 0; i < in_children.size(); ++i)
                {
                    const Json::Value& childJson = in_children[i];
                    WidgetDesc childDesc = WidgetParserUtils::ParseWidget(childJson, in_definitionLocation, in_definitionPath);
                    output.push_back(childDesc);
                }
                
                return output;
            }
            //-------------------------------------------------------
            /// Builds a property map containing all of the properties
            /// that the intended widget type can contain. This includes
            /// all base widget properties plus the linked component
            /// and child properties.
            ///
            /// @author Ian Copland
            ///
            /// @param The component descriptions.
            /// @param The component property links.
            /// @param The child descriptions.
            /// @param The child property links.
            ///
            /// @return The property map.
            //-------------------------------------------------------
            Core::PropertyMap BuildPropertyMap(const std::vector<ComponentDesc>& in_componentDescs, const std::vector<PropertyLink>& in_componentPropertyLinks,
                                         const std::vector<WidgetDesc>& in_childDescs, const std::vector<PropertyLink>& in_childPropertyLinks)
            {
                //define the properties.
                std::vector<Core::PropertyMap::PropertyDesc> descs = Widget::GetPropertyDescs();
                
                //add linked component properties
                for (auto& link : in_componentPropertyLinks)
                {
                    auto componentDesc = GetComponentDescWithName(in_componentDescs, link.GetLinkedOwner());
                    Core::PropertyMap::PropertyDesc desc;
                    desc.m_type = componentDesc.GetProperties().GetType(link.GetLinkedProperty());
                    desc.m_name = link.GetLinkName();
                    descs.push_back(desc);
                }
                
                //add linked properties
                for (auto& link : in_childPropertyLinks)
                {
                    WidgetDesc widgetDesc;
                    if (GetWidgetDescWithName(in_childDescs, link.GetLinkedOwner(), widgetDesc) == false)
                    {
                        CS_LOG_FATAL("Could not find widget desc with name: " + link.GetLinkedOwner());
                    }
                    Core::PropertyMap::PropertyDesc desc;
                    desc.m_type = widgetDesc.GetProperties().GetType(link.GetLinkedProperty());
                    desc.m_name = link.GetLinkName();
                    descs.push_back(desc);
                }
                
                //build the property map
                Core::PropertyMap output(descs);
                
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
            void ParseDefaultValues(const Json::Value& in_defaults, Core::StorageLocation in_definitionLocation, const std::string& in_definitionPath, Core::PropertyMap& out_properties)
            {
                for(auto it = in_defaults.begin(); it != in_defaults.end(); ++it)
                {
                    if(out_properties.HasKey(it.memberName()) == true)
                    {
                        WidgetParserUtils::SetProperty(it.memberName(), (*it), in_definitionLocation, in_definitionPath, out_properties);
                    }
                    else
                    {
                        CS_LOG_FATAL("Property with name does not exist: " + std::string(it.memberName()));
                    }
                }
            }
            //-------------------------------------------------------
            /// Parses and builds the linked properties between the
            /// widget and components.
            ///
            /// @author Ian Copland
            ///
            /// @param The component properties json.
            ///
            /// @return The list of component property links.
            //-------------------------------------------------------
            std::vector<PropertyLink> ParseLinkedComponentProperties(const Json::Value& in_properties, const std::vector<ComponentDesc>& in_componentDescs)
            {
                const char k_componentLinkTypeAllKey[] = "all";
                const char k_componentLinkTypeNoneKey[] = "none";
                
                std::vector<PropertyLink> links;
                
                for (auto linkedComponentIt = in_properties.begin(); linkedComponentIt != in_properties.end(); ++linkedComponentIt)
                {
                    std::string linkedComponentName = linkedComponentIt.memberName();
                    Json::Value linkedComponentJson = (*linkedComponentIt);
                    CS_ASSERT((linkedComponentJson.isString() == true || linkedComponentJson.isObject() == true), "A component link must be an object or a string (containing either 'all' or 'none').");
                    
                    if (linkedComponentJson.isString() == true)
                    {
                        
                        std::string lowerValue = linkedComponentJson.asString();
                        Core::StringUtils::ToLowerCase(lowerValue);
                        
                        if (lowerValue == k_componentLinkTypeAllKey)
                        {
                            ComponentDesc componentDesc = GetComponentDescWithName(in_componentDescs, linkedComponentName);
                            for (const auto& propertyName : componentDesc.GetProperties().GetKeys())
                            {
                                links.push_back(PropertyLink(propertyName, linkedComponentName, propertyName));
                            }
                        }
                        else if (lowerValue == k_componentLinkTypeNoneKey)
                        {
                            //If none is selected we don't want any links, so do nothing.
                        }
                        else
                        {
                            CS_LOG_FATAL("Invalid property link value for component: " + linkedComponentName);
                        }
                    }
                    else if (linkedComponentJson.isObject() == true)
                    {
                        for (auto linkedPropertyIt = linkedComponentJson.begin(); linkedPropertyIt != linkedComponentJson.end(); ++linkedPropertyIt)
                        {
                            std::string linkName = linkedPropertyIt.memberName();
                            Json::Value linkedPropertyJson = (*linkedPropertyIt);
                            CS_ASSERT(linkedPropertyJson.isString(), "A linked property name must be a string.");
                            
                            std::string linkedPropertyName = linkedPropertyJson.asString();
                            links.push_back(PropertyLink(linkName, linkedComponentName, linkedPropertyName));
                        }
                    }
                }
                
                return links;
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
            std::vector<PropertyLink> ParseLinkedChildProperties(const Json::Value& in_properties)
            {
                std::vector<PropertyLink> links;
                
                for (auto linkedWidgetIt = in_properties.begin(); linkedWidgetIt != in_properties.end(); ++linkedWidgetIt)
                {
                    std::string linkedWidgetName = linkedWidgetIt.memberName();
                    Json::Value linkedWidgetJson = (*linkedWidgetIt);
                    CS_ASSERT(linkedWidgetJson.isObject() == true, "A child link must be an object.");
                    
                    for (auto linkedPropertyIt = linkedWidgetJson.begin(); linkedPropertyIt != linkedWidgetJson.end(); ++linkedPropertyIt)
                    {
                        std::string linkName = linkedPropertyIt.memberName();
                        Json::Value linkedPropertyJson = (*linkedPropertyIt);
                        CS_ASSERT(linkedPropertyJson.isString(), "A linked property name must be a string.");
                        
                        std::string linkedPropertyName = linkedPropertyJson.asString();
                        links.push_back(PropertyLink(linkName, linkedWidgetName, linkedPropertyName));
                    }
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
                const char k_widgetTypeKey[] = "Type";
                const char k_widgetComponentsKey[] = "Components";
                const char k_widgetComponentPropertiesKey[] = "ComponentPropertyLinks";
                const char k_widgetChildrenKey[] = "Children";
                const char k_widgetChildPropertiesKey[] = "ChildPropertyLinks";
                const char k_widgetPropertyDefaultsKey[] = "DefaultPropertyValues";
                
                ComponentFactory* componentFactory = Core::Application::Get()->GetSystem<ComponentFactory>();
                
                //read the json
                Json::Value root;
                if (Core::JsonUtils::ReadJson(in_storageLocation, in_filepath, root) == false)
                {
                    CS_LOG_ERROR("Cannot read widget def file: " + in_filepath);
                    out_resource->SetLoadState(Core::Resource::LoadState::k_failed);
                    if(in_delegate != nullptr)
                    {
                        Core::Application::Get()->GetTaskScheduler()->ScheduleMainThreadTask(std::bind(in_delegate, out_resource));
                    }
                    return;
                }
                
                //get the type
                WidgetDef* widgetDef = (WidgetDef*)out_resource.get();
                
                CS_ASSERT(root.isMember(k_widgetTypeKey), "Widget def must have Type");
                std::string typeName = root[k_widgetTypeKey].asString();
                
                //parse components
                std::string definitionFileName;
                std::string pathToDefinition;
                Core::StringUtils::SplitFilename(in_filepath, definitionFileName, pathToDefinition);
            
                const Json::Value& componentsJson = root[k_widgetComponentsKey];
                std::vector<ComponentDesc> componentDescs;
                if(componentsJson.isNull() == false)
                {
                    componentDescs = ParseComponents(componentsJson, in_storageLocation, pathToDefinition, componentFactory);
                }
                
                //parse component property links
                std::vector<PropertyLink> componentPropertyLinks;
                const Json::Value& componentPropertiesJson = root[k_widgetComponentPropertiesKey];
                if(componentPropertiesJson.isNull() == false)
                {
                    componentPropertyLinks = ParseLinkedComponentProperties(componentPropertiesJson, componentDescs);
                }
                
                //parse children
                const Json::Value& childrenJson = root[k_widgetChildrenKey];
                std::vector<WidgetDesc> childDescs;
                if(childrenJson.isNull() == false)
                {
                    CS_ASSERT(childrenJson.isArray() == true, "'" + std::string(k_widgetChildrenKey) + "' in a Widget Def must be an array.");
                    childDescs = ParseChildWidgets(childrenJson, in_storageLocation, pathToDefinition);
                }
                
                //parse child property links
                std::vector<PropertyLink> childPropertyLinks;
                const Json::Value& childProperties = root[k_widgetChildPropertiesKey];
                if(childProperties.isNull() == false)
                {
                    childPropertyLinks = ParseLinkedChildProperties(childProperties);
                }
                
                //build the default values property map and read the default values from the json
                Core::PropertyMap defaultProperties = BuildPropertyMap(componentDescs, componentPropertyLinks, childDescs, childPropertyLinks);
                const Json::Value& defaults = root[k_widgetPropertyDefaultsKey];
                if(defaults.isNull() == false)
                {
                    ParseDefaultValues(defaults, in_storageLocation, pathToDefinition, defaultProperties);
                }

                //build the widget def.
                widgetDef->Build(typeName, defaultProperties, componentDescs, componentPropertyLinks, childDescs, childPropertyLinks);
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
            //TODO: Async support.
            CS_LOG_FATAL("Asynchronous loading of Widget Def is currently not supported. Feature coming soon!");
        }
    }
}
