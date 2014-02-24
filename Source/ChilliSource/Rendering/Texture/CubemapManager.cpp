/*
 *  ICubemapManager.cpp
 *  moFlow
 *
 *  Created by Tag Games on 01/10/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Rendering/Texture/CubemapManager.h>
#include <ChilliSource/Rendering/Texture/Cubemap.h>

#include <ChilliSource/Core/Image/ImageResourceProvider.h>
#include <ChilliSource/Core/Image/Image.h>
#include <ChilliSource/Core/Base/Colour.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>

namespace ChilliSource
{
	namespace Rendering
	{
		DEFINE_NAMED_INTERFACE(ICubemapManager);
		
		//----------------------------------------------------------------
		/// Constructor
		//----------------------------------------------------------------
		ICubemapManager::ICubemapManager()
		{
			
		}
		//----------------------------------------------------------------
		/// Is A
		///
		/// Query the interface type
		/// @param The interface to compare
		/// @return Whether the object implements that interface
		//----------------------------------------------------------------
		bool ICubemapManager::IsA(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == ICubemapManager::InterfaceID;
		}
		//----------------------------------------------------------------
		/// Get Resource Type
		///
		/// @return The type of resource this manager handles
		//----------------------------------------------------------------
		Core::InterfaceIDType ICubemapManager::GetResourceType() const
		{
			return ICubemap::InterfaceID;
		}
		//----------------------------------------------------------------
		/// Manages Resource Of Type
		///
		/// @return Whether this object manages the object of type
		//----------------------------------------------------------------
		bool ICubemapManager::ManagesResourceOfType(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == ICubemap::InterfaceID;
		}
		//----------------------------------------------------------------
		/// Get Provider Type
		///
		/// @return The type of resource it consumes from resource provider
		//----------------------------------------------------------------
		Core::InterfaceIDType ICubemapManager::GetProviderType() const
		{
			return Core::CImage::InterfaceID;
		}
		//-----------------------------------------------------------------
		/// Get Resource From File
		///
		/// Generic call to get the managers resource
        /// @param The storage location to load from
		/// @param File path to resource
		/// @return Generic pointer to object type
		//-----------------------------------------------------------------
		Core::ResourcePtr ICubemapManager::GetResourceFromFile(Core::StorageLocation ineStorageLocation, const std::string &instrFilePath)
		{
			return GetCubemapFromFile(ineStorageLocation, instrFilePath, Core::CImage::Format::k_default, false);
		}
		//-----------------------------------------------------------------
		/// Async Get Resource From File
		///
		/// Generic call to get the managers resource
        /// @param The storage location to load from
		/// @param File path to resource
		/// @return Generic pointer to object type
		//-----------------------------------------------------------------
		Core::ResourcePtr ICubemapManager::AsyncGetResourceFromFile(Core::StorageLocation ineStorageLocation, const std::string &instrFilePath)
		{
			return AsyncGetCubemapFromFile(ineStorageLocation, instrFilePath, Core::CImage::Format::k_default, false);
		}
		//----------------------------------------------------------------
		/// Get Cubemap From File
		///
		/// Loads the Cubemap from resource and returns a handle to it.
		/// Alternately if the Cubemap already exists it will return the
		/// handle without loading
		///
        /// @param The storage location to load from
		/// @param Base File name
		/// @param Generate mip-maps. Default = false
		/// @return A handle to the Cubemap
		//----------------------------------------------------------------
		CubemapPtr ICubemapManager::GetCubemapFromFile(Core::StorageLocation ineStorageLocation, const std::string & inFilePath, Core::CImage::Format ineFormat, bool inbWithMipsMaps)
		{
			MapStringToResourcePtr::iterator pExistingResource = mMapFilenameToResource.find(inFilePath);
			
			if(pExistingResource == mMapFilenameToResource.end()) 
			{
                std::string strPath;
                std::string strExt;
                Core::CStringUtils::SplitBaseFilename(inFilePath, strPath, strExt);
                
				for (u32 nProvider = 0; nProvider < mResourceProviders.size(); nProvider++)
				{
                    if(mResourceProviders[nProvider]->CanCreateResourceFromFileWithExtension(strExt))
                    {
                        CubemapPtr pCubemap = CreateCubemapResource();
                        DYNAMIC_ARRAY<Core::ResourcePtr> aImages;
                        aImages.reserve(6);
                        
                        strExt = "." + strExt;
                        
                        for(u32 i=0; i<6; ++i)
                        {
                            Core::ResourcePtr pSourceImage(new Core::CImage());
                            std::string strFileName = strPath + STRING_CAST(i+1) + strExt;
                            if(static_cast<IImageResourceProvider*>(mResourceProviders[nProvider])->CreateImageFromFile(ineStorageLocation, strFileName, ineFormat, pSourceImage))
                            {
                                Core::CImage* pImage = (Core::CImage*)(pSourceImage.get());
                                pImage->SetName(strFileName);
                                pImage->SetLoaded(true);
                                aImages.push_back(pSourceImage);
                            }
                        }
                        
                        if(aImages.size() != 6)
                        {
                            ERROR_LOG("Cannot find all resources for Cubemap with base path " + inFilePath);
                            return CubemapPtr();
                        }
                        
                        if(CreateCubemapFromImages(aImages, inbWithMipsMaps, pCubemap))
                        {
                            DEBUG_LOG("Loading Cubemap with base " + inFilePath);
                            
                            mMapFilenameToResource.insert(std::make_pair(inFilePath, SHARED_PTR_CAST<Core::IResource>(pCubemap)));
                            pCubemap->SetName(inFilePath);
                            pCubemap->SetOwningResourceManager(this);
                            pCubemap->SetLoaded(true);
                            pCubemap->SetFilename(inFilePath);
                            pCubemap->SetStorageLocation(ineStorageLocation);
                            return pCubemap;
                        }
                    }
				}
			}
			else
			{
				return SHARED_PTR_CAST<ICubemap>(pExistingResource->second);
			}
			
			ERROR_LOG("Cannot find resource for Cubemap with base path " + inFilePath);
			return CubemapPtr();
		}
		//-----------------------------------------------------------------
		/// Async Get Cubemap From File
		///
		/// Generic call to get the managers resource. This will
		/// load the resource on a seperate thread but will return the
		/// resource pointer synchronously. Before using the resource
		/// pointer the IsLoaded flag should be checked
		///
        /// @param The storage location to load from
		/// @param File path to resource
		/// @param Image format
		/// @param Enable mip-mapping
		/// @return Generic pointer to resource type
		//-----------------------------------------------------------------
		CubemapPtr ICubemapManager::AsyncGetCubemapFromFile(Core::StorageLocation ineStorageLocation, const std::string & inFilePath, Core::CImage::Format ineFormat, bool inbWithMipsMaps)
		{
			MapStringToResourcePtr::iterator pExistingResource = mMapFilenameToResource.find(inFilePath);
			
			if(pExistingResource == mMapFilenameToResource.end()) 
			{
                std::string strPath;
                std::string strExt;
                Core::CStringUtils::SplitBaseFilename(inFilePath, strPath, strExt);
                strExt = "." + strExt;
                
				ImageDesc Desc;
                for(u32 i=0; i<6; ++i)
                {
                    Desc.strFilenames[i] = strPath + STRING_CAST(i+1) + strExt;
                    Desc.pImageResources.push_back(Core::ResourcePtr(new Core::CImage()));
                }
	
				Desc.eImageFormat = ineFormat;
				Desc.bUseMipmaps = inbWithMipsMaps;
				Desc.eStorageLocation = ineStorageLocation;
				Desc.pCubemapResource = CreateCubemapResource();
				Desc.pCubemapResource->SetFilename(inFilePath);
				Desc.pCubemapResource->SetStorageLocation(ineStorageLocation);

				//Load image as a task. Once it completed we can load the Cubemap as it should be done on the main thread
				Task1<ImageDesc&> ImageLoadTask(this, &ICubemapManager::ImageLoadTask, Desc);
				CTaskScheduler::ScheduleTask(ImageLoadTask);
				
				//add resource to the resource map
				mMapFilenameToResource.insert(std::make_pair(inFilePath, SHARED_PTR_CAST<Core::IResource>(Desc.pCubemapResource)));
				
				return Desc.pCubemapResource;
			} 
			else 
			{
				return SHARED_PTR_CAST<ICubemap>(pExistingResource->second);
			}
		}
		//-----------------------------------------------------------------------------------
		/// Image Load Task
		///
		/// Load the source image for the Cubemap
		///
		/// @param Image descriptor
		//-----------------------------------------------------------------------------------
		void ICubemapManager::ImageLoadTask(ImageDesc& inDesc)
		{
            std::string strPath;
            std::string strExt;
            Core::CStringUtils::SplitBaseFilename(inDesc.strFilenames[0], strPath, strExt);
            
			for (u32 nProvider = 0; nProvider < mResourceProviders.size(); nProvider++)
			{
                if(mResourceProviders[nProvider]->CanCreateResourceFromFileWithExtension(strExt))
                {
                    for(u32 i=0; i<6; ++i)
                    {
                        if(static_cast<IImageResourceProvider*>(mResourceProviders[nProvider])->CreateImageFromFile(inDesc.eStorageLocation, inDesc.strFilenames[i], inDesc.eImageFormat, inDesc.pImageResources[i]))
                        {
                            DEBUG_LOG("Loading image " + inDesc.strFilenames[i]);
                            
                            Core::CImage* pImage = (Core::CImage*)(inDesc.pImageResources[i].get());
                            pImage->SetName(inDesc.strFilenames[i]);
                            pImage->SetLoaded(true);
                        }
                    }
                    
                    //Load the Cubemap from this image
                    CTaskScheduler::ScheduleMainThreadTask(Task3<const DYNAMIC_ARRAY<Core::ResourcePtr>&, bool, CubemapPtr&>(this, &ICubemapManager::CubemapLoadTask, inDesc.pImageResources, inDesc.bUseMipmaps, inDesc.pCubemapResource));
                    return;
                }
			}
			
			ERROR_LOG("Cannot find resource for image with name " + inDesc.strFilenames[0]);
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
		void ICubemapManager::CubemapLoadTask(const DYNAMIC_ARRAY<Core::ResourcePtr>& inaImages, bool inbWithMipsMaps, CubemapPtr& outpCubemap)
		{
            if(CreateCubemapFromImages(inaImages, inbWithMipsMaps, outpCubemap))
            {
                outpCubemap->SetName(inaImages[0]->GetName());
                outpCubemap->SetOwningResourceManager(this);
            }
            else
            {
                ERROR_LOG("Cannot create Cubemap from image " + inaImages[0]->GetName());
                outpCubemap = CubemapPtr();
                return;
            }

			outpCubemap->SetLoaded(true);
		}
	}
}

