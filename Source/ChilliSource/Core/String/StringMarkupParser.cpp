//
//  StringMarkupParser.cpp
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

#include <ChilliSource/Core/String/StringMarkupParser.h>

#include <ChilliSource/Core/String/UTF8StringUtils.h>

namespace ChilliSource
{
    namespace
    {
        const char k_markupStart = '[';
        const char k_markupSeparator = '=';
        const char k_markupEnd = ']';
    }
    
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    StringMarkupParser::StringMarkupParser(const MarkupDef& in_markupDef)
    :m_markupDef(in_markupDef)
    {
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    std::string StringMarkupParser::Parse(const std::string& in_string, const MarkupFoundDelegate& in_callback)
    {
        std::string out_string;
        u32 index = 0;
        
        auto it = in_string.begin();
        while(it < in_string.end())
        {
            auto character = UTF8StringUtils::Next(it);
            
            if(character != k_markupStart)
            {
                UTF8StringUtils::Append(character, out_string);
                if(character != ' ' && character != '\t' && character != '\n')
                {
                    ++index;
                }
            }
            else
            {
                // Found a mark up, check it
                ParseRecursive(index, it, out_string, in_callback);
            }
        }
        
        return out_string;
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    std::string StringMarkupParser::ParseRecursive(u32 in_indexInString, std::string::const_iterator& out_iterator, std::string& out_string, const MarkupFoundDelegate& in_callback)
    {
        // Found some mark-up. What type is it?
        std::string type;
        UTF8Char nextChar = '\0';
        
        while (nextChar != k_markupSeparator)
        {
            nextChar = UTF8StringUtils::Next(out_iterator);
            
            if(nextChar != k_markupSeparator && nextChar != ' ')
            {
                type += nextChar;
            }
        }
        
        // Variable type has been located
        std::string varName;
        
        // There may be some whitespace that we need to ignore
        nextChar = UTF8StringUtils::Next(out_iterator);
        if(nextChar != ' ')
        {
            varName += nextChar;
        }
        
        // Find the closing bracket
        while (nextChar != k_markupEnd)
        {
            nextChar = UTF8StringUtils::Next(out_iterator);
            
            if(nextChar != k_markupEnd && nextChar != k_markupStart && nextChar != ' ')
            {
                varName += nextChar;
            }
            
            // Nested variable
            if(nextChar == k_markupStart)
            {
                std::string variableName;
                
                const auto& subType = ParseRecursive(in_indexInString, out_iterator, variableName, in_callback);
                
                // Check if the keyword is nestable
                if(m_markupDef.HasKeyword(subType))
                {
                    CS_ASSERT(m_markupDef.IsKeywordNestable(subType) == true, "Keyword \"" + subType + "\" isn't a nestable type");
                }
                
                varName += variableName;
            }
        }
        
        if(m_markupDef.HasKeyword(type))
        {
            const auto& variable = in_callback(type, varName, in_indexInString);
            out_string.append(variable);
        }
        else
        {
            out_string.append(k_markupStart + type + k_markupSeparator + varName + k_markupEnd);
        }
        
        return type;
    }
}
