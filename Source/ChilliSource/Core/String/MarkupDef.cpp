//
//  MarkupDef.cpp
//  Chilli Source
//  Created by Nicolas Tanda on 08/07/2015.
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

#include <ChilliSource/Core/String/MarkupDef.h>

namespace ChilliSource
{
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    void MarkupDef::AddKeyword(const std::string& in_name, bool in_nestable)
    {
        bool hasKeyword = HasKeyword(in_name);
        CS_ASSERT(hasKeyword == false, "The markup definition already has \"" + in_name + "\" as a keyword");
        
        m_keywords[in_name] = in_nestable;
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    bool MarkupDef::HasKeyword(const std::string& in_name) const
    {
        return m_keywords.find(in_name) != m_keywords.end();
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    bool MarkupDef::IsKeywordNestable(const std::string& in_name) const
    {
        std::unordered_map<std::string, bool>::const_iterator itKeyword = m_keywords.find(in_name);
        if(itKeyword != m_keywords.end())
        {
            return itKeyword->second;
        }
        return false;
    }
}