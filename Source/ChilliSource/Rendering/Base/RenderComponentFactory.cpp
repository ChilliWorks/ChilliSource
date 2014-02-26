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
#include <ChilliSource/Core/Resource/ResourceManagerDispenser.h>
#include <ChilliSource/Core/Base/Screen.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/String/StringParser.h>

namespace ChilliSource
{
	namespace Rendering
	{
		DEFINE_NAMED_INTERFACE(RenderComponentFactory);
        //--------------------------------------------------------
        /// Constructor
        ///
        /// Managers that are required
		//--------------------------------------------------------
        RenderComponentFactory::RenderComponentFactory(RenderSystem* inpRenderSystem) 
        : mpRenderSystem(inpRenderSystem), mpMeshManager(nullptr), mpMaterialManager(nullptr), mpSpriteSheetManager(nullptr), mpTextureManager(nullptr), mpRenderCapabilities(nullptr)
        {
			if(!mpSpriteSheetManager)
            {
                mpSpriteSheetManager = static_cast<SpriteSheetManager*>(Core::CResourceManagerDispenser::GetSingletonPtr()->GetResourceManagerForType(SpriteSheet::InterfaceID));
            }
            if(!mpTextureManager)
            {
                mpTextureManager = static_cast<TextureManager*>(Core::CResourceManagerDispenser::GetSingletonPtr()->GetResourceManagerForType(Texture::InterfaceID));
            }
            if(!mpMaterialManager)
            {
                mpMaterialManager = static_cast<MaterialManager*>(Core::CResourceManagerDispenser::GetSingletonPtr()->GetResourceManagerForType(Material::InterfaceID));
            }
            if(!mpMeshManager)
            {
                mpMeshManager = static_cast<MeshManager*>(Core::CResourceManagerDispenser::GetSingletonPtr()->GetResourceManagerForType(Mesh::InterfaceID));
            }
            if (!mpRenderCapabilities)
            {
            	mpRenderCapabilities = Core::CApplication::GetSystemImplementing<RenderCapabilities>();
            	CS_ASSERT(mpRenderCapabilities, "Render Component Factory is missing required system: Render Capabilities.");
            }
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
        //--------------------------------------------------------
        /// Create Component
        ///
        /// Creates a component with the given name from the
        /// given parameters
        ///
        /// @param Type Name
        /// @param Param Dictionary
        //--------------------------------------------------------
        Core::ComponentPtr RenderComponentFactory::CreateComponent(const std::string & insTypeName, const Core::ParamDictionary & insParamDictionary)
        {
            if (insTypeName == StaticMeshComponent::TypeName) 
            {
				std::string sMeshName;
				if (insParamDictionary.TryGetValue("MeshName", sMeshName))
                {
                    std::string strMeshLocation("Package");
                    insParamDictionary.TryGetValue("MeshLocation", strMeshLocation);
                    
                    std::string strMaterialLocation("Package");
                    insParamDictionary.TryGetValue("MaterialLocation", strMaterialLocation);
                    
					std::string sMaterialName;
					if (insParamDictionary.TryGetValue("MaterialName", sMaterialName)) 
                    {
						return CreateStaticMeshComponent(Core::GetStorageLocationFromString(strMeshLocation), sMeshName, Core::GetStorageLocationFromString(strMaterialLocation), sMaterialName);
					} 
                    else 
                    {
						return CreateStaticMeshComponent(Core::GetStorageLocationFromString(strMeshLocation), sMeshName);
					}
				} 
                else 
                {
					CS_WARNING_LOG("Insufficient parameters to create a StaticMeshComponent");
				}
			} 
            else if (insTypeName == CameraComponent::TypeName) 
            {
				f32 fHorFOV = 45.0f;
				if (insParamDictionary.HasValue("FOV")) 
                {
					fHorFOV = Core::ParseF32(insParamDictionary.ValueForKey("FOV"));
				}
				
				f32 fNearClipZ = 0.1f;
				if (insParamDictionary.HasValue("NearClippingZ")) 
                {
					fNearClipZ = Core::ParseF32(insParamDictionary.ValueForKey("NearClippingZ"));
				}
				
				f32 fFarClipZ = 1000.0f;
				if (insParamDictionary.HasValue("FarClippingZ")) 
                {
					fFarClipZ = Core::ParseF32(insParamDictionary.ValueForKey("FarClippingZ"));
				}
				
				CameraComponentPtr pResult = CreateCameraComponent(fHorFOV, fNearClipZ, fFarClipZ);
				
				if (insParamDictionary.HasValue("ClearColour")) 
                {
					Core::CColour cClearCol = Core::ParseColour(insParamDictionary.ValueForKey("ClearColour"));
					pResult->SetClearColour(cClearCol);
				}
				
				return pResult;
            }
            else if (insTypeName == SpriteComponent::TypeName)
            {
				SpriteComponentPtr pSprite(new SpriteComponent());
				
				std::string strMaterialName;
				if (insParamDictionary.TryGetValue("MaterialName", strMaterialName))
				{
					std::string strSpriteSheet;
					std::string strSpriteSheetIndex;
					if (insParamDictionary.TryGetValue("SpriteSheet", strSpriteSheet) && insParamDictionary.TryGetValue("SpriteSheetIndex", strSpriteSheetIndex))
					{
						SpriteSheetPtr pSpriteSheet = mpSpriteSheetManager->GetSpriteSheetFromFile(Core::StorageLocation::k_package, strSpriteSheet);
						u32 udwIndex = pSpriteSheet->GetFrameIndexByID(strSpriteSheetIndex);
						
						MaterialPtr pMaterial = mpMaterialManager->GetMaterialFromFile(Core::StorageLocation::k_package, strMaterialName);
						pSprite = CreateSpriteComponent(pSpriteSheet, udwIndex, pMaterial);
						
						u32 udwTargetDensity = 1;
						std::string strTargetDensity;
						if(insParamDictionary.TryGetValue("TargetDensity", strTargetDensity))
						{
							udwTargetDensity = Core::ParseS32(strTargetDensity);
						}
						f32 fScale = udwTargetDensity / Core::IFileSystem::GetDeviceResourcesDensity();
						
						pSprite->GetMaterial()->SetTexture(pSpriteSheet->GetTexture());
						pSprite->SetDimensions(pSpriteSheet->GetSizeForFrame(udwIndex) * fScale);
						pSprite->SetUVs(pSpriteSheet->GetUVsForFrame(udwIndex));
					}
					else
					{
						pSprite = CreateSpriteComponent(Core::CVector2::ZERO, Core::StorageLocation::k_package, strMaterialName);
					}
				
					if (insParamDictionary.HasValue("Transparent"))
					{
						pSprite->GetMaterial()->SetTransparent(true);
					}
				}
				
				std::string strSize;
				if (insParamDictionary.TryGetValue("Size", strSize))
				{
					pSprite->SetDimensions(Core::ParseVector2(strSize));					
				}
				
				std::string strColour;
				if (insParamDictionary.TryGetValue("Colour", strColour))
				{
					pSprite->SetColour(Core::ParseColour(strColour));
				}		
				
				std::string strOriginAlignment;
				if (insParamDictionary.TryGetValue("LocalAlignment", strOriginAlignment))
				{
					pSprite->SetOriginAlignment(Core::AlignmentAnchorFromString(strOriginAlignment));
				}
												
				return pSprite;
			}
			
			return Core::ComponentPtr();				
        }
		//--------------------------------------------------------
		/// Get Owning Render System Pointer
		///
		/// @return The render system which created us
		//--------------------------------------------------------
		RenderSystem* RenderComponentFactory::GetOwningRenderSystemPtr()
		{
			return mpRenderSystem;
		}
        //---------------------------------------------------------------------------
        /// Create Sprite Component
        ///
        /// @param Vector containing width and height of sprite
        /// @param The storage location to load from
        /// @param Material file
        /// @return an instantiated sprite object
        //---------------------------------------------------------------------------
        SpriteComponentPtr RenderComponentFactory::CreateSpriteComponent(const Core::CVector2 &invDims, Core::StorageLocation ineStorageLocation, const std::string& instrMaterialFilePath)
        {
            SpriteComponentPtr pSprite(new SpriteComponent());
            pSprite->SetMaterial(mpMaterialManager->GetMaterialFromFile(ineStorageLocation, instrMaterialFilePath));
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
        SpriteComponentPtr RenderComponentFactory::CreateSpriteComponent(const Core::CVector2 &invDims, const MaterialPtr& inpMaterial)
        {
            SpriteComponentPtr pSprite(new SpriteComponent());
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
        SpriteComponentPtr RenderComponentFactory::CreateSpriteComponent(const SpriteSheetPtr& pSpriteSheet, u32 inTpageIndex, const MaterialPtr& inpMaterial)
        {
            SpriteComponentPtr pSprite(new SpriteComponent());
            pSprite->SetMaterial(inpMaterial);
            pSprite->SetDimensions(pSpriteSheet->GetSizeForFrame(inTpageIndex));
            pSprite->SetUVs(pSpriteSheet->GetUVsForFrame(inTpageIndex));
            
            return pSprite;
        }
        //---------------------------------------------------------------------------
        /// Create Sprite Component
        ///
        /// @param The storage location to load from
        /// @param Sprite sheet file
        /// @param Index into data tpage
        /// @param Material
        /// @return an instantiated sprite object
        //---------------------------------------------------------------------------
        SpriteComponentPtr RenderComponentFactory::CreateSpriteComponent(Core::StorageLocation ineStorageLocation, const std::string& instrSpriteSheetTexture, u32 inTpageIndex, const MaterialPtr& inpMaterial)
        {
            SpriteComponentPtr pSprite(new SpriteComponent());
            pSprite->SetMaterial(inpMaterial);
            SpriteSheetPtr pSpriteSheet = mpSpriteSheetManager->GetSpriteSheetFromFile(ineStorageLocation, instrSpriteSheetTexture);
            pSprite->SetDimensions(pSpriteSheet->GetSizeForFrame(inTpageIndex));
            pSprite->SetUVs(pSpriteSheet->GetUVsForFrame(inTpageIndex));
            
            return pSprite;
        }//---------------------------------------------------------------------------
        /// Create Sprite Component
        ///
        /// @param The storage location to load from
        /// @param Sprite sheet file
        /// @param String id for tpage index
        /// @param Material
        /// @return an instantiated sprite object
        //---------------------------------------------------------------------------
        SpriteComponentPtr RenderComponentFactory::CreateSpriteComponent(Core::StorageLocation ineStorageLocation, const std::string& instrSpriteSheetTexture, const std::string& instrTpageID, const MaterialPtr& inpMaterial)
        {
            SpriteComponentPtr pSprite(new SpriteComponent());
            pSprite->SetMaterial(inpMaterial);
            SpriteSheetPtr pSpriteSheet = mpSpriteSheetManager->GetSpriteSheetFromFile(ineStorageLocation, instrSpriteSheetTexture);
            u32 udwTpageID = pSpriteSheet->GetFrameIndexByID(instrTpageID);
            pSprite->SetDimensions(pSpriteSheet->GetSizeForFrame(udwTpageID));
            pSprite->SetUVs(pSpriteSheet->GetUVsForFrame(udwTpageID));
            
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
		StaticMeshComponentPtr RenderComponentFactory::CreateStaticMeshComponent(const MeshPtr& inpModel)
		{
			StaticMeshComponentPtr pResult(new StaticMeshComponent());
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
		StaticMeshComponentPtr RenderComponentFactory::CreateStaticMeshComponent(const MeshPtr& inpModel, const MaterialPtr& inpMaterial)
		{
			StaticMeshComponentPtr pResult(new StaticMeshComponent());
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
		StaticMeshComponentPtr RenderComponentFactory::CreateStaticMeshComponent(const MeshPtr& inpModel, Core::StorageLocation ineStorageLocation, const std::string & insMaterialName)
		{
			MaterialPtr pMaterial = mpMaterialManager->GetMaterialFromFile(ineStorageLocation, insMaterialName);
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
		StaticMeshComponentPtr RenderComponentFactory::CreateStaticMeshComponent(Core::StorageLocation ineStorageLocation, const std::string& instrModelFilePath)
		{
            MeshPtr pModel = mpMeshManager->GetModelFromFile(ineStorageLocation, instrModelFilePath);
            StaticMeshComponentPtr pResult(new StaticMeshComponent());
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
		StaticMeshComponentPtr RenderComponentFactory::CreateStaticMeshComponent(Core::StorageLocation ineStorageLocation, const std::string& instrModelFileName, const MaterialPtr& inpMaterial)
        {
			MeshPtr pModel = mpMeshManager->GetModelFromFile(ineStorageLocation, instrModelFileName);
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
		StaticMeshComponentPtr RenderComponentFactory::CreateStaticMeshComponent(Core::StorageLocation ineModelStorageLocation, const std::string& instrModelFilePath, Core::StorageLocation ineMaterialStorageLocation, const std::string& instrMaterialFilePath)
		{
			MeshPtr pModel = mpMeshManager->GetModelFromFile(ineModelStorageLocation, instrModelFilePath);
			MaterialPtr pMaterial = mpMaterialManager->GetMaterialFromFile(ineMaterialStorageLocation, instrMaterialFilePath);
			
			if (pModel && pMaterial)
			{
				return CreateStaticMeshComponent(pModel, pMaterial);
			}
			else
			{
				return StaticMeshComponentPtr();
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
		AnimatedMeshComponentPtr RenderComponentFactory::CreateAnimatedMeshComponent(const MeshPtr& inpModel)
		{
			AnimatedMeshComponentPtr pResult(new AnimatedMeshComponent());
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
		AnimatedMeshComponentPtr RenderComponentFactory::CreateAnimatedMeshComponent(const MeshPtr& inpModel, const MaterialPtr& inpMaterial)
		{
			AnimatedMeshComponentPtr pResult(new AnimatedMeshComponent());
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
		AnimatedMeshComponentPtr RenderComponentFactory::CreateAnimatedMeshComponent(const MeshPtr& inpModel, Core::StorageLocation ineStorageLocation, const std::string & insMaterialName)
		{
			MaterialPtr pMaterial = mpMaterialManager->GetMaterialFromFile(ineStorageLocation, insMaterialName);
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
		AnimatedMeshComponentPtr RenderComponentFactory::CreateAnimatedMeshComponent(Core::StorageLocation ineStorageLocation, const std::string& instrModelFilePath)
		{
            MeshPtr pModel = mpMeshManager->GetModelFromFile(ineStorageLocation, instrModelFilePath);
            AnimatedMeshComponentPtr pResult(new AnimatedMeshComponent());
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
		AnimatedMeshComponentPtr RenderComponentFactory::CreateAnimatedMeshComponent(Core::StorageLocation ineStorageLocation, const std::string& instrModelFileName, const MaterialPtr& inpMaterial)
        {
			MeshPtr pModel = mpMeshManager->GetModelFromFile(ineStorageLocation, instrModelFileName);
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
		AnimatedMeshComponentPtr RenderComponentFactory::CreateAnimatedMeshComponent(Core::StorageLocation ineModelStorageLocation, const std::string& instrModelFilePath, Core::StorageLocation ineMaterialStorageLocation, const std::string& instrMaterialFilePath)
		{
			MeshPtr pModel = mpMeshManager->GetModelFromFile(ineModelStorageLocation, instrModelFilePath);
			MaterialPtr pMaterial = mpMaterialManager->GetMaterialFromFile(ineMaterialStorageLocation, instrMaterialFilePath);
			
			if (pModel && pMaterial)
			{
				return CreateAnimatedMeshComponent(pModel, pMaterial);
			}
			else
			{
				return AnimatedMeshComponentPtr();
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
		CameraComponentPtr RenderComponentFactory::CreateCameraComponent(const f32 infFOV, const f32 infNear, const f32 infFar, bool inbIsOrthographic)
		{
			CameraDescription desc;
			desc.vViewSize = Core::CScreen::GetOrientedDimensions();
			desc.fAspect = (desc.vViewSize.x/desc.vViewSize.y);
			desc.fFOV = infFOV/desc.fAspect;
			desc.fNearClipping = infNear;
			desc.fFarClipping = infFar;
			desc.ClearCol = Core::CColour::WHITE;
			desc.IsOrthographic = inbIsOrthographic;
			desc.bShouldResizeToScreen = true;
			desc.bShouldRotateToScreen = true;
			
			CameraComponentPtr pCamera(new CameraComponent(desc));
			pCamera->SetViewportOrientation(Core::CScreen::GetOrientation());
			return pCamera;
		}
        //---------------------------------------------------------------------------
		/// Create Ambient Light Component
        ///
        /// @return Light component
		//---------------------------------------------------------------------------
        AmbientLightComponentPtr RenderComponentFactory::CreateAmbientLightComponent() const
        {
            AmbientLightComponentPtr pLight(new AmbientLightComponent());
            return pLight;
        }
		//---------------------------------------------------------------------------
		/// Create Directional Light Component
        ///
        /// @param Shadow map resolution (Zero for no shadows)
        /// @return Light component
		//---------------------------------------------------------------------------
		DirectionalLightComponentPtr RenderComponentFactory::CreateDirectionalLightComponent(u32 inudwShadowMapRes) const
		{
            TextureSPtr pShadowMap;
            TextureSPtr pShadowMapDebug;
            
            if(mpRenderCapabilities->IsShadowMappingSupported() == true && inudwShadowMapRes > 0)
            {
                pShadowMap = mpTextureManager->CreateTextureResource();
                mpTextureManager->CreateEmptyTexture(inudwShadowMapRes, inudwShadowMapRes, Core::CImage::Format::k_Depth16, pShadowMap);

#ifdef MOFLOW_SHADOW_DEBUG
                pShadowMapDebug = mpTextureManager->CreateTextureResource();
                mpTextureManager->CreateEmptyTexture(inudwShadowMapRes, inudwShadowMapRes, Core::CImage::Format::k_RGB888, pShadowMapDebug);
#endif
            }
            
            DirectionalLightComponentPtr pLight(new DirectionalLightComponent(pShadowMap, pShadowMapDebug));
			return pLight;
		}
        //---------------------------------------------------------------------------
		/// Create Point Light Component
        ///
        /// @return Light component
		//---------------------------------------------------------------------------
        PointLightComponentPtr RenderComponentFactory::CreatePointLightComponent() const
        {
            PointLightComponentPtr pLight(new PointLightComponent());
            return pLight;
        }
	}
}
