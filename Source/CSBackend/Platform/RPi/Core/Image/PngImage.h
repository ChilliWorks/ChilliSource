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

#ifdef CS_TARGETPLATFORM_RPI

#ifndef _CSBACKEND_PLATFORM_RPI_CORE_IMAGE_PNGIMAGE_H_
#define _CSBACKEND_PLATFORM_RPI_CORE_IMAGE_PNGIMAGE_H_

#include <CSBackend/Platform/RPi/ForwardDeclarations.h>
#include <ChilliSource/Core/File/FileSystem.h>

namespace CSBackend
{
	namespace RPi
	{
		/// Wrapper around libpng to load png images
		///
		class PngImage final
		{
		public:

			///
			PngImage();

			/// Loads the image
			///
			///@param location
			///		Location to load from
			/// @param filePath
			///		File path to load from
			///
			PngImage(ChilliSource::StorageLocation location, const std::string& filePath);

			///
			~PngImage();

			/// Loads the image
			///
			/// @param location
			///		Location to load from
			/// @param filePath
			///		File path to load from
			///
			void Load(ChilliSource::StorageLocation location, const std::string& filePath);

			/// Destroy the image data. If the data has already been destroyed then pass
			/// false to reset.
			///
			/// @param releaseData
			///		TRUE to destroy the data, FALSE to simply reset
			///
			void Release(bool releaseData = true);

			/// @return TRUE if loaded
			///
			bool IsLoaded() const;

			/// @return Height of the image in pixels
			///
			s32 GetHeight() const;

			/// @return Width of the image in pixels
			///
			s32 GetWidth() const;

			/// @return Image data
			///
			u8 * GetImageData() const;

			/// @return Size of the image data in bytes
			///
			u32 GetDataSize() const;

			/// @retrun Format of the image
			///
			ChilliSource::ImageFormat GetImageFormat() const;

		protected:
			/// Load with lib png
			/// @param stream
			///		The stream lib png should use to read the data.
			///
			/// @return TRUE if successful
			///
			bool LoadWithLibPng(const ChilliSource::IBinaryInputStreamUPtr& stream);

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
