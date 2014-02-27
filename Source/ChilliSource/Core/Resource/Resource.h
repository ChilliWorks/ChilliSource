/*
 *  Resource.h
 *  moFlo
 *
 *  Created by Tag Games on 30/09/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */


#ifndef _MOFLO_CORE_RESOURCE_H_
#define _MOFLO_CORE_RESOURCE_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/QueryableInterface.h>
#include <ChilliSource/Core/String/StringUtils.h>
#include <ChilliSource/Core/File/FileSystem.h>

#include <thread>

namespace ChilliSource
{
	namespace Core
	{
		class ResourceManager;
	
		class Resource : public QueryableInterface
		{
		public:
			Resource();
			virtual ~Resource(){}
			
			//-------------------------------------------------------
			/// Get Name
			///
			/// @return The resource's name
			//-------------------------------------------------------
			const std::string& GetName() const;
			//-------------------------------------------------------
			/// Set Name
			///
			/// @param The resource's name (usually the filename)
			//-------------------------------------------------------
			void SetName(const std::string& instrName);
			//-------------------------------------------------------
			/// Get Owning Resource Manager
			///
			/// @param The resource manager who has ownership of us
			//-------------------------------------------------------
			void SetOwningResourceManager(ResourceManager* inpResMgr);
			//-------------------------------------------------------
			/// Get Owning Resource Manager
			///
			/// @return The resource manager who has ownership of us
			//-------------------------------------------------------
			ResourceManager* GetOwningResourceManager();
			//-------------------------------------------------------
			/// Is Loaded
			///
			/// @return Whether the resource is loaded into memory
			//-------------------------------------------------------
			bool IsLoaded() const;
			//-------------------------------------------------------
			/// Set Loaded
			///
			/// @param Whether the resource is loaded into memory
			//-------------------------------------------------------
			void SetLoaded(bool inbLoaded);
			//-------------------------------------------------------
			/// Set Filename
			///
			/// @param The filename of the resource.
			//-------------------------------------------------------
			void SetFilename(std::string instrFilename);
			//-------------------------------------------------------
			/// Get Filename
			///
			/// @return The filename of the resource.
			//-------------------------------------------------------
			const std::string& GetFilename() const;
			//-------------------------------------------------------
			/// Set Storage Location
			///
			/// @param The storage location of the resource.
			//-------------------------------------------------------
			void SetStorageLocation(StorageLocation ineStorageLocation);
			//-------------------------------------------------------
			/// Get Storage Location
			///
			/// @return The storage location of the resource.
			//-------------------------------------------------------
			const StorageLocation& GetStorageLocation() const;
			//-------------------------------------------------------
			/// Wait Til Loaded
			///
			/// Block the calling thread until the resource has loaded
			//-------------------------------------------------------
			void WaitTilLoaded();
			//-------------------------------------------------------
			/// Release
			///
			/// Tell our owning resource manager to free us
			//-------------------------------------------------------
			void Release();
			
		protected:
			
			std::string mstrName;
			std::string mstrFilename;
			StorageLocation meStorageLocation;
			
			bool mbIsHighRes;
			bool mbIsLoaded;
			
			ResourceManager* mpOwningResMgr;

			std::shared_ptr<std::condition_variable> mpCondition;
			std::shared_ptr<std::mutex> mpMutex;
		};
	}
}

#endif
