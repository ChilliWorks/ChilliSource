//
//  XML.cpp
//  Chilli Source
//  Created by Ian Copland on 12/06/2014.
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
#include <ChilliSource/Core/XML/XML.h>

namespace ChilliSource
{
    namespace Core
    {
        //---------------------------------------------------------
        //---------------------------------------------------------
        XML::XML(std::unique_ptr<rapidxml::xml_document<>> in_document, std::unique_ptr<s8[]> in_data)
            : m_document(std::move(in_document)), m_data(std::move(in_data))
        {
        }
        //---------------------------------------------------------
        //---------------------------------------------------------
        rapidxml::xml_document<>* XML::GetDocument() const
        {
            return m_document.get();
        }
    }
}