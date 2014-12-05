//
//  WidgetParserUtils.cpp
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

#include <ChilliSource/UI/Base/WidgetParserUtils.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Container/Property/PropertyMap.h>
#include <ChilliSource/Core/Json/JsonUtils.h>
#include <ChilliSource/Core/Localisation/LocalisedText.h>
#include <ChilliSource/Core/Resource/ResourcePool.h>
#include <ChilliSource/Core/String/StringParser.h>
#include <ChilliSource/Rendering/Base/HorizontalTextJustification.h>
#include <ChilliSource/Rendering/Base/VerticalTextJustification.h>
#include <ChilliSource/Rendering/Font/Font.h>
#include <ChilliSource/Rendering/Texture/Texture.h>
#include <ChilliSource/Rendering/Texture/TextureAtlas.h>
#include <ChilliSource/UI/Base/Widget.h>
#include <ChilliSource/UI/Base/WidgetDef.h>
#include <ChilliSource/UI/Base/WidgetDesc.h>
#include <ChilliSource/UI/Base/WidgetFactory.h>
#include <ChilliSource/UI/Base/WidgetPropertyTypes.h>
#include <ChilliSource/UI/Base/WidgetTemplate.h>
#include <ChilliSource/UI/Drawable/DrawableDesc.h>
#include <ChilliSource/UI/Drawable/IDrawable.h>
#include <ChilliSource/UI/Layout/ILayout.h>
#include <ChilliSource/UI/Layout/LayoutDesc.h>
#include <ChilliSource/UI/Layout/LayoutType.h>

#include <json/json.h>

namespace ChilliSource
{
	namespace UI
	{
        namespace WidgetParserUtils
        {
            //-------------------------------------------------------
            /// Loads a storage location and file path from the given
            /// json. If a storage location is not specified this
            /// will use the resource is considered to be relative
            /// to the given path and location.
            ///
            /// @author Ian Copland
            ///
            /// @param The json object.
            /// @param The relative storage location.
            /// @param The relative directory path.
            ///
            /// @return a pair containing the storage location and
            /// file path of the resource.
            //-------------------------------------------------------
            std::pair<Core::StorageLocation, std::string> ParseResource(const Json::Value& in_jsonValue, Core::StorageLocation in_relStorageLocation, const std::string& in_relDirectoryPath)
            {
                CS_ASSERT(in_jsonValue.isObject(), "Resource json must be an object.");
                CS_ASSERT(in_jsonValue.isMember("Path") == true, "Resource json must contain a Path.");
                
                const Json::Value& pathJson = in_jsonValue.get("Path", Json::nullValue);
                CS_ASSERT(pathJson.isString() == true, "Path must be a string.");
                
                std::string outputPath = pathJson.asString();
                
                Core::StorageLocation outputLocation;
                if (in_jsonValue.isMember("Location") == true)
                {
                    const Json::Value& locationJson = in_jsonValue.get("Location", Json::nullValue);
                    CS_ASSERT(locationJson.isString() == true, "Location must be a string.");

                    outputLocation = Core::ParseStorageLocation(locationJson.asString());
                }
                else
                {
                    outputLocation = in_relStorageLocation;
                    outputPath = Core::StringUtils::StandardiseDirectoryPath(in_relDirectoryPath) + outputPath;
                }
                
                return std::make_pair(outputLocation, outputPath);
            }
            //-------------------------------------------------------
            //-------------------------------------------------------
            void SetProperty(const std::string& in_propertyName, const Json::Value& in_jsonValue, Core::StorageLocation in_relStorageLocation, const std::string& in_relDirectoryPath, Core::PropertyMap& out_propertyMap)
            {
                auto propertyType = out_propertyMap.GetType(in_propertyName);
                
                if (propertyType == &WidgetPropertyTypes::k_texture)
                {
                    auto resourcePair = ParseResource(in_jsonValue, in_relStorageLocation, in_relDirectoryPath);
                    auto texture = Core::Application::Get()->GetResourcePool()->LoadResource<Rendering::Texture>(resourcePair.first, resourcePair.second);
                    out_propertyMap.SetProperty(in_propertyName, texture);
                }
                else if (propertyType == &WidgetPropertyTypes::k_textureAtlas)
                {
                    auto resourcePair = ParseResource(in_jsonValue, in_relStorageLocation, in_relDirectoryPath);
                    auto textureAtlas = Core::Application::Get()->GetResourcePool()->LoadResource<Rendering::TextureAtlas>(resourcePair.first, resourcePair.second);
                    out_propertyMap.SetProperty(in_propertyName, textureAtlas);
                }
                else if (propertyType == &WidgetPropertyTypes::k_font)
                {
                    auto resourcePair = ParseResource(in_jsonValue, in_relStorageLocation, in_relDirectoryPath);
                    auto font = Core::Application::Get()->GetResourcePool()->LoadResource<Rendering::Font>(resourcePair.first, resourcePair.second);
                    out_propertyMap.SetProperty(in_propertyName, font);
                }
                else if (propertyType == &WidgetPropertyTypes::k_localisedText)
                {
                    auto resourcePair = ParseResource(in_jsonValue, in_relStorageLocation, in_relDirectoryPath);
                    auto localisedText = Core::Application::Get()->GetResourcePool()->LoadResource<Core::LocalisedText>(resourcePair.first, resourcePair.second);
                    out_propertyMap.SetProperty(in_propertyName, localisedText);
                }
                else if (propertyType == &WidgetPropertyTypes::k_drawableDesc)
                {
                    CS_ASSERT(in_jsonValue.isObject(), "Value can only be specified as an object: " + in_propertyName);
                    DrawableDesc drawbleDesc(in_jsonValue, in_relStorageLocation, in_relDirectoryPath);
                    out_propertyMap.SetProperty(in_propertyName, drawbleDesc);
                }
                else if (propertyType == &WidgetPropertyTypes::k_layoutDesc)
                {
                    CS_ASSERT(in_jsonValue.isObject(), "Value can only be specified as an object: " + in_propertyName);
                    LayoutDesc layoutDesc(in_jsonValue);
                    out_propertyMap.SetProperty(in_propertyName, layoutDesc);
                }
                else
                {
                    CS_ASSERT(in_jsonValue.isString(), "Value can only be specified as a string: " + in_propertyName);
                    out_propertyMap.ParseProperty(in_propertyName, in_jsonValue.asString());
                }
            }
            //-------------------------------------------------------
            //-------------------------------------------------------
            WidgetDesc ParseWidget(const Json::Value& in_template, const std::string& in_name, const Json::Value& in_children, const Json::Value& in_hierarchy, Core::StorageLocation in_templateLocation, const std::string& in_templatePath)
            {
                CS_ASSERT(in_template.isMember("Type") == true, "Widget template must have type");
                
                std::string outputType = in_template["Type"].asString();
                Core::PropertyMap outputProperties;
                std::vector<WidgetDesc> outputChildren;
                
                if(outputType == "Template")
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
                        path = Core::StringUtils::ResolveParentedDirectories(in_templatePath + path);
                    }
                    
                    //Template widgets need to be created as a hierarchy so that we can set properties such as layout
                    //on the widget without affecting the contents of the template and vice-versa.
                    outputType = "Widget";
                    outputProperties = Core::PropertyMap(Widget::GetPropertyDescs());

                    //TODO: this will not work with async loading.
                    WidgetTemplateCSPtr widgetTemplate = Core::Application::Get()->GetResourcePool()->LoadResource<WidgetTemplate>(location, path);
                    outputChildren.push_back(widgetTemplate->GetWidgetDesc());
                }
                else
                {
                    auto widgetFactory = Core::Application::Get()->GetWidgetFactory();
                    WidgetDefCSPtr widgetDef = widgetFactory->GetDefinition(outputType);
                    outputProperties = widgetDef->GetDefaultProperties();
                }
                
                outputProperties.SetProperty("Name", in_name);
                
                for(auto it = in_template.begin(); it != in_template.end(); ++it)
                {
                    std::string propertyName = it.memberName();
                    
                    if (propertyName == "TemplateLocation" || propertyName == "TemplatePath" || propertyName == "Children" || propertyName == "Hierarchy" || propertyName == "Type")
                    {
                        //Ignore these as they are handled elsewhere but we do not want them to be included
                        //in the properties list
                    }
                    else if (outputProperties.HasKey(propertyName) == true)
                    {
                        SetProperty(propertyName, (*it), in_templateLocation, in_templatePath, outputProperties);
                    }
                    else
                    {
                        CS_LOG_FATAL("Property with name does not exist: " + std::string(propertyName));
                    }
                }
                
                if(in_hierarchy.isNull() == false)
                {
                    if(in_hierarchy.isNull() == false && in_hierarchy.isArray() == true && in_children.isNull() == false)
                    {
                        for(u32 i=0; i<in_hierarchy.size(); ++i)
                        {
                            const Json::Value& hierarchyItem = in_hierarchy[i];
                            std::string name = hierarchyItem["Name"].asString();
                            
                            const Json::Value& hierarchyChildren = hierarchyItem["Children"];
                            const Json::Value& widget = in_children[name];
                            
                            WidgetDesc childDesc = ParseWidget(widget, name, in_children, hierarchyChildren, in_templateLocation, in_templatePath);
                            outputChildren.push_back(childDesc);
                        }
                    }
                }
                
                return WidgetDesc(outputType, outputProperties, outputChildren);
            }
        }
	}
}

