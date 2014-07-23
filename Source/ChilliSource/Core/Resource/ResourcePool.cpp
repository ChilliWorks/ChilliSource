//
//  ResourcePool.cpp
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

#include <ChilliSource/Core/Resource/ResourcePool.h>

#include <ChilliSource/Core/Resource/ResourceProvider.h>

namespace ChilliSource
{
    namespace Core
    {
        CS_DEFINE_NAMEDTYPE(ResourcePool);
        
        //------------------------------------------------------------------------------------
        //------------------------------------------------------------------------------------
        ResourcePoolUPtr ResourcePool::Create()
        {
            return ResourcePoolUPtr(new ResourcePool());
        }
        //------------------------------------------------------------------------------------
        //------------------------------------------------------------------------------------
        bool ResourcePool::IsA(InterfaceIDType in_interfaceId) const
        {
            return in_interfaceId == ResourcePool::InterfaceID;
        }
        //------------------------------------------------------------------------------------
        //------------------------------------------------------------------------------------
        void ResourcePool::AddProvider(ResourceProvider* in_provider)
        {
            CS_ASSERT(in_provider != nullptr, "Cannot add null resource provider to pool");
            auto itDescriptor = m_descriptors.find(in_provider->GetResourceType());
            
            if(itDescriptor == m_descriptors.end())
            {
                PoolDesc desc;
                desc.m_providers.push_back(in_provider);
                m_descriptors.insert(std::make_pair(in_provider->GetResourceType(), desc));
            }
            else
            {
                itDescriptor->second.m_providers.push_back(in_provider);
            }
        }
        //------------------------------------------------------------------------------------
        //------------------------------------------------------------------------------------
        ResourceProvider* ResourcePool::FindProvider(const std::string& in_filePath, const PoolDesc& in_desc)
        {
            //Find the resource provider that can handle this extension
            std::string fileName;
            std::string fileExtension;
            Core::StringUtils::SplitBaseFilename(in_filePath, fileName, fileExtension);
            
            u32 numProviders = in_desc.m_providers.size();
            for(u32 i=0; i<numProviders; ++i)
            {
                if(in_desc.m_providers[i]->CanCreateResourceWithFileExtension(fileExtension))
                {
                    return in_desc.m_providers[i];
                }
            }
            
            CS_LOG_ERROR("Failed to find resource provider for extension " + fileExtension);
            return nullptr;
        }
        //------------------------------------------------------------------------------------
        //------------------------------------------------------------------------------------
        Resource::ResourceId ResourcePool::GenerateResourceId(const std::string& in_uniqueId) const
        {
            return HashCRC32::GenerateHashCode(in_uniqueId);
        }
        //------------------------------------------------------------------------------------
        //------------------------------------------------------------------------------------
        Resource::ResourceId ResourcePool::GenerateResourceId(StorageLocation in_location, const std::string& in_filePath, const IResourceOptionsBaseCSPtr& in_options) const
        {
            std::string combined(ToString((u32)in_location) + in_filePath);
            u32 fileHash = HashCRC32::GenerateHashCode(combined);
            
            if(in_options == nullptr)
            {
                return fileHash;
            }
            
            u32 optionsHash = in_options->GenerateHash();
            
            u64 combinedHash = fileHash + ((u64)(optionsHash) << 32);
            return HashCRC32::GenerateHashCode((const s8*)&combinedHash, sizeof(u64));
        }
        //-------------------------------------------------------------------------------------
        /// Resources often have references to other resources and therefore multiple release passes
        /// are required until no more resources are released
        //-------------------------------------------------------------------------------------
        void ResourcePool::ReleaseAllUnused()
        {
            CS_ASSERT(Application::Get()->GetTaskScheduler()->IsMainThread() == true, "Resources can only be released on the main thread");
            
            std::unique_lock<std::mutex> lock(m_mutex);
            u32 numReleased = 0;
            
            do
            {
                numReleased = 0;
                
                for(auto& descEntry : m_descriptors)
                {
                    for(auto itResource = descEntry.second.m_cachedResources.begin(); itResource != descEntry.second.m_cachedResources.end(); /*NO INCREMENT*/)
                    {
                        if(itResource->second.use_count() == 1)
                        {
                            //The pool is the sole owner so we can safely release the object
                            CS_LOG_VERBOSE("Releasing resource from pool " + itResource->second->GetName());
                            itResource = descEntry.second.m_cachedResources.erase(itResource);
                            numReleased++;
                        }
                        else
                        {
                            ++itResource;
                        }
                    }
                }
            }
            while(numReleased > 0);
        }
        //-------------------------------------------------------------------------------------
        //-------------------------------------------------------------------------------------
        void ResourcePool::Release(const Resource* in_resource)
        {
            CS_ASSERT(Application::Get()->GetTaskScheduler()->IsMainThread() == true, "Resources can only be released on the main thread");
            CS_ASSERT(in_resource != nullptr, "Pool cannot release null resource");
            //Find the descriptor that handles this type of resource
            
            std::unique_lock<std::mutex> lock(m_mutex);
            auto itDescriptor = m_descriptors.find(in_resource->GetInterfaceID());
            CS_ASSERT(itDescriptor != m_descriptors.end(), "Failed to find resource pool for " + in_resource->GetInterfaceTypeName());
            
            auto& cachedResources(itDescriptor->second.m_cachedResources);
            
            for(auto itResource = cachedResources.begin(); itResource != cachedResources.end(); /*NO INCREMENT*/)
            {
                if(itResource->second.get() == in_resource)
                {
					ResourceSPtr& resource = itResource->second;
					CS_ASSERT((resource.use_count() <= 1), "Cannot release a resource if it is owned by another object (i.e. use_count > 0) : (" + resource->GetName() + ")");
					CS_LOG_VERBOSE("Releasing resource from pool " + resource->GetName());
					cachedResources.erase(itResource);
                    return;
                }
                else
                {
                    ++itResource;
                }
            }
        }
        //------------------------------------------------------------------------------------
        /// At this stage in the app lifecycle all app and system references to resource
        /// should have been released. If the resource pool still has resources then this
        /// indicated leaks.
        //------------------------------------------------------------------------------------
        void ResourcePool::Destroy()
        {
            ReleaseAllUnused();
            
            bool error = false;
            
            for(auto& descEntry : m_descriptors)
            {
                for(auto itResource = descEntry.second.m_cachedResources.begin(); itResource != descEntry.second.m_cachedResources.end(); ++itResource)
                {
                    //The pool is the sole owner so we can safely release the object
                    CS_LOG_ERROR("Resource still in use: " + itResource->second->GetName());
					error = true;
                }
            }
            
            if(error == true)
            {
                CS_LOG_FATAL("Resources are still in use. Indicates that there is leaky references");
            }
        }
        //------------------------------------------------------------------------------------
        //------------------------------------------------------------------------------------
        void ResourcePool::OnMemoryWarning()
        {
            ReleaseAllUnused();
        }
    }
}
