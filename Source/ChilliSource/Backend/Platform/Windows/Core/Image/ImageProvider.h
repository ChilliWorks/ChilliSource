//
//  ImageProvider.h
//  Chilli Source
//
//  Created by I Copland in 2011
//  Copyright ©2010 Tag Games Limited. All rights reserved.
//

#ifndef _CHILLISOURCE_BACKEND_PLATFORM_WINDOWS_CORE_IMAGE_IMAGEPROVIDER_H_
#define _CHILLISOURCE_BACKEND_PLATFORM_WINDOWS_CORE_IMAGE_IMAGEPROVIDER_H_

#include <ChilliSource/Backend/Platform/Windows/ForwardDeclarations.h>
#include <ChilliSource/Core/Image/Image.h>
#include <ChilliSource/Core/Image/ImageProvider.h>

namespace ChilliSource
{
	namespace Windows
	{
		//--------------------------------------------------------------
		/// The windows backend for the Image Provider. This loads
		/// PNG images from file.
		///
		/// @author Ian Copland
		//--------------------------------------------------------------
		class ImageProvider final : public Core::ImageProvider
		{
		public:
			CS_DECLARE_NAMEDTYPE(ImageProvider);
			//--------------------------------------------------------
			/// Is the object of the given interface type
			///
			/// @author I Copland
			///
			/// @param Interface type to query
			///
			/// @return Whether the object is of given type
			//--------------------------------------------------------
			bool IsA(Core::InterfaceIDType in_interfaceId) const override;
			//----------------------------------------------------------------
			/// Queries whether or not this resource provider can create
			/// resources of the given type.
			///
			/// @author I Copland
			///
			/// @param Resource to compare against
			///
			/// @return Whether the object can load a resource of given type
			//----------------------------------------------------------------
			bool CanCreateResourceOfKind(Core::InterfaceIDType in_interfaceId) const override;
			//----------------------------------------------------------------
			/// Queries whether or not this resource provider can create
			/// resources from files with the given extension.
			///
			/// @author I Copland
			///
			/// @param Extension to compare against
			///
			/// @return Whether the object can load a resource with that extension
			//----------------------------------------------------------------
			bool CanCreateResourceFromFileWithExtension(const std::string & in_extension) const override;
			//----------------------------------------------------------------
			/// Creates a new image resource from file.
			///
			/// @author I Copland
			///
			/// @param Storage Location
			/// @param File path to resource
			/// @param [Out] Resource
			///
			/// @return Success
			//----------------------------------------------------------------
			bool CreateResourceFromFile(Core::StorageLocation in_storageLocation, const std::string & in_filepath, Core::ResourceOldSPtr& out_resource) override;
			//----------------------------------------------------
			/// Creates a new resource from file asynchronously.
			/// The resource will be returned immediately but
			/// cannot be used until the loaded flag is set. This
			/// can be queried using IsLoaded() on the resource.
			///
			/// @author I Copland
			///
			/// @param The storage location.
			/// @param The filepath.
			/// @param [Out] The output resource.
			///
			/// @return Whether or not the resource async load was
			/// successfully started.
			//----------------------------------------------------
			bool AsyncCreateResourceFromFile(Core::StorageLocation in_storageLocation, const std::string & in_filePath, Core::ResourceOldSPtr& out_resource) override;
		private:
			friend Core::ImageProviderUPtr Core::ImageProvider::Create();
			//-------------------------------------------------------
			/// Private constructor to force use of factory method
			///
			/// @author I Copland
			//-------------------------------------------------------
			ImageProvider(){}

		};
	}
}



#endif
