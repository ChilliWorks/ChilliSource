//
//  WindowwsStringUtils.cpp
//  Chilli Source
//
//  Created by Ian Copland on 03/03/2014.
//  Copyright 2014 Tag Games. All rights reserved.
//

#ifdef CS_TARGETPLATFORM_WINDOWS

#include <CSBackend/Platform/Windows/Core/String/WindowsStringUtils.h>

#include <ChilliSource/Core/String/StringUtils.h>

#include <algorithm>

namespace CSBackend
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
			//------------------------------------------------------------
			//------------------------------------------------------------
			std::wstring ConvertStandardPathToWindows(const std::string& in_standardPath)
			{
				std::wstring path = UTF8ToUTF16(in_standardPath);

				std::replace(path.begin(), path.end(), L'/', L'\\');

				if (path[path.size() - 1] == L'\\')
				{
					path = path.substr(0, path.size() - 1);
				}

				return path;
			}
			//------------------------------------------------------------
			//------------------------------------------------------------
			std::string ConvertWindowsDirectoryPathToStandard(const std::wstring& in_windowsPath)
			{
				return CSCore::StringUtils::StandardiseDirectoryPath(UTF16ToUTF8(in_windowsPath));
			}
			//------------------------------------------------------------
			//------------------------------------------------------------
			std::string ConvertWindowsFilePathToStandard(const std::wstring& in_windowsPath)
			{
				return CSCore::StringUtils::StandardiseFilePath(UTF16ToUTF8(in_windowsPath));
			}
		}
	}
}

#endif