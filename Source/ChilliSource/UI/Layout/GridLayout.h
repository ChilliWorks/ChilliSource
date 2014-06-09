//
//  GridLayout.h
//  Chilli Source
//  Created by Scott Downie on 09/06/2014.
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


#ifndef _CHILLISOURCE_UI_LAYOUT_GRIDLAYOUT_H_
#define _CHILLISOURCE_UI_LAYOUT_GRIDLAYOUT_H_

#include <ChilliSource/ChilliSource.h>

#include <ChilliSource/Core/Math/Vector2.h>
#include <ChilliSource/UI/Layout/ILayout.h>

namespace ChilliSource
{
    namespace UI
    {
        //----------------------------------------------------------------------------------------
        /// Lays out widgets in a grid where each widget has a single cell. The cells
        /// have spacing between them and the entire grid has a surrounding margin.
        ///
        /// The grid size is fixed and will not expand
        ///
        /// @author S Downie
        //----------------------------------------------------------------------------------------
        class GridLayout final : public ILayout
        {
        public:
            //----------------------------------------------------------------------------------------
            /// Describes the increment order of the cells.
            ///
            /// Col. Major: 0123
            ///             4567
            ///
            /// Row. Major: 0246
            ///             1357
            ///
            /// @author S Downie
            //----------------------------------------------------------------------------------------
            enum class CellOrder
            {
                k_colMajor,
                k_rowMajor
            };
            
            //----------------------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @param The number of rows that make up the grid. The size of each row is based
            /// on the number of rows and the overall layout size, governed by the owning widget
            //----------------------------------------------------------------------------------------
            void SetNumRows(u32 in_numRows);
            //----------------------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @param The number of columns that make up the grid. The size of each column is based
            /// on the number of columns and the overall layout size, governed by the owning widget
            //----------------------------------------------------------------------------------------
            void SetNumCols(u32 in_numCols);
            //----------------------------------------------------------------------------------------
            /// Set the relative border size margins for each edge. These are relative to
            /// the overall layout size (i.e owning widget size).
            ///
            /// @author S Downie
            ///
            /// @param Top
            /// @param Right
            /// @param Bottom
            /// @param Left
            //----------------------------------------------------------------------------------------
            void SetRelativeMargins(f32 in_top, f32 in_right, f32 in_bottom, f32 in_left);
            //----------------------------------------------------------------------------------------
            /// Set the absolute border size margins for each edge.
            ///
            /// @author S Downie
            ///
            /// @param Top
            /// @param Right
            /// @param Bottom
            /// @param Left
            //----------------------------------------------------------------------------------------
            void SetAbsoluteMargins(f32 in_top, f32 in_right, f32 in_bottom, f32 in_left);
            //----------------------------------------------------------------------------------------
            /// Set the relative spacing between items horizontally. This is relative to the overall layout size
            /// (i.e. the owning widget)
            ///
            /// @author S Downie
            ///
            /// @param Spacing
            //----------------------------------------------------------------------------------------
            void SetRelativeHSpacing(f32 in_spacing);
            //----------------------------------------------------------------------------------------
            /// Set the absolute spacing between items horizontally.
            ///
            /// @author S Downie
            ///
            /// @param Spacing
            //----------------------------------------------------------------------------------------
            void SetAbsoluteHSpacing(f32 in_spacing);
            //----------------------------------------------------------------------------------------
            /// Set the relative spacing between items vertically. This is relative to the overall layout size
            /// (i.e. the owning widget)
            ///
            /// @author S Downie
            ///
            /// @param Spacing
            //----------------------------------------------------------------------------------------
            void SetRelativeVSpacing(f32 in_spacing);
            //----------------------------------------------------------------------------------------
            /// Set the absolute spacing between items vertically.
            ///
            /// @author S Downie
            ///
            /// @param Spacing
            //----------------------------------------------------------------------------------------
            void SetAbsoluteVSpacing(f32 in_spacing);
            //----------------------------------------------------------------------------------------
            /// Set the cell order to be row or column major
            ///
            /// @author S Downie
            ///
            /// @param Cell order
            //----------------------------------------------------------------------------------------
            void SetCellOrder(CellOrder in_order);
            //----------------------------------------------------------------------------------------
            /// Lays out widgets in a grid where each widget has a single cell.
            /// The layout will use the currently set padding and margins
            ///
            /// @author S Downie
            ///
            /// @param Widget that is performing the layout
            /// @param Widgets to layout
            //----------------------------------------------------------------------------------------
            void BuildLayout(const Widget* in_root, const std::vector<WidgetSPtr>& in_widgets) override;
            //----------------------------------------------------------------------------------------
            /// Get the final size of the widget that occupies the space at the given index
            ///
            /// @author S Downie
            ///
            /// @param Space index
            ///
            /// @return Final absolute screen size
            //----------------------------------------------------------------------------------------
            Core::Vector2 GetSizeForIndex(u32 in_index) const override;
            //----------------------------------------------------------------------------------------
            /// Get the local position of the widget that occupies the space at the given index
            ///
            /// @author S Downie
            ///
            /// @param Space index
            ///
            /// @return Local position in layout space (aligned middle centre of the cell)
            //----------------------------------------------------------------------------------------
            Core::Vector2 GetPositionForIndex(u32 in_index) const override;
            
        private:
            
            Core::Vector2 m_cellSize;
            
            Core::Vector2 m_marginSizeTop;
            Core::Vector2 m_marginSizeBottom;
            Core::Vector2 m_marginSizeLeft;
            Core::Vector2 m_marginSizeRight;
            
            Core::Vector2 m_spacingSizeH;
            Core::Vector2 m_spacingSizeV;
            f32 m_finalSpacingSizeH;
            f32 m_finalSpacingSizeV;
            f32 m_firstCellPosX;
            f32 m_firstCellPosY;
            
            u32 m_numRows = 1;
            u32 m_numCols = 1;
            
            CellOrder m_cellOrder = CellOrder::k_colMajor;
        };
    }
}

#endif