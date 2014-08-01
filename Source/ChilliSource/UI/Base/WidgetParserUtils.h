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
#include <ChilliSource/UI/Base/PropertyMap.h>

#include <json/forwards.h>

namespace ChilliSource
{
	namespace UI
	{
        namespace WidgetParserUtils
        {
            //-------------------------------------------------------
            /// From the given JSON value parse the values of the drawable
            /// types into a property map. Drawables can be specified
            /// with a relative path and this needs conversion to an
            /// absolute path
            ///
            /// @author S Downie
            ///
            /// @param Json drawable
            /// @param Absolute storage location
            /// @param Absolute file path (no file name)
            ///
            /// @return Property values
            //-------------------------------------------------------
            PropertyMap ParseDrawableValues(const Json::Value& in_drawable, Core::StorageLocation in_location, const std::string& in_absPath);
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
            PropertyMap ParseLayoutValues(const Json::Value& in_layout);
        }
	}
}

#endif
