//
//  WindowwsStringUtils.h
//  ChilliSource
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

#ifndef _CSBACKEND_PLATFORM_WINDOWS_CORE_STRING_WINDOWSSTRINGUTILS_H_
#define _CSBACKEND_PLATFORM_WINDOWS_CORE_STRING_WINDOWSSTRINGUTILS_H_

#include <CSBackend/Platform/Windows/ForwardDeclarations.h>

#include <codecvt>
#include <locale>
#include <string>

namespace CSBackend
{
	namespace Windows
	{
		//------------------------------------------------------------
		/// A collection of utility methods for using strings on windows.
		/// This is mostly methods for working with wstrings.
		//------------------------------------------------------------
		namespace WindowsStringUtils
		{
			//------------------------------------------------------------
			/// Converts from a UTF-8 encoded string to a UTF-16 encoded
			/// wstring.
			///
			/// @author I Copland
			///
			/// @param The UTF-8 string.
			/// @return The UTF-16 wstring.
			//------------------------------------------------------------
			std::wstring UTF8ToUTF16(const std::string& in_utf8String);
			//------------------------------------------------------------
			/// Converts from a UTF-16 encoded wstring to a UTF-8 encoded
			/// string.
			///
			/// @author I Copland
			///
			/// @param The UTF-16 wstring.
			/// @return The UTF-8 string.
			//------------------------------------------------------------
			std::string UTF16ToUTF8(const std::wstring& in_utf16String);
			//------------------------------------------------------------
			/// Converts the given standard string to a windows format
			/// wide string.
			///
			/// @author I Copland
			///
			/// @param The standard path string.
			/// @return The same path in a windows format wide string.
			//------------------------------------------------------------
			std::wstring ConvertStandardPathToWindows(const std::string& in_standardPath);
			//------------------------------------------------------------
			/// Converts the given directory path to windows format.
			///
			/// @author I Copland
			///
			/// @param The windows format wide string.
			/// @return The same path standardised in a string.
			//------------------------------------------------------------
			std::string ConvertWindowsDirectoryPathToStandard(const std::wstring& in_windowsPath);
			//------------------------------------------------------------
			/// Converts the given file path to windows format.
			///
			/// @author S Downie
			///
			/// @param The windows format wide string.
			/// @return The same path standardised in a string.
			//------------------------------------------------------------
			std::string ConvertWindowsFilePathToStandard(const std::wstring& in_windowsPath);
		}
	}
}

#endif

#endif
