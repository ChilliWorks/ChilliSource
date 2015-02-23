//
//  StringUtils.h
//  Chilli Source
//  Created by Stuart McGaw on 25/10/2010.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2010 Tag Games Limited
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

#ifndef _CHILLISOURCE_CORE_STRINGUTILITIES_H_
#define _CHILLISOURCE_CORE_STRINGUTILITIES_H_

#include <ChilliSource/ChilliSource.h>

#include <sstream>
#include <string>
#include <vector>

namespace ChilliSource 
{
	namespace Core
	{
		/** Utility class for manipulating Strings.  */
		namespace StringUtils
		{
			typedef std::stringstream StrStreamType;

            enum class StringTimeFormat
            {
                k_YYYYDDMMHHMM,
                k_total
            };
            
            /// Constant blank string, useful for returning by ref where local does not exist
			const std::string BLANK;
            
			/** Removes any whitespace characters, be it standard space or
			 TABs and so on.
			 @remarks
			 The user may specify wether they want to trim only the
			 beginning or the end of the String ( the default action is
			 to trim both).
			 */
			void Trim( std::string& str, bool left = true, bool right = true );
            
            /** Remove the number of characters from the end of the string.
			 */
			void Chop( std::string& str, u32 inudwNumCharacters);
            
            /** Remove any of the given chars from the end of the string.
			 */
			void ChopTrailingChars( std::string& str, s8 inbyChar);
	
            /** Ensures the input string is terminated by a singular instance of
			 inputted terminator
			 @param 
             delims The string to terminate
             @param
             delims The string used to terminate first string
             */
            void TerminateStringWith(std::string& instrMain, const std::string & instrEnding);
			
			/** Returns a StringVector that contains all the substrings delimited
			 by the characters in the passed <code>delims</code> argument.
			 @param
			 delims A list of delimiter characters to split by
			 @param
			 maxSplits The maximum number of splits to perform (0 for unlimited splits). If this
			 parameters is > 0, the splitting process will stop after this many splits, left to right.
			 */
			std::vector< std::string > Split( const std::string& str, const std::string& delims = "\t\n ", unsigned int maxSplits = 0);

			/** Returns a StringVector that contains all the substrings delimited
			 by the characters in the passed <code>delims</code> argument, 
			 or in the <code>doubleDelims</code> argument, which is used to include (normal) 
			 delimeters in the tokenised string. For example, "strings like this".
			 @param
			 delims A list of delimiter characters to split by
			 @param
			 delims A list of double delimeters characters to tokenise by
			 @param
			 maxSplits The maximum number of splits to perform (0 for unlimited splits). If this
			 parameters is > 0, the splitting process will stop after this many splits, left to right.
			 */
			std::vector< std::string > Tokenise( const std::string& str, const std::string& delims = "\t\n ", const std::string& doubleDelims = "\"", unsigned int maxSplits = 0);

			/** Lower-cases all the characters in the string.
			 */
			void ToLowerCase( std::string& str );

			/** Upper-cases all the characters in the string.
			 */
			void ToUpperCase( std::string& str );

			/** Upper-cases first character, Lower-cases the rest.
			 */
            void ToFirstLetterCapitalised( std::string& str );
            
			/** Performs first letter capitalisation on each word
             ** and removes space characters.
			 */
            void ToPascalCase( std::string& str );

			/** Returns whether the string begins with the pattern passed in.
			 @param pattern The pattern to compare with.
			 @param lowerCase If true, the start of the string will be lower cased before
			 comparison, pattern should also be in lower case.
			 */
			bool StartsWith(const std::string& str, const std::string& pattern, bool lowerCase = true);

			/** Returns whether the string ends with the pattern passed in.
			 @param pattern The pattern to compare with.
			 @param lowerCase If true, the end of the string will be lower cased before
			 comparison, pattern should also be in lower case.
			 */
			bool EndsWith(const std::string& str, const std::string& pattern, bool lowerCase = true);

			/** Method for standardising file paths - use forward slashes only, removes trailing slashes
			 */
			std::string StandardiseFilePath( const std::string &init);
            
            /** Method for standardising directory paths - use forward slashes only, end with slash.
			 */
			std::string StandardiseDirectoryPath( const std::string &init);

			/** Method for splitting a fully qualified filename into the base name
			 and path.
			 @remarks
			 Path is standardised as in standardisePath
			 */
			void SplitFilename(const std::string& qualifiedName, std::string& outBasename, std::string& outPath);

			/** Method for splitting a fully qualified filename into the base name,
			 extension and path.
			 @remarks
			 Path is standardised as in standardisePath
			 */
			void SplitFullFilename(const std::string& qualifiedName, std::string& outBasename, std::string& outExtention, std::string& outPath);
            
			/** Method for splitting a filename into the base name
			 and extension.
			 */
			void SplitBaseFilename(const std::string& fullName, std::string& outBasename, std::string& outExtention);
			
			/** Simple pattern-matching routine allowing a wildcard pattern.
			 @param str String to test
			 @param pattern Pattern to match against; can include simple '*' wildcards
			 @param caseSensitive Whether the match is case sensitive or not
			 */
			bool Match(const std::string& str, const std::string& pattern, bool caseSensitive = true);

			/** replace all instances of a sub-string with a another sub-string.
			 @param source Source string
			 @param replaceWhat Sub-string to find and replace
			 @param replaceWithWhat Sub-string to replace with (the new sub-string)
			 @returns An updated string with the sub-string replaced
			 */
			const std::string ReplaceAll(const std::string& source, const std::string& replaceWhat, const std::string& replaceWithWhat);

			/** converts a string to a unix timestamp
			 @param source String to convert to unix timestamp
			 @param format Format of source string
			 @return unix timestamp or 0
			 */
			TimeIntervalSecs ToTimestamp(const std::string& instrSource, StringTimeFormat ineFormat);
			
			// Utility function for comparing two characters in a case insensitive way
            bool CaseInsensitiveCompare(const char & inA, const char & inB);
            
            //-------------------------------------------------------
            /// Inserts variables into the given string with the
            /// values using basic markup. The markup is in the form
            /// [var=VariablenName], and the value for the variable is
            /// looked up in the given param dictionary.
            ///
            /// For example "Time remaining: [var=TimeRemaining]" with
            /// a param dictionary containig "TimeRemaining"="12" will
            /// become "Time Remaining: 12".
            ///
            /// The markup is also recursive, meaning that the
            /// following is valid: "[var=TextValue[var=ValueIndex]]".
			///
            /// @param N Tanda
            ///
			/// @param The UTF-8 string which contains the markup.
			/// @param The param dictionary containing the variables.
            ///
            /// @return The output formatted UTF-8 string.
			//-------------------------------------------------------
			std::string InsertVariables(const std::string& in_text, const Core::ParamDictionary& in_params);
            
            /** Checks the std::string is a inValid number inValue. */
			bool isNumber(const std::string& inVal);
            
            //----------------------------------------------------
            /// Convert a list of string values to a CSV string
            ///
            /// @author S Downie
            ///
            /// @param List of values
            /// @param [Out] CSV string
            //----------------------------------------------------
            void ToCSV(const std::vector<std::string>& in_values, std::string& out_csv);
            //-------------------------------------------------------
            /// Encodes a URL to use correct escape values
            ///
            /// @param String to URL encode
            ///
            /// @return URL encoded string
            //------------------------------------------------------
            std::string URLEncode(const std::string& in_url);
            //------------------------------------------------------
            /// @param String to URL decode
            ///
            /// @return decoded string
            //------------------------------------------------------
            std::string URLDecode(const std::string& in_src);
            //-------------------------------------------------------
            /// Converts a value in the range [0,255] to hex value
            ///
            /// @author S Downie
            ///
            /// @param Integer value in range [0,255]
            ///
            /// @return String hex representation of integer
            //-------------------------------------------------------
            std::string CharToHex(u8 in_dec);
            //-------------------------------------------------------
            /// Resolves ".." parented directories in paths relative
            /// to the root.
            ///
            /// NOTE: In CS even absolute paths are relative to the sandboxed
            /// storage locations and it is forbidden to go "up" beyond the root
            /// of each storage location.
            ///
            /// Examples:
            ///     "A/B.ext" -> "A/B.ext"
            ///     "A/../B.ext" -> "B.ext"
            ///     "A/../../B.ext" -> Assert
            ///
            /// @author S Downie
            ///
            /// @param File path containing parented links ("..")
            ///
            /// @return File path with ".." resolved
            //-------------------------------------------------------
            std::string ResolveParentedDirectories(const std::string& in_relPath);
		};
	}
}

#endif
