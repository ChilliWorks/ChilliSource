//
//  CSImageProvider.h
//  Chilli Source
//
//  Created by Scott Downie on 10/08/2012.
//  Copyright (c) 2012 Tag Games. All rights reserved.
//

#ifndef _CHILLISOURCE_CORE_IMAGE_CSIMAGEPROVIDER_H_
#define _CHILLISOURCE_CORE_IMAGE_CSIMAGEPROVIDER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Image/ImageProvider.h>

namespace ChilliSource
{
    namespace Core
    {
        //-------------------------------------------------------------
        /// A resource provider that creates images from moimage files.
        ///
        /// @author S Downie
        //-------------------------------------------------------------
        class CSImageProvider final : public ResourceProviderOld
        {
        public:
            CS_DECLARE_NAMEDTYPE(CSImageProvider);
            //-------------------------------------------------------
            /// Factory method
            ///
            /// @author S Downie
            ///
            /// @return New backend with ownership transferred
            //-------------------------------------------------------
            static CSImageProviderUPtr Create();
            //-------------------------------------------------------
            /// Is the object of the given interface type.
            ///
            /// @author S Downie
            ///
            /// @param Interface type to query
            ///
            /// @return Whether the object is of given type
            //-------------------------------------------------------
            bool IsA(Core::InterfaceIDType in_interfaceId) const override;
            //-------------------------------------------------------
            /// Queries whether or not this provider can create
            /// resources of the given type.
            ///
            /// @author S Downie
            ///
            /// @param Resource to compare against
            ///
            /// @return Whether the object can load a resource of
            /// given type
            //-------------------------------------------------------
            bool CanCreateResourceOfKind(Core::InterfaceIDType in_interfaceId) const override;
            //-------------------------------------------------------
            /// Queries whether or not this provider can create
            /// resources from files with the given extension.
            ///
            /// @author S Downie
            ///
            /// @param Extension to compare against
            ///
            /// @return Whether the object can load a resource with
            /// that extension
            //-------------------------------------------------------
            bool CanCreateResourceFromFileWithExtension(const std::string& in_extension) const override;
            //-------------------------------------------------------
            /// Creates a new image from file.
            ///
            /// @author S Downie
            ///
            /// @param The storage location to load from
            /// @param File path to resource
            /// @param [Out] Resource
            ///
            /// @return Whether the resource loaded
            //-------------------------------------------------------
            bool CreateResourceFromFile(Core::StorageLocation in_storageLocation, const std::string& in_filepath, Core::ResourceOldSPtr& out_resource) override;
            //----------------------------------------------------
            /// Creates a new resource from file asynchronously.
            /// The resource will be returned immediately but
            /// cannot be used until the loaded flag is set. This
            /// can be queried using IsLoaded() on the resource.
            ///
            /// @author S Downie
            ///
            /// @param The storage location.
            /// @param The filepath.
            /// @param [Out] The output resource.
            ///
            /// @return Whether or not the resource async load was
            /// successfully started.
            //----------------------------------------------------
			bool AsyncCreateResourceFromFile(StorageLocation in_storageLocation, const std::string& in_filePath, ResourceOldSPtr& out_resource) override;
        private:

            //-------------------------------------------------------
            /// Private constructor to force use of factory method
            ///
            /// @author S Downie
            //-------------------------------------------------------
            CSImageProvider(){}
        };
    }
}

#endif

