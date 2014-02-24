/*
 * File: ResourceGroupManager.h
 * Date: 11/11/2010 2010 
 * Description: Handles the grouping of resources so that they can be loaded in bundles. By type or by detail.
 */

/*
 * Author: Scott Downie
 * Version: v 1.0
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#ifndef _MO_FLO_CORE_RESOURCE_GROUP_MANAGER_H_
#define _MO_FLO_CORE_RESOURCE_GROUP_MANAGER_H_

#include <ChilliSource/Core/ForwardDeclarations.h>

namespace ChilliSource
{
	namespace Core
	{
		class IResourceGroupManager
		{
		public:
			IResourceGroupManager(CResourceManagerDispenser* inpResMgrDispenser);
			~IResourceGroupManager();
			
			//-----------------------------------------------------
			/// Create Group
			///
			/// Create a new resource group with the given name
			/// @param Group name (must be unique)
			/// @param Folder directory that houses resources
			//-----------------------------------------------------
			void CreateGroup(const std::string& instrGroupName, const std::string& instrDirectory);
			//-----------------------------------------------------
			/// Load Group
			///
			/// Load the resources inside the given groups 
			/// directory. This will load them into the relative
			/// resource managers resource pool
			//-----------------------------------------------------
			void LoadGroup(const std::string& instrGroupName);
			//-----------------------------------------------------
			/// Unload Group
			///
			/// Free the resources inside the given groups 
			/// directory.
			//-----------------------------------------------------
			void UnloadGroup(const std::string& instrGroupName);
			//-----------------------------------------------------
			/// Destroy Group
			///
			/// Free the resources inside the given groups 
			/// directory and remove the group from the set
			//-----------------------------------------------------
			void DestroyGroup(const std::string& instrGroupName);
			//-----------------------------------------------------
			/// Unload All 
			///
			/// Free all the resources for our groups. Usually
			/// invoked on low memory warning
			//-----------------------------------------------------
			void UnloadAll();
			//-----------------------------------------------------
			/// Destroy All 
			///
			/// Free all the resources for our groups and remove
			/// the groups from the set
			//-----------------------------------------------------
			void DestroyAll();
			//-----------------------------------------------------
			/// Invalidate Cache
			///
			/// The system will let us know that all our groups 
			/// resources have been freed. We will need to load 
			/// them again on request
			//-----------------------------------------------------
			void InvalidateCache();
			
		private:
			
			class CResourceGroup
			{
			public:
				CResourceGroup() : meCurrentStatus(Status::k_notLoaded){}
				
				std::string mstrDirectory;
				
				DYNAMIC_ARRAY<ResourcePtr> mResources;
				
				enum class Status{k_loaded, k_notLoaded};
				Status meCurrentStatus;
			};
			
			typedef SHARED_PTR<CResourceGroup> ResourceGroupPtr;
			
			typedef HASH_MAP<std::string, ResourceGroupPtr> MapStringToResourceGroup;
			typedef MapStringToResourceGroup::iterator MapStringToResourceGroupItr;
			MapStringToResourceGroup mMapNameToResourceGroup;
			
			CResourceManagerDispenser* mpResMgrDispenser;
		};
	}
}

#endif