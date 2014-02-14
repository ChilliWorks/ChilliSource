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

#include <ChilliSource/Core/Resource/Resource.h>
#include <ChilliSource/Core/Resource/ResourceManager.h>

namespace moFlo
{
	namespace Core
	{
		//-------------------------------------------------------
		/// Constructor
		///
		/// Default
		//-------------------------------------------------------
		IResource::IResource(): mpOwningResMgr(NULL), mbIsHighRes(false), mbIsLoaded(false), mpMutex(new boost::mutex()),
				mpCondition(new boost::condition()), meStorageLocation(SL_NONE), mstrFilename("")
		{
		}
		//-------------------------------------------------------
		/// Get Name
		///
		/// @return The resource's name
		//-------------------------------------------------------
		const std::string& IResource::GetName() const
		{
			return mstrName;
		}
		//-------------------------------------------------------
		/// Set Name
		///
		/// @param The resource's name (usually the filename)
		//-------------------------------------------------------
		void IResource::SetName(const std::string& instrName)
		{
			mstrName = instrName;
		}
		//-------------------------------------------------------
		/// Is Loaded
		///
		/// @return Whether the resource is loaded into memory
		//-------------------------------------------------------
		bool IResource::IsLoaded() const
		{
			return mbIsLoaded;
		}
		//-------------------------------------------------------
		/// Set Loaded
		///
		/// @param Whether the resource is loaded into memory
		//-------------------------------------------------------
		void IResource::SetLoaded(bool inbLoaded)
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
		void IResource::SetFilename(std::string instrFilename)
		{
			mstrFilename = instrFilename;
		}
		//-------------------------------------------------------
		/// Get Filename
		///
		/// @return The filename of the resource.
		//-------------------------------------------------------
		const std::string& IResource::GetFilename() const
		{
			return mstrFilename;
		}
		//-------------------------------------------------------
		/// Set Storage Location
		///
		/// @param The storage location of the resource.
		//-------------------------------------------------------
		void IResource::SetStorageLocation(STORAGE_LOCATION ineStorageLocation)
		{
			meStorageLocation = ineStorageLocation;
		}
		//-------------------------------------------------------
		/// Get Storage Location
		///
		/// @return The storage location of the resource.
		//-------------------------------------------------------
		const STORAGE_LOCATION& IResource::GetStorageLocation() const
		{
			return meStorageLocation;
		}
		//-------------------------------------------------------
		/// Set Owning Resource Manager
		///
		/// @param The resource manager who has ownership of us
		//-------------------------------------------------------
		void IResource::SetOwningResourceManager(IResourceManager* inpResMgr)
		{
			mpOwningResMgr = inpResMgr;
		}
		//-------------------------------------------------------
		/// Get Owning Resource Manager
		///
		/// @return The resource manager who has ownership of us
		//-------------------------------------------------------
		IResourceManager* IResource::GetOwningResourceManager()
		{
			return mpOwningResMgr;
		}
		//-------------------------------------------------------
		/// Wait Til Loaded
		///
		/// Block the calling thread until the resource has loaded
		//-------------------------------------------------------
		void IResource::WaitTilLoaded()
		{
			boost::mutex::scoped_lock Lock(*mpMutex);

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
		void IResource::Release()
		{
			if (mpOwningResMgr) 
            {
				mpOwningResMgr->Release(this);
			}
		}
	}
}
