/*
*  ImageLoader.cpp
*  FatherTed
*
*  Created by Tag Games on 01/10/2010.
*  Updated for Android use by Ian Copland on 23/03/2011
*  Copyright 2010 Tag Games. All rights reserved.
*
*/

#include <ChilliSource/Backend/Platform/Windows/ImageLoaderWindows.h>
#include <ChilliSource/Backend/Platform/Windows/ImageLoading/PngImage.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/String/StringUtils.h>
#include <ChilliSource/Core/Base/Screen.h>
#include <ChilliSource/Core/Image/Image.h>

namespace ChilliSource
{
	namespace Windows
	{
		const std::string PNGExtension("png");

		//----------------------------------------------------------------
		/// Is A
		///
		/// Is the object of the given interface type
		/// @param Interface type to query
		/// @return Whether the object is of given type
		//----------------------------------------------------------------
		bool ImageLoader::IsA(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == IResourceProvider::InterfaceID;
		}
		//----------------------------------------------------------------
		/// Can Create Resource Of Kind
		///
		/// @param Resource to compare against
		/// @return Whether the object can load a resource of given type
		//----------------------------------------------------------------
		bool ImageLoader::CanCreateResourceOfKind(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == Core::CImage::InterfaceID;
		}
		//----------------------------------------------------------------
		/// Can Create Resource From File With Extension
		///
		/// @param Extension to compare against
		/// @return Whether the object can load a resource with that extension
		//----------------------------------------------------------------
		bool ImageLoader::CanCreateResourceFromFileWithExtension(const std::string & inExtension) const
		{
			return (inExtension == PNGExtension);
		}
		//----------------------------------------------------------------
		/// Create Resource From File
		///
		/// @param Storage Location
		/// @param File path to resource
		/// @param Out: Resource
		/// @return Success
		//----------------------------------------------------------------
		bool ImageLoader::CreateResourceFromFile(Core::STORAGE_LOCATION ineLocation, const std::string & inFilePath, Core::ResourcePtr& outpResource)
		{
			//Based on the extension decide how to load the file
			return CreateImageFromFile(ineLocation, inFilePath, Core::CImage::FORMAT_DEFAULT, outpResource);
		}
		//----------------------------------------------------------------
		/// Create Image From File
		///
		/// @param Storage location to load from
		/// @param File path to resource
		/// @param Image format
		/// @param Out: Resource
		/// @return Sucess
		//----------------------------------------------------------------
		bool ImageLoader::CreateImageFromFile(Core::STORAGE_LOCATION ineLocation, const std::string & inFilePath, Core::CImage::Format ineFormat, Core::ResourcePtr& outpResource)
		{
            Core::FileStreamPtr pImageFile = Core::CApplication::GetFileSystemPtr()->CreateFileStream(ineLocation, inFilePath, Core::FM_READ_BINARY);
	
			if(pImageFile && !pImageFile->IsBad())
			{
                std::string strName;
                std::string strExtension;
                
                Core::CStringUtils::SplitBaseFilename(inFilePath, strName, strExtension);
                
				//Based on the extension decide how to load the file
				std::string abyData;
				pImageFile->GetAll(abyData);

				if(strExtension == PNGExtension)
				{
					CreatePNGImageFromFile(ineLocation, inFilePath, ineFormat, (Core::CImage*)outpResource.get());
					return true;
				}
			}

			return false;
		}
		//----------------------------------------------------------------
		/// Create UI Image From File
		///
		/// Create a UI image using 
		///
		/// @param Storage 
		/// @param File path to resource
		/// @param Image format
		/// @param Out: Image data
		/// @return Success
		//----------------------------------------------------------------
		bool ImageLoader::CreatePNGImageFromFile(Core::STORAGE_LOCATION ineLocation, const std::string & inFilePath, Core::CImage::Format ineFormat, Core::CImage* outpImage)
		{
			moFlo::WindowsPlatform::CPngImage image;
			image.Load(ineLocation, inFilePath);

			if(image.IsLoaded())
			{
				if(ineFormat == Core::CImage::FORMAT_DEFAULT)
				{
					ineFormat = meDefaultFormat;
				}

				u32 udwWidth = image.GetWidth();
				u32 udwHeight = image.GetHeight();
				u32 udwArea = udwWidth * udwHeight;

				u8* pubyBitmapData8888 = image.GetImageData();

				//We always load the image as RGBA_8888 but we convert on the fly to the correct format
				switch(ineFormat)
				{
				case Core::CImage::RGBA_4444:
					{
						DEBUG_LOG("Converting to RGBA_4444");

						u8* pubyBitmapData4444 = RGBA8888ToRGBA4444(pubyBitmapData8888, udwArea);

						free(pubyBitmapData8888);
						pubyBitmapData8888 = pubyBitmapData4444;
						break;
					}
				case Core::CImage::RGB_565:
					{
						DEBUG_LOG("Converting to RGBA_565");

						u8* pubyBitmapData565 = RGBA8888ToRGB565(pubyBitmapData8888, udwArea);

						free(pubyBitmapData8888);
						pubyBitmapData8888 = pubyBitmapData565;
						break;
					}
				case Core::CImage::LUMA_88:
					{
						DEBUG_LOG("Converting to LUMA_88");

						u8* pubyBitmapData88 = RGBA8888ToLUM88(pubyBitmapData8888, udwArea);

						free(pubyBitmapData8888);
						pubyBitmapData8888 = pubyBitmapData88;

					}

				default:
					break;
				};

				//create the cimage
				outpImage->SetData(pubyBitmapData8888);
				outpImage->SetWidth(udwWidth);
				outpImage->SetHeight(udwHeight);
				outpImage->SetFormat(ineFormat);

				//release the png image without deallocating the image data
				image.Release(false);

				return true;
			}
			else
			{
				ERROR_LOG("Failed to load image: " + inFilePath);
				image.Release();
				return false;
			}
		}
		//----------------------------------------------------------------
		/// RGBA8888 To RGB565
		///
		/// Downsize the image from 32 bits to 16 bits no alpha
		///
		/// @param RGBA8888 data
		/// @param Width x Height
		/// @return RGB565 data
		//-----------------------------------------------------------------
		u8* ImageLoader::RGBA8888ToRGB565(u8* inpData, u32 inudwArea)
		{
			u32 udwBytesPerPixel = 2;

			u8* pubyBitmapData565 = (u8*)calloc(inudwArea, udwBytesPerPixel);
			u32* pPixel32 = (u32*)inpData;
			u16* pPixel16 = (u16*)pubyBitmapData565;

			for(u32 i=0; i<inudwArea; ++i, ++pPixel32)
			{
				*pPixel16++ = ((((*pPixel32 >> 0) & 0xFF) >> 3) << 11) | 
					((((*pPixel32 >> 8) & 0xFF) >> 2) << 5) | 
					((((*pPixel32 >> 16) & 0xFF) >> 3) << 0);
			}

			return pubyBitmapData565;
		}
		//----------------------------------------------------------------
		/// RGBA8888 To RGBA4444
		///
		/// Downsize the image from 32 bits to 16 bits
		///
		/// @param RGBA8888 data
		/// @param Width x Height
		/// @return RGBA4444 data
		//-----------------------------------------------------------------
		u8* ImageLoader::RGBA8888ToRGBA4444(u8* inpData, u32 inudwArea)
		{
			u32 udwBytesPerPixel = 2;

			u8* pubyBitmapData4444 = (u8*)calloc(inudwArea, udwBytesPerPixel);
			u32* pPixel32 = (u32*)inpData;
			u16* pPixel16 = (u16*)pubyBitmapData4444;

			for(u32 i=0; i<inudwArea; ++i, ++pPixel32)
			{
				*pPixel16++ = 
					((((*pPixel32 >> 0) & 0xFF) >> 4) << 12) | // R
					((((*pPixel32 >> 8) & 0xFF) >> 4) << 8) | // G
					((((*pPixel32 >> 16) & 0xFF) >> 4) << 4) | // B
					((((*pPixel32 >> 24) & 0xFF) >> 4) << 0); // A
			}

			return pubyBitmapData4444;
		}
		//----------------------------------------------------------------
		/// RGBA8888 To LUM88
		///
		/// Downsize the image from 32 bits to 16 bits
		///
		/// @param RGBA8888 data
		/// @param Width x Height
		/// @return LUM88 data
		//-----------------------------------------------------------------
		u8* ImageLoader::RGBA8888ToLUM88(u8* inpData, u32 inudwArea)
		{
			u32 udwBytesPerPixel = 2;

			u8* pubyBitmapData88 = (u8*)calloc(inudwArea, udwBytesPerPixel);
			u32* pPixel32 = (u32*)inpData;
			u16* pPixel16 = (u16*)pubyBitmapData88;

			for(u32 i=0; i<inudwArea; ++i, ++pPixel32)
			{
				*pPixel16++ = 
					((*pPixel32)& 0xFF) | // L
					((*pPixel32 >> 16) & 0xFF00); // A
			}

			return pubyBitmapData88;
		}
	}
}
