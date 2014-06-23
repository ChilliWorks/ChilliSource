/*
 *  StringUtils.cpp
 *  ToolUtils
 *
 *  Created by Ian Copland on 16/03/2012.
 *  Copyright 2012 Tag Games Ltd. All rights reserved.
 *
 */

#include "ToolUtils/StringUtils.h"
#include <algorithm>

namespace ToolUtils
{
	//============================================
	/// String Utils
	//============================================
	namespace StringUtils
	{
		//--------------------------------------------------------
		/// Standardise Path
		//--------------------------------------------------------
		std::string StandardisePath(const std::string& instrPath)
		{
			std::string strPath = instrPath;
			std::replace( strPath.begin(), strPath.end(), '\\', '/' );

            if(instrPath.find('.') == std::string::npos)
            {
                if( strPath.length() != 0 && strPath[strPath.length() - 1] != '/' )
                    strPath += '/';
            }
			
			return strPath;
		}
		//--------------------------------------------------------
		/// To Lower Case
		//--------------------------------------------------------
		std::string ToLowerCase(const std::string& instrString)
		{
			std::string strOutput = instrString;
			std::transform(strOutput.begin(), strOutput.end(), strOutput.begin(), tolower);
			return strOutput;
		}
		//--------------------------------------------------------
		/// To Upper Case
		//--------------------------------------------------------
		std::string ToUpperCase(const std::string& instrString)
		{
			std::string strOutput = instrString;
			std::transform(strOutput.begin(), strOutput.end(), strOutput.begin(), toupper);
			return strOutput;
		}
		//--------------------------------------------------------
		/// Starts With
		//--------------------------------------------------------
		bool StartsWith(const std::string& instrString, const std::string& instrPattern)
		{
			size_t thisLen = instrString.length();
			size_t patternLen = instrPattern.length();
			if (thisLen < patternLen || patternLen == 0)
				return false;
			
			std::string startOfThis = instrString.substr(0, patternLen);
            std::string strPattern = instrPattern;
			
			return (startOfThis == strPattern);
		}
		//--------------------------------------------------------
		/// Ends With
		//--------------------------------------------------------
		bool EndsWith(const std::string& instrString, const std::string& instrPattern)
		{
			size_t thisLen = instrString.length();
			size_t patternLen = instrPattern.length();
			if (thisLen < patternLen || patternLen == 0)
				return false;
			
			std::string endOfThis = instrString.substr(thisLen - patternLen, patternLen);
            std::string strPattern = instrPattern;

			return (endOfThis == strPattern);
		}
		//--------------------------------------------------------
		/// String Cast (templated - bool specialisation)
		//--------------------------------------------------------
		template<> std::string StringCast<>(bool inValue)
		{
			if (inValue == true)
			{
				return "true";
			}
			else
			{
				return "false";
			}
		}
	}
}