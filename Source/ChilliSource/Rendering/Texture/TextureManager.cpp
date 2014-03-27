/*
 *  TextureManager.cpp
 *  MoFlow
 *
 *  Created by Tag Games on 01/10/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Rendering/Texture/TextureManager.h>
#include <ChilliSource/Rendering/Texture/Texture.h>

#include <ChilliSource/Core/Image/ImageProvider.h>
#include <ChilliSource/Core/Image/Image.h>
#include <ChilliSource/Core/Base/Colour.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>

namespace ChilliSource
{
	namespace Rendering
	{
		CS_DEFINE_NAMEDTYPE(TextureManager);
		
		//----------------------------------------------------------------
		/// Constructor
		//----------------------------------------------------------------
		TextureManager::TextureManager()
		{
		}
		//----------------------------------------------------------------
		/// Is A
		///
		/// Query the interface type
		/// @param The interface to compare
		/// @return Whether the object implements that interface
		//----------------------------------------------------------------
		bool TextureManager::IsA(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == TextureManager::InterfaceID;
		}
		//----------------------------------------------------------------
		/// Get Resource Type
		///
		/// @return The type of resource this manager handles
		//----------------------------------------------------------------
		Core::InterfaceIDType TextureManager::GetResourceType() const
		{
			return Texture::InterfaceID;
		}
		//----------------------------------------------------------------
		/// Manages Resource Of Type
		///
		/// @return Whether this object manages the object of type
		//----------------------------------------------------------------
		bool TextureManager::ManagesResourceOfType(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == Texture::InterfaceID;
		}
		//----------------------------------------------------------------
		/// Get Provider Type
		///
		/// @return The type of resource it consumes from resource provider
		//----------------------------------------------------------------
		Core::InterfaceIDType TextureManager::GetProviderType() const
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
		Core::ResourceSPtr TextureManager::GetResourceFromFile(Core::StorageLocation ineStorageLocation, const std::string &instrFilePath)
		{
			MapStringToResourceSPtr::iterator pExistingResource = mMapFilenameToResource.find(instrFilePath);
			
			if(pExistingResource == mMapFilenameToResource.end())
			{
				Core::ResourceSPtr pSourceImage(new Core::Image());
				
				for (u32 nProvider = 0; nProvider < mResourceProviders.size(); nProvider++)
				{
					if(mResourceProviders[nProvider]->CreateResourceFromFile(ineStorageLocation, instrFilePath, pSourceImage))
					{
						Core::Image* pImage = (Core::Image*)(pSourceImage.get());
						pImage->SetName(instrFilePath);
						pImage->SetLoaded(true);
						
						TextureSPtr pTexture = CreateTextureResource();
						if(CreateTextureFromImage(pImage, pTexture))
						{
							CS_LOG_DEBUG("Loading texture " + instrFilePath);
							
							mMapFilenameToResource.insert(std::make_pair(instrFilePath, std::static_pointer_cast<Core::Resource>(pTexture)));
							pTexture->SetName(instrFilePath);
							pTexture->SetOwningResourceManager(this);
							pTexture->SetLoaded(true);
							pTexture->SetFilename(instrFilePath);
							pTexture->SetStorageLocation(ineStorageLocation);
							return pTexture;
						}
					}
				}
			}
			else
			{
				return std::static_pointer_cast<Texture>(pExistingResource->second);
			}
			
			CS_LOG_FATAL("Cannot find resource for texture with path " + instrFilePath);
			return GetDefaultTexture();
		}
		//-----------------------------------------------------------------
		/// Async Get Resource From File
		///
		/// Generic call to get the managers resource
        /// @param The storage location to load from
		/// @param File path to resource
		/// @return Generic pointer to object type
		//-----------------------------------------------------------------
		Core::ResourceSPtr TextureManager::AsyncGetResourceFromFile(Core::StorageLocation ineStorageLocation, const std::string &instrFilePath)
		{
			MapStringToResourceSPtr::iterator pExistingResource = mMapFilenameToResource.find(instrFilePath);
			
			if(pExistingResource == mMapFilenameToResource.end())
			{
				ImageDesc Desc;
				Desc.strFilename = instrFilePath;
				Desc.eStorageLocation = ineStorageLocation;
				Desc.pImageResource = Core::ResourceSPtr(new Core::Image());
				Desc.pTextureResource = CreateTextureResource();
				
				Desc.pTextureResource->SetFilename(instrFilePath);
				Desc.pTextureResource->SetStorageLocation(ineStorageLocation);
                
				//Load image as a task. Once it completed we can load the texture as it should be done on the main thread
				Core::Task<ImageDesc&> ImageLoadTask(this, &TextureManager::ImageLoadTask, Desc);
				Core::TaskScheduler::ScheduleTask(ImageLoadTask);
				
				//add resource to the resource map
				mMapFilenameToResource.insert(std::make_pair(instrFilePath, std::static_pointer_cast<Core::Resource>(Desc.pTextureResource)));
				
				return Desc.pTextureResource;
			}
			else
			{
				return std::static_pointer_cast<Texture>(pExistingResource->second);
			}
		}
		//-----------------------------------------------------------------------------------
		/// Image Load Task
		///
		/// Load the source image for the texture
		///
		/// @param Image descriptor
		//-----------------------------------------------------------------------------------
		void TextureManager::ImageLoadTask(ImageDesc& inDesc)
		{
			for (u32 nProvider = 0; nProvider < mResourceProviders.size(); nProvider++) 
			{
				if(mResourceProviders[nProvider]->CreateResourceFromFile(inDesc.eStorageLocation, inDesc.strFilename, inDesc.pImageResource))
				{
					CS_LOG_DEBUG("Loading image " + inDesc.strFilename);
					
					Core::Image* pImage = (Core::Image*)(inDesc.pImageResource.get());
					pImage->SetName(inDesc.strFilename);
					pImage->SetLoaded(true);
					
					//Load the texture from this image
					Core::TaskScheduler::ScheduleMainThreadTask(Core::Task<const Core::ResourceSPtr&, TextureSPtr&>(this, &TextureManager::TextureLoadTask, inDesc.pImageResource, inDesc.pTextureResource));
					return;
				}
			}
			
			CS_LOG_FATAL("Cannot find resource for image with name " + inDesc.strFilename);
		}
		//-----------------------------------------------------------------------------------
		/// Texture Load Task
		///
		/// Load the texture based on the source image
		///
		/// @param Source image
		/// @param With mipmapping
		/// @param Texture to create
		//-----------------------------------------------------------------------------------
		void TextureManager::TextureLoadTask(const Core::ResourceSPtr& inpImage, TextureSPtr& outpTexture)
		{
			Core::Image* pImage = (Core::Image*)(inpImage.get());
			if(CreateTextureFromImage(pImage, outpTexture))
			{
				outpTexture->SetName(pImage->GetName());
				outpTexture->SetOwningResourceManager(this);
			}
			else
			{
				CS_LOG_FATAL("Cannot create texture from image " + pImage->GetName());
				outpTexture = GetDefaultTexture();
			}

			outpTexture->SetLoaded(true);
		}
		//-----------------------------------------------------------------
		/// Get Default Texture
		///
		/// Returns a 2x2 white texture
		/// @return Pointer to default texture
		//-----------------------------------------------------------------
		const TextureSPtr & TextureManager::GetDefaultTexture()
        {
			if(!mpDefaultTexture)
            {
				CreateDefaultTexture();
			}
			
			return mpDefaultTexture;
		}
		//-----------------------------------------------------------------
		/// Create Default Texture
		///
		/// Creates a 2x2 white texture
		/// @return Pointer to default texture
		//-----------------------------------------------------------------
		void TextureManager::CreateDefaultTexture()
		{
            mpDefaultTexture = CreateTextureResource();
            mpDefaultTexture->SetName("CSDefaultTexture");
            
			u32 numPixels = 4;
			u32 numBytesPerPixel = 4;

			u8 * pData = (u8*)malloc(numPixels * numBytesPerPixel);

			memset(pData, 255, (numPixels * numBytesPerPixel));

			Core::Image * pImage = new Core::Image();
			pImage->SetData(pData);
			pImage->SetWidth(2);
			pImage->SetHeight(2);
			pImage->SetFormat(Core::Image::Format::k_default);

			CreateTextureFromImage(pImage, mpDefaultTexture);

			CS_SAFEDELETE(pImage);
            
            mpDefaultTexture->SetLoaded(true);
		}
	}
}

