//
//  HListLayoutDef.cpp
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

#include <ChilliSource/UI/Layout/HListLayoutDef.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/String/StringParser.h>
#include <ChilliSource/UI/Layout/HListLayout.h>

#include <json/json.h>

namespace ChilliSource
{
    namespace UI
    {
        CS_DEFINE_NAMEDTYPE(HListLayoutDef);
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        HListLayoutDef::HListLayoutDef(const Json::Value& in_json)
        {
            const char k_typeKey[] = "Type";
            const char k_numCellsKey[] = "NumCells";
            const char k_relativeMarginsKey[] = "RelMargins";
            const char k_absoluteMarginsKey[] = "AbsMargins";
            const char k_relativeSpacingKey[] = "RelSpacing";
            const char k_absoluteSpacingKey[] = "AbsSpacing";
            
            for(auto it = in_json.begin(); it != in_json.end(); ++it)
            {
                CS_ASSERT((*it).isString() == true, "All properties in a Layout Description must be a string: " + std::string(it.memberName()));
                
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
                else if (key == k_numCellsKey)
                {
                    m_numCells = Core::ParseU32(value);
                }
                else if (key == k_relativeSpacingKey)
                {
                    m_relativeSpacing = Core::ParseF32(value);
                }
                else if (key == k_absoluteSpacingKey)
                {
                    m_absoluteSpacing = Core::ParseF32(value);
                }
                else if (key == k_typeKey)
                {
                    //ignore
                }
                else
                {
                    CS_LOG_FATAL("Invalid property found in a List layout description: " + key);
                }
            }
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        HListLayoutDef::HListLayoutDef(u32 in_numCells, const Core::Vector4& in_relMargins, const Core::Vector4& in_absMargins, f32 in_relSpacing, f32 in_absSpacing)
            : m_numCells(in_numCells), m_relativeMargins(in_relMargins), m_absoluteMargins(in_absMargins), m_relativeSpacing(in_relSpacing), m_absoluteSpacing(in_absSpacing)
        {
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        bool HListLayoutDef::IsA(Core::InterfaceIDType in_interfaceId) const
        {
            return (LayoutDef::InterfaceID == in_interfaceId || HListLayoutDef::InterfaceID == in_interfaceId);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        u32 HListLayoutDef::GetNumCells() const
        {
            return m_numCells;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        Core::Vector4 HListLayoutDef::GetRelativeMargins() const
        {
            return m_relativeMargins;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        Core::Vector4 HListLayoutDef::GetAbsoluteMargins() const
        {
            return m_absoluteMargins;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        f32 HListLayoutDef::GetRelativeSpacing() const
        {
            return m_relativeSpacing;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        f32 HListLayoutDef::GetAbsoluteSpacing() const
        {
            return m_absoluteSpacing;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        LayoutUPtr HListLayoutDef::CreateLayout(LayoutComponent* in_owner) const
        {
            return LayoutUPtr(new HListLayout(in_owner, m_numCells, m_relativeMargins, m_absoluteMargins, m_relativeSpacing, m_absoluteSpacing));
        }
    }
}
