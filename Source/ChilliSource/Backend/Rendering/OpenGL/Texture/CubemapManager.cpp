/*
 *  GLCubemapManager.cpp
 *  moFlo
 *
 *  Created by Tag Games on 01/10/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Backend/Rendering/OpenGL/Texture/CubemapManager.h>

#include <ChilliSource/Backend/Rendering/OpenGL/Texture/Cubemap.h>
#include <ChilliSource/Core/Image/ImageResourceProvider.h>

namespace ChilliSource
{
	namespace OpenGL
	{
		//----------------------------------------------------------------
		/// Create Cubemap Resource
		///
		/// @return Concrete Cubemap resource based on the render system
		//----------------------------------------------------------------
		ChilliSource::Rendering::CubemapSPtr CubemapManager::CreateCubemapResource()
		{
			Rendering::CubemapSPtr pCubemap(new Cubemap(this));
			AddRestorableCubemap(pCubemap);
			return pCubemap;
		}
		//----------------------------------------------------------------
		/// Create Cubemap From Image
		///
		/// @param Images
		/// @param Generate mip maps
		/// @param Out: Cubemap resource
		/// @return Success
		//----------------------------------------------------------------
		bool CubemapManager::CreateCubemapFromImages(const std::vector<Core::ResourceSPtr>& inaImages, bool inbWithMipsMaps, ChilliSource::Rendering::CubemapSPtr& outpCubemap)
		{
			std::static_pointer_cast<Cubemap>(outpCubemap)->Init(inaImages, inbWithMipsMaps);
			return true;
		}
		//----------------------------------------------------------------
		/// Restore
		///
		/// Restore all Cubemaps either from file or from cached images.
		//----------------------------------------------------------------
		void CubemapManager::Restore()
		{
#ifdef CS_TARGETPLATFORM_ANDROID
			for(std::vector<Rendering::CubemapWPtr>::iterator it = mapCubemapCache.begin(); it != mapCubemapCache.end(); ++it)
			{
				if (Rendering::CubemapSPtr pCubemap = (*it).lock())
				{
					if(pCubemap->IsLoaded())
					{
						std::shared_ptr<Cubemap> pOpenGLCubemap = std::static_pointer_cast<Cubemap>(pCubemap);
                        
                        //If the Cubemap was loaded from file then reload it.
                        if(pOpenGLCubemap->GetFilename() != "" && pOpenGLCubemap->GetStorageLocation() != Core::StorageLocation::k_none)
                        {
                            std::vector<Core::ResourceSPtr> aImages;
                            aImages.reserve(6);
                            
                            std::string strPath;
                            std::string strExt;
                            Core::StringUtils::SplitBaseFilename(pOpenGLCubemap->GetFilename(), strPath, strExt);
                            
                            for (u32 nProvider = 0; nProvider < mResourceProviders.size(); nProvider++)
                            {
                                if(mResourceProviders[nProvider]->CanCreateResourceFromFileWithExtension(strExt))
                                {
                                    strExt = "." + strExt;
                                    
                                    for(u32 i=0; i<6; ++i)
                                    {
                                        Core::ResourceSPtr pSourceImage(new Core::Image());
                                        Core::Image* pImage = (Core::Image*)(pSourceImage.get());
                                        std::string strFileName = strPath + Core::ToString(i+1) + strExt;
                                        if(static_cast<Core::ImageResourceProvider*>(mResourceProviders[nProvider])->CreateImageFromFile(pOpenGLCubemap->GetStorageLocation(), strFileName, pOpenGLCubemap->GetImageFormat(), pSourceImage))
                                        {
                                            pImage->SetName(strFileName);
                                            pImage->SetLoaded(true);
                                            aImages.push_back(pSourceImage);
                                        }
                                    }
                                    
                                    if(aImages.size() == 6)
                                    {
                                        CreateCubemapFromImages(aImages, pOpenGLCubemap->HasMipMaps(), pCubemap);
                                    }
                                    else
                                    {
                                        CS_LOG_ERROR("Cannot find all resources for Cubemap with base path " + pOpenGLCubemap->GetFilename());
                                    }
                                    
                                    break;
                                }
                            }
                        }
					}
				}
			}

			mapBackedUpImages.clear();
#endif
		}
		//----------------------------------------------------------------
		/// Add Restorable Cubemap
		///
		/// Adds the Cubemap to the list of all created Cubemaps. This
		/// list is used to restore the Cubemaps when the context is lost.
		///
		/// @param The Cubemap pointer.
		//----------------------------------------------------------------
		void CubemapManager::AddRestorableCubemap(const Rendering::CubemapSPtr& inpCubemap)
		{
#ifdef CS_TARGETPLATFORM_ANDROID
			mapCubemapCache.push_back(Rendering::CubemapWPtr(inpCubemap));
#endif
		}
		//----------------------------------------------------------------
		/// Remove Restorable Cubemap
		///
		/// Removes the Cubemap from the list of all created Cubemaps. This
		/// list is used to restore the Cubemaps when the context is lost.
		///
		/// @param The Cubemap pointer.
		//----------------------------------------------------------------
		void CubemapManager::RemoveRestorableCubemap(Cubemap* inpCubemap)
		{
#ifdef CS_TARGETPLATFORM_ANDROID
			for(std::vector<Rendering::CubemapWPtr>::iterator it = mapCubemapCache.begin(); it != mapCubemapCache.end(); ++it)
			{
				if (Rendering::CubemapSPtr pCubemap = (*it).lock())
				{
					if (pCubemap.get() == inpCubemap)
					{
						mapCubemapCache.erase(it);
						break;
					}
				}
			}
#endif
		}
	}
}

