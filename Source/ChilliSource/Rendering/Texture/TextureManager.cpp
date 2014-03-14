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

#include <ChilliSource/Core/Image/ImageResourceProvider.h>
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
			return GetTextureFromFile(ineStorageLocation, instrFilePath, Core::Image::Format::k_default, false);
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
			return AsyncGetTextureFromFile(ineStorageLocation, instrFilePath, Core::Image::Format::k_default, false);
		}
		//----------------------------------------------------------------
		/// Get Texture From File
		///
		/// Loads the texture from resource and returns a handle to it.
		/// Alternately if the texture already exists it will return the
		/// handle without loading
		///
        /// @param The storage location to load from
		/// @param File name
		/// @param Generate mip-maps. Default = false
		/// @return A handle to the texture
		//----------------------------------------------------------------
		TextureSPtr TextureManager::GetTextureFromFile(Core::StorageLocation ineStorageLocation, const std::string & inFilePath, Core::Image::Format ineFormat, bool inbWithMipsMaps)
		{
			MapStringToResourceSPtr::iterator pExistingResource = mMapFilenameToResource.find(inFilePath);
			
			if(pExistingResource == mMapFilenameToResource.end()) 
			{
				Core::ResourceSPtr pSourceImage(new Core::Image());
				
				for (u32 nProvider = 0; nProvider < mResourceProviders.size(); nProvider++) 
				{
					if(static_cast<Core::ImageResourceProvider*>(mResourceProviders[nProvider])->CreateImageFromFile(ineStorageLocation, inFilePath, ineFormat, pSourceImage))
					{
						Core::Image* pImage = (Core::Image*)(pSourceImage.get());
						pImage->SetName(inFilePath);
						pImage->SetLoaded(true);
						
						TextureSPtr pTexture = CreateTextureResource();
						if(CreateTextureFromImage(pImage, inbWithMipsMaps, pTexture))
						{
							CS_LOG_DEBUG("Loading texture " + inFilePath);
							
							mMapFilenameToResource.insert(std::make_pair(inFilePath, std::static_pointer_cast<Core::Resource>(pTexture)));
							pTexture->SetName(inFilePath);
							pTexture->SetOwningResourceManager(this);
							pTexture->SetLoaded(true);
							pTexture->SetFilename(inFilePath);
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
			
			CS_LOG_ERROR("Cannot find resource for texture with path " + inFilePath);
			return GetDefaultTexture();
		}
		//-----------------------------------------------------------------
		/// Async Get Texture From File
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
		TextureSPtr TextureManager::AsyncGetTextureFromFile(Core::StorageLocation ineStorageLocation, const std::string & inFilePath, Core::Image::Format ineFormat, bool inbWithMipsMaps)
		{
			MapStringToResourceSPtr::iterator pExistingResource = mMapFilenameToResource.find(inFilePath);
			
			if(pExistingResource == mMapFilenameToResource.end()) 
			{
				ImageDesc Desc;
				Desc.strFilename = inFilePath;
				Desc.eImageFormat = ineFormat;
				Desc.bUseMipmaps = inbWithMipsMaps;
				Desc.eStorageLocation = ineStorageLocation;
				Desc.pImageResource = Core::ResourceSPtr(new Core::Image());
				Desc.pTextureResource = CreateTextureResource();
				
				Desc.pTextureResource->SetFilename(inFilePath);
				Desc.pTextureResource->SetStorageLocation(ineStorageLocation);

				//Load image as a task. Once it completed we can load the texture as it should be done on the main thread
				Core::Task<ImageDesc&> ImageLoadTask(this, &TextureManager::ImageLoadTask, Desc);
				Core::TaskScheduler::ScheduleTask(ImageLoadTask);
				
				//add resource to the resource map
				mMapFilenameToResource.insert(std::make_pair(inFilePath, std::static_pointer_cast<Core::Resource>(Desc.pTextureResource)));
				
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
				if(static_cast<Core::ImageResourceProvider*>(mResourceProviders[nProvider])->CreateImageFromFile(inDesc.eStorageLocation, inDesc.strFilename, inDesc.eImageFormat, inDesc.pImageResource))
				{
					CS_LOG_DEBUG("Loading image " + inDesc.strFilename);
					
					Core::Image* pImage = (Core::Image*)(inDesc.pImageResource.get());
					pImage->SetName(inDesc.strFilename);
					pImage->SetLoaded(true);
					
					//Load the texture from this image
					Core::TaskScheduler::ScheduleMainThreadTask(Core::Task<const Core::ResourceSPtr&, bool, TextureSPtr&>(this, &TextureManager::TextureLoadTask, inDesc.pImageResource, inDesc.bUseMipmaps, inDesc.pTextureResource));
					return;
				}
			}
			
			CS_LOG_ERROR("Cannot find resource for image with name " + inDesc.strFilename);
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
		void TextureManager::TextureLoadTask(const Core::ResourceSPtr& inpImage, bool inbWithMipsMaps, TextureSPtr& outpTexture)
		{
			Core::Image* pImage = (Core::Image*)(inpImage.get());
			if(CreateTextureFromImage(pImage, inbWithMipsMaps, outpTexture))
			{
				outpTexture->SetName(pImage->GetName());
				outpTexture->SetOwningResourceManager(this);
			}
			else
			{
				CS_LOG_ERROR("Cannot create texture from image " + pImage->GetName());
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

			CreateTextureFromImage(pImage, false, mpDefaultTexture);

			CS_SAFEDELETE(pImage);
            
            mpDefaultTexture->SetLoaded(true);
		}
	}
}

