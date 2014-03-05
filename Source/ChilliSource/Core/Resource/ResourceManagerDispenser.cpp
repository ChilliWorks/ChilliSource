//
//  ResourceManagerDispenser.cpp
//  moFloTest
//
//  Created by Scott Downie on 02/05/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#include <ChilliSource/Core/Resource/ResourceManagerDispenser.h>
#include <ChilliSource/Core/Resource/ResourceGroupManager.h>
#include <ChilliSource/Core/Resource/ResourceManager.h>
#include <ChilliSource/Core/Resource/ResourceProvider.h>

#include <ChilliSource/Rendering/Texture/Texture.h>
#include <ChilliSource/Rendering/Sprite/SpriteSheet.h>

#include <ChilliSource/Rendering/Font/Font.h>

namespace ChilliSource
{
    namespace Core
    {
        ResourceManagerDispenser* ResourceManagerDispenser::pInstance = nullptr;
        //----------------------------------------------------
        /// Constructor
        ///
        /// Singleton instance
        //----------------------------------------------------
        ResourceManagerDispenser::ResourceManagerDispenser(Application* inpApp) : mpApp(inpApp)
        {
            pInstance = this;
			mpResourceGroupMgr = new ResourceGroupManager(pInstance);
        }
        //--------------------------------------------------------------------------------------------------
        /// Get Singleton Ptr
        ///
        /// @return Singleton instance
        //--------------------------------------------------------------------------------------------------
        ResourceManagerDispenser* ResourceManagerDispenser::GetSingletonPtr()
        {
            return pInstance;
        }
		//--------------------------------------------------------------------------------------------------
		/// @return Singleton instance
		//--------------------------------------------------------------------------------------------------
		ResourceManagerDispenser& ResourceManagerDispenser::GetSingleton()
        {
			return *pInstance;
		}
        //--------------------------------------------------------------------------------------------------
		/// Register Resource Manager
		///
		/// Adds a resource manager to the application pool. This allows systems to load resources
		/// that they themselves cannot provide
		/// @param Handle to resource manager
		//--------------------------------------------------------------------------------------------------
		void ResourceManagerDispenser::RegisterResourceManager(ResourceManager * inpResourceManager)
		{
			inpResourceManager->SetApplicationOwner(mpApp);
			mResourceManagers.push_back(inpResourceManager);
		}
        //--------------------------------------------------------------------------------------------------
        /// Get Resource Manager With Interface
        ///
        /// Looks for a manager with the given interface
        /// @param The type ID of the manager interface you are seeking
        /// @return Manager that implements the given interface or nullptr if none available
        //--------------------------------------------------------------------------------------------------
        ResourceManager* ResourceManagerDispenser::GetResourceManagerWithInterface(InterfaceIDType inInterfaceID)
        {
            for (std::vector<ResourceManager*>::iterator pMgr = mResourceManagers.begin(); pMgr != mResourceManagers.end(); ++pMgr) 
			{
				if ((*pMgr)->IsA(inInterfaceID)) 
				{
					return (*pMgr);
				}
			}
            
            CS_LOG_WARNING("Application cannot find resource manager for type");
            return nullptr;
        }
		//--------------------------------------------------------------------------------------------------
		/// Get Resource Manager For Type
		///
		/// Looks through the applications pool of resource managers til it finds one that can create
		/// a resource of given type
		/// @param Resource type ID that you wish to load
		//--------------------------------------------------------------------------------------------------
		ResourceManager* ResourceManagerDispenser::GetResourceManagerForType(InterfaceIDType inResourceInterfaceID)
		{
			for (std::vector<ResourceManager*>::iterator pMgr = mResourceManagers.begin(); pMgr != mResourceManagers.end(); ++pMgr) 
			{
				if ((*pMgr)->ManagesResourceOfType(inResourceInterfaceID)) 
				{
					return (*pMgr);
				}
			}
			
			CS_LOG_WARNING("Application cannot find resource manager for type");
			return nullptr;
		}
		//--------------------------------------------------------------------------------------------------
		/// Get Resource Manager For Extension
		///
		/// Looks through the applications pool of resource managers til it finds one that can create
		/// a resource with the given extension
		/// @param Resource extension string
		//--------------------------------------------------------------------------------------------------
		ResourceManager* ResourceManagerDispenser::GetResourceManagerForExtension(const std::string &instrExtension)
		{
			for (std::vector<ResourceManager*>::iterator pMgr = mResourceManagers.begin(); pMgr != mResourceManagers.end(); ++pMgr) 
			{
				if((*pMgr)->ManagesResourceWithExtension(instrExtension)) 
				{
					return (*pMgr);
				}
			}
			
			CS_LOG_WARNING("Application cannot find resource manager for extension " + instrExtension);
			return nullptr;
		}
        //----------------------------------------------------------------------
		/// Free Resource Caches
		///
		/// Release the cached resources that are not in use
		//----------------------------------------------------------------------
		void ResourceManagerDispenser::FreeResourceCaches()
		{
			//Tell our resource managers to ditch their cache
            u32 udwReleasedCount = 0;
			do
			{
                udwReleasedCount = 0;
                for (std::vector<ResourceManager*>::iterator pMgr = mResourceManagers.begin(); pMgr != mResourceManagers.end(); ++pMgr)
                {
                    udwReleasedCount += (*pMgr)->ReleaseAllUnused();
                }
			}
			while (udwReleasedCount > 0);
			//Free our resource groups too. Actually all the resources will be freed by this stage
			//Just let the group manager know this
			mpResourceGroupMgr->InvalidateCache();
		}
        //--------------------------------------------------------------------------------------------------
        /// Get Resource Group Manager
        ///
        /// @return A reference to the resource group manager
        //--------------------------------------------------------------------------------------------------
        ResourceGroupManager& ResourceManagerDispenser::GetResourceGroupManager()
        {
            return *mpResourceGroupMgr;
        }
        //--------------------------------------------------------------------------------------------------
        /// Get Resource Group Manager Pointer
        ///
        /// @return A pointer to the resource group manager
        //--------------------------------------------------------------------------------------------------
        ResourceGroupManager* ResourceManagerDispenser::GetResourceGroupManagerPtr()
        {
            return mpResourceGroupMgr;
        }
        //----------------------------------------------------------------------
        /// Set Resource Providers
        ///
        /// Iterate through the list of resource providers and add them to
        /// any managers of that type. This allows the managers to create
        /// their own resources
        ///
        /// @param Vector of resource provider pointers
        //----------------------------------------------------------------------
        void ResourceManagerDispenser::SetResourceProviders(const std::vector<ResourceProvider*> inProviders)
        {
            //For each resource manager, give it resource providers of the same resource type
			for (std::vector<ResourceManager*>::iterator pMgr = mResourceManagers.begin(); pMgr != mResourceManagers.end(); ++pMgr) 
			{
				InterfaceIDType ResourceType = (*pMgr)->GetProviderType();
				std::vector<ResourceProvider*> UsefulResourceProviders;
				
				for (std::vector<ResourceProvider*>::const_iterator pProv = inProviders.begin(); pProv != inProviders.end(); ++pProv) 
				{
					if ((*pProv)->CanCreateResourceOfKind(ResourceType)) 
					{
						UsefulResourceProviders.push_back((*pProv));
					}
				}
				(*pMgr)->SetResourceProviders(UsefulResourceProviders);
			}
        }
        //----------------------------------------------------------------------
        //----------------------------------------------------------------------
        void ResourceManagerDispenser::ReleaseAll()
        {
        	//Cleanup all resource managers
        	FreeResourceCaches();

        	//check that there are no resources still in use
#ifdef CS_ENABLE_DEBUG

        	for (std::vector<ResourceManager*>::iterator pMgr = mResourceManagers.begin(); pMgr != mResourceManagers.end(); ++pMgr)
			{
        		std::vector<ResourceDesc> loadedResources = (*pMgr)->GetAllLoadedResources();
        		if (loadedResources.size() != 0)
        		{
        			CS_LOG_ERROR("Could not release all resources from manager. The following resources are still in use:");
        			for (const ResourceDesc& resourceDesc : loadedResources)
        			{
        				CS_LOG_ERROR("  " + resourceDesc.strFileName);
        			}
        		}
			}
#endif
        }
        //--------------------------------------------------------------------
        /// Destructor
        //--------------------------------------------------------------------
        ResourceManagerDispenser::~ResourceManagerDispenser()
        {
        	ReleaseAll();
            CS_SAFEDELETE(mpResourceGroupMgr);
        }
    }
}
