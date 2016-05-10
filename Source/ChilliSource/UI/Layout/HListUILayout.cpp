//
//  HListUILayout.cpp
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

#include <ChilliSource/UI/Layout/HListUILayout.h>

#include <ChilliSource/Core/Math/Vector4.h>
#include <ChilliSource/UI/Base/Widget.h>

namespace ChilliSource
{
    CS_DEFINE_NAMEDTYPE(HListUILayout);
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    HListUILayout::HListUILayout(LayoutUIComponent* in_layoutComponent, u32 in_numCells, const Vector4& in_relMargins, const Vector4& in_absMargins, f32 in_relSpacing, f32 in_absSpacing)
        : UILayout(in_layoutComponent), m_gridLayout(in_layoutComponent, GridUILayout::CellOrder::k_rowMajor, 1, in_numCells, in_relMargins, in_absMargins, in_relSpacing, in_absSpacing, 0.0f, 0.0f)
    {
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    bool HListUILayout::IsA(InterfaceIDType in_interfaceId) const
    {
        return (UILayout::InterfaceID == in_interfaceId || HListUILayout::InterfaceID == in_interfaceId);
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    u32 HListUILayout::GetNumCells() const
    {
        return m_gridLayout.GetNumCols();
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    Vector4 HListUILayout::GetRelativeMargins() const
    {
        return m_gridLayout.GetRelativeMargins();
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    Vector4 HListUILayout::GetAbsoluteMargins() const
    {
        return m_gridLayout.GetAbsoluteMargins();
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    f32 HListUILayout::GetRelativeSpacing() const
    {
        return m_gridLayout.GetRelativeHSpacing();
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    f32 HListUILayout::GetAbsoluteSpacing() const
    {
        return m_gridLayout.GetAbsoluteHSpacing();
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    void HListUILayout::SetNumCells(u32 in_numCells)
    {
        CS_ASSERT(in_numCells > 0, "Cannot create a list with 0 cells");
        m_gridLayout.SetNumCols(in_numCells);
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    void HListUILayout::SetRelativeMargins(f32 in_top, f32 in_right, f32 in_bottom, f32 in_left)
    {
        m_gridLayout.SetRelativeMargins(in_top, in_right, in_bottom, in_left);
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    void HListUILayout::SetAbsoluteMargins(f32 in_top, f32 in_right, f32 in_bottom, f32 in_left)
    {
        m_gridLayout.SetAbsoluteMargins(in_top, in_right, in_bottom, in_left);
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    void HListUILayout::SetRelativeSpacing(f32 in_spacing)
    {
        m_gridLayout.SetRelativeHSpacing(in_spacing);
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    void HListUILayout::SetAbsoluteSpacing(f32 in_spacing)
    {
        m_gridLayout.SetAbsoluteHSpacing(in_spacing);
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    void HListUILayout::BuildLayout()
    {
        m_gridLayout.BuildLayout();
    }
    //------------------------------------------------------------------------------
    /// The cell size if fixed and uniform so the index is not required
    //------------------------------------------------------------------------------
    Vector2 HListUILayout::GetSizeForIndex(u32 in_index) const
    {
        return m_gridLayout.GetSizeForIndex(in_index);
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    Vector2 HListUILayout::GetPositionForIndex(u32 in_index) const
    {
        return m_gridLayout.GetPositionForIndex(in_index);
    }
}