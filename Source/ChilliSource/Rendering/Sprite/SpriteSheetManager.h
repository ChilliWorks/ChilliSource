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

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Resource/ResourceManager.h>
#include <ChilliSource/Core/Image/Image.h>
#include <ChilliSource/Rendering/Sprite/SpriteSheet.h>

namespace ChilliSource
{
	namespace Rendering
	{
		class SpriteSheetManager : public Core::ResourceManager
		{
		public:
			CS_DECLARE_NAMEDTYPE(SpriteSheetManager);
			SpriteSheetManager();

			//----------------------------------------------------------------
			/// Is A
			///
			/// Query the interface type
			/// @param The interface to compare
			/// @return Whether the object implements that interface
			//----------------------------------------------------------------
			bool IsA(Core::InterfaceIDType inInterfaceID) const override;
			//----------------------------------------------------------------
			/// Get Resource Type
			///
			/// @return The type of resource this manager handles
			//----------------------------------------------------------------
			Core::InterfaceIDType GetResourceType() const override;
			//----------------------------------------------------------------
			/// Get Provider Type
			///
			/// @return The type of resource it consumes from resource provider
			//----------------------------------------------------------------
			Core::InterfaceIDType GetProviderType() const override;
			//----------------------------------------------------------------
			/// Manages Resource Of Type
			///
			/// @return Whether this object manages the object of type
			//----------------------------------------------------------------
			bool ManagesResourceOfType(Core::InterfaceIDType inInterfaceID) const override;
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
			SpriteSheetSPtr GetSpriteSheetFromFile(Core::StorageLocation ineStorageLocation, const std::string &inFilePath, Core::Image::Format ineFormat = Core::Image::Format::k_default, bool inbWithMipsMaps = false);
			//-----------------------------------------------------------------
			/// Get Resource From File
			///
			/// Generic call to get the managers resource
            /// @param The storage location to load from
			/// @param File path to resource
			/// @return Generic pointer to object type
			//-----------------------------------------------------------------
			Core::ResourceSPtr GetResourceFromFile(Core::StorageLocation ineStorageLocation, const std::string &instrFilePath) override;
			
		private:
            
            TextureManager* mpTextureManager;
		};
	}
}

#endif