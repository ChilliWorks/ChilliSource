//
//  StringUtils.cpp
//  ChilliSource
//  Created by Ian Copland on 16/03/2012.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2012 Tag Games Limited
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

#include "StringUtils.h"

#include <algorithm>

namespace StringUtils
{
	//--------------------------------------------------------
	//--------------------------------------------------------
	std::string StandardisePath(const std::string& in_path)
	{
		std::string strPath = in_path;
		std::replace( strPath.begin(), strPath.end(), '\\', '/' );
		strPath = ReplaceAll(strPath, "//", "/");

        if(strPath.find('.') == std::string::npos)
        {
            if( strPath.length() != 0 && strPath[strPath.length() - 1] != '/' )
                strPath += '/';
        }
		
		return strPath;
	}
	//--------------------------------------------------------
	//--------------------------------------------------------
	std::string ToLowerCase(const std::string& in_string)
	{
		std::string strOutput = in_string;
		std::transform(strOutput.begin(), strOutput.end(), strOutput.begin(), tolower);
		return strOutput;
	}
	//--------------------------------------------------------
	//--------------------------------------------------------
	std::string ToUpperCase(const std::string& in_string)
	{
		std::string strOutput = in_string;
		std::transform(strOutput.begin(), strOutput.end(), strOutput.begin(), toupper);
		return strOutput;
	}
	//--------------------------------------------------------
	//--------------------------------------------------------
	bool StartsWith(const std::string& in_string, const std::string& in_pattern)
	{
		size_t thisLen = in_string.length();
		size_t patternLen = in_pattern.length();
		if (thisLen < patternLen || patternLen == 0)
			return false;
		
		std::string startOfThis = in_string.substr(0, patternLen);
        std::string strPattern = in_pattern;
		
		return (startOfThis == strPattern);
	}
	//--------------------------------------------------------
	//--------------------------------------------------------
	bool EndsWith(const std::string& in_string, const std::string& in_pattern)
	{
		size_t thisLen = in_string.length();
		size_t patternLen = in_pattern.length();
		if (thisLen < patternLen || patternLen == 0)
			return false;
		
		std::string endOfThis = in_string.substr(thisLen - patternLen, patternLen);
        std::string strPattern = in_pattern;

		return (endOfThis == strPattern);
	}
	//--------------------------------------------------------
	//--------------------------------------------------------
    std::string ReplaceAll(const std::string& in_source, const std::string& in_replaceWhat, const std::string& in_replaceWithWhat)
    {
    	std::string result = in_source;
        while(1)
        {
            std::string::size_type pos = result.find(in_replaceWhat);
            if (pos == std::string::npos) break;
            result.replace(pos,in_replaceWhat.size(),in_replaceWithWhat);
        }
        return result;
    }
	//--------------------------------------------------------
	//--------------------------------------------------------
	template<> std::string StringCast<>(bool in_value)
	{
		if (in_value == true)
		{
			return "true";
		}
		else
		{
			return "false";
		}
	}
}
