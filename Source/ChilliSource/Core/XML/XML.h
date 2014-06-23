//
//  XML.h
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

#ifndef _CHILLISOURCE_CORE_XML_XML_H_
#define _CHILLISOURCE_CORE_XML_XML_H_

#include <ChilliSource/ChilliSource.h>

#include <rapidxml/rapidxml.hpp>

namespace ChilliSource
{
    namespace Core
    {
        //----------------------------------------------------------------
        /// A container class for an xml document. Rapid XML requires that
        /// parsed data have a greater lifetime than the document, so this
        /// holds on to and cleans up both.
        ///
        /// @author Ian Copland
        //----------------------------------------------------------------
        class XML final
        {
        public:
            CS_DECLARE_NOCOPY(XML);
            //---------------------------------------------------------
            /// Convenience typedefs for all of the main rapid xml
            /// types.
            ///
            /// @author Ian Copland
            //---------------------------------------------------------
            using Base = rapidxml::xml_base<>;
            using Node = rapidxml::xml_node<>;
            using Attribute = rapidxml::xml_attribute<>;
            using Document = rapidxml::xml_document<>;
			//---------------------------------------------------------
			/// Constructor.
			///
			/// @author Ian Copland
			///
			/// @param The rapid xml document.
			/// @param The parsed xml data.
			//---------------------------------------------------------
			XML(std::unique_ptr<Document> in_document, std::unique_ptr<s8[]> in_data);
            //---------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The xml document pointer.
            //---------------------------------------------------------
            Document* GetDocument() const;
        private:
            std::unique_ptr<Document> m_document;
            std::unique_ptr<s8[]> m_data;
        };
    }
}
#endif
