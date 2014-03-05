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
			if (inpPng == NULL)
			{
				CS_LOG_ERROR("ReadPngData() has failed because inpPng is NULL :S");
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
			mpData = NULL;
			meFormat = Core::Image::Format::k_RGBA8888;
		}
		//----------------------------------------------------------------------------------
		/// Constructor
		//----------------------------------------------------------------------------------
		PngImage::PngImage(Core::StorageLocation ineStorageLocation, std::string instrFilename)
		{
			mbIsLoaded = false;
			mdwHeight = -1;
			mdwWidth = -1;
			mpData = NULL;
			meFormat = Core::Image::Format::k_RGBA8888;

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
		void PngImage::Load(Core::StorageLocation ineStorageLocation, std::string instrFilename)
		{
			//create the file stream
			ChilliSource::Core::FileStreamSPtr stream = ChilliSource::Core::Application::GetFileSystemPtr()->CreateFileStream(ineStorageLocation, instrFilename, ChilliSource::Core::FileMode::k_readBinary);

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
				mpData = (u8*)NULL;
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
		/// Get Image Format
		//----------------------------------------------------------------------------------
		Core::Image::Format PngImage::GetImageFormat()
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
			png_structp pPng = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
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
				png_destroy_read_struct(&pPng, (png_infopp)NULL, (png_infopp)NULL);
				return false;
			}

			//setup jump
			if (setjmp(png_jmpbuf(pPng)))
			{
				CS_LOG_ERROR("Error while loading PNG.");
				png_destroy_read_struct(&pPng, &pInfo, (png_infopp)NULL);
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
			mpData = new u8[dwRowBytes * udwHeight];

			for (u32 pass = 0; pass < number_of_passes; pass++)
			{
				for (u32 y = 0; y < udwHeight; y += 1)
				{
					png_bytep row = (mpData + y * dwRowBytes);
					png_read_rows(pPng, &row, NULL, 1);
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
				meFormat = Core::Image::Format::k_Lum8;
				break;
			case PNG_COLOR_TYPE_GRAY_ALPHA:
				meFormat = Core::Image::Format::k_LumA88;
				break;
			case PNG_COLOR_TYPE_RGB:
				meFormat = Core::Image::Format::k_RGB888;
				break;
			case PNG_COLOR_TYPE_RGB_ALPHA:
				meFormat = Core::Image::Format::k_RGBA8888;
				break;
			default:
				CS_LOG_ERROR("Trying to load a PNG with an unknown colour format!");
				png_read_end(pPng, NULL);
				png_destroy_read_struct(&pPng, &pInfo, (png_infopp)NULL);
				return false;
				break;
			}

			//end the read
			png_read_end(pPng, NULL);

			//deallocate everything
			png_destroy_read_struct(&pPng, &pInfo, (png_infopp)NULL);

			inStream->Close();

			return true;
		}
		//----------------------------------------------------------------------------------
		/// Convert Format From RGB888 To LUM8
		//----------------------------------------------------------------------------------
		void PngImage::ConvertFormatFromRGB888ToLUM8()
		{
			const u32 udwBytesPerPixel = 2;
			u32 udwArea = mdwWidth * mdwHeight;

			if (meFormat != Core::Image::Format::k_RGB888)
			{
				CS_LOG_ERROR("Trying to convert from RGB888 to LUM8 but current format is not RGB888!");
				return;
			}

			u8* pubyBitmapData8 = (u8*)calloc(udwArea, udwBytesPerPixel);
			PixelRGB888* pPixel24 = (PixelRGB888*)mpData;
			PixelLUM8* pPixel8 = (PixelLUM8*)pubyBitmapData8;

			for(u32 i=0; i<udwArea; ++i, ++pPixel24, ++pPixel8)
			{
				//read the first byte as the greyscale colour
				pPixel8->mbyLum = pPixel24->mbyR;
			}


			CS_SAFEDELETE_ARRAY(mpData);
			mpData = (u8*)pubyBitmapData8;
			meFormat = Core::Image::Format::k_Lum8;
		}
		//----------------------------------------------------------------------------------
		/// Convert Format From RGB8888 To LUMA88
		//----------------------------------------------------------------------------------
		void PngImage::ConvertFormatFromRGB8888ToLUMA88()
		{
			const u32 udwBytesPerPixel = 2;
			u32 udwArea = mdwWidth * mdwHeight;

			if (meFormat != Core::Image::Format::k_RGBA8888)
			{
				CS_LOG_ERROR("Trying to convert from RGBA8888 to LUMA88 but current format is not RGBA8888!");
				return;
			}

			u8* pubyBitmapData88 = (u8*)calloc(udwArea, udwBytesPerPixel);
			PixelRGBA8888* pPixel32 = (PixelRGBA8888*)mpData;
			PixelLUMA88* pPixel16 = (PixelLUMA88*)pubyBitmapData88;

			for(u32 i=0; i<udwArea; ++i, ++pPixel32, ++pPixel16)
			{
				pPixel16->mbyLum = pPixel32->mbyR;
				pPixel16->mbyA = pPixel32->mbyA;
			}

			CS_SAFEDELETE_ARRAY(mpData);
			mpData = (u8*)pubyBitmapData88;
			meFormat = Core::Image::Format::k_LumA88;
		}
		//----------------------------------------------------------------------------------
		/// Convert Format From RGB888 To RGB565
		//----------------------------------------------------------------------------------
		void PngImage::ConvertFormatFromRGB888ToRGB565()
		{
			const u32 udwBytesPerPixel = 2;
			u32 udwArea = mdwWidth * mdwHeight;

			if (meFormat != Core::Image::Format::k_RGB888)
			{
				CS_LOG_ERROR("Trying to convert from RGB888 to RGB565 but current format is not RGB888!");
				return;
			}

			u8* pubyBitmapData565 = (u8*)calloc(udwArea, udwBytesPerPixel);
			PixelRGB888* pPixel24 = (PixelRGB888*)mpData;
			PixelRGB545* pPixel16 = (PixelRGB545*)pubyBitmapData565;

			for(u32 i=0; i<udwArea; ++i, ++pPixel24, ++pPixel16)
			{
				pPixel16->mwData =
						((((pPixel24->mbyR) & 0xFF) >> 3) << 11) |
						((((pPixel24->mbyG) & 0xFF) >> 2) <<  5) |
						((((pPixel24->mbyB) & 0xFF) >> 3) <<  0);
			}

			CS_SAFEDELETE_ARRAY(mpData);
			mpData = (u8*)pubyBitmapData565;
			meFormat = Core::Image::Format::k_RGB565;
		}
		//----------------------------------------------------------------------------------
		/// Convert Format From RGBA8888 To RGBA4444
		//----------------------------------------------------------------------------------
		void PngImage::ConvertFormatFromRGBA8888ToRGBA4444()
		{
			const u32 udwBytesPerPixel = 2;
			u32 udwArea = mdwWidth * mdwHeight;

			if (meFormat != Core::Image::Format::k_RGBA8888)
			{
				CS_LOG_ERROR("Trying to convert from RGBA8888 to RGBA4444 but current format is not RGBA8888!");
				return;
			}

			u8* pubyBitmapData4444 = (u8*)calloc(udwArea, udwBytesPerPixel);
			u32* pPixel32 = (u32*)mpData;
			u16* pPixel16 = (u16*)pubyBitmapData4444;

			for(u32 i=0; i<udwArea; ++i, ++pPixel32)
			{
				*pPixel16++ =
				((((*pPixel32 >> 0) & 0xFF) >> 4) << 12) | // R
				((((*pPixel32 >> 8) & 0xFF) >> 4) << 8) | // G
				((((*pPixel32 >> 16) & 0xFF) >> 4) << 4) | // B
				((((*pPixel32 >> 24) & 0xFF) >> 4) << 0); // A
			}

			CS_SAFEDELETE_ARRAY(mpData);
			mpData = (u8*)pubyBitmapData4444;
			meFormat = Core::Image::Format::k_RGBA4444;
		}
	}
}
