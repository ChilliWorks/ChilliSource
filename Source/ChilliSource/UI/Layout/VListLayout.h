//
//  VListLayout.h
//  Chilli Source
//  Created by Scott Downie on 23/04/2014.
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

#ifndef _CHILLISOURCE_UI_LAYOUT_VLISTLAYOUT_H_
#define _CHILLISOURCE_UI_LAYOUT_VLISTLAYOUT_H_

#include <ChilliSource/ChilliSource.h>

#include <ChilliSource/Core/Math/Vector2.h>
#include <ChilliSource/UI/Layout/GridLayout.h>
#include <ChilliSource/UI/Layout/Layout.h>

namespace ChilliSource
{
    namespace UI
    {
        //------------------------------------------------------------------------------
        /// Lays out widgets in a vertical list where each widget has a single row. The
        /// rows have spacing between them and the entire list has a surrounding margin.
        /// The list is filled top to bottom.
        ///
        /// The list size is fixed and will not expand
        ///
        /// @author S Downie
        //------------------------------------------------------------------------------
        class VListLayout final : public Layout
        {
        public:
            CS_DECLARE_NAMEDTYPE(VListLayout);
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
            /// @return The number of cells that make up the list. The size of each cell is
            /// based on the number of cells and the overall layout size, governed by the
            /// owning widget
            //------------------------------------------------------------------------------
            u32 GetNumCells() const;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The relative border size margins for each edge. These are relative
            /// to the overall layout size (i.e owning widget size).X is top, Y is right,
            /// Z is bottom and W is left.
            //------------------------------------------------------------------------------
            Core::Vector4 GetRelativeMargins() const;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The absolute border size margins for each edge. X is top, Y is
            /// right, Z is bottom and W is left.
            //------------------------------------------------------------------------------
            Core::Vector4 GetAbsoluteMargins() const;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The relative spacing between items. This is relative to the overall
            /// layout size (i.e. the owning widget)
            //------------------------------------------------------------------------------
            f32 GetRelativeSpacing() const;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The absolute spacing between items.
            //------------------------------------------------------------------------------
            f32 GetAbsoluteSpacing() const;
            //------------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @param The number of cells that make up the list. The size of each cell is
            /// based on the number of cells and the overall layout size, governed by the
            /// owning widget
            //------------------------------------------------------------------------------
            void SetNumCells(u32 in_numCells);
            //------------------------------------------------------------------------------
            /// Set the relative border size margins for each edge. These are relative to
            /// the overall layout size (i.e owning widget size).
            ///
            /// @author S Downie
            ///
            /// @param Top
            /// @param Right
            /// @param Bottom
            /// @param Left
            //------------------------------------------------------------------------------
            void SetRelativeMargins(f32 in_top, f32 in_right, f32 in_bottom, f32 in_left);
            //------------------------------------------------------------------------------
            /// Set the absolute border size margins for each edge.
            ///
            /// @author S Downie
            ///
            /// @param Top
            /// @param Right
            /// @param Bottom
            /// @param Left
            //------------------------------------------------------------------------------
            void SetAbsoluteMargins(f32 in_top, f32 in_right, f32 in_bottom, f32 in_left);
            //------------------------------------------------------------------------------
            /// Set the relative spacing between items. This is relative to the overall
            /// layout size (i.e. the owning widget)
            ///
            /// @author S Downie
            ///
            /// @param Spacing
            //------------------------------------------------------------------------------
            void SetRelativeSpacing(f32 in_spacing);
            //------------------------------------------------------------------------------
            /// Set the absolute spacing between items.
            ///
            /// @author S Downie
            ///
            /// @param Spacing
            //------------------------------------------------------------------------------
            void SetAbsoluteSpacing(f32 in_spacing);
            
        private:
            friend class VListLayoutDef;
            //------------------------------------------------------------------------------
            /// Constructor. Creates a vlist layout with the given properties.
            ///
            /// @author Ian Copland
            ///
            /// @param The owning component.
            /// @param The number of cells.
            /// @param The relative margins.
            /// @param The absolute margins.
            /// @param The relative spacing.
            /// @param The absolute spacing.
            //------------------------------------------------------------------------------
            VListLayout(LayoutComponent* in_layoutComponent, u32 in_numCells, const Core::Vector4& in_relMargins, const Core::Vector4& in_absMargins, f32 in_relSpacing, f32 in_absSpacing);
            //------------------------------------------------------------------------------
            /// Create the list layout sizes and positions based on the current layout
            /// properties and the owning widget.
            ///
            /// @author S Downie
            //------------------------------------------------------------------------------
            void BuildLayout() override;
            //------------------------------------------------------------------------------
            /// Get the final size of the widget that occupies the space at the given index
            ///
            /// @author S Downie
            ///
            /// @param Space index
            ///
            /// @return Final absolute screen size
            //------------------------------------------------------------------------------
            Core::Vector2 GetSizeForIndex(u32 in_index) const override;
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
            Core::Vector2 GetPositionForIndex(u32 in_index) const override;
            
            GridLayout m_gridLayout;
        };
    }
}

#endif