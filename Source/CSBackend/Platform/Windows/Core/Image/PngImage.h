/*
 *  PngLoader.h
 *  iOSTemplate
 *
 *  Created by Ian Copland on 06/04/2011.
 *  Copyright 2011 Tag Games Ltd. All rights reserved.
 *
 */

#ifdef CS_TARGETPLATFORM_WINDOWS

#ifndef _CHILLISOURCE_WINDOWSPLATFORM_IMAGELOADING_PNGIMAGE_
#define _CHILLISOURCE_WINDOWSPLATFORM_IMAGELOADING_PNGIMAGE_

#include <CSBackend/Platform/Windows/ForwardDeclarations.h>
#include <ChilliSource/Core/File/FileSystem.h>

namespace CSBackend
{
	namespace Windows
	{
		class PngImage
		{
		public:
			//----------------------------------------------------------------------------------
			/// Constructor
			//----------------------------------------------------------------------------------
			PngImage();
			//----------------------------------------------------------------------------------
			/// Constructor
			//----------------------------------------------------------------------------------
			PngImage(CSCore::StorageLocation ineLocation, const std::string& instrFilename);
			//----------------------------------------------------------------------------------
			/// Destructor
			//----------------------------------------------------------------------------------
			virtual ~PngImage();
			//----------------------------------------------------------------------------------
			/// Load PNG
			///
			/// Loads a png from a file within documents or the package.
			/// @param Storgae location of the file
			/// @param std::string instrFilename - the path to the file relative to either
			///									   documents or the package.
			//----------------------------------------------------------------------------------
			void Load(CSCore::StorageLocation ineLocation, const std::string& instrFilename);
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
			//----------------------------------------------------------------------------------
			/// @author S Downie
			///
			/// @return Size of image data in bytes
			//----------------------------------------------------------------------------------
			u32 GetDataSize() const;
			//----------------------------------------------------------------------------------
			/// @author S Downie
			///
			/// @return the image format
			//----------------------------------------------------------------------------------
			CSCore::ImageFormat GetImageFormat() const;
		protected:
			//----------------------------------------------------------------------------------
			/// Load with lib png
			///
			/// Loads the png data using lib png
			/// @param FileStreamSPtr inStream - the stream lib png should use to read the data.
			//----------------------------------------------------------------------------------
			bool LoadWithLibPng(CSCore::FileStreamSPtr inStream);

			bool mbIsLoaded;
			s32 mdwHeight;
			s32 mdwWidth;
			u8 * mpData;
			CSCore::ImageFormat m_format;
			u32 m_dataSize = 0;
		};
	}
}

#endif

#endif
