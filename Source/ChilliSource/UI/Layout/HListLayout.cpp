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
#include <ChilliSource/UI/Layout/LayoutType.h>

namespace ChilliSource
{
    namespace UI
    {
        namespace
        {
            const std::vector<PropertyMap::PropertyDesc> k_propertyDescs =
            {
                {PropertyType::k_string, "Type"},
                {PropertyType::k_int, "NumCells"},
                {PropertyType::k_float, "RelSpacing"},
                {PropertyType::k_float, "AbsSpacing"},
                {PropertyType::k_vec4, "RelMargins"},
                {PropertyType::k_vec4, "AbsMargins"}
            };
        }
        
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        HListLayout::HListLayout()
        {
            m_gridLayout.SetNumRows(1);
            m_gridLayout.SetCellOrder(GridLayout::CellOrder::k_colMajor);
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        HListLayout::HListLayout(const PropertyMap& in_properties)
        {
            m_gridLayout.SetNumRows(1);
            m_gridLayout.SetCellOrder(GridLayout::CellOrder::k_colMajor);
            
            SetNumCells(in_properties.GetPropertyOrDefault("NumCells", (s32)GetNumCells()));
            SetRelativeSpacing(in_properties.GetPropertyOrDefault("RelSpacing", GetRelativeSpacing()));
            SetAbsoluteSpacing(in_properties.GetPropertyOrDefault("AbsSpacing", GetAbsoluteSpacing()));
            
            Core::Vector4 relMargins(in_properties.GetPropertyOrDefault("RelMargins", GetRelativeMargins()));
            SetRelativeMargins(relMargins.x, relMargins.y, relMargins.z, relMargins.w);
            Core::Vector4 absMargins(in_properties.GetPropertyOrDefault("AbsMargins", GetAbsoluteMargins()));
            SetAbsoluteMargins(absMargins.x, absMargins.y, absMargins.z, absMargins.w);
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        std::vector<PropertyMap::PropertyDesc> HListLayout::GetPropertyDescs()
        {
            return k_propertyDescs;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        LayoutType HListLayout::GetType() const
        {
            return LayoutType::k_hList;
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        u32 HListLayout::GetNumCells() const
        {
            return m_gridLayout.GetNumCols();
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        Core::Vector4 HListLayout::GetRelativeMargins() const
        {
            return m_gridLayout.GetRelativeMargins();
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        Core::Vector4 HListLayout::GetAbsoluteMargins() const
        {
            return m_gridLayout.GetAbsoluteMargins();
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        f32 HListLayout::GetRelativeSpacing() const
        {
            return m_gridLayout.GetRelativeHSpacing();
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        f32 HListLayout::GetAbsoluteSpacing() const
        {
            return m_gridLayout.GetAbsoluteHSpacing();
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void HListLayout::SetNumCells(u32 in_numCells)
        {
            CS_ASSERT(in_numCells > 0, "Cannot create a list with 0 cells");
            m_gridLayout.SetNumCols(in_numCells);
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void HListLayout::SetRelativeMargins(f32 in_top, f32 in_right, f32 in_bottom, f32 in_left)
        {
            m_gridLayout.SetRelativeMargins(in_top, in_right, in_bottom, in_left);
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void HListLayout::SetAbsoluteMargins(f32 in_top, f32 in_right, f32 in_bottom, f32 in_left)
        {
            m_gridLayout.SetAbsoluteMargins(in_top, in_right, in_bottom, in_left);
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void HListLayout::SetRelativeSpacing(f32 in_spacing)
        {
            m_gridLayout.SetRelativeHSpacing(in_spacing);
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void HListLayout::SetAbsoluteSpacing(f32 in_spacing)
        {
            m_gridLayout.SetAbsoluteHSpacing(in_spacing);
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void HListLayout::BuildLayout()
        {
            m_gridLayout.BuildLayout();
        }
        //----------------------------------------------------------------------------------------
        /// The cell size if fixed and uniform so the index is not required
        //----------------------------------------------------------------------------------------
        Core::Vector2 HListLayout::GetSizeForIndex(u32 in_index) const
        {
            return m_gridLayout.GetSizeForIndex(in_index);
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        Core::Vector2 HListLayout::GetPositionForIndex(u32 in_index) const
        {
            return m_gridLayout.GetPositionForIndex(in_index);
        }
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        void HListLayout::SetWidget(Widget* in_widget)
        {
            m_gridLayout.SetWidget(in_widget);
        }
    }
}