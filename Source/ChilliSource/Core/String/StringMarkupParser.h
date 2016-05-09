//
//  StringMarkupParser.h
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

#ifndef _CHILLISOURCE_CORE_STRING_STRINGMARKUPPARSER_H_
#define _CHILLISOURCE_CORE_STRING_STRINGMARKUPPARSER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/String/MarkupDef.h>

#include <functional>

namespace ChilliSource
{
    //------------------------------------------------------------------------------
    /// Defines a markup parser
    ///
    /// @author N Tanda
    //------------------------------------------------------------------------------
    class StringMarkupParser final
    {
    public:
        //------------------------------------------------------------------------------
        /// Callback for when a markup has been found in the string.
        /// A markup is composed by a keyword and a value in the form [keyword= value]
        ///
        /// @author N Tanda
        ///
        /// @param in_keywordName - The keyword name found in the string
        /// @param in_keywordValue - The value of the keyword
        /// @param in_indexInString - The index of the string within which the output will
        ///        be placed at
        ///
        /// @return The value replacing the markup in the string
        //------------------------------------------------------------------------------
        using MarkupFoundDelegate = std::function<std::string(const std::string& in_keywordName, const std::string& in_keywordValue, u32 in_indexInString)>;
        
        //------------------------------------------------------------------------------
        /// Constructor
        ///
        /// @author N Tanda
        //------------------------------------------------------------------------------
        StringMarkupParser() = default;
        //------------------------------------------------------------------------------
        /// Constructor
        ///
        /// @author N Tanda
        ///
        /// @param in_markupDef - The markup definition
        //------------------------------------------------------------------------------
        StringMarkupParser(const MarkupDef& in_markupDef);
        //------------------------------------------------------------------------------
        /// Parses the given string using the callback function passed to decide what to
        /// do with the markups found.
        ///
        /// @author N Tanda
        ///
        /// @param in_string - The string to parse
        /// @param in_callback - The delegate to callback when a markup has been found
        ///
        /// @return The parsed string
        //------------------------------------------------------------------------------
        std::string Parse(const std::string& in_string, const MarkupFoundDelegate& in_callback);
        
    private:
        //------------------------------------------------------------------------------
        /// Parses the given string using the callback function passed to decide what to
        /// do with the markups found.
        ///
        /// @author N Tanda
        ///
        /// @param in_indexInString - The index of the string within which the output
        ///        will be placed at
        /// @param out_iterator - [OUT] The iterator going through the text
        /// @param out_string - [OUT] The text output
        /// @param in_callback - The delegate to callback when a markup has been found
        ///
        /// @return The keyword name found in the string
        //------------------------------------------------------------------------------
        std::string ParseRecursive(u32 in_indexInString, std::string::const_iterator& out_iterator, std::string& out_string, const MarkupFoundDelegate& in_callback);
        
    private:
        MarkupDef m_markupDef;
    };
}

#endif
