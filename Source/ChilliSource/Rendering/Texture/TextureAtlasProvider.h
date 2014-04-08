//
//  TextureAtlasProvider.h
//  Chilli Source
//
//  Created by Scott Downie on 22/10/2010.
//  Copyright 2010 Tag Games. All rights reserved.
//

#ifndef _CHILLISOURCE_RENDERING_TEXTURE_TEXTUREATLASPROVIDER_H_
#define _CHILLISOURCE_RENDERING_TEXTURE_TEXTUREATLASPROVIDER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Resource/ResourceProvider.h>
#include <ChilliSource/Rendering/Texture/TextureAtlas.h>

namespace ChilliSource
{
	namespace Rendering
	{
        //-------------------------------------------------------
        /// Factory loader for creating texture atlas resources
        /// from file. This loader is responsible for loading the
        /// keys and frame values.
        ///
        /// @author S Downie
        //-------------------------------------------------------
		class TextureAtlasProvider final : public Core::ResourceProvider
		{
		public:
            
            CS_DECLARE_NAMEDTYPE(TextureAtlasProvider);
            
            //-------------------------------------------------------
            /// Factory method
            ///
            /// @author S Downie
            ///
            /// @return New backend with ownership transferred
            //-------------------------------------------------------
            static TextureAtlasProviderUPtr Create();
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
			/// @return The resource type this provider loads
			//----------------------------------------------------------------------------
			Core::InterfaceIDType GetResourceType() const override;
			//----------------------------------------------------------------------------
			/// @author S Downie
			///
			/// @param Extension to compare
            ///
			/// @return Whether the object can create a resource with the given extension
			//----------------------------------------------------------------------------
			bool CanCreateResourceWithFileExtension(const std::string& in_extension) const override;
			
		private:
            //-------------------------------------------------------
            /// Private constructor to force use of factory method
            ///
            /// @author S Downie
            //-------------------------------------------------------
            TextureAtlasProvider() = default;
			//----------------------------------------------------------------------------
			/// Loads the two files that constitute an atlas resource and
            /// parses them into the output resource. Check the resource load state
            /// for success or failure.
            ///
            /// @author S Downie
			///
			/// @param Location to load from
			/// @param Filename
			/// @param [Out] Resource object
			//----------------------------------------------------------------------------
			void CreateResourceFromFile(Core::StorageLocation in_location, const std::string& in_filePath, Core::ResourceSPtr& out_resource) override;
            //----------------------------------------------------------------------------
			/// Loads the two files that constitute a atlas resource and
            /// parses them into the output resource on a background thread. Delegate
            /// is called on completion. Check the resource load state for success or failure
            ///
            /// @author S Downie
			///
			/// @param Location to load from
			/// @param Filename
            /// @param Completion delegate
			/// @param [Out] Resource object
			//----------------------------------------------------------------------------
			void CreateResourceFromFileAsync(Core::StorageLocation in_location, const std::string& in_filePath, const Core::ResourceProvider::AsyncLoadDelegate& in_delegate, Core::ResourceSPtr& out_resource) override;
            //----------------------------------------------------------------------------
			/// Performs the actual loading delegated to by the 2 create methods
            ///
            /// @author S Downie
			///
			/// @param Location to load from
			/// @param Filename
            /// @param Completion delegate
			/// @param [Out] Resource object
			//----------------------------------------------------------------------------
            void LoadResource(Core::StorageLocation in_location, const std::string& in_filePath, const Core::ResourceProvider::AsyncLoadDelegate& in_delegate, Core::ResourceSPtr& out_resource);
            //----------------------------------------------------------------------------
			/// Loads the file containing the frame data for each texture in the atlas
            ///
            /// @author S Downie
			///
			/// @param Location to load from
			/// @param Filename
            /// @param [Out] Descriptor
			//----------------------------------------------------------------------------
            void LoadFrames(Core::StorageLocation in_location, const std::string& in_filePath, TextureAtlas::Descriptor& out_desc);
            //----------------------------------------------------------------------------
			/// Loads the file containing the map key data for each texture in the atlas
            ///
            /// @author S Downie
			///
			/// @param Location to load from
			/// @param Filename
            /// @param [Out] Descriptor
			//----------------------------------------------------------------------------
            void LoadMap(Core::StorageLocation in_location, const std::string& in_filePath, TextureAtlas::Descriptor& out_desc);
		};
	}
}

#endif

