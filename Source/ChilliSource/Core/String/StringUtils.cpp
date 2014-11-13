//
//  StringUtils.cpp
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

#include <ChilliSource/Core/String/StringUtils.h>

#include <ChilliSource/Core/Base/Utils.h>
#include <ChilliSource/Core/Container/ParamDictionary.h>
#include <ChilliSource/Core/String/StringParser.h>
#include <ChilliSource/Core/String/UTF8StringUtils.h>

#include <algorithm>
#include <cctype>

namespace ChilliSource
{
	namespace Core
	{
        namespace StringUtils
        {
            namespace
            {
                const std::string kastrTimeFormat[(u32)StringTimeFormat::k_total] =
                {
                    "YYYY-MM-DD-HH:MM",
                };
                
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
                /// @param Params
                /// @param Out: Text string (UTF-8)
                /// @param Out: String iterator (UTF-8)
                //-------------------------------------------------------
                void ReplaceVariableRecursive(const Core::ParamDictionary& insParams, std::string& outstrText, std::string::const_iterator& it)
                {
                    //Found some mark-up. What type is it?
                    std::string strType;
                    UTF8Char byNextChar = '\0';
                    
                    while(byNextChar != '=')
                    {
                        byNextChar = UTF8StringUtils::Next(it);
                        
                        if(byNextChar != '=' && byNextChar != ' ')
                        {
                            strType += byNextChar;
                        }
                    }
                    
                    // Variable type has been located
                    std::string strVarName;
                    std::string strVarValue;
                    
                    // There may be some whitespace that we need to ignore
                    byNextChar = UTF8StringUtils::Next(it);
                    if(byNextChar != ' ')
                    {
                        strVarName += byNextChar;
                    }
                    
                    // Find the closing bracket
                    while(byNextChar != ']')
                    {
                        byNextChar = UTF8StringUtils::Next(it);
                        
                        if(byNextChar != ']' && byNextChar != '[' && byNextChar != ' ')
                        {
                            strVarName += byNextChar;
                        }
                        
                        // Nested variable
                        if(byNextChar == '[')
                        {
                            std::string strVariableName;
                            ReplaceVariableRecursive(insParams, strVariableName, it);
                            strVarName += strVariableName;
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
                
                /**
                 Converts the given time to a timestamp in seconds
                 */
                TimeIntervalSecs ToTimeIntervalSecs(const std::string& instrYear,
                                                    const std::string& instrMonth,
                                                    const std::string& instrDays,
                                                    const std::string& instrHours,
                                                    const std::string& instrMins,
                                                    const std::string& instrSecs)
                {
                    u32 udwYears = ParseS32(instrYear);
                    u32 udwMonths = ParseS32(instrMonth);
                    if(udwMonths > 0)
                        udwMonths--;// Don't cound full month for current month
                    u32 udwDays = ParseS32(instrDays);
                    if(udwDays > 0)
                        udwDays--;// Don't count full day as starts on 1st of month
                    u32 udwHours = ParseS32(instrHours);
                    if(udwHours > 0)
                        udwHours--;// Don't count 1 hour of days
                    u32 udwMins = ParseS32(instrMins);
                    u32 udwSecs = ParseS32(instrSecs);
                    
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
            

            //-----------------------------------------------------------------------
            void Trim(std::string& str, bool left, bool right)
            {
                static const std::string delims = " \t\r";
                if(right)
                    str.erase(str.find_last_not_of(delims)+1); // trim right
                if(left)
                    str.erase(0, str.find_first_not_of(delims)); // trim left
            }

            //-----------------------------------------------------------------------
            void Chop( std::string& str, u32 inudwNumCharacters)
            {
                str.erase(str.length() - inudwNumCharacters);
            }
            
            //-----------------------------------------------------------------------
            void ChopTrailingChars( std::string& str, s8 inbyChar)
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
            void TerminateStringWith(std::string &instrMain, const std::string &instrEnding)
            {
                // Simplest case, already terminated
                if( EndsWith(instrMain, instrEnding))
                    return;
                
                // Assume no partials if not already terminated
                instrMain += instrEnding;
            }
            
            //-----------------------------------------------------------------------
            std::vector<std::string> Split( const std::string& str, const std::string& delims, unsigned int maxSplits)
            {
                std::vector<std::string> ret;
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
            std::vector< std::string > Tokenise( const std::string& str, const std::string& singleDelims, const std::string& doubleDelims, unsigned int maxSplits)
            {
                std::vector<std::string> ret;
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
            void ToLowerCase(std::string& str)
            {
                std::transform(
                               str.begin(),
                               str.end(),
                               str.begin(),
                               tolower);
            }


            //-----------------------------------------------------------------------
            void ToUpperCase(std::string& str)
            {
                std::transform(
                               str.begin(),
                               str.end(),
                               str.begin(),
                               toupper);
            }
            
            //-----------------------------------------------------------------------
            void ToFirstLetterCapitalised(std::string& str)
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
            void ToPascalCase(std::string& str)
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
            bool StartsWith(const std::string& str, const std::string& pattern, bool lowerCase)
            {
                size_t thisLen = str.length();
                size_t patternLen = pattern.length();
                if (thisLen < patternLen || patternLen == 0)
                    return false;

                if (lowerCase)
                    return std::equal(pattern.begin(), pattern.end(), str.begin(), StringUtils::CaseInsensitiveCompare);
                
                return std::equal(pattern.begin(), pattern.end(), str.begin());
            }


            //-----------------------------------------------------------------------
            bool EndsWith(const std::string& str, const std::string& pattern, bool lowerCase)
            {
                size_t thisLen = str.length();
                size_t patternLen = pattern.length();
                if (thisLen < patternLen || patternLen == 0)
                    return false;

                if (lowerCase)
                    return std::equal(pattern.begin(), pattern.end(), str.begin() + (thisLen - patternLen), StringUtils::CaseInsensitiveCompare);

                return std::equal(pattern.begin(), pattern.end(), str.begin() + (thisLen - patternLen));
            }

            //-----------------------------------------------------------------------
            std::string StandardiseFilePath(const std::string& init)
            {
                std::string path = init;
                
                std::replace( path.begin(), path.end(), '\\', '/' );
                path = ReplaceAll(path, "//", "/");
                
                ChopTrailingChars(path, '/');
                
                return path;
            }
            
            //-----------------------------------------------------------------------
            std::string StandardiseDirectoryPath(const std::string& init)
            {
                std::string path = init;
                
                std::replace( path.begin(), path.end(), '\\', '/' );
                path = ReplaceAll(path, "//", "/");
                
                if(path.empty() == false && path.back() != '/')
                {
                    TerminateStringWith(path, "/");
                }
                
                return path;
            }

            //-----------------------------------------------------------------------
            void SplitFilename(const std::string& qualifiedName,
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
            void SplitBaseFilename(const std::string& fullName,
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
            void SplitFullFilename(	const std::string& qualifiedName,
                                               std::string& outBasename, std::string& outExtention, std::string& outPath )
            {
                std::string fullName;
                SplitFilename( qualifiedName, fullName, outPath );
                SplitBaseFilename( fullName, outBasename, outExtention );
            }

            //-----------------------------------------------------------------------
            bool Match(const std::string& str, const std::string& pattern, bool caseSensitive)
            {
                std::string tmpStr = str;
                std::string tmpPattern = pattern;
                if (!caseSensitive)
                {
                    StringUtils::ToLowerCase(tmpStr);
                    StringUtils::ToLowerCase(tmpPattern);
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
            const std::string ReplaceAll(const std::string& source, const std::string& replaceWhat, const std::string& replaceWithWhat)
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
            TimeIntervalSecs ToTimestamp(const std::string& instrSource, StringTimeFormat ineFormat)
            {
                std::string strTime = instrSource;
                std::string strFormat = kastrTimeFormat[(u32)ineFormat];
                
                if(StringTimeFormat::k_YYYYDDMMHHMM == ineFormat)
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
            
            bool CaseInsensitiveCompare(const char &inA, const char &inB)
            {
                return tolower(inA) == tolower(inB);
            }
            

        
            //-------------------------------------------------------
            //-------------------------------------------------------
            std::string InsertVariables(const std::string& in_text, const Core::ParamDictionary& in_params)
            {
                std::string output;
                
                auto it = in_text.begin();
                while(it < in_text.end())
                {
                    auto character = UTF8StringUtils::Next(it);

                    if(character != '[')
                    {
                        UTF8StringUtils::Append(character, output);
                    }
                    else
                    {
                        // Found a mark up, check it
                        ReplaceVariableRecursive(in_params, output, it);
                    }
                }
                
                return output;
            }

            //-- Private
            

            //-----------------------------------------------------------------------
            bool isNumber(const std::string& val)
            {
                std::istringstream str(val);
                float tst;
                str >> tst;
                return !str.fail() && str.eof();
            }
            //----------------------------------------------------
            //----------------------------------------------------
            void ToCSV(const std::vector<std::string>& in_values, std::string& out_csv)
            {
                if(in_values.empty())
                    return;
                
                u32 numVals = in_values.size() - 1;
                for(u32 i=0; i<numVals; ++i)
                {
                    out_csv += in_values[i];
                    out_csv += ",";
                }
                
                //Add the last one without a following comma
                out_csv += in_values[numVals];
            }
            //---------------------------------------------------------
            //---------------------------------------------------------
            std::string URLEncode(const std::string& instrURL)
            {
                std::string strEscaped;
                
                u32 dwMax = instrURL.length();
                for(u32 i = 0; i < dwMax; i++)
                {
                    if( (48 <= instrURL[i] && instrURL[i] <= 57) ||			// 0-9
                       (65 <= instrURL[i] && instrURL[i] <= 90) ||			// ABC...XYZ
                       (97 <= instrURL[i] && instrURL[i] <= 122) || 		// abc...xyz
                       (instrURL[i]=='~' || instrURL[i]=='-' || instrURL[i]=='_' || instrURL[i]=='.') )
                    {
                        strEscaped.append(&instrURL[i], 1);
                    }
                    else
                    {
                        strEscaped.append("%");
                        strEscaped.append(StringUtils::CharToHex((u8)instrURL[i])); // Converts char 255 to string "FF"
                    }
                }
                
                return strEscaped;
            }
            //---------------------------------------------------------
            //---------------------------------------------------------
            std::string URLDecode(const std::string& instrSrc)
            {
                // Note from RFC1630: "Sequences which start with a percent
                // sign but are not followed by two hexadecimal characters
                // (0-9, A-F) are reserved for future extension"
                
                const unsigned char* pSrc = (const unsigned char*)instrSrc.c_str();
                const int udwSourceLength = instrSrc.length();
                const unsigned char* const pSourceEnd = pSrc + udwSourceLength;
                // last decodable '%'
                const unsigned char* const pSourceLastDecode = pSourceEnd - 2;
                
                char* const pStart = new char[udwSourceLength];
                char* pEnd = pStart;
                
                while(pSrc < pSourceLastDecode)
                {
                    if(*pSrc == '%')
                    {
                        char dec1 = static_cast<char>(Utils::HexToDec((pSrc + 1)));
                        char dec2 = static_cast<char>(Utils::HexToDec((pSrc + 2)));
                        if(-1 != dec1 && -1 != dec2)
                        {
                            *pEnd++ = (dec1 << 4) + dec2;
                            pSrc += 3;
                            continue;
                        }
                    }
                    *pEnd++ = *pSrc++;
                }
                
                // the last 2- chars
                while(pSrc < pSourceEnd)
                {
                    *pEnd++ = *pSrc++;
                }
                
                std::string sResult(pStart, pEnd);
                delete [] pStart;
                return sResult;
            }
            //---------------------------------------------------------
            //---------------------------------------------------------
            std::string CharToHex(u8 inubyDec)
            {
                u8 ubyDig1 = (inubyDec&0xF0) >> 4;
                u8 ubyDig2 = (inubyDec&0x0F);
                if ( 0 <= ubyDig1 && ubyDig1 <= 9) ubyDig1 += 48;    		// 0,48 in ASCII
                if (10 <= ubyDig1 && ubyDig1 <=15) ubyDig1 += 65 - 10; 		// A,65 in ASCII
                if ( 0 <= ubyDig2 && ubyDig2 <= 9) ubyDig2 += 48;
                if (10 <= ubyDig2 && ubyDig2 <=15) ubyDig2 += 65 - 10;
                
                std::string strResult;
                strResult.append((char*)&ubyDig1, 1);
                strResult.append((char*)&ubyDig2, 1);
                
                return strResult;
            }
            //-------------------------------------------------------
            //-------------------------------------------------------
            std::string ResolveParentedDirectories(const std::string& in_relPath)
            {
                std::string standardRelPath = StandardiseFilePath(in_relPath);
                
                std::vector<std::string> relDirs = Split(standardRelPath, "/");
                
                for(u32 i=0; i<relDirs.size(); )
                {
                    if(relDirs[i] == "..")
                    {
                        CS_ASSERT(i > 0, "StringUtils::ResolveParentedDirectories Cannot go up directories beyond the root");
                        relDirs.erase(relDirs.begin() + i);
                        relDirs.erase(relDirs.begin() + i - 1);
                        --i;
                    }
                    else
                    {
                        ++i;
                    }
                }

                std::string absPath;
                
                if(relDirs.size() > 0)
                {
                    for(u32 i=0; i<relDirs.size()-1; ++i)
                    {
                        absPath += relDirs[i] + "/";
                    }
                
                    absPath += relDirs.back();
                }
                
                return absPath;
            }
        }
	}
}
