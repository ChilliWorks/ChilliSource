//
//  LayoutDef.h
//  Chilli Source
//  Created by Ian Copland on 08/05/2015.
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

#ifndef _CHILLISOURCE_UI_LAYOUT_LAYOUTDEF_H_
#define _CHILLISOURCE_UI_LAYOUT_LAYOUTDEF_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/QueryableInterface.h>
#include <ChilliSource/Core/File/StorageLocation.h>

#include <json/forwards.h>

namespace ChilliSource
{
    namespace UI
    {
        //------------------------------------------------------------------------------
        /// A definition of a layout which can be built from a json object. This is
        /// effectively an immutable container for the properties of a specific type of
        /// layout.
        ///
        /// Layout Defs can be either created using the concrete def's constructor or
        /// using the Create() method. This creates the def from json which describes the
        /// type and properties of the def.
        ///
        /// All layouts defs contain the following property. Specific def types can also
        /// contain further properties.
        ///
        /// "Type": Describes the type of layout def.
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------------
        class LayoutDef : public Core::QueryableInterface
        {
        public:
            CS_DECLARE_NAMEDTYPE(LayoutDef);
            //------------------------------------------------------------------------------
            /// Creates a new layout def from json which describes the type of def and its
            /// properties.
            ///
            /// @author Ian Copland
            ///
            /// @param The JSON object containing the layout properties.
            //------------------------------------------------------------------------------
            static LayoutDefCUPtr Create(const Json::Value& in_json);
            //------------------------------------------------------------------------------
            /// Destructor
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            virtual ~LayoutDef() {};
        protected:
            friend class LayoutComponent;
            //------------------------------------------------------------------------------
            /// Creates a new instance of the layout described by this definition. This
            /// should typically only be called by a layout component.
            ///
            /// @author Ian Copland
            ///
            /// @param The owning component.
            ///
            /// @return a new layout instance.
            //------------------------------------------------------------------------------
            virtual LayoutUPtr CreateLayout(LayoutComponent* in_owner) const = 0;
        };
    }
}

#endif
