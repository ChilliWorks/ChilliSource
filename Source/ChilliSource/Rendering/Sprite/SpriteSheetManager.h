/*
 * File: SpriteManager.h
 * Date: 22/10/2010 2010 
 * Description: 
 */

/*
 * Author: Scott Downie
 * Version: v 1.0
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#ifndef _MO_FLO_RENDERING_SPRITE_MANAGER_H_
#define _MO_FLO_RENDERING_SPRITE_MANAGER_H_

#include <ChilliSource/Core/Resource/ResourceManager.h>
#include <ChilliSource/Core/Image/Image.h>

#include <ChilliSource/Rendering/ForwardDeclarations.h>
#include <ChilliSource/Rendering/Sprite/SpriteSheet.h>

namespace moFlo
{
	namespace Rendering
	{
		class ISpriteSheetManager : public Core::IResourceManager
		{
		public:
			DECLARE_NAMED_INTERFACE(ISpriteSheetManager);
			ISpriteSheetManager();

			//----------------------------------------------------------------
			/// Is A
			///
			/// Query the interface type
			/// @param The interface to compare
			/// @return Whether the object implements that interface
			//----------------------------------------------------------------
			bool IsA(Core::InterfaceIDType inInterfaceID) const;
			//----------------------------------------------------------------
			/// Get Resource Type
			///
			/// @return The type of resource this manager handles
			//----------------------------------------------------------------
			Core::InterfaceIDType GetResourceType() const;
			//----------------------------------------------------------------
			/// Get Provider Type
			///
			/// @return The type of resource it consumes from resource provider
			//----------------------------------------------------------------
			Core::InterfaceIDType GetProviderType() const; 
			//----------------------------------------------------------------
			/// Manages Resource Of Type
			///
			/// @return Whether this object manages the object of type
			//----------------------------------------------------------------
			bool ManagesResourceOfType(Core::InterfaceIDType inInterfaceID) const;
			//----------------------------------------------------------------
			/// Get Sprite Data From File
			///
			/// Creates (lazily loads) sprite data from file. If it already 
			/// exists it will return that instance
            ///
            /// @param The storage location to load from
            /// @param The filepath
            /// @param The image format
            /// @param Whether or not mip maps should be used
            /// @return the spritesheet resource pointer
			//----------------------------------------------------------------
			SpriteSheetPtr GetSpriteSheetFromFile(Core::StorageLocation ineStorageLocation, const std::string &inFilePath, Core::CImage::Format ineFormat = Core::CImage::Format::k_default, bool inbWithMipsMaps = false);
			//-----------------------------------------------------------------
			/// Get Resource From File
			///
			/// Generic call to get the managers resource
            /// @param The storage location to load from
			/// @param File path to resource
			/// @return Generic pointer to object type
			//-----------------------------------------------------------------
			Core::ResourcePtr GetResourceFromFile(Core::StorageLocation ineStorageLocation, const std::string &instrFilePath);
			
		private:
            
            ITextureManager* mpTextureManager;
		};
	}
}

#endif