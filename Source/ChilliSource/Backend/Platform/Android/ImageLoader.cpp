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
			return inInterfaceID == Core::ResourceProvider::InterfaceID;
		}
		//----------------------------------------------------------------
		/// Can Create Resource Of Kind
		//----------------------------------------------------------------
		bool ImageLoader::CanCreateResourceOfKind(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == Core::Image::InterfaceID;
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
		bool ImageLoader::CreateResourceFromFile(Core::StorageLocation ineStorageLocation, const std::string & inFilePath, Core::ResourceSPtr& outpResource)
		{
			return CreateImageFromFile(ineStorageLocation, inFilePath, Core::Image::Format::k_RGBA8888, outpResource);
		}
		//----------------------------------------------------------------
		/// Create Image From File
		//----------------------------------------------------------------
		bool ImageLoader::CreateImageFromFile(Core::StorageLocation ineStorageLocation, const std::string & inFilePath, Core::Image::Format ineFormat, Core::ResourceSPtr& outpResource)
		{
            Core::FileStreamSPtr pImageFile = Core::Application::GetFileSystemPtr()->CreateFileStream(ineStorageLocation, inFilePath, Core::FileMode::k_readBinary);
            
			if(pImageFile && !pImageFile->IsBad())
			{
                std::string strName;
                std::string strExtension;
                
                Core::StringUtils::SplitBaseFilename(inFilePath, strName, strExtension);
                
				if(strExtension == PNGExtension)
				{
					CreatePNGImageFromFile(ineStorageLocation, inFilePath, ineFormat, (Core::Image*)outpResource.get());
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
		void ImageLoader::CreatePNGImageFromFile(Core::StorageLocation ineStorageLocation, const std::string & inFilePath, Core::Image::Format ineFormat, Core::Image* outpImage)
		{
			//load the png image
			ChilliSource::Android::CPngImage image;
			image.Load(ineStorageLocation, inFilePath);

			//check the image has loaded
			if(image.IsLoaded() == false)
			{
				image.Release();
				CS_LOG_ERROR("Failed to load image: " + inFilePath);
				return;
			}

			//get the formats
			Core::Image::Format intendedFormat = ineFormat;
			Core::Image::Format currentFormat = image.GetImageFormat();

			//if the format is not already greyscale, then convert the image data to the requested format
			if (FormatIsGreyscale(currentFormat) == false)
			{
				if(intendedFormat == Core::Image::Format::k_default)
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
					CS_LOG_ERROR("Trying to load an image with an alpha channel in a format that doesnt. This is not supported on android currently.");
					return;
				}

				//convert to the intended format if not already in it
				if (currentFormat != intendedFormat)
				{
					switch (intendedFormat)
					{
					case Core::Image::Format::k_Lum8:
						image.ConvertFormatFromRGB888ToLUM8();
						break;
					case Core::Image::Format::k_LumA88:
						image.ConvertFormatFromRGB8888ToLUMA88();
						break;
					case Core::Image::Format::k_RGB565:
						image.ConvertFormatFromRGB888ToRGB565();
						break;
					case Core::Image::Format::k_RGBA4444:
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
		bool ImageLoader::FormatContainsAlpha(Core::Image::Format ineFormat) const
		{
			if (ineFormat == Core::Image::Format::k_LumA88 || ineFormat == Core::Image::Format::k_RGBA4444 || ineFormat == Core::Image::Format::k_RGBA8888)
				return true;
			return false;
		}
		//----------------------------------------------------------------
		/// Format Is Greyscale
		//----------------------------------------------------------------
		bool ImageLoader::FormatIsGreyscale(Core::Image::Format ineFormat) const
		{
			if (ineFormat == Core::Image::Format::k_Lum8 || ineFormat == Core::Image::Format::k_LumA88)
				return true;
			return false;
		}
		//----------------------------------------------------------------
		/// Remove Alpha From Format Type
		//----------------------------------------------------------------
		Core::Image::Format ImageLoader::RemoveAlphaFromFormatType(Core::Image::Format ineFormat) const
		{
			switch (ineFormat)
			{
			case Core::Image::Format::k_LumA88:
				return Core::Image::Format::k_Lum8;
			case Core::Image::Format::k_RGBA4444:
				return Core::Image::Format::k_RGB565;
			case Core::Image::Format::k_RGBA8888:
				return Core::Image::Format::k_RGB888;
			default:
				return ineFormat;
			}
		}
	}
}
