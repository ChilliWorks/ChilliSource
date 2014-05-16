//
//  TextureAtlasProvider.h
//  Chilli Source
//  Created by Scott Downie on 22/10/2010.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2010 Tag Games Limited
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
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
			friend class Core::Application;
			//-------------------------------------------------------
			/// Factory method
			///
			/// @author S Downie
			///
			/// @return New backend with ownership transferred
			//-------------------------------------------------------
			static TextureAtlasProviderUPtr Create();
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
			/// @param File path
            /// @param Options to customise the creation
			/// @param [Out] Resource object
			//----------------------------------------------------------------------------
			void CreateResourceFromFile(Core::StorageLocation in_location, const std::string& in_filePath, const Core::IResourceOptionsBaseCSPtr& in_options, const Core::ResourceSPtr& out_resource) override;
            //----------------------------------------------------------------------------
			/// Loads the two files that constitute a atlas resource and
            /// parses them into the output resource on a background thread. Delegate
            /// is called on completion. Check the resource load state for success or failure
            ///
            /// @author S Downie
			///
			/// @param Location to load from
			/// @param File path
            /// @param Options to customise the creation
            /// @param Completion delegate
			/// @param [Out] Resource object
			//----------------------------------------------------------------------------
			void CreateResourceFromFileAsync(Core::StorageLocation in_location, const std::string& in_filePath, const Core::IResourceOptionsBaseCSPtr& in_options, const Core::ResourceProvider::AsyncLoadDelegate& in_delegate, const Core::ResourceSPtr& out_resource) override;
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
			void LoadResource(Core::StorageLocation in_location, const std::string& in_filePath, const Core::ResourceProvider::AsyncLoadDelegate& in_delegate, const Core::ResourceSPtr& out_resource);
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

