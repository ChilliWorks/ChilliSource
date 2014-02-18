/*
 *  IResourceProvider.h
 *  MoFlow
 *
 *  Created by Tag Games on 30/09/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#ifndef _MOFLO_CORE_RESOURCEPROVIDER_H_
#define _MOFLO_CORE_RESOURCEPROVIDER_H_

#include <ChilliSource/Core/System/System.h>
#include <ChilliSource/Core/Base/QueryableInterface.h>
#include <ChilliSource/Core/Resource/Resource.h>
#include <ChilliSource/Core/File/FileSystem.h>

namespace moFlo
{
	namespace Core
	{
		/// IResourceProvider defines an interface for systems which produce resource objects from files
		class IResourceProvider : public ISystem
		{
		public:
			virtual ~IResourceProvider(){}
			DECLARE_NAMED_INTERFACE(IResourceProvider);
			virtual bool CanCreateResourceOfKind(InterfaceIDType inInterfaceID) const = 0;
			virtual bool CanCreateResourceFromFileWithExtension(const std::string & inExtension) const = 0;
			
			//---Filepath is relative to the resources directory - either the documents or the package
			virtual bool CreateResourceFromFile(StorageLocation ineStorageLocation, const std::string & inFilePath, ResourcePtr& outpResource) = 0;
			virtual bool AsyncCreateResourceFromFile(StorageLocation ineStorageLocation, const std::string & inFilePath, ResourcePtr& outpResource)
            {
                WARNING_LOG("No implementation for AsyncCreateResourceFromFile"); 
                return false;
            };
			virtual bool StreamResourceFromFile(StorageLocation ineStorageLocation, const std::string & inFilePath, ResourcePtr& outpResource)
            {
                WARNING_LOG("No implementation for StreamResourceFromFile"); 
                return false;
            }
			

		};
	}	
}

#endif