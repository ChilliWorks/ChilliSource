//
//  PngLoader.h
//  Chilli Source
//  Created by Ian Copland on 06/04/2011.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2011 Tag Games Limited
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
			PngImage(ChilliSource::StorageLocation ineLocation, const std::string& instrFilename);
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
			void Load(ChilliSource::StorageLocation ineLocation, const std::string& instrFilename);
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
			ChilliSource::ImageFormat GetImageFormat() const;
		protected:
			//----------------------------------------------------------------------------------
			/// Load with lib png
			///
			/// Loads the png data using lib png
			/// @param FileStreamSPtr inStream - the stream lib png should use to read the data.
			//----------------------------------------------------------------------------------
			bool LoadWithLibPng(ChilliSource::FileStreamSPtr inStream);

			bool mbIsLoaded;
			s32 mdwHeight;
			s32 mdwWidth;
			u8 * mpData;
			ChilliSource::ImageFormat m_format;
			u32 m_dataSize = 0;
		};
	}
}

#endif

#endif
