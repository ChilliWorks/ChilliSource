//
//  MaterialFactory.cpp
//  MoFlow
//
//  Created by Scott Downie on 05/08/2013.
//  Copyright (c) 2013 Tag Games Ltd. All rights reserved.
//

#include <ChilliSource/Rendering/Material/MaterialFactory.h>
#include <ChilliSource/Rendering/Material/Material.h>
#include <ChilliSource/Rendering/Shader/Shader.h>
#include <ChilliSource/Rendering/Shader/ShaderManager.h>
#include <ChilliSource/Rendering/Texture/Texture.h>
#include <ChilliSource/Rendering/Texture/TextureManager.h>
#include <ChilliSource/Rendering/Texture/Cubemap.h>
#include <ChilliSource/Rendering/Texture/CubemapManager.h>
#include <ChilliSource/Rendering/Base/RenderCapabilities.h>
#include <ChilliSource/Core/Resource/ResourceManagerDispenser.h>
#include <ChilliSource/Core/Base/Application.h>

namespace ChilliSource
{
    namespace Rendering
    {
        DEFINE_NAMED_INTERFACE(MaterialFactory);
        
        //---------------------------------------------------
        /// Constructor
        //---------------------------------------------------
        MaterialFactory::MaterialFactory()
        : mpTextureManager(nullptr), mpShaderManager(nullptr), mpCubemapManager(nullptr), mpRenderCapabilities(nullptr)
        {
            
        }
        //---------------------------------------------------
        /// Is A
        //---------------------------------------------------
        bool MaterialFactory::IsA(Core::InterfaceIDType inInterfaceID) const
        {
            return inInterfaceID == MaterialFactory::InterfaceID;
        }
        //---------------------------------------------------
        /// Create Custom
        //---------------------------------------------------
        MaterialPtr MaterialFactory::CreateCustom() const
        {
            Material* pMaterial = new Material();
			return MaterialPtr(pMaterial);
        }
        
        //---------------------------------------------------
        /// Create GUI
        //---------------------------------------------------
        MaterialPtr MaterialFactory::CreateGUI() const
        {
            Material* pMaterial = new Material();
            
            pMaterial->SetShaderProgram(ShaderPass::k_ambient, GetShaderManager()->GetShaderFromFile(Core::StorageLocation::k_package, "Core/GUI"));
            pMaterial->SetActiveShaderProgram(ShaderPass::k_ambient);
            pMaterial->SetTransparent(true);
            pMaterial->SetCullFace(CullFace::k_front);
            pMaterial->SetCullingEnabled(false);
            
			return MaterialPtr(pMaterial);
        }
        //---------------------------------------------------
        /// Create GUI Distance Font
        //---------------------------------------------------
        MaterialPtr MaterialFactory::CreateGUIDistanceFont() const
        {
            Material* pMaterial = new Material();
            
            pMaterial->SetShaderProgram(ShaderPass::k_ambient, GetShaderManager()->GetShaderFromFile(Core::StorageLocation::k_package, "Core/GUIDistanceFont"));
            pMaterial->SetActiveShaderProgram(ShaderPass::k_ambient);
            pMaterial->SetTransparent(true);
            pMaterial->SetCullFace(CullFace::k_front);
            pMaterial->SetCullingEnabled(false);
            
			return MaterialPtr(pMaterial);
        }
        //---------------------------------------------------
        /// Create GUI Distance Font Outlined
        //---------------------------------------------------
        MaterialPtr MaterialFactory::CreateGUIDistanceFontOutlined() const
        {
            Material* pMaterial = new Material();
            pMaterial->SetShaderProgram(ShaderPass::k_ambient, GetShaderManager()->GetShaderFromFile(Core::StorageLocation::k_package, "Core/GUIDistanceFontOutlined"));
            pMaterial->SetActiveShaderProgram(ShaderPass::k_ambient);
            pMaterial->SetTransparent(true);
            pMaterial->SetCullFace(CullFace::k_front);
            pMaterial->SetCullingEnabled(false);
			return MaterialPtr(pMaterial);
        }
        //---------------------------------------------------
        /// Create Sprite
        //---------------------------------------------------
        MaterialPtr MaterialFactory::CreateSprite(const TexturePtr& inpTex) const
        {
            Material* pMaterial = new Material();
            pMaterial->SetShaderProgram(ShaderPass::k_ambient, GetShaderManager()->GetShaderFromFile(Core::StorageLocation::k_package, "Core/Sprite"));
            pMaterial->SetActiveShaderProgram(ShaderPass::k_ambient);
            pMaterial->SetTexture(inpTex);
            pMaterial->SetTransparent(false);
            pMaterial->SetCullFace(CullFace::k_front);
            pMaterial->SetCullingEnabled(false);
			return MaterialPtr(pMaterial);
        }
        //---------------------------------------------------
        /// Create Static
        //---------------------------------------------------
        MaterialPtr MaterialFactory::CreateStatic(const TexturePtr& inpTex) const
        {
            Material* pMaterial = new Material();
            pMaterial->SetShaderProgram(ShaderPass::k_ambient, GetShaderManager()->GetShaderFromFile(Core::StorageLocation::k_package, "Core/Static"));
            pMaterial->SetActiveShaderProgram(ShaderPass::k_ambient);
            pMaterial->SetTexture(inpTex);
            pMaterial->SetTransparent(false);
            pMaterial->SetCullFace(CullFace::k_front);
            pMaterial->SetCullingEnabled(true);
			return MaterialPtr(pMaterial);
        }
        //---------------------------------------------------
        /// Create Static
        //---------------------------------------------------
        MaterialPtr MaterialFactory::CreateStaticAmbient(const TexturePtr& inpTex) const
        {
            Material* pMaterial = new Material();
            pMaterial->SetShaderProgram(ShaderPass::k_ambient, GetShaderManager()->GetShaderFromFile(Core::StorageLocation::k_package, "Core/StaticAmbient"));
            pMaterial->SetActiveShaderProgram(ShaderPass::k_ambient);
            pMaterial->SetTexture(inpTex);
            pMaterial->SetTransparent(false);
            pMaterial->SetCullFace(CullFace::k_front);
            pMaterial->SetCullingEnabled(true);
			return MaterialPtr(pMaterial);
        }
        //---------------------------------------------------
        /// Create Static Blinn
        //---------------------------------------------------
        MaterialPtr MaterialFactory::CreateStaticBlinn(const TexturePtr& inpTex) const
        {
            Material* pMaterial = new Material();
            pMaterial->SetShaderProgram(ShaderPass::k_ambient, GetShaderManager()->GetShaderFromFile(Core::StorageLocation::k_package, "Core/StaticAmbient"));
            pMaterial->SetShaderProgram(ShaderPass::k_directional, GetShaderManager()->GetShaderFromFile(Core::StorageLocation::k_package, "Core/StaticBlinnDirectional"));
            pMaterial->SetShaderProgram(ShaderPass::k_point, GetShaderManager()->GetShaderFromFile(Core::StorageLocation::k_package, "Core/StaticBlinnPoint"));
            pMaterial->SetActiveShaderProgram(ShaderPass::k_ambient);
            pMaterial->SetTexture(inpTex);
            pMaterial->SetTransparent(false);
            pMaterial->SetCullFace(CullFace::k_front);
            pMaterial->SetCullingEnabled(true);
			return MaterialPtr(pMaterial);
        }
        //---------------------------------------------------
        /// Create Static Blinn Shadowed
        //---------------------------------------------------
        MaterialPtr MaterialFactory::CreateStaticBlinnShadowed(const TexturePtr& inpTex) const
        {
        	if (GetRenderCapabilities()->IsShadowMappingSupported() == false)
			{
				return CreateStaticBlinn(inpTex);
			}

            Material* pMaterial = new Material();
            pMaterial->SetShaderProgram(ShaderPass::k_ambient, GetShaderManager()->GetShaderFromFile(Core::StorageLocation::k_package, "Core/StaticAmbient"));
            pMaterial->SetShaderProgram(ShaderPass::k_directional, GetShaderManager()->GetShaderFromFile(Core::StorageLocation::k_package, "Core/StaticBlinnShadowedDirectional"));
            pMaterial->SetShaderProgram(ShaderPass::k_point, GetShaderManager()->GetShaderFromFile(Core::StorageLocation::k_package, "Core/StaticBlinnPoint"));
            pMaterial->SetActiveShaderProgram(ShaderPass::k_ambient);
            pMaterial->SetTexture(inpTex);
            pMaterial->SetTransparent(false);
            pMaterial->SetCullFace(CullFace::k_front);
            pMaterial->SetCullingEnabled(true);
			return MaterialPtr(pMaterial);
        }
        //---------------------------------------------------
        /// Create Static Blinn Per Vertex
        //---------------------------------------------------
        MaterialPtr MaterialFactory::CreateStaticBlinnPerVertex(const TexturePtr& inpTex) const
        {
            Material* pMaterial = new Material();
            pMaterial->SetShaderProgram(ShaderPass::k_ambient, GetShaderManager()->GetShaderFromFile(Core::StorageLocation::k_package, "Core/StaticAmbient"));
            pMaterial->SetShaderProgram(ShaderPass::k_directional, GetShaderManager()->GetShaderFromFile(Core::StorageLocation::k_package, "Core/StaticBlinnPerVertexDirectional"));
            pMaterial->SetShaderProgram(ShaderPass::k_point, GetShaderManager()->GetShaderFromFile(Core::StorageLocation::k_package, "Core/StaticBlinnPerVertexPoint"));
            pMaterial->SetActiveShaderProgram(ShaderPass::k_ambient);
            pMaterial->SetTexture(inpTex);
            pMaterial->SetTransparent(false);
            pMaterial->SetCullFace(CullFace::k_front);
            pMaterial->SetCullingEnabled(true);
			return MaterialPtr(pMaterial);
        }
        //---------------------------------------------------
        /// Create Static Blinn Per Vertex
        //---------------------------------------------------
        MaterialPtr MaterialFactory::CreateStaticBlinnPerVertexShadowed(const TexturePtr& inpTex) const
        {
        	if (GetRenderCapabilities()->IsShadowMappingSupported() == false)
			{
				return CreateStaticBlinnPerVertex(inpTex);
			}

            Material* pMaterial = new Material();
            pMaterial->SetShaderProgram(ShaderPass::k_ambient, GetShaderManager()->GetShaderFromFile(Core::StorageLocation::k_package, "Core/StaticAmbient"));
            pMaterial->SetShaderProgram(ShaderPass::k_directional, GetShaderManager()->GetShaderFromFile(Core::StorageLocation::k_package, "Core/StaticBlinnPerVertexShadowedDirectional"));
            pMaterial->SetShaderProgram(ShaderPass::k_point, GetShaderManager()->GetShaderFromFile(Core::StorageLocation::k_package, "Core/StaticBlinnPerVertexPoint"));
            pMaterial->SetActiveShaderProgram(ShaderPass::k_ambient);
            pMaterial->SetTexture(inpTex);
            pMaterial->SetTransparent(false);
            pMaterial->SetCullFace(CullFace::k_front);
            pMaterial->SetCullingEnabled(true);
			return MaterialPtr(pMaterial);
        }
        //---------------------------------------------------
        /// Create Static Directional Shadow Map
        //---------------------------------------------------
        MaterialPtr MaterialFactory::CreateStaticDirectionalShadowMap() const
        {
            Material* pMaterial = new Material();
            pMaterial->SetShaderProgram(ShaderPass::k_ambient, GetShaderManager()->GetShaderFromFile(Core::StorageLocation::k_package, "Core/StaticDirectionalShadowMap"));
            pMaterial->SetTransparent(false);
            pMaterial->SetCullFace(CullFace::k_front);
            pMaterial->SetCullingEnabled(true);
            
#ifdef MOFLOW_SHADOW_DEBUG
            pMaterial->SetColourWriteEnabled(true);
#else
            pMaterial->SetColourWriteEnabled(false);
#endif
            
			return MaterialPtr(pMaterial);
        }
        //---------------------------------------------------
        /// Create Animated
        //---------------------------------------------------
        MaterialPtr MaterialFactory::CreateAnimated(const TexturePtr& inpTex) const
        {
            Material* pMaterial = new Material();
            pMaterial->SetShaderProgram(ShaderPass::k_ambient, GetShaderManager()->GetShaderFromFile(Core::StorageLocation::k_package, "Core/Animated"));
            pMaterial->SetActiveShaderProgram(ShaderPass::k_ambient);
            pMaterial->SetTexture(inpTex);
            pMaterial->SetTransparent(false);
            pMaterial->SetCullFace(CullFace::k_front);
            pMaterial->SetCullingEnabled(true);
			return MaterialPtr(pMaterial);
        }
        //---------------------------------------------------
        /// Create Animated Ambient
        //---------------------------------------------------
        MaterialPtr MaterialFactory::CreateAnimatedAmbient(const TexturePtr& inpTex) const
        {
            Material* pMaterial = new Material();
            pMaterial->SetShaderProgram(ShaderPass::k_ambient, GetShaderManager()->GetShaderFromFile(Core::StorageLocation::k_package, "Core/AnimatedAmbient"));
            pMaterial->SetActiveShaderProgram(ShaderPass::k_ambient);
            pMaterial->SetTexture(inpTex);
            pMaterial->SetTransparent(false);
            pMaterial->SetCullFace(CullFace::k_front);
            pMaterial->SetCullingEnabled(true);
			return MaterialPtr(pMaterial);
        }
        //---------------------------------------------------
        /// Create Animated Blinn
        //---------------------------------------------------
        MaterialPtr MaterialFactory::CreateAnimatedBlinn(const TexturePtr& inpTex) const
        {
            Material* pMaterial = new Material();
            pMaterial->SetShaderProgram(ShaderPass::k_ambient, GetShaderManager()->GetShaderFromFile(Core::StorageLocation::k_package, "Core/AnimatedAmbient"));
            pMaterial->SetShaderProgram(ShaderPass::k_directional, GetShaderManager()->GetShaderFromFile(Core::StorageLocation::k_package, "Core/AnimatedBlinnDirectional"));
            pMaterial->SetShaderProgram(ShaderPass::k_point, GetShaderManager()->GetShaderFromFile(Core::StorageLocation::k_package, "Core/AnimatedBlinnPoint"));
            pMaterial->SetActiveShaderProgram(ShaderPass::k_ambient);
            pMaterial->SetTexture(inpTex);
            pMaterial->SetTransparent(false);
            pMaterial->SetCullFace(CullFace::k_front);
            pMaterial->SetCullingEnabled(true);
			return MaterialPtr(pMaterial);
        }
        //---------------------------------------------------
        /// Create Animated Blinn Shadowed
        //---------------------------------------------------
        MaterialPtr MaterialFactory::CreateAnimatedBlinnShadowed(const TexturePtr& inpTex) const
        {
        	if (GetRenderCapabilities()->IsShadowMappingSupported() == false)
			{
				return CreateAnimatedBlinn(inpTex);
			}

            Material* pMaterial = new Material();
            pMaterial->SetShaderProgram(ShaderPass::k_ambient, GetShaderManager()->GetShaderFromFile(Core::StorageLocation::k_package, "Core/AnimatedAmbient"));
            pMaterial->SetShaderProgram(ShaderPass::k_directional, GetShaderManager()->GetShaderFromFile(Core::StorageLocation::k_package, "Core/AnimatedBlinnShadowedDirectional"));
            pMaterial->SetShaderProgram(ShaderPass::k_point, GetShaderManager()->GetShaderFromFile(Core::StorageLocation::k_package, "Core/AnimatedBlinnPoint"));
            pMaterial->SetActiveShaderProgram(ShaderPass::k_ambient);
            pMaterial->SetTexture(inpTex);
            pMaterial->SetTransparent(false);
            pMaterial->SetCullFace(CullFace::k_front);
            pMaterial->SetCullingEnabled(true);
			return MaterialPtr(pMaterial);
        }
        //---------------------------------------------------
        /// Create Animated Blinn Per Vertex
        //---------------------------------------------------
        MaterialPtr MaterialFactory::CreateAnimatedBlinnPerVertex(const TexturePtr& inpTex) const
        {
            Material* pMaterial = new Material();
            pMaterial->SetShaderProgram(ShaderPass::k_ambient, GetShaderManager()->GetShaderFromFile(Core::StorageLocation::k_package, "Core/AnimatedAmbient"));
            pMaterial->SetShaderProgram(ShaderPass::k_directional, GetShaderManager()->GetShaderFromFile(Core::StorageLocation::k_package, "Core/AnimatedBlinnPerVertexDirectional"));
            pMaterial->SetShaderProgram(ShaderPass::k_point, GetShaderManager()->GetShaderFromFile(Core::StorageLocation::k_package, "Core/AnimatedBlinnPerVertexPoint"));
            pMaterial->SetActiveShaderProgram(ShaderPass::k_ambient);
            pMaterial->SetTexture(inpTex);
            pMaterial->SetTransparent(false);
            pMaterial->SetCullFace(CullFace::k_front);
            pMaterial->SetCullingEnabled(true);
			return MaterialPtr(pMaterial);
        }
        //---------------------------------------------------
        /// Create Animated Blinn Per Vertex
        //---------------------------------------------------
        MaterialPtr MaterialFactory::CreateAnimatedBlinnPerVertexShadowed(const TexturePtr& inpTex) const
        {
        	if (GetRenderCapabilities()->IsShadowMappingSupported() == false)
        	{
        		return CreateAnimatedBlinnPerVertex(inpTex);
        	}

            Material* pMaterial = new Material();
            pMaterial->SetShaderProgram(ShaderPass::k_ambient, GetShaderManager()->GetShaderFromFile(Core::StorageLocation::k_package, "Core/AnimatedAmbient"));
            pMaterial->SetShaderProgram(ShaderPass::k_directional, GetShaderManager()->GetShaderFromFile(Core::StorageLocation::k_package, "Core/AnimatedBlinnPerVertexShadowedDirectional"));
            pMaterial->SetShaderProgram(ShaderPass::k_point, GetShaderManager()->GetShaderFromFile(Core::StorageLocation::k_package, "Core/AnimatedBlinnPerVertexPoint"));
            pMaterial->SetActiveShaderProgram(ShaderPass::k_ambient);
            pMaterial->SetTexture(inpTex);
            pMaterial->SetTransparent(false);
            pMaterial->SetCullFace(CullFace::k_front);
            pMaterial->SetCullingEnabled(true);
			return MaterialPtr(pMaterial);
        }
        //---------------------------------------------------
        /// Create Animated Directional Shadow Map
        //---------------------------------------------------
        MaterialPtr MaterialFactory::CreateAnimatedDirectionalShadowMap() const
        {
            Material* pMaterial = new Material();
            pMaterial->SetShaderProgram(ShaderPass::k_ambient, GetShaderManager()->GetShaderFromFile(Core::StorageLocation::k_package, "Core/AnimatedDirectionalShadowMap"));
            pMaterial->SetTransparent(false);
            pMaterial->SetCullFace(CullFace::k_front);
            pMaterial->SetCullingEnabled(true);
            
#ifdef MOFLOW_SHADOW_DEBUG
            pMaterial->SetColourWriteEnabled(true);
#else
            pMaterial->SetColourWriteEnabled(false);
#endif
            
			return MaterialPtr(pMaterial);
        }
        //---------------------------------------------------
        /// Get Texture Manager
        //---------------------------------------------------
        ITextureManager* MaterialFactory::GetTextureManager() const
        {
            if(mpTextureManager == nullptr)
            {
                mpTextureManager = GET_RESOURCE_MANAGER(ITextureManager);
            }
            
            return mpTextureManager;
        }
        //---------------------------------------------------
        /// Get Shader Manager
        //---------------------------------------------------
        IShaderManager* MaterialFactory::GetShaderManager() const
        {
            if(mpShaderManager == nullptr)
            {
                mpShaderManager = GET_RESOURCE_MANAGER(IShaderManager);
            }
            
            return mpShaderManager;
        }
        //---------------------------------------------------
        /// Get Cubemap Manager
        //---------------------------------------------------
        ICubemapManager* MaterialFactory::GetCubemapManager() const
        {
            if(mpCubemapManager == nullptr)
            {
                mpCubemapManager = GET_RESOURCE_MANAGER(ICubemapManager);
            }
            
            return mpCubemapManager;
        }
        //---------------------------------------------------
        /// Get Render Capabilities
        //---------------------------------------------------
        RenderCapabilities* MaterialFactory::GetRenderCapabilities() const
        {
        	if(mpRenderCapabilities == nullptr)
			{
        		mpRenderCapabilities = Core::CApplication::GetSystemImplementing<RenderCapabilities>();
        		CS_ASSERT(mpRenderCapabilities, "Material Factor is missing required system: Render Capabilities.");
			}

			return mpRenderCapabilities;
        }
    }
}
