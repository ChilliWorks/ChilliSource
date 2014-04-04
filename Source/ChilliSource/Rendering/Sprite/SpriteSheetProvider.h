//
//  SpriteSheetProvider.h
//  Chilli Source
//
//  Created by Scott Downie on 22/10/2010.
//  Copyright 2010 Tag Games. All rights reserved.
//

#ifndef _CHILLISOURCE_RENDERING_SPRITE_SPRITESHEETPROVIDER_H_
#define _CHILLISOURCE_RENDERING_SPRITE_SPRITESHEETPROVIDER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Resource/ResourceProviderOld.h>

namespace ChilliSource
{
	namespace Rendering
	{
        //-------------------------------------------------------
        /// Factory loader for creating sprite sheet resources
        /// from file. Sprite sheets are texture atlases
        /// with. This loader is responsible for loading the
        /// sprite sheet map and frame information.
        ///
        /// @author S Downie
        //-------------------------------------------------------
		class SpriteSheetProvider final : public Core::ResourceProviderOld
		{
		public:
            
            CS_DECLARE_NAMEDTYPE(SpriteSheetProvider);
            
            //-------------------------------------------------------
            /// Factory method
            ///
            /// @author S Downie
            ///
            /// @return New backend with ownership transferred
            //-------------------------------------------------------
            static SpriteSheetProviderUPtr Create();
			//-------------------------------------------------------------------------
			/// @author S Downie
			///
			/// @param Interface to compare
            ///
			/// @return Whether the object implements the given interface
			//-------------------------------------------------------------------------
			bool IsA(Core::InterfaceIDType in_interfaceId) const override;
			//----------------------------------------------------------------------------
			/// @author S Downie
			///
			/// @param Type to compare
            ///
			/// @return Whether the object can create a resource of given type
			//----------------------------------------------------------------------------
			bool CanCreateResourceOfKind(Core::InterfaceIDType in_interfaceId) const override;
			//----------------------------------------------------------------------------
			/// @author S Downie
			///
			/// @param Extension to compare
            ///
			/// @return Whether the object can create a resource with the given extension
			//----------------------------------------------------------------------------
			bool CanCreateResourceFromFileWithExtension(const std::string& in_extension) const override;
			
		private:
            //-------------------------------------------------------
            /// Private constructor to force use of factory method
            ///
            /// @author S Downie
            //-------------------------------------------------------
            SpriteSheetProvider(){}
			//----------------------------------------------------------------------------
			/// Loads the two files that constitute a sprite sheet resource and
            /// parses them into the output resource
            ///
            /// @author S Downie
			///
			/// @param Location to load from
			/// @param Filename
			/// @param [Out] Resource object
            ///
			/// @return Whether the resource loaded
			//----------------------------------------------------------------------------
			bool CreateResourceFromFile(Core::StorageLocation in_location, const std::string& in_filePath, Core::ResourceOldSPtr& out_resource) override;
            //----------------------------------------------------------------------------
			/// Loads the two files that constitute a sprite sheet resource and
            /// parses them into the output resource on a background thread. The IsLoaded
            /// flag of the resource is set on completion
            ///
            /// @author S Downie
			///
			/// @param Location to load from
			/// @param Filename
			/// @param [Out] Resource object
            ///
			/// @return Whether the resource is about to be loaded
			//----------------------------------------------------------------------------
			bool AsyncCreateResourceFromFile(Core::StorageLocation in_location, const std::string& in_filePath, Core::ResourceOldSPtr& out_resource) override;
            //----------------------------------------------------------------------------
			/// Loads the file containing the frame data for each sprite in the sheet
            ///
            /// @author S Downie
			///
			/// @param Location to load from
			/// @param Filename
			/// @param [Out] Sprite sheet resource object
			//----------------------------------------------------------------------------
            void LoadFrames(Core::StorageLocation in_location, const std::string& in_filePath, SpriteSheetSPtr& out_resource);
            //----------------------------------------------------------------------------
			/// Loads the file containing the map key data for each sprite in the sheet
            ///
            /// @author S Downie
			///
			/// @param Location to load from
			/// @param Filename
			/// @param [Out] Sprite sheet resource object
			//----------------------------------------------------------------------------
            void LoadMap(Core::StorageLocation in_location, const std::string& in_filePath, SpriteSheetSPtr& out_resource);
		};
	}
}

#endif

