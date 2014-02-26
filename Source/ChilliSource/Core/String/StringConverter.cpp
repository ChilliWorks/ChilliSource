/** 
 * File:  CStringConverter.cpp
 * Date: 25/10/2010
 * Description:
 */

/** 
 *
 * Author: Stuart McGaw 
 * Version: 1.0 - MoFlow
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#include <ChilliSource/Core/String/StringConverter.h>

#include <sstream>
#include <ChilliSource/Core/String/UTF8String.h>
#include <ChilliSource/Core/String/StringUtils.h>

using std::stringstream;
using std::istringstream;


namespace ChilliSource
{
	namespace Core
	{
        // Helper function to count items in a string seperated by a space ' ' character
        static const u32 EnumerateItems(const std::string& instrList)
        {
            u32 itemCount = 0;
            u32 i = 0;
            const u32 size = instrList.size();
            while(i < size)
            {
                if(instrList[i] != ' ')
                {
                    itemCount++;
                    while(instrList[i] != ' ' &&  i < size)i++;
                }
                
                while(instrList[i] == ' ' &&  i < size)i++;
            }
            
            return itemCount;
        }
        
		//-----------------------------------------------------------------------
		std::string  CStringConverter:: ToString(f32 val, unsigned short precision, 
										 unsigned short width, char fill, std::ios::fmtflags flags)
		{
			stringstream stream;
			stream.precision(precision);
			stream.width(width);
			stream.fill(fill);
			if (flags)
				stream.setf(flags);
			stream << val;
			return stream.str();
		}
        //-----------------------------------------------------------------------
        std::string CStringConverter:: ToString(f64 inVal, unsigned short precision,
                                    unsigned short width, char fill, std::ios::fmtflags flags)
        {
            stringstream stream;
			stream.precision(precision);
			stream.width(width);
			stream.fill(fill);
			if (flags)
				stream.setf(flags);
			stream << inVal;
			return stream.str();
        }
		//-----------------------------------------------------------------------
		std::string  CStringConverter:: ToString(s32 val, 
										 unsigned short width, char fill, std::ios::fmtflags flags)
		{
			stringstream stream;
			stream.width(width);
			stream.fill(fill);
			if (flags)
				stream.setf(flags);
			stream << val;
			return stream.str();
		}
        std::string  CStringConverter:: ToString(s32 val)
		{
            char outBuffer[16];
          
            u32 ret = sprintf(outBuffer, "%d", val);
           
            return std::string(outBuffer, outBuffer + ret);
		}
		//-----------------------------------------------------------------------
		std::string  CStringConverter:: ToString(u32 val,
										 unsigned short width, char fill, std::ios::fmtflags flags)
		{
            stringstream stream;
			stream.width(width);
			stream.fill(fill);
			if (flags)
				stream.setf(flags);
			stream << val;
			return stream.str();
		}
        
        std::string  CStringConverter:: ToString(u32 val)
		{
            char outBuffer[16];

            u32 ret = sprintf(outBuffer, "%u", val);
            
            return std::string(outBuffer, outBuffer + ret);
		}
        
#ifdef TARGET_OS_IPHONE
        std::string CStringConverter::ToString(size_t inVal)
        {
            return ToString((u32)inVal);
        }
#endif
        
		std::string  CStringConverter:: ToString(u64 val, 
										 unsigned short width, char fill, std::ios::fmtflags flags)
		{
			stringstream stream;
			stream.width(width);
			stream.fill(fill);
			if (flags)
				stream.setf(flags);
			stream << val;
			return stream.str();
		}
		std::string  CStringConverter:: ToString(s64 val, 
										 unsigned short width, char fill, std::ios::fmtflags flags)
		{
			stringstream stream;
			stream.width(width);
			stream.fill(fill);
			if (flags)
				stream.setf(flags);
			stream << val;
			return stream.str();
		}
		
		//-----------------------------------------------------------------------
		std::string  CStringConverter:: ToString(const CVector2& val)
		{
			stringstream stream;
			stream << val.x << " " << val.y;
			return stream.str();
		}
		//-----------------------------------------------------------------------
		std::string  CStringConverter:: ToString(const CVector3& val)
		{
			stringstream stream;
			stream << val.x << " " << val.y << " " << val.z;
			return stream.str();
		}
		//-----------------------------------------------------------------------
		std::string  CStringConverter:: ToString(const CVector4& val)
		{
			stringstream stream;
			stream << val.x << " " << val.y << " " << val.z << " " << val.w;
			return stream.str();
		}
		//-----------------------------------------------------------------------
		std::string  CStringConverter:: ToString(bool val, bool yesNo)
		{
			if (val)
			{
				if (yesNo)
				{
					return "yes";
				}
				else
				{
					return "true";
				}
			}
			else
				if (yesNo)
				{
					return "no";
				}
				else
				{
					return "false";
				}
		}
		//-----------------------------------------------------------------------
		std::string  CStringConverter:: ToString(const CMatrix4x4 & val)
		{
			stringstream stream;
			stream << val.m[0] << " " 
			<< val.m[1] << " "             
			<< val.m[2] << " "             
			<< val.m[3] << " "             
			<< val.m[4] << " "             
			<< val.m[5] << " "             
			<< val.m[6] << " "             
			<< val.m[7] << " "             
			<< val.m[8] << " "             
			<< val.m[9] << " "             
			<< val.m[10] << " "             
			<< val.m[11] << " "             
			<< val.m[12] << " "             
			<< val.m[13] << " "             
			<< val.m[14] << " "             
			<< val.m[15];
			return stream.str();
		}
		//-----------------------------------------------------------------------
		std::string  CStringConverter:: ToString(const CQuaternion& val)
		{
			stringstream stream;
			stream  << val.w << " " << val.x << " " << val.y << " " << val.z;
			return stream.str();
		}
		//-----------------------------------------------------------------------
		std::string  CStringConverter:: ToString(const CColour& val)
		{
			stringstream stream;
			stream << val.r << " " << val.g << " " << val.b << " " << val.a;
			return stream.str();
		}
        //-----------------------------------------------------------------------
        std::string CStringConverter::ToString(StorageLocation instrStorageLocation)
        {
            switch (instrStorageLocation)
            {
                case StorageLocation::k_package:
                    return "Package";
                case StorageLocation::k_saveData:
                    return "SaveData";
                case StorageLocation::k_cache:
                    return "Cache";
                case StorageLocation::k_DLC:
                    return "DLC";
                default:
                    return "";
            }
        }
		//-----------------------------------------------------------------------
		f32  CStringConverter:: ParseFloat(const std::string & val)
		{
            return strtod(val.c_str(), nullptr);
		}
		//-----------------------------------------------------------------------
		int  CStringConverter:: ParseInt(const std::string & val)
		{
	        return strtol(val.c_str(), nullptr, 10);
    	}
		//-----------------------------------------------------------------------
		unsigned int  CStringConverter:: ParseUnsignedInt(const std::string & val)
		{
			return strtoul(val.c_str(), nullptr, 10);
		}
		//-----------------------------------------------------------------------
        // DEPRECATED 
		long  CStringConverter:: ParseLong(const std::string & val)
		{
			// Use istringstream for direct correspondence with ToString
			istringstream str(val);
			long ret = 0;
			str >> ret;
			
			return ret;
		}
		//-----------------------------------------------------------------------
        // DEPRECATED 
		unsigned long  CStringConverter:: ParseUnsignedLong(const std::string & val)
		{
			// Use istringstream for direct correspondence with ToString
			istringstream str(val);
			unsigned long ret = 0;
			str >> ret;
			
			return ret;
		}
		//-----------------------------------------------------------------------
		bool  CStringConverter:: ParseBool(const std::string& val)
		{
			return (CStringUtils::StartsWith(val, "true") || CStringUtils::StartsWith(val, "yes")
					|| CStringUtils::StartsWith(val, "1"));
		}
		//-----------------------------------------------------------------------
		CVector2  CStringConverter:: ParseVector2(const std::string& val)
		{
            u32 udwSize = EnumerateItems(val);

			if (udwSize != 2)
			{
				return CVector2::ZERO;
			}
			else
			{
                CVector2 vRet;
                sscanf(val.c_str(), "%f %f", &vRet.x, &vRet.y);
				return vRet;
			}
		}
		//-----------------------------------------------------------------------
		CVector3  CStringConverter:: ParseVector3(const std::string& val)
		{
            u32 udwSize = EnumerateItems(val);

			if (udwSize != 3)
			{
				return CVector3::ZERO;
			}
			else
			{
				CVector3 vRet;
                sscanf(val.c_str(), "%f %f %f", &vRet.x, &vRet.y, &vRet.z);
				return vRet;
			}
		}
		//-----------------------------------------------------------------------
		CVector4  CStringConverter:: ParseVector4(const std::string& val)
		{
            u32 udwSize = EnumerateItems(val);
			
			if (udwSize != 4)
			{
				return CVector4::ZERO;
			}
			else
			{
				CVector4 vRet;
                sscanf(val.c_str(), "%f %f %f %f", &vRet.x, &vRet.y, &vRet.z, &vRet.w);
				return vRet;
			}
		}
		//-----------------------------------------------------------------------
		CMatrix4x4  CStringConverter:: ParseMatrix4(const std::string& val)
		{
            u32 udwSize = EnumerateItems(val);
		
			if (udwSize != 16)
			{
				return CMatrix4x4::IDENTITY;
			}
			else
			{
                CMatrix4x4 matRet;
                
                sscanf(val.c_str(), "%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f ",
                       &matRet.m[0],    &matRet.m[1],     &matRet.m[2],     &matRet.m[3],
                       &matRet.m[4],    &matRet.m[5],     &matRet.m[6],     &matRet.m[7],
                       &matRet.m[8],    &matRet.m[9],     &matRet.m[10],    &matRet.m[11],
                       &matRet.m[12],   &matRet.m[13],    &matRet.m[14],    &matRet.m[15]);
                
                return matRet;
                
			}
		}
		//-----------------------------------------------------------------------
		CQuaternion  CStringConverter:: ParseQuaternion(const std::string& val)
		{
            u32 udwSize = EnumerateItems(val);

			if (udwSize != 4)
			{
				return CQuaternion::IDENTITY;
			}
			else
			{
                CQuaternion qRet;
                sscanf(val.c_str(), "%f %f %f %f", &qRet.w, &qRet.x, &qRet.y, &qRet.z);
                return qRet;
			}
		}
		//-----------------------------------------------------------------------
		CColour  CStringConverter:: ParseColourValue(const std::string& val)
		{
            u32 udwSize = EnumerateItems(val);
            
			CColour cRet(CColour::BLACK);
            
			if (udwSize == 4)
			{
                sscanf(val.c_str(), "%f %f %f %f", &cRet.r, &cRet.g, &cRet.b, &cRet.a);

			}
			else if (udwSize == 3)
			{
                sscanf(val.c_str(), "%f %f %f", &cRet.r, &cRet.g, &cRet.b);
			}
            return cRet;
		}
		//-----------------------------------------------------------------------
        StorageLocation CStringConverter::ParseStorageLocation(const std::string &instrStorageLocation)
		{
			if(instrStorageLocation == "Package")		return StorageLocation::k_package;
			if(instrStorageLocation == "SaveData")		return StorageLocation::k_saveData;
			if(instrStorageLocation == "Cache")         return StorageLocation::k_cache;
			if(instrStorageLocation == "DLC")           return StorageLocation::k_DLC;
			
			//No test function found
			return StorageLocation::k_package;
		}
		//-----------------------------------------------------------------------
		bool  CStringConverter::isNumber(const std::string& val)
		{
			istringstream str(val);
			float tst;
			str >> tst;
			return !str.fail() && str.eof();
		}
    }
}
