/*
 * File: SpriteSheetLoader.h
 * Date: 22/10/2010 2010 
 * Description: Loads the sprite data from external file. This will include animation frames, timings and the tpage
 */

/*
 * Author: Scott Downie
 * Version: v 1.0
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#ifndef _MO_FLO_RENDERING_SPRITE_LOADER_H_
#define _MO_FLO_RENDERING_SPRITE_LOADER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Resource/ResourceProvider.h>
#include <ChilliSource/Rendering/Sprite/SpriteSheetManager.h>

namespace ChilliSource
{
	namespace Rendering
	{
		class SpriteSheetLoader : public Core::ResourceProvider
		{
		public:
            //-------------------------------------------------------
            /// Factory method
            ///
            /// @author S Downie
            ///
            /// @return New backend with ownership transferred
            //-------------------------------------------------------
            static SpriteSheetLoaderUPtr Create();
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
            //-------------------------------------------------------
            /// Private constructor to force use of factory method
            ///
            /// @author S Downie
            //-------------------------------------------------------
            SpriteSheetLoader(){}
			//----------------------------------------------------------------------------
			/// Create Resource From File
			///
			/// @param Location to load from
			/// @param Filename
			/// @param Out: Resource object
			/// @return Whether the resource loaded
			//----------------------------------------------------------------------------
			bool CreateResourceFromFile(Core::StorageLocation ineLocation, const std::string & inFilePath, Core::ResourceSPtr& outpResource) override;
		};
	}
}

#endif

