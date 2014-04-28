/*
 *  RenderComponentFactory.cpp
 *  moFloTest
 *
 *  Created by Scott Downie on 30/09/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */


#include <ChilliSource/Rendering/Base/RenderComponentFactory.h>
#include <ChilliSource/Rendering/Base/RenderSystem.h>
#include <ChilliSource/Rendering/Texture/Texture.h>
#include <ChilliSource/Rendering/Texture/TextureAtlas.h>

#include <ChilliSource/Rendering/Camera/CameraComponent.h>
#include <ChilliSource/Rendering/Model/StaticMeshComponent.h>
#include <ChilliSource/Rendering/Model/AnimatedMeshComponent.h>
#include <ChilliSource/Rendering/Sprite/SpriteComponent.h>
#include <ChilliSource/Rendering/Lighting/LightComponent.h>
#include <ChilliSource/Rendering/Lighting/AmbientLightComponent.h>
#include <ChilliSource/Rendering/Lighting/DirectionalLightComponent.h>
#include <ChilliSource/Rendering/Lighting/PointLightComponent.h>
#include <ChilliSource/Rendering/Base/RenderCapabilities.h>

#include <ChilliSource/Rendering/Base/AlignmentAnchors.h>
#include <ChilliSource/Core/Container/ParamDictionary.h>
#include <ChilliSource/Core/Resource/ResourcePool.h>
#include <ChilliSource/Core/Image/ImageCompression.h>
#include <ChilliSource/Core/Image/ImageFormat.h>
#include <ChilliSource/Core/Base/Screen.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/String/StringParser.h>

namespace ChilliSource
{
	namespace Rendering
	{
		CS_DEFINE_NAMEDTYPE(RenderComponentFactory);
        //--------------------------------------------------------
        /// Constructor
        ///
        /// Managers that are required
		//--------------------------------------------------------
        RenderComponentFactory::RenderComponentFactory(RenderSystem* inpRenderSystem) 
        : mpRenderSystem(inpRenderSystem)
        {
            m_resourcePool = Core::Application::Get()->GetResourcePool();
            CS_ASSERT(m_resourcePool, "Render component factory is missing required system: Resource Pool");
            
            mpRenderCapabilities = Core::Application::Get()->GetSystem<RenderCapabilities>();
            CS_ASSERT(mpRenderCapabilities, "Render Component Factory is missing required system: Render Capabilities.");
            
            m_screen = Core::Application::Get()->GetSystem<Core::Screen>();
            CS_ASSERT(m_screen, "Render Component Factory is missing required system: Screen.");
        }
        //--------------------------------------------------------
        /// Is A
        ///
        /// @return If the object is of the given interface type
        //--------------------------------------------------------
		bool RenderComponentFactory::IsA(Core::InterfaceIDType inInterfaceID) const
        {
			return (inInterfaceID == RenderComponentFactory::InterfaceID);
		}
        //--------------------------------------------------------
		/// Can Produce Component With Interface
		///
		/// Used to determine if this factory can produce 
		/// component of given type.
		///
		/// @param The ID of the component to create
		/// @return Whether the object can create component of ID
		//--------------------------------------------------------
		bool RenderComponentFactory::CanProduceComponentWithInterface(Core::InterfaceIDType inTypeID) const
		{
			return
            (SpriteComponent::InterfaceID == inTypeID)	||
			(CameraComponent::InterfaceID == inTypeID)	|| 
			(StaticMeshComponent::InterfaceID == inTypeID) || 
			(LightComponent::InterfaceID == inTypeID) ||
			(DirectionalLightComponent::InterfaceID == inTypeID);
		}
        //--------------------------------------------------------
		/// Can Produce Component With Type Name
		///
		/// Used to determine if this factory can produce 
		/// component of given type.
		///
		/// @param The name of the component to create
		/// @return Whether the object can create component
		//--------------------------------------------------------
		bool RenderComponentFactory::CanProduceComponentWithTypeName(const std::string & incName) const
        {			
			return
            (SpriteComponent::TypeName == incName)	||
            (CameraComponent::TypeName == incName)	|| 
            (StaticMeshComponent::TypeName == incName) || 
            (LightComponent::TypeName == incName) ||
            (DirectionalLightComponent::TypeName == incName);
		}
        //---------------------------------------------------------------------------
        /// Create Sprite Component
        ///
        /// @param Vector containing width and height of sprite
        /// @param The storage location to load from
        /// @param Material file
        /// @return an instantiated sprite object
        //---------------------------------------------------------------------------
        SpriteComponentUPtr RenderComponentFactory::CreateSpriteComponent(const Core::Vector2 &invDims, Core::StorageLocation ineStorageLocation, const std::string& instrMaterialFilePath)
        {
            SpriteComponentUPtr pSprite(new SpriteComponent());
            MaterialCSPtr pMaterial = m_resourcePool->LoadResource<Material>(ineStorageLocation, instrMaterialFilePath);
            pSprite->SetMaterial(pMaterial);
            pSprite->SetDimensions(invDims);
            return pSprite;
        }
        //---------------------------------------------------------------------------
        /// Create Sprite Component
        ///
        /// @param Vector containing width and height of sprite
        /// @param Material
        /// @return an instantiated sprite object
        //---------------------------------------------------------------------------
        SpriteComponentUPtr RenderComponentFactory::CreateSpriteComponent(const Core::Vector2 &invDims, const MaterialCSPtr& inpMaterial)
        {
            SpriteComponentUPtr pSprite(new SpriteComponent());
            pSprite->SetDimensions(invDims);
            pSprite->SetMaterial(inpMaterial);
            return pSprite;
        }
        //---------------------------------------------------------------------------
        /// Create Sprite Component
        ///
        /// @param Sprite sheet
        /// @param Sprite sheet indes
        /// @param Material
        /// @return an instantiated sprite object
        //---------------------------------------------------------------------------
        SpriteComponentUPtr RenderComponentFactory::CreateSpriteComponent(const TextureAtlasCSPtr& pTextureAtlas, const std::string& in_textureId, const MaterialCSPtr& inpMaterial)
        {
            SpriteComponentUPtr pSprite(new SpriteComponent());
            pSprite->SetMaterial(inpMaterial);
            pSprite->SetDimensions(pTextureAtlas->GetFrameSize(in_textureId));
            pSprite->SetUVs(pTextureAtlas->GetFrameUVs(in_textureId));
            
            return pSprite;
        }
        //---------------------------------------------------------------------------
        /// Create Sprite Component
        ///
        /// @param The storage location to load from
        /// @param Sprite sheet file
        /// @param String id for tpage index
        /// @param Material
        /// @return an instantiated sprite object
        //---------------------------------------------------------------------------
        SpriteComponentUPtr RenderComponentFactory::CreateSpriteComponent(Core::StorageLocation ineStorageLocation, const std::string& instrTextureAtlas, const std::string& instrTpageID, const MaterialCSPtr& inpMaterial)
        {
            SpriteComponentUPtr pSprite(new SpriteComponent());
            pSprite->SetMaterial(inpMaterial);
            TextureAtlasCSPtr pTextureAtlas = m_resourcePool->LoadResource<TextureAtlas>(ineStorageLocation, instrTextureAtlas);
            pSprite->SetDimensions(pTextureAtlas->GetFrameSize(instrTpageID));
            pSprite->SetUVs(pTextureAtlas->GetFrameUVs(instrTpageID));
            
            return pSprite;
        }
        //---------------------------------------------------------------------------
        /// Create Static Mesh Component
        ///
        /// Uses the material supplied with the mesh
        ///
		/// @param Mesh 
        /// @return Static mesh component
        //---------------------------------------------------------------------------
		StaticMeshComponentUPtr RenderComponentFactory::CreateStaticMeshComponent(const MeshCSPtr& inpModel)
		{
			StaticMeshComponentUPtr pResult(new StaticMeshComponent());
			pResult->AttachMesh(inpModel);
			return pResult;
		}
        //---------------------------------------------------------------------------
        /// Create Static Mesh Component
        ///
		/// @param Mesh 
        /// @param Material 
        /// @return Static mesh component
        //---------------------------------------------------------------------------
		StaticMeshComponentUPtr RenderComponentFactory::CreateStaticMeshComponent(const MeshCSPtr& inpModel, const MaterialCSPtr& inpMaterial)
		{
			StaticMeshComponentUPtr pResult(new StaticMeshComponent());
			pResult->AttachMesh(inpModel, inpMaterial);
			return pResult;
		}
        //---------------------------------------------------------------------------
        /// Create Static Mesh Component
        ///
		/// @param Mesh 
        /// @param Material location
        /// @param Material file
        /// @return Static mesh component
        //---------------------------------------------------------------------------
		StaticMeshComponentUPtr RenderComponentFactory::CreateStaticMeshComponent(const MeshCSPtr& inpModel, Core::StorageLocation ineStorageLocation, const std::string & insMaterialName)
		{
			MaterialCSPtr pMaterial = m_resourcePool->LoadResource<Material>(ineStorageLocation, insMaterialName);
			return CreateStaticMeshComponent(inpModel, pMaterial);
		}
        //---------------------------------------------------------------------------
        /// Create Static Mesh Component
        ///
        /// Loads the material from the model file
        ///
		/// @param Mesh location 
        /// @param Mesh file
        /// @return Static mesh component
        //---------------------------------------------------------------------------
		StaticMeshComponentUPtr RenderComponentFactory::CreateStaticMeshComponent(Core::StorageLocation ineStorageLocation, const std::string& instrModelFilePath)
		{
            MeshCSPtr pModel = Core::Application::Get()->GetResourcePool()->LoadResource<Mesh>(ineStorageLocation, instrModelFilePath);
            StaticMeshComponentUPtr pResult(new StaticMeshComponent());
			pResult->AttachMesh(pModel);
			return pResult;
		}
        //---------------------------------------------------------------------------
        /// Create Static Mesh Component
        ///
		/// @param Mesh location 
        /// @param Mesh file
        /// @param Material
        /// @return Static mesh component
        //---------------------------------------------------------------------------
		StaticMeshComponentUPtr RenderComponentFactory::CreateStaticMeshComponent(Core::StorageLocation ineStorageLocation, const std::string& instrModelFilePath, const MaterialCSPtr& inpMaterial)
        {
			MeshCSPtr pModel = Core::Application::Get()->GetResourcePool()->LoadResource<Mesh>(ineStorageLocation, instrModelFilePath);
			return CreateStaticMeshComponent(pModel, inpMaterial);
		}
        //---------------------------------------------------------------------------
        /// Create Static Mesh Component
        ///
		/// @param Mesh location 
        /// @param Mesh file
        /// @param Material location
        /// @param Material file
        /// @return Static mesh component
        //---------------------------------------------------------------------------
		StaticMeshComponentUPtr RenderComponentFactory::CreateStaticMeshComponent(Core::StorageLocation ineModelStorageLocation, const std::string& instrModelFilePath, Core::StorageLocation ineMaterialStorageLocation, const std::string& instrMaterialFilePath)
		{
            MeshCSPtr pModel = Core::Application::Get()->GetResourcePool()->LoadResource<Mesh>(ineModelStorageLocation, instrModelFilePath);
            MaterialCSPtr pMaterial = m_resourcePool->LoadResource<Material>(ineMaterialStorageLocation, instrMaterialFilePath);
			
			if (pModel && pMaterial)
			{
				return CreateStaticMeshComponent(pModel, pMaterial);
			}
			else
			{
				return nullptr;
			}
		}
        //---------------------------------------------------------------------------
        /// Create Animated Mesh Component
        ///
        /// Uses the material supplied with the mesh
        ///
		/// @param Mesh 
        /// @return Animated mesh component
        //---------------------------------------------------------------------------
		AnimatedMeshComponentUPtr RenderComponentFactory::CreateAnimatedMeshComponent(const MeshCSPtr& inpModel)
		{
			AnimatedMeshComponentUPtr pResult(new AnimatedMeshComponent());
			pResult->AttachMesh(inpModel);
			return pResult;
		}
        //---------------------------------------------------------------------------
        /// Create Animated Mesh Component
        ///
		/// @param Mesh 
        /// @param Material 
        /// @return Animated mesh component
        //---------------------------------------------------------------------------
		AnimatedMeshComponentUPtr RenderComponentFactory::CreateAnimatedMeshComponent(const MeshCSPtr& inpModel, const MaterialCSPtr& inpMaterial)
		{
			AnimatedMeshComponentUPtr pResult(new AnimatedMeshComponent());
			pResult->AttachMesh(inpModel, inpMaterial);
			return pResult;
		}
        //---------------------------------------------------------------------------
        /// Create Animated Mesh Component
        ///
		/// @param Mesh 
        /// @param Material location
        /// @param Material file
        /// @return Animated mesh component
        //---------------------------------------------------------------------------
		AnimatedMeshComponentUPtr RenderComponentFactory::CreateAnimatedMeshComponent(const MeshCSPtr& inpModel, Core::StorageLocation ineStorageLocation, const std::string & insMaterialName)
		{
            MaterialCSPtr pMaterial = m_resourcePool->LoadResource<Material>(ineStorageLocation, insMaterialName);
			return CreateAnimatedMeshComponent(inpModel, pMaterial);
		}
        //---------------------------------------------------------------------------
        /// Create Animated Mesh Component
        ///
        /// Loads the material from the model file
        ///
		/// @param Mesh location 
        /// @param Mesh file
        /// @return Animated mesh component
        //---------------------------------------------------------------------------
		AnimatedMeshComponentUPtr RenderComponentFactory::CreateAnimatedMeshComponent(Core::StorageLocation ineStorageLocation, const std::string& instrModelFilePath)
		{
            MeshCSPtr pModel = Core::Application::Get()->GetResourcePool()->LoadResource<Mesh>(ineStorageLocation, instrModelFilePath);
            AnimatedMeshComponentUPtr pResult(new AnimatedMeshComponent());
			pResult->AttachMesh(pModel);
			return pResult;
		}
        //---------------------------------------------------------------------------
        /// Create Animated Mesh Component
        ///
		/// @param Mesh location 
        /// @param Mesh file
        /// @param Material
        /// @return Animated mesh component
        //---------------------------------------------------------------------------
		AnimatedMeshComponentUPtr RenderComponentFactory::CreateAnimatedMeshComponent(Core::StorageLocation ineStorageLocation, const std::string& instrModelFilePath, const MaterialCSPtr& inpMaterial)
        {
            MeshCSPtr pModel = Core::Application::Get()->GetResourcePool()->LoadResource<Mesh>(ineStorageLocation, instrModelFilePath);
			return CreateAnimatedMeshComponent(pModel, inpMaterial);
		}
        //---------------------------------------------------------------------------
        /// Create Animated Mesh Component
        ///
		/// @param Mesh location 
        /// @param Mesh file
        /// @param Material location
        /// @param Material file
        /// @return Animated mesh component
        //---------------------------------------------------------------------------
		AnimatedMeshComponentUPtr RenderComponentFactory::CreateAnimatedMeshComponent(Core::StorageLocation ineModelStorageLocation, const std::string& instrModelFilePath, Core::StorageLocation ineMaterialStorageLocation, const std::string& instrMaterialFilePath)
		{
            MeshCSPtr pModel = Core::Application::Get()->GetResourcePool()->LoadResource<Mesh>(ineModelStorageLocation, instrModelFilePath);
            MaterialCSPtr pMaterial = m_resourcePool->LoadResource<Material>(ineMaterialStorageLocation, instrMaterialFilePath);
			
			if (pModel && pMaterial)
			{
				return CreateAnimatedMeshComponent(pModel, pMaterial);
			}
			else
			{
				return nullptr;
			}
		}
		//---------------------------------------------------------------------------
		/// Create Camera Component
		///
		/// @param Field of view
		/// @param Near clipping plane
		/// @param Far clipping plane
		/// @return an instantiated camera object
		//---------------------------------------------------------------------------
		CameraComponentUPtr RenderComponentFactory::CreateCameraComponent(const f32 infFOV, const f32 infNear, const f32 infFar, bool inbIsOrthographic)
		{
			CameraDescription desc;
			desc.vViewSize = m_screen->GetResolution();
			desc.fAspect = (desc.vViewSize.x/desc.vViewSize.y);
			desc.fFOV = infFOV;
			desc.fNearClipping = infNear;
			desc.fFarClipping = infFar;
			desc.ClearCol = Core::Colour::k_white;
			desc.IsOrthographic = inbIsOrthographic;
			desc.bShouldResizeToScreen = true;
			
			CameraComponentUPtr pCamera(new CameraComponent(desc));
			return pCamera;
		}
        //---------------------------------------------------------------------------
		/// Create Ambient Light Component
        ///
        /// @return Light component
		//---------------------------------------------------------------------------
        AmbientLightComponentUPtr RenderComponentFactory::CreateAmbientLightComponent() const
        {
            AmbientLightComponentUPtr pLight(new AmbientLightComponent());
            return pLight;
        }
		//---------------------------------------------------------------------------
		/// Create Directional Light Component
        ///
        /// @param Shadow map resolution (Zero for no shadows)
        /// @return Light component
		//---------------------------------------------------------------------------
		DirectionalLightComponentUPtr RenderComponentFactory::CreateDirectionalLightComponent(u32 inudwShadowMapRes) const
		{
            static u32 s_shadowMapCount = 0;
            
            TextureSPtr pShadowMap;
            TextureSPtr pShadowMapDebug;
            
            if(mpRenderCapabilities->IsShadowMappingSupported() == true && inudwShadowMapRes > 0)
            {
                Core::ResourcePool* resourcePool = Core::Application::Get()->GetResourcePool();
                
                pShadowMap = resourcePool->CreateResource<Rendering::Texture>("_ShadowMap" + Core::ToString(s_shadowMapCount));
                Texture::Descriptor desc;
                desc.m_width = inudwShadowMapRes;
                desc.m_height = inudwShadowMapRes;
                desc.m_format = Core::ImageFormat::k_Depth16;
                desc.m_compression = Core::ImageCompression::k_none;
                desc.m_dataSize = 0;
                pShadowMap->Build(desc, nullptr);
        
#ifdef CS_ENABLE_DEBUGSHADOW
                pShadowMapDebug = resourcePool->CreateResource<Rendering::Texture>("_ShadowMapDebug" + Core::ToString(s_shadowMapCount));
                desc.m_width = inudwShadowMapRes;
                desc.m_height = inudwShadowMapRes;
                desc.m_format = Core::ImageFormat::k_RGB888;
                desc.m_compression = Core::ImageCompression::k_none;
                desc.m_dataSize = 0;
                pShadowMapDebug->Build(desc, nullptr);
#endif
                
                s_shadowMapCount++;
            }
            
            DirectionalLightComponentUPtr pLight(new DirectionalLightComponent(pShadowMap, pShadowMapDebug));
			return pLight;
		}
        //---------------------------------------------------------------------------
		/// Create Point Light Component
        ///
        /// @return Light component
		//---------------------------------------------------------------------------
        PointLightComponentUPtr RenderComponentFactory::CreatePointLightComponent() const
        {
            PointLightComponentUPtr pLight(new PointLightComponent());
            return pLight;
        }
	}
}
