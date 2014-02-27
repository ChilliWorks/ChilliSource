/*
 * File: SpriteManager.cpp
 * Date: 22/10/2010 2010 
 * Description: 
 */

/*
 * Author: Scott Downie
 * Version: v 1.0
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#include <ChilliSource/Rendering/Sprite/SpriteSheetManager.h>
#include <ChilliSource/Rendering/Texture/TextureManager.h>
#include <ChilliSource/Rendering/Texture/Texture.h>

#include <ChilliSource/Core/Resource/ResourceProvider.h>
#include <ChilliSource/Core/Resource/ResourceManagerDispenser.h>

namespace ChilliSource
{
	namespace Rendering
	{
		DEFINE_NAMED_INTERFACE(SpriteSheetManager);
		
		//-----------------------------------------------------------------
		/// Constructor
		///
		/// Default
		//-----------------------------------------------------------------
		SpriteSheetManager::SpriteSheetManager() : mpTextureManager(nullptr)
		{
		}
		//----------------------------------------------------------------
		/// Is A
		///
		/// Query the interface type
		/// @param The interface to compare
		/// @return Whether the object implements that interface
		//----------------------------------------------------------------
		bool SpriteSheetManager::IsA(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == SpriteSheetManager::InterfaceID;
		}
		//----------------------------------------------------------------
		/// Get Resource Type
		///
		/// @return The type of resource this manager handles
		//----------------------------------------------------------------
		Core::InterfaceIDType SpriteSheetManager::GetResourceType() const
		{
			return SpriteSheet::InterfaceID;
		}
		//----------------------------------------------------------------
		/// Manages Resource Of Type
		///
		/// @return Whether this object manages the object of type
		//----------------------------------------------------------------
		bool SpriteSheetManager::ManagesResourceOfType(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == SpriteSheet::InterfaceID;
		}
		//----------------------------------------------------------------
		/// Get Provider Type
		///
		/// @return The type of resource it consumes from resource provider
		//----------------------------------------------------------------
		Core::InterfaceIDType SpriteSheetManager::GetProviderType() const
		{
			return SpriteSheet::InterfaceID;
		}
		//-----------------------------------------------------------------
		/// Get Resource From File
		///
		/// Generic call to get the managers resource
        /// @param The storage location to load from
		/// @param File path to resource
		/// @return Generic pointer to object type
		//-----------------------------------------------------------------
		Core::ResourceSPtr SpriteSheetManager::GetResourceFromFile(Core::StorageLocation ineStorageLocation, const std::string &instrFilePath)
		{
			return GetSpriteSheetFromFile(ineStorageLocation, instrFilePath, Core::Image::Format::k_default, false);
		}
		//----------------------------------------------------------------
		/// Get Sprite Data From File
		///
		/// Creates (lazily loads) sprite data from file. If the 
		/// sprite data is already loaded it returns a handle to it
        /// @param The storage location to load from
        /// @param the filepath
        /// @param image format
        /// @param whether or not to use mipmaps
        /// @return The sprites sheet resource pointer
		//----------------------------------------------------------------
		SpriteSheetSPtr SpriteSheetManager::GetSpriteSheetFromFile(Core::StorageLocation ineStorageLocation, const std::string &inFilePath, Core::Image::Format ineFormat, bool inbWithMipsMaps)
		{
            //It's the texture that is passed in so we need to load the binary file
            std::string strSpriteSheetFile;
            std::string strExt;
            Core::StringUtils::SplitBaseFilename(inFilePath, strSpriteSheetFile, strExt);
            strSpriteSheetFile += ".bin";
            
			MapStringToResourcePtr::iterator pExistingResource = mMapFilenameToResource.find(strSpriteSheetFile);
			
			if(pExistingResource == mMapFilenameToResource.end()) 
			{
				Core::ResourceSPtr pResource(new SpriteSheet());
				
				for(size_t nProvider = 0; nProvider < mResourceProviders.size(); nProvider++) 
				{
					if(mResourceProviders[nProvider]->CreateResourceFromFile(ineStorageLocation, strSpriteSheetFile, pResource)) 
					{
						//Add it to the cache
						CS_DEBUG_LOG("Loading sprite data " + strSpriteSheetFile);
						mMapFilenameToResource.insert(std::make_pair(strSpriteSheetFile, pResource));
						
						SpriteSheetSPtr pSpriteSheet = std::static_pointer_cast<SpriteSheet>(pResource);
						pSpriteSheet->SetName(strSpriteSheetFile);
						pSpriteSheet->SetFilename(inFilePath);
						pSpriteSheet->SetStorageLocation(ineStorageLocation);
						pSpriteSheet->SetOwningResourceManager(this);
						pSpriteSheet->SetLoaded(true);
                        
                        if(!mpTextureManager)
                        {
                            mpTextureManager = Core::ResourceManagerDispenser::GetSingletonPtr()->GetResourceManagerWithInterface<TextureManager>();
                        }
                        
                        std::string strNewFilePath = inFilePath;
                        TextureSPtr pTexture = mpTextureManager->GetTextureFromFile(ineStorageLocation, strNewFilePath, ineFormat, inbWithMipsMaps);
                        pSpriteSheet->SetTexture(pTexture);
                        
						return pSpriteSheet;
					}
				}
			} 
			else 
			{
				return std::static_pointer_cast<SpriteSheet>(pExistingResource->second);
			}
			
			//Resource not found
			CS_ERROR_LOG("Cannot find resource for sprite data with path " + inFilePath);
			return SpriteSheetSPtr();
		}
	}
}
