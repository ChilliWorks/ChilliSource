//  The MIT License (MIT)
//
//  Copyright (c) 2016 Tag Games Limited
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

#ifndef _CHILLISOURCE_INPUT_TEXTENTRY_TEXTENTRYCAPITALISATION_H_
#define _CHILLISOURCE_INPUT_TEXTENTRY_TEXTENTRYCAPITALISATION_H_

#include <ChilliSource/ChilliSource.h>

#include <functional>

namespace ChilliSource
{
    /// An enum describing the various methods of keyboard
    /// capitalisation that can be used.
    ///
    ///  - None: will not capitalise anything.
    ///  - Words: will capitalise the first letter
    ///	   of each word.
    ///  - Sentences: will capitalise the first letter
    ///	   of each sentence.
    ///  - All: will capitalise every letter.
    ///
    enum class TextEntryCapitalisation
    {
        k_none,
        k_words,
        k_sentences,
        k_all
    };

    /// Parses keyboard capitalisation method from a string.
    ///
    /// @param string
    ///     The string.
    ///
    /// @return The capitalisation method.
    ///
    TextEntryCapitalisation ParseCapitalisationFormat(const std::string& string) noexcept;
}

#endif