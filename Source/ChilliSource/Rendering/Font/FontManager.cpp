/*
 * File: FontManager.cpp
 * Date: 26/10/2010 2010 
 * Description: 
 */

/*
 * Author: Scott Downie
 * Version: v 1.0
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#include <ChilliSource/Rendering/Font/FontManager.h>

#include <ChilliSource/Rendering/Texture/TextureManager.h>
#include <ChilliSource/Rendering/Sprite/SpriteSheetManager.h>

#include <ChilliSource/Core/Resource/ResourceProvider.h>
#include <ChilliSource/Core/Resource/ResourceManagerDispenser.h>
#include <ChilliSource/Core/Image/Image.h>

namespace ChilliSource
{
	namespace Rendering
	{
		DEFINE_NAMED_INTERFACE(FontManager);
		
		//-----------------------------------------------------------------
		/// Constructor
		///
		/// Default
		//-----------------------------------------------------------------
		FontManager::FontManager() : mpSpriteSheetManager(nullptr)
		{
		}
		//----------------------------------------------------------------
		/// Is A
		///
		/// Query the interface type
		/// @param The interface to compare
		/// @return Whether the object implements that interface
		//----------------------------------------------------------------
		bool FontManager::IsA(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == FontManager::InterfaceID;
		}
		//----------------------------------------------------------------
		/// Get Resource Type
		///
		/// @return The type of resource this manager handles
		//----------------------------------------------------------------
		Core::InterfaceIDType FontManager::GetResourceType() const
		{
			return Font::InterfaceID;
		}
		//----------------------------------------------------------------
		/// Manages Resource Of Type
		///
		/// @return Whether this object manages the object of type
		//----------------------------------------------------------------
		bool FontManager::ManagesResourceOfType(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == Font::InterfaceID;
		}
		//----------------------------------------------------------------
		/// Get Provider Type
		///
		/// @return The type of resource it consumes from resource provider
		//----------------------------------------------------------------
		Core::InterfaceIDType FontManager::GetProviderType() const
		{
			return Font::InterfaceID;
		}
		//-----------------------------------------------------------------
		/// Get Resource From File
		///
		/// Generic call to get the managers resource
		/// @param File path to resource
		/// @return Generic pointer to object type
		///
        /// @param The storage location to load from
		/// @param File name
		//-----------------------------------------------------------------
		Core::ResourcePtr FontManager::GetResourceFromFile(ChilliSource::Core::StorageLocation ineStorageLocation, const std::string &instrFilePath)
		{
			return GetFontFromFile(ineStorageLocation, instrFilePath, Core::CImage::Format::k_default);
		}
		//----------------------------------------------------------------
		/// Get Font From File
		///
		/// Creates (lazily loads) fontfrom file. If the 
		/// font is already loaded it returns a handle to it
		///
        /// @param The storage location to load from
		/// @param File name
		/// @param Image format
		//----------------------------------------------------------------
		FontPtr FontManager::GetFontFromFile(ChilliSource::Core::StorageLocation ineStorageLocation, const std::string &inFilePath, Core::CImage::Format ineFormat)
		{
            //It's the texture that is passed in so we need to load the alphabet file
            std::string strFontFile;
            std::string strExt;
            Core::CStringUtils::SplitBaseFilename(inFilePath, strFontFile, strExt);
            strFontFile += ".alphabet";
            
			MapStringToResourcePtr::iterator pExistingResource = mMapFilenameToResource.find(strFontFile);
			
			if(pExistingResource == mMapFilenameToResource.end()) 
			{
				Core::ResourcePtr pResource(new Font());
				
				for(u32 nProvider = 0; nProvider < mResourceProviders.size(); nProvider++) 
				{
					if(mResourceProviders[nProvider]->CreateResourceFromFile(ineStorageLocation, strFontFile, pResource)) 
					{
						CS_DEBUG_LOG("Loading font " + strFontFile);
                        
						mMapFilenameToResource.insert(std::make_pair(strFontFile, pResource));
						
						FontPtr pFont = std::static_pointer_cast<Font>(pResource);
						pFont->SetName(strFontFile);
						pFont->SetOwningResourceManager(this);
						pFont->SetLoaded(true);
                        
                        if(!mpSpriteSheetManager)
                        {
                            mpSpriteSheetManager = Core::CResourceManagerDispenser::GetSingletonPtr()->GetResourceManagerWithInterface<ISpriteSheetManager>();
                        }
                        
                        SpriteSheetPtr pFontData = mpSpriteSheetManager->GetSpriteSheetFromFile(ineStorageLocation, inFilePath, ineFormat, false);
                        pFont->SetSpriteSheet(pFontData);
						return pFont;
					}
				}
			} 
			else 
			{
				return std::static_pointer_cast<Font>(pExistingResource->second);
			}
			
			//Resource not found
			CS_ERROR_LOG("Cannot find resource for font with path " + inFilePath);
			return FontPtr();
		}
	}
}
