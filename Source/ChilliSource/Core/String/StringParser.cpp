//
//  StringParser.cpp
//  Chilli Source
//  Created by Ian Copland on 25/02/2014.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2014 Tag Games Limited
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

#include <ChilliSource/Core/String/StringParser.h>

#include <ChilliSource/Core/String/StringUtils.h>
#include <ChilliSource/Rendering/Base/SurfaceFormat.h>

#include <cstdlib>
#include <string>

namespace ChilliSource
{
    namespace Core
    {
        namespace
        {
            //------------------------------------------------------------
            /// Helper function to count items in a string seperated by a
            /// space ' ' character.
            ///
            /// @author Ian Copland
            ///
            /// @param The string list.
            ///
            /// @return The number of items.
            //------------------------------------------------------------
            static const u32 EnumerateItems(const std::string& in_stringList)
            {
                u32 itemCount = 0;
                u32 i = 0;
                const u32 size = in_stringList.size();
                while(i < size)
                {
                    if(in_stringList[i] != ' ')
                    {
                        itemCount++;
                        while(in_stringList[i] != ' ' &&  i < size)i++;
                    }
                    
                    while(in_stringList[i] == ' ' &&  i < size)i++;
                }
                
                return itemCount;
            }
        }
        
        //------------------------------------------------------------
        //------------------------------------------------------------
        f32 ParseF32(const std::string& in_string)
        {
            return (f32)strtod(in_string.c_str(), nullptr);
        }
        //------------------------------------------------------------
        //------------------------------------------------------------
        s32 ParseS32(const std::string& in_string)
        {
            return strtol(in_string.c_str(), nullptr, 10);
        }
        //------------------------------------------------------------
        //------------------------------------------------------------
        u32 ParseU32(const std::string& in_string)
        {
            return strtoul(in_string.c_str(), nullptr, 10);
        }
        //------------------------------------------------------------
        //------------------------------------------------------------
        s64 ParseS64(const std::string& in_string)
        {
            std::istringstream str(in_string);
			long ret = 0;
			str >> ret;
			return ret;
        }
        //------------------------------------------------------------
        //------------------------------------------------------------
        u64 ParseU64(const std::string& in_string)
        {
            std::istringstream str(in_string);
			unsigned long ret = 0;
			str >> ret;
			return ret;
        }
        //------------------------------------------------------------
        //------------------------------------------------------------
        bool ParseBool(const std::string& in_string)
        {
            return (StringUtils::StartsWith(in_string, "true") || StringUtils::StartsWith(in_string, "yes") || StringUtils::StartsWith(in_string, "1"));
        }
        //------------------------------------------------------------
        //------------------------------------------------------------
        Vector2 ParseVector2(const std::string& in_string)
        {
            u32 udwSize = EnumerateItems(in_string);
            CS_ASSERT((udwSize == 2), "Invalid number of elements for Vector2.");
            
			if (udwSize != 2)
			{
				return Vector2::k_zero;
			}
			else
			{
                Vector2 vRet;
				CS_SSCANF(in_string.c_str(), "%f %f", &vRet.x, &vRet.y);
				return vRet;
			}
        }
        //------------------------------------------------------------
        //------------------------------------------------------------
        Vector3 ParseVector3(const std::string& in_string)
        {
            u32 udwSize = EnumerateItems(in_string);
            CS_ASSERT((udwSize == 3), "Invalid number of elements for Vector3.");
            
			if (udwSize != 3)
			{
				return Vector3::k_zero;
			}
			else
			{
				Vector3 vRet;
				CS_SSCANF(in_string.c_str(), "%f %f %f", &vRet.x, &vRet.y, &vRet.z);
				return vRet;
			}
        }
        //------------------------------------------------------------
        //------------------------------------------------------------
        Vector4 ParseVector4(const std::string& in_string)
        {
            u32 udwSize = EnumerateItems(in_string);
            CS_ASSERT((udwSize == 4), "Invalid number of elements for Vector4.");
            
			if (udwSize != 4)
			{
				return Vector4::k_zero;
			}
			else
			{
				Vector4 vRet;
				CS_SSCANF(in_string.c_str(), "%f %f %f %f", &vRet.x, &vRet.y, &vRet.z, &vRet.w);
				return vRet;
			}
        }
        //------------------------------------------------------------
        //------------------------------------------------------------
        Matrix3 ParseMatrix3(const std::string& in_string)
        {
            u32 udwSize = EnumerateItems(in_string);
            CS_ASSERT((udwSize == 9), "Invalid number of elements for Matrix3.");
            
			if (udwSize != 9)
			{
				return Matrix3::k_identity;
			}
			else
			{
                Matrix3 matRet;
                
				CS_SSCANF(in_string.c_str(), "%f %f %f %f %f %f %f %f %f",
                          &matRet.m[0], &matRet.m[1], &matRet.m[2],
                          &matRet.m[3], &matRet.m[4], &matRet.m[5],
                          &matRet.m[6], &matRet.m[7], &matRet.m[8]);
                
                return matRet;
			}
        }
        //------------------------------------------------------------
        //------------------------------------------------------------
        Matrix4 ParseMatrix4(const std::string& in_string)
        {
            u32 udwSize = EnumerateItems(in_string);
            CS_ASSERT((udwSize == 16), "Invalid number of elements for Matrix4.");
            
			if (udwSize != 16)
			{
				return Matrix4::k_identity;
			}
			else
			{
                Matrix4 matRet;
                
				CS_SSCANF(in_string.c_str(), "%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f ",
						&matRet.m[0], &matRet.m[1], &matRet.m[2], &matRet.m[3],
						&matRet.m[4], &matRet.m[5], &matRet.m[6], &matRet.m[7],
						&matRet.m[8], &matRet.m[9], &matRet.m[10], &matRet.m[11],
						&matRet.m[12], &matRet.m[13], &matRet.m[14], &matRet.m[15]);
                
                return matRet;
                
			}
        }
        //------------------------------------------------------------
        //------------------------------------------------------------
        Quaternion ParseQuaternion(const std::string& in_string)
        {
            u32 udwSize = EnumerateItems(in_string);
            CS_ASSERT((udwSize == 4), "Invalid number of elements for Quaternion.");
            
			if (udwSize != 4)
			{
				return Quaternion::k_identity;
			}
			else
			{
                Quaternion qRet;
				CS_SSCANF(in_string.c_str(), "%f %f %f %f", &qRet.w, &qRet.x, &qRet.y, &qRet.z);
                return qRet;
			}
        }
        //------------------------------------------------------------
        //------------------------------------------------------------
        Colour ParseColour(const std::string& in_string)
        {
            u32 udwSize = EnumerateItems(in_string);
            CS_ASSERT((udwSize == 3 || udwSize == 4), "Invalid number of elements for Colour.");
            
			Colour cRet(Colour::k_black);
            
			if (udwSize == 4)
			{
				CS_SSCANF(in_string.c_str(), "%f %f %f %f", &cRet.r, &cRet.g, &cRet.b, &cRet.a);
			}
			else if (udwSize == 3)
			{
                CS_SSCANF(in_string.c_str(), "%f %f %f", &cRet.r, &cRet.g, &cRet.b);
			}
            return cRet;
        }
        //------------------------------------------------------------
        //------------------------------------------------------------
        StorageLocation ParseStorageLocation(const std::string &in_string)
        {
            std::string lowerCase = in_string;
            StringUtils::ToLowerCase(lowerCase);
            
            if(lowerCase == "package")
            {
                return StorageLocation::k_package;
            }
			else if(lowerCase == "savedata")
            {
                return StorageLocation::k_saveData;
            }
			else if(lowerCase == "cache")
            {
                return StorageLocation::k_cache;
            }
			else if(lowerCase == "dlc")
            {
                return StorageLocation::k_DLC;
            }
            else if(lowerCase == "root")
            {
                return StorageLocation::k_root;
            }
            else if(lowerCase == "chillisource")
            {
                return StorageLocation::k_chilliSource;
            }
            else if(lowerCase == "none")
            {
                return StorageLocation::k_none;
            }
			
            CS_ASSERT(false, "Invalid storage location.");
			CS_LOG_ERROR("String Parser: Invalid storage location.");
			return StorageLocation::k_package;
        }
        //------------------------------------------------------------
        //------------------------------------------------------------
        Rendering::SurfaceFormat ParseSurfaceFormat(const std::string& in_surfaceFormat)
        {
            std::string lowerCase = in_surfaceFormat;
            StringUtils::ToLowerCase(lowerCase);
            
            if (lowerCase == "rgb565_depth24")
            {
                return Rendering::SurfaceFormat::k_rgb565_depth24;
            }
            else if (lowerCase == "rgb565_depth32")
            {
                return Rendering::SurfaceFormat::k_rgb565_depth32;
            }
            else if (lowerCase == "rgb888_depth24")
            {
                return Rendering::SurfaceFormat::k_rgb888_depth24;
            }
            else if (lowerCase == "rgb888_depth32")
            {
                return Rendering::SurfaceFormat::k_rgb888_depth32;
            }
            
            CS_ASSERT(false, "Invalid surface format.");
            CS_LOG_ERROR("String Parser: Invalid surface format.");
            return Rendering::SurfaceFormat::k_rgb565_depth24;
        }
    }
}
