/** 
 * File: ImageLoader.h
 * Date: 1 Oct 2010
 * Description: iOS 3_X image resource provider
 */

/** 
 *
 * Author Stuart 
 * Version 1.0 - moFlo
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#ifndef _MOFLO_PLATFORM_IOS_IMAGELOADER_H_
#define _MOFLO_PLATFORM_IOS_IMAGELOADER_H_

#include <ChilliSource/Core/Image/ImageResourceProvider.h>
#include <ChilliSource/Core/Image/Image.h>

#include <string>

namespace moFlo
{
	namespace iOSPlatform
	{		
		class ImageLoader : public IImageResourceProvider
		{
		public:
			
			virtual ~ImageLoader(){}
			//----------------------------------------------------------------
			/// Is A
			///
			/// Is the object of the given interface type
			/// @param Interface type to query
			/// @return Whether the object is of given type
			//----------------------------------------------------------------
			bool IsA(Core::InterfaceIDType inInterfaceID) const;
			//----------------------------------------------------------------
			/// Can Create Resource Of Kind
			///
			/// @param Resource to compare against
			/// @return Whether the object can load a resource of given type
			//----------------------------------------------------------------
			bool CanCreateResourceOfKind(Core::InterfaceIDType inInterfaceID) const;
			//----------------------------------------------------------------
			/// Can Create Resource From File With Extension
			///
			/// @param Extension to compare against
			/// @return Whether the object can load a resource with that extension
			//----------------------------------------------------------------
			bool CanCreateResourceFromFileWithExtension(const std::string & inExtension) const;
			//----------------------------------------------------------------
			/// Create Resource From File
			///
            /// @param The storage location to load from
			/// @param File path to resource
			/// @param Out: Resource
			/// @return Whether the resource loaded 
			//----------------------------------------------------------------
			bool CreateResourceFromFile(Core::StorageLocation ineStorageLocation, const std::string & inFilePath, Core::ResourcePtr& outpResource);
			//----------------------------------------------------------------
			/// Create Image From File
			///
            /// @param The storage location to load from
			/// @param File path to resource
			/// @param Image format
			/// @param Out: Resource
			/// @return Whether the resource loaded 
			//----------------------------------------------------------------
			bool CreateImageFromFile(Core::StorageLocation ineStorageLocation, const std::string & inFilePath, Core::CImage::Format ineFormat, Core::ResourcePtr& outpResource);
			
		private:
            //----------------------------------------------------------------
            /// Create PNG Image From File
            ///
            /// Create a PNG image using the Apple API
            ///
            /// @param Image data in bytes
			/// @param Size of data in bytes
			/// @param Whether the asset is high res
			/// @param Image format
			/// @param Out: Image resource
            //----------------------------------------------------------------
            void CreatePNGImageFromFile(const s8* inpbyData, u32 inudwDataSize, Core::CImage::Format ineFormat, Core::CImage* outpImage);
            //----------------------------------------------------------------
            /// Create JPG Image From File
            ///
            /// Create a JPG image using the Apple API
            ///
            /// @param Image data in bytes
			/// @param Size of data in bytes
			/// @param Whether the asset is high res
			/// @param Image format
			/// @param Out: Image resource
            //----------------------------------------------------------------
            void CreateJPGImageFromFile(const s8* inpbyData, u32 inudwDataSize, Core::CImage::Format ineFormat, Core::CImage* outpImage);
			//----------------------------------------------------------------
			/// Create PVR Image From File
			///
			/// Create an image in raw byte format
			///
            /// @param Image data in bytes
			/// @param Size of data in bytes
			/// @param Whether the asset is high res
			/// @param Out: Image resource
			//----------------------------------------------------------------
			void CreatePVRImageFromFile(const s8* inpbyData, u32 inudwDataSize, Core::CImage* outpImage);
			//----------------------------------------------------------------
			/// RGBA8888 To RGB565
			///
			/// Downsize the image from 32 bits to 16 bits no alpha
			///
			/// @param RGBA8888 data
			/// @param Width x Height
			/// @return RGB565 data
			//-----------------------------------------------------------------
			u8* RGBA8888ToRGB565(u8* inpData, u32 inudwArea);
			//----------------------------------------------------------------
			/// RGBA8888 To RGBA4444
			///
			/// Downsize the image from 32 bits to 16 bits
			///
			/// @param RGBA8888 data
			/// @param Width x Height
			/// @return RGBA4444 data
			//-----------------------------------------------------------------
			u8* RGBA8888ToRGBA4444(u8* inpData, u32 inudwArea);
			//----------------------------------------------------------------
			/// RGBA8888 To LUM88
			///
			/// Downsize the image from 32 bits to 16 bits
			///
			/// @param RGBA8888 data
			/// @param Width x Height
			/// @return LUM88 data
			//-----------------------------------------------------------------
			u8* RGBA8888ToLUM88(u8* inpData, u32 inudwArea);
            //----------------------------------------------------------------
			/// RGBA8888 To LUM8
			///
			/// Downsize the image from 32 bits to 8 bits
			///
			/// @param RGBA8888 data
			/// @param Width x Height
			/// @return LUM8 data
			//-----------------------------------------------------------------
            u8* RGBA8888ToLUM8(u8* inpData, u32 inudwArea);
		};
	}
}



#endif
