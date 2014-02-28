/** 
 * File: ImageLoader.h
 * Date: 1 Oct 2010
 * Description: iOS 3_X image resource provider
 */

/** 
 *
 * Author Stuart / Ian Copland
 * Version 1.0 - moFlo
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#ifndef _MOFLO_PLATFORM_ANDROID_IMAGELOADER_H_
#define _MOFLO_PLATFORM_ANDROID_IMAGELOADER_H_

#include <ChilliSource/Core/Image/ImageResourceProvider.h>
#include <ChilliSource/Core/Image/Image.h>

namespace ChilliSource
{
	namespace Android
	{
		class ImageLoader : public Core::ImageResourceProvider
		{
		public:
			ImageLoader();
			virtual ~ImageLoader(){}
			//----------------------------------------------------------------
			/// Is A
			///
			/// Is the object of the given interface type
			/// @param Interface type to query
			/// @return Whether the object is of given type
			//----------------------------------------------------------------
			virtual bool IsA(Core::InterfaceIDType inInterfaceID) const;
			//----------------------------------------------------------------
			/// Can Create Resource Of Kind
			///
			/// @param Resource to compare against
			/// @return Whether the object can load a resource of given type
			//----------------------------------------------------------------
			virtual bool CanCreateResourceOfKind(Core::InterfaceIDType inInterfaceID) const;
			//----------------------------------------------------------------
			/// Can Create Resource From File With Extension
			///
			/// @param Extension to compare against
			/// @return Whether the object can load a resource with that extension
			//----------------------------------------------------------------
			virtual bool CanCreateResourceFromFileWithExtension(const std::string & inExtension) const;
			//----------------------------------------------------------------
			/// Create Resource From File
			///
			/// @param The storage location to load from
			/// @param File path to resource
			/// @return Instantiated resource
			//----------------------------------------------------------------
			virtual bool CreateResourceFromFile(Core::StorageLocation ineStorageLocation, const std::string & inFilePath, Core::ResourceSPtr& outpResource);
			//----------------------------------------------------------------
			/// Create Image From File
			///
			/// @param The storage location to load from
			/// @param File path to resource
			/// @param the image format
			/// @return Instantiated resource
			//----------------------------------------------------------------
			virtual bool CreateImageFromFile(Core::StorageLocation ineStorageLocation, const std::string & inFilePath, Core::Image::Format ineFormat, Core::ResourceSPtr& outpResource);
		private:
			//----------------------------------------------------------------
			/// Create Png Image From File
			///
			/// Create a png image using lib png
			///
			/// @param The storage location to load from
			/// @param File path to resource
			/// @param The high res asset
			/// @param The image format
			/// @return Instantiated resource
			//----------------------------------------------------------------
			void CreatePNGImageFromFile(Core::StorageLocation ineStorageLocation, const std::string & inFilePath, Core::Image::Format ineFormat, Core::Image* outpImage);
			//----------------------------------------------------------------
			/// Format Contains Alpha
			///
			/// @param The image format
			/// @return whether or not this image format contains an alpha channel
			//----------------------------------------------------------------
			bool FormatContainsAlpha(Core::Image::Format ineFormat) const;
			//----------------------------------------------------------------
			/// Format Is Greyscale
			///
			/// @param The image format
			/// @return whether or not this image format is greyscale
			//----------------------------------------------------------------
			bool FormatIsGreyscale(Core::Image::Format ineFormat) const;
			//----------------------------------------------------------------
			/// Remove Alpha From Format Type
			///
			/// @param The image format
			/// @return the equivelant of this format that does not have alpha
			//----------------------------------------------------------------
			Core::Image::Format RemoveAlphaFromFormatType(Core::Image::Format ineFormat) const;
		};
	}
}



#endif
