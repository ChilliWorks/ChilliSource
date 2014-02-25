/*
 *  ResourceManager.h
 *  MoFlow
 *
 *  Created by Tag Games on 01/10/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#ifndef _MOFLO_CORE_RESOURCEMANAGER_H_
#define _MOFLO_CORE_RESOURCEMANAGER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/QueryableInterface.h>
#include <ChilliSource/Core/Resource/Resource.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>
#include <ChilliSource/Core/File/FileSystem.h>

namespace ChilliSource
{
	namespace Core
	{
        struct ResourceDesc
        {
            ResourceDesc(std::string instrFileName, Core::StorageLocation ineStorageLocation)
            :strFileName(instrFileName), eStorageLocation(ineStorageLocation)
            {}
            
            ResourceDesc()
            :strFileName(""), eStorageLocation(Core::StorageLocation::k_none)
            {}
            
            std::string strFileName;
            Core::StorageLocation eStorageLocation;
        };
        
		class IResourceManager : public IQueryableInterface
		{	
		public:
			IResourceManager();
			virtual ~IResourceManager(){}
			//-----------------------------------------------------------------
			/// Set Application Owner
			///
			/// @param Application
			//-----------------------------------------------------------------
			void SetApplicationOwner(CApplication* inpApplication);
			//-----------------------------------------------------------------
			/// Get Resource Type
			///
			/// @return ID of the resource type managed by this object
			//-----------------------------------------------------------------
			virtual InterfaceIDType GetResourceType() const = 0;
			//-----------------------------------------------------------------
			/// Get Provider Type
			///
			/// @return ID of the resource type consumed by the object from the provider
			//-----------------------------------------------------------------
			virtual InterfaceIDType GetProviderType() const = 0; 
			//-----------------------------------------------------------------
			/// Manages Resource of Type
			///
			/// @param Resource ID 
			/// @return Whether the manager handles resources of type
			//-----------------------------------------------------------------
			virtual bool ManagesResourceOfType(InterfaceIDType inInterfaceID) const = 0;
			//-----------------------------------------------------------------
			/// Manages Resource with Extension
			///
			/// @param Extension 
			/// @return Whether the manager handles resources with extension 
			//-----------------------------------------------------------------
			bool ManagesResourceWithExtension(const std::string &instrExtension) const;
			//-----------------------------------------------------------------
			/// Set Resource Providers
			///
			/// @param Vector of the resource loaders that this manager will use
			//-----------------------------------------------------------------
			void SetResourceProviders(const DYNAMIC_ARRAY<IResourceProvider*> & inVecResourceProviders);
			//-----------------------------------------------------------------
			/// Release
			///
			/// Remove the resource from the managers cache
			/// This will destroy the resource if nothing else is using it.
			/// Warning: If you attempt to load this resource again it will
			/// load regardless of whether anything else is pointing to it.
			///
			/// @param Resource pointer
			//-----------------------------------------------------------------
			virtual void Release(IResource* inpResource);
			//-----------------------------------------------------------------
			/// Release All
			///
			/// Remove all resources from the managers cache. These will
			/// be destroyed if nothing else is sharing them. If other objects
			/// are sharing them and a get call is issued the resource will
			/// be loaded again. You have been warned. Use ReleseAllUnused()
			//-----------------------------------------------------------------
			virtual void ReleaseAll();
			//-----------------------------------------------------------------
			/// Release All Unused
			///
			/// Safer release method that will purge the cache of any
			/// resources not currently in use
			//-----------------------------------------------------------------
			virtual u32 ReleaseAllUnused();
			//-----------------------------------------------------------------
			/// Get Resource From File
			///
			/// Generic call to get the managers resource
			///
            /// @param The storage location to load from
			/// @param File path to resource
			/// @return Generic pointer to resource type
			//-----------------------------------------------------------------
			virtual ResourcePtr GetResourceFromFile(StorageLocation ineStorageLocation, const std::string &instrFilePath) = 0;
			//-----------------------------------------------------------------
			/// Async Get Resource From File
			///
			/// Generic call to get the managers resource. This will
			/// load the resource on a seperate thread but will return the
			/// resource pointer synchronously. Before using the resource
			/// pointer the IsLoaded flag should be checked
			///
            /// @param The storage location to load from
			/// @param File path to resource
			/// @return Generic pointer to resource type
			//-----------------------------------------------------------------
			virtual ResourcePtr AsyncGetResourceFromFile(StorageLocation ineStorageLocation, const std::string & inFilePath) {return ResourcePtr();};
			//-----------------------------------------------------------------
			/// Get Resource Provider By Extension
			///
			/// returns the first resource provider found that handles the
			/// passed in file extension.
			///
			/// @param file extension string
			/// @return resource provider
			//-----------------------------------------------------------------
			IResourceProvider* GetResourceProviderByExtension(const std::string &instrExtension);
            //-----------------------------------------------------------------
            /// Get All Loaded Resources
            ///
            /// returns a list of strings of all currently in-use resources.
            ///
            /// @return List of current loaded resources
			//-----------------------------------------------------------------
			virtual DYNAMIC_ARRAY<ResourceDesc> GetAllLoadedResources() const;
		protected:
			
			DYNAMIC_ARRAY<IResourceProvider*> mResourceProviders;
			CApplication* mpApplicationOwner;
			
			typedef HASH_MAP<std::string, ResourcePtr> MapStringToResourcePtr;
			MapStringToResourcePtr mMapFilenameToResource;
		};
	}
}

#endif
