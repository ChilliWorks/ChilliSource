//
//  StringMarkupParser.cpp
//  ChilliSource
//
//  Created by Nicolas Tanda on 08/07/2015.
//  Copyright (c) 2015 Chilli Source. All rights reserved.
//

#include <ChilliSource/Core/String/StringMarkupParser.h>

#include <ChilliSource/Core/String/UTF8StringUtils.h>

namespace ChilliSource
{
    namespace Core
    {
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
                
                if(character != '[')
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
                    ParseRecursive(it, index, out_string, in_callback);
                }
            }
            
            return out_string;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        std::string StringMarkupParser::ParseRecursive(std::string::const_iterator& out_iterator, u32& out_index, std::string& out_string, const MarkupFoundDelegate& in_callback)
        {
            // Found some mark-up. What type is it?
            std::string type;
            UTF8Char nextChar = '\0';
            
            while (nextChar != '=')
            {
                nextChar = UTF8StringUtils::Next(out_iterator);
                
                if(nextChar != '=' && nextChar != ' ')
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
            while (nextChar != ']')
            {
                nextChar = UTF8StringUtils::Next(out_iterator);
                
                if(nextChar != ']' && nextChar != '[' && nextChar != ' ')
                {
                    varName += nextChar;
                }
                
                // Nested variable
                if(nextChar == '[')
                {
                    std::string variableName;
                    
                    const auto& subType = ParseRecursive(out_iterator, out_index, variableName, in_callback);
                    
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
                const auto& variable = in_callback(type, varName, out_index);
                out_string.append(variable);
            }
            else
            {
                out_string.append("[" + type + "= " + varName + "]");
            }
            
            return type;
        }
    }
}
