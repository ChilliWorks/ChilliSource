//
//  ResourcePool.h
//  Chilli Source
//
//  Created by Scott Downie on 03/04/2014.
//  Copyright (c) 2014 Tag Games Ltd. All rights reserved.
//

#ifndef _CHILLISOURCE_CORE_RESOURCE_RESOURCEPOOL_H_
#define _CHILLISOURCE_CORE_RESOURCE_RESOURCEPOOL_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Resource/Resource.h>
#include <ChilliSource/Core/Resource/ResourceProvider.h>
#include <ChilliSource/Core/System/AppSystem.h>

#include <unordered_map>

namespace ChilliSource
{
    namespace Core
    {
        //------------------------------------------------------------------------------------
        /// System that allows the loading and caching of resources. Resource can be loaded
        /// from file based on the resource type and are cached. Subsequent requests for the
        /// same resource will return the cached copy. The resource pool always holds the last
        /// reference to any resource.
        ///
        /// @author S Downie
        //------------------------------------------------------------------------------------
        class ResourcePool final : public AppSystem
        {
        public:
            
            CS_DECLARE_NAMEDTYPE(ResourcePool);
            
            //------------------------------------------------------------------------------------
            /// Factory method for creating the system
            ///
            /// @author S Downie
            ///
            /// @return Ownership of new system instance
            //------------------------------------------------------------------------------------
            static ResourcePoolUPtr Create();
            //------------------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @param Interface ID to compare
            ///
            /// @return Whether the object is of the given interface type
            //------------------------------------------------------------------------------------
            bool IsA(InterfaceIDType in_interfaceId) const override;
            //------------------------------------------------------------------------------------
            /// Load the resource of given type from the file location. If the resource at this
            /// location has previously been loaded then the cached version will be returned.
            ///
            /// The resource is immutable
            ///
            /// If the resource cannot be loaded or no loader exists for that type of resource
            /// then null will be returned
            ///
            /// @author S Downie
            ///
            /// @param Storage location
            /// @param File path
            ///
            /// @return Resource or null
            //-------------------------------------------------------------------------------------
            template <typename TResourceType> std::shared_ptr<const TResourceType> LoadResource(StorageLocation in_location, const std::string& in_filePath);
            //------------------------------------------------------------------------------------
            /// Load the resource of given type from the file location. If the resource at this
            /// location has previously been loaded then the cached version will be returned.
            ///
            /// The resource is immutable
            ///
            /// If the file requires loading it will be done on a background thread. The resource
            /// will be returned immediately but will not be usable until the resource state
            /// changes to k_loaded
            ///
            /// If the loader exists for that type of resource then null will be returned. If
            /// loading fails then the resource will have its state changed to k_failed
            ///
            /// @author S Downie
            ///
            /// @param Storage location
            /// @param File path
            ///
            /// @return Resource or null
            //-------------------------------------------------------------------------------------
            template <typename TResourceType> std::shared_ptr<const TResourceType> LoadResourceAsync(StorageLocation in_location, const std::string& in_filePath);
            //-------------------------------------------------------------------------------------
            /// Forces the pool to release its handle to any unused resources of the given type.
            /// If a resource is still in use the pool will keep it in the cache. The pool is
            /// therefore the last reference to a resource and the resource will be destroyed once
            /// it is removed from the pool
            ///
            /// @author S Downie
            //-------------------------------------------------------------------------------------
            template <typename TResourceType> void ReleaseUnused();
            //-------------------------------------------------------------------------------------
            /// Forces the pool to release its handle to any unused resources.
            /// If a resource is still in use the pool will keep it in the cache. The pool is
            /// therefore the last reference to a resource and the resource will be destroyed once
            /// it is removed from the pool
            ///
            /// @author S Downie
            //-------------------------------------------------------------------------------------
            void ReleaseAllUnused();
            //-------------------------------------------------------------------------------------
            /// Remove the resource from the resource pool and cause it
            /// to be destroyed. This will assert if the resource has
            /// any references so call with a raw pointer i.e.
            ///
            ///
            ///  rawPtr = sharedPtr.get();
            ///  sharedPtr.reset();
            ///  pool->Release(rawPtr);
            ///
            ///
            /// @author S Downie
            ///
            /// @param Resource to release
			//-------------------------------------------------------------------------------------
            void Release(Resource* in_resource);
            //------------------------------------------------------------------------------------
            /// Called when the system is destroyed after the system lifecycle destroy.
            /// Flushes the resource caches and errors if any resources are still in use
            ///
            /// @author S Downie
            //------------------------------------------------------------------------------------
            void Destroy();
            
        private:
            
            using ResourceId = u32;
            //-------------------------------------------------------------------------------------
            /// Descriptor that holds the providers and cached resources for a given type
            ///
            /// @author S Downie
            //-------------------------------------------------------------------------------------
            struct PoolDesc
            {
                std::vector<ResourceProvider*> m_providers;
                std::unordered_map<ResourceId, ResourceSPtr> m_cachedResources;
            };
            //------------------------------------------------------------------------------------
            /// Called when the system receieves a memory warning. This will force the pool
            /// to release any unused resources
            ///
            /// @author S Downie
            //------------------------------------------------------------------------------------
            void OnMemoryWarning() override;
            //------------------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @param File path
            /// @param Descriptor
            ///
            /// @return Resource provider in descriptor that can load file or null
            //------------------------------------------------------------------------------------
            ResourceProvider* FindProvider(const std::string& in_filePath, const PoolDesc& in_desc);
            //------------------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @param Location
            /// @param File path
            ///
            /// @return Unique ID based on the location and path
            //------------------------------------------------------------------------------------
            ResourceId GenerateResourceId(StorageLocation in_location, const std::string& in_filePath) const;
            
        private:
            
            std::unordered_map<InterfaceIDType, PoolDesc> m_descriptors;
        };

        //-------------------------------------------------------------------------------------
        //-------------------------------------------------------------------------------------
        template <typename TResourceType> std::shared_ptr<const TResourceType> ResourcePool::LoadResource(StorageLocation in_location, const std::string& in_filePath)
        {
            auto itDescriptor = m_descriptors.find(TResourceType::InterfaceIDType);
            
            if(itDescriptor == m_descriptors.end())
            {
                CS_LOG_ERROR("Failed to find resource provider for " + TResourceType::TypeName);
                return nullptr;
            }
            
            PoolDesc& desc(itDescriptor->second);
            
            //Check descriptor and see if this resource already exists
            ResourceId resourceId = GenerateResourceId(in_location, in_filePath);
            auto itResource = desc.m_cachedResources.find(resourceId);
            if(itResource != desc.m_cachedResources.end())
            {
                return std::static_pointer_cast<TResourceType>(itResource->second);
            }
            
            //Find a provider that can load this resource
            ResourceProvider* provider = FindProvider(in_filePath, desc);
            if(provider == nullptr)
            {
                return nullptr;
            }
            
            //Load the resource
            ResourceSPtr resource(TResourceType::Create());
            provider->CreateResourceFromFile(in_location, in_filePath, resource);
            if(resource->GetLoadState() != Resource::LoadState::k_loaded)
            {
                CS_LOG_ERROR("Failed to create resource for " + in_filePath);
                return nullptr;
            }
            
            //Add it to the cache
            desc.m_cachedResources.insert(std::make_pair(resourceId, resource));
            
            return std::static_pointer_cast<TResourceType>(resource);
        }
        //-------------------------------------------------------------------------------------
        //-------------------------------------------------------------------------------------
        template <typename TResourceType> std::shared_ptr<const TResourceType> ResourcePool::LoadResourceAsync(StorageLocation in_location, const std::string& in_filePath)
        {
            auto itDescriptor = m_descriptors.find(TResourceType::InterfaceIDType);
            
            if(itDescriptor == m_descriptors.end())
            {
                CS_LOG_ERROR("Failed to find resource provider for " + TResourceType::TypeName);
                return nullptr;
            }
            
            PoolDesc& desc(itDescriptor->second);
            
            //Check descriptor and see if this resource already exists
            ResourceId resourceId = GenerateResourceId(in_location, in_filePath);
            auto itResource = desc.m_cachedResources.find(resourceId);
            if(itResource != desc.m_cachedResources.end())
            {
                return std::static_pointer_cast<TResourceType>(itResource->second);
            }
            
            //Find a provider that can load this resource
            ResourceProvider* provider = FindProvider(in_filePath, desc);
            if(provider == nullptr)
            {
                return nullptr;
            }
            
            //Load the resource
            ResourceSPtr resource(TResourceType::Create());
            provider->CreateResourceFromFileAsync(in_location, in_filePath, resource);
            
            if(resource->GetLoadState() == Resource::LoadState::k_failed)
            {
                CS_LOG_ERROR("Failed to create resource for " + in_filePath);
                return nullptr;
            }
            
            //Add it to the cache
            desc.m_cachedResources.insert(std::make_pair(resourceId, resource));
            
            return std::static_pointer_cast<TResourceType>(resource);
        }
        //-------------------------------------------------------------------------------------
        /// Resources often have references to other resources and therefore multiple release passes
        /// are required until no more resources are released
        //-------------------------------------------------------------------------------------
        template <typename TResourceType> void ResourcePool::ReleaseUnused()
        {
            //Find the descriptor that handles this type of resource
            auto itDescriptor = m_descriptors.find(TResourceType::InterfaceIDType);
            CS_ASSERT(itDescriptor != m_descriptors.end(), "Failed to find resource provider for " + TResourceType::TypeName);

            auto& cachedResources(itDescriptor->second.m_cachedResources);
            
            u32 numReleased = 0;
            
            do
            {
                numReleased = 0;
                
                for(auto itResource = cachedResources.begin(); itResource != cachedResources.end(); /*NO INCREMENT*/)
                {
                    if(itResource->second.use_count() == 1)
                    {
                        //The pool is the sole owner so we can safely release the object
                        CS_LOG_DEBUG("Releasing resource from pool " + itResource->second->GetName());
                        itResource = cachedResources.erase(itResource);
                        numReleased++;
                    }
                    else
                    {
                        ++itResource;
                    }
                }
            }
            while(numReleased > 0);
        }
    }
}

#endif
