//
//  GridLayout.cpp
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

#include <ChilliSource/UI/Layout/GridLayout.h>

#include <ChilliSource/Core/Math/Vector4.h>
#include <ChilliSource/UI/Base/Widget.h>
#include <ChilliSource/UI/Layout/LayoutComponent.h>

namespace ChilliSource
{
    namespace UI
    {
        CS_DEFINE_NAMEDTYPE(GridLayout);
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        GridLayout::GridLayout(LayoutComponent* in_layoutComponent, GridLayout::CellOrder in_cellOrder, u32 in_numRows, u32 in_numCols, const Core::Vector4& in_relMargins, const Core::Vector4& in_absMargins,
                               f32 in_relHorizSpacing, f32 in_absHorizSpacing, f32 in_relVertSpacing, f32 in_absVertSpacing)
            : Layout(in_layoutComponent), m_cellOrder(in_cellOrder), m_numRows(in_numRows), m_numCols(in_numCols), m_marginSizeTop(in_relMargins.x, in_absMargins.x), m_marginSizeRight(in_relMargins.y, in_absMargins.y),
            m_marginSizeBottom(in_relMargins.z, in_absMargins.z), m_marginSizeLeft(in_relMargins.w, in_absMargins.w), m_spacingSizeH(in_relHorizSpacing, in_absHorizSpacing), m_spacingSizeV(in_relVertSpacing, in_absVertSpacing)
        {
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        bool GridLayout::IsA(Core::InterfaceIDType in_interfaceId) const
        {
            return (Layout::InterfaceID == in_interfaceId || GridLayout::InterfaceID == in_interfaceId);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        u32 GridLayout::GetNumRows() const
        {
            return m_numRows;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        u32 GridLayout::GetNumCols() const
        {
            return m_numCols;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        Core::Vector4 GridLayout::GetRelativeMargins() const
        {
            return Core::Vector4(m_marginSizeTop.x, m_marginSizeRight.x, m_marginSizeBottom.x, m_marginSizeLeft.x);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        Core::Vector4 GridLayout::GetAbsoluteMargins() const
        {
            return Core::Vector4(m_marginSizeTop.y, m_marginSizeRight.y, m_marginSizeBottom.y, m_marginSizeLeft.y);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        f32 GridLayout::GetRelativeHSpacing() const
        {
            return m_spacingSizeH.x;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        f32 GridLayout::GetAbsoluteHSpacing() const
        {
            return m_spacingSizeH.y;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        f32 GridLayout::GetRelativeVSpacing() const
        {
            return m_spacingSizeV.x;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        f32 GridLayout::GetAbsoluteVSpacing() const
        {
            return m_spacingSizeV.y;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        GridLayout::CellOrder GridLayout::GetCellOrder() const
        {
            return m_cellOrder;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void GridLayout::SetNumRows(u32 in_numRos)
        {
            CS_ASSERT(in_numRos > 0, "Cannot create a grid with 0 rows");
            m_numRows = in_numRos;
            
            GetComponent()->OnLayoutChanged();
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void GridLayout::SetNumCols(u32 in_numCols)
        {
            CS_ASSERT(in_numCols > 0, "Cannot create a grid with 0 columns");
            m_numCols = in_numCols;
            
            GetComponent()->OnLayoutChanged();
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void GridLayout::SetRelativeMargins(f32 in_top, f32 in_right, f32 in_bottom, f32 in_left)
        {
            m_marginSizeTop.x = in_top;
            m_marginSizeRight.x = in_right;
            m_marginSizeBottom.x = in_bottom;
            m_marginSizeLeft.x = in_left;
            
            GetComponent()->OnLayoutChanged();
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void GridLayout::SetAbsoluteMargins(f32 in_top, f32 in_right, f32 in_bottom, f32 in_left)
        {
            m_marginSizeTop.y = in_top;
            m_marginSizeRight.y = in_right;
            m_marginSizeBottom.y = in_bottom;
            m_marginSizeLeft.y = in_left;
            
            GetComponent()->OnLayoutChanged();
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void GridLayout::SetRelativeHSpacing(f32 in_spacing)
        {
            m_spacingSizeH.x = in_spacing;
            
            GetComponent()->OnLayoutChanged();
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void GridLayout::SetAbsoluteHSpacing(f32 in_spacing)
        {
            m_spacingSizeH.y = in_spacing;
            
            GetComponent()->OnLayoutChanged();
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void GridLayout::SetRelativeVSpacing(f32 in_spacing)
        {
            m_spacingSizeV.x = in_spacing;
            
            GetComponent()->OnLayoutChanged();
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void GridLayout::SetAbsoluteVSpacing(f32 in_spacing)
        {
            m_spacingSizeV.y = in_spacing;
            
            GetComponent()->OnLayoutChanged();
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void GridLayout::SetCellOrder(CellOrder in_order)
        {
            m_cellOrder = in_order;
            
            GetComponent()->OnLayoutChanged();
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void GridLayout::BuildLayout()
        {
            CS_ASSERT(GetComponent()->GetWidget() != nullptr, "Cannot build layout when Layout Component is not attached to widget.");
            
            //The list doesn't care about the widgets as the size and position of each cell
            //is only relative to the number of cells and the root widget size
            Core::Vector2 rootSize = GetComponent()->GetWidget()->GetFinalSize();
            
            //The margins and spacing are relative to the root widget size
            m_finalSpacingSizeH = m_spacingSizeH.y + (rootSize.x * m_spacingSizeH.x);
            m_finalSpacingSizeV = m_spacingSizeV.y + (rootSize.y * m_spacingSizeV.x);
            
            f32 finalMarginTop = m_marginSizeTop.y + (rootSize.y * m_marginSizeTop.x);
            f32 finalMarginBottom = m_marginSizeBottom.y + (rootSize.y * m_marginSizeBottom.x);
            f32 finalMarginLeft = m_marginSizeLeft.y + (rootSize.x * m_marginSizeLeft.x);
            f32 finalMarginRight = m_marginSizeRight.y + (rootSize.x * m_marginSizeRight.x);
            
            m_cellSize.x = (rootSize.x - finalMarginLeft - finalMarginRight - (m_finalSpacingSizeH * (m_numCols-1)))/m_numCols;
            m_cellSize.y = (rootSize.y - finalMarginTop - finalMarginBottom - (m_finalSpacingSizeV * (m_numRows-1)))/m_numRows;
            
            m_firstCellPosX = finalMarginLeft;
            m_firstCellPosY = rootSize.y - finalMarginTop;
        }
        //------------------------------------------------------------------------------
        /// The cell size if fixed and uniform so the index is not required
        //------------------------------------------------------------------------------
        Core::Vector2 GridLayout::GetSizeForIndex(u32 in_index) const
        {
            CS_ASSERT(in_index < (m_numRows * m_numCols), "Cannot have more items in grid than number of cells(" + Core::ToString(m_numRows * m_numCols) + ")");
            return m_cellSize;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        Core::Vector2 GridLayout::GetPositionForIndex(u32 in_index) const
        {
            //Aligned to the middle centre in cell space
            CS_ASSERT(in_index < (m_numRows * m_numCols), "Cannot have more items in grid than number of cells(" + Core::ToString(m_numRows * m_numCols) + ")");
            
            u32 indexCol = 0;
            u32 indexRow = 0;
            switch(m_cellOrder)
            {
                case CellOrder::k_colMajor:
                    indexCol = in_index % m_numCols;
                    indexRow = in_index / m_numCols;
                    break;
                case CellOrder::k_rowMajor:
                    indexCol = in_index / m_numRows;
                    indexRow = in_index % m_numRows;
                    break;
            }
        
            //Col
            f32 spacingSizeH = indexCol == 0 ? 0.0f : m_finalSpacingSizeH;
            f32 xPos = m_firstCellPosX + (m_cellSize.x * (indexCol + 1)) + (spacingSizeH * indexCol);
            xPos -= (m_cellSize.x * 0.5f);
            
            //Row
            f32 spacingSizeV = indexRow == 0 ? 0.0f : m_finalSpacingSizeV;
            f32 yPos = m_firstCellPosY - (m_cellSize.y * (indexRow + 1)) - (spacingSizeV * indexRow);
            yPos += (m_cellSize.y * 0.5f);
            
            return Core::Vector2(xPos, yPos);
        }
    }
}