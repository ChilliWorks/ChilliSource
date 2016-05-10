//
//  HListUILayoutDef.h
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

#ifndef _CHILLISOURCE_UI_LAYOUT_HLISTUILAYOUTDEF_H_
#define _CHILLISOURCE_UI_LAYOUT_HLISTUILAYOUTDEF_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Math/Vector2.h>
#include <ChilliSource/Core/Math/Vector4.h>
#include <ChilliSource/UI/Layout/HListUILayout.h>
#include <ChilliSource/UI/Layout/UILayoutDef.h>

namespace ChilliSource
{
    //------------------------------------------------------------------------------
    /// A definition of a HList UILayout which can be built from a json object. This is
    /// effectively an immutable container for the properties of a HList UILayout.
    ///
    /// A HList UILayout Def contains the following property in addition to those
    /// defined in the Layouy Def base class documentation:
    ///
    /// "NumCells": The number of cells.
    ///
    /// "RelMargins": The relative padding from the widget edge to the cells
    ///
    /// "AbsMargins": The absolute padding from the widget edge to the cells
    ///
    /// "RelSpacing": The relative spacing between cells
    ///
    /// "AbsSpacing": The absolute spacing between cells
    ///
    /// @author Ian Copland
    //------------------------------------------------------------------------------
    class HListUILayoutDef final : public UILayoutDef
    {
    public:
        CS_DECLARE_NAMEDTYPE(HListUILayoutDef);
        //------------------------------------------------------------------------------
        /// Constructor. Creates an empty hlist layout definition.
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------------
        HListUILayoutDef() = default;
        //------------------------------------------------------------------------------
        /// Constructor. Creates a standard hlist layout definition from json.
        ///
        /// @author Ian Copland
        ///
        /// @param The JSON object containing the drawable properties.
        //------------------------------------------------------------------------------
        HListUILayoutDef(const Json::Value& in_json);
        //------------------------------------------------------------------------------
        /// Constructor. Creates a hlist layout definition with the given properties.
        ///
        /// @author Ian Copland
        ///
        /// @param The cell order.
        /// @param The number of cells.
        /// @param The relative margins.
        /// @param The absolute margins.
        /// @param The relative spacing.
        /// @param The absolute spacing.
        //------------------------------------------------------------------------------
        HListUILayoutDef(u32 in_numCells, const Vector4& in_relMargins, const Vector4& in_absMargins, f32 in_relSpacing, f32 in_absSpacing);
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
        bool IsA(InterfaceIDType in_interfaceId) const override;
        //------------------------------------------------------------------------------
        /// @author Ian Copland
        ///
        /// @return The number of cells.
        //------------------------------------------------------------------------------
        u32 GetNumCells() const;
        //------------------------------------------------------------------------------
        /// @author Ian Copland
        ///
        /// @return The relative border size margins for each edge. These are relative
        /// to the overall layout size (i.e owning widget size). X is top, Y is right,
        /// Z is bottom and W is left.
        //------------------------------------------------------------------------------
        Vector4 GetRelativeMargins() const;
        //------------------------------------------------------------------------------
        /// @author Ian Copland
        ///
        /// @return The absolute border size margins for each edge. X is top, Y is
        /// right, Z is bottom and W is left.
        //------------------------------------------------------------------------------
        Vector4 GetAbsoluteMargins() const;
        //------------------------------------------------------------------------------
        /// @author Ian Copland
        ///
        /// @return The relative spacing between cells.
        //------------------------------------------------------------------------------
        f32 GetRelativeSpacing() const;
        //------------------------------------------------------------------------------
        /// @author Ian Copland
        ///
        /// @return The absolute spacing between cells.
        //------------------------------------------------------------------------------
        f32 GetAbsoluteSpacing() const;
        
    protected:
        //------------------------------------------------------------------------------
        /// Creates a new instance of a hlist layout as described by this definition.
        /// This should typically only be called by a layout component.
        ///
        /// @author Ian Copland
        ///
        /// @param The owning component.
        ///
        /// @return a new layout instance.
        //------------------------------------------------------------------------------
        UILayoutUPtr CreateLayout(LayoutUIComponent* in_owner) const override;
        
        u32 m_numCells = 1;
        Vector4 m_relativeMargins;
        Vector4 m_absoluteMargins;
        f32 m_relativeSpacing = 0.0f;
        f32 m_absoluteSpacing = 0.0f;
    };
}

#endif
