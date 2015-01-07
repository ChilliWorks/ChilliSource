//
//  GridLayoutDef.cpp
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

#include <ChilliSource/UI/Layout/GridLayoutDef.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/String/StringParser.h>
#include <ChilliSource/UI/Layout/GridLayout.h>

#include <json/json.h>

namespace ChilliSource
{
    namespace UI
    {
        namespace
        {
            //------------------------------------------------------------------------------
            /// Parses the given cell order string. If the string is not a valid cell order
            /// the app is considered to be in an irrecoverable state and will terminate.
            ///
            /// @author Ian Copland
            ///
            /// @param The cell order in string form.
            ///
            /// @return The parsed cell order.
            //------------------------------------------------------------------------------
            GridLayout::CellOrder ParseCellOrder(const std::string& in_cellOrderString)
            {
                const char k_columnMajor[] = "colmajor";
                const char k_rowMajor[] = "rowmajor";

                std::string cellOrderString = in_cellOrderString;
                Core::StringUtils::ToLowerCase(cellOrderString);

                if(cellOrderString == k_columnMajor)
                {
                    return GridLayout::CellOrder::k_colMajor;
                }
                else if(cellOrderString == k_rowMajor)
                {
                    return GridLayout::CellOrder::k_rowMajor;
                }
                
                CS_LOG_FATAL("Unknown GridLayout cell order: " + in_cellOrderString);
                return GridLayout::CellOrder::k_colMajor;
            }
        }
        
        CS_DEFINE_NAMEDTYPE(GridLayoutDef);
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        GridLayoutDef::GridLayoutDef(const Json::Value& in_json)
        {
            const char k_typeKey[] = "Type";
            const char k_cellOrderKey[] = "CellOrder";
            const char k_numRowsKey[] = "NumRows";
            const char k_numColumnsKey[] = "NumCols";
            const char k_relativeMarginsKey[] = "RelMargins";
            const char k_absoluteMarginsKey[] = "AbsMargins";
            const char k_relativeHSpacingKey[] = "RelHSpacing";
            const char k_absoluteHSpacingKey[] = "AbsHSpacing";
            const char k_relativeVSpacingKey[] = "RelVSpacing";
            const char k_absoluteVSpacingKey[] = "AbsVSpacing";

            for(auto it = in_json.begin(); it != in_json.end(); ++it)
            {
                CS_ASSERT((*it).isString() == true, "All properties in a Layout Def must be a string: " + std::string(it.memberName()));

                std::string key = it.memberName();
                std::string value = (*it).asString();

                if (key == k_relativeMarginsKey)
                {
                    m_relativeMargins = Core::ParseVector4(value);
                }
                else if (key == k_absoluteMarginsKey)
                {
                    m_absoluteMargins = Core::ParseVector4(value);
                }
                else if (key == k_numRowsKey)
                {
                    m_numRows = Core::ParseU32(value);
                }
                else if (key == k_numColumnsKey)
                {
                    m_numCols = Core::ParseU32(value);
                }
                else if (key == k_relativeHSpacingKey)
                {
                    m_relativeHSpacing = Core::ParseF32(value);
                }
                else if (key == k_absoluteHSpacingKey)
                {
                    m_absoluteHSpacing = Core::ParseF32(value);
                }
                else if (key == k_relativeVSpacingKey)
                {
                    m_relativeVSpacing = Core::ParseF32(value);
                }
                else if (key == k_absoluteVSpacingKey)
                {
                    m_absoluteVSpacing = Core::ParseF32(value);
                }
                else if (key == k_cellOrderKey)
                {
                    m_cellOrder = ParseCellOrder(value);
                }
                else if (key == k_typeKey)
                {
                    //ignore
                }
                else
                {
                    CS_LOG_FATAL("Invalid property found in a Grid layout description: " + key);
                }
            }
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        GridLayoutDef::GridLayoutDef(GridLayout::CellOrder in_cellOrder, u32 in_numRows, u32 in_numCols, const Core::Vector4& in_relMargins, const Core::Vector4& in_absMargins,
                      f32 in_relHorizSpacing, f32 in_absHorizSpacing, f32 in_relVertSpacing, f32 in_absVertSpacing)
            : m_cellOrder(in_cellOrder), m_numRows(in_numRows), m_numCols(in_numCols), m_relativeMargins(in_relMargins), m_absoluteMargins(in_absMargins), m_relativeHSpacing(in_relHorizSpacing),
              m_absoluteHSpacing(in_absHorizSpacing), m_relativeVSpacing(in_relVertSpacing), m_absoluteVSpacing(in_absVertSpacing)
        {
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        bool GridLayoutDef::IsA(Core::InterfaceIDType in_interfaceId) const
        {
            return (LayoutDef::InterfaceID == in_interfaceId || GridLayoutDef::InterfaceID == in_interfaceId);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        u32 GridLayoutDef::GetNumRows() const
        {
            return m_numRows;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        u32 GridLayoutDef::GetNumCols() const
        {
            return m_numCols;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        Core::Vector4 GridLayoutDef::GetRelativeMargins() const
        {
            return m_relativeMargins;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        Core::Vector4 GridLayoutDef::GetAbsoluteMargins() const
        {
            return m_absoluteMargins;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        f32 GridLayoutDef::GetRelativeHSpacing() const
        {
            return m_relativeHSpacing;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        f32 GridLayoutDef::GetAbsoluteHSpacing() const
        {
            return m_absoluteHSpacing;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        f32 GridLayoutDef::GetRelativeVSpacing() const
        {
            return m_relativeVSpacing;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        f32 GridLayoutDef::GetAbsoluteVSpacing() const
        {
            return m_absoluteVSpacing;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        GridLayout::CellOrder GridLayoutDef::GetCellOrder() const
        {
            return m_cellOrder;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        LayoutUPtr GridLayoutDef::CreateLayout(LayoutComponent* in_owner) const
        {
            return LayoutUPtr(new GridLayout(in_owner, m_cellOrder, m_numRows, m_numCols, m_relativeMargins, m_absoluteMargins, m_relativeHSpacing, m_absoluteHSpacing, m_relativeVSpacing, m_absoluteVSpacing));
        }
    }
}
