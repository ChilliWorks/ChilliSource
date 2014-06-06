//
//  WindowsFileUtils.cpp
//  Chilli Source
//
//  Created by Ian Copland on 28/02/2014.
//  Copyright 2014 Tag Games. All rights reserved.
//

#ifdef CS_TARGETPLATFORM_WINDOWS

#include <CSBackend/Platform/Windows/Core/File/WindowsFileUtils.h>

namespace ChilliSource
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