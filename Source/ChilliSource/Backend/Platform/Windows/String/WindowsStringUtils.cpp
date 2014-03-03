//
//  WindowwsStringUtils.cpp
//  Chilli Source
//
//  Created by Ian Copland on 03/03/2014.
//  Copyright 2014 Tag Games. All rights reserved.
//

#include <ChilliSource/Backend/Platform/Windows/String/WindowsStringUtils.h>

namespace ChilliSource
{
	namespace Windows
	{
		namespace WindowsStringUtils
		{
			//------------------------------------------------------------
			//------------------------------------------------------------
			std::wstring UTF8ToUTF16(const std::string& in_utf8String)
			{
				std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
				return converter.from_bytes(in_utf8String);
			}
			//------------------------------------------------------------
			//------------------------------------------------------------
			std::string UTF16ToUTF8(const std::wstring& in_utf16String)
			{
				std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
				return converter.to_bytes(in_utf16String);
			}
		}
	}
}