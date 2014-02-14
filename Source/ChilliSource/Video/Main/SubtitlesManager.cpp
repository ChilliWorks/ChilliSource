/*
 *  SubtitlesManager.cpp
 *  moFlow
 *
 *  Created by Ian Copland 21/02/2013.
 *  Copyright 2013 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Video/Main/SubtitlesManager.h>
#include <ChilliSource/Video/Main/Subtitles.h>
#include <ChilliSource/Core/Resource/ResourceProvider.h>

namespace moFlo
{
	namespace Video
	{
		DEFINE_NAMED_INTERFACE(CSubtitlesManager);
		
		//----------------------------------------------------------------
		/// Is A
		//----------------------------------------------------------------
		bool CSubtitlesManager::IsA(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == CSubtitlesManager::InterfaceID;
		}
		//----------------------------------------------------------------
		/// Get Resource Type
		//----------------------------------------------------------------
		Core::InterfaceIDType CSubtitlesManager::GetResourceType() const
		{
			return CSubtitles::InterfaceID;
		}
		//----------------------------------------------------------------
		/// Manages Resource Of Type
		//----------------------------------------------------------------
		bool CSubtitlesManager::ManagesResourceOfType(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == CSubtitles::InterfaceID;
		}
		//----------------------------------------------------------------
		/// Get Provider Type
		//----------------------------------------------------------------
		Core::InterfaceIDType CSubtitlesManager::GetProviderType() const
		{
			return CSubtitles::InterfaceID;
		}
		//-----------------------------------------------------------------
		/// Get Resource From File
		//-----------------------------------------------------------------
		Core::ResourcePtr CSubtitlesManager::GetResourceFromFile(Core::STORAGE_LOCATION ineStorageLocation, const std::string &instrFilePath)
		{
			return GetSubtitlesFromFile(ineStorageLocation, instrFilePath);
		}
		//-----------------------------------------------------------------
		/// Async Get Resource From File
		//-----------------------------------------------------------------
		Core::ResourcePtr CSubtitlesManager::AsyncGetResourceFromFile(Core::STORAGE_LOCATION ineStorageLocation, const std::string &instrFilePath)
		{
			return AsyncGetSubtitlesFromFile(ineStorageLocation, instrFilePath);
		}
		//----------------------------------------------------------------
		/// Get Subtitles From File
		//----------------------------------------------------------------
		SubtitlesPtr CSubtitlesManager::GetSubtitlesFromFile(Core::STORAGE_LOCATION ineStorageLocation, const std::string & inFilePath)
		{
			MapStringToResourcePtr::iterator pExistingResource = mMapFilenameToResource.find(inFilePath);
			
			if(pExistingResource == mMapFilenameToResource.end()) 
			{
				Core::ResourcePtr pResource(new CSubtitles());
				for(u32 nProvider = 0; nProvider < mResourceProviders.size(); nProvider++) 
				{
					if(mResourceProviders[nProvider]->CreateResourceFromFile(ineStorageLocation, inFilePath, pResource))
					{
						DEBUG_LOG("Loading Subtitles " + inFilePath);
						mMapFilenameToResource.insert(std::make_pair(inFilePath, pResource));
						
						SubtitlesPtr pSubtitles = SHARED_PTR_CAST<CSubtitles>(pResource);
						pSubtitles->SetName(inFilePath);
						pSubtitles->SetOwningResourceManager(this);
						pSubtitles->SetFilename(inFilePath);
						pSubtitles->SetStorageLocation(ineStorageLocation);
						pSubtitles->SetLoaded(true);
						return pSubtitles;
					}
				}
			} 
			else 
			{
				return SHARED_PTR_CAST<CSubtitles>(pExistingResource->second);
			}
			
			ERROR_LOG("Cannot find resource for Subtitles with path " + inFilePath);
			return SubtitlesPtr();
		}
		//----------------------------------------------------------------
		/// Async Get Subtitles From File
		//----------------------------------------------------------------
		SubtitlesPtr CSubtitlesManager::AsyncGetSubtitlesFromFile(Core::STORAGE_LOCATION ineStorageLocation, const std::string & inFilePath)
		{
			MapStringToResourcePtr::iterator pExistingResource = mMapFilenameToResource.find(inFilePath);
			
			if(pExistingResource == mMapFilenameToResource.end()) 
			{
				Core::ResourcePtr pResource(new CSubtitles());
				for(u32 nProvider = 0; nProvider < mResourceProviders.size(); nProvider++) 
				{
					SubtitlesPtr pSubtitles = SHARED_PTR_CAST<CSubtitles>(pResource);
					pSubtitles->SetName(inFilePath);
					pSubtitles->SetFilename(inFilePath);
					pSubtitles->SetStorageLocation(ineStorageLocation);
					pSubtitles->SetOwningResourceManager(this);
					pSubtitles->SetLoaded(false);
					
					if(mResourceProviders[nProvider]->AsyncCreateResourceFromFile(ineStorageLocation, inFilePath, pResource))
					{
						DEBUG_LOG("Loading Subtitles " + inFilePath);
						mMapFilenameToResource.insert(std::make_pair(inFilePath, pResource));
						return pSubtitles;
					}
				}
			} 
			else 
			{
				return SHARED_PTR_CAST<CSubtitles>(pExistingResource->second);
			}
			
			ERROR_LOG("Cannot find resource for Subtitles with path " + inFilePath);
			return SubtitlesPtr();
		}
	}
}
