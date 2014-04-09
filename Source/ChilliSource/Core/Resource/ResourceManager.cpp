/*
 *  ResourceManager.cpp
 *  iOSTemplate
 *
 *  Created by Scott Downie on 18/08/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Core/Resource/ResourceManager.h>
#include <ChilliSource/Core/Resource/ResourceProviderOld.h>

namespace ChilliSource
{
	namespace Core
	{
		//-----------------------------------------------------------------
		/// Constructor
		///
		/// Default
		//-----------------------------------------------------------------
		ResourceManager::ResourceManager() : mpApplicationOwner(nullptr)
		{
		}
		//-----------------------------------------------------------------
		/// Set Application Owner
		///
		/// @param Application
		//-----------------------------------------------------------------
		void ResourceManager::SetApplicationOwner(Application* inpApplication)
		{
			mpApplicationOwner = inpApplication;
		}
		//-----------------------------------------------------------------
		/// Set Resource Providers
		///
		/// @param Vector of the resource loaders that this manager will use
		//-----------------------------------------------------------------
		void ResourceManager::SetResourceProviderOlds(const std::vector<ResourceProviderOld*> & inVecResourceProviderOlds)
		{
			mResourceProviderOlds = inVecResourceProviderOlds;
		}
		//-----------------------------------------------------------------
		/// Manages Resource with Extension
		///
		/// @param Extension 
		/// @return Whether the manager handles resources with extension 
		//-----------------------------------------------------------------
		bool ResourceManager::ManagesResourceWithExtension(const std::string &instrExtension) const
		{
			for (u32 nProvider = 0; nProvider < mResourceProviderOlds.size(); nProvider++) 
			{
				if(mResourceProviderOlds[nProvider]->CanCreateResourceFromFileWithExtension(instrExtension))
				{
					return true;
				}
			}
			
			return false;
		}
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
		void ResourceManager::Release(ResourceOld* inpResource)
		{
			for(MapStringToResourceSPtr::iterator it = mMapFilenameToResource.begin(); it != mMapFilenameToResource.end(); ++it)
			{
				if(it->second.get() == inpResource)
				{
					ResourceOldSPtr pResource = it->second;
					CS_ASSERT((pResource.use_count() <= 2), "Cannot release a resource if it is owned by another object (i.e. use_count > 0) : (" + pResource->GetName() + ")");
					CS_LOG_VERBOSE("Releasing resource from cache " + inpResource->GetName());
					mMapFilenameToResource.erase(it);
					return;
				}
			}
		}
		//-----------------------------------------------------------------
		/// Release All
		///
		/// Remove all resources from the managers cache. These will
		/// be destroyed if nothing else is sharing them. If other objects
		/// are sharing them and a get call is issued the resource will
		/// be loaded again. You have been warned. Use ReleseAllUnused()
		//-----------------------------------------------------------------
		void ResourceManager::ReleaseAll()
		{
			for(MapStringToResourceSPtr::iterator it = mMapFilenameToResource.begin(); it != mMapFilenameToResource.end(); ++it)
			{
				ResourceOldSPtr pResource = it->second;
				CS_ASSERT((pResource.use_count() <= 2), "Cannot release a resource if it is owned by another object (i.e. use_count > 0) : (" + pResource->GetName() + ")");
				CS_LOG_VERBOSE("Releasing resource from cache " + pResource->GetName());
			}

			mMapFilenameToResource.clear();
		}
		//-----------------------------------------------------------------
		/// Release All Unused
		///
		/// Safer release method that will purge the cache of any
		/// resources not currently in use
		//-----------------------------------------------------------------
		u32 ResourceManager::ReleaseAllUnused()
		{
            u32 udwNumReleased = 0;
			for(MapStringToResourceSPtr::iterator it = mMapFilenameToResource.begin(); it != mMapFilenameToResource.end(); )
			{
				if(it->second.use_count() == 1)
				{
					CS_LOG_VERBOSE("Releasing resource from cache " + it->second->GetName());
					it = mMapFilenameToResource.erase(it);
                    ++udwNumReleased;
				}
				else
				{
					++it;
				}
			}
            
            return udwNumReleased;
		}
		//-----------------------------------------------------------------
		/// Get Resource Provider By Extension
		///
		/// returns the first resource provider found that handles the
		/// passed in file extension.
		///
		/// @param file extension string
		/// @return resource provider
		//-----------------------------------------------------------------
		ResourceProviderOld* ResourceManager::GetResourceProviderOldByExtension(const std::string &instrExtension)
		{
			for (std::vector<ResourceProviderOld*>::iterator it = mResourceProviderOlds.begin(); it != mResourceProviderOlds.end(); it++)
			{
				if ((*it)->CanCreateResourceFromFileWithExtension(instrExtension) == true)
				{
					return (*it);
				}
			}
			return nullptr;
		}
        
        //-----------------------------------------------------------------
        /// Get All Loaded Resources
        ///
        /// returns a list of strings of all currently in-use resources.
        ///
        /// @return List of current loaded resources
        //-----------------------------------------------------------------
        std::vector<ResourceDesc> ResourceManager::GetAllLoadedResources() const
        {
            std::vector<ResourceDesc> asUsedResources;
            for(MapStringToResourceSPtr::const_iterator it = mMapFilenameToResource.begin(); it != mMapFilenameToResource.end(); ++it )
			{
                ResourceDesc sResource;
                sResource.eStorageLocation = it->second->GetStorageLocation();
                sResource.strFileName = it->second->GetFilename();
                asUsedResources.push_back(sResource);
            }
            return asUsedResources;
        }

	}
}
