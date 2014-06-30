//
//  ParamDictionarySerialiser.h
//  Chilli Source
//  Created by Scott Downie on 15/05/2014.
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

#ifndef _CHILLISOURCE_CORE_CONTAINER_PARAMDICTIONARYSERIALISER_H_
#define _CHILLISOURCE_CORE_CONTAINER_PARAMDICTIONARYSERIALISER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/XML/XMLUtils.h>

namespace ChilliSource
{
	namespace Core 
    {
        //-----------------------------------------------------------------
        /// Methods for serialising param dictionaries to and from a
        /// variety of formats
        ///
        /// @author S Downie
        //-----------------------------------------------------------------
		namespace ParamDictionarySerialiser
        {
            //-----------------------------------------------------------------
            /// Convert the param dictionary to XML and store it in the given
            /// XML element.
            ///
            /// Format: <Params><String key = "key" value = "value"/></Params>
            ///
            /// @author A Glass
            ///
            /// @param Param dictionary to serialise
            /// @param [Out] XML element
            //-----------------------------------------------------------------
            void ToXml(const ParamDictionary& in_dict, XML::Node* out_element);
            //-----------------------------------------------------------------
            /// Convert the XML to a param dictionary of key/values
            ///
            /// Format: <Params><String key = "key" value = "value"/></Params>
            ///
            /// @author A Glass
            ///
            /// @param XML element
            ///
            /// @return Param dictionary
            //-----------------------------------------------------------------
            ParamDictionary FromXml(const XML::Node* in_element);
            //-----------------------------------------------------------------
            /// Convert the param dictionary to a string config representation
            ///
            /// Format: "key1 = value1; key2 = value2;"
            ///
            /// @author S McGaw
            ///
            /// @param Param dictionary
            ///
            /// @return String
            //-----------------------------------------------------------------
            std::string ToString(const ParamDictionary& in_dict);
            //-----------------------------------------------------------------
            /// Convert the given string to a param dictionary of key/values
            ///
            /// Format: "key1 = value1; key2 = value2;"
            ///
            /// @author S McGaw
            ///
            /// @param String
            ///
            /// @return Param dictionary
            //-----------------------------------------------------------------
            ParamDictionary FromString(const std::string& in_string);
		};
	}
}

#endif