//
//  ImageProvider.cpp
//  Chilli Source
//
//  Created by I Copland in 2011
//  Copyright ©2010 Tag Games Limited. All rights reserved.
//

#include <ChilliSource/Backend/Platform/Windows/Core/Image/ImageProvider.h>

#include <ChilliSource/Backend/Platform/Windows/Core/Image/PngImage.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/Screen.h>
#include <ChilliSource/Core/Image/Image.h>
#include <ChilliSource/Core/String/StringUtils.h>

namespace ChilliSource
{
	namespace Windows
	{
		namespace
		{
			const std::string k_pngExtension("png");

			//----------------------------------------------------------------
			/// Create a new image from a PNG file.
			///
			/// @author I Copland
			///
			/// @param Storage 
			/// @param File path to resource
			/// @param Whether the asset is high resolution
			/// @param [Out] Image data
			///
			/// @return Success
			//----------------------------------------------------------------
			bool CreatePNGImageFromFile(Core::StorageLocation in_storageLocation, const std::string & in_filepath, Core::Image* out_image)
			{
				ChilliSource::Windows::PngImage image;
				image.Load(in_storageLocation, in_filepath);

				if (image.IsLoaded())
				{
					u32 udwWidth = image.GetWidth();
					u32 udwHeight = image.GetHeight();
					u32 udwArea = udwWidth * udwHeight;

					u8* pubyBitmapData8888 = image.GetImageData();

					//create the cimage
					out_image->SetData(pubyBitmapData8888);
					out_image->SetWidth(udwWidth);
					out_image->SetHeight(udwHeight);
					out_image->SetFormat(Core::Image::Format::k_RGBA8888);

					//release the png image without deallocating the image data
					image.Release(false);

					return true;
				}
				else
				{
					CS_LOG_ERROR("Failed to load image: " + in_filepath);
					image.Release();
					return false;
				}
			}
		}

		CS_DEFINE_NAMEDTYPE(ImageProvider);
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		bool ImageProvider::IsA(Core::InterfaceIDType in_interfaceId) const
		{
			return (in_interfaceId == Core::ResourceProvider::InterfaceID || in_interfaceId == Core::ImageProvider::InterfaceID || in_interfaceId == ImageProvider::InterfaceID);
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		bool ImageProvider::CanCreateResourceOfKind(Core::InterfaceIDType in_interfaceId) const
		{
			return in_interfaceId == Core::Image::InterfaceID;
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		bool ImageProvider::CanCreateResourceFromFileWithExtension(const std::string & in_extension) const
		{
			return (in_extension == k_pngExtension);
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		bool ImageProvider::CreateResourceFromFile(Core::StorageLocation in_storageLocation, const std::string & in_filepath, Core::ResourceSPtr& out_resource)
		{
			if (Core::StringUtils::EndsWith(in_filepath, k_pngExtension, true) == true)
			{
				return CreatePNGImageFromFile(in_storageLocation, in_filepath, (Core::Image*)out_resource.get());
			}

			return false;
		}
		//----------------------------------------------------
		//----------------------------------------------------
		bool ImageProvider::AsyncCreateResourceFromFile(Core::StorageLocation in_storageLocation, const std::string & in_filePath, Core::ResourceSPtr& out_resource)
		{
			CS_LOG_WARNING("Async loading is not implemented by the Image Provider.");

			return false;
		}
	}
}
