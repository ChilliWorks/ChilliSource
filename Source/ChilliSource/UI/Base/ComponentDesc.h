//
//  ComponentDesc.h
//  Chilli Source
//  Created by Ian Copland on 19/11/2014.
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

#ifndef _CHILLISOURCE_UI_BASE_COMPONENTDESC_H_
#define _CHILLISOURCE_UI_BASE_COMPONENTDESC_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Container/Property/PropertyMap.h>

namespace ChilliSource
{
    namespace UI
    {
        //--------------------------------------------------------------------------
        /// A container for a description of a single component, comprising of a
        /// type and a map of properties for the given component type. Typically
        /// this is only used in the creation of a WidgetDef. In most cases a user
        /// will not need to deal with this directly, creating a WidgetDef from
        /// file where this will be handled by the resource provider. This is
        /// immutable after construction.
        ///
        /// @param Ian Copland
        //--------------------------------------------------------------------------
        class ComponentDesc final
        {
        public:
            //------------------------------------------------------------------
            /// Constructor. Creates an empty description.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------
            ComponentDesc() = default;
            //------------------------------------------------------------------
            /// Constructor.
            ///
            /// @author Ian Copland
            ///
            /// @param The name of the component type this desc can create.
            /// @param The name of the component instance.
            /// @param The map of properties the component should be created
            /// with.
            //------------------------------------------------------------------
            ComponentDesc(const std::string& in_type, const std::string& in_name, const Core::PropertyMap& in_propertyMap);
            //------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The name of the component type this desc can create.
            //------------------------------------------------------------------
            const std::string& GetType() const;
            //------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The name of the component instance.
            //------------------------------------------------------------------
            const std::string& GetName() const;
            //------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The map of properties the component should be created
            /// with.
            //------------------------------------------------------------------
            const Core::PropertyMap& GetProperties() const;
        private:
            std::string m_type;
            std::string m_name;
            Core::PropertyMap m_propertyMap;
        };
    }
}

#endif
