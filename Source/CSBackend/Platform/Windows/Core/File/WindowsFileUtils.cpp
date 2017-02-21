//
//  WindowsFileUtils.cpp
//  ChilliSource
//  Created by Ian Copland on 28/02/2014.
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

#include <CSBackend/Platform/Windows/Core/File/WindowsFileUtils.h>

namespace CSBackend
{
	namespace Windows
	{
		namespace WindowsFileUtils
		{
			//--------------------------------------------------------
			//--------------------------------------------------------
			BOOL WindowsCreateDirectory(LPCTSTR in_directory, LPSECURITY_ATTRIBUTES in_securityAttributes)
			{
				return CreateDirectory(in_directory, in_securityAttributes);
			}
			//--------------------------------------------------------
			//--------------------------------------------------------
			BOOL WindowsCopyFile(LPCTSTR in_sourceFilename, LPCTSTR in_destinationFilename, BOOL in_failIfExists)
			{
				return CopyFile(in_sourceFilename, in_destinationFilename, in_failIfExists);
			}
			//--------------------------------------------------------
			//--------------------------------------------------------
			BOOL WindowsDeleteFile(LPCTSTR in_filename)
			{
				return DeleteFile(in_filename);
			}
			//--------------------------------------------------------
			//--------------------------------------------------------
			BOOL WindowsRemoveDirectory(LPCTSTR in_directory)
			{
				return RemoveDirectory(in_directory);
			}
			//--------------------------------------------------------
			//--------------------------------------------------------
			HANDLE WindowsFindFirstFile(LPCTSTR in_directory, LPWIN32_FIND_DATA in_findFileData)
			{
				return FindFirstFile(in_directory, in_findFileData);
			}
			//--------------------------------------------------------
			//--------------------------------------------------------
			BOOL WindowsFindNextFile(HANDLE in_previousFile, LPWIN32_FIND_DATA in_findFileData)
			{
				return FindNextFile(in_previousFile, in_findFileData);
			}
		}
	}
}

#endif
