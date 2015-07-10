//
//  MarkupDef.cpp
//  ChilliSource
//
//  Created by Nicolas Tanda on 08/07/2015.
//  Copyright (c) 2015 Chilli Source. All rights reserved.
//

#include <ChilliSource/Core/String/MarkupDef.h>

namespace ChilliSource
{
    namespace Core
    {
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void MarkupDef::AddKeyword(const std::string& in_name, bool in_nestable)
        {
            bool hasKeyword = HasKeyword(in_name);
            CS_ASSERT(hasKeyword == false, "The markup definition already has \"" + in_name + "\" as a keyword");
            
            if(hasKeyword == false)
            {
                Keyword keyword;
                keyword.m_nestable = in_nestable;
                m_keywords[in_name] = keyword;
            }
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
            std::unordered_map<std::string, Keyword>::const_iterator itKeyword = m_keywords.find(in_name);
            if(itKeyword != m_keywords.end())
            {
                return itKeyword->second.m_nestable;
            }
            return false;
        }
    }
}