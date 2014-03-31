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
#include <ChilliSource/Rendering/Texture/TextureManager.h>

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
		CS_DEFINE_NAMEDTYPE(RenderComponentFactory);
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
                mpSpriteSheetManager = static_cast<SpriteSheetManager*>(Core::ResourceManagerDispenser::GetSingletonPtr()->GetResourceManagerForType(SpriteSheet::InterfaceID));
            }
            if(!mpTextureManager)
            {
                mpTextureManager = static_cast<TextureManager*>(Core::ResourceManagerDispenser::GetSingletonPtr()->GetResourceManagerForType(Texture::InterfaceID));
            }
            if(!mpMaterialManager)
            {
                mpMaterialManager = static_cast<MaterialManager*>(Core::ResourceManagerDispenser::GetSingletonPtr()->GetResourceManagerForType(Material::InterfaceID));
            }
            if(!mpMeshManager)
            {
                mpMeshManager = static_cast<MeshManager*>(Core::ResourceManagerDispenser::GetSingletonPtr()->GetResourceManagerForType(Mesh::InterfaceID));
            }
            if (!mpRenderCapabilities)
            {
            	mpRenderCapabilities = Core::Application::Get()->GetSystem_Old<RenderCapabilities>();
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
        Core::ComponentUPtr RenderComponentFactory::CreateComponent(const std::string & insTypeName, const Core::ParamDictionary & insParamDictionary)
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
						return CreateStaticMeshComponent(Core::ParseStorageLocation(strMeshLocation), sMeshName, Core::ParseStorageLocation(strMaterialLocation), sMaterialName);
					} 
                    else 
                    {
						return CreateStaticMeshComponent(Core::ParseStorageLocation(strMeshLocation), sMeshName);
					}
				} 
                else 
                {
					CS_LOG_WARNING("Insufficient parameters to create a StaticMeshComponent");
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
				
				CameraComponentUPtr pResult = CreateCameraComponent(fHorFOV, fNearClipZ, fFarClipZ);
				
				if (insParamDictionary.HasValue("ClearColour")) 
                {
					Core::Colour cClearCol = Core::ParseColour(insParamDictionary.ValueForKey("ClearColour"));
					pResult->SetClearColour(cClearCol);
				}
				
				return Core::ComponentUPtr(std::move(pResult));
            }
            else if (insTypeName == SpriteComponent::TypeName)
            {
				SpriteComponentUPtr pSprite(new SpriteComponent());
				
				std::string strMaterialName;
				if (insParamDictionary.TryGetValue("MaterialName", strMaterialName))
				{
					std::string strSpriteSheet;
					std::string strSpriteSheetIndex;
					if (insParamDictionary.TryGetValue("SpriteSheet", strSpriteSheet) && insParamDictionary.TryGetValue("SpriteSheetIndex", strSpriteSheetIndex))
					{
						SpriteSheetSPtr pSpriteSheet = std::static_pointer_cast<SpriteSheet>(mpSpriteSheetManager->GetResourceFromFile(Core::StorageLocation::k_package, strSpriteSheet));
						u32 udwIndex = pSpriteSheet->GetFrameIndexByID(strSpriteSheetIndex);
						
						MaterialSPtr pMaterial = mpMaterialManager->GetMaterialFromFile(Core::StorageLocation::k_package, strMaterialName);
						pSprite = CreateSpriteComponent(pSpriteSheet, udwIndex, pMaterial);
						
						u32 udwTargetDensity = 1;
						std::string strTargetDensity;
						if(insParamDictionary.TryGetValue("TargetDensity", strTargetDensity))
						{
							udwTargetDensity = Core::ParseS32(strTargetDensity);
						}
						f32 fScale = udwTargetDensity / Core::FileSystem::GetDeviceResourcesDensity();
						
						pSprite->GetMaterial()->SetTexture(pSpriteSheet->GetTexture());
						pSprite->SetDimensions(pSpriteSheet->GetSizeForFrame(udwIndex) * fScale);
						pSprite->SetUVs(pSpriteSheet->GetUVsForFrame(udwIndex));
					}
					else
					{
						pSprite = CreateSpriteComponent(Core::Vector2::ZERO, Core::StorageLocation::k_package, strMaterialName);
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
					pSprite->SetOriginAlignment(AlignmentAnchorFromString(strOriginAlignment));
				}
												
				return Core::ComponentUPtr(std::move(pSprite));
			}
			
			return nullptr;
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
        SpriteComponentUPtr RenderComponentFactory::CreateSpriteComponent(const Core::Vector2 &invDims, Core::StorageLocation ineStorageLocation, const std::string& instrMaterialFilePath)
        {
            SpriteComponentUPtr pSprite(new SpriteComponent());
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
        SpriteComponentUPtr RenderComponentFactory::CreateSpriteComponent(const Core::Vector2 &invDims, const MaterialSPtr& inpMaterial)
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
        SpriteComponentUPtr RenderComponentFactory::CreateSpriteComponent(const SpriteSheetSPtr& pSpriteSheet, u32 inTpageIndex, const MaterialSPtr& inpMaterial)
        {
            SpriteComponentUPtr pSprite(new SpriteComponent());
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
        SpriteComponentUPtr RenderComponentFactory::CreateSpriteComponent(Core::StorageLocation ineStorageLocation, const std::string& instrSpriteSheetTexture, u32 inTpageIndex, const MaterialSPtr& inpMaterial)
        {
            SpriteComponentUPtr pSprite(new SpriteComponent());
            pSprite->SetMaterial(inpMaterial);
            SpriteSheetSPtr pSpriteSheet = LOAD_RESOURCE(SpriteSheet, ineStorageLocation, instrSpriteSheetTexture);
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
        SpriteComponentUPtr RenderComponentFactory::CreateSpriteComponent(Core::StorageLocation ineStorageLocation, const std::string& instrSpriteSheetTexture, const std::string& instrTpageID, const MaterialSPtr& inpMaterial)
        {
            SpriteComponentUPtr pSprite(new SpriteComponent());
            pSprite->SetMaterial(inpMaterial);
            SpriteSheetSPtr pSpriteSheet = LOAD_RESOURCE(SpriteSheet, ineStorageLocation, instrSpriteSheetTexture);
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
		StaticMeshComponentUPtr RenderComponentFactory::CreateStaticMeshComponent(const MeshSPtr& inpModel)
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
		StaticMeshComponentUPtr RenderComponentFactory::CreateStaticMeshComponent(const MeshSPtr& inpModel, const MaterialSPtr& inpMaterial)
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
		StaticMeshComponentUPtr RenderComponentFactory::CreateStaticMeshComponent(const MeshSPtr& inpModel, Core::StorageLocation ineStorageLocation, const std::string & insMaterialName)
		{
			MaterialSPtr pMaterial = mpMaterialManager->GetMaterialFromFile(ineStorageLocation, insMaterialName);
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
            MeshSPtr pModel = mpMeshManager->GetModelFromFile(ineStorageLocation, instrModelFilePath);
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
		StaticMeshComponentUPtr RenderComponentFactory::CreateStaticMeshComponent(Core::StorageLocation ineStorageLocation, const std::string& instrModelFileName, const MaterialSPtr& inpMaterial)
        {
			MeshSPtr pModel = mpMeshManager->GetModelFromFile(ineStorageLocation, instrModelFileName);
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
			MeshSPtr pModel = mpMeshManager->GetModelFromFile(ineModelStorageLocation, instrModelFilePath);
			MaterialSPtr pMaterial = mpMaterialManager->GetMaterialFromFile(ineMaterialStorageLocation, instrMaterialFilePath);
			
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
		AnimatedMeshComponentUPtr RenderComponentFactory::CreateAnimatedMeshComponent(const MeshSPtr& inpModel)
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
		AnimatedMeshComponentUPtr RenderComponentFactory::CreateAnimatedMeshComponent(const MeshSPtr& inpModel, const MaterialSPtr& inpMaterial)
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
		AnimatedMeshComponentUPtr RenderComponentFactory::CreateAnimatedMeshComponent(const MeshSPtr& inpModel, Core::StorageLocation ineStorageLocation, const std::string & insMaterialName)
		{
			MaterialSPtr pMaterial = mpMaterialManager->GetMaterialFromFile(ineStorageLocation, insMaterialName);
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
            MeshSPtr pModel = mpMeshManager->GetModelFromFile(ineStorageLocation, instrModelFilePath);
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
		AnimatedMeshComponentUPtr RenderComponentFactory::CreateAnimatedMeshComponent(Core::StorageLocation ineStorageLocation, const std::string& instrModelFileName, const MaterialSPtr& inpMaterial)
        {
			MeshSPtr pModel = mpMeshManager->GetModelFromFile(ineStorageLocation, instrModelFileName);
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
			MeshSPtr pModel = mpMeshManager->GetModelFromFile(ineModelStorageLocation, instrModelFilePath);
			MaterialSPtr pMaterial = mpMaterialManager->GetMaterialFromFile(ineMaterialStorageLocation, instrMaterialFilePath);
			
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
			desc.vViewSize = Core::Screen::GetOrientedDimensions();
			desc.fAspect = (desc.vViewSize.x/desc.vViewSize.y);
			desc.fFOV = infFOV/desc.fAspect;
			desc.fNearClipping = infNear;
			desc.fFarClipping = infFar;
			desc.ClearCol = Core::Colour::WHITE;
			desc.IsOrthographic = inbIsOrthographic;
			desc.bShouldResizeToScreen = true;
			desc.bShouldRotateToScreen = true;
			
			CameraComponentUPtr pCamera(new CameraComponent(desc));
			pCamera->SetViewportOrientation(Core::Screen::GetOrientation());
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
            TextureSPtr pShadowMap;
            TextureSPtr pShadowMapDebug;
            
            if(mpRenderCapabilities->IsShadowMappingSupported() == true && inudwShadowMapRes > 0)
            {
                pShadowMap = mpTextureManager->CreateTextureResource();
                mpTextureManager->CreateEmptyTexture(inudwShadowMapRes, inudwShadowMapRes, Core::Image::Format::k_Depth16, pShadowMap);

#ifdef CS_ENABLE_DEBUGSHADOW
                pShadowMapDebug = mpTextureManager->CreateTextureResource();
                mpTextureManager->CreateEmptyTexture(inudwShadowMapRes, inudwShadowMapRes, Core::Image::Format::k_RGB888, pShadowMapDebug);
#endif
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
