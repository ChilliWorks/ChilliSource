/*
 *  StringUtils.h
 *  ToolUtils
 *
 *  Created by Ian Copland on 16/03/2012.
 *  Copyright 2012 Tag Games Ltd. All rights reserved.
 *
 */

#ifndef _TOOLUTILS_STRINGUTILS_H_
#define _TOOLUTILS_STRINGUTILS_H_
 
#include <string>
#include <sstream>

namespace ToolUtils
{
	//============================================
	/// String Utils
	///
	/// A "static class" used for processing strings
	//============================================
	namespace StringUtils
	{
		//--------------------------------------------------------
		/// Standardise Path
		///
		/// Ensures a filepath is in the correct format. This will
		/// ensure "/" is used, remove preceeding "/" and append
		/// "/" to the end if the path does not contain a "."
		///
		/// @param the original path.
		/// @output the standardised path.
		//--------------------------------------------------------
		std::string StandardisePath(const std::string& instrPath);
		//--------------------------------------------------------
		/// To Lower Case
		///
		/// Returns the equivalent string in all lower case.
		///
		/// @param the input string.
		/// @param the output string.
		//--------------------------------------------------------
		std::string ToLowerCase(const std::string& instrString);
		//--------------------------------------------------------
		/// To Upper Case
		///
		/// Returns the equivalent string in all upper case.
		///
		/// @param the input string.
		/// @param the output string.
		//--------------------------------------------------------
		std::string ToUpperCase(const std::string& instrString);
		//--------------------------------------------------------
		/// Starts With
		///
		/// Returns whether the string begins with the pattern passed in.
		///
		/// @param pattern The pattern to compare with.
		/// @param lowerCase If true, the start of the string will be lower cased before
		/// comparison, pattern should also be in lower case.
		/// @output The result.
		//--------------------------------------------------------
		bool StartsWith(const std::string& instrString, const std::string& instrPattern);
		//--------------------------------------------------------
		/// Ends With
		///
		/// Returns whether the string ends with the pattern passed in.
		///
		/// @param pattern The pattern to compare with.
		/// @param lowerCase If true, the end of the string will be lower cased before
		/// comparison, pattern should also be in lower case.
		/// @output The result.
		//--------------------------------------------------------
		bool EndsWith(const std::string& instrString, const std::string& instrPattern);
		//--------------------------------------------------------
		/// String Cast (templated)
		///
		/// Casts a value to a string.
		///
		/// @param the value.
		/// @output the string.
		//--------------------------------------------------------
		template<class T> std::string StringCast(T inValue)
		{
			std::stringstream stream;
			stream << inValue;
			return stream.str();
		}
		//--------------------------------------------------------
		/// String Cast (templated - bool specialisation)
		///
		/// Casts a bool to a string.
		///
		/// @param the bool.
		/// @output the string.
		//--------------------------------------------------------
		template<> std::string StringCast<>(bool inValue);
		//--------------------------------------------------------
		/// Parse String (templated)
		///
		/// Parses the content of a string to a base type.
		///
		/// @param The string.
		/// @output the value.
		//--------------------------------------------------------
		template<class T> T ParseString(const std::string& instrString)
		{
			std::istringstream strString(instrString);
			T output;
			strString >> output;
			return output;
		}
	}
}

#endif