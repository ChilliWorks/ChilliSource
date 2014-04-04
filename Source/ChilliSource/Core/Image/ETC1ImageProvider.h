//
//  ETC1ImageProvider.h
//  Chilli Source
//
//  Created by I Copland on 09/11/2012.
//  Copyright (c) 2012 Tag Games. All rights reserved.
//

#ifndef _CHILLISOURCE_CORE_IMAGE_ETC1IMAGEPROVIDER_H_
#define _CHILLISOURCE_CORE_IMAGE_ETC1IMAGEPROVIDER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Resource/ResourceProviderOld.h>

namespace ChilliSource
{
    namespace Core
    {
        //--------------------------------------------------------------------
        /// An image provider for the ETC1 format. The ETC1 format is not
        /// available on all platforms and you should be sure it is supported
        /// if using it.
        ///
        /// @author I Copland
        //--------------------------------------------------------------------
        class ETC1ImageProvider final : public Core::ResourceProviderOld
        {
        public:
            CS_DECLARE_NAMEDTYPE(ETC1ImageProvider);
            //-------------------------------------------------------------
            /// Factory method
            ///
            /// @author S Downie
            ///
            /// @return New backend with ownership transferred
            //-------------------------------------------------------------
            static ETC1ImageProviderUPtr Create();
            //-------------------------------------------------------------
            /// Is the object of the given interface type
            ///
            /// @author I Copland
            ///
            /// @param Interface type to query
            ///
            /// @return Whether the object is of given type
            //-------------------------------------------------------------
            bool IsA(InterfaceIDType in_interfaceId) const override;
            //-------------------------------------------------------------
            /// Allows querying of whether or not the provider can create
            /// resources of the given type.
            ///
            /// @author I Copland
            ///
            /// @param Resource to compare against
            ///
            /// @return Whether the object can load a resource of given type
            //-------------------------------------------------------------
            bool CanCreateResourceOfKind(InterfaceIDType in_interfaceId) const override;
            //----------------------------------------------------------------
            /// Allows querying of the whether or not the provider can create
            /// resources from files with the given extension.
            ///
            /// @author I Copland
            ///
            /// @param Extension to compare against
            ///
            /// @return Whether the object can load a resource with that
            /// extension
            //----------------------------------------------------------------
            bool CanCreateResourceFromFileWithExtension(const std::string& in_extension) const override;
            //----------------------------------------------------------------
            /// Creates a new image resource from file.
            ///
            /// @author I Copland
            ///
            /// @param The storage location to load from
            /// @param File path to resource
            /// @param [Out] Resource
            ///
            /// @return Whether the resource loaded
            //----------------------------------------------------------------
            bool CreateResourceFromFile(StorageLocation in_storageLocation, const std::string& in_filepath, ResourceOldSPtr& out_resource) override;
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
            ETC1ImageProvider() {}
        };
    }
}

#endif
