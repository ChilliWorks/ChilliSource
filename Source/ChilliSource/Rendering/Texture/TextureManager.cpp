/*
 *  ITextureManager.cpp
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
		DEFINE_NAMED_INTERFACE(ITextureManager);
		
		//----------------------------------------------------------------
		/// Constructor
		//----------------------------------------------------------------
		ITextureManager::ITextureManager()
		{
			
		}
		//----------------------------------------------------------------
		/// Is A
		///
		/// Query the interface type
		/// @param The interface to compare
		/// @return Whether the object implements that interface
		//----------------------------------------------------------------
		bool ITextureManager::IsA(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == ITextureManager::InterfaceID;
		}
		//----------------------------------------------------------------
		/// Get Resource Type
		///
		/// @return The type of resource this manager handles
		//----------------------------------------------------------------
		Core::InterfaceIDType ITextureManager::GetResourceType() const
		{
			return ITexture::InterfaceID;
		}
		//----------------------------------------------------------------
		/// Manages Resource Of Type
		///
		/// @return Whether this object manages the object of type
		//----------------------------------------------------------------
		bool ITextureManager::ManagesResourceOfType(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == ITexture::InterfaceID;
		}
		//----------------------------------------------------------------
		/// Get Provider Type
		///
		/// @return The type of resource it consumes from resource provider
		//----------------------------------------------------------------
		Core::InterfaceIDType ITextureManager::GetProviderType() const
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
		Core::ResourcePtr ITextureManager::GetResourceFromFile(Core::StorageLocation ineStorageLocation, const std::string &instrFilePath)
		{
			return GetTextureFromFile(ineStorageLocation, instrFilePath, Core::CImage::Format::k_default, false);
		}
		//-----------------------------------------------------------------
		/// Async Get Resource From File
		///
		/// Generic call to get the managers resource
        /// @param The storage location to load from
		/// @param File path to resource
		/// @return Generic pointer to object type
		//-----------------------------------------------------------------
		Core::ResourcePtr ITextureManager::AsyncGetResourceFromFile(Core::StorageLocation ineStorageLocation, const std::string &instrFilePath)
		{
			return AsyncGetTextureFromFile(ineStorageLocation, instrFilePath, Core::CImage::Format::k_default, false);
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
		TexturePtr ITextureManager::GetTextureFromFile(Core::StorageLocation ineStorageLocation, const std::string & inFilePath, Core::CImage::Format ineFormat, bool inbWithMipsMaps)
		{
			MapStringToResourcePtr::iterator pExistingResource = mMapFilenameToResource.find(inFilePath);
			
			if(pExistingResource == mMapFilenameToResource.end()) 
			{
				Core::ResourcePtr pSourceImage(new Core::CImage());
				
				for (u32 nProvider = 0; nProvider < mResourceProviders.size(); nProvider++) 
				{
					if(static_cast<Core::IImageResourceProvider*>(mResourceProviders[nProvider])->CreateImageFromFile(ineStorageLocation, inFilePath, ineFormat, pSourceImage))
					{
						Core::CImage* pImage = (Core::CImage*)(pSourceImage.get());
						pImage->SetName(inFilePath);
						pImage->SetLoaded(true);
						
						TexturePtr pTexture = CreateTextureResource();
						if(CreateTextureFromImage(pImage, inbWithMipsMaps, pTexture))
						{
							CS_DEBUG_LOG("Loading texture " + inFilePath);
							
							mMapFilenameToResource.insert(std::make_pair(inFilePath, std::static_pointer_cast<Core::IResource>(pTexture)));
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
				return std::static_pointer_cast<ITexture>(pExistingResource->second);
			}
			
			CS_ERROR_LOG("Cannot find resource for texture with path " + inFilePath);
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
		TexturePtr ITextureManager::AsyncGetTextureFromFile(Core::StorageLocation ineStorageLocation, const std::string & inFilePath, Core::CImage::Format ineFormat, bool inbWithMipsMaps)
		{
			MapStringToResourcePtr::iterator pExistingResource = mMapFilenameToResource.find(inFilePath);
			
			if(pExistingResource == mMapFilenameToResource.end()) 
			{
				ImageDesc Desc;
				Desc.strFilename = inFilePath;
				Desc.eImageFormat = ineFormat;
				Desc.bUseMipmaps = inbWithMipsMaps;
				Desc.eStorageLocation = ineStorageLocation;
				Desc.pImageResource = Core::ResourcePtr(new Core::CImage());
				Desc.pTextureResource = CreateTextureResource();
				
				Desc.pTextureResource->SetFilename(inFilePath);
				Desc.pTextureResource->SetStorageLocation(ineStorageLocation);

				//Load image as a task. Once it completed we can load the texture as it should be done on the main thread
				Core::Task<ImageDesc&> ImageLoadTask(this, &ITextureManager::ImageLoadTask, Desc);
				Core::CTaskScheduler::ScheduleTask(ImageLoadTask);
				
				//add resource to the resource map
				mMapFilenameToResource.insert(std::make_pair(inFilePath, std::static_pointer_cast<Core::IResource>(Desc.pTextureResource)));
				
				return Desc.pTextureResource;
			} 
			else 
			{
				return std::static_pointer_cast<ITexture>(pExistingResource->second);
			}
		}
		//-----------------------------------------------------------------------------------
		/// Image Load Task
		///
		/// Load the source image for the texture
		///
		/// @param Image descriptor
		//-----------------------------------------------------------------------------------
		void ITextureManager::ImageLoadTask(ImageDesc& inDesc)
		{
			for (u32 nProvider = 0; nProvider < mResourceProviders.size(); nProvider++) 
			{
				if(static_cast<Core::IImageResourceProvider*>(mResourceProviders[nProvider])->CreateImageFromFile(inDesc.eStorageLocation, inDesc.strFilename, inDesc.eImageFormat, inDesc.pImageResource))
				{
					CS_DEBUG_LOG("Loading image " + inDesc.strFilename);
					
					Core::CImage* pImage = (Core::CImage*)(inDesc.pImageResource.get());
					pImage->SetName(inDesc.strFilename);
					pImage->SetLoaded(true);
					
					//Load the texture from this image
					Core::CTaskScheduler::ScheduleMainThreadTask(Core::Task<const Core::ResourcePtr&, bool, TexturePtr&>(this, &ITextureManager::TextureLoadTask, inDesc.pImageResource, inDesc.bUseMipmaps, inDesc.pTextureResource));
					return;
				}
			}
			
			CS_ERROR_LOG("Cannot find resource for image with name " + inDesc.strFilename);
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
		void ITextureManager::TextureLoadTask(const Core::ResourcePtr& inpImage, bool inbWithMipsMaps, TexturePtr& outpTexture)
		{
			Core::CImage* pImage = (Core::CImage*)(inpImage.get());
			if(CreateTextureFromImage(pImage, inbWithMipsMaps, outpTexture))
			{
				outpTexture->SetName(pImage->GetName());
				outpTexture->SetOwningResourceManager(this);
			}
			else
			{
				CS_ERROR_LOG("Cannot create texture from image " + pImage->GetName());
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
		const TexturePtr & ITextureManager::GetDefaultTexture()
        {
			if(!mpDefaultTexture)
            {
				mpDefaultTexture = CreateTextureResource();
				
				CreateDefaultTexture();
				
				mpDefaultTexture->SetLoaded(true);
				mpDefaultTexture->SetName("MoFlowDefaultTexture");
			}
			
			return mpDefaultTexture;
		}
		//-----------------------------------------------------------------
		/// Create Default Texture
		///
		/// Creates a 2x2 white texture
		/// @return Pointer to default texture
		//-----------------------------------------------------------------
		void ITextureManager::CreateDefaultTexture()
		{
			u32 numPixels = 4;
			u32 numBytesPerPixel = 4;

			u8 * pData = (u8*)malloc(numPixels * numBytesPerPixel);

			memset(pData, 255, (numPixels * numBytesPerPixel));

			Core::CImage * pImage = new Core::CImage();
			pImage->SetData(pData);
			pImage->SetWidth(2);
			pImage->SetHeight(2);
			pImage->SetFormat(Core::CImage::Format::k_default);

			CreateTextureFromImage(pImage, false, mpDefaultTexture);

			CS_SAFE_DELETE(pImage);
		}
	}
}

