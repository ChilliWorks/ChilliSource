//
//  MaterialLoader.h
//  MoFlow
//
//  Created by Scott Downie on 22/11/2010.
//  Copyright 2010 Tag Games. All rights reserved.
//

#ifndef _MO_FLO_RENDERING_MATERIAL_LOADER_H_
#define _MO_FLO_RENDERING_MATERIAL_LOADER_H_

#include <ChilliSource/Rendering/ForwardDeclarations.h>
#include <ChilliSource/Rendering/Material/Material.h>

#include <ChilliSource/Core/ForwardDeclarations.h>
#include <ChilliSource/Core/Resource/ResourceProvider.h>

namespace moFlo
{
	namespace Rendering
	{
		class CMaterialLoader : public Core::IResourceProvider
		{
		public:
			//-------------------------------------------------------------------------
			/// Constructor
			///
			/// @param The render capabilities.
			//-------------------------------------------------------------------------
			CMaterialLoader(IRenderCapabilities* inpRenderCapabilities);
			//-------------------------------------------------------------------------
			/// Is A
			///
			/// @param Interface to compare
			/// @return Whether the object implements the given interface
			//-------------------------------------------------------------------------
			bool IsA(Core::InterfaceIDType inInterfaceID) const;
			//----------------------------------------------------------------------------
			/// Can Create Resource of Kind
			///
			/// @param Type to compare
			/// @return Whether the object can create a resource of given type
			//----------------------------------------------------------------------------
			bool CanCreateResourceOfKind(Core::InterfaceIDType inInterfaceID) const;
			//----------------------------------------------------------------------------
			/// Can Create Resource From File With Extension
			///
			/// @param Type to compare
			/// @param Extension to compare
			/// @return Whether the object can create a resource with the given extension
			//----------------------------------------------------------------------------
			bool CanCreateResourceFromFileWithExtension(const std::string & inExtension) const;
			
		private:
			
            struct TextureDesc
            {
                std::string mstrFile;
                Core::STORAGE_LOCATION meLocation;
                bool mbMipMapped;
            };
            
			//----------------------------------------------------------------------------
			/// Create Resource From File
			///
            /// @param The storage location to load from
			/// @param Filename
			/// @param Out: Resource object
			/// @return Whether the resource was loaded 
			//----------------------------------------------------------------------------
			bool CreateResourceFromFile(Core::STORAGE_LOCATION ineStorageLocation, const std::string & inFilePath, Core::ResourcePtr& outpResource);
			//----------------------------------------------------------------------------
			/// Async Create Resource From File
			///
            /// @param The storage location to load from
			/// @param Filename
			/// @param Out: Resource object
			/// @return Whether the resource was loaded 
			//----------------------------------------------------------------------------
			bool AsyncCreateResourceFromFile(Core::STORAGE_LOCATION ineStorageLocation, const std::string & inFilePath, Core::ResourcePtr& outpResource);
			//----------------------------------------------------------------------------
			/// Build Material Task
			///
            /// @param The storage location to load from
			/// @param Filename
			/// @param Out: Resource object
			//----------------------------------------------------------------------------
			void BuildMaterialTask(Core::STORAGE_LOCATION ineStorageLocation, const std::string & inFilePath, Core::ResourcePtr& outpResource);
			//----------------------------------------------------------------------------
			/// Set Loaded Task
			///
			/// @param Out: Resource object
			//----------------------------------------------------------------------------
			void SetLoadedTask(Core::ResourcePtr& outpResource);
			//----------------------------------------------------------------------------
			/// Build Material From File
			///
            /// @param The storage location to load from
			/// @param Filename
            /// @param Out: Shaders to load
            /// @param Out: Textures to load
            /// @param Out: Cubemaps to load
			/// @param Out: Resource object
			/// @return Whether the resource was loaded 
			//----------------------------------------------------------------------------
            bool BuildMaterialFromFile(Core::STORAGE_LOCATION ineStorageLocation, const std::string & inFilePath,
                                       DYNAMIC_ARRAY<std::pair<ShaderPass, std::pair<Core::STORAGE_LOCATION, std::string> > >& outaShaderFiles,
                                       DYNAMIC_ARRAY<TextureDesc>& outaTextureFiles,
                                       DYNAMIC_ARRAY<TextureDesc>& outaCubemapFiles,
                                       Core::ResourcePtr& outpResource);
			//----------------------------------------------------------------------------
			/// Convert String To Blend Function
			///
			/// The material files define their functions in string format. We
			/// need to convert them to moFlo's blend type
			///
			/// @param String function
			/// @return Blend type
			//----------------------------------------------------------------------------
			AlphaBlend ConvertStringToBlendFunction(const std::string &instrFunc);
            //----------------------------------------------------------------------------
            /// Convert String To Cull Face
            ///
            /// @param String
            /// @return Cull face type
            //----------------------------------------------------------------------------
            CullFace ConvertStringToCullFace(const std::string &instrFace);
            //----------------------------------------------------------------------------
            /// Get Shader Files For Material Type
            ///
            /// Each material type has associated shader programs. This function will
            /// populate the container with the shader locations
            ///
            /// @param Material type
            /// @param Out: Shader files
            //----------------------------------------------------------------------------
            void GetShaderFilesForMaterialType(const std::string& instrType, DYNAMIC_ARRAY<std::pair<ShaderPass, std::pair<Core::STORAGE_LOCATION, std::string> > >& outaShaderFiles) const;

            IRenderCapabilities* mpRenderCapabilities;
		};
	}
}

#endif
