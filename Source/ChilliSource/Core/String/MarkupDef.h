//
//  MarkupDef.h
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

#ifndef _CHILLISOURCE_CORE_STRING_MARKUPDEF_H_
#define _CHILLISOURCE_CORE_STRING_MARKUPDEF_H_

#include <ChilliSource/ChilliSource.h>

#include <unordered_map>

namespace ChilliSource
{
    //------------------------------------------------------------------------------
    /// Defines the markups
    ///
    /// @author N Tanda
    //------------------------------------------------------------------------------
    class MarkupDef final
    {
    public:
        //------------------------------------------------------------------------------
        /// Adds a keyword to the markup
        ///
        /// @author N Tanda
        ///
        /// @param in_name - The unique name of the keyword.
        /// @param in_nestable - Whether the keyword can include other markups
        //------------------------------------------------------------------------------
        void AddKeyword(const std::string& in_name, bool in_nestable);
        //------------------------------------------------------------------------------
        /// @author N Tanda
        ///
        /// @param in_name - The unique name of the keyword.
        ///
        /// @return Whether the keyword is known
        //------------------------------------------------------------------------------
        bool HasKeyword(const std::string& in_name) const;
        //------------------------------------------------------------------------------
        /// @author N Tanda
        ///
        /// @param in_name - The unique name of the keyword.
        ///
        /// @return Whether the keyword can include other markups
        //------------------------------------------------------------------------------
        bool IsKeywordNestable(const std::string& in_name) const;
        
    private:
        std::unordered_map<std::string, bool> m_keywords;
    };
}

#endif
