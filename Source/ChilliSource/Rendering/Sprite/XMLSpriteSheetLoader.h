/*
 *  XMLSpriteSheetLoader.h
 *  moFlow
 *
 *  Created by Stuart McGaw on 13/05/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#ifndef _MOFLO_RENDERING_SPRITE_LOADER_H_
#define _MOFLO_RENDERING_SPRITE_LOADER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Resource/ResourceProvider.h>
#include <ChilliSource/Rendering/Sprite/SpriteSheetManager.h>

namespace ChilliSource
{
	namespace Rendering
	{
		class XMLSpriteSheetLoader : public Core::IResourceProvider
		{
		public:
			XMLSpriteSheetLoader();
			virtual ~XMLSpriteSheetLoader();
			
			//-------------------------------------------------------------------------
			/// Is A
			///
			/// @param Interface to compare
			/// @return Whether the object implements the given interface
			//-------------------------------------------------------------------------
			bool IsA(Core::InterfaceIDType inInterfaceID) const override;
			//----------------------------------------------------------------------------
			/// Can Create Resource of Kind
			///
			/// @param Type to compare
			/// @return Whether the object can create a resource of given type
			//----------------------------------------------------------------------------
			bool CanCreateResourceOfKind(Core::InterfaceIDType inInterfaceID) const override;
			//----------------------------------------------------------------------------
			/// Can Create Resource From File With Extension
			///
			/// @param Type to compare
			/// @param Extension to compare
			/// @return Whether the object can create a resource with the given extension
			//----------------------------------------------------------------------------
			bool CanCreateResourceFromFileWithExtension(const std::string & inExtension) const override;
			
		private:
			
			//----------------------------------------------------------------------------
			/// Create Resource From File
			///
            /// @param The storage location to load from
			/// @param Filename
			/// @param Out: Resource object
			/// @return Whether the resource loaded
			//----------------------------------------------------------------------------
			bool CreateResourceFromFile(Core::StorageLocation ineStorageLocation, const std::string & inFilePath, Core::ResourcePtr& outpResource) override;
		};
	}
}

#endif

