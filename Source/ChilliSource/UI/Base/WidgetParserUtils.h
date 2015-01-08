//
//  WidgetParserUtils.h
//  Chilli Source
//  Created by Scott Downie on 29/07/2014.
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

#ifndef _CHILLISOURCE_UI_BASE_WIDGETPARSERUTILS_H_
#define _CHILLISOURCE_UI_BASE_WIDGETPARSERUTILS_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Container/Property/PropertyMap.h>
#include <ChilliSource/Core/File/StorageLocation.h>

#include <json/forwards.h>

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
            /// @param [Optional] The relative storage location.
            /// @param [Optional] The relative directory path.
            ///
            /// @return a pair containing the storage location and
            /// file path of the resource.
            //-------------------------------------------------------
            std::pair<Core::StorageLocation, std::string> ParseResource(const Json::Value& in_jsonValue, Core::StorageLocation in_relStorageLocation = Core::StorageLocation::k_package, const std::string& in_relDirectoryPath = "");
            //-------------------------------------------------------
            /// Sets a property in a property map from a json value.
            /// The value type must be a string or a json object or
            /// this will assert.
            ///
            /// @author Ian Copland
            ///
            /// @param The property name.
            /// @param The json value.
            /// @param [Out] The output property map.
            //-------------------------------------------------------
            void SetProperty(const std::string& in_propertyName, const Json::Value& in_jsonValue, Core::StorageLocation in_relStorageLocation, const std::string& in_relDirectoryPath, Core::PropertyMap& out_propertyMap);
            //-------------------------------------------------------
            /// From the given JSON value parse the values of the property
            /// types into the given container. Some of the properties
            /// require conversion from relative to absolute paths
            /// hence the definition path info.
            ///
            /// @author S Downie
            ///
            /// @param The widget json.
            /// @param Template location.
            /// @param Template path (no file name)
            ///
            /// @return The output widget description.
            //-------------------------------------------------------
            WidgetDesc ParseWidget(const Json::Value& in_widget, Core::StorageLocation in_templateLocation, const std::string& in_templatePath);
        }
	}
}

#endif
