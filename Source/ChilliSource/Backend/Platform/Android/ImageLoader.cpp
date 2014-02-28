/*
 *  ImageLoader.cpp
 *  FatherTed
 *
 *  Created by Tag Games on 01/10/2010.
 *  Updated for Android use by Ian Copland on 23/03/2011
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */



#include <ChilliSource/Backend/Platform/Android/ImageLoader.h>
#include <ChilliSource/Core/String/StringUtils.h>
#include <ChilliSource/Core/Base/Application.h>

#include <ChilliSource/Core/Image/Image.h>

#include <ChilliSource/Backend/Platform/Android/ImageLoading/PngImage.h>

namespace ChilliSource
{
	namespace Android
	{
		const std::string PNGExtension("png");
		const std::string JPGExtension("jpg");
		const std::string JPEGExtension("jpeg");
		const std::string PVRExtension("pvr");
	
		ImageLoader::ImageLoader()
		{
			
		}
		//----------------------------------------------------------------
		/// Is A
		//----------------------------------------------------------------
		bool ImageLoader::IsA(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == IResourceProvider::InterfaceID;
		}
		//----------------------------------------------------------------
		/// Can Create Resource Of Kind
		//----------------------------------------------------------------
		bool ImageLoader::CanCreateResourceOfKind(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == Core::CImage::InterfaceID;
		}
		//----------------------------------------------------------------
		/// Can Create Resource From File With Extension
		//----------------------------------------------------------------
		bool ImageLoader::CanCreateResourceFromFileWithExtension(const std::string & inExtension) const
		{
			return (inExtension == PNGExtension);
		}
		//----------------------------------------------------------------
		/// Create Resource From File
		//----------------------------------------------------------------
		bool ImageLoader::CreateResourceFromFile(Core::STORAGE_LOCATION ineStorageLocation, const std::string & inFilePath, Core::ResourcePtr& outpResource)
		{
			return CreateImageFromFile(ineStorageLocation, inFilePath, Core::CImage::RGBA_8888, outpResource);
		}
		//----------------------------------------------------------------
		/// Create Image From File
		//----------------------------------------------------------------
		bool ImageLoader::CreateImageFromFile(Core::STORAGE_LOCATION ineStorageLocation, const std::string & inFilePath, Core::CImage::Format ineFormat, Core::ResourcePtr& outpResource)
		{
            Core::FileStreamPtr pImageFile = Core::Application::GetFileSystemPtr()->CreateFileStream(ineStorageLocation, inFilePath, Core::FM_READ_BINARY);
            
			if(pImageFile && !pImageFile->IsBad())
			{
                std::string strName;
                std::string strExtension;
                
                Core::CStringUtils::SplitBaseFilename(inFilePath, strName, strExtension);
                
				if(strExtension == PNGExtension)
				{
					CreatePNGImageFromFile(ineStorageLocation, inFilePath, ineFormat, (Core::CImage*)outpResource.get());
					return true;
				}
				else if(strExtension == PVRExtension)
				{
					return false;
				}
				else if(strExtension == JPGExtension || strExtension == JPEGExtension)
				{
					return false;
				}
			}
			return false;
		}
		//----------------------------------------------------------------
		/// Create UI Image From File
		//----------------------------------------------------------------
		void ImageLoader::CreatePNGImageFromFile(Core::STORAGE_LOCATION ineStorageLocation, const std::string & inFilePath, Core::CImage::Format ineFormat, Core::CImage* outpImage)
		{
			//load the png image
			ChilliSource::Android::CPngImage image;
			image.Load(ineStorageLocation, inFilePath);

			//check the image has loaded
			if(image.IsLoaded() == false)
			{
				image.Release();
				ERROR_LOG("Failed to load image: " + inFilePath);
				return;
			}

			//get the formats
			Core::CImage::Format intendedFormat = ineFormat;
			Core::CImage::Format currentFormat = image.GetImageFormat();

			//if the format is not already greyscale, then convert the image data to the requested format
			if (FormatIsGreyscale(currentFormat) == false)
			{
				if(intendedFormat == Core::CImage::FORMAT_DEFAULT)
				{
					intendedFormat = meDefaultFormat;
				}

				//insure alpha data is not being asked for if it is not available.
				if (FormatContainsAlpha(currentFormat) == false)
					intendedFormat = RemoveAlphaFromFormatType(intendedFormat);

				//currently removal of the alpha channel is not supported, so through an error if this is requested.
				if (FormatContainsAlpha(currentFormat) == true && FormatContainsAlpha(intendedFormat) == false)
				{
					image.Release();
					ERROR_LOG("Trying to load an image with an alpha channel in a format that doesnt. This is not supported on android currently.");
					return;
				}

				//convert to the intended format if not already in it
				if (currentFormat != intendedFormat)
				{
					switch (intendedFormat)
					{
					case Core::CImage::LUM_8:
						image.ConvertFormatFromRGB888ToLUM8();
						break;
					case Core::CImage::LUMA_88:
						image.ConvertFormatFromRGB8888ToLUMA88();
						break;
					case Core::CImage::RGB_565:
						image.ConvertFormatFromRGB888ToRGB565();
						break;
					case Core::CImage::RGBA_4444:
						image.ConvertFormatFromRGBA8888ToRGBA4444();
						break;
					default:
						break;
					}
				}
			}

			//create the cimage
			outpImage->SetData(image.GetImageData());
			outpImage->SetWidth(image.GetWidth());
			outpImage->SetHeight(image.GetHeight());
			outpImage->SetFormat(image.GetImageFormat());

			//release the png image without deallocating the image data
			image.Release(false);
		}
		//----------------------------------------------------------------
		/// Format Contains Alpha
		//----------------------------------------------------------------
		bool ImageLoader::FormatContainsAlpha(Core::CImage::Format ineFormat) const
		{
			if (ineFormat == Core::CImage::LUMA_88 || ineFormat == Core::CImage::RGBA_4444 || ineFormat == Core::CImage::RGBA_8888)
				return true;
			return false;
		}
		//----------------------------------------------------------------
		/// Format Is Greyscale
		//----------------------------------------------------------------
		bool ImageLoader::FormatIsGreyscale(Core::CImage::Format ineFormat) const
		{
			if (ineFormat == Core::CImage::LUM_8 || ineFormat == Core::CImage::LUMA_88)
				return true;
			return false;
		}
		//----------------------------------------------------------------
		/// Remove Alpha From Format Type
		//----------------------------------------------------------------
		Core::CImage::Format ImageLoader::RemoveAlphaFromFormatType(Core::CImage::Format ineFormat) const
		{
			switch (ineFormat)
			{
			case Core::CImage::LUMA_88:
				return Core::CImage::LUM_8;
			case Core::CImage::RGBA_4444:
				return Core::CImage::RGB_565;
			case Core::CImage::RGBA_8888:
				return Core::CImage::RGB_888;
			default:
				return ineFormat;
			}
		}
	}
}
