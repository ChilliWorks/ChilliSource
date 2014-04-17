/*
 *  PngLoader.h
 *  iOSTemplate
 *
 *  Created by Ian Copland on 06/04/2011.
 *  Copyright 2011 Tag Games Ltd. All rights reserved.
 *
 */

#include <ChilliSource/Backend/Platform/Android/Core/Image/PngImage.h>

#include <ChilliSource/Backend/Platform/Android/Core/Image/LibPng/png.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Image/ImageFormat.h>

namespace ChilliSource
{
	namespace Android
	{
		//----------------------------------------------------------------------------------
		/// Read Png Data
		///
		/// A replacement for the default libPng file reading function. This is needed so
		/// the c style file io functions can be replaced with moFlo functions, enabling
		/// loading from the package.
		///
		/// @param The currently open Png decorder
		/// @param The output data.
		/// @param The length of the data.
		//----------------------------------------------------------------------------------
		void ReadPngData(png_structp inpPng, png_bytep inpData, png_size_t indwLength)
		{
			if (inpPng == nullptr)
			{
				CS_LOG_ERROR("ReadPngData() has failed because inpPng is nullptr :S");
				return;
			}

			ChilliSource::Core::FileStream* pStream = (ChilliSource::Core::FileStream*)png_get_io_ptr(inpPng);

			if (pStream->IsBad() == true || pStream->IsOpen() == false)
			{
				CS_LOG_ERROR("ReadPngData() has failed due to a problem with the filestream.");
				png_error(inpPng, "Read Error");
			}

			pStream->Read((s8*)inpData, indwLength);
		}
		//----------------------------------------------------------------------------------
		/// Constructor
		//----------------------------------------------------------------------------------
		PngImage::PngImage()
		{
			mbIsLoaded = false;
			mdwHeight = -1;
			mdwWidth = -1;
			mpData = nullptr;
			meFormat = Core::ImageFormat::k_RGBA8888;
		}
		//----------------------------------------------------------------------------------
		/// Constructor
		//----------------------------------------------------------------------------------
		PngImage::PngImage(Core::StorageLocation ineStorageLocation, const std::string& instrFilename)
		{
			mbIsLoaded = false;
			mdwHeight = -1;
			mdwWidth = -1;
			mpData = nullptr;
			meFormat = Core::ImageFormat::k_RGBA8888;

			Load(ineStorageLocation, instrFilename);
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
		/// Load
		//----------------------------------------------------------------------------------
		void PngImage::Load(Core::StorageLocation ineStorageLocation, const std::string& instrFilename)
		{
			//create the file stream
			ChilliSource::Core::FileStreamSPtr stream = ChilliSource::Core::Application::Get()->GetFileSystem()->CreateFileStream(ineStorageLocation, instrFilename, ChilliSource::Core::FileMode::k_readBinary);

			//insure the stream is not broken
			if (stream == ChilliSource::Core::FileStreamSPtr() || stream->IsBad() == true || stream->IsOpen() == false)
			{
				stream->Close();
				return;
			}

			//load from lib png
			if (LoadWithLibPng(stream) == true)
			{
				mbIsLoaded = true;
			}

			//close the stream
			stream->Close();
		}
		//----------------------------------------------------------------------------------
		/// Release
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
		//----------------------------------------------------------------------------------
		bool PngImage::IsLoaded()
		{
			return mbIsLoaded;
		}
		//----------------------------------------------------------------------------------
		/// Get Height
		//----------------------------------------------------------------------------------
		s32 PngImage::GetHeight()
		{
			return mdwHeight;
		}
		//----------------------------------------------------------------------------------
		/// Get Width
		//----------------------------------------------------------------------------------
		s32 PngImage::GetWidth()
		{
			return mdwWidth;
		}
		//----------------------------------------------------------------------------------
		/// Get Image Data
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
		/// Get Image Format
		//----------------------------------------------------------------------------------
		Core::ImageFormat PngImage::GetImageFormat()
		{
			return meFormat;
		}
		//----------------------------------------------------------------------------------
		/// Load with lib png
		//----------------------------------------------------------------------------------
		bool PngImage::LoadWithLibPng(Core::FileStreamSPtr inStream)
		{
			//-------- Intialisation
			//read the header to insure it is indeed a png
			s32 dwHeaderSize = 8;
			u8 * ubyHeader = new u8[dwHeaderSize];
			inStream->Read((s8*)ubyHeader, dwHeaderSize);
			bool bIsPng = !png_sig_cmp(ubyHeader, 0, dwHeaderSize);
			delete[] ubyHeader;

			//if its not a PNG return.
			if (bIsPng == false)
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
			int dwBitDepth = -1;
			int dwColorType = -1;
			int dwInterlaceType = -1;
			int dwCompressionMethod = -1;
			int dwFilterMethod = -1;
			png_get_IHDR(pPng, pInfo, &udwWidth, &udwHeight, &dwBitDepth, &dwColorType, &dwInterlaceType, &dwCompressionMethod, &dwFilterMethod);

			//-------- Image Transformations
			//turn palleted pngs into RGB
			if (dwColorType == PNG_COLOR_TYPE_PALETTE)
				png_set_palette_to_rgb(pPng);

			//force greyscale images to 8 bit
			if (dwColorType == PNG_COLOR_TYPE_GRAY && dwBitDepth < 8)
				png_set_expand_gray_1_2_4_to_8(pPng);

			//if there is compressed alpha data, expand into a full alpha channel
			if (png_get_valid(pPng, pInfo, PNG_INFO_tRNS)) png_set_tRNS_to_alpha(pPng);

			//strip 16bit per channel images down to 8 bit per channel (Note the possibilty of 16bit per channel images not working due to pngs using big endianness. This can
			//be fixed by using png_set_swap())
			if (dwBitDepth == 16)
				png_set_strip_16(pPng);

			//insure bit depth is always 8.
			if (dwBitDepth < 8) png_set_packing(pPng);

			//-------- Read Image
			//tell libpng to handling interlacing
			int number_of_passes = png_set_interlace_handling(pPng);

			//after all these transformations, update the png info
			png_read_update_info(pPng, pInfo);

			//read the image into the data buffer
			int dwRowBytes = png_get_rowbytes(pPng, pInfo);
			m_dataSize = dwRowBytes * udwHeight;
			mpData = new u8[m_dataSize];

			for (u32 pass = 0; pass < number_of_passes; pass++)
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
				meFormat = Core::ImageFormat::k_Lum8;
				break;
			case PNG_COLOR_TYPE_GRAY_ALPHA:
				meFormat = Core::ImageFormat::k_LumA88;
				break;
			case PNG_COLOR_TYPE_RGB:
				meFormat = Core::ImageFormat::k_RGB888;
				break;
			case PNG_COLOR_TYPE_RGB_ALPHA:
				meFormat = Core::ImageFormat::k_RGBA8888;
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

			inStream->Close();

			return true;
		}
	}
}
