/*
*  PngLoader.h
*  iOSTemplate
*
*  Created by Ian Copland on 06/04/2011.
*  Copyright 2011 Tag Games Ltd. All rights reserved.
*
*/

#include <moFlo/Platform/Windows/ImageLoading/PngImage.h>
#include <moFlo/Platform/Windows/ImageLoading/LibPng/png.h>
#include <moFlo/Core/Application.h>

//----------------------------------------------------------------------------------
/// Read Png Data
///
/// A replacement for the default libPng file reading function. This is needed so
/// the c style file io functions can be replaced with moFlo functions, enabling
/// loading from the package.
/// @param png_structp png_ptr - The currently open Png decorder
/// @param png_bytep data - The output data.
/// @param png_size_t length - The length of the data.
//----------------------------------------------------------------------------------
void ReadPngData(png_structp png_ptr, png_bytep data, png_size_t length)
{
	if (png_ptr == NULL)
		return;

	moFlo::Core::IFileStream* pStream = (moFlo::Core::IFileStream*)png_get_io_ptr(png_ptr);
	pStream->Read((s8*)data, length);

	if (pStream->IsBad() == true)
		png_error(png_ptr, "Read Error");
}

namespace moFlo
{
	namespace WindowsPlatform
	{
		//----------------------------------------------------------------------------------
		/// Constructor
		//----------------------------------------------------------------------------------
		CPngImage::CPngImage()
		{
			mbIsLoaded = false;
			mdwHeight = -1;
			mdwWidth = -1;
			mpData = NULL;
		}
		//----------------------------------------------------------------------------------
		/// Constructor
		//----------------------------------------------------------------------------------
		CPngImage::CPngImage(Core::STORAGE_LOCATION ineLocation, const std::string& instrFilename)
		{
			mbIsLoaded = false;
			mdwHeight = -1;
			mdwWidth = -1;
			mpData = NULL;

			Load(ineLocation, instrFilename);
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
		/// Load PNG
		///
		/// Loads a png from a file within documents or the package.
		/// @param Storgae location of the file
		/// @param std::string instrFilename - the path to the file relative to either
		///									   documents or the package.
		//----------------------------------------------------------------------------------
		void CPngImage::Load(Core::STORAGE_LOCATION ineLocation, const std::string& instrFilename)
		{
			//create the file stream
			moFlo::Core::FileStreamPtr stream = moFlo::Core::CApplication::GetFileSystemPtr()->CreateFileStream(ineLocation, instrFilename, moFlo::Core::FM_READ_BINARY);

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
		///
		/// Releases the image data
		/// @param bool inReleaseImageData - Whether or not to release the image data
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
		///
		/// returns whether or not the image is loaded
		//----------------------------------------------------------------------------------
		bool CPngImage::IsLoaded()
		{
			return mbIsLoaded;
		}
		//----------------------------------------------------------------------------------
		/// Get Height
		///
		/// returns the height of the image
		//----------------------------------------------------------------------------------
		s32 CPngImage::GetHeight()
		{
			return mdwHeight;
		}
		//----------------------------------------------------------------------------------
		/// Get Width
		///
		/// returns the width of the image
		//----------------------------------------------------------------------------------
		s32 CPngImage::GetWidth()
		{
			return mdwWidth;
		}
		//----------------------------------------------------------------------------------
		/// Get Image Data
		///
		/// returns the image data.
		//----------------------------------------------------------------------------------
		u8 * CPngImage::GetImageData()
		{
			return mpData;
		}
		//----------------------------------------------------------------------------------
		/// Load with lib png
		///
		/// Loads the png data using lib png
		/// @param FileStreamPtr inStream - the steam lib png should use to read the data.
		//----------------------------------------------------------------------------------
		bool CPngImage::LoadWithLibPng(Core::FileStreamPtr inStream)
		{
			//insure that it is indeed a png
			const s32 dwHeaderSize = 8;
			u8 ubyHeader[dwHeaderSize];
			inStream->Read((s8*)ubyHeader, dwHeaderSize);

			if (png_sig_cmp(ubyHeader, 0, dwHeaderSize) == true)
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
			s32 dwRowBytes = png_get_rowbytes(pPng, pInfo);
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

			//end the read
			png_read_end(pPng, NULL);

			//deallocate everything
			png_destroy_read_struct(&pPng, &pInfo, (png_infopp)NULL);
			return true;
		}
	}
}
