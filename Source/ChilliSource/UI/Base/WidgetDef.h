//
//  WidgetDef.h
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

#ifndef _CHILLISOURCE_UI_BASE_WIDGETDEF_H_
#define _CHILLISOURCE_UI_BASE_WIDGETDEF_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Container/Property/PropertyMap.h>
#include <ChilliSource/Core/Resource/Resource.h>
#include <ChilliSource/UI/Base/ComponentDesc.h>
#include <ChilliSource/UI/Base/PropertyLink.h>
#include <ChilliSource/UI/Base/WidgetDesc.h>

#include <vector>

namespace ChilliSource
{
    namespace UI
    {
        //-------------------------------------------------------------
        /// A resource that holds the description for defining new
        /// widget types. Widgets created from defines have provate implementation
        /// details such as child widgets and are supposed to be treated as
        /// as single widget rather than as a screen. Widgets created from
        /// defines rarely have any properties set only their behaviour and
        /// hiearchy is fixed and there they require positioning and drawables
        /// etc before they can be added to the canvas.
        ///
        /// @author S Downie
        //-------------------------------------------------------------
        class WidgetDef final : public Core::Resource
        {
        public:
            CS_DECLARE_NAMEDTYPE(WidgetDef);
            //-------------------------------------------------------
            /// Is the object of the given interface type.
            ///
            /// @author S Downie
            ///
            /// @param Interface type to query
            ///
            /// @return Whether the object is of given type
            //-------------------------------------------------------
            bool IsA(Core::InterfaceIDType in_interfaceId) const override;
            //-------------------------------------------------------
            /// Build the widget def resource, consisting of its
            /// default properties, components and children.
            ///
            /// @author Ian Copland
            ///
            /// @param The name of the widget type.
            /// @param The property map containing all of the keys
            /// for this widget type--both the base properties all
            /// widgets have and linked properties. The values of
            /// each property is the default value in any instance
            /// of the widget being used.
            /// @param The list of components. This can be empty if
            /// no components are required.
            /// @param The links between properties of the widget def
            /// and properties of components.
            /// @param The list of internal child descriptions. This
            /// can be empty if no internal children are required.
            /// @param The links between properties of the widget
            /// def and properties of internal children.
            //-------------------------------------------------------
            void Build(const std::string& in_typeName, const Core::PropertyMap& in_defaultProperties, const std::vector<ComponentDesc>& in_componentDescs, const std::vector<PropertyLink>& in_componentPropertyLinks,
                       const std::vector<WidgetDesc>& in_childDescs, const std::vector<PropertyLink>& in_childPropertyLinks);
            //-------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The name of the widget type.
            //-------------------------------------------------------
            const std::string& GetTypeName() const;
            //-------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The property map containing all of the keys
            /// for this widget type--both the base properties all
            /// widgets have and linked properties. All values are
            /// set to their default value.
            //-------------------------------------------------------
            const Core::PropertyMap& GetDefaultProperties() const;
            //-------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The list of component descriptions.
            //-------------------------------------------------------
            const std::vector<ComponentDesc>& GetComponentDescs() const;
            //-------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The links between properties of the widget
            /// def and properties of components.
            //-------------------------------------------------------
            const std::vector<PropertyLink>& GetComponentPropertyLinks() const;
            //-------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The list of internal child descriptions.
            //-------------------------------------------------------
            const std::vector<WidgetDesc>& GetChildDescs() const;
            //-------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The links between properties of the widget
            /// def and properties of internal children.
            //-------------------------------------------------------
            const std::vector<PropertyLink>& GetChildPropertyLinks() const;
            
        private:
            friend class Core::ResourcePool;
            //-------------------------------------------------------
            /// Factory method
            ///
            /// @author S Downie
            ///
            /// @return New backend with ownership transferred
            //-------------------------------------------------------
            static WidgetDefUPtr Create();
            //-------------------------------------------------------
            /// Private constructor to force use of factory method
            ///
            /// @author S Downie
            //-------------------------------------------------------
            WidgetDef() = default;
            
        private:
            std::string m_typeName;
            Core::PropertyMap m_defaultProperties;
            std::vector<ComponentDesc> m_componentDescs;
            std::vector<PropertyLink> m_componentPropertyLinks;
            std::vector<WidgetDesc> m_childDescs;
            std::vector<PropertyLink> m_childPropertyLinks;
        };
    }
}

#endif

