//
//  StringUtils.h
//  moFlow
//
//  Created by Stuart McGaw on 25/10/2010.
//  Copyright 2011 Tag Games. All rights reserved.
//

#include <moFlo/Core/StringUtils.h>
#include <algorithm>
#include <cctype>

#if (TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE) && __OBJC__
#import <Foundation/Foundation.h>
#endif

namespace moFlo
{
	namespace Core
	{
		const std::string kastrTimeFormat[moFlo::Core::NUM_TIME_FORMATS] =
		{
			"YYYY-MM-DD-HH:MM",
		};
		//-----------------------------------------------------------------------
		const std::string CStringUtils::BLANK;
        const UTF8String CStringUtils::UTF8_BLANK;
		const UTF8String CStringUtils::UTF8_MISSING("MISSING");
		//-----------------------------------------------------------------------
		void CStringUtils::Trim(std::string& str, bool left, bool right)
		{
			static const std::string delims = " \t\r";
			if(right)
				str.erase(str.find_last_not_of(delims)+1); // trim right
			if(left)
				str.erase(0, str.find_first_not_of(delims)); // trim left
		}

        //-----------------------------------------------------------------------
        void CStringUtils::Chop( std::string& str, u32 inudwNumCharacters)
        {
            str.erase(str.length() - inudwNumCharacters);
        }
        
        //-----------------------------------------------------------------------
        void CStringUtils::ChopTrailingChars( std::string& str, s8 inbyChar)
        {
            u32 udwNumCharsToRemove = 0;
            for(std::string::reverse_iterator it = str.rbegin(); it != str.rend(); ++it)
            {
                if((*it) == inbyChar)
                {
                    ++udwNumCharsToRemove;
                }
                else
                {
                    break;
                }
            }
            
            if(udwNumCharsToRemove > 0)
            {
                Chop(str, udwNumCharsToRemove);
            }
        }

        //-----------------------------------------------------------------------
        void CStringUtils::TerminateStringWith(std::string &instrMain, const std::string &instrEnding)
        {
            // Simplest case, already terminated
            if( EndsWith(instrMain, instrEnding))
                return;
            
            // Assume no partials if not already terminated
            instrMain += instrEnding;
        }
        
		//-----------------------------------------------------------------------
		DYNAMIC_ARRAY<std::string> CStringUtils::Split( const std::string& str, const std::string& delims, unsigned int maxSplits)
		{
			DYNAMIC_ARRAY<std::string> ret;
			// Pre-allocate some space for performance
			ret.reserve(maxSplits ? maxSplits+1 : 10);    // 10 is guessed capacity for most case
			
			unsigned int numSplits = 0;
			
			// Use STL methods 
			size_t start, pos;
			start = 0;
			do 
			{
				pos = str.find_first_of(delims, start);
				if (pos == start)
				{
					// Do nothing
					start = pos + 1;
				}
				else if (pos == std::string::npos || (maxSplits && numSplits == maxSplits))
				{
					// Copy the rest of the string
					ret.push_back( str.substr(start) );
					break;
				}
				else
				{
					// Copy up to delimiter
					ret.push_back( str.substr(start, pos - start) );
					start = pos + 1;
				}
				// parse up to next real data
				start = str.find_first_not_of(delims, start);
				++numSplits;
				
			} while (pos != std::string::npos);
			
			
			
			return ret;
		}


		//-----------------------------------------------------------------------
		DYNAMIC_ARRAY< std::string > CStringUtils::Tokenise( const std::string& str, const std::string& singleDelims, const std::string& doubleDelims, unsigned int maxSplits)
		{
			DYNAMIC_ARRAY<std::string> ret;
			// Pre-allocate some space for performance
			ret.reserve(maxSplits ? maxSplits+1 : 10);    // 10 is guessed capacity for most case
			
			unsigned int numSplits = 0;
			std::string delims = singleDelims + doubleDelims;
			
			// Use STL methods 
			size_t start, pos;
			char curDoubleDelim = 0;
			start = 0;
			do 
			{
				if (curDoubleDelim != 0)
				{
					pos = str.find(curDoubleDelim, start);
				}
				else
				{
					pos = str.find_first_of(delims, start);
				}
				
				if (pos == start)
				{
					char curDelim = str.at(pos);
					if (doubleDelims.find_first_of(curDelim) != std::string::npos)
					{
						curDoubleDelim = curDelim;
					}
					// Do nothing
					start = pos + 1;
				}
				else if (pos == std::string::npos || (maxSplits && numSplits == maxSplits))
				{
					if (curDoubleDelim != 0)
					{
						//Missing closer. Warn or throw exception?
					}
					// Copy the rest of the string
					ret.push_back( str.substr(start) );
					break;
				}
				else
				{
					if (curDoubleDelim != 0)
					{
						curDoubleDelim = 0;
					}
					
					// Copy up to delimiter
					ret.push_back( str.substr(start, pos - start) );
					start = pos + 1;
				}
				if (curDoubleDelim == 0)
				{
					// parse up to next real data
					start = str.find_first_not_of(singleDelims, start);
				}
				
				++numSplits;
				
			} while (pos != std::string::npos);
			
			return ret;
		}

		//-----------------------------------------------------------------------
		void CStringUtils::ToLowerCase(std::string& str)
		{
			std::transform(
						   str.begin(),
						   str.end(),
						   str.begin(),
						   tolower);
		}


		//-----------------------------------------------------------------------
		void CStringUtils::ToUpperCase(std::string& str) 
		{
			std::transform(
						   str.begin(),
						   str.end(),
						   str.begin(),
						   toupper);
		}
        
		//-----------------------------------------------------------------------
		void CStringUtils::ToFirstLetterCapitalised(std::string& str)
		{
            std::string strLower=str;
            std::string strUpper=str;
            ToLowerCase(strLower);
            ToUpperCase(strUpper);
            if(strUpper.length() > 0)
                str=strUpper.substr(0,1);
            if(strLower.length() > 1)
                str=str+strLower.substr(1,strLower.length()-1);
		}
        
		//-----------------------------------------------------------------------
		void CStringUtils::ToPascalCase(std::string& str)
		{
            std::string strPascal;
            std::string strChar;
            bool bNewWord = true;
            for(std::string::iterator it=str.begin(); it!=str.end(); ++it)
            {
                strChar=(*it);
                if(strChar==" " || strChar=="_" || strChar=="-")
                {
                    bNewWord=true;
                }
                else
                {
                    if(bNewWord)
                    {
                        ToUpperCase(strChar);
                    }
                    else
                    {
                        ToLowerCase(strChar);
                    }
                    strPascal=strPascal+strChar;
                    
                    bNewWord=false;
                }
            }

            str=strPascal;
		}

		//-----------------------------------------------------------------------
		bool CStringUtils::StartsWith(const std::string& str, const std::string& pattern, bool lowerCase)
		{
			size_t thisLen = str.length();
			size_t patternLen = pattern.length();
			if (thisLen < patternLen || patternLen == 0)
				return false;

			if (lowerCase)
                return std::equal(pattern.begin(), pattern.end(), str.begin(), CStringUtils::CaseInsensitiveCompare);
			
			return std::equal(pattern.begin(), pattern.end(), str.begin());
		}


		//-----------------------------------------------------------------------
		bool CStringUtils::EndsWith(const std::string& str, const std::string& pattern, bool lowerCase)
		{
			size_t thisLen = str.length();
			size_t patternLen = pattern.length();
			if (thisLen < patternLen || patternLen == 0)
				return false;

			if (lowerCase)
                return std::equal(pattern.begin(), pattern.end(), str.begin() + (thisLen - patternLen), CStringUtils::CaseInsensitiveCompare);

            return std::equal(pattern.begin(), pattern.end(), str.begin() + (thisLen - patternLen));
		}

		//-----------------------------------------------------------------------
		std::string CStringUtils::StandardisePath(const std::string& init)
		{
			std::string path = init;
			
			std::replace( path.begin(), path.end(), '\\', '/' );
            
            // Find last "/" and use it to determine if text
            // between it and the end of the string is a filename
            // has to be a rough guess as directories can be named like files
            size_t lastSlash = path.find_last_of('/');
            
            // No slashes in path so just use start
            if(lastSlash == std::string::npos)
                lastSlash = 0;
            
            if(path.find('.', lastSlash) == std::string::npos)
            {
                if( path.length() != 0)
                    TerminateStringWith(path, "/");
            }
			
			return path;
		}

		//-----------------------------------------------------------------------
		void CStringUtils::SplitFilename(const std::string& qualifiedName, 
									   std::string& outBasename, std::string& outPath)
		{
			std::string path = qualifiedName;
			// Replace \ with / first
			std::replace( path.begin(), path.end(), '\\', '/' );
			// split based on final /
			size_t i = path.find_last_of('/');
			
			if (i == std::string::npos)
			{
				outPath.clear();
				outBasename = qualifiedName;
			}
			else
			{
				outBasename = path.substr(i+1, path.size() - i - 1);
				outPath = path.substr(0, i+1);
			}
			
		}


		//-----------------------------------------------------------------------
		void CStringUtils::SplitBaseFilename(const std::string& fullName, 
										  std::string& outBasename, std::string& outExtention)
		{
			size_t i = fullName.find_last_of(".");
			if (i == std::string::npos)
			{
				outExtention.clear();
				outBasename = fullName;
			}
			else
			{
				outExtention = fullName.substr(i+1);
				outBasename = fullName.substr(0, i);
			}
		}

		// ----------------------------------------------------------------------------------------------------------------------------------------------
		void CStringUtils::SplitFullFilename(	const std::string& qualifiedName, 
										   std::string& outBasename, std::string& outExtention, std::string& outPath )
		{
			std::string fullName;
			SplitFilename( qualifiedName, fullName, outPath );
			SplitBaseFilename( fullName, outBasename, outExtention );
		}

		//-----------------------------------------------------------------------
		bool CStringUtils::Match(const std::string& str, const std::string& pattern, bool caseSensitive)
		{
			std::string tmpStr = str;
			std::string tmpPattern = pattern;
			if (!caseSensitive)
			{
				CStringUtils::ToLowerCase(tmpStr);
				CStringUtils::ToLowerCase(tmpPattern);
			}
			
			std::string::const_iterator strIt = tmpStr.begin();
			std::string::const_iterator patIt = tmpPattern.begin();
			std::string::const_iterator lastWildCardIt = tmpPattern.end();
			while (strIt != tmpStr.end() && patIt != tmpPattern.end())
			{
				if (*patIt == '*')
				{
					lastWildCardIt = patIt;
					// Skip over looking for next character
					++patIt;
					if (patIt == tmpPattern.end())
					{
						// Skip right to the end since * matches the entire rest of the string
						strIt = tmpStr.end();
					}
					else
					{
						// scan until we find next pattern character
						while(strIt != tmpStr.end() && *strIt != *patIt)
							++strIt;
					}
				}
				else
				{
					if (*patIt != *strIt)
					{
						if (lastWildCardIt != tmpPattern.end())
						{
							// The last wildcard can match this incorrect sequence
							// rewind pattern to wildcard and keep searching
							patIt = lastWildCardIt;
							lastWildCardIt = tmpPattern.end();
						}
						else
						{
							// no wildwards left
							return false;
						}
					}
					else
					{
						++patIt;
						++strIt;
					}
				}
				
			}
			// If we reached the end of both the pattern and the string, we succeeded
			if (patIt == tmpPattern.end() && strIt == tmpStr.end())
			{
				return true;
			}
			else
			{
				return false;
			}
			
		}

		//-----------------------------------------------------------------------
		const std::string CStringUtils::ReplaceAll(const std::string& source, const std::string& replaceWhat, const std::string& replaceWithWhat)
		{
			std::string result = source;
			while(1)
			{
				std::string::size_type pos = result.find(replaceWhat);
				if (pos == std::string::npos) break;
				result.replace(pos,replaceWhat.size(),replaceWithWhat);
			}
			return result;
		}
		
		/** converts a string to a unix timestamp
		 */
		TimeIntervalSecs CStringUtils::ToTimestamp(const std::string& instrSource, moFlo::Core::StringTimeFormat ineFormat)
		{
			std::string strTime = instrSource;
			std::string strFormat = moFlo::Core::kastrTimeFormat[ineFormat];
			
			if(moFlo::Core::TIME_FORMAT_YYYYDDMMHHMM == ineFormat)
			{
				std::string::size_type pos = strFormat.find('-');
				std::string strYear = strTime.substr(0, pos);
				
				strFormat = strFormat.substr(pos + 1, std::string::npos);
				strTime = strTime.substr(pos + 1, std::string::npos);
				pos = strFormat.find('-');
				std::string strMonth = strTime.substr(0, pos);
				
				strFormat = strFormat.substr(pos + 1, std::string::npos);
				strTime = strTime.substr(pos + 1, std::string::npos);
				pos = strFormat.find('-');
				std::string strDay = strTime.substr(0, pos);
				
				strFormat = strFormat.substr(pos + 1, std::string::npos);
				strTime = strTime.substr(pos + 1, std::string::npos);
				pos = strFormat.find(':');
				std::string strHour = strTime.substr(0, pos);
				
				std::string strMins = strTime.substr(pos + 1, std::string::npos);
				
				return ToTimeIntervalSecs(strYear, strMonth, strDay, strHour, strMins, "0");
			}
			
			return 0;
		}


		const std::string CStringUtils::StandardiseClassName(const std::string & insClassName){
				if (StartsWith(insClassName,"C",false) || StartsWith(insClassName,"I",false)) {
					return insClassName.substr(1,insClassName.length()-1);
				}
			return insClassName;
		}
        
        bool CStringUtils::CaseInsensitiveCompare(const char &inA, const char &inB)
        {
            return tolower(inA) == tolower(inB);
        }
		
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
		void CStringUtils::ReplaceVariableRecursive(const UTF8String& instrText, const Core::ParamDictionary& insParams, UTF8String& outstrText, UTF8String::iterator& it)
		{
			//Found some mark-up. What type is it?
			std::string strType;
			s8 byNextChar = '\0';
			
			while(byNextChar != '=')
			{
				byNextChar = (s8)instrText.next(it);
				
				if(byNextChar != '=' && byNextChar != ' ')
				{
					strType += byNextChar;
				}
			}
			
			// Variable type has been located
			std::string strVarName;
			std::string strVarValue;
			
			// There may be some whitespace that we need to ignore
			byNextChar = (s8)instrText.next(it);
			if(byNextChar != ' ')
			{
				strVarName += byNextChar;
			}
			
			// Find the closing bracket
			while(byNextChar != ']')
			{
				byNextChar = (s8)instrText.next(it);
				
				if(byNextChar != ']' && byNextChar != '[' && byNextChar != ' ')
				{
					strVarName += byNextChar;
				}
				
				// Nested variable
				if(byNextChar == '[')
				{
					UTF8String strVariableName;
					ReplaceVariableRecursive(instrText, insParams, strVariableName, it);
					strVarName += strVariableName.ToASCII();
				}
			}
            
            bool bVariableReplaced = false;
			
			if(strType == "var")
			{
				if(insParams.TryGetValue(strVarName, strVarValue))
				{
					//Let's replace the mark-up with the value
					outstrText.append(strVarValue);
                    bVariableReplaced = true;
				}
			}
            
			// If not, readd the [xxx= ]
			if(!bVariableReplaced)
			{
				outstrText.append("[" + strType + "= " + strVarName + "]");
			}
		}
    
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
        void CStringUtils::InsertVariables(const UTF8String& instrText, const Core::ParamDictionary& insParams, UTF8String& outstrText)
		{
			UTF8String::iterator it = (UTF8String::iterator)instrText.begin();
			while(it != instrText.end())
			{
				UTF8String::Char NextUTFChar = instrText.next(it);
				
				if(NextUTFChar != '[')
				{
					outstrText.appendChar(NextUTFChar);
				}
				else
				{
					// Found a mark up, check it
					ReplaceVariableRecursive(instrText, insParams, outstrText, it);
				}
			}
		}
        
#if (TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE) && __OBJC__
        /**
         //Given a NSString it converts it to a UTF-8 string
         @param input string
         @returns utf8 string
         */
        UTF8String CStringUtils::NSStringToUTF8String(NSString* inpString)   
        {   
            NSStringEncoding pEncode    =   CFStringConvertEncodingToNSStringEncoding (kCFStringEncodingUTF8);   
            NSData* pSData              =   [inpString dataUsingEncoding : pEncode];    
            
            return UTF8String ((UTF8String::CodePoint*) [pSData bytes], [pSData length]);   
        }
        /**
         //Given a NSString UTF-8 map each code point to a char in the std::string
         @param NSString utf8 string
         @returns std::string
         */
        std::string CStringUtils::NSStringToString(NSString* inpString)   
        {
            if(inpString == nil)
                return "";
            return [inpString cStringUsingEncoding:NSUTF8StringEncoding];
        }
        /**
         //Given a UTF-8 convert to NSString
         @param utf8 string
         @returns NSString
         */
        NSString* CStringUtils::UTF8StringToNSString(const UTF8String& instrString)
        {
            s8* pData = (s8*)instrString.data();
            size_t Length = instrString.data_size();
            
            if(pData == NULL || Length == 0)
                return [[[NSString alloc] init] autorelease];
            
            NSStringEncoding pEncode = CFStringConvertEncodingToNSStringEncoding (kCFStringEncodingUTF8);  
            
            NSString* result = [[[NSString alloc] initWithBytes:pData length:Length encoding:pEncode] autorelease];
            return result;
        }
        /**
         //Given a std::string UTF-8 convert to a NSString utf8
         @param std::string
         @returns NSString
         */
        NSString* CStringUtils::StringToNSString(const std::string& instrString)
        {
            return [NSString stringWithCString:instrString.c_str() encoding: NSUTF8StringEncoding];
        }
#endif
		
		//-- Private
		
		/**
		 Converts the given time to a timestamp in seconds
		 */
		TimeIntervalSecs CStringUtils::ToTimeIntervalSecs(const std::string& instrYear,
														  const std::string& instrMonth,
														  const std::string& instrDays,
														  const std::string& instrHours,
														  const std::string& instrMins,
														  const std::string& instrSecs)
		{
			u32 udwYears = CStringConverter::ParseInt(instrYear);
			u32 udwMonths = CStringConverter::ParseInt(instrMonth);
			if(udwMonths > 0)
				udwMonths--;// Don't cound full month for current month
			u32 udwDays = CStringConverter::ParseInt(instrDays);
			if(udwDays > 0)
				udwDays--;// Don't count full day as starts on 1st of month
			u32 udwHours = CStringConverter::ParseInt(instrHours);
			if(udwHours > 0)
				udwHours--;// Don't count 1 hour of days
			u32 udwMins = CStringConverter::ParseInt(instrMins);
			u32 udwSecs = CStringConverter::ParseInt(instrSecs);
			
			u32 udwTotalDays = 0;
			for(u32 i = 1970; i < udwYears; i++)
			{
				if((i % 4) != 0)
					udwTotalDays += 365;
				else
				{
					udwTotalDays += 366;
				}
			}
			for(u32 i = 0; i < udwMonths; i++)
			{
				if((i == 3) || (i == 5) || (i == 8) || (i == 10))
					udwTotalDays += 30;
				else
					if(i == 1)
					{
						if((udwYears % 4) != 0)
							udwTotalDays += 28;
						else
							udwTotalDays += 29;
					}
					else
						udwTotalDays += 31;
			}
			udwTotalDays += udwDays;
			
			u32 udwTotalHours = (udwTotalDays * 24) + udwHours;
			u32 udwTotalMins = (udwTotalHours * 60) + udwMins;
			u32 udwTotalSecs = (udwTotalMins * 60) + udwSecs;
			
			return udwTotalSecs;
		}
	}
}
