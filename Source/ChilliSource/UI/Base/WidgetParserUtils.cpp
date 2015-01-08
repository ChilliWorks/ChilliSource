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
#include <ChilliSource/UI/Base/PropertyTypes.h>
#include <ChilliSource/UI/Base/Widget.h>
#include <ChilliSource/UI/Base/WidgetDef.h>
#include <ChilliSource/UI/Base/WidgetDesc.h>
#include <ChilliSource/UI/Base/WidgetFactory.h>
#include <ChilliSource/UI/Base/WidgetTemplate.h>
#include <ChilliSource/UI/Drawable/DrawableDef.h>
#include <ChilliSource/UI/Layout/LayoutDef.h>

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
                const char k_resourceFilePathKey[] = "Path";
                const char k_resourceLocationKey[] = "Location";
                
                CS_ASSERT(in_jsonValue.isObject(), "Resource json must be an object.");
                CS_ASSERT(in_jsonValue.isMember(k_resourceFilePathKey) == true, "Resource json must contain a '" + std::string(k_resourceFilePathKey) + "' key.");
                
                const Json::Value& pathJson = in_jsonValue.get(k_resourceFilePathKey, Json::nullValue);
                CS_ASSERT(pathJson.isString() == true, "'" + std::string(k_resourceFilePathKey) + "' must be a string.");
                
                std::string outputPath = pathJson.asString();
                
                Core::StorageLocation outputLocation;
                if (in_jsonValue.isMember(k_resourceLocationKey) == true)
                {
                    const Json::Value& locationJson = in_jsonValue.get(k_resourceLocationKey, Json::nullValue);
                    CS_ASSERT(locationJson.isString() == true, "'" + std::string(k_resourceLocationKey) + "' must be a string.");

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
                
                if (propertyType == PropertyTypes::Texture())
                {
                    auto resourcePair = ParseResource(in_jsonValue, in_relStorageLocation, in_relDirectoryPath);
                    auto texture = Core::Application::Get()->GetResourcePool()->LoadResource<Rendering::Texture>(resourcePair.first, resourcePair.second);
                    out_propertyMap.SetProperty(in_propertyName, texture);
                }
                else if (propertyType == PropertyTypes::TextureAtlas())
                {
                    auto resourcePair = ParseResource(in_jsonValue, in_relStorageLocation, in_relDirectoryPath);
                    auto textureAtlas = Core::Application::Get()->GetResourcePool()->LoadResource<Rendering::TextureAtlas>(resourcePair.first, resourcePair.second);
                    out_propertyMap.SetProperty(in_propertyName, textureAtlas);
                }
                else if (propertyType == PropertyTypes::Font())
                {
                    auto resourcePair = ParseResource(in_jsonValue, in_relStorageLocation, in_relDirectoryPath);
                    auto font = Core::Application::Get()->GetResourcePool()->LoadResource<Rendering::Font>(resourcePair.first, resourcePair.second);
                    out_propertyMap.SetProperty(in_propertyName, font);
                }
                else if (propertyType == PropertyTypes::LocalisedText())
                {
                    auto resourcePair = ParseResource(in_jsonValue, in_relStorageLocation, in_relDirectoryPath);
                    auto localisedText = Core::Application::Get()->GetResourcePool()->LoadResource<Core::LocalisedText>(resourcePair.first, resourcePair.second);
                    out_propertyMap.SetProperty(in_propertyName, localisedText);
                }
                else if (propertyType == PropertyTypes::DrawableDef())
                {
                    CS_ASSERT(in_jsonValue.isObject(), "Value can only be specified as an object: " + in_propertyName);
                    DrawableDefCSPtr drawableDef = DrawableDef::Create(in_jsonValue, in_relStorageLocation, in_relDirectoryPath);
                    out_propertyMap.SetProperty(in_propertyName, drawableDef);
                }
                else if (propertyType == PropertyTypes::LayoutDef())
                {
                    CS_ASSERT(in_jsonValue.isObject(), "Value can only be specified as an object: " + in_propertyName);
                    LayoutDefCSPtr layoutDef = LayoutDef::Create(in_jsonValue);
                    out_propertyMap.SetProperty(in_propertyName, layoutDef);
                }
                else
                {
                    CS_ASSERT(in_jsonValue.isString(), "Value can only be specified as a string: " + in_propertyName);
                    out_propertyMap.ParseProperty(in_propertyName, in_jsonValue.asString());
                }
            }
            //-------------------------------------------------------
            //-------------------------------------------------------
            WidgetDesc ParseWidget(const Json::Value& in_widget, Core::StorageLocation in_templateLocation, const std::string& in_templatePath)
            {
                const char k_widgetTypeKey[] = "Type";
                const char k_widgetChildrenKey[] = "Children";
                const char k_templateTypeName[] = "Template";
                const char k_templateFilePathKey[] = "TemplatePath";
                const char k_templateLocationKey[] = "TemplateLocation";
                
                CS_ASSERT(in_widget.isMember(k_widgetTypeKey) == true, "Widget must have '" + std::string(k_widgetTypeKey) + "' key.");
                
                std::string outputType = in_widget[k_widgetTypeKey].asString();
                Core::PropertyMap outputProperties;
                std::vector<WidgetDesc> outputChildren;
                
                if(outputType == k_templateTypeName)
                {
                    //This type is a special case in which the property values are read from a separate template file
                    CS_ASSERT(in_widget.isMember(k_templateFilePathKey), "Link to template file must have '" + std::string(k_templateFilePathKey) + "' key.");
                    
                    bool relativePath = in_widget.isMember(k_templateLocationKey) == false;
                    Core::StorageLocation location = in_templateLocation;
                    std::string path = in_widget[k_templateFilePathKey].asString();
                    
                    if(relativePath == false)
                    {
                        location = Core::ParseStorageLocation(in_widget[k_templateLocationKey].asString());
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
                
                for(auto it = in_widget.begin(); it != in_widget.end(); ++it)
                {
                    std::string propertyName = it.memberName();
                    
                    //ignore template properties, children and type since they're already handled.
                    if (propertyName != k_templateLocationKey && propertyName != k_templateFilePathKey && propertyName != k_widgetChildrenKey && propertyName != k_widgetTypeKey)
                    {
                        if (outputProperties.HasKey(propertyName) == true)
                        {
                            SetProperty(propertyName, (*it), in_templateLocation, in_templatePath, outputProperties);
                        }
                        else
                        {
                            CS_LOG_FATAL("Property with name does not exist: " + std::string(propertyName));
                        }
                    }
                }
                
                const Json::Value& childrenJson = in_widget.get(k_widgetChildrenKey, Json::nullValue);
                if(childrenJson.isNull() == false)
                {
                    CS_ASSERT(childrenJson.isArray() == true, "The '" + std::string(k_widgetChildrenKey) + "' must be in an array.");
                    
                    for(u32 i = 0; i < childrenJson.size(); ++i)
                    {
                        const Json::Value& childJson = childrenJson[i];
                        WidgetDesc childDesc = ParseWidget(childJson, in_templateLocation, in_templatePath);
                        outputChildren.push_back(childDesc);
                    }
                }
                
                return WidgetDesc(outputType, outputProperties, outputChildren);
            }
        }
	}
}

