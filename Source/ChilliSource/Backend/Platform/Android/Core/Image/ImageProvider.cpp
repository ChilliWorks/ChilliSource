//
//  ImageProvider.h
//  Chilli Source
//
//  Created by I Copland in 2011
//  Copyright ©2010 Tag Games Limited. All rights reserved.
//

#include <ChilliSource/Backend/Platform/Android/Core/Image/ImageProvider.h>

#include <ChilliSource/Backend/Platform/Android/Core/Image/PngImage.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Image/Image.h>
#include <ChilliSource/Core/String/StringUtils.h>

namespace ChilliSource
{
	namespace Android
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
			/// @param Image format
			/// @param [Out] Image data
			///
			/// @return Success
			//----------------------------------------------------------------
			bool CreatePNGImageFromFile(Core::StorageLocation in_storageLocation, const std::string& in_filepath, Core::Image* out_image)
			{
				//load the png image
				ChilliSource::Android::PngImage image;
				image.Load(in_storageLocation, in_filepath);

				//check the image has loaded
				if(image.IsLoaded() == false)
				{
					image.Release();
					CS_LOG_ERROR("Failed to load image: " + in_filepath);
					return false;
				}

				//create the cimage
				out_image->SetData(image.GetImageData());
				out_image->SetWidth(image.GetWidth());
				out_image->SetHeight(image.GetHeight());
				out_image->SetFormat(image.GetImageFormat());

				//release the png image without deallocating the image data
				image.Release(false);

				return true;
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
	}
}
