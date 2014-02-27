//
//  ResourceManagerDispenser.h
//  moFloTest
//
//  Created by Scott Downie on 02/05/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#ifndef _MO_FLO_CORE_RESOURCE_MANAGER_DISPENSER_H_
#define _MO_FLO_CORE_RESOURCE_MANAGER_DISPENSER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/QueryableInterface.h>

#define GET_RESOURCE_MANAGER(TYPE) ChilliSource::Core::CResourceManagerDispenser::GetSingletonPtr()->GetResourceManagerWithInterface<TYPE>() 
#define LOAD_RESOURCE(TYPE, LOCATION, FILENAME) std::static_pointer_cast<TYPE>(ChilliSource::Core::CResourceManagerDispenser::GetSingletonPtr()->GetResourceManagerForType(TYPE::InterfaceID)->GetResourceFromFile(LOCATION, FILENAME))
#define ASYNC_LOAD_RESOURCE(TYPE, LOCATION, FILENAME) std::static_pointer_cast<TYPE>(ChilliSource::Core::CResourceManagerDispenser::GetSingletonPtr()->GetResourceManagerForType(TYPE::InterfaceID)->AsyncGetResourceFromFile(LOCATION, FILENAME))

namespace ChilliSource
{
    namespace Core
    {
		class CResourceManagerDispenser
        {
        public:
            CResourceManagerDispenser(Application* inpApp);
            ~CResourceManagerDispenser();
            //--------------------------------------------------------------------------------------------------
            /// Get Singleton Ptr
            ///
            /// @return Singleton instance
            //--------------------------------------------------------------------------------------------------
            static CResourceManagerDispenser* GetSingletonPtr();
			//--------------------------------------------------------------------------------------------------
            /// @return Singleton instance
            //--------------------------------------------------------------------------------------------------
            static CResourceManagerDispenser& GetSingleton();
            //--------------------------------------------------------------------------------------------------
			/// Register Resource Manager
			///
			/// Adds a resource manager to the application pool. This allows systems to load resources
			/// that they themselves cannot provide
			/// @param Handle to resource manager
			//--------------------------------------------------------------------------------------------------
			void RegisterResourceManager(IResourceManager * inpResourceManager);
            //--------------------------------------------------------------------------------------------------
            /// Get Resource Manager With Interface
            ///
            /// Looks for a manager with the given interface
            /// @param The type ID of the manager interface you are seeking
            /// @return Manager that implements the given interface or nullptr if none available
            //--------------------------------------------------------------------------------------------------
            IResourceManager* GetResourceManagerWithInterface(InterfaceIDType inInterfaceID);
            //--------------------------------------------------------------------------------------------------
			/// Get Resource Manager With Interface
			///
			/// Convenience template function for the above
			/// @return Manager that implements the templated interface or nullptr if none available
			//--------------------------------------------------------------------------------------------------
			template <typename T> T* GetResourceManagerWithInterface()
            {
				return static_cast<T*>(GetResourceManagerWithInterface(T::InterfaceID));
			}
			//--------------------------------------------------------------------------------------------------
			/// Get Resource Manager For Type
			///
			/// Looks through the applications pool of resource managers til it finds one that can create
			/// a resource of given type
			/// @param Resource type ID that you wish to load
			//--------------------------------------------------------------------------------------------------
			IResourceManager* GetResourceManagerForType(InterfaceIDType inResourceInterfaceID);
			//--------------------------------------------------------------------------------------------------
			/// Get Resource Manager For Extension
			///
			/// Looks through the applications pool of resource managers til it finds one that can create
			/// a resource with the given extension
			/// @param Resource extension string
			//--------------------------------------------------------------------------------------------------
			IResourceManager* GetResourceManagerForExtension(const std::string &instrExtension);
            //--------------------------------------------------------------------------------------------------
			/// Get Resource Group Manager
			///
			/// @return A reference to the resource group manager
			//--------------------------------------------------------------------------------------------------
			IResourceGroupManager& GetResourceGroupManager();
			//--------------------------------------------------------------------------------------------------
			/// Get Resource Group Manager Pointer
			///
			/// @return A pointer to the resource group manager
			//--------------------------------------------------------------------------------------------------
			IResourceGroupManager* GetResourceGroupManagerPtr();
            //----------------------------------------------------------------------
            /// Free Resource Caches
            ///
            /// Release the cached resources that are not in use
            //----------------------------------------------------------------------
            void FreeResourceCaches();
            //----------------------------------------------------------------------
            /// Set Resource Providers
            ///
            /// Iterate through the list of resource providers and add them to
            /// any managers of that type. This allows the managers to create
            /// their own resources
            ///
            /// @param Vector of resource provider pointers
            //----------------------------------------------------------------------
            void SetResourceProviders(const std::vector<IResourceProvider*> inProviders);
            
        private:
            std::vector<IResourceManager*> mResourceManagers;
            Application* mpApp;
            IResourceGroupManager* mpResourceGroupMgr;
            
            static CResourceManagerDispenser* pInstance;
        };
    }
}

#endif
