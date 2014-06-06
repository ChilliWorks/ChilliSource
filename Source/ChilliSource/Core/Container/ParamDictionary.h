//
//  ParamDictionary.h
//  Chilli Source
//  Created by Stuart McGaw on 15/02/2011.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2011 Tag Games Limited
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

#ifndef _CHILLISOURCE_CORE_CONTAINER_PARAMDICTIONARY_H_
#define _CHILLISOURCE_CORE_CONTAINER_PARAMDICTIONARY_H_

#include <ChilliSource/ChilliSource.h>

#include <unordered_map>

namespace ChilliSource
{
	namespace Core 
    {
        //-----------------------------------------------------------------
        /// Dictionary wrapper that maps string keys to string values
        /// and provides convenience setters and accessors
        ///
        /// @author S McGaw
        //-----------------------------------------------------------------
		class ParamDictionary final : public std::unordered_map<std::string, std::string>
        {
		public:
            
            //-----------------------------------------------------------------
            /// Constructor default
            ///
            /// @author S Downie
            //-----------------------------------------------------------------
            ParamDictionary() = default;
            //-----------------------------------------------------------------
            /// Constructor from initialiser list of key/value pairs
            ///
            /// @author S Downie
            ///
            /// @param Initialiser list of key/value pairs
            //-----------------------------------------------------------------
            ParamDictionary(const std::initializer_list<std::pair<std::string, std::string>>& in_initList);
            //-----------------------------------------------------------------
            /// @author S McGaw
            ///
            /// @param Key
            /// @param Value
            //-----------------------------------------------------------------
            void SetValue(const std::string& in_key, const std::string& in_value);
            //-----------------------------------------------------------------
            /// @author S McGaw
            ///
            /// @param Key
            ///
            /// @return Whether key exists in dictionary
            //-----------------------------------------------------------------
            bool HasKey(const std::string& in_key) const;
            //-----------------------------------------------------------------
            /// @author S McGaw
            ///
            /// @param Key
            /// @param [Out] Storage for value if the key exists
            ///
            /// @return Whether key exists and value has been set
            //-----------------------------------------------------------------
            bool TryGetValue(const std::string& in_key, std::string& out_value) const;
            //-----------------------------------------------------------------
            /// @author S McGaw
            ///
            /// @param Key
            ///
            /// @return Value at key or empty
            //-----------------------------------------------------------------
            const std::string& GetValue(const std::string& in_key) const;
            //-----------------------------------------------------------------
            /// @author S Downie
            ///
            /// @param Key
            /// @param Default value to return if key does not exist
            ///
            /// @return Value at key or default
            //-----------------------------------------------------------------
            const std::string& GetValueOrDefault(const std::string& in_key, const std::string& in_default) const;
		};
	}
}

#endif