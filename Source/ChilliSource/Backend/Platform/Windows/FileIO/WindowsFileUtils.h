//
//  WindowsFileUtils.h
//  Chilli Source
//
//  Created by Ian Copland on 28/02/2014.
//  Copyright 2014 Tag Games. All rights reserved.
//

#ifndef _CHILLISOURCE_BACKEND_PLATFORM_WINDOWS_FILE_WINDOWSFILEUTILS_H_
#define _CHILLISOURCE_BACKEND_PLATFORM_WINDOWS_FILE_WINDOWSFILEUTILS_H_

#include <windows.h>

namespace ChilliSource
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
			/// @author I Copland
			///
			/// @param the directory to create.
			/// @param The security attributes. Can be null.
			/// @return Whether or not the directory was created.
			//--------------------------------------------------------
			BOOL WindowsCreateDirectory(LPCTSTR in_directory, LPSECURITY_ATTRIBUTES in_securityAttributes);
			//--------------------------------------------------------
			/// Copies a file using the windows API.
			///
			/// @author I Copland
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
			/// @author I Copland
			///
			/// @param the filename to delete.
			/// @return Whether or not the file was deleted.
			//--------------------------------------------------------
			BOOL WindowsDeleteFile(LPCTSTR in_filename);
			//--------------------------------------------------------
			/// Deletes a directory using the windows API.
			///
			/// @author I Copland
			///
			/// @param the directory to delete.
			/// @return Whether or not the directory was deleted.
			//--------------------------------------------------------
			BOOL WindowsRemoveDirectory(LPCTSTR in_directory);
		}
	}
}

#endif