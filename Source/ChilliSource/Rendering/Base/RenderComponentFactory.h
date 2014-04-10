/*
 *  RenderComponentFactory.h
 *  moFlo
 *
 *  Created by Scott Downie on 29/09/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#ifndef _MO_FLO_RENDERING_RENDER_COMPONENT_FACTORY_H_
#define _MO_FLO_RENDERING_RENDER_COMPONENT_FACTORY_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Entity/ComponentFactory.h>
#include <ChilliSource/Core/File/FileSystem.h>

namespace ChilliSource
{
	namespace Rendering
	{
		//======================================================
		/// Render Component Factory - Interface
		///
		/// Allows objects to instantiate renderable components
		//======================================================
		class RenderComponentFactory : public Core::ComponentFactory
		{
		public: 
			CS_DECLARE_NAMEDTYPE(RenderComponentFactory);
			RenderComponentFactory(RenderSystem* inpRenderSystem);
			virtual ~RenderComponentFactory(){}
            //--------------------------------------------------------
            /// Is A
            ///
            /// @return If the object is of the given interface type
            //--------------------------------------------------------
            bool IsA(Core::InterfaceIDType inInterfaceID) const override;
            //--------------------------------------------------------
            /// Can Produce Component With Interface
            ///
            /// Used to determine if this factory can produce 
            /// component of given type.
            ///
            /// @param The ID of the component to create
            /// @return Whether the object can create component of ID
            //--------------------------------------------------------
            bool CanProduceComponentWithInterface(Core::InterfaceIDType inTypeID) const override;
            //--------------------------------------------------------
            /// Can Produce Component With Type Name
            ///
            /// Used to determine if this factory can produce 
            /// component of given type.
            ///
            /// @param The name of the component to create
            /// @return Whether the object can create component
            //--------------------------------------------------------
            bool CanProduceComponentWithTypeName(const std::string & incName) const override;
			
            //---------------------------------------------------------------------------
			/// Create Static Sprite Component
			///
			/// A number of overloads 
			//---------------------------------------------------------------------------
			SpriteComponentUPtr CreateSpriteComponent(const Core::Vector2 &invDims, const MaterialCSPtr& inMaterial);
			SpriteComponentUPtr CreateSpriteComponent(const Core::Vector2 &invDims, Core::StorageLocation ineStorageLocation, const std::string& instrMaterialFilePath);
			SpriteComponentUPtr CreateSpriteComponent(const TextureAtlasCSPtr& pTextureAtlas, const std::string& in_textureId, const MaterialCSPtr& inpMaterial);
            SpriteComponentUPtr CreateSpriteComponent(Core::StorageLocation ineStorageLocation, const std::string& instrTextureAtlas, const std::string& inTpageID, const MaterialCSPtr& inpMaterial);

            //---------------------------------------------------------------------------
			/// Create Static Mesh Component
			///
			/// A number of overloads 
			//---------------------------------------------------------------------------
            StaticMeshComponentUPtr CreateStaticMeshComponent(const MeshCSPtr& inpModel);
			StaticMeshComponentUPtr CreateStaticMeshComponent(const MeshCSPtr& inpModel, const MaterialCSPtr& inpMaterial);
			StaticMeshComponentUPtr CreateStaticMeshComponent(const MeshCSPtr& inpModel, Core::StorageLocation ineStorageLocation, const std::string & insMaterialName);
			StaticMeshComponentUPtr CreateStaticMeshComponent(Core::StorageLocation ineStorageLocation, const std::string& instrModelFileName);
			StaticMeshComponentUPtr CreateStaticMeshComponent(Core::StorageLocation ineStorageLocation, const std::string& instrModelFileName, const MaterialCSPtr& inpMaterial);
			StaticMeshComponentUPtr CreateStaticMeshComponent(Core::StorageLocation ineModelStorageLocation, const std::string& instrModelFileName, Core::StorageLocation ineMaterialStorageLocation, const std::string& instrMaterialFileName);
            
            //---------------------------------------------------------------------------
			/// Create Animated Mesh Component
			///
			/// A number of overloads 
			//---------------------------------------------------------------------------
            AnimatedMeshComponentUPtr CreateAnimatedMeshComponent(const MeshCSPtr& inpModel);
			AnimatedMeshComponentUPtr CreateAnimatedMeshComponent(const MeshCSPtr& inpModel, const MaterialCSPtr& inpMaterial);
			AnimatedMeshComponentUPtr CreateAnimatedMeshComponent(const MeshCSPtr& inpModel, Core::StorageLocation ineStorageLocation, const std::string & insMaterialName);
			AnimatedMeshComponentUPtr CreateAnimatedMeshComponent(Core::StorageLocation ineStorageLocation, const std::string& instrModelFileName);
			AnimatedMeshComponentUPtr CreateAnimatedMeshComponent(Core::StorageLocation ineStorageLocation, const std::string& instrModelFileName, const MaterialCSPtr& inpMaterial);
			AnimatedMeshComponentUPtr CreateAnimatedMeshComponent(Core::StorageLocation ineModelStorageLocation, const std::string& instrModelFileName, Core::StorageLocation ineMaterialStorageLocation, const std::string& instrMaterialFileName);
			
			//---------------------------------------------------------------------------
			/// Create Camera Component
			///
			/// A number of overloads
			//---------------------------------------------------------------------------
			CameraComponentUPtr CreateCameraComponent(const f32 infFOV = 45.0f, const f32 infNear = 1.0f, const f32 infFar = 1000.0f, bool inbIsOrthographic = false);
			
			//---------------------------------------------------------------------------
			/// Create Light Component
			///
			/// A number of overloads
			//---------------------------------------------------------------------------
            AmbientLightComponentUPtr CreateAmbientLightComponent() const;
			DirectionalLightComponentUPtr CreateDirectionalLightComponent(u32 inudwShadowMapRes = 0) const;
            PointLightComponentUPtr CreatePointLightComponent() const;

		protected:
			
			RenderSystem* mpRenderSystem;
            
			TextureManager* mpTextureManager;
			Core::ResourcePool* m_resourcePool;
			RenderCapabilities* mpRenderCapabilities;
		};
	}
}

#endif
