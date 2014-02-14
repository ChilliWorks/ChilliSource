/*
 *  PngLoader.h
 *  iOSTemplate
 *
 *  Created by Ian Copland on 06/04/2011.
 *  Copyright 2011 Tag Games Ltd. All rights reserved.
 *
 */

#include <ChilliSource/Platform/Android/ImageLoading/PngImage.h>
#include <ChilliSource/Platform/Android/ImageLoading/LibPng/png.h>
#include <ChilliSource/Core/Main/Application.h>

namespace moFlo
{
	namespace AndroidPlatform
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
				ERROR_LOG("ReadPngData() has failed because inpPng is NULL :S");
				return;
			}

			moFlo::Core::IFileStream* pStream = (moFlo::Core::IFileStream*)png_get_io_ptr(inpPng);

			if (pStream->IsBad() == true || pStream->IsOpen() == false)
			{
				ERROR_LOG("ReadPngData() has failed due to a problem with the filestream.");
				png_error(inpPng, "Read Error");
			}

			pStream->Read((s8*)inpData, indwLength);
		}
		//----------------------------------------------------------------------------------
		/// Constructor
		//----------------------------------------------------------------------------------
		CPngImage::CPngImage()
		{
			mbIsLoaded = false;
			mdwHeight = -1;
			mdwWidth = -1;
			mpData = NULL;
			meFormat = Core::CImage::RGBA_8888;
		}
		//----------------------------------------------------------------------------------
		/// Constructor
		//----------------------------------------------------------------------------------
		CPngImage::CPngImage(Core::STORAGE_LOCATION ineStorageLocation, std::string instrFilename)
		{
			mbIsLoaded = false;
			mdwHeight = -1;
			mdwWidth = -1;
			mpData = NULL;
			meFormat = Core::CImage::RGBA_8888;

			Load(ineStorageLocation, instrFilename);
		}
		//----------------------------------------------------------------------------------
		/// Destructor
		//----------------------------------------------------------------------------------
		CPngImage::~CPngImage()
		{
			if (mbIsLoaded == true)
			{
				Release();
			}
		}
		//----------------------------------------------------------------------------------
		/// Load
		//----------------------------------------------------------------------------------
		void CPngImage::Load(Core::STORAGE_LOCATION ineStorageLocation, std::string instrFilename)
		{
			//create the file stream
			moFlo::Core::FileStreamPtr stream = moFlo::Core::CApplication::GetFileSystemPtr()->CreateFileStream(ineStorageLocation, instrFilename, moFlo::Core::FM_READ_BINARY);

			//insure the stream is not broken
			if (stream == moFlo::Core::FileStreamPtr() || stream->IsBad() == true || stream->IsOpen() == false)
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
		void CPngImage::Release(bool inReleaseImageData)
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
		bool CPngImage::IsLoaded()
		{
			return mbIsLoaded;
		}
		//----------------------------------------------------------------------------------
		/// Get Height
		//----------------------------------------------------------------------------------
		s32 CPngImage::GetHeight()
		{
			return mdwHeight;
		}
		//----------------------------------------------------------------------------------
		/// Get Width
		//----------------------------------------------------------------------------------
		s32 CPngImage::GetWidth()
		{
			return mdwWidth;
		}
		//----------------------------------------------------------------------------------
		/// Get Image Data
		//----------------------------------------------------------------------------------
		u8 * CPngImage::GetImageData()
		{
			return mpData;
		}
		//----------------------------------------------------------------------------------
		/// Get Image Format
		//----------------------------------------------------------------------------------
		Core::CImage::Format CPngImage::GetImageFormat()
		{
			return meFormat;
		}
		//----------------------------------------------------------------------------------
		/// Load with lib png
		//----------------------------------------------------------------------------------
		bool CPngImage::LoadWithLibPng(Core::FileStreamPtr inStream)
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
				ERROR_LOG("PNG header invalid.");
				return false;
			}

			//create the png structure
			png_structp pPng = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
			if (!pPng)
			{
				ERROR_LOG("Could not create pPng");
				return false;
			}

			//create the png info
			png_infop pInfo = png_create_info_struct(pPng);
			if (!pInfo)
			{
				ERROR_LOG("Could not create pInfo");
				png_destroy_read_struct(&pPng, (png_infopp)NULL, (png_infopp)NULL);
				return false;
			}

			//setup jump
			if (setjmp(png_jmpbuf(pPng)))
			{
				ERROR_LOG("Error while loading PNG.");
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
				meFormat = Core::CImage::LUM_8;
				break;
			case PNG_COLOR_TYPE_GRAY_ALPHA:
				meFormat = Core::CImage::LUMA_88;
				break;
			case PNG_COLOR_TYPE_RGB:
				meFormat = Core::CImage::RGB_888;
				break;
			case PNG_COLOR_TYPE_RGB_ALPHA:
				meFormat = Core::CImage::RGBA_8888;
				break;
			default:
				ERROR_LOG("Trying to load a PNG with an unknown colour format!");
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
		void CPngImage::ConvertFormatFromRGB888ToLUM8()
		{
			const u32 udwBytesPerPixel = 2;
			u32 udwArea = mdwWidth * mdwHeight;

			if (meFormat != Core::CImage::RGB_888)
			{
				ERROR_LOG("Trying to convert from RGB888 to LUM8 but current format is not RGB888!");
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


			SAFE_DELETE_ARRAY(mpData);
			mpData = (u8*)pubyBitmapData8;
			meFormat = Core::CImage::LUM_8;
		}
		//----------------------------------------------------------------------------------
		/// Convert Format From RGB8888 To LUMA88
		//----------------------------------------------------------------------------------
		void CPngImage::ConvertFormatFromRGB8888ToLUMA88()
		{
			const u32 udwBytesPerPixel = 2;
			u32 udwArea = mdwWidth * mdwHeight;

			if (meFormat != Core::CImage::RGBA_8888)
			{
				ERROR_LOG("Trying to convert from RGBA8888 to LUMA88 but current format is not RGBA8888!");
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

			SAFE_DELETE_ARRAY(mpData);
			mpData = (u8*)pubyBitmapData88;
			meFormat = Core::CImage::LUMA_88;
		}
		//----------------------------------------------------------------------------------
		/// Convert Format From RGB888 To RGB565
		//----------------------------------------------------------------------------------
		void CPngImage::ConvertFormatFromRGB888ToRGB565()
		{
			const u32 udwBytesPerPixel = 2;
			u32 udwArea = mdwWidth * mdwHeight;

			if (meFormat != Core::CImage::RGB_888)
			{
				ERROR_LOG("Trying to convert from RGB888 to RGB565 but current format is not RGB888!");
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

			SAFE_DELETE_ARRAY(mpData);
			mpData = (u8*)pubyBitmapData565;
			meFormat = Core::CImage::RGB_565;
		}
		//----------------------------------------------------------------------------------
		/// Convert Format From RGBA8888 To RGBA4444
		//----------------------------------------------------------------------------------
		void CPngImage::ConvertFormatFromRGBA8888ToRGBA4444()
		{
			const u32 udwBytesPerPixel = 2;
			u32 udwArea = mdwWidth * mdwHeight;

			if (meFormat != Core::CImage::RGBA_8888)
			{
				ERROR_LOG("Trying to convert from RGBA8888 to RGBA4444 but current format is not RGBA8888!");
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

			SAFE_DELETE_ARRAY(mpData);
			mpData = (u8*)pubyBitmapData4444;
			meFormat = Core::CImage::RGBA_4444;
		}
	}
}
