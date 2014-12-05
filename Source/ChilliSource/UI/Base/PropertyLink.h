//
//  PropertyLink.h
//  Chilli Source
//  Created by Ian Copland on 24/11/2014.
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

#ifndef _CHILLISOURCE_UI_BASE_PROPERTYLINK_H_
#define _CHILLISOURCE_UI_BASE_PROPERTYLINK_H_

#include <ChilliSource/ChilliSource.h>

namespace ChilliSource
{
    namespace UI
    {
        //--------------------------------------------------------------------------
        /// A container for a description of a property which links to another. This
        /// provides a name for the link, the name of the linked propery owner and
        /// the linked property name. This is immutable after construction.
        ///
        /// @param Ian Copland
        //--------------------------------------------------------------------------
        class PropertyLink final
        {
        public:
            //------------------------------------------------------------------
            /// Constructor. Creates an empty property link.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------
            PropertyLink() = default;
            //------------------------------------------------------------------
            /// Constructor.
            ///
            /// @author Ian Copland
            ///
            /// @param The name of the property link.
            /// @param The name of the linked property owner.
            /// @param The name of the linked property.
            //------------------------------------------------------------------
            PropertyLink(const std::string& in_linkName, const std::string& in_linkedOwner, const std::string& in_linkedProperty);
            //------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The name of the property link.
            //------------------------------------------------------------------
            const std::string& GetLinkName() const;
            //------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The name of the linked property owner
            //------------------------------------------------------------------
            const std::string& GetLinkedOwner() const;
            //------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The name of the linked property.
            //------------------------------------------------------------------
            const std::string& GetLinkedProperty() const;
        private:
            std::string m_linkName = "";
            std::string m_linkedOwner = "";
            std::string m_linkedProperty = "";
        };
    }
}

#endif
