//
//  StringUtils.h
//  moFlow
//
//  Created by Stuart McGaw on 25/10/2010.
//  Copyright 2011 Tag Games. All rights reserved.
//

#ifndef _MOFLO_CORE_STRINGUTILITIES_H_
#define _MOFLO_CORE_STRINGUTILITIES_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/String/UTF8String.h>
#include <ChilliSource/Core/Container/ParamDictionary.h>

#include <sstream>
#include <string>

#if (TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE) && __OBJC__
@class NSString;
#endif

namespace ChilliSource 
{
	namespace Core
	{
		enum class StringTimeFormat
		{
            k_YYYYDDMMHHMM,
            k_total
		};
		
		/** Utility class for manipulating Strings.  */
		class CStringUtils
		{
		public:
			typedef std::stringstream StrStreamType;

			/** Removes any whitespace characters, be it standard space or
			 TABs and so on.
			 @remarks
			 The user may specify wether they want to trim only the
			 beginning or the end of the String ( the default action is
			 to trim both).
			 */
			static void Trim( std::string& str, bool left = true, bool right = true );
            
            /** Remove the number of characters from the end of the string.
			 */
			static void Chop( std::string& str, u32 inudwNumCharacters);
            
            /** Remove any of the given chars from the end of the string.
			 */
			static void ChopTrailingChars( std::string& str, s8 inbyChar);
	
            /** Ensures the input string is terminated by a singular instance of
			 inputted terminator
			 @param 
             delims The string to terminate
             @param
             delims The string used to terminate first string
             */
            static void TerminateStringWith(std::string& instrMain, const std::string & instrEnding); 
			
			/** Returns a StringVector that contains all the substrings delimited
			 by the characters in the passed <code>delims</code> argument.
			 @param
			 delims A list of delimiter characters to split by
			 @param
			 maxSplits The maximum number of splits to perform (0 for unlimited splits). If this
			 parameters is > 0, the splitting process will stop after this many splits, left to right.
			 */
			static DYNAMIC_ARRAY< std::string > Split( const std::string& str, const std::string& delims = "\t\n ", unsigned int maxSplits = 0);

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
			static DYNAMIC_ARRAY< std::string > Tokenise( const std::string& str, const std::string& delims = "\t\n ", const std::string& doubleDelims = "\"", unsigned int maxSplits = 0);

			/** Lower-cases all the characters in the string.
			 */
			static void ToLowerCase( std::string& str );

			/** Upper-cases all the characters in the string.
			 */
			static void ToUpperCase( std::string& str );

			/** Upper-cases first character, Lower-cases the rest.
			 */
            static void ToFirstLetterCapitalised( std::string& str );
            
			/** Performs first letter capitalisation on each word
             ** and removes space characters.
			 */
            static void ToPascalCase( std::string& str );

			/** Returns whether the string begins with the pattern passed in.
			 @param pattern The pattern to compare with.
			 @param lowerCase If true, the start of the string will be lower cased before
			 comparison, pattern should also be in lower case.
			 */
			static bool StartsWith(const std::string& str, const std::string& pattern, bool lowerCase = true);

			/** Returns whether the string ends with the pattern passed in.
			 @param pattern The pattern to compare with.
			 @param lowerCase If true, the end of the string will be lower cased before
			 comparison, pattern should also be in lower case.
			 */
			static bool EndsWith(const std::string& str, const std::string& pattern, bool lowerCase = true);

			/** Method for standardising paths - use forward slashes only, end with slash.
			 */
			static std::string StandardisePath( const std::string &init);

			/** Method for splitting a fully qualified filename into the base name
			 and path.
			 @remarks
			 Path is standardised as in standardisePath
			 */
			static void SplitFilename(const std::string& qualifiedName, std::string& outBasename, std::string& outPath);

			/** Method for splitting a fully qualified filename into the base name,
			 extension and path.
			 @remarks
			 Path is standardised as in standardisePath
			 */
			static void SplitFullFilename(const std::string& qualifiedName, std::string& outBasename, std::string& outExtention, std::string& outPath);
            
			/** Method for splitting a filename into the base name
			 and extension.
			 */
			static void SplitBaseFilename(const std::string& fullName, std::string& outBasename, std::string& outExtention);
			
			/** Simple pattern-matching routine allowing a wildcard pattern.
			 @param str String to test
			 @param pattern Pattern to match against; can include simple '*' wildcards
			 @param caseSensitive Whether the match is case sensitive or not
			 */
			static bool Match(const std::string& str, const std::string& pattern, bool caseSensitive = true);

			/** replace all instances of a sub-string with a another sub-string.
			 @param source Source string
			 @param replaceWhat Sub-string to find and replace
			 @param replaceWithWhat Sub-string to replace with (the new sub-string)
			 @returns An updated string with the sub-string replaced
			 */
			static const std::string ReplaceAll(const std::string& source, const std::string& replaceWhat, const std::string& replaceWithWhat);

			/** converts a string to a unix timestamp
			 @param source String to convert to unix timestamp
			 @param format Format of source string
			 @return unix timestamp or 0
			 */
			static TimeIntervalSecs ToTimestamp(const std::string& instrSource, ChilliSource::Core::StringTimeFormat ineFormat);
			
			/**
			 //Given a classname in the form CClass or IClass would return Class. Useful for datadriven shenanigans
			 @param input name
			 @returns standardised name
			 */
			static const std::string StandardiseClassName(const std::string & insClassName);
            /**
			 //Given a std::string it converts it to a UTF-8 string
			 @param input string
			 @returns utf8 string
			 */
            static inline UTF8String StringToUTF8String(const std::string& instrString)
            {
                return UTF8String(instrString);
            }
            /**
			 //Given a UTF-8 map each code point to a char in the std::string
			 @param utf8 string
			 @returns std::string
			 */
            static inline std::string UTF8StringToString(const UTF8String& instrString)
            {
                return instrString.ToASCII();
            }
            
#if (TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE) && __OBJC__
            /**
			 //Given a NSString it converts it to a UTF-8 string
			 @param input string
			 @returns utf8 string
			 */
			static UTF8String NSStringToUTF8String(NSString* inpString);
            /**
			 //Given a NSString UTF-8 map each code point to a char in the std::string
			 @param NSString utf8 string
			 @returns std::string
			 */
			static std::string NSStringToString(NSString* inpString);
            /**
			 //Given a UTF-8 convert to NSString
			 @param utf8 string
			 @returns NSString
			 */
			static NSString* UTF8StringToNSString(const UTF8String& instrString);
            /**
			 //Given a std::string UTF-8 convert to a NSString utf8
			 @param std::string
			 @returns NSString
			 */
            static NSString* StringToNSString(const std::string& instrString);
#endif
			// Utility function for comparing two characters in a case insensitive way
            static bool CaseInsensitiveCompare(const char & inA, const char & inB);
            
            //-------------------------------------------------------
			/// Insert Variables
			///
			/// Find any variable mark-up and insert the
			/// value of the variables
			///
			/// For instance "My string contains [var =a] variable
			/// and [var= b] variable called a and b
			///
			/// @param Text string
			/// @param Params
            /// @param Out: Text string
			//-------------------------------------------------------
			static void InsertVariables(const UTF8String& instrText, const Core::ParamDictionary& insParams, UTF8String& outstrText);
            
			/// Constant blank string, useful for returning by ref where local does not exist
			static const std::string BLANK;
            static const UTF8String UTF8_BLANK;
			static const UTF8String UTF8_MISSING;
			
		private:
			//-------------------------------------------------------
			/// Replace Variables Recursive
			///
			/// Find any variable or nested variable mark-up and insert the
			/// value of the variables
			///
			/// For instance "My string contains [var =a] variable
			/// and [var= b] variable called a and b
			///
			/// Furthermore, [var= Var_[var= a]_b] has variables
			/// called a, and e.g.  Var_12_b   (if a == "12")
			///
			/// @param Text string
			/// @param Params
			/// @param Out: Text string
			/// @param Out: String iterator
			//-------------------------------------------------------
			static void ReplaceVariableRecursive(const UTF8String& instrText, const Core::ParamDictionary& insParams, UTF8String& outstrText, UTF8String::iterator& it);
			
			/**
			 Converts the given time to a timestamp in seconds
			 @param Year
			 @param Month
			 @param Days
			 @param Hours
			 @param Seconds
			 @param Minutes
			 @param Seconds
			 */			 
			static TimeIntervalSecs ToTimeIntervalSecs(const std::string& instrYear,
													   const std::string& instrMonth,
													   const std::string& instrDays,
													   const std::string& instrHours,
													   const std::string& instrMins,
													   const std::string& instrSecs);
            
            /** Checks the std::string is a inValid number inValue. */
			static bool isNumber(const std::string& inVal);
		};
	}
}

#endif
