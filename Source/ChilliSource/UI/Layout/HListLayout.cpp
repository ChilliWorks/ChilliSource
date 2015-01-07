//
//  HListLayout.cpp
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

#include <ChilliSource/UI/Layout/HListLayout.h>

#include <ChilliSource/Core/Math/Vector4.h>
#include <ChilliSource/UI/Base/Widget.h>

namespace ChilliSource
{
    namespace UI
    {
        CS_DEFINE_NAMEDTYPE(HListLayout);
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        HListLayout::HListLayout(LayoutComponent* in_layoutComponent, u32 in_numCells, const Core::Vector4& in_relMargins, const Core::Vector4& in_absMargins, f32 in_relSpacing, f32 in_absSpacing)
            : Layout(in_layoutComponent), m_gridLayout(in_layoutComponent, GridLayout::CellOrder::k_colMajor, 1, in_numCells, in_relMargins, in_absMargins, in_relSpacing, in_absSpacing, 0.0f, 0.0f)
        {
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        bool HListLayout::IsA(Core::InterfaceIDType in_interfaceId) const
        {
            return (Layout::InterfaceID == in_interfaceId || HListLayout::InterfaceID == in_interfaceId);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        u32 HListLayout::GetNumCells() const
        {
            return m_gridLayout.GetNumCols();
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        Core::Vector4 HListLayout::GetRelativeMargins() const
        {
            return m_gridLayout.GetRelativeMargins();
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        Core::Vector4 HListLayout::GetAbsoluteMargins() const
        {
            return m_gridLayout.GetAbsoluteMargins();
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        f32 HListLayout::GetRelativeSpacing() const
        {
            return m_gridLayout.GetRelativeHSpacing();
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        f32 HListLayout::GetAbsoluteSpacing() const
        {
            return m_gridLayout.GetAbsoluteHSpacing();
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void HListLayout::SetNumCells(u32 in_numCells)
        {
            CS_ASSERT(in_numCells > 0, "Cannot create a list with 0 cells");
            m_gridLayout.SetNumCols(in_numCells);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void HListLayout::SetRelativeMargins(f32 in_top, f32 in_right, f32 in_bottom, f32 in_left)
        {
            m_gridLayout.SetRelativeMargins(in_top, in_right, in_bottom, in_left);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void HListLayout::SetAbsoluteMargins(f32 in_top, f32 in_right, f32 in_bottom, f32 in_left)
        {
            m_gridLayout.SetAbsoluteMargins(in_top, in_right, in_bottom, in_left);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void HListLayout::SetRelativeSpacing(f32 in_spacing)
        {
            m_gridLayout.SetRelativeHSpacing(in_spacing);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void HListLayout::SetAbsoluteSpacing(f32 in_spacing)
        {
            m_gridLayout.SetAbsoluteHSpacing(in_spacing);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void HListLayout::BuildLayout()
        {
            m_gridLayout.BuildLayout();
        }
        //------------------------------------------------------------------------------
        /// The cell size if fixed and uniform so the index is not required
        //------------------------------------------------------------------------------
        Core::Vector2 HListLayout::GetSizeForIndex(u32 in_index) const
        {
            return m_gridLayout.GetSizeForIndex(in_index);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        Core::Vector2 HListLayout::GetPositionForIndex(u32 in_index) const
        {
            return m_gridLayout.GetPositionForIndex(in_index);
        }
    }
}