//
//  VListUILayout.cpp
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

#include <ChilliSource/UI/Layout/VListUILayout.h>

#include <ChilliSource/Core/Math/Vector4.h>
#include <ChilliSource/UI/Base/Widget.h>

namespace ChilliSource
{
    CS_DEFINE_NAMEDTYPE(VListUILayout);
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    VListUILayout::VListUILayout(LayoutUIComponent* in_layoutComponent, u32 in_numCells, const Vector4& in_relMargins, const Vector4& in_absMargins, f32 in_relSpacing, f32 in_absSpacing)
        : UILayout(in_layoutComponent), m_gridLayout(in_layoutComponent, GridUILayout::CellOrder::k_colMajor, in_numCells, 1, in_relMargins, in_absMargins, 0.0f, 0.0f, in_relSpacing, in_absSpacing)
    {
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    bool VListUILayout::IsA(InterfaceIDType in_interfaceId) const
    {
        return (UILayout::InterfaceID == in_interfaceId || VListUILayout::InterfaceID == in_interfaceId);
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    u32 VListUILayout::GetNumCells() const
    {
        return m_gridLayout.GetNumRows();
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    Vector4 VListUILayout::GetRelativeMargins() const
    {
        return m_gridLayout.GetRelativeMargins();
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    Vector4 VListUILayout::GetAbsoluteMargins() const
    {
        return m_gridLayout.GetAbsoluteMargins();
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    f32 VListUILayout::GetRelativeSpacing() const
    {
        return m_gridLayout.GetRelativeVSpacing();
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    f32 VListUILayout::GetAbsoluteSpacing() const
    {
        return m_gridLayout.GetAbsoluteVSpacing();
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    void VListUILayout::SetNumCells(u32 in_numCells)
    {
        CS_ASSERT(in_numCells > 0, "Cannot create a list with 0 cells");
        m_gridLayout.SetNumRows(in_numCells);
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    void VListUILayout::SetRelativeMargins(f32 in_top, f32 in_right, f32 in_bottom, f32 in_left)
    {
        m_gridLayout.SetRelativeMargins(in_top, in_right, in_bottom, in_left);
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    void VListUILayout::SetAbsoluteMargins(f32 in_top, f32 in_right, f32 in_bottom, f32 in_left)
    {
        m_gridLayout.SetAbsoluteMargins(in_top, in_right, in_bottom, in_left);
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    void VListUILayout::SetRelativeSpacing(f32 in_spacing)
    {
        m_gridLayout.SetRelativeVSpacing(in_spacing);
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    void VListUILayout::SetAbsoluteSpacing(f32 in_spacing)
    {
        m_gridLayout.SetAbsoluteVSpacing(in_spacing);
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    void VListUILayout::BuildLayout()
    {
        m_gridLayout.BuildLayout();
    }
    //------------------------------------------------------------------------------
    /// The cell size if fixed and uniform so the index is not required
    //------------------------------------------------------------------------------
    Vector2 VListUILayout::GetSizeForIndex(u32 in_index) const
    {
        return m_gridLayout.GetSizeForIndex(in_index);
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    Vector2 VListUILayout::GetPositionForIndex(u32 in_index) const
    {
        return m_gridLayout.GetPositionForIndex(in_index);
    }
}