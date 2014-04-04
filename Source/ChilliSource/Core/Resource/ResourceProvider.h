//
//  ResourceProvider.h
//  Chilli Source
//
//  Created by S Downie on 30/09/2010.
//  Copyright 2010 Tag Games. All rights reserved.
//

#ifndef _CHILLISOURCE_CORE_RESOURCE_RESOURCEPROVIDER_H_
#define _CHILLISOURCE_CORE_RESOURCE_RESOURCEPROVIDER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/File/StorageLocation.h>
#include <ChilliSource/Core/System/AppSystem.h>

namespace ChilliSource
{
	namespace Core
	{
        //------------------------------------------------------------
		/// ResourceProviderOld defines an interface for systems which
        /// produce resource objects from files.
        ///
        /// @author S Downie
        //------------------------------------------------------------
		class ResourceProvider : public AppSystem
		{
		public:
            CS_DECLARE_NAMEDTYPE(ResourceProvider);
            //----------------------------------------------------
            /// Allows querying of the resource type this provider
            /// can create.
            ///
            /// @author S Downie
            ///
            /// @param The interface ID of the resourouce.
            ///
            /// @return Whether or not the resource can be created.
            //----------------------------------------------------
			virtual bool CanCreateResourceType(InterfaceIDType in_interfaceId) const = 0;
            //----------------------------------------------------
            /// Allows querying of the resource type this provider
            /// can create.
            ///
            /// @author S Downie
            ///
            /// @param The extension of the resource file.
            ///
            /// @return Whether or not the resource can be created.
            //----------------------------------------------------
			virtual bool CanCreateResourceWithFileExtension(const std::string& in_extension) const = 0;
            //----------------------------------------------------
            /// Creates a new resource from file.
            ///
            /// @author S Downie
            ///
            /// @param The storage location.
            /// @param The filepath.
            /// @param [Out] The output resource.
            //----------------------------------------------------
			virtual void CreateResourceFromFile(StorageLocation in_storageLocation, const std::string& in_filePath, ResourceSPtr& out_resource) = 0;
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
            //----------------------------------------------------
			virtual void CreateResourceFromFileAsync(StorageLocation in_storageLocation, const std::string& in_filePath, ResourceSPtr& out_resource) = 0;
            //----------------------------------------------------
            /// Destructor.
            ///
            /// @author S Downie
            //----------------------------------------------------
			virtual ~ResourceProvider() {}
		};
	}	
}

#endif