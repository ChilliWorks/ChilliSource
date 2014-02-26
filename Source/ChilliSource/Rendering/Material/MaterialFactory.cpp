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
        DEFINE_NAMED_INTERFACE(CMaterialFactory);
        
        //---------------------------------------------------
        /// Constructor
        //---------------------------------------------------
        CMaterialFactory::CMaterialFactory()
        : mpTextureManager(nullptr), mpShaderManager(nullptr), mpCubemapManager(nullptr), mpRenderCapabilities(nullptr)
        {
            
        }
        //---------------------------------------------------
        /// Is A
        //---------------------------------------------------
        bool CMaterialFactory::IsA(Core::InterfaceIDType inInterfaceID) const
        {
            return inInterfaceID == CMaterialFactory::InterfaceID;
        }
        //---------------------------------------------------
        /// Create Custom
        //---------------------------------------------------
        MaterialPtr CMaterialFactory::CreateCustom() const
        {
            CMaterial* pMaterial = new CMaterial();
			return MaterialPtr(pMaterial);
        }
        
        //---------------------------------------------------
        /// Create GUI
        //---------------------------------------------------
        MaterialPtr CMaterialFactory::CreateGUI() const
        {
            CMaterial* pMaterial = new CMaterial();
            
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
        MaterialPtr CMaterialFactory::CreateGUIDistanceFont() const
        {
            CMaterial* pMaterial = new CMaterial();
            
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
        MaterialPtr CMaterialFactory::CreateGUIDistanceFontOutlined() const
        {
            CMaterial* pMaterial = new CMaterial();
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
        MaterialPtr CMaterialFactory::CreateSprite(const TexturePtr& inpTex) const
        {
            CMaterial* pMaterial = new CMaterial();
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
        MaterialPtr CMaterialFactory::CreateStatic(const TexturePtr& inpTex) const
        {
            CMaterial* pMaterial = new CMaterial();
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
        MaterialPtr CMaterialFactory::CreateStaticAmbient(const TexturePtr& inpTex) const
        {
            CMaterial* pMaterial = new CMaterial();
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
        MaterialPtr CMaterialFactory::CreateStaticBlinn(const TexturePtr& inpTex) const
        {
            CMaterial* pMaterial = new CMaterial();
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
        MaterialPtr CMaterialFactory::CreateStaticBlinnShadowed(const TexturePtr& inpTex) const
        {
        	if (GetRenderCapabilities()->IsShadowMappingSupported() == false)
			{
				return CreateStaticBlinn(inpTex);
			}

            CMaterial* pMaterial = new CMaterial();
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
        MaterialPtr CMaterialFactory::CreateStaticBlinnPerVertex(const TexturePtr& inpTex) const
        {
            CMaterial* pMaterial = new CMaterial();
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
        MaterialPtr CMaterialFactory::CreateStaticBlinnPerVertexShadowed(const TexturePtr& inpTex) const
        {
        	if (GetRenderCapabilities()->IsShadowMappingSupported() == false)
			{
				return CreateStaticBlinnPerVertex(inpTex);
			}

            CMaterial* pMaterial = new CMaterial();
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
        MaterialPtr CMaterialFactory::CreateStaticDirectionalShadowMap() const
        {
            CMaterial* pMaterial = new CMaterial();
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
        MaterialPtr CMaterialFactory::CreateAnimated(const TexturePtr& inpTex) const
        {
            CMaterial* pMaterial = new CMaterial();
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
        MaterialPtr CMaterialFactory::CreateAnimatedAmbient(const TexturePtr& inpTex) const
        {
            CMaterial* pMaterial = new CMaterial();
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
        MaterialPtr CMaterialFactory::CreateAnimatedBlinn(const TexturePtr& inpTex) const
        {
            CMaterial* pMaterial = new CMaterial();
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
        MaterialPtr CMaterialFactory::CreateAnimatedBlinnShadowed(const TexturePtr& inpTex) const
        {
        	if (GetRenderCapabilities()->IsShadowMappingSupported() == false)
			{
				return CreateAnimatedBlinn(inpTex);
			}

            CMaterial* pMaterial = new CMaterial();
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
        MaterialPtr CMaterialFactory::CreateAnimatedBlinnPerVertex(const TexturePtr& inpTex) const
        {
            CMaterial* pMaterial = new CMaterial();
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
        MaterialPtr CMaterialFactory::CreateAnimatedBlinnPerVertexShadowed(const TexturePtr& inpTex) const
        {
        	if (GetRenderCapabilities()->IsShadowMappingSupported() == false)
        	{
        		return CreateAnimatedBlinnPerVertex(inpTex);
        	}

            CMaterial* pMaterial = new CMaterial();
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
        MaterialPtr CMaterialFactory::CreateAnimatedDirectionalShadowMap() const
        {
            CMaterial* pMaterial = new CMaterial();
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
        ITextureManager* CMaterialFactory::GetTextureManager() const
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
        IShaderManager* CMaterialFactory::GetShaderManager() const
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
        ICubemapManager* CMaterialFactory::GetCubemapManager() const
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
        IRenderCapabilities* CMaterialFactory::GetRenderCapabilities() const
        {
        	if(mpRenderCapabilities == nullptr)
			{
        		mpRenderCapabilities = Core::CApplication::GetSystemImplementing<IRenderCapabilities>();
        		MOFLOW_ASSERT(mpRenderCapabilities, "Material Factor is missing required system: Render Capabilities.");
			}

			return mpRenderCapabilities;
        }
    }
}
