//
//  WindowsFileUtils.h
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

#ifndef _CSBACKEND_PLATFORM_WINDOWS_FILE_WINDOWSFILEUTILS_H_
#define _CSBACKEND_PLATFORM_WINDOWS_FILE_WINDOWSFILEUTILS_H_

#include <CSBackend/Platform/Windows/ForwardDeclarations.h>

#include <windows.h>

namespace CSBackend
{
	namespace Windows
	{
		//----------------------------------------------------------------
		/// A wrapper for windows specfic file system functionality. This
		/// is required becuase windows has defined macros named similarly
		/// to methods in the engine, meaning we need to undefine them. To
		/// use them we then need to call though this wrapper.
		//----------------------------------------------------------------
		namespace WindowsFileUtils
		{
			//--------------------------------------------------------
			/// Creates a new directory using the windows API.
			///
			/// @author Ian Copland
			///
			/// @param the directory to create.
			/// @param The security attributes. Can be null.
			/// @return Whether or not the directory was created.
			//--------------------------------------------------------
			BOOL WindowsCreateDirectory(LPCTSTR in_directory, LPSECURITY_ATTRIBUTES in_securityAttributes);
			//--------------------------------------------------------
			/// Copies a file using the windows API.
			///
			/// @author Ian Copland
			///
			/// @param the source location
			/// @param The destination location.
			/// @param Whether or not to fail if the file already
			/// exists.
			/// @return Whether or not the copy failed.
			//--------------------------------------------------------
			BOOL WindowsCopyFile(LPCTSTR in_sourceFilename, LPCTSTR in_destinationFilename, BOOL in_failIfExists);
			//--------------------------------------------------------
			/// Deletes a file using the windows API.
			///
			/// @author Ian Copland
			///
			/// @param the filename to delete.
			/// @return Whether or not the file was deleted.
			//--------------------------------------------------------
			BOOL WindowsDeleteFile(LPCTSTR in_filename);
			//--------------------------------------------------------
			/// Deletes a directory using the windows API.
			///
			/// @author Ian Copland
			///
			/// @param the directory to delete.
			/// @return Whether or not the directory was deleted.
			//--------------------------------------------------------
			BOOL WindowsRemoveDirectory(LPCTSTR in_directory);
			//--------------------------------------------------------
			/// Firsts the first file in the given directory. The
			/// directory can contain wildcard characters (* or ?).
			///
			/// @author Ian Copland
			///
			/// @param the directory to find.
			/// @param Out: Data on the found file.
			/// @return A handle to the first found file.
			//--------------------------------------------------------
			HANDLE WindowsFindFirstFile(LPCTSTR in_directory, LPWIN32_FIND_DATA in_findFileData);
			//--------------------------------------------------------
			/// Continues a file seach started using FileFirstFile. The
			/// directory can contain wildcard characters (* or ?).
			///
			/// @author Ian Copland
			///
			/// @param the directory to find.
			/// @param Out: Data on the found file.
			/// @return A handle to the first found file.
			//--------------------------------------------------------
			BOOL WindowsFindNextFile(HANDLE in_previousFile, LPWIN32_FIND_DATA in_findFileData);
		}
	}
}

#endif

#endif
