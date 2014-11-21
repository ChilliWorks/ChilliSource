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
#include <ChilliSource/Core/Resource/ResourcePool.h>
#include <ChilliSource/Core/String/StringParser.h>
#include <ChilliSource/UI/Base/Widget.h>
#include <ChilliSource/UI/Base/WidgetDesc.h>
#include <ChilliSource/UI/Base/WidgetFactory.h>
#include <ChilliSource/UI/Base/WidgetTemplate.h>
#include <ChilliSource/UI/Drawable/DrawableType.h>
#include <ChilliSource/UI/Drawable/IDrawable.h>
#include <ChilliSource/UI/Layout/ILayout.h>
#include <ChilliSource/UI/Layout/LayoutType.h>
#include <ChilliSource/UI/Text/TextDrawable.h>

#include <json/json.h>

namespace ChilliSource
{
	namespace UI
	{
        namespace WidgetParserUtils
        {
            namespace
            {
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
                    else if(in_propName == "TextDrawable")
                    {
                        //Special case for drawable
                        CS_ASSERT(in_json.isObject(), "Value can only be specified as object: " + in_propName);
                        in_propMap.SetProperty(in_propName, WidgetParserUtils::ParseTextDrawableValues(in_json));
                    }
                    else
                    {
                        CS_ASSERT(in_json.isString(), "Value can only be specified as string: " + in_propName);
                        in_propMap.SetProperty(in_propMap.GetType(in_propName), in_propName, in_json.asString());
                    }
                }
            }
            
            //-------------------------------------------------------
            //-------------------------------------------------------
            WidgetDesc ParseWidget(const Json::Value& in_template, const std::string& in_name, const Json::Value& in_children, const Json::Value& in_hierarchy, Core::StorageLocation in_templateLocation, const std::string& in_templatePath)
            {
                CS_ASSERT(in_template.isMember("Type") == true, "Widget template must have type");
                
                std::string outputType = in_template["Type"].asString();
                PropertyMap outputProperties;
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
                    outputProperties = PropertyMap(Widget::GetPropertyDescs());

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
                    
                    if (propertyName == "TemplateLocation" || propertyName == "TemplatePath" || propertyName == "Children" || propertyName == "Hierarchy")
                    {
                        //Ignore these as they are handled elsewhere but we do not want them to be included
                        //in the properties list
                    }
                    else if (outputProperties.HasKey(it.memberName()) == true)
                    {
                        SetProperty(propertyName, *it, in_templateLocation, in_templatePath, outputProperties);
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

