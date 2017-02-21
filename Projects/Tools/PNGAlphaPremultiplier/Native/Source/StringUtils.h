//
//  StringUtils.h
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

#ifndef _PNGALPHAPREMULTIPLIER_STRINGUTILS_H_
#define _PNGALPHAPREMULTIPLIER_STRINGUTILS_H_
 
#include <string>
#include <sstream>

//---------------------------------------------------------------
/// A collection of convenience functions for working with strings.
///
/// @author Ian Copland
//---------------------------------------------------------------
namespace StringUtils
{
	//--------------------------------------------------------
	/// Ensures a filepath is in the correct format. This will
	/// ensure "/" is used, remove preceeding "/" and append
	/// "/" to the end if the path does not contain a "."
	///
	/// @author Ian Copland
	///
	/// @param the original path.
	///
	/// @return the standardised path.
	//--------------------------------------------------------
	std::string StandardisePath(const std::string& in_path);
	//--------------------------------------------------------
	/// Returns the equivalent string in all lower case.
	///
	/// @author Ian Copland
	///
	/// @param the input string.
	///
	/// @return the output string.
	//--------------------------------------------------------
	std::string ToLowerCase(const std::string& in_string);
	//--------------------------------------------------------
	/// Returns the equivalent string in all upper case.
	///
	/// @author Ian Copland
	///
	/// @param the input string.
	///
	/// @param the output string.
	//--------------------------------------------------------
	std::string ToUpperCase(const std::string& in_string);
	//--------------------------------------------------------
	/// Returns whether the string begins with the pattern 
	/// passed in.
	///
	/// @author Ian Copland
	///
	/// @param The input string.
	/// @param The pattern to compare with.
	///
	/// @return Whether or not the pattern matches.
	//--------------------------------------------------------
	bool StartsWith(const std::string& in_string, const std::string& in_pattern);
	//--------------------------------------------------------
	/// Returns whether the string ends with the pattern passed 
	/// in.
	///
	/// @author Ian Copland
	///
	/// @param The input string.
	/// @param The pattern to compare with.
	///
	/// @return Whether or not the pattern matches.
	//--------------------------------------------------------
	bool EndsWith(const std::string& in_string, const std::string& in_pattern);
	//--------------------------------------------------------
	/// Replaces all instances of a string "replaceWhat" with
	/// string "replaceWithWhat" in the source string.
	///
	/// @author Ian Copland
	///
	/// @param The source string.
	/// @param The string to replace.
	/// @param The string to replace with.
	///
	/// @return The output string.
	//--------------------------------------------------------
    std::string ReplaceAll(const std::string& in_source, const std::string& in_replaceWhat, const std::string& in_replaceWithWhat);
	//--------------------------------------------------------
	/// Converts a value to a string.
	///
	/// @author Ian Copland
	///
	/// @param the value.
	///
	/// @return the string.
	//--------------------------------------------------------
	template<class T> std::string StringCast(T in_value)
	{
		std::stringstream stream;
		stream << in_value;
		return stream.str();
	}
	//--------------------------------------------------------
	/// Casts a bool to a string.
	///
	/// @author Ian Copland
	///
	/// @param the bool.
	///
	/// @return the string.
	//--------------------------------------------------------
	template<> std::string StringCast<>(bool in_value);
	//--------------------------------------------------------
	/// Parses the content of a string to a base type.
	///
	/// @author Ian Copland
	///
	/// @param The string.
	///
	/// @return the value.
	//--------------------------------------------------------
	template<class T> T ParseString(const std::string& in_string)
	{
		std::istringstream stream(in_string);
		T output;
		stream >> output;
		return output;
	}
}

#endif
