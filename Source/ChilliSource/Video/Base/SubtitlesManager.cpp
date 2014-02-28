/*
 *  SubtitlesManager.cpp
 *  moFlow
 *
 *  Created by Ian Copland 21/02/2013.
 *  Copyright 2013 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Video/Base/SubtitlesManager.h>
#include <ChilliSource/Video/Base/Subtitles.h>
#include <ChilliSource/Core/Resource/ResourceProvider.h>

namespace ChilliSource
{
	namespace Video
	{
		DEFINE_NAMED_INTERFACE(SubtitlesManager);
		
		//----------------------------------------------------------------
		/// Is A
		//----------------------------------------------------------------
		bool SubtitlesManager::IsA(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == SubtitlesManager::InterfaceID;
		}
		//----------------------------------------------------------------
		/// Get Resource Type
		//----------------------------------------------------------------
		Core::InterfaceIDType SubtitlesManager::GetResourceType() const
		{
			return Subtitles::InterfaceID;
		}
		//----------------------------------------------------------------
		/// Manages Resource Of Type
		//----------------------------------------------------------------
		bool SubtitlesManager::ManagesResourceOfType(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == Subtitles::InterfaceID;
		}
		//----------------------------------------------------------------
		/// Get Provider Type
		//----------------------------------------------------------------
		Core::InterfaceIDType SubtitlesManager::GetProviderType() const
		{
			return Subtitles::InterfaceID;
		}
		//-----------------------------------------------------------------
		/// Get Resource From File
		//-----------------------------------------------------------------
		Core::ResourceSPtr SubtitlesManager::GetResourceFromFile(Core::StorageLocation ineStorageLocation, const std::string &instrFilePath)
		{
			return GetSubtitlesFromFile(ineStorageLocation, instrFilePath);
		}
		//-----------------------------------------------------------------
		/// Async Get Resource From File
		//-----------------------------------------------------------------
		Core::ResourceSPtr SubtitlesManager::AsyncGetResourceFromFile(Core::StorageLocation ineStorageLocation, const std::string &instrFilePath)
		{
			return AsyncGetSubtitlesFromFile(ineStorageLocation, instrFilePath);
		}
		//----------------------------------------------------------------
		/// Get Subtitles From File
		//----------------------------------------------------------------
		SubtitlesSPtr SubtitlesManager::GetSubtitlesFromFile(Core::StorageLocation ineStorageLocation, const std::string & inFilePath)
		{
			MapStringToResourcePtr::iterator pExistingResource = mMapFilenameToResource.find(inFilePath);
			
			if(pExistingResource == mMapFilenameToResource.end()) 
			{
				Core::ResourceSPtr pResource(new Subtitles());
				for(u32 nProvider = 0; nProvider < mResourceProviders.size(); nProvider++) 
				{
					if(mResourceProviders[nProvider]->CreateResourceFromFile(ineStorageLocation, inFilePath, pResource))
					{
						CS_LOG_DEBUG("Loading Subtitles " + inFilePath);
						mMapFilenameToResource.insert(std::make_pair(inFilePath, pResource));
						
						SubtitlesSPtr pSubtitles = std::static_pointer_cast<Subtitles>(pResource);
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
				return std::static_pointer_cast<Subtitles>(pExistingResource->second);
			}
			
			CS_LOG_ERROR("Cannot find resource for Subtitles with path " + inFilePath);
			return SubtitlesSPtr();
		}
		//----------------------------------------------------------------
		/// Async Get Subtitles From File
		//----------------------------------------------------------------
		SubtitlesSPtr SubtitlesManager::AsyncGetSubtitlesFromFile(Core::StorageLocation ineStorageLocation, const std::string & inFilePath)
		{
			MapStringToResourcePtr::iterator pExistingResource = mMapFilenameToResource.find(inFilePath);
			
			if(pExistingResource == mMapFilenameToResource.end()) 
			{
				Core::ResourceSPtr pResource(new Subtitles());
				for(u32 nProvider = 0; nProvider < mResourceProviders.size(); nProvider++) 
				{
					SubtitlesSPtr pSubtitles = std::static_pointer_cast<Subtitles>(pResource);
					pSubtitles->SetName(inFilePath);
					pSubtitles->SetFilename(inFilePath);
					pSubtitles->SetStorageLocation(ineStorageLocation);
					pSubtitles->SetOwningResourceManager(this);
					pSubtitles->SetLoaded(false);
					
					if(mResourceProviders[nProvider]->AsyncCreateResourceFromFile(ineStorageLocation, inFilePath, pResource))
					{
						CS_LOG_DEBUG("Loading Subtitles " + inFilePath);
						mMapFilenameToResource.insert(std::make_pair(inFilePath, pResource));
						return pSubtitles;
					}
				}
			} 
			else 
			{
				return std::static_pointer_cast<Subtitles>(pExistingResource->second);
			}
			
			CS_LOG_ERROR("Cannot find resource for Subtitles with path " + inFilePath);
			return SubtitlesSPtr();
		}
	}
}
