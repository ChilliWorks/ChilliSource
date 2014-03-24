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
			bool AsyncCreateResourceFromFile(Core::StorageLocation in_storageLocation, const std::string & in_filePath, Core::ResourceSPtr& out_resource) override;
        private:
            friend Core::ImageProviderUPtr Core::ImageProvider::Create();
            
            //-----------------------------------------------------------
            /// Private constructor to force use of factory method
            ///
            /// @author S Downie
            //-----------------------------------------------------------
            ImageProvider(){}
		};
	}
}



#endif
