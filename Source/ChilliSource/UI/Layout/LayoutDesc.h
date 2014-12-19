//
//  LayoutDesc.h
//  Chilli Source
//  Created by Ian Copland on 21/11/2014.
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

#ifndef _CHILLISOURCE_UI_LAYOUT_LAYOUTDESC_H_
#define _CHILLISOURCE_UI_LAYOUT_LAYOUTDESC_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Container/Property/PropertyType.h>
#include <ChilliSource/Core/Math/Vector4.h>
#include <ChilliSource/UI/Layout/GridLayout.h>
#include <ChilliSource/UI/Layout/LayoutType.h>

#include <json/forwards.h>

namespace ChilliSource
{
    namespace UI
    {
        //---------------------------------------------------------------------
        /// A description of a layout which can be built from a json object.
        /// This contains all possible properties that a layout can contain but
        /// can only be constructed using a valid set of properties. Invalid
        /// properties will remain as their default value.
        ///
        /// The properties contained in a layout desc are as follows:
        ///
        /// "Type": Describes the type of layout that should be used. The layout
        /// type determines which of the other properties can be used.
        ///
        /// "RelMargins": The relative padding from the widget edge to the cells
        /// in any layout.
        ///
        /// "AbsMargins": The absolute padding from the widget edge to the cells
        /// in any layout.
        ///
        /// "NumRows": The number of rows in a Grid layout.
        ///
        /// "NumCols": The number of columns in a Grid layout.
        ///
        /// "RelHSpacing": The relative horizontal spacing between cells in a
        /// Grid layout.
        ///
        /// "AbsHSpacing": The absolute horizonal spacing between cells in a
        /// Grid layout.
        ///
        /// "RelVSpacing": The relative vertical spacing between cells in a Grid
        /// layout.
        ///
        /// "AbsVSpacing": The absolute vertical spacing between cells in a Grid
        /// layout.
        ///
        /// "CellOrder": The order that cells will be placed into a Grid layout:
        /// row or column major order.
        ///
        /// "NumCells": The number of cells in a HList or VList layout.
        ///
        /// "RelSpacing": The relative spacing between cells in a HList or VList
        /// layout.
        ///
        /// "AbsSpacing": The absolute spacing between cells in a HList or VList
        /// layout.
        ///
        /// @author Ian Copland
        //---------------------------------------------------------------------
        class LayoutDesc final
        {
        public:
            //--------------------------------------------------------------
            /// Constructor. Creates an empty layout description.
            ///
            /// @author Ian Copland
            //--------------------------------------------------------------
            LayoutDesc() = default;
            //--------------------------------------------------------------
            /// Constructor. Creates the layout description from a JSON
            /// object. The values loaded from Json are described in the
            /// class documentation.
            ///
            /// @author Ian Copland
            ///
            /// @param The JSON object containing the layout properties.
            //--------------------------------------------------------------
            LayoutDesc(const Json::Value& in_json);
            //--------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The type of layout.
            //--------------------------------------------------------------
            LayoutType GetType() const;
            //--------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The relative padding from the widget edge to the cells
            /// in any layout.
            //--------------------------------------------------------------
            Core::Vector4 GetRelativeMargins() const;
            //--------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The absolute padding from the widget edge to the cells
            /// in any layout.
            //--------------------------------------------------------------
            Core::Vector4 GetAbsoluteMargins() const;
            //--------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The number of rows in a Grid layout.
            //--------------------------------------------------------------
            u32 GetNumRows() const;
            //--------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The number of columns in a Grid layout.
            //--------------------------------------------------------------
            u32 GetNumCols() const;
            //--------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The relative spacing between cells horizontally in a
            /// Grid layout.
            //--------------------------------------------------------------
            f32 GetRelativeHSpacing() const;
            //--------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The absolute spacing between cells horizontally in a
            /// Grid layout.
            //--------------------------------------------------------------
            f32 GetAbsoluteHSpacing() const;
            //--------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return the relative spacing between cells vertically in a
            /// Grid layout.
            //--------------------------------------------------------------
            f32 GetRelativeVSpacing() const;
            //--------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The absolute spacing between cells vertically in a
            /// Grid layout.
            //--------------------------------------------------------------
            f32 GetAbsoluteVSpacing() const;
            //--------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The order that cells will be placed into a Grid
            /// layout: row or column major order.
            //--------------------------------------------------------------
            GridLayout::CellOrder GetCellOrder() const;
            //--------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The relative spacing between cells in a HList or
            /// VList layout.
            //--------------------------------------------------------------
            f32 GetRelativeSpacing() const;
            //--------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The absolute spacing between cells in a HList or
            /// VList layout.
            //--------------------------------------------------------------
            f32 GetAbsoluteSpacing() const;
            //--------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The number of cells in a HList or VList layout.
            //--------------------------------------------------------------
            u32 GetNumCells() const;
        private:
            //--------------------------------------------------------------
            /// Initialises the layout description as a Grid layout from the
            /// given Json. This will error if any properties are provided
            /// that are not valid in a Grid layout.
            ///
            /// @author Ian Copland
            ///
            /// @param The JSON object containing the layout properties.
            //--------------------------------------------------------------
            void InitAsGrid(const Json::Value& in_json);
            //--------------------------------------------------------------
            /// Initialises the layout description as a List layout from the
            /// given Json. This will error if any properties are provided
            /// that are not valid in a List layout.
            ///
            /// @author Ian Copland
            ///
            /// @param The JSON object containing the layout properties.
            //--------------------------------------------------------------
            void InitAsList(const Json::Value& in_json);
            
            LayoutType m_type = LayoutType::k_grid;
            Core::Vector4 m_relativeMargins;
            Core::Vector4 m_absoluteMargins;
            u32 m_numRows = 1;
            u32 m_numCols = 1;
            f32 m_relativeHSpacing = 0.0f;
            f32 m_absoluteHSpacing = 0.0f;
            f32 m_relativeVSpacing = 0.0f;
            f32 m_absoluteVSpacing = 0.0f;
            GridLayout::CellOrder m_cellOrder = GridLayout::CellOrder::k_rowMajor;
            f32 m_relativeSpacing = 0.0f;
            f32 m_absoluteSpacing = 0.0f;
            u32 m_numCells = 1;
        };
    }
}

#endif
