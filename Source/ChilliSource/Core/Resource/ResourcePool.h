//
//  ResourcePool.h
//  Chilli Source
//  Created by Scott Downie on 03/04/2014.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2014 Tag Games Limited
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//

#ifndef _CHILLISOURCE_CORE_RESOURCE_RESOURCEPOOL_H_
#define _CHILLISOURCE_CORE_RESOURCE_RESOURCEPOOL_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/File/TaggedFilePathResolver.h>
#include <ChilliSource/Core/Resource/IResourceOptions.h>
#include <ChilliSource/Core/Resource/Resource.h>
#include <ChilliSource/Core/Resource/ResourceProvider.h>
#include <ChilliSource/Core/System/AppSystem.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>

#include <functional>
#include <mutex>
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
            /// @author S Downie
            ///
            /// @param Resource provider that will be assigned to load a specific resource type
            //------------------------------------------------------------------------------------
            void AddProvider(ResourceProvider* in_provider);
            //------------------------------------------------------------------------------------
            /// Creates an unloaded mutable resource with the given Id. The resource will be
            /// cached by the pool. The responsibility for loading or building the resource
            /// lies with the app code. Attempting to create a resource with an exisiting Id will
            /// cause an assertion.
            ///
            /// The resource is mutable
            ///
            /// NOTE: Resource should not start with '_'. This is reserved for internal engine
            /// resources.
            ///
            /// @author S Downie
            ///
            /// @param Unique Id
            ///
            /// @return Resource or null
            //-------------------------------------------------------------------------------------
            template <typename TResourceType> std::shared_ptr<TResourceType> CreateResource(const std::string& in_uniqueId);
            //------------------------------------------------------------------------------------
            /// Return the cached resource with the given unique ID. If the resource
            /// does not exist then null is returned.
            ///
            /// The resource is immutable
            ///
            /// @author S Downie
            ///
            /// @param Unique ID
            ///
            /// @return Resource or null
            //-------------------------------------------------------------------------------------
            template <typename TResourceType> std::shared_ptr<const TResourceType> GetResource(const std::string& in_uniqueId) const;
            //------------------------------------------------------------------------------------
            /// Returns all cached resources of the requested type.  The resources are immutable.
            ///
            /// @author I Copland
            ///
            /// @return A list of all resource of the requested type.
            //-------------------------------------------------------------------------------------
            template <typename TResourceType> std::vector<std::shared_ptr<const TResourceType>> GetAllResources() const;
            //------------------------------------------------------------------------------------
            /// Load the resource of given type from the file location. If the resource at this
            /// location has previously been loaded then the cached version will be returned.
            /// Check the load state of the resources of that type prior to use.
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
            /// @param Load options that are used by the underlying providers to customise
            /// initialisation
            ///
            /// @return Resource or null
            //-------------------------------------------------------------------------------------
            template <typename TResourceType> std::shared_ptr<const TResourceType> LoadResource(StorageLocation in_location, const std::string& in_filePath, const IResourceOptionsCSPtr<TResourceType>& in_options = nullptr);
            //------------------------------------------------------------------------------------
            /// Forec the reload of the resource of given type from the file location. This will
            /// overwrite the currently cached version. This will fail if no cached resource
            /// is found. Check the load state of the resources of that type prior to use
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
            /// @param Load options that are used by the underlying providers to customise
            /// initialisation
            ///
            /// @return Resource or null
            //-------------------------------------------------------------------------------------
            template <typename TResourceType> std::shared_ptr<const TResourceType> RefreshResource(StorageLocation in_location, const std::string& in_filePath, const IResourceOptionsCSPtr<TResourceType>& in_options = nullptr);
            //------------------------------------------------------------------------------------
            /// Force the reload of the resources of the given type that are currently cached.
            /// Check the load state of the resources of that type prior to use, although if they
            /// have loaded once then chances are they will load again.
            ///
            /// @author S Downie
            //-------------------------------------------------------------------------------------
            template <typename TResourceType> void RefreshResources();
            //------------------------------------------------------------------------------------
            /// Load the resource of given type from the file location. If the resource at this
            /// location has previously been loaded then the cached version will be returned in the
            /// delegate.
            ///
            /// The resource is immutable
            ///
            /// If the file requires loading it will be done on a background thread. The resource
            /// will be returned in the delegate. Check the resource load state to see
            /// if it has loaded successfully or not. NOTE: The resource may be null which also
            /// indicates failure
            ///
            /// @author S Downie
            ///
            /// @param Storage location
            /// @param File path
            /// @param Load options that are used by the underlying providers to customise
            /// initialisation
            /// @param Delegate to trigger when the resource is loaded or failed. Note: Always
            /// called on the main thread
            //-------------------------------------------------------------------------------------
            template <typename TResourceType> void LoadResourceAsync(StorageLocation in_location, const std::string& in_filePath, const IResourceOptionsCSPtr<TResourceType>& in_options, const std::function<void(const std::shared_ptr<const TResourceType>&)>& in_delegate);
            //------------------------------------------------------------------------------------
            /// Load the resource of given type from the file location. If the resource at this
            /// location has previously been loaded then the cached version will be returned in the
            /// delegate.
            ///
            /// The resource is immutable
            ///
            /// If the file requires loading it will be done on a background thread. The resource
            /// will be returned in the delegate. Check the resource load state to see
            /// if it has loaded successfully or not. NOTE: The resource may be null which also
            /// indicates failure
            ///
            /// @author S Downie
            ///
            /// @param Storage location
            /// @param File path
            /// @param Delegate to trigger when the resource is loaded or failed. Note: Always
            /// called on the main thread
            //-------------------------------------------------------------------------------------
            template <typename TResourceType> void LoadResourceAsync(StorageLocation in_location, const std::string& in_filePath, const std::function<void(const std::shared_ptr<const TResourceType>&)>& in_delegate);
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
            ///  rawCPtr = sharedPtr.get();
            ///  sharedPtr.reset();
            ///  pool->Release(rawCPtr);
            ///
            ///
            /// @author S Downie
            ///
            /// @param Resource to release
			//-------------------------------------------------------------------------------------
            void Release(const Resource* in_resource);
            //------------------------------------------------------------------------------------
            /// Called when the system is destroyed after the system lifecycle destroy.
            /// Flushes the resource caches and errors if any resources are still in use
            ///
            /// @author S Downie
            //------------------------------------------------------------------------------------
            void Destroy();
            
        private:
            
            //-------------------------------------------------------------------------------------
            /// Descriptor that holds the providers and cached resources for a given type
            ///
            /// @author S Downie
            //-------------------------------------------------------------------------------------
            struct PoolDesc
            {
                std::vector<ResourceProvider*> m_providers;
                std::unordered_map<Resource::ResourceId, ResourceSPtr> m_cachedResources;
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
            /// @param Loading options
            ///
            /// @return Unique ID based on the location, path and options
            //------------------------------------------------------------------------------------
            Resource::ResourceId GenerateResourceId(StorageLocation in_location, const std::string& in_filePath, const IResourceOptionsBaseCSPtr& in_options) const;
            //------------------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @param Unique string ID
            ///
            /// @return Unique ID based on the location and path
            //------------------------------------------------------------------------------------
            Resource::ResourceId GenerateResourceId(const std::string& in_uniqueId) const;
            
        private:
            
            std::unordered_map<InterfaceIDType, PoolDesc> m_descriptors;
            mutable std::mutex m_mutex;
        };
        //------------------------------------------------------------------------------------
        //-------------------------------------------------------------------------------------
        template <typename TResourceType> std::shared_ptr<const TResourceType> ResourcePool::GetResource(const std::string& in_uniqueId) const
        {
            CS_ASSERT(in_uniqueId.empty() == false, "Cannot find resource with empty unique Id");
            
            std::unique_lock<std::mutex> lock(m_mutex);
            
            auto itDescriptor = m_descriptors.find(TResourceType::InterfaceID);
            
            if(itDescriptor == m_descriptors.end())
            {
                CS_LOG_ERROR("Failed to find resource provider for " + TResourceType::TypeName);
                return nullptr;
            }
            
            const PoolDesc& desc(itDescriptor->second);
            
            //Check descriptor and see if this resource already exists
            Resource::ResourceId resourceId = GenerateResourceId(in_uniqueId);
            auto itResource = desc.m_cachedResources.find(resourceId);
            if(itResource != desc.m_cachedResources.end())
            {
                return std::static_pointer_cast<const TResourceType>(itResource->second);
            }
            
            return nullptr;
        }
        //------------------------------------------------------------------------------------
        //-------------------------------------------------------------------------------------
        template <typename TResourceType> std::vector<std::shared_ptr<const TResourceType>> ResourcePool::GetAllResources() const
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            
            std::vector<std::shared_ptr<const TResourceType>> output;
            
            auto itDescriptor = m_descriptors.find(TResourceType::InterfaceID);
            if(itDescriptor != m_descriptors.end())
            {
                const PoolDesc& desc(itDescriptor->second);
                for (const auto& resource : desc.m_cachedResources)
                {
                    output.push_back(std::static_pointer_cast<const TResourceType>(resource.second));
                }
            }
            
            return output;
        }
        //------------------------------------------------------------------------------------
        //-------------------------------------------------------------------------------------
        template <typename TResourceType> std::shared_ptr<TResourceType> ResourcePool::CreateResource(const std::string& in_uniqueId)
        {
            CS_ASSERT(Application::Get()->GetTaskScheduler()->IsMainThread() == true, "Resources can only be created on the main thread");
            
            Resource::ResourceId resourceId = GenerateResourceId(in_uniqueId);
            std::shared_ptr<TResourceType> resource(TResourceType::Create());
            resource->SetId(resourceId);
            resource->SetName(in_uniqueId);
            
            std::unique_lock<std::mutex> lock(m_mutex);
            
            auto itDescriptor = m_descriptors.find(TResourceType::InterfaceID);
            if(itDescriptor == m_descriptors.end())
            {
                PoolDesc desc;
                desc.m_cachedResources.insert(std::make_pair(resourceId, resource));
                m_descriptors.insert(std::make_pair(TResourceType::InterfaceID, desc));
                return resource;
            }
            
            //Check to make sure this doesn't already exist
            PoolDesc& desc(itDescriptor->second);
            CS_ASSERT(desc.m_cachedResources.find(resourceId) == desc.m_cachedResources.end(), "Resource with Id: " + in_uniqueId + " already exists");
            desc.m_cachedResources.insert(std::make_pair(resourceId, resource));
            
            return resource;
        }
        //-------------------------------------------------------------------------------------
        //-------------------------------------------------------------------------------------
        template <typename TResourceType> std::shared_ptr<const TResourceType> ResourcePool::LoadResource(StorageLocation in_location, const std::string& in_filePath, const IResourceOptionsCSPtr<TResourceType>& in_options)
        {
            CS_ASSERT(Application::Get()->GetTaskScheduler()->IsMainThread() == true, "Resources can only be loaded on the main thread - use LoadResourceAsync");
            CS_ASSERT(in_filePath.empty() == false, "Cannot load resource with no file path");
            
            std::unique_lock<std::mutex> lock(m_mutex);
            auto itDescriptor = m_descriptors.find(TResourceType::InterfaceID);
            
            if(itDescriptor == m_descriptors.end())
            {
                CS_LOG_ERROR("Failed to find resource provider for " + TResourceType::TypeName);
                return nullptr;
            }
            
            PoolDesc& desc(itDescriptor->second);
            
            //Find a provider that can load this resource
			ResourceProvider* provider = FindProvider(in_filePath, desc);
            if(provider == nullptr)
            {
                return nullptr;
            }
            lock.unlock();
            
            IResourceOptionsBaseCSPtr options(in_options);
            if(options == nullptr)
            {
                options = provider->GetDefaultOptions();
            }
            
            //Check descriptor and see if this resource already exists
			Resource::ResourceId resourceId = GenerateResourceId(in_location, in_filePath, options);
            
            lock.lock();
            auto itResource = desc.m_cachedResources.find(resourceId);
            if(itResource != desc.m_cachedResources.end())
            {
                return std::static_pointer_cast<TResourceType>(itResource->second);
            }
            lock.unlock();
            
            //Load the resource
            ResourceSPtr resource(TResourceType::Create());
            resource->SetStorageLocation(in_location);
			resource->SetFilePath(in_filePath);
			resource->SetName(in_filePath);
            resource->SetOptions(options);
            resource->SetId(resourceId);

			std::string deviceFilePath = Application::Get()->GetTaggedFilePathResolver()->ResolveFilePath(in_location, in_filePath);
            provider->CreateResourceFromFile(in_location, deviceFilePath, options, resource);
            if(resource->GetLoadState() != Resource::LoadState::k_loaded)
            {
				CS_LOG_ERROR("Failed to create resource for " + in_filePath);
                return nullptr;
            }
            
            lock.lock();
            //Check the async call hasn't sneaked in here with the same resource
            itResource = desc.m_cachedResources.find(resourceId);
            if(itResource == desc.m_cachedResources.end())
            {
                desc.m_cachedResources.insert(std::make_pair(resourceId, resource));
            }
            else
            {
                resource = itResource->second;
            }
            lock.unlock();
            
            return std::static_pointer_cast<TResourceType>(resource);
        }
        //-------------------------------------------------------------------------------------
        //-------------------------------------------------------------------------------------
        template <typename TResourceType> std::shared_ptr<const TResourceType> ResourcePool::RefreshResource(StorageLocation in_location, const std::string& in_filePath, const IResourceOptionsCSPtr<TResourceType>& in_options)
        {
            CS_ASSERT(Application::Get()->GetTaskScheduler()->IsMainThread() == true, "Resources can only be refreshed on the main thread");
            CS_ASSERT(in_filePath.empty() == false, "Cannot refresh resource with no file path");
            
			std::unique_lock<std::mutex> lock(m_mutex);
            auto itDescriptor = m_descriptors.find(TResourceType::InterfaceID);
            
            if(itDescriptor == m_descriptors.end())
            {
                CS_LOG_ERROR("Failed to find resource provider for " + TResourceType::TypeName);
                return nullptr;
            }
            
            PoolDesc& desc(itDescriptor->second);
            
            //Find a provider that can load this resource
			ResourceProvider* provider = FindProvider(in_filePath, desc);
            if(provider == nullptr)
            {
                return nullptr;
            }
            lock.unlock();
            
            IResourceOptionsBaseCSPtr options(in_options);
            if(options == nullptr)
            {
                options = provider->GetDefaultOptions();
            }
            
            //Check descriptor and see if this resource already exists
			Resource::ResourceId resourceId = GenerateResourceId(in_location, in_filePath, options);
            
            lock.lock();
            auto itResource = desc.m_cachedResources.find(resourceId);
            if(itResource == desc.m_cachedResources.end())
            {
				CS_LOG_ERROR("Failed to refresh non-existing resource for " + in_filePath);
                return nullptr;
            }
            
            //Load the resource
            ResourceSPtr resource(itResource->second);
            
            lock.unlock();
            
            resource->SetLoadState(Resource::LoadState::k_loading);
			std::string deviceFilePath = Application::Get()->GetTaggedFilePathResolver()->ResolveFilePath(in_location, in_filePath);
            provider->CreateResourceFromFile(in_location, deviceFilePath, options, resource);
            if(resource->GetLoadState() != Resource::LoadState::k_loaded)
            {
                CS_LOG_ERROR("Failed to refresh resource for " + deviceFilePath);
                return nullptr;
            }
            
            return std::static_pointer_cast<TResourceType>(resource);
        }
        //------------------------------------------------------------------------------------
        //-------------------------------------------------------------------------------------
        template <typename TResourceType> void ResourcePool::RefreshResources()
        {
            CS_ASSERT(Application::Get()->GetTaskScheduler()->IsMainThread() == true, "Resources can only be refreshed on the main thread");
            
            std::unique_lock<std::mutex> lock(m_mutex);
            
            auto itDescriptor = m_descriptors.find(TResourceType::InterfaceID);
            
            if(itDescriptor == m_descriptors.end())
            {
                CS_LOG_ERROR("Failed to find resource provider for " + TResourceType::TypeName);
                return;
            }
            
            PoolDesc& desc(itDescriptor->second);
            
            for(auto& resourceEntry : desc.m_cachedResources)
            {
                ResourceSPtr& resource(resourceEntry.second);
                
                if(resource->GetStorageLocation() != CSCore::StorageLocation::k_none)
                {
                    //Find a provider that can load this resource
                    ResourceProvider* provider = FindProvider(resource->GetFilePath(), desc);
                    if(provider == nullptr)
                    {
                        CS_LOG_ERROR("Failed to find resource provider for " + resource->GetName());
                        continue;
                    }
                    
                    resource->SetLoadState(Resource::LoadState::k_loading);
                    std::string deviceFilePath = Application::Get()->GetTaggedFilePathResolver()->ResolveFilePath(resource->GetStorageLocation(), resource->GetFilePath());
                    provider->CreateResourceFromFile(resource->GetStorageLocation(), deviceFilePath, resource->GetOptions(), resource);
                    if(resource->GetLoadState() != Resource::LoadState::k_loaded)
                    {
                        CS_LOG_ERROR("Failed to refresh resource for " + resource->GetName());
                        continue;
                    }
                }
            }
        }
        //-------------------------------------------------------------------------------------
        //-------------------------------------------------------------------------------------
        template <typename TResourceType> void ResourcePool::LoadResourceAsync(StorageLocation in_location, const std::string& in_filePath, const std::function<void(const std::shared_ptr<const TResourceType>&)>& in_delegate)
        {
            LoadResourceAsync(in_location, in_filePath, IResourceOptionsCSPtr<TResourceType>(), in_delegate);
        }
        //-------------------------------------------------------------------------------------
        //-------------------------------------------------------------------------------------
        template <typename TResourceType> void ResourcePool::LoadResourceAsync(StorageLocation in_location, const std::string& in_filePath, const IResourceOptionsCSPtr<TResourceType>& in_options, const std::function<void(const std::shared_ptr<const TResourceType>&)>& in_delegate)
        {
            CS_ASSERT(in_filePath.empty() == false, "Cannot load resource async with no file path");
            CS_ASSERT(in_delegate != nullptr, "Cannot load resource async with null delegate");
            
            std::unique_lock<std::mutex> lock(m_mutex);
            auto itDescriptor = m_descriptors.find(TResourceType::InterfaceID);
            
            if(itDescriptor == m_descriptors.end())
            {
                CS_LOG_ERROR("Failed to find resource provider for " + TResourceType::TypeName);
                in_delegate(nullptr);
                return;
            }
            
            PoolDesc& desc(itDescriptor->second);
            
            //Find a provider that can load this resource
			ResourceProvider* provider = FindProvider(in_filePath, desc);
            if(provider == nullptr)
            {
                in_delegate(nullptr);
                return;
            }
            lock.unlock();
            
            IResourceOptionsBaseCSPtr options(in_options);
            if(options == nullptr)
            {
                options = provider->GetDefaultOptions();
            }
            
            //Check descriptor and see if this resource already exists
			Resource::ResourceId resourceId = GenerateResourceId(in_location, in_filePath, options);
            
            lock.lock();
            auto itResource = desc.m_cachedResources.find(resourceId);
            if(itResource != desc.m_cachedResources.end())
            {
                in_delegate(std::static_pointer_cast<const TResourceType>(itResource->second));
                return;
            }
            
            //Load the resource
            ResourceSPtr resource(TResourceType::Create());
            resource->SetStorageLocation(in_location);
			resource->SetFilePath(in_filePath);
			resource->SetName(in_filePath);
            resource->SetOptions(options);
            resource->SetId(resourceId);

            //Add it to the cache
            desc.m_cachedResources.insert(std::make_pair(resourceId, resource));
            lock.unlock();
            
            ResourceProvider::AsyncLoadDelegate convertDelegate([=](const ResourceSPtr& in_resource)
            {
                in_delegate(std::static_pointer_cast<const TResourceType>(in_resource));
            });

			std::string deviceFilePath = Application::Get()->GetTaggedFilePathResolver()->ResolveFilePath(in_location, in_filePath);
            provider->CreateResourceFromFileAsync(in_location, deviceFilePath, options, convertDelegate, resource);
        }
        //-------------------------------------------------------------------------------------
        /// Resources often have references to other resources and therefore multiple release passes
        /// are required until no more resources are released
        //-------------------------------------------------------------------------------------
        template <typename TResourceType> void ResourcePool::ReleaseUnused()
        {
            CS_ASSERT(Application::Get()->GetTaskScheduler()->IsMainThread() == true, "Resources can only be released on the main thread");
            
            std::unique_lock<std::mutex> lock(m_mutex);
            //Find the descriptor that handles this type of resource
            auto itDescriptor = m_descriptors.find(TResourceType::InterfaceID);
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
                        CS_LOG_VERBOSE("Releasing resource from pool " + itResource->second->GetName());
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
