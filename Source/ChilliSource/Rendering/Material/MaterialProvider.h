//
//  MaterialProvider.h
//  Chilli Source
//
//  Created by Scott Downie on 22/11/2010.
//  Copyright 2010 Tag Games. All rights reserved.
//

#ifndef _CHILLISOURCE_RENDERING_MATERIAL_MATERIALPROVIDER_H_
#define _CHILLISOURCE_RENDERING_MATERIAL_MATERIALPROVIDER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Resource/ResourceProvider.h>

namespace ChilliSource
{
	namespace Rendering
	{
        //-------------------------------------------------------------------------
        /// Factory for creating material resources from material files
        ///
        /// @author S Downie
        //-------------------------------------------------------------------------
		class MaterialProvider final : public Core::ResourceProvider
		{
		public:
            
            CS_DECLARE_NAMEDTYPE(MaterialProvider);
            
            //-------------------------------------------------------------------------
            /// Holds the description of a shader as required by the material
            /// including which pass it applies to and where the shader file is
            ///
            /// @author S Downie
            //-------------------------------------------------------------------------
            struct ShaderDesc
            {
                std::string m_filePath;
                Core::StorageLocation m_location;
                ShaderPass m_pass;
            };
            //-------------------------------------------------------------------------
            /// Holds the description of a texture as required by the material
            /// including where the texture file is
            ///
            /// @author S Downie
            //-------------------------------------------------------------------------
            struct TextureDesc
            {
                std::string m_filePath;
                Core::StorageLocation m_location;
                bool m_shouldMipMap;
            };
            //-------------------------------------------------------------------------
            /// Factory method
            ///
            /// @author S Downie
			///
			/// @param The render capabilities.
            ///
            /// @return New instance with ownership transferred
			//-------------------------------------------------------------------------
			static MaterialProviderUPtr Create(RenderCapabilities* in_renderCapabilities);
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
			/// @return The resource type this provider creates
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
            ///
			/// @param The render capabilities.
            //-------------------------------------------------------
            MaterialProvider(RenderCapabilities* in_renderCapabilities);
			//----------------------------------------------------------------------------
            /// Load the material resource from the given material file. Check the
            /// resource load state for success or failure.
            ///
			/// @author S Downie
			///
            /// @param The storage location to load from
			/// @param File path
			/// @param [Out] Resource object
			//----------------------------------------------------------------------------
			void CreateResourceFromFile(Core::StorageLocation in_location, const std::string& in_filePath, Core::ResourceSPtr& out_resource) override;
			//----------------------------------------------------------------------------
            /// Load the material resource from the given material file on a background
            /// thread. Calls the delegate on completion. Check the
            /// resource load state for success or failure.
            ///
			/// @author S Downie
			///
            /// @param The storage location to load from
			/// @param File path
            /// @param Completion delegate
			/// @param [Out] Resource object
			//----------------------------------------------------------------------------
			void CreateResourceFromFileAsync(Core::StorageLocation in_location, const std::string& in_filePath, const Core::ResourceProvider::AsyncLoadDelegate& in_delegate, Core::ResourceSPtr& out_resource) override;
			//----------------------------------------------------------------------------
			/// Build Material Task
			///
            /// @param The storage location to load from
			/// @param File path
            /// @param Completion delegate
            ///
			/// @param [Out] Resource object
			//----------------------------------------------------------------------------
			void BuildMaterialTask(Core::StorageLocation in_location, const std::string& in_filePath, const Core::ResourceProvider::AsyncLoadDelegate& in_delegate, Core::ResourceSPtr& out_resource);
			//----------------------------------------------------------------------------
			/// Build Material From File
			///
            /// @param The storage location to load from
			/// @param File path
            /// @param [Out] Shaders to load
            /// @param [Out] Textures to load
            /// @param [Out] Cubemaps to load
			/// @param [Out] Material resource
            ///
			/// @return Whether the resource was loaded 
			//----------------------------------------------------------------------------
            bool BuildMaterialFromFile(Core::StorageLocation in_location, const std::string& in_filePath,
                                       std::vector<ShaderDesc>& out_shaderFiles,
                                       std::vector<TextureDesc>& out_textureFiles,
                                       std::vector<TextureDesc>& out_cubemapFiles,
                                       Material* out_material);

        private:

            RenderCapabilities* m_renderCapabilities;
		};
	}
}

#endif
