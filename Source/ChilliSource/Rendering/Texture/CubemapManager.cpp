/*
 *  CubemapManager.cpp
 *  moFlow
 *
 *  Created by Tag Games on 01/10/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Rendering/Texture/CubemapManager.h>
#include <ChilliSource/Rendering/Texture/Cubemap.h>

#include <ChilliSource/Core/Image/ImageProvider.h>
#include <ChilliSource/Core/Image/Image.h>
#include <ChilliSource/Core/Base/Colour.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>

namespace ChilliSource
{
	namespace Rendering
	{
		CS_DEFINE_NAMEDTYPE(CubemapManager);
		
		//----------------------------------------------------------------
		/// Constructor
		//----------------------------------------------------------------
		CubemapManager::CubemapManager()
		{
			
		}
		//----------------------------------------------------------------
		/// Is A
		///
		/// Query the interface type
		/// @param The interface to compare
		/// @return Whether the object implements that interface
		//----------------------------------------------------------------
		bool CubemapManager::IsA(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == CubemapManager::InterfaceID;
		}
		//----------------------------------------------------------------
		/// Get Resource Type
		///
		/// @return The type of resource this manager handles
		//----------------------------------------------------------------
		Core::InterfaceIDType CubemapManager::GetResourceType() const
		{
			return Cubemap::InterfaceID;
		}
		//----------------------------------------------------------------
		/// Manages Resource Of Type
		///
		/// @return Whether this object manages the object of type
		//----------------------------------------------------------------
		bool CubemapManager::ManagesResourceOfType(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == Cubemap::InterfaceID;
		}
		//----------------------------------------------------------------
		/// Get Provider Type
		///
		/// @return The type of resource it consumes from resource provider
		//----------------------------------------------------------------
		Core::InterfaceIDType CubemapManager::GetProviderType() const
		{
			return Core::Image::InterfaceID;
		}
		//-----------------------------------------------------------------
		/// Get Resource From File
		///
		/// Generic call to get the managers resource
        /// @param The storage location to load from
		/// @param File path to resource
		/// @return Generic pointer to object type
		//-----------------------------------------------------------------
		Core::ResourceOldSPtr CubemapManager::GetResourceFromFile(Core::StorageLocation ineStorageLocation, const std::string &instrFilePath)
		{
			MapStringToResourceSPtr::iterator pExistingResource = mMapFilenameToResource.find(instrFilePath);
			
			if(pExistingResource == mMapFilenameToResource.end())
			{
                std::string strPath;
                std::string strExt;
                Core::StringUtils::SplitBaseFilename(instrFilePath, strPath, strExt);
                
				for (u32 nProvider = 0; nProvider < mResourceProviderOlds.size(); nProvider++)
				{
                    if(mResourceProviderOlds[nProvider]->CanCreateResourceFromFileWithExtension(strExt))
                    {
                        CubemapSPtr pCubemap = CreateCubemapResource();
                        std::vector<Core::ResourceOldSPtr> aImages;
                        aImages.reserve(6);
                        
                        strExt = "." + strExt;
                        
                        for(u32 i=0; i<6; ++i)
                        {
                            Core::ResourceOldSPtr pSourceImage(new Core::Image());
                            std::string strFileName = strPath + Core::ToString(i+1) + strExt;
                            if(mResourceProviderOlds[nProvider]->CreateResourceFromFile(ineStorageLocation, strFileName, pSourceImage))
                            {
                                Core::Image* pImage = (Core::Image*)(pSourceImage.get());
                                pImage->SetName(strFileName);
                                pImage->SetLoaded(true);
                                aImages.push_back(pSourceImage);
                            }
                        }
                        
                        if(aImages.size() != 6)
                        {
                            CS_LOG_ERROR("Cannot find all resources for Cubemap with base path " + instrFilePath);
                            return CubemapSPtr();
                        }
                        
                        if(CreateCubemapFromImages(aImages, pCubemap))
                        {
                            CS_LOG_DEBUG("Loading Cubemap with base " + instrFilePath);
                            
                            mMapFilenameToResource.insert(std::make_pair(instrFilePath, std::static_pointer_cast<Core::ResourceOld>(pCubemap)));
                            pCubemap->SetName(instrFilePath);
                            pCubemap->SetOwningResourceManager(this);
                            pCubemap->SetLoaded(true);
                            pCubemap->SetFilename(instrFilePath);
                            pCubemap->SetStorageLocation(ineStorageLocation);
                            return pCubemap;
                        }
                    }
				}
			}
			else
			{
				return std::static_pointer_cast<Cubemap>(pExistingResource->second);
			}
			
			CS_LOG_ERROR("Cannot find resource for Cubemap with base path " + instrFilePath);
			return nullptr;
		}
		//-----------------------------------------------------------------
		/// Async Get Resource From File
		///
		/// Generic call to get the managers resource
        /// @param The storage location to load from
		/// @param File path to resource
		/// @return Generic pointer to object type
		//-----------------------------------------------------------------
		Core::ResourceOldSPtr CubemapManager::AsyncGetResourceFromFile(Core::StorageLocation ineStorageLocation, const std::string &instrFilePath)
		{
			MapStringToResourceSPtr::iterator pExistingResource = mMapFilenameToResource.find(instrFilePath);
			
			if(pExistingResource == mMapFilenameToResource.end())
			{
                std::string strPath;
                std::string strExt;
                Core::StringUtils::SplitBaseFilename(instrFilePath, strPath, strExt);
                strExt = "." + strExt;
                
				ImageDesc Desc;
                for(u32 i=0; i<6; ++i)
                {
                    Desc.strFilenames[i] = strPath + Core::ToString(i+1) + strExt;
                    Desc.pImageResources.push_back(Core::ResourceOldSPtr(new Core::Image()));
                }
                
				Desc.eStorageLocation = ineStorageLocation;
				Desc.pCubemapResource = CreateCubemapResource();
				Desc.pCubemapResource->SetFilename(instrFilePath);
				Desc.pCubemapResource->SetStorageLocation(ineStorageLocation);
                
				//Load image as a task. Once it completed we can load the Cubemap as it should be done on the main thread
				Core::Task<ImageDesc&> ImageLoadTask(this, &CubemapManager::ImageLoadTask, Desc);
				Core::TaskScheduler::ScheduleTask(ImageLoadTask);
				
				//add resource to the resource map
				mMapFilenameToResource.insert(std::make_pair(instrFilePath, std::static_pointer_cast<Core::ResourceOld>(Desc.pCubemapResource)));
				
				return Desc.pCubemapResource;
			}
			else
			{
				return std::static_pointer_cast<Cubemap>(pExistingResource->second);
			}
		}
		//-----------------------------------------------------------------------------------
		/// Image Load Task
		///
		/// Load the source image for the Cubemap
		///
		/// @param Image descriptor
		//-----------------------------------------------------------------------------------
		void CubemapManager::ImageLoadTask(ImageDesc& inDesc)
		{
            std::string strPath;
            std::string strExt;
            Core::StringUtils::SplitBaseFilename(inDesc.strFilenames[0], strPath, strExt);
            
			for (u32 nProvider = 0; nProvider < mResourceProviderOlds.size(); nProvider++)
			{
                if(mResourceProviderOlds[nProvider]->CanCreateResourceFromFileWithExtension(strExt))
                {
                    for(u32 i=0; i<6; ++i)
                    {
                        if(mResourceProviderOlds[nProvider]->CreateResourceFromFile(inDesc.eStorageLocation, inDesc.strFilenames[i], inDesc.pImageResources[i]))
                        {
                            CS_LOG_DEBUG("Loading image " + inDesc.strFilenames[i]);
                            
                            Core::Image* pImage = (Core::Image*)(inDesc.pImageResources[i].get());
                            pImage->SetName(inDesc.strFilenames[i]);
                            pImage->SetLoaded(true);
                        }
                    }
                    
                    //Load the Cubemap from this image
                    Core::TaskScheduler::ScheduleMainThreadTask(Core::Task<const std::vector<Core::ResourceOldSPtr>&, CubemapSPtr&>(this, &CubemapManager::CubemapLoadTask, inDesc.pImageResources, inDesc.pCubemapResource));
                    return;
                }
			}
			
			CS_LOG_ERROR("Cannot find resource for image with name " + inDesc.strFilenames[0]);
		}
		//-----------------------------------------------------------------------------------
		/// Cubemap Load Task
		///
		/// Load the Cubemap based on the source image
		///
		/// @param Source images
		/// @param With mipmapping
		/// @param Cubemap to create
		//-----------------------------------------------------------------------------------
		void CubemapManager::CubemapLoadTask(const std::vector<Core::ResourceOldSPtr>& inaImages, CubemapSPtr& outpCubemap)
		{
            if(CreateCubemapFromImages(inaImages, outpCubemap))
            {
                outpCubemap->SetName(inaImages[0]->GetName());
                outpCubemap->SetOwningResourceManager(this);
            }
            else
            {
                CS_LOG_ERROR("Cannot create Cubemap from image " + inaImages[0]->GetName());
                outpCubemap = nullptr;
                return;
            }

			outpCubemap->SetLoaded(true);
		}
	}
}

