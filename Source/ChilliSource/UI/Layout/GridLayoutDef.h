//
//  GridLayoutDef.h
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

#ifndef _CHILLISOURCE_UI_LAYOUT_GRIDLAYOUTDEF_H_
#define _CHILLISOURCE_UI_LAYOUT_GRIDLAYOUTDEF_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Math/Vector2.h>
#include <ChilliSource/Core/Math/Vector4.h>
#include <ChilliSource/UI/Layout/GridLayout.h>
#include <ChilliSource/UI/Layout/LayoutDef.h>

namespace ChilliSource
{
    namespace UI
    {
        //------------------------------------------------------------------------------
        /// A definition of a Grid Layout which can be built from a json object. This is
        /// effectively an immutable container for the properties of a Grid Layout.
        ///
        /// A Grid Layout Def contains the following property in addition to those
        /// defined in the Layout Def base class documentation:
        ///
        /// "CellOrder": The order that cells will be placed: row or column major order.
        ///
        /// "NumRows": The number of rows.
        ///
        /// "NumCols": The number of columns.
        ///
        /// "RelMargins": The relative padding from the widget edge to the cells
        /// in any layout.
        ///
        /// "AbsMargins": The absolute padding from the widget edge to the cells
        /// in any layout.
        ///
        /// "RelHSpacing": The relative horizontal spacing between cells.
        ///
        /// "AbsHSpacing": The absolute horizonal spacing between cells.
        ///
        /// "RelVSpacing": The relative vertical spacing between cells.
        ///
        /// "AbsVSpacing": The absolute vertical spacing between cells.
        ///
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------------
        class GridLayoutDef final : public LayoutDef
        {
        public:
            CS_DECLARE_NAMEDTYPE(GridLayoutDef);
            //------------------------------------------------------------------------------
            /// Constructor. Creates an empty grid layout definition.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            GridLayoutDef() = default;
            //------------------------------------------------------------------------------
            /// Constructor. Creates a standard grid layout definition from json.
            ///
            /// @author Ian Copland
            ///
            /// @param The JSON object containing the drawable properties.
            //------------------------------------------------------------------------------
            GridLayoutDef(const Json::Value& in_json);
            //------------------------------------------------------------------------------
            /// Constructor. Creates a grid layout definition with the given properties.
            ///
            /// @author Ian Copland
            ///
            /// @param The cell order.
            /// @param The number of rows.
            /// @param The number of columns.
            /// @param The relative margins.
            /// @param The absolute margins.
            /// @param The relative horizontal spacing.
            /// @param The absolute horizontal spacing.
            /// @param The relative vertical spacing.
            /// @param The absolute vertical spacing.
            //------------------------------------------------------------------------------
            GridLayoutDef(GridLayout::CellOrder in_cellOrder, u32 in_numRows, u32 in_numCols, const Core::Vector4& in_relMargins, const Core::Vector4& in_absMargins,
                          f32 in_relHorizSpacing, f32 in_absHorizSpacing, f32 in_relVertSpacing, f32 in_absVertSpacing);
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
            /// @return the cell order to be row or column major.
            //------------------------------------------------------------------------------
            GridLayout::CellOrder GetCellOrder() const;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The number of rows that make up the grid. The size of each row is
            /// based on the number of rows and the overall layout size, governed by the
            /// owning widget.
            //------------------------------------------------------------------------------
            u32 GetNumRows() const;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The number of columns that make up the grid. The size of each column
            /// is based on the number of columns and the overall layout size, governed by
            /// the owning widget.
            //------------------------------------------------------------------------------
            u32 GetNumCols() const;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The relative border size margins for each edge. These are relative
            /// to the overall layout size (i.e owning widget size). X is top, Y is right,
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
            /// @return The relative spacing between items horizontally. This is relative to
            /// the overall layout size (i.e. the owning widget)
            //------------------------------------------------------------------------------
            f32 GetRelativeHSpacing() const;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The absolute spacing between items horizontally.
            //------------------------------------------------------------------------------
            f32 GetAbsoluteHSpacing() const;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return the relative spacing between items vertically. This is relative to
            /// the overall layout size (i.e. the owning widget)
            //------------------------------------------------------------------------------
            f32 GetRelativeVSpacing() const;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The absolute spacing between items vertically.
            //------------------------------------------------------------------------------
            f32 GetAbsoluteVSpacing() const;
            
        private:
            //------------------------------------------------------------------------------
            /// Creates a new instance of a grid layout as described by this definition.
            /// This should typically only be called by a layout component.
            ///
            /// @author Ian Copland
            ///
            /// @param The owning component.
            ///
            /// @return a new layout instance.
            //------------------------------------------------------------------------------
            LayoutUPtr CreateLayout(LayoutComponent* in_owner) const override;
            
            GridLayout::CellOrder m_cellOrder = GridLayout::CellOrder::k_rowMajor;
            u32 m_numRows = 1;
            u32 m_numCols = 1;
            Core::Vector4 m_relativeMargins;
            Core::Vector4 m_absoluteMargins;
            f32 m_relativeHSpacing = 0.0f;
            f32 m_absoluteHSpacing = 0.0f;
            f32 m_relativeVSpacing = 0.0f;
            f32 m_absoluteVSpacing = 0.0f;
        };
    }
}

#endif
