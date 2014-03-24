/*
 * File: FontManager.h
 * Date: 26/10/2010 2010 
 * Description: Manages the resource providers for font and caches the loaded font data
 */

/*
 * Author: Scott Downie
 * Version: v 1.0
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#ifndef _MO_FLO_GUI_FONT_MANAGER_H_
#define _MO_FLO_GUI_FONT_MANAGER_H_

#include <ChilliSource/Rendering/Font/Font.h>

#include <ChilliSource/Core/Resource/ResourceManager.h>
#include <ChilliSource/Core/Image/Image.h>

namespace ChilliSource
{
	namespace Rendering
	{
		class FontManager : public Core::ResourceManager
		{
		public:
			CS_DECLARE_NAMEDTYPE(FontManager);
			FontManager();
			
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
			Core::ResourceSPtr GetResourceFromFile(Core::StorageLocation ineStorageLocation, const std::string &instrFilePath) override;
			
		private:
			
            SpriteSheetManager* mpSpriteSheetManager;
		};
	}
}

#endif
