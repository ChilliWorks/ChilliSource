//
//  WindowwsStringUtils.cpp
//  Chilli Source
//  Created by Ian Copland on 03/03/2011.
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
				return ChilliSource::StringUtils::StandardiseDirectoryPath(UTF16ToUTF8(in_windowsPath));
			}
			//------------------------------------------------------------
			//------------------------------------------------------------
			std::string ConvertWindowsFilePathToStandard(const std::wstring& in_windowsPath)
			{
				return ChilliSource::StringUtils::StandardiseFilePath(UTF16ToUTF8(in_windowsPath));
			}
		}
	}
}

#endif