//
//  LayoutComponent.h
//  Chilli Source
//  Created by Ian Copland on 07/01/2015.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2015 Tag Games Limited
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

#ifndef _CHILLISOURCE_UI_LAYOUT_LAYOUTCOMPONENT_H_
#define _CHILLISOURCE_UI_LAYOUT_LAYOUTCOMPONENT_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Container/Property/PropertyMap.h>
#include <ChilliSource/Core/Event/EventConnection.h>
#include <ChilliSource/Core/Math/Vector2.h>
#include <ChilliSource/Input/Pointer/Pointer.h>
#include <ChilliSource/UI/Base/Component.h>
#include <ChilliSource/UI/Layout/Layout.h>

namespace ChilliSource
{
    namespace UI
    {
        //------------------------------------------------------------------------------
        /// A component for applying layouts to the children of the owning widget.
        /// There are 3 types of layout: Grid, HList and VList.
        ///
        /// A LayoutComponent contains the following properties:
        ///
        /// "Layout": The definition of the layout that will be applied. The
        /// contents of a layout def are described in the layout def documentation.
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------------
        class LayoutComponent final : public Component
        {
        public:
            CS_DECLARE_NAMEDTYPE(LayoutComponent);
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The list of properties supported by a layout component.
            //------------------------------------------------------------------------------
            static const std::vector<Core::PropertyMap::PropertyDesc>& GetPropertyDescs();
            //------------------------------------------------------------------------------
            /// Allows querying of whether or not this system implements the interface
            /// described by the given interface Id. Typically this is not called directly
            /// as the templated equivalent IsA<Interface>() is preferred.
            ///
            /// @author Ian Copland
            ///
            /// @param The interface Id.
            ///
            /// @return Whether or not the interface is implemented.
            //------------------------------------------------------------------------------
            bool IsA(Core::InterfaceIDType in_interfaceId) const override;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The layout object that will be used to apply the layout to children.
            /// This can be changed directly, for example to increase the number of cells,
            /// etc.
            //------------------------------------------------------------------------------
            Layout* GetLayout();
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return A const pointer to the layout object that will be used to apply the
            /// layout to children.
            //------------------------------------------------------------------------------
            const Layout* GetLayout() const;
            //------------------------------------------------------------------------------
            /// Applies a new Layout from the given layout definition.
            ///
            /// @author Ian Copland
            ///
            /// @param The layout defintion.
            //------------------------------------------------------------------------------
            void ApplyLayoutDef(const LayoutDefCSPtr& in_layoutDef);
            //------------------------------------------------------------------------------
            /// Create the grid layout sizes and positions based on the current layout
            /// properties and the owning widget.
            ///
            /// @author S Downie
            //------------------------------------------------------------------------------
            void BuildLayout();
            //------------------------------------------------------------------------------
            /// Get the final size of the widget that occupies the space at the given index
            ///
            /// @author S Downie
            ///
            /// @param Space index
            ///
            /// @return Final absolute screen size
            //------------------------------------------------------------------------------
            Core::Vector2 GetSizeForIndex(u32 in_index) const;
            //------------------------------------------------------------------------------
            /// Get the local position of the widget that occupies the space at the given
            /// index
            ///
            /// @author S Downie
            ///
            /// @param Space index
            ///
            /// @return Local position in layout space (aligned middle centre of the cell)
            //------------------------------------------------------------------------------
            Core::Vector2 GetPositionForIndex(u32 in_index) const;
            //------------------------------------------------------------------------------
            /// Called when the internal layout changes so that the widget can be told to
            /// update its children. This is for internal use and should not be called by
            /// the user of the API.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            void OnLayoutChanged();
        private:
            friend class ComponentFactory;
            //------------------------------------------------------------------------------
            /// Constructor that builds the component from key-value properties. The
            /// properties used to create a drawable component are described in the class
            /// documentation.
            ///
            /// @author Ian Copland
            ///
            /// @param The component name.
            /// @param The property map.
            //------------------------------------------------------------------------------
            LayoutComponent(const std::string& in_componentName, const Core::PropertyMap& in_properties);
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The layout definition that was used to create the underlying layout
            /// object.
            //------------------------------------------------------------------------------
            const LayoutDefCSPtr& GetLayoutDef() const;
            
            LayoutDefCSPtr m_layoutDef;
            LayoutUPtr m_layout;
        };
    }
}

#endif