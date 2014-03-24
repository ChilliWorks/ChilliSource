/*
 *  GLTextureManager.cpp
 *  moFlo
 *
 *  Created by Tag Games on 01/10/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Backend/Rendering/OpenGL/Texture/TextureManager.h>

#include <ChilliSource/Backend/Rendering/OpenGL/Texture/Texture.h>
#include <ChilliSource/Core/Image/ImageProvider.h>

namespace ChilliSource
{
	namespace OpenGL
	{
		//----------------------------------------------------------------
		/// Create Texture Resource
		///
		/// @return Concrete texture resource based on the render system
		//----------------------------------------------------------------
		ChilliSource::Rendering::TextureSPtr TextureManager::CreateTextureResource()
		{
			Rendering::TextureSPtr pTexture(new Texture(this));
			AddRestorableTexture(pTexture);
			return pTexture;
		}
        //----------------------------------------------------------------
        /// Create Image From Texture
        ///
        /// Rendersystem specific implementations should override this
        /// to return a shared pointer to their Image object
        ///
        /// @param mopFlow Texture to create Image from
        //----------------------------------------------------------------
        bool TextureManager::CreateImageFromTexture(Rendering::Texture* inpTexture, Core::ImageSPtr& outpImage)
        {
            return ((Texture*)inpTexture)->CreateImage(outpImage);
        }
		//----------------------------------------------------------------
		/// Create Texture From Image
		///
		/// @param Image
		/// @param Generate mip maps
		/// @param Out: Texture resource
		/// @return Success
		//----------------------------------------------------------------
		bool TextureManager::CreateTextureFromImage(Core::Image * inpImage, ChilliSource::Rendering::TextureSPtr& outpTexture)
		{
			std::static_pointer_cast<Texture>(outpTexture)->Init(inpImage);
			return true;
		}
		//----------------------------------------------------------------
		/// Create Empty Texture
		///
		/// @param Texture width
		/// @param Texture height
		/// @return Texture resource
		//----------------------------------------------------------------
		bool TextureManager::CreateEmptyTexture(u32 inudwWidth, u32 inudwHeight, Core::Image::Format ineFormat, ChilliSource::Rendering::TextureSPtr& outpTexture)
		{
			std::static_pointer_cast<Texture>(outpTexture)->Init(inudwWidth, inudwHeight, ineFormat);
			outpTexture->SetLoaded(true);
			return true;
		}
		//----------------------------------------------------------------
		/// Backup
		///
		/// Backs up all textures that cannot be restored from file in
		/// memory.
		//----------------------------------------------------------------
		void TextureManager::Backup()
		{
#ifdef CS_TARGETPLATFORM_ANDROID
			for(std::vector<Rendering::TextureWPtr>::iterator it = mapTextureCache.begin(); it != mapTextureCache.end(); ++it)
			{
				if (Rendering::TextureSPtr pTexture = (*it).lock())
				{
					if(pTexture->IsLoaded())
					{
						//if the image was not loaded from file, then we'll need to cache it
						if(pTexture->GetFilename() == "" && pTexture->GetStorageLocation() == Core::StorageLocation::k_none)
						{
//							Core::ImageSPtr pImage;
//							CreateImageFromTexture(pTexture.get(), pImage);
//							std::shared_ptr<Texture> pOpenGLTexture = std::static_pointer_cast<Texture>(pTexture);
//							pOpenGLTexture->Reset();
//							mapBackedUpImages.insert(std::pair<Texture*, Core::ImageSPtr>(pOpenGLTexture.get(), pImage));

							std::shared_ptr<Texture> pOpenGLTexture = std::static_pointer_cast<Texture>(pTexture);
							pOpenGLTexture->Reset();
							mapBackedUpImages.insert(std::pair<Texture*, Core::ImageSPtr>(pOpenGLTexture.get(), Core::ImageSPtr()));
						}
					}
				}
			}
#endif
		}
		//----------------------------------------------------------------
		/// Restore
		///
		/// Restore all textures either from file or from cached images.
		//----------------------------------------------------------------
		void TextureManager::Restore()
		{
#ifdef CS_TARGETPLATFORM_ANDROID

			for(std::vector<Rendering::TextureWPtr>::iterator it = mapTextureCache.begin(); it != mapTextureCache.end(); ++it)
			{
				if (Rendering::TextureSPtr pTexture = (*it).lock())
				{
					if(pTexture->IsLoaded())
					{
						std::shared_ptr<Texture> pOpenGLTexture = std::static_pointer_cast<Texture>(pTexture);
						Core::ImageSPtr pImage = Core::ImageSPtr(new Core::Image());
						Core::ResourceSPtr pImageResource = std::static_pointer_cast<Core::Resource>(pImage);

						//If the texture was loaded from file then reload it.
						if(pOpenGLTexture->GetFilename() != "" && pOpenGLTexture->GetStorageLocation() != Core::StorageLocation::k_none)
						{
							for (u32 nProvider = 0; nProvider < mResourceProviders.size(); nProvider++)
							{
								if(static_cast<Core::ImageResourceProvider*>(mResourceProviders[nProvider])->CreateImageFromFile(pOpenGLTexture->GetStorageLocation(), pOpenGLTexture->GetFilename(), pOpenGLTexture->GetImageFormat(), pImageResource))
								{
									pImage->SetName(pOpenGLTexture->GetFilename());
									pImage->SetLoaded(true);
									CreateTextureFromImage(pImage.get(), pOpenGLTexture->HasMipMaps(), pTexture);
								}
							}
						}
						else
						{
							//if the image used for this texture was cached, then recreate from it
							std::unordered_map<Texture*, Core::ImageSPtr>::iterator it = mapBackedUpImages.find(pOpenGLTexture.get());
							if (it != mapBackedUpImages.end() && it->second != nullptr)
							{
								CreateTextureFromImage(it->second.get(), pOpenGLTexture->HasMipMaps(), pTexture);
							}
							else
							{
								pOpenGLTexture->Init(pOpenGLTexture->GetWidth(), pOpenGLTexture->GetHeight(), pOpenGLTexture->GetImageFormat());
							}
						}
					}
				}
			}
            
            //rebuild the default texture
			CreateDefaultTexture();

			mapBackedUpImages.clear();
#endif
		}
		//----------------------------------------------------------------
		/// Add Restorable Texture
		///
		/// Adds the texture to the list of all created textures. This
		/// list is used to restore the textures when the context is lost.
		///
		/// @param The texture pointer.
		//----------------------------------------------------------------
		void TextureManager::AddRestorableTexture(const Rendering::TextureSPtr& inpTexture)
		{
#ifdef CS_TARGETPLATFORM_ANDROID
			mapTextureCache.push_back(Rendering::TextureWPtr(inpTexture));
#endif
		}
		//----------------------------------------------------------------
		/// Remove Restorable Texture
		///
		/// Removes the texture from the list of all created textures. This
		/// list is used to restore the textures when the context is lost.
		///
		/// @param The texture pointer.
		//----------------------------------------------------------------
		void TextureManager::RemoveRestorableTexture(Texture* inpTexture)
		{
#ifdef CS_TARGETPLATFORM_ANDROID
			for(std::vector<Rendering::TextureWPtr>::iterator it = mapTextureCache.begin(); it != mapTextureCache.end(); ++it)
			{
				if (Rendering::TextureSPtr pTexture = (*it).lock())
				{
					if (pTexture.get() == inpTexture)
					{
						mapTextureCache.erase(it);
						break;
					}
				}
			}
#endif
		}
		//----------------------------------------------------------------
		/// Destructor
		//----------------------------------------------------------------
		TextureManager::~TextureManager()
		{
			mpDefaultTexture.reset();
		}
	}
}

