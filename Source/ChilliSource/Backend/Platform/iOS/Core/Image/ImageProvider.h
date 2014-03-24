//
//  ImageProvider.cpp
//  Chilli Source
//
//  Created by S McGaw on 01/10/2010.
//  Copyright 2010 Tag Games. All rights reserved.
//

#ifndef _CHILLISOURCE_BACKEND_PLATFORM_IOS_IMAGE_IMAGEPROVIDER_H_
#define _CHILLISOURCE_BACKEND_PLATFORM_IOS_IMAGE_IMAGEPROVIDER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Backend/Platform/iOS/ForwardDeclarations.h>
#include <ChilliSource/Core/Image/ImageProvider.h>
#include <ChilliSource/Core/Image/Image.h>

#include <string>

namespace ChilliSource
{
	namespace iOS
	{
        //----------------------------------------------------------------
        /// The iOS backend for the image resource provider. This provides
        /// PNG, JPEG and PVR images.
        ///
        /// @author S McGaw
        //----------------------------------------------------------------
		class ImageProvider : public Core::ImageProvider
		{
		public:
            CS_DECLARE_NAMEDTYPE(ImageProvider);
			//----------------------------------------------------------
			/// Is the object of the given interface type
            ///
            /// @author S McGaw
            ///
			/// @param Interface type to query
            ///
			/// @return Whether the object is of given type
			//----------------------------------------------------------
			bool IsA(Core::InterfaceIDType in_interfaceId) const override;
			//----------------------------------------------------------
			/// Whether or not the provider can create resources
            /// of the given type.
			///
            /// @author S McGaw
            ///
			/// @param Resource to compare against
            ///
			/// @return Whether the object can load a resource of given
            /// type
			//----------------------------------------------------------
			bool CanCreateResourceOfKind(Core::InterfaceIDType in_interfaceId) const override;
			//----------------------------------------------------------
			/// Whether or not the provider can create resources from
            /// files with the given extension.
            ///
			/// @author S McGaw
            ///
			/// @param Extension to compare against
            ///
			/// @return Whether the object can load a resource with that
            /// extension
			//----------------------------------------------------------
			bool CanCreateResourceFromFileWithExtension(const std::string & in_extension) const override;
			//----------------------------------------------------------
			/// Creates a new image resource from file.
			///
            /// @author S McGaw
            ///
            /// @param The storage location to load from
			/// @param File path to resource
			/// @param [Out] Resource
            ///
			/// @return Whether the resource loaded 
			//-----------------------------------------------------------
			bool CreateResourceFromFile(Core::StorageLocation in_storageLocation, const std::string & in_filepath, Core::ResourceSPtr& out_resource) override;
			//-----------------------------------------------------------
			/// Creates a new image resource from file with the given image
            /// format.
			///
            /// @author S McGaw
            ///
            /// @param The storage location to load from
			/// @param File path to resource
			/// @param Image format
			/// @param [Out] Resource
            ///
			/// @return Whether the resource loaded 
			//-----------------------------------------------------------
			bool CreateImageFromFile(Core::StorageLocation in_storageLocation, const std::string & in_filepath, Core::Image::Format in_format, Core::ResourceSPtr& out_resource) override;
        private:
            friend Core::ImageProviderUPtr Core::ImageProvider::Create();
            
            //-----------------------------------------------------------
            /// Private constructor to force use of factory method
            ///
            /// @author S Downie
            //-----------------------------------------------------------
            ImageProvider(){}
            //-----------------------------------------------------------
            /// Create a PNG image using the Apple API
            ///
            /// @author S McGaw
            ///
            /// @param Image data in bytes
			/// @param Size of data in bytes
			/// @param Whether the asset is high res
			/// @param Image format
			/// @param [Out] Image resource
            //-----------------------------------------------------------
            void CreatePNGImageFromFile(const s8* in_data, u32 in_dataSize, Core::Image::Format in_format, Core::Image* out_image);
            //-----------------------------------------------------------
            /// Create a JPG image using the Apple API
            ///
            /// @author S McGaw
            ///
            /// @param Image data in bytes
			/// @param Size of data in bytes
			/// @param Whether the asset is high res
			/// @param Image format
			/// @param [Out] Image resource
            //-----------------------------------------------------------
            void CreateJPGImageFromFile(const s8* in_data, u32 in_dataSize, Core::Image::Format in_format, Core::Image* out_image);
			//-----------------------------------------------------------
			/// Create an image in raw byte format
            ///
            /// @author S McGaw
			///
            /// @param Image data in bytes
			/// @param Size of data in bytes
			/// @param Whether the asset is high res
			/// @param [Out] Image resource
			//------------------------------------------------------------
			void CreatePVRImageFromFile(const s8* in_data, u32 in_dataSize, Core::Image* out_image);
			//------------------------------------------------------------
			/// Downsize the image from 32 bits to 16 bits no alpha
            ///
            /// @author S McGaw
			///
			/// @param RGBA8888 data
			/// @param Width x Height
            ///
			/// @return RGB565 data
			//------------------------------------------------------------
			u8* RGBA8888ToRGB565(u8* in_data, u32 in_area);
			//------------------------------------------------------------
			/// Downsize the image from 32 bits to 16 bits
            ///
            /// @author S McGaw
			///
			/// @param RGBA8888 data
			/// @param Width x Height
            ///
			/// @return RGBA4444 data
			//------------------------------------------------------------
			u8* RGBA8888ToRGBA4444(u8* in_data, u32 in_area);
			//------------------------------------------------------------
			/// Downsize the image from 32 bits to 16 bits
            ///
            /// @author S McGaw
			///
			/// @param RGBA8888 data
			/// @param Width x Height
            ///
			/// @return LUM88 data
			//------------------------------------------------------------
			u8* RGBA8888ToLUM88(u8* in_data, u32 in_area);
            //------------------------------------------------------------
			/// Downsize the image from 32 bits to 8 bits
            ///
            /// @author S McGaw
			///
			/// @param RGBA8888 data
			/// @param Width x Height
            ///
			/// @return LUM8 data
			//------------------------------------------------------------
            u8* RGBA8888ToLUM8(u8* in_data, u32 in_area);
		};
	}
}



#endif
