//
//  VListLayout.cpp
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

#include <ChilliSource/UI/Layout/VListLayout.h>

#include <ChilliSource/Core/Math/Vector4.h>
#include <ChilliSource/UI/Base/Widget.h>

namespace ChilliSource
{
    namespace UI
    {
        namespace
        {
            const std::vector<PropertyMap::PropertyDesc> k_propertyDescs =
            {
                {PropertyType::k_string, "Type", "VList"},
                {PropertyType::k_int, "NumCells", "1"},
                {PropertyType::k_float, "RelSpacing", "0"},
                {PropertyType::k_float, "AbsSpacing", "0"},
                {PropertyType::k_vec4, "RelMargins", "0 0 0 0"},
                {PropertyType::k_vec4, "AbsMargins", "0 0 0 0"}
            };
        }
        
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        VListLayout::VListLayout()
        {
            m_gridLayout.SetNumCols(1);
            m_gridLayout.SetCellOrder(GridLayout::CellOrder::k_rowMajor);
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        VListLayout::VListLayout(const PropertyMap& in_properties)
        {
            m_gridLayout.SetNumCols(1);
            m_gridLayout.SetCellOrder(GridLayout::CellOrder::k_rowMajor);
            
            SetNumCells(in_properties.GetProperty<s32>("NumCells"));
            SetRelativeSpacing(in_properties.GetProperty<f32>("RelSpacing"));
            SetAbsoluteSpacing(in_properties.GetProperty<f32>("AbsSpacing"));
            
            Core::Vector4 relMargins(in_properties.GetProperty<Core::Vector4>("RelMargins"));
            SetRelativeMargins(relMargins.x, relMargins.y, relMargins.z, relMargins.w);
            Core::Vector4 absMargins(in_properties.GetProperty<Core::Vector4>("AbsMargins"));
            SetAbsoluteMargins(absMargins.x, absMargins.y, absMargins.z, absMargins.w);
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        std::vector<PropertyMap::PropertyDesc> VListLayout::GetPropertyDescs()
        {
            return k_propertyDescs;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void VListLayout::SetNumCells(u32 in_numCells)
        {
            CS_ASSERT(in_numCells > 0, "Cannot create a list with 0 cells");
            m_gridLayout.SetNumRows(in_numCells);
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void VListLayout::SetRelativeMargins(f32 in_top, f32 in_right, f32 in_bottom, f32 in_left)
        {
            m_gridLayout.SetRelativeMargins(in_top, in_right, in_bottom, in_left);
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void VListLayout::SetAbsoluteMargins(f32 in_top, f32 in_right, f32 in_bottom, f32 in_left)
        {
            m_gridLayout.SetAbsoluteMargins(in_top, in_right, in_bottom, in_left);
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void VListLayout::SetRelativeSpacing(f32 in_spacing)
        {
            m_gridLayout.SetRelativeVSpacing(in_spacing);
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void VListLayout::SetAbsoluteSpacing(f32 in_spacing)
        {
            m_gridLayout.SetAbsoluteVSpacing(in_spacing);
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void VListLayout::BuildLayout()
        {
            m_gridLayout.BuildLayout();
        }
        //----------------------------------------------------------------------------------------
        /// The cell size if fixed and uniform so the index is not required
        //----------------------------------------------------------------------------------------
        Core::Vector2 VListLayout::GetSizeForIndex(u32 in_index) const
        {
            return m_gridLayout.GetSizeForIndex(in_index);
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        Core::Vector2 VListLayout::GetPositionForIndex(u32 in_index) const
        {
            return m_gridLayout.GetPositionForIndex(in_index);
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void VListLayout::SetWidget(Widget* in_widget)
        {
            m_gridLayout.SetWidget(in_widget);
        }
    }
}