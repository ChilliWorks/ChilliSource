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

#include <ChilliSource/UI/Drawable/DrawableType.h>
#include <ChilliSource/UI/Drawable/IDrawable.h>
#include <ChilliSource/UI/Layout/ILayout.h>
#include <ChilliSource/UI/Layout/LayoutType.h>

#include <json/json.h>

namespace ChilliSource
{
	namespace UI
	{
        namespace WidgetParserUtils
        {
            //-------------------------------------------------------
            //-------------------------------------------------------
            PropertyMap ParseDrawableValues(const Json::Value& in_drawable, Core::StorageLocation in_location, const std::string& in_absPath)
            {
                DrawableType type = ParseDrawableType(in_drawable["Type"].asString());
                auto supportedProperties = IDrawable::GetPropertyDescs(type);
                PropertyMap result(supportedProperties);
                
                if(type != DrawableType::k_none)
                {
                    bool relativePath = in_drawable.isMember("TextureLocation") == false;
                    
                    if(relativePath == true)
                    {
                        result.SetProperty(PropertyType::k_string, "TextureLocation", Core::ToString(in_location));
                    }
                    
                    for(const auto& propDesc : supportedProperties)
                    {
                        if(in_drawable.isMember(propDesc.m_name) == true)
                        {
                            std::string value = in_drawable[propDesc.m_name].asString();
                            
                            if(propDesc.m_name == "TexturePath" && relativePath == true)
                            {
                                value = in_absPath + value;
                            }
                            
                            result.SetProperty(propDesc.m_type, propDesc.m_name, value);
                        }
                    }
                }
                
                return result;
            }
            //-------------------------------------------------------
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
        }
	}
}

