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
#include <ChilliSource/Core/System/AppSystem.h>
#include <ChilliSource/Core/Base/QueryableInterface.h>
#include <ChilliSource/Core/Resource/Resource.h>
#include <ChilliSource/Core/File/FileSystem.h>

namespace ChilliSource
{
	namespace Core
	{
        //------------------------------------------------------------
		/// ResourceProvider defines an interface for systems which
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
			virtual bool CanCreateResourceOfKind(InterfaceIDType in_interfaceID) const = 0;
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
			virtual bool CanCreateResourceFromFileWithExtension(const std::string& in_extension) const = 0;
            //----------------------------------------------------
            /// Creates a new resource from file.
            ///
            /// @author S Downie
            ///
            /// @param The storage location.
            /// @param The filepath.
            /// @param [Out] The output resource.
            ///
            /// @return Whether or not the resource was successfully
            /// created.
            //----------------------------------------------------
			virtual bool CreateResourceFromFile(StorageLocation in_storageLocation, const std::string& in_filePath, ResourceSPtr& out_resource) = 0;
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
			virtual bool AsyncCreateResourceFromFile(StorageLocation in_storageLocation, const std::string& in_filePath, ResourceSPtr& out_resource) {return false;};
            //----------------------------------------------------
            /// Destructor.
            ///
            /// @author S Downie
            //----------------------------------------------------
			virtual ~ResourceProvider() {}
            
            virtual bool StreamResourceFromFile(StorageLocation in_storageLocation, const std::string& in_filePath, ResourceSPtr& out_resource) {return false;};
		};
	}	
}

#endif