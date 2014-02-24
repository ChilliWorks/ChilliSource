/*
 *  SkeletalAnimationManager.cpp
 *  MoFlowSkeleton
 *
 *  Created by Ian Copland on 17/10/2011.
 *  Copyright 2011 Tag Games Ltd. All rights reserved.CSkinnedAnimation
 *
 */

#include <ChilliSource/Rendering/Model/SkinnedAnimationManager.h>
#include <ChilliSource/Core/Resource/ResourceProvider.h>
#include <ChilliSource/Core/Base/Application.h>


namespace ChilliSource
{
	namespace Rendering
	{
		
		DEFINE_NAMED_INTERFACE(CSkinnedAnimationManager);
		
		//-----------------------------------------------------------------
		/// Constructor
		//-----------------------------------------------------------------
		CSkinnedAnimationManager::CSkinnedAnimationManager()
		{
		}
		//----------------------------------------------------------------
		/// Is A
		//----------------------------------------------------------------
		bool CSkinnedAnimationManager::IsA(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == CSkinnedAnimationManager::InterfaceID;
		}
		//----------------------------------------------------------------
		/// Get Resource Type
		//----------------------------------------------------------------
		Core::InterfaceIDType CSkinnedAnimationManager::GetResourceType() const
		{
			return CSkinnedAnimation::InterfaceID;
		}
		//----------------------------------------------------------------
		/// Manages Resource Of Type
		//----------------------------------------------------------------
		bool CSkinnedAnimationManager::ManagesResourceOfType(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == CSkinnedAnimation::InterfaceID;
		}
		//----------------------------------------------------------------
		/// Get Provider Type
		//----------------------------------------------------------------
		Core::InterfaceIDType CSkinnedAnimationManager::GetProviderType() const
		{
			return CSkinnedAnimation::InterfaceID;
		}
		//-----------------------------------------------------------------
		/// Get Resource From File
		//-----------------------------------------------------------------
		Core::ResourcePtr CSkinnedAnimationManager::GetResourceFromFile(Core::StorageLocation ineStorageLocation, const std::string &instrFilePath)
		{
			return GetSkinnedAnimationFromFile(ineStorageLocation, instrFilePath);
		}
		//-----------------------------------------------------------------
		/// Async Get Resource From File
		//-----------------------------------------------------------------
		Core::ResourcePtr CSkinnedAnimationManager::AsyncGetResourceFromFile(Core::StorageLocation ineStorageLocation, const std::string &instrFilePath)
		{
			return AsyncGetSkinnedAnimationFromFile(ineStorageLocation, instrFilePath);
		}
		//-----------------------------------------------------------------
		/// Get Skinned Animation From File
		//-----------------------------------------------------------------
		SkinnedAnimationPtr CSkinnedAnimationManager::GetSkinnedAnimationFromFile(Core::StorageLocation ineStorageLocation, const std::string &instrFilePath)
		{
			MapStringToResourcePtr::iterator pExistingResource = mMapFilenameToResource.find(instrFilePath);
			
			if(pExistingResource == mMapFilenameToResource.end()) 
			{
				Core::ResourcePtr pResource(new CSkinnedAnimation());
				
				for(u32 nProvider = 0; nProvider < mResourceProviders.size(); nProvider++) 
				{
					//Check if the resource provider can load this extension
					std::string strName, strExt;
					Core::CStringUtils::SplitBaseFilename(instrFilePath, strName, strExt);
					
					if(mResourceProviders[nProvider]->CanCreateResourceFromFileWithExtension(strExt))
					{
						if(mResourceProviders[nProvider]->CreateResourceFromFile(ineStorageLocation, instrFilePath, pResource)) 
						{
							//Add it to the cache
							DEBUG_LOG("Loading skinned animation " + instrFilePath);
							mMapFilenameToResource.insert(std::make_pair(instrFilePath , pResource));
							pResource->SetName(instrFilePath);
							pResource->SetFilename(instrFilePath);
							pResource->SetStorageLocation(ineStorageLocation);
							pResource->SetOwningResourceManager(static_cast<Core::IResourceManager*>(this));
							pResource->SetLoaded(true);
							
							return SHARED_PTR_CAST<CSkinnedAnimation>(pResource);
						}
					}
				}
			} 
			else 
			{
				return SHARED_PTR_CAST<CSkinnedAnimation>(pExistingResource->second);
			}
			
			//Resource not found
			ERROR_LOG("Cannot find resource for skinned animation with path " + instrFilePath);
			return SkinnedAnimationPtr();
		}
		//-----------------------------------------------------------------
		/// Async Get Skinned Animation From File
		//-----------------------------------------------------------------
		SkinnedAnimationPtr CSkinnedAnimationManager::AsyncGetSkinnedAnimationFromFile(Core::StorageLocation ineStorageLocation, const std::string &instrFilePath)
		{
			MapStringToResourcePtr::iterator pExistingResource = mMapFilenameToResource.find(instrFilePath);
			
			if(pExistingResource == mMapFilenameToResource.end()) 
			{
				Core::ResourcePtr pResource(new CSkinnedAnimation());
				
				for(u32 nProvider = 0; nProvider < mResourceProviders.size(); nProvider++) 
				{
					//Check if the resource provider can load this extension
					std::string strName, strExt;
					Core::CStringUtils::SplitBaseFilename(instrFilePath, strName, strExt);
					
					if(mResourceProviders[nProvider]->CanCreateResourceFromFileWithExtension(strExt))
					{
						
						DEBUG_LOG("Loading skinned animation " + instrFilePath);
						
						pResource->SetName(instrFilePath);
						pResource->SetFilename(instrFilePath);
						pResource->SetStorageLocation(ineStorageLocation);
						pResource->SetOwningResourceManager(this);
						pResource->SetLoaded(false);
						
						if(mResourceProviders[nProvider]->AsyncCreateResourceFromFile(ineStorageLocation, instrFilePath, pResource)) 
						{
							//Add it to the cache
							mMapFilenameToResource.insert(std::make_pair(instrFilePath, pResource));
							return SHARED_PTR_CAST<CSkinnedAnimation>(pResource);
						}
					}
				}
			} 
			else 
			{
				return SHARED_PTR_CAST<CSkinnedAnimation>(pExistingResource->second);
			}
			
			//Resource not found
			ERROR_LOG("Cannot find resource for skinned animation with path " + instrFilePath);
			return SkinnedAnimationPtr();
		}
		//-----------------------------------------------------------------
		/// Destructor
		///
		//-----------------------------------------------------------------
		CSkinnedAnimationManager::~CSkinnedAnimationManager()
		{
		}
	}
}
