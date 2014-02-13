/*
 *  PngLoader.h
 *  iOSTemplate
 *
 *  Created by Ian Copland on 06/04/2011.
 *  Copyright 2011 Tag Games Ltd. All rights reserved.
 *
 */

#ifndef _MOFLO_WINDOWSPLATFORM_IMAGELOADING_PNGIMAGE_
#define _MOFLO_WINDOWSPLATFORM_IMAGELOADING_PNGIMAGE_

#include <ChilliSource/Core/FileIO/FileSystem.h>

namespace moFlo
{
	namespace WindowsPlatform
	{
		class CPngImage
		{
		public:
			//----------------------------------------------------------------------------------
			/// Constructor
			//----------------------------------------------------------------------------------
			CPngImage();
			//----------------------------------------------------------------------------------
			/// Constructor
			//----------------------------------------------------------------------------------
			CPngImage(Core::STORAGE_LOCATION ineLocation, const std::string& instrFilename);
			//----------------------------------------------------------------------------------
			/// Destructor
			//----------------------------------------------------------------------------------
			virtual ~CPngImage();
			//----------------------------------------------------------------------------------
			/// Load PNG
			///
			/// Loads a png from a file within documents or the package.
			/// @param Storgae location of the file
			/// @param std::string instrFilename - the path to the file relative to either
			///									   documents or the package.
			//----------------------------------------------------------------------------------
			void Load(Core::STORAGE_LOCATION ineLocation, const std::string& instrFilename);
			//----------------------------------------------------------------------------------
			/// Release
			///
			/// Releases the image data
			/// @param bool inReleaseImageData - Whether or not to release the image data
			//----------------------------------------------------------------------------------
			void Release(bool inReleaseImageData = true);
			//----------------------------------------------------------------------------------
			/// Is Loaded
			///
			/// returns whether or not the image is loaded
			//----------------------------------------------------------------------------------
			bool IsLoaded();
			//----------------------------------------------------------------------------------
			/// Get Height
			///
			/// returns the height of the image
			//----------------------------------------------------------------------------------
			s32 GetHeight();
			//----------------------------------------------------------------------------------
			/// Get Width
			///
			/// returns the width of the image
			//----------------------------------------------------------------------------------
			s32 GetWidth();
			//----------------------------------------------------------------------------------
			/// Get Image Data
			///
			/// returns the image data.
			//----------------------------------------------------------------------------------
			u8 * GetImageData();
		protected:
			//----------------------------------------------------------------------------------
			/// Load with lib png
			///
			/// Loads the png data using lib png
			/// @param FileStreamPtr inStream - the stream lib png should use to read the data.
			//----------------------------------------------------------------------------------
			bool LoadWithLibPng(Core::FileStreamPtr inStream);

			bool mbIsLoaded;
			s32 mdwHeight;
			s32 mdwWidth;
			u8 * mpData;
		};
	}
}

#endif
