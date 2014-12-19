//
//  WidgetDesc.h
//  Chilli Source
//  Created by Ian Copland on 19/11/2014.
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

#ifndef _CHILLISOURCE_UI_BASE_WIDGETDESC_H_
#define _CHILLISOURCE_UI_BASE_WIDGETDESC_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Container/Property/PropertyMap.h>

#include <vector>

namespace ChilliSource
{
    namespace UI
    {
        //--------------------------------------------------------------------------
        /// A container for a description of a single widget, comprising of a type
        /// and a map of properties for the given widget type and a list of child
        /// widgets. Typically this is only used in the creation of a WidgetDef and
        /// in most cases this does not need to be dealt with directly as the
        /// resource provider handles it when loading a widget from file. This is
        /// immutable after construction.
        ///
        /// @param Ian Copland
        //--------------------------------------------------------------------------
        class WidgetDesc final
        {
        public:
            //------------------------------------------------------------------
            /// Constructor. Creates an empty description.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------
            WidgetDesc() = default;
            //------------------------------------------------------------------
            /// Constructor.
            ///
            /// @author Ian Copland
            ///
            /// @param The name of the widget type this desc will create.
            /// @param The map of properties the widget should be created with.
            /// @param The list of child widget descriptions.
            //------------------------------------------------------------------
            WidgetDesc(const std::string& in_type, const Core::PropertyMap& in_properties, const std::vector<WidgetDesc>& in_childDescs);
            //------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The name of the widget type this desc will create.
            //------------------------------------------------------------------
            const std::string& GetType() const;
            //------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The map of properties the widget should be created with.
            //------------------------------------------------------------------
            const Core::PropertyMap& GetProperties() const;
            //------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The list of child widget descriptions.
            //------------------------------------------------------------------
            const std::vector<WidgetDesc>& GetChildDescs() const;
        private:
            std::string m_type = "";
            Core::PropertyMap m_propertyMap;
            std::vector<WidgetDesc> m_childDescs;
        };
    }
}

#endif
