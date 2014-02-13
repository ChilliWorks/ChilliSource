/*
 *  GLTextureManager.cpp
 *  moFlo
 *
 *  Created by Tag Games on 01/10/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/RenderingPlatform/OpenGL/TextureManager.h>
#include <ChilliSource/RenderingPlatform/OpenGL/Texture.h>
#include <ChilliSource/Core/ImageResourceProvider.h>

namespace moFlo
{
	namespace OpenGL
	{
		//----------------------------------------------------------------
		/// Create Texture Resource
		///
		/// @return Concrete texture resource based on the render system
		//----------------------------------------------------------------
		moFlo::Rendering::TexturePtr CTextureManager::CreateTextureResource()
		{
			Rendering::TexturePtr pTexture(new CTexture(this));
			AddRestorableTexture(pTexture);
			return pTexture;
		}
        //----------------------------------------------------------------
        /// Create Image From Texture
        ///
        /// Rendersystem specific implementations should override this
        /// to return a shared pointer to their CImage object
        ///
        /// @param mopFlow Texture to create Image from
        //----------------------------------------------------------------
        bool CTextureManager::CreateImageFromTexture(Rendering::ITexture* inpTexture, Core::ImagePtr& outpImage)
        {
            return ((CTexture*)inpTexture)->CreateImage(outpImage);
        }
		//----------------------------------------------------------------
		/// Create Texture From Image
		///
		/// @param Image
		/// @param Generate mip maps
		/// @param Out: Texture resource
		/// @return Success
		//----------------------------------------------------------------
		bool CTextureManager::CreateTextureFromImage(Core::CImage * inpImage, bool inbWithMipsMaps, moFlo::Rendering::TexturePtr& outpTexture)
		{
			SHARED_PTR_CAST<CTexture>(outpTexture)->Init(inpImage, inbWithMipsMaps);
			return true;
		}
		//----------------------------------------------------------------
		/// Create Empty Texture
		///
		/// @param Texture width
		/// @param Texture height
		/// @return Texture resource
		//----------------------------------------------------------------
		bool CTextureManager::CreateEmptyTexture(u32 inudwWidth, u32 inudwHeight, Core::CImage::Format ineFormat, moFlo::Rendering::TexturePtr& outpTexture)
		{
			SHARED_PTR_CAST<CTexture>(outpTexture)->Init(inudwWidth, inudwHeight, ineFormat);
			outpTexture->SetLoaded(true);
			return true;
		}
		//----------------------------------------------------------------
		/// Backup
		///
		/// Backs up all textures that cannot be restored from file in
		/// memory.
		//----------------------------------------------------------------
		void CTextureManager::Backup()
		{
#ifdef TARGET_ANDROID
			for(DYNAMIC_ARRAY<Rendering::TextureWeakPtr>::iterator it = mapTextureCache.begin(); it != mapTextureCache.end(); ++it)
			{
				if (Rendering::TexturePtr pTexture = (*it).lock())
				{
					if(pTexture->IsLoaded())
					{
						//if the image was not loaded from file, then we'll need to cache it
						if(pTexture->GetFilename() == "" && pTexture->GetStorageLocation() == Core::SL_NONE)
						{
//							Core::ImagePtr pImage;
//							CreateImageFromTexture(pTexture.get(), pImage);
//							SHARED_PTR<CTexture> pOpenGLTexture = SHARED_PTR_CAST<CTexture>(pTexture);
//							pOpenGLTexture->Reset();
//							mapBackedUpImages.insert(std::pair<CTexture*, Core::ImagePtr>(pOpenGLTexture.get(), pImage));

							SHARED_PTR<CTexture> pOpenGLTexture = SHARED_PTR_CAST<CTexture>(pTexture);
							pOpenGLTexture->Reset();
							mapBackedUpImages.insert(std::pair<CTexture*, Core::ImagePtr>(pOpenGLTexture.get(), Core::ImagePtr()));
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
		void CTextureManager::Restore()
		{
#ifdef TARGET_ANDROID

			//rebuild the default texture
			CreateDefaultTexture();

			for(DYNAMIC_ARRAY<Rendering::TextureWeakPtr>::iterator it = mapTextureCache.begin(); it != mapTextureCache.end(); ++it)
			{
				if (Rendering::TexturePtr pTexture = (*it).lock())
				{
					if(pTexture->IsLoaded())
					{
						SHARED_PTR<CTexture> pOpenGLTexture = SHARED_PTR_CAST<CTexture>(pTexture);
						Core::ImagePtr pImage = Core::ImagePtr(new Core::CImage());
						Core::ResourcePtr pImageResource = SHARED_PTR_CAST<Core::IResource>(pImage);

						//If the texture was loaded from file then reload it.
						if(pOpenGLTexture->GetFilename() != "" && pOpenGLTexture->GetStorageLocation() != Core::SL_NONE)
						{
							for (u32 nProvider = 0; nProvider < mResourceProviders.size(); nProvider++)
							{
								if(static_cast<IImageResourceProvider*>(mResourceProviders[nProvider])->CreateImageFromFile(pOpenGLTexture->GetStorageLocation(), pOpenGLTexture->GetFilename(), pOpenGLTexture->GetImageFormat(), pImageResource))
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
							HASH_MAP<CTexture*, Core::ImagePtr>::iterator it = mapBackedUpImages.find(pOpenGLTexture.get());
							if (it != mapBackedUpImages.end() && it->second != NULL)
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
		void CTextureManager::AddRestorableTexture(const Rendering::TexturePtr& inpTexture)
		{
#ifdef TARGET_ANDROID
			mapTextureCache.push_back(Rendering::TextureWeakPtr(inpTexture));
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
		void CTextureManager::RemoveRestorableTexture(CTexture* inpTexture)
		{
#ifdef TARGET_ANDROID
			for(DYNAMIC_ARRAY<Rendering::TextureWeakPtr>::iterator it = mapTextureCache.begin(); it != mapTextureCache.end(); ++it)
			{
				if (Rendering::TexturePtr pTexture = (*it).lock())
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
	}
}

