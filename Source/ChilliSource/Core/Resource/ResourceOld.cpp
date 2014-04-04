/*
 * File: Resource.cpp
 * Date: 15/11/2010 2010 
 * Description: 
 */

/*
 * Author: Scott Downie
 * Version: v 1.0
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#include <ChilliSource/Core/Resource/ResourceOld.h>
#include <ChilliSource/Core/Resource/ResourceManager.h>

namespace ChilliSource
{
	namespace Core
	{
		//-------------------------------------------------------
		/// Constructor
		///
		/// Default
		//-------------------------------------------------------
		ResourceOld::ResourceOld(): mpOwningResMgr(nullptr), mbIsHighRes(false), mbIsLoaded(false), mpMutex(new std::mutex()),
        mpCondition(new std::condition_variable()), meStorageLocation(StorageLocation::k_none), mstrFilename("")
		{
		}
		//-------------------------------------------------------
		/// Get Name
		///
		/// @return The resource's name
		//-------------------------------------------------------
		const std::string& ResourceOld::GetName() const
		{
			return mstrName;
		}
		//-------------------------------------------------------
		/// Set Name
		///
		/// @param The resource's name (usually the filename)
		//-------------------------------------------------------
		void ResourceOld::SetName(const std::string& instrName)
		{
			mstrName = instrName;
		}
		//-------------------------------------------------------
		/// Is Loaded
		///
		/// @return Whether the resource is loaded into memory
		//-------------------------------------------------------
		bool ResourceOld::IsLoaded() const
		{
			return mbIsLoaded;
		}
		//-------------------------------------------------------
		/// Set Loaded
		///
		/// @param Whether the resource is loaded into memory
		//-------------------------------------------------------
		void ResourceOld::SetLoaded(bool inbLoaded)
		{
			mbIsLoaded = inbLoaded;

			if(mbIsLoaded == true)
			{
				mpCondition->notify_all();
			}
		}
		//-------------------------------------------------------
		/// Set Filename
		///
		/// @param The filename of the resource.
		//-------------------------------------------------------
		void ResourceOld::SetFilename(std::string instrFilename)
		{
			mstrFilename = instrFilename;
		}
		//-------------------------------------------------------
		/// Get Filename
		///
		/// @return The filename of the resource.
		//-------------------------------------------------------
		const std::string& ResourceOld::GetFilename() const
		{
			return mstrFilename;
		}
		//-------------------------------------------------------
		/// Set Storage Location
		///
		/// @param The storage location of the resource.
		//-------------------------------------------------------
		void ResourceOld::SetStorageLocation(StorageLocation ineStorageLocation)
		{
			meStorageLocation = ineStorageLocation;
		}
		//-------------------------------------------------------
		/// Get Storage Location
		///
		/// @return The storage location of the resource.
		//-------------------------------------------------------
		const StorageLocation& ResourceOld::GetStorageLocation() const
		{
			return meStorageLocation;
		}
		//-------------------------------------------------------
		/// Set Owning Resource Manager
		///
		/// @param The resource manager who has ownership of us
		//-------------------------------------------------------
		void ResourceOld::SetOwningResourceManager(ResourceManager* inpResMgr)
		{
			mpOwningResMgr = inpResMgr;
		}
		//-------------------------------------------------------
		/// Get Owning Resource Manager
		///
		/// @return The resource manager who has ownership of us
		//-------------------------------------------------------
		ResourceManager* ResourceOld::GetOwningResourceManager()
		{
			return mpOwningResMgr;
		}
		//-------------------------------------------------------
		/// Wait Til Loaded
		///
		/// Block the calling thread until the resource has loaded
		//-------------------------------------------------------
		void ResourceOld::WaitTilLoaded()
		{
            std::unique_lock<std::mutex> Lock(*mpMutex);

			while(mbIsLoaded == false)
			{
				mpCondition->wait(Lock);
			}
		}
		//-------------------------------------------------------
		/// Release
		///
		/// Tell our owning resource manager to free us
		//-------------------------------------------------------
		void ResourceOld::Release()
		{
			if (mpOwningResMgr) 
            {
				mpOwningResMgr->Release(this);
			}
		}
	}
}
