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

#include <ChilliSource/Core/Resource/ResourceProviderOld.h>
#include <ChilliSource/Core/Resource/ResourceManagerDispenser.h>
#include <ChilliSource/Core/Image/Image.h>

namespace ChilliSource
{
	namespace Rendering
	{
		CS_DEFINE_NAMEDTYPE(FontManager);
		
		//-----------------------------------------------------------------
		/// Constructor
		///
		/// Default
		//-----------------------------------------------------------------
		FontManager::FontManager()
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
		Core::ResourceOldSPtr FontManager::GetResourceFromFile(ChilliSource::Core::StorageLocation ineStorageLocation, const std::string &instrFilePath)
		{
			//It's the texture that is passed in so we need to load the alphabet file
            std::string strFontFile;
            std::string strExt;
            Core::StringUtils::SplitBaseFilename(instrFilePath, strFontFile, strExt);
            strFontFile += ".alphabet";
            
			MapStringToResourceSPtr::iterator pExistingResource = mMapFilenameToResource.find(strFontFile);
			
			if(pExistingResource == mMapFilenameToResource.end())
			{
				Core::ResourceOldSPtr pResource(new Font());
				
				for(u32 nProvider = 0; nProvider < mResourceProviderOlds.size(); nProvider++)
				{
					if(mResourceProviderOlds[nProvider]->CreateResourceFromFile(ineStorageLocation, strFontFile, pResource))
					{
						CS_LOG_VERBOSE("Loading font " + strFontFile);
                        
						mMapFilenameToResource.insert(std::make_pair(strFontFile, pResource));
						
						FontSPtr pFont = std::static_pointer_cast<Font>(pResource);
						pFont->SetName(strFontFile);
						pFont->SetOwningResourceManager(this);
                        
                        SpriteSheetSPtr pFontData = LOAD_RESOURCE(SpriteSheet, ineStorageLocation, instrFilePath);
                        pFont->SetSpriteSheet(pFontData);
						return pFont;
					}
				}
			}
			else
			{
				return pExistingResource->second;
			}
			
			//Resource not found
			CS_LOG_FATAL("Cannot find resource for font with path " + instrFilePath);
			return nullptr;
		}
	}
}
