//
//  ParamDictionary.cpp
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

#include <ChilliSource/Core/Container/ParamDictionary.h>

namespace ChilliSource
{
	namespace Core
    {
        //-----------------------------------------------------------------
        //-----------------------------------------------------------------
        ParamDictionary::ParamDictionary(const std::initializer_list<std::pair<std::string, std::string>>& in_initList)
        {
            for(const auto& pair : in_initList)
            {
                insert(pair);
            }
        }
        //-----------------------------------------------------------------
        //-----------------------------------------------------------------
        void ParamDictionary::SetValue(const std::string& in_key, const std::string& in_value)
        {
            insert(std::make_pair(in_key, in_value));
        }
        //-----------------------------------------------------------------
        //-----------------------------------------------------------------
        bool ParamDictionary::HasKey(const std::string& in_key) const
        {
            return find(in_key) != end();
        }
        //-----------------------------------------------------------------
        //-----------------------------------------------------------------
        bool ParamDictionary::TryGetValue(const std::string& in_key, std::string& out_value) const
        {
            auto it = find(in_key);
            
            if(it != end())
            {
                out_value = it->second;
                return true;
            }
            
            return false;
        }
        //-----------------------------------------------------------------
        //-----------------------------------------------------------------
        const std::string& ParamDictionary::GetValue(const std::string& in_key) const
        {
            return GetValueOrDefault(in_key, "");
        }
        //-----------------------------------------------------------------
        //-----------------------------------------------------------------
        const std::string& ParamDictionary::GetValueOrDefault(const std::string& in_key, const std::string& in_default) const
        {
            auto it = find(in_key);
            
            if(it != end())
            {
                return it->second;
            }
            
            return in_default;
        }
	}
}
