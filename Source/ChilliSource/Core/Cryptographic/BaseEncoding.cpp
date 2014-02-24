//
//  BaseEncoding.cpp
//  iOSTemplate
//
//  Created by Scott Downie on 06/07/2011.
//  Copyright 2011 Tag Games. All rights reserved.

/* 
 base64.cpp and base64.h
 
 Copyright (C) 2004-2008 René Nyffenegger
 
 This source code is provided 'as-is', without any express or implied
 warranty. In no event will the author be held liable for any damages
 arising from the use of this software.
 
 Permission is granted to anyone to use this software for any purpose,
 including commercial applications, and to alter it and redistribute it
 freely, subject to the following restrictions:
 
 1. The origin of this source code must not be misrepresented; you must not
 claim that you wrote the original source code. If you use this source code
 in a product, an acknowledgment in the product documentation would be
 appreciated but is not required.
 
 2. Altered source versions must be plainly marked as such, and must not be
 misrepresented as being the original source code.
 
 3. This notice may not be removed or altered from any source distribution.
 
 René Nyffenegger rene.nyffenegger@adp-gmbh.ch
 
 */

#include <ChilliSource/Core/Cryptographic/BaseEncoding.h>

namespace ChilliSource
{
    //The printable characters that will be output
    const std::string kstrBaseEncodedChars("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/");
    
    //--------------------------------------------------------------------------
    /// Base 64 Encode
    //--------------------------------------------------------------------------
    std::string CBaseEncoding::Base64Encode(const std::string& instrVal)
    {
        u32 udwSize = instrVal.length();
        u32 udwBase64Size = (udwSize + 2 - ((udwSize + 2) % 3)) / 3 * 4;
        
        //Fill with equals
        std::string strResult(udwBase64Size, '=');
        	 
        u8 const* paubyBuffer = (u8*)instrVal.c_str();  
        u32 udwIndex = 0; 
        
        for(u32 i=0; i<udwSize / 3; ++i)
        {
            strResult[udwIndex++] = kstrBaseEncodedChars[paubyBuffer[0] >> 2];
            strResult[udwIndex++] = kstrBaseEncodedChars[((paubyBuffer[0] << 4) | (paubyBuffer[1] >> 4)) & 0x3f];
            strResult[udwIndex++] = kstrBaseEncodedChars[((paubyBuffer[1] << 2) | (paubyBuffer[2] >> 6)) & 0x3f];
            strResult[udwIndex++] = kstrBaseEncodedChars[paubyBuffer[2] & 0x3f];
            paubyBuffer += 3;
        }
        	 
        u32 udwRemainder = udwSize % 3;
        switch(udwRemainder)
        {
            case 0:
                break;
            case 1:
                strResult[udwIndex++] = kstrBaseEncodedChars[paubyBuffer[0] >> 2];
                strResult[udwIndex++] = kstrBaseEncodedChars[(paubyBuffer[0] << 4) & 0x3f];
                break;
            case 2:
                strResult[udwIndex++] = kstrBaseEncodedChars[paubyBuffer[0] >> 2];
                strResult[udwIndex++] = kstrBaseEncodedChars[((paubyBuffer[0] << 4) | (paubyBuffer[1] >> 4)) & 0x3f];
                strResult[udwIndex++] = kstrBaseEncodedChars[(paubyBuffer[1] << 2) & 0x3f];
                break;
        };
        
        return strResult;
    }
	//--------------------------------------------------------------------------
    /// Base 64 Encode
    //--------------------------------------------------------------------------
    std::string CBaseEncoding::Base64Encode(const s8* inpData, u32 inudwSize)
    {
        u32 udwSize = inudwSize;
        u32 udwBase64Size = (udwSize + 2 - ((udwSize + 2) % 3)) / 3 * 4;
        
        //Fill with equals
        std::string strResult(udwBase64Size, '=');
		
        u8 const* paubyBuffer = (u8*)inpData;  
        u32 udwIndex = 0; 
        
        for(u32 i=0; i<udwSize / 3; ++i)
        {
            strResult[udwIndex++] = kstrBaseEncodedChars[paubyBuffer[0] >> 2];
            strResult[udwIndex++] = kstrBaseEncodedChars[((paubyBuffer[0] << 4) | (paubyBuffer[1] >> 4)) & 0x3f];
            strResult[udwIndex++] = kstrBaseEncodedChars[((paubyBuffer[1] << 2) | (paubyBuffer[2] >> 6)) & 0x3f];
            strResult[udwIndex++] = kstrBaseEncodedChars[paubyBuffer[2] & 0x3f];
            paubyBuffer += 3;
        }
		
        u32 udwRemainder = udwSize % 3;
        switch(udwRemainder)
        {
            case 0:
                break;
            case 1:
                strResult[udwIndex++] = kstrBaseEncodedChars[paubyBuffer[0] >> 2];
                strResult[udwIndex++] = kstrBaseEncodedChars[(paubyBuffer[0] << 4) & 0x3f];
                break;
            case 2:
                strResult[udwIndex++] = kstrBaseEncodedChars[paubyBuffer[0] >> 2];
                strResult[udwIndex++] = kstrBaseEncodedChars[((paubyBuffer[0] << 4) | (paubyBuffer[1] >> 4)) & 0x3f];
                strResult[udwIndex++] = kstrBaseEncodedChars[(paubyBuffer[1] << 2) & 0x3f];
                break;
        };
        
        return strResult;
    }
    //--------------------------------------------------------------------------
    /// Base 64 Decode
    //--------------------------------------------------------------------------
    std::string CBaseEncoding::Base64Decode(const std::string& instrVal)
    {
        std::string strResult;
        strResult.reserve(instrVal.length());
        
        //The input bytes are grouped into threes
        u8 auby3[3];
        u8 auby4[4];
        
        u32 udwStringLength = instrVal.length();
        u32 i = 0;
        u32 j = 0;
        u32 udwIndex = 0;
        
        while (udwStringLength-- && (instrVal[udwIndex] != '=')) 
        {
            auby4[i++] = instrVal[udwIndex]; 
            udwIndex++;
            
            if(i == 4) 
            {
                for(i=0; i<4; i++)
                {
                    auby4[i] = kstrBaseEncodedChars.find(auby4[i]);
                }
                
                auby3[0] = (auby4[0] << 2) + ((auby4[1] & 0x30) >> 4);
                auby3[1] = ((auby4[1] & 0xf) << 4) + ((auby4[2] & 0x3c) >> 2);
                auby3[2] = ((auby4[2] & 0x3) << 6) + auby4[3];
                
                for(i=0; (i<3); i++)
                {
                    strResult += auby3[i];
                }
                i = 0;
            }
        }
        
        if(i > 0) 
        {
            for(j=i; j<4; j++)
            {
                auby4[j] = '\0';
            }
            
            for(j = 0; j <4; j++)
            {
                auby4[j] = kstrBaseEncodedChars.find(auby4[j]);
            }
            
            auby3[0] = (auby4[0] << 2) + ((auby4[1] & 0x30) >> 4);
            auby3[1] = ((auby4[1] & 0xf) << 4) + ((auby4[2] & 0x3c) >> 2);
            auby3[2] = ((auby4[2] & 0x3) << 6) + auby4[3];
            
            for(j=0; j<(i-1); j++) 
            {
                strResult += auby3[j];
            }
        }
        
        return strResult;
    }
	//--------------------------------------------------------------------------
	/// Base 64 Decode
	//--------------------------------------------------------------------------
	s32 CBaseEncoding::Base64Decode(const std::string& instrVal, s8* inpDataOut, u32 inudwSize)
	{
		s8* psbyOutData = new s8[instrVal.size()];
		u32 udwDataIndex = 0;
        
        //The input bytes are grouped into threes
        u8 auby3[3];
        u8 auby4[4];
        
        u32 udwStringLength = instrVal.length();
        u32 i = 0;
        u32 j = 0;
        u32 udwIndex = 0;
        
        while (udwStringLength-- && (instrVal[udwIndex] != '=')) 
        {
            auby4[i++] = instrVal[udwIndex]; 
            udwIndex++;
            
            if(i == 4) 
            {
                for(i=0; i<4; i++)
                {
                    auby4[i] = kstrBaseEncodedChars.find(auby4[i]);
                }
                
                auby3[0] = (auby4[0] << 2) + ((auby4[1] & 0x30) >> 4);
                auby3[1] = ((auby4[1] & 0xf) << 4) + ((auby4[2] & 0x3c) >> 2);
                auby3[2] = ((auby4[2] & 0x3) << 6) + auby4[3];
                
                for(i=0; (i<3); i++)
                {
                    psbyOutData[udwDataIndex++] = auby3[i];
                }
                i = 0;
            }
        }
        
        if(i > 0) 
        {
            for(j=i; j<4; j++)
            {
                auby4[j] = '\0';
            }
            
            for(j = 0; j <4; j++)
            {
                auby4[j] = kstrBaseEncodedChars.find(auby4[j]);
            }
            
            auby3[0] = (auby4[0] << 2) + ((auby4[1] & 0x30) >> 4);
            auby3[1] = ((auby4[1] & 0xf) << 4) + ((auby4[2] & 0x3c) >> 2);
            auby3[2] = ((auby4[2] & 0x3) << 6) + auby4[3];
            
            for(j=0; j<(i-1); j++) 
            {
               psbyOutData[udwDataIndex++] = auby3[j];
            }
        }
		
		for(u32 i = 0; i < std::min(inudwSize, udwDataIndex); i++)
		{
			inpDataOut[i] = psbyOutData[i];
		}
		
		delete[] psbyOutData;
		
		return udwDataIndex;
	}
	//--------------------------------------------------------------------------
	/// Char to Hex
	/// Converts a value in the range [0,255] to hex value
	//--------------------------------------------------------------------------
	std::string CBaseEncoding::CharToHex(u8 inubyDec)
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
	//--------------------------------------------------------------------------
	/// Encodes a URL to use correct escape values
	//--------------------------------------------------------------------------
	std::string CBaseEncoding::URLEncode(const std::string& instrURL)
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
				strEscaped.append(CBaseEncoding::CharToHex((u8)instrURL[i])); // Converts char 255 to string "FF"
			}
		}
		
		return strEscaped;
	}
    //--------------------------------------------------------------------------
	/// Hex to Dec
	//--------------------------------------------------------------------------
    u8 CBaseEncoding::HexToDec(const u8* inpHex)
    {
        if(*inpHex >= '0' && *inpHex <= '9')
        {
            return *inpHex - '0';
        }
        else
        if(*inpHex >= 'A' && *inpHex <= 'F')
        {
            return 10 + ('A' - *inpHex);
        }
        else
        {
            ERROR_LOG("Invalid hex value of \""+STRING_CAST(*inpHex)+"\"");
        }
        
        return -1;
    }
    //--------------------------------------------------------------------------
	/// Decodes a URL to a string
	//--------------------------------------------------------------------------
    std::string CBaseEncoding::URLDecode(const std::string& instrSrc)
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
                char dec1 = static_cast<char>(HexToDec((pSrc + 1)));
                char dec2 = static_cast<char>(HexToDec((pSrc + 2)));
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
}