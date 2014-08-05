//
//  PropertyAccessor.h
//  Chilli Source
//  Created by Scott Downie on 05/08/2014.
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

#ifndef _CHILLISOURCE_UI_BASE_PROPERTYACCESSOR_H_
#define _CHILLISOURCE_UI_BASE_PROPERTYACCESSOR_H_

#include <ChilliSource/ChilliSource.h>

#include <functional>

namespace ChilliSource 
{
	namespace UI
	{
        //-----------------------------------------------------
        /// Interface that allows accessors with different
        /// signatures to be stored in the same container
        /// and then cast to the correct type
        ///
        /// @author S Downie
        //-----------------------------------------------------
        class IPropertyAccessor
        {
        public:
            //-----------------------------------------------------
            /// Virtual destructor
            ///
            /// @author S Downie
            //-----------------------------------------------------
            virtual ~IPropertyAccessor(){}
        };
        
        //-----------------------------------------------------
        /// Object that holds functions that act as setters
        /// and getters
        ///
        /// @author S Downie
        //-----------------------------------------------------
        template <typename TPropType> class PropertyAccessor : public IPropertyAccessor
        {
        public:
            
            //-----------------------------------------------------
            /// Constructor
            ///
            /// @author S Downie
            ///
            /// @param Setter function
            /// @param Getter function
            //-----------------------------------------------------
            PropertyAccessor(const std::function<void(TPropType)>& in_setter, const std::function<TPropType()>& in_getter)
            : m_setter(in_setter), m_getter(in_getter)
            {
                
            }
            //-----------------------------------------------------
            /// Returns value via the getter
            ///
            /// @author S Downie
            ///
            /// @return Value
            //-----------------------------------------------------
            TPropType Get() const
            {
                return m_getter();
            }
            //-----------------------------------------------------
            /// Sets value via the setter
            ///
            /// @author S Downie
            ///
            /// @param Value
            //-----------------------------------------------------
            void Set(const TPropType& in_value)
            {
                m_setter(in_value);
            }
            
        private:
            
            std::function<void(TPropType)> m_setter;
            std::function<TPropType()> m_getter;
        };
	}
}

#endif
