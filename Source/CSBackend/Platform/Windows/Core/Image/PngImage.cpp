//
//  PngLoader.cpp
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

#include <CSBackend/Platform/Windows/Core/Image/PngImage.h>

#include <png/png.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Image/ImageFormat.h>

//----------------------------------------------------------------------------------
/// Read Png Data
///
/// A replacement for the default libPng file reading function. This is needed so
/// the c style file io functions can be replaced with Chilli source functions,
/// enabling loading from the package.
/// @param png_structp png_ptr - The currently open Png decorder
/// @param png_bytep data - The output data.
/// @param png_size_t length - The length of the data.
//----------------------------------------------------------------------------------
void ReadPngData(png_structp png_ptr, png_bytep data, png_size_t length)
{
	if (png_ptr == nullptr)
		return;

	ChilliSource::FileStream* pStream = (ChilliSource::FileStream*)png_get_io_ptr(png_ptr);
	pStream->Read(reinterpret_cast<s8*>(data), s32(length));
}

namespace CSBackend
{
	namespace Windows
	{
		//----------------------------------------------------------------------------------
		/// Constructor
		//----------------------------------------------------------------------------------
		PngImage::PngImage()
		{
			mbIsLoaded = false;
			mdwHeight = -1;
			mdwWidth = -1;
			mpData = nullptr;
		}
		//----------------------------------------------------------------------------------
		/// Constructor
		//----------------------------------------------------------------------------------
		PngImage::PngImage(ChilliSource::StorageLocation ineLocation, const std::string& instrFilename)
		{
			mbIsLoaded = false;
			mdwHeight = -1;
			mdwWidth = -1;
			mpData = nullptr;

			Load(ineLocation, instrFilename);
		}
		//----------------------------------------------------------------------------------
		/// Destructor
		//----------------------------------------------------------------------------------
		PngImage::~PngImage()
		{
			if (mbIsLoaded == true)
			{
				Release();
			}
		}
		//----------------------------------------------------------------------------------
		/// Load PNG
		///
		/// Loads a png from a file within documents or the package.
		/// @param Storgae location of the file
		/// @param std::string instrFilename - the path to the file relative to either
		///									   documents or the package.
		//----------------------------------------------------------------------------------
		void PngImage::Load(ChilliSource::StorageLocation ineLocation, const std::string& instrFilename)
		{
			//create the file stream
			ChilliSource::FileStreamSPtr stream = ChilliSource::Application::Get()->GetFileSystem()->CreateFileStream(ineLocation, instrFilename, ChilliSource::FileMode::k_readBinary);

			//insure the stream is not broken
			if (stream == nullptr)
			{
				return;
			}

			//load from lib png
			if (LoadWithLibPng(stream) == true)
			{
				mbIsLoaded = true;
			}
		}
		//----------------------------------------------------------------------------------
		/// Release
		///
		/// Releases the image data
		/// @param bool inReleaseImageData - Whether or not to release the image data
		//----------------------------------------------------------------------------------
		void PngImage::Release(bool inReleaseImageData)
		{
			if (mbIsLoaded != false)
			{
				mbIsLoaded = false;
				mdwHeight = -1;
				mdwWidth = -1;
				if (inReleaseImageData)
					delete[] mpData;
				mpData = (u8*)nullptr;
			}
		}
		//----------------------------------------------------------------------------------
		/// Is Loaded
		///
		/// returns whether or not the image is loaded
		//----------------------------------------------------------------------------------
		bool PngImage::IsLoaded()
		{
			return mbIsLoaded;
		}
		//----------------------------------------------------------------------------------
		/// Get Height
		///
		/// returns the height of the image
		//----------------------------------------------------------------------------------
		s32 PngImage::GetHeight()
		{
			return mdwHeight;
		}
		//----------------------------------------------------------------------------------
		/// Get Width
		///
		/// returns the width of the image
		//----------------------------------------------------------------------------------
		s32 PngImage::GetWidth()
		{
			return mdwWidth;
		}
		//----------------------------------------------------------------------------------
		/// Get Image Data
		///
		/// returns the image data.
		//----------------------------------------------------------------------------------
		u8 * PngImage::GetImageData()
		{
			return mpData;
		}
		//----------------------------------------------------------------------------------
		//----------------------------------------------------------------------------------
		u32 PngImage::GetDataSize() const
		{
			return m_dataSize;
		}
		//----------------------------------------------------------------------------------
		//----------------------------------------------------------------------------------
		ChilliSource::ImageFormat PngImage::GetImageFormat() const
		{
			return m_format;
		}
		//----------------------------------------------------------------------------------
		/// Load with lib png
		///
		/// Loads the png data using lib png
		/// @param FileStreamSPtr inStream - the steam lib png should use to read the data.
		//----------------------------------------------------------------------------------
		bool PngImage::LoadWithLibPng(ChilliSource::FileStreamSPtr inStream)
		{
			//insure that it is indeed a png
			const s32 dwHeaderSize = 8;
			u8 ubyHeader[dwHeaderSize];
			inStream->Read((s8*)ubyHeader, dwHeaderSize);

			if (png_sig_cmp(ubyHeader, 0, dwHeaderSize) > 0)
			{
				CS_LOG_ERROR("PNG header invalid.");
				return false;
			}

			//create the png structure
			png_structp pPng = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
			if (!pPng)
			{
				CS_LOG_ERROR("Could not create pPng");
				return false;
			}

			//create the png info
			png_infop pInfo = png_create_info_struct(pPng);
			if (!pInfo)
			{
				CS_LOG_ERROR("Could not create pInfo");
				png_destroy_read_struct(&pPng, (png_infopp)nullptr, (png_infopp)nullptr);
				return false;
			}

			//setup jump
			if (setjmp(png_jmpbuf(pPng)))
			{
				CS_LOG_ERROR("Error while loading PNG.");
				png_destroy_read_struct(&pPng, &pInfo, (png_infopp)nullptr);
				return false;
			}

			//Setup the ReadPngData function for use within libPng
			png_set_read_fn(pPng,(void*)inStream.get(), ReadPngData);

			//tell it that we've ready read 8 bytes of data
			png_set_sig_bytes(pPng, dwHeaderSize);

			//read the chunks of data upto the actual image data
			png_read_info(pPng, pInfo);

			//get the info thats just been read
			png_uint_32 udwWidth = 0;
			png_uint_32 udwHeight = 0;
			s32 dwBitDepth = -1;
			s32 dwColorType = -1;
			s32 dwInterlaceType = -1;
			s32 dwCompressionMethod = -1;
			s32 dwFilterMethod = -1;
			png_get_IHDR(pPng, pInfo, &udwWidth, &udwHeight, &dwBitDepth, &dwColorType, &dwInterlaceType, &dwCompressionMethod, &dwFilterMethod);

			//--transformations
			//sorts out grey scale images, probably not needed, but put in anyway
			if (dwColorType == PNG_COLOR_TYPE_PALETTE) png_set_palette_to_rgb(pPng);
			if (dwColorType == PNG_COLOR_TYPE_GRAY && dwBitDepth < 8) png_set_expand_gray_1_2_4_to_8(pPng);
			if (png_get_valid(pPng, pInfo, PNG_INFO_tRNS)) png_set_tRNS_to_alpha(pPng);

			//insure all files use 1 bit each for R,G,B and A
			if (dwBitDepth == 16) png_set_strip_16(pPng);

			//unpack compressed pixels
			if (dwBitDepth < 8) png_set_packing(pPng);

			//pack in an alpha value if there isn't one.
			if (dwColorType == PNG_COLOR_TYPE_RGB || dwColorType == PNG_COLOR_TYPE_GRAY)
				png_set_add_alpha(pPng, (u8)255, PNG_FILLER_AFTER);

			//for the sake of simplicity, turn grey scale images into RGB
			if (dwColorType == PNG_COLOR_TYPE_GRAY || dwColorType == PNG_COLOR_TYPE_GRAY_ALPHA)
				png_set_gray_to_rgb(pPng);

			//set to little endianness
			if (dwBitDepth == 16)
				png_set_swap(pPng);

			//tell libpng to handling interlacing
			s32 number_of_passes = png_set_interlace_handling(pPng);

			//after all these transformations, update the png info
			png_read_update_info(pPng, pInfo);

			//read the image into the data buffer
			s32 dwRowBytes = s32(png_get_rowbytes(pPng, pInfo));
			m_dataSize = dwRowBytes * udwHeight;
			mpData = new u8[m_dataSize];

			for (s32 pass = 0; pass < number_of_passes; pass++)
			{
				for (u32 y = 0; y < udwHeight; y += 1)
				{
					png_bytep row = (mpData + y * dwRowBytes);
					png_read_rows(pPng, &row, nullptr, 1);
				}
			}

			//store the width and height
			mdwWidth = (s32)udwWidth;
			mdwHeight = (s32)udwHeight;

			//-------- Get the image format
			png_get_IHDR(pPng, pInfo, &udwWidth, &udwHeight, &dwBitDepth, &dwColorType, &dwInterlaceType, &dwCompressionMethod, &dwFilterMethod);
			switch (dwColorType)
			{
			case PNG_COLOR_TYPE_GRAY:
				m_format = ChilliSource::ImageFormat::k_Lum8;
				break;
			case PNG_COLOR_TYPE_GRAY_ALPHA:
				m_format = ChilliSource::ImageFormat::k_LumA88;
				break;
			case PNG_COLOR_TYPE_RGB:
				m_format = ChilliSource::ImageFormat::k_RGB888;
				break;
			case PNG_COLOR_TYPE_RGB_ALPHA:
				m_format = ChilliSource::ImageFormat::k_RGBA8888;
				break;
			default:
				CS_LOG_ERROR("Trying to load a PNG with an unknown colour format!");
				png_read_end(pPng, nullptr);
				png_destroy_read_struct(&pPng, &pInfo, (png_infopp)nullptr);
				return false;
				break;
			}

			//end the read
			png_read_end(pPng, nullptr);

			//deallocate everything
			png_destroy_read_struct(&pPng, &pInfo, (png_infopp)nullptr);
			return true;
		}
	}
}

#endif
