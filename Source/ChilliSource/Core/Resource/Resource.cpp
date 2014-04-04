//
//  Resource.cpp
//  Chilli Source
//
//  Created by Scott Downie on 30/09/2010.
//  Copyright (c) 2010 Tag Games Ltd. All rights reserved.
//

#include <ChilliSource/Core/Resource/Resource.h>

#include <ChilliSource/Core/Resource/ResourcePool.h>

namespace ChilliSource
{
	namespace Core
	{
		//-------------------------------------------------------
		//-------------------------------------------------------
		Resource::Resource(ResourcePool* in_resourcePool)
        : m_resourcePool(in_resourcePool), m_location(StorageLocation::k_none), m_loadState(LoadState::k_loading)
		{
		}
        //-------------------------------------------------------
        //-------------------------------------------------------
        Resource::LoadState Resource::GetLoadState() const
        {
            return m_loadState;
        }
        //-------------------------------------------------------
        /// If we set the load state to loaded or failed
        /// we need to notify any waiting parties that the resource
        /// is now ready
        //-------------------------------------------------------
        void Resource::SetLoadState(LoadState in_loadState)
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            
            m_loadState = in_loadState;
            
            if(m_loadState == LoadState::k_loaded || m_loadState == LoadState::k_failed)
            {
                m_loadWaitCondition.notify_all();
            }
        }
		//-------------------------------------------------------
		//-------------------------------------------------------
		void Resource::SetFilePath(const std::string& in_filePath)
		{
			m_filePath = in_filePath;
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		const std::string& Resource::GetFilePath() const
		{
			return m_filePath;
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		void Resource::SetStorageLocation(StorageLocation in_location)
		{
			m_location = in_location;
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
        StorageLocation Resource::GetStorageLocation() const
		{
			return m_location;
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		void Resource::WaitTilLoaded()
		{
            std::unique_lock<std::mutex> lock(m_mutex);

			while(m_loadState == LoadState::k_loading)
			{
				m_loadWaitCondition.wait(lock);
			}
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		void Resource::Release()
		{
            m_resourcePool->Release(this);
		}
	}
}
