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
        CS_DEFINE_NAMEDTYPE(MaterialFactory);
        
        //-------------------------------------------------------
        //-------------------------------------------------------
        MaterialFactoryUPtr MaterialFactory::Create(TextureManager* in_texMgr, ShaderManager* in_shaderMgr, CubemapManager* in_cubemapMgr, RenderCapabilities* in_renderCapabilities)
        {
            return MaterialFactoryUPtr(new MaterialFactory(in_texMgr, in_shaderMgr, in_cubemapMgr, in_renderCapabilities));
        }
        
        //---------------------------------------------------
        /// Constructor
        //---------------------------------------------------
        MaterialFactory::MaterialFactory(TextureManager* in_texMgr, ShaderManager* in_shaderMgr, CubemapManager* in_cubemapMgr, RenderCapabilities* in_renderCapabilities)
        : mpTextureManager(in_texMgr), mpShaderManager(in_shaderMgr), mpCubemapManager(in_cubemapMgr), mpRenderCapabilities(in_renderCapabilities)
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
        MaterialUPtr MaterialFactory::CreateCustom() const
        {
            Material* pMaterial = new Material();
			return MaterialUPtr(pMaterial);
        }
        
        //---------------------------------------------------
        /// Create GUI
        //---------------------------------------------------
        MaterialUPtr MaterialFactory::CreateGUI() const
        {
            Material* pMaterial = new Material();
            
            pMaterial->SetShaderProgram(ShaderPass::k_ambient, GetShaderManager()->GetShaderFromFile(Core::StorageLocation::k_package, "Core/GUI"));
            pMaterial->SetActiveShaderProgram(ShaderPass::k_ambient);
            pMaterial->SetTransparent(true);
            pMaterial->SetCullFace(CullFace::k_front);
            pMaterial->SetCullingEnabled(false);
            
			return MaterialUPtr(pMaterial);
        }
        //---------------------------------------------------
        /// Create GUI Distance Font
        //---------------------------------------------------
        MaterialUPtr MaterialFactory::CreateGUIDistanceFont() const
        {
            Material* pMaterial = new Material();
            
            pMaterial->SetShaderProgram(ShaderPass::k_ambient, GetShaderManager()->GetShaderFromFile(Core::StorageLocation::k_package, "Core/GUIDistanceFont"));
            pMaterial->SetActiveShaderProgram(ShaderPass::k_ambient);
            pMaterial->SetTransparent(true);
            pMaterial->SetCullFace(CullFace::k_front);
            pMaterial->SetCullingEnabled(false);
            
			return MaterialUPtr(pMaterial);
        }
        //---------------------------------------------------
        /// Create GUI Distance Font Outlined
        //---------------------------------------------------
        MaterialUPtr MaterialFactory::CreateGUIDistanceFontOutlined() const
        {
            Material* pMaterial = new Material();
            pMaterial->SetShaderProgram(ShaderPass::k_ambient, GetShaderManager()->GetShaderFromFile(Core::StorageLocation::k_package, "Core/GUIDistanceFontOutlined"));
            pMaterial->SetActiveShaderProgram(ShaderPass::k_ambient);
            pMaterial->SetTransparent(true);
            pMaterial->SetCullFace(CullFace::k_front);
            pMaterial->SetCullingEnabled(false);
			return MaterialUPtr(pMaterial);
        }
        //---------------------------------------------------
        /// Create Sprite
        //---------------------------------------------------
        MaterialUPtr MaterialFactory::CreateSprite(const TextureSPtr& inpTex) const
        {
            Material* pMaterial = new Material();
            pMaterial->SetShaderProgram(ShaderPass::k_ambient, GetShaderManager()->GetShaderFromFile(Core::StorageLocation::k_package, "Core/Sprite"));
            pMaterial->SetActiveShaderProgram(ShaderPass::k_ambient);
            pMaterial->SetTexture(inpTex);
            pMaterial->SetTransparent(false);
            pMaterial->SetCullFace(CullFace::k_front);
            pMaterial->SetCullingEnabled(false);
			return MaterialUPtr(pMaterial);
        }
        //---------------------------------------------------
        /// Create Static
        //---------------------------------------------------
        MaterialUPtr MaterialFactory::CreateStatic(const TextureSPtr& inpTex) const
        {
            Material* pMaterial = new Material();
            pMaterial->SetShaderProgram(ShaderPass::k_ambient, GetShaderManager()->GetShaderFromFile(Core::StorageLocation::k_package, "Core/Static"));
            pMaterial->SetActiveShaderProgram(ShaderPass::k_ambient);
            pMaterial->SetTexture(inpTex);
            pMaterial->SetTransparent(false);
            pMaterial->SetCullFace(CullFace::k_front);
            pMaterial->SetCullingEnabled(true);
			return MaterialUPtr(pMaterial);
        }
        //---------------------------------------------------
        /// Create Static
        //---------------------------------------------------
        MaterialUPtr MaterialFactory::CreateStaticAmbient(const TextureSPtr& inpTex) const
        {
            Material* pMaterial = new Material();
            pMaterial->SetShaderProgram(ShaderPass::k_ambient, GetShaderManager()->GetShaderFromFile(Core::StorageLocation::k_package, "Core/StaticAmbient"));
            pMaterial->SetActiveShaderProgram(ShaderPass::k_ambient);
            pMaterial->SetTexture(inpTex);
            pMaterial->SetTransparent(false);
            pMaterial->SetCullFace(CullFace::k_front);
            pMaterial->SetCullingEnabled(true);
			return MaterialUPtr(pMaterial);
        }
        //---------------------------------------------------
        /// Create Static Blinn
        //---------------------------------------------------
        MaterialUPtr MaterialFactory::CreateStaticBlinn(const TextureSPtr& inpTex) const
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
			return MaterialUPtr(pMaterial);
        }
        //---------------------------------------------------
        /// Create Static Blinn Shadowed
        //---------------------------------------------------
        MaterialUPtr MaterialFactory::CreateStaticBlinnShadowed(const TextureSPtr& inpTex) const
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
			return MaterialUPtr(pMaterial);
        }
        //---------------------------------------------------
        /// Create Static Blinn Per Vertex
        //---------------------------------------------------
        MaterialUPtr MaterialFactory::CreateStaticBlinnPerVertex(const TextureSPtr& inpTex) const
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
			return MaterialUPtr(pMaterial);
        }
        //---------------------------------------------------
        /// Create Static Blinn Per Vertex
        //---------------------------------------------------
        MaterialUPtr MaterialFactory::CreateStaticBlinnPerVertexShadowed(const TextureSPtr& inpTex) const
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
			return MaterialUPtr(pMaterial);
        }
        //---------------------------------------------------
        /// Create Static Directional Shadow Map
        //---------------------------------------------------
        MaterialUPtr MaterialFactory::CreateStaticDirectionalShadowMap() const
        {
            Material* pMaterial = new Material();
            pMaterial->SetShaderProgram(ShaderPass::k_ambient, GetShaderManager()->GetShaderFromFile(Core::StorageLocation::k_package, "Core/StaticDirectionalShadowMap"));
            pMaterial->SetTransparent(false);
            pMaterial->SetCullFace(CullFace::k_front);
            pMaterial->SetCullingEnabled(true);
            
#ifdef CS_ENABLE_DEBUGSHADOW
            pMaterial->SetColourWriteEnabled(true);
#else
            pMaterial->SetColourWriteEnabled(false);
#endif
            
			return MaterialUPtr(pMaterial);
        }
        //---------------------------------------------------
        /// Create Animated
        //---------------------------------------------------
        MaterialUPtr MaterialFactory::CreateAnimated(const TextureSPtr& inpTex) const
        {
            Material* pMaterial = new Material();
            pMaterial->SetShaderProgram(ShaderPass::k_ambient, GetShaderManager()->GetShaderFromFile(Core::StorageLocation::k_package, "Core/Animated"));
            pMaterial->SetActiveShaderProgram(ShaderPass::k_ambient);
            pMaterial->SetTexture(inpTex);
            pMaterial->SetTransparent(false);
            pMaterial->SetCullFace(CullFace::k_front);
            pMaterial->SetCullingEnabled(true);
			return MaterialUPtr(pMaterial);
        }
        //---------------------------------------------------
        /// Create Animated Ambient
        //---------------------------------------------------
        MaterialUPtr MaterialFactory::CreateAnimatedAmbient(const TextureSPtr& inpTex) const
        {
            Material* pMaterial = new Material();
            pMaterial->SetShaderProgram(ShaderPass::k_ambient, GetShaderManager()->GetShaderFromFile(Core::StorageLocation::k_package, "Core/AnimatedAmbient"));
            pMaterial->SetActiveShaderProgram(ShaderPass::k_ambient);
            pMaterial->SetTexture(inpTex);
            pMaterial->SetTransparent(false);
            pMaterial->SetCullFace(CullFace::k_front);
            pMaterial->SetCullingEnabled(true);
			return MaterialUPtr(pMaterial);
        }
        //---------------------------------------------------
        /// Create Animated Blinn
        //---------------------------------------------------
        MaterialUPtr MaterialFactory::CreateAnimatedBlinn(const TextureSPtr& inpTex) const
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
			return MaterialUPtr(pMaterial);
        }
        //---------------------------------------------------
        /// Create Animated Blinn Shadowed
        //---------------------------------------------------
        MaterialUPtr MaterialFactory::CreateAnimatedBlinnShadowed(const TextureSPtr& inpTex) const
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
			return MaterialUPtr(pMaterial);
        }
        //---------------------------------------------------
        /// Create Animated Blinn Per Vertex
        //---------------------------------------------------
        MaterialUPtr MaterialFactory::CreateAnimatedBlinnPerVertex(const TextureSPtr& inpTex) const
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
			return MaterialUPtr(pMaterial);
        }
        //---------------------------------------------------
        /// Create Animated Blinn Per Vertex
        //---------------------------------------------------
        MaterialUPtr MaterialFactory::CreateAnimatedBlinnPerVertexShadowed(const TextureSPtr& inpTex) const
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
			return MaterialUPtr(pMaterial);
        }
        //---------------------------------------------------
        /// Create Animated Directional Shadow Map
        //---------------------------------------------------
        MaterialUPtr MaterialFactory::CreateAnimatedDirectionalShadowMap() const
        {
            Material* pMaterial = new Material();
            pMaterial->SetShaderProgram(ShaderPass::k_ambient, GetShaderManager()->GetShaderFromFile(Core::StorageLocation::k_package, "Core/AnimatedDirectionalShadowMap"));
            pMaterial->SetTransparent(false);
            pMaterial->SetCullFace(CullFace::k_front);
            pMaterial->SetCullingEnabled(true);
            
#ifdef CS_ENABLE_DEBUGSHADOW
            pMaterial->SetColourWriteEnabled(true);
#else
            pMaterial->SetColourWriteEnabled(false);
#endif
            
			return MaterialUPtr(pMaterial);
        }
        //---------------------------------------------------
        /// Get Texture Manager
        //---------------------------------------------------
        TextureManager* MaterialFactory::GetTextureManager() const
        {
            return mpTextureManager;
        }
        //---------------------------------------------------
        /// Get Shader Manager
        //---------------------------------------------------
        ShaderManager* MaterialFactory::GetShaderManager() const
        {
            return mpShaderManager;
        }
        //---------------------------------------------------
        /// Get Cubemap Manager
        //---------------------------------------------------
        CubemapManager* MaterialFactory::GetCubemapManager() const
        {
            return mpCubemapManager;
        }
        //---------------------------------------------------
        /// Get Render Capabilities
        //---------------------------------------------------
        RenderCapabilities* MaterialFactory::GetRenderCapabilities() const
        {
			return mpRenderCapabilities;
        }
    }
}
