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
		class RenderComponentFactory : public Core::IComponentFactory
		{
		public: 
			DECLARE_NAMED_INTERFACE(RenderComponentFactory);
			RenderComponentFactory(RenderSystem* inpRenderSystem);
			virtual ~RenderComponentFactory(){}
            //--------------------------------------------------------
            /// Is A
            ///
            /// @return If the object is of the given interface type
            //--------------------------------------------------------
            bool IsA(Core::InterfaceIDType inInterfaceID) const;
            //--------------------------------------------------------
            /// Can Produce Component With Interface
            ///
            /// Used to determine if this factory can produce 
            /// component of given type.
            ///
            /// @param The ID of the component to create
            /// @return Whether the object can create component of ID
            //--------------------------------------------------------
            bool CanProduceComponentWithInterface(Core::InterfaceIDType inTypeID) const;
            //--------------------------------------------------------
            /// Can Produce Component With Type Name
            ///
            /// Used to determine if this factory can produce 
            /// component of given type.
            ///
            /// @param The name of the component to create
            /// @return Whether the object can create component
            //--------------------------------------------------------
            bool CanProduceComponentWithTypeName(const std::string & incName) const;
            //--------------------------------------------------------
            /// Create Component
            ///
            /// Creates a component with the given name from the
            /// given parameters
            ///
            /// @param Type Name
            /// @param Param Dictionary
            //--------------------------------------------------------
            Core::ComponentPtr CreateComponent(const std::string & insTypeName, const Core::ParamDictionary & insParamDictionary);
			//--------------------------------------------------------
			/// Get Owning Render System Pointer
			///
			/// @return The render system which created us
			//--------------------------------------------------------
			RenderSystem* GetOwningRenderSystemPtr();
			
            //---------------------------------------------------------------------------
			/// Create Static Sprite Component
			///
			/// A number of overloads 
			//---------------------------------------------------------------------------
			SpriteComponentSPtr CreateSpriteComponent(const Core::CVector2 &invDims, const MaterialSPtr& inMaterial);
			SpriteComponentSPtr CreateSpriteComponent(const Core::CVector2 &invDims, Core::StorageLocation ineStorageLocation, const std::string& instrMaterialFilePath);
			SpriteComponentSPtr CreateSpriteComponent(const SpriteSheetSPtr& pSpriteSheet, u32 inTpageIndex, const MaterialSPtr& inpMaterial);
			SpriteComponentSPtr CreateSpriteComponent(Core::StorageLocation ineStorageLocation, const std::string& instrSpriteSheet, u32 inTpageIndex, const MaterialSPtr& inpMaterial);
            SpriteComponentSPtr CreateSpriteComponent(Core::StorageLocation ineStorageLocation, const std::string& instrSpriteSheet, const std::string& inTpageID, const MaterialSPtr& inpMaterial);

            //---------------------------------------------------------------------------
			/// Create Static Mesh Component
			///
			/// A number of overloads 
			//---------------------------------------------------------------------------
            StaticMeshComponentSPtr CreateStaticMeshComponent(const MeshSPtr& inpModel);
			StaticMeshComponentSPtr CreateStaticMeshComponent(const MeshSPtr& inpModel, const MaterialSPtr& inpMaterial);
			StaticMeshComponentSPtr CreateStaticMeshComponent(const MeshSPtr& inpModel, Core::StorageLocation ineStorageLocation, const std::string & insMaterialName);
			StaticMeshComponentSPtr CreateStaticMeshComponent(Core::StorageLocation ineStorageLocation, const std::string& instrModelFileName);
			StaticMeshComponentSPtr CreateStaticMeshComponent(Core::StorageLocation ineStorageLocation, const std::string& instrModelFileName, const MaterialSPtr& inpMaterial);
			StaticMeshComponentSPtr CreateStaticMeshComponent(Core::StorageLocation ineModelStorageLocation, const std::string& instrModelFileName, Core::StorageLocation ineMaterialStorageLocation, const std::string& instrMaterialFileName);
            
            //---------------------------------------------------------------------------
			/// Create Animated Mesh Component
			///
			/// A number of overloads 
			//---------------------------------------------------------------------------
            AnimatedMeshComponentSPtr CreateAnimatedMeshComponent(const MeshSPtr& inpModel);
			AnimatedMeshComponentSPtr CreateAnimatedMeshComponent(const MeshSPtr& inpModel, const MaterialSPtr& inpMaterial);
			AnimatedMeshComponentSPtr CreateAnimatedMeshComponent(const MeshSPtr& inpModel, Core::StorageLocation ineStorageLocation, const std::string & insMaterialName);
			AnimatedMeshComponentSPtr CreateAnimatedMeshComponent(Core::StorageLocation ineStorageLocation, const std::string& instrModelFileName);
			AnimatedMeshComponentSPtr CreateAnimatedMeshComponent(Core::StorageLocation ineStorageLocation, const std::string& instrModelFileName, const MaterialSPtr& inpMaterial);
			AnimatedMeshComponentSPtr CreateAnimatedMeshComponent(Core::StorageLocation ineModelStorageLocation, const std::string& instrModelFileName, Core::StorageLocation ineMaterialStorageLocation, const std::string& instrMaterialFileName);
			
			//---------------------------------------------------------------------------
			/// Create Camera Component
			///
			/// A number of overloads
			//---------------------------------------------------------------------------
			CameraComponentSPtr CreateCameraComponent(const f32 infFOV = 45.0f, const f32 infNear = 1.0f, const f32 infFar = 1000.0f, bool inbIsOrthographic = false);
			
			//---------------------------------------------------------------------------
			/// Create Light Component
			///
			/// A number of overloads
			//---------------------------------------------------------------------------
            AmbientLightComponentSPtr CreateAmbientLightComponent() const;
			DirectionalLightComponentSPtr CreateDirectionalLightComponent(u32 inudwShadowMapRes = 0) const;
            PointLightComponentSPtr CreatePointLightComponent() const;

		protected:
			
			RenderSystem* mpRenderSystem;
            
            MeshManager* mpMeshManager;
			MaterialManager* mpMaterialManager;
			TextureManager* mpTextureManager;
			SpriteSheetManager* mpSpriteSheetManager;
			RenderCapabilities* mpRenderCapabilities;
		};
	}
}

#endif
