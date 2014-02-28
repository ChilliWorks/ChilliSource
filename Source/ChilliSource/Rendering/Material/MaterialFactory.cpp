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
        MaterialSPtr MaterialFactory::CreateCustom() const
        {
            Material* pMaterial = new Material();
			return MaterialSPtr(pMaterial);
        }
        
        //---------------------------------------------------
        /// Create GUI
        //---------------------------------------------------
        MaterialSPtr MaterialFactory::CreateGUI() const
        {
            Material* pMaterial = new Material();
            
            pMaterial->SetShaderProgram(ShaderPass::k_ambient, GetShaderManager()->GetShaderFromFile(Core::StorageLocation::k_package, "Core/GUI"));
            pMaterial->SetActiveShaderProgram(ShaderPass::k_ambient);
            pMaterial->SetTransparent(true);
            pMaterial->SetCullFace(CullFace::k_front);
            pMaterial->SetCullingEnabled(false);
            
			return MaterialSPtr(pMaterial);
        }
        //---------------------------------------------------
        /// Create GUI Distance Font
        //---------------------------------------------------
        MaterialSPtr MaterialFactory::CreateGUIDistanceFont() const
        {
            Material* pMaterial = new Material();
            
            pMaterial->SetShaderProgram(ShaderPass::k_ambient, GetShaderManager()->GetShaderFromFile(Core::StorageLocation::k_package, "Core/GUIDistanceFont"));
            pMaterial->SetActiveShaderProgram(ShaderPass::k_ambient);
            pMaterial->SetTransparent(true);
            pMaterial->SetCullFace(CullFace::k_front);
            pMaterial->SetCullingEnabled(false);
            
			return MaterialSPtr(pMaterial);
        }
        //---------------------------------------------------
        /// Create GUI Distance Font Outlined
        //---------------------------------------------------
        MaterialSPtr MaterialFactory::CreateGUIDistanceFontOutlined() const
        {
            Material* pMaterial = new Material();
            pMaterial->SetShaderProgram(ShaderPass::k_ambient, GetShaderManager()->GetShaderFromFile(Core::StorageLocation::k_package, "Core/GUIDistanceFontOutlined"));
            pMaterial->SetActiveShaderProgram(ShaderPass::k_ambient);
            pMaterial->SetTransparent(true);
            pMaterial->SetCullFace(CullFace::k_front);
            pMaterial->SetCullingEnabled(false);
			return MaterialSPtr(pMaterial);
        }
        //---------------------------------------------------
        /// Create Sprite
        //---------------------------------------------------
        MaterialSPtr MaterialFactory::CreateSprite(const TextureSPtr& inpTex) const
        {
            Material* pMaterial = new Material();
            pMaterial->SetShaderProgram(ShaderPass::k_ambient, GetShaderManager()->GetShaderFromFile(Core::StorageLocation::k_package, "Core/Sprite"));
            pMaterial->SetActiveShaderProgram(ShaderPass::k_ambient);
            pMaterial->SetTexture(inpTex);
            pMaterial->SetTransparent(false);
            pMaterial->SetCullFace(CullFace::k_front);
            pMaterial->SetCullingEnabled(false);
			return MaterialSPtr(pMaterial);
        }
        //---------------------------------------------------
        /// Create Static
        //---------------------------------------------------
        MaterialSPtr MaterialFactory::CreateStatic(const TextureSPtr& inpTex) const
        {
            Material* pMaterial = new Material();
            pMaterial->SetShaderProgram(ShaderPass::k_ambient, GetShaderManager()->GetShaderFromFile(Core::StorageLocation::k_package, "Core/Static"));
            pMaterial->SetActiveShaderProgram(ShaderPass::k_ambient);
            pMaterial->SetTexture(inpTex);
            pMaterial->SetTransparent(false);
            pMaterial->SetCullFace(CullFace::k_front);
            pMaterial->SetCullingEnabled(true);
			return MaterialSPtr(pMaterial);
        }
        //---------------------------------------------------
        /// Create Static
        //---------------------------------------------------
        MaterialSPtr MaterialFactory::CreateStaticAmbient(const TextureSPtr& inpTex) const
        {
            Material* pMaterial = new Material();
            pMaterial->SetShaderProgram(ShaderPass::k_ambient, GetShaderManager()->GetShaderFromFile(Core::StorageLocation::k_package, "Core/StaticAmbient"));
            pMaterial->SetActiveShaderProgram(ShaderPass::k_ambient);
            pMaterial->SetTexture(inpTex);
            pMaterial->SetTransparent(false);
            pMaterial->SetCullFace(CullFace::k_front);
            pMaterial->SetCullingEnabled(true);
			return MaterialSPtr(pMaterial);
        }
        //---------------------------------------------------
        /// Create Static Blinn
        //---------------------------------------------------
        MaterialSPtr MaterialFactory::CreateStaticBlinn(const TextureSPtr& inpTex) const
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
			return MaterialSPtr(pMaterial);
        }
        //---------------------------------------------------
        /// Create Static Blinn Shadowed
        //---------------------------------------------------
        MaterialSPtr MaterialFactory::CreateStaticBlinnShadowed(const TextureSPtr& inpTex) const
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
			return MaterialSPtr(pMaterial);
        }
        //---------------------------------------------------
        /// Create Static Blinn Per Vertex
        //---------------------------------------------------
        MaterialSPtr MaterialFactory::CreateStaticBlinnPerVertex(const TextureSPtr& inpTex) const
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
			return MaterialSPtr(pMaterial);
        }
        //---------------------------------------------------
        /// Create Static Blinn Per Vertex
        //---------------------------------------------------
        MaterialSPtr MaterialFactory::CreateStaticBlinnPerVertexShadowed(const TextureSPtr& inpTex) const
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
			return MaterialSPtr(pMaterial);
        }
        //---------------------------------------------------
        /// Create Static Directional Shadow Map
        //---------------------------------------------------
        MaterialSPtr MaterialFactory::CreateStaticDirectionalShadowMap() const
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
            
			return MaterialSPtr(pMaterial);
        }
        //---------------------------------------------------
        /// Create Animated
        //---------------------------------------------------
        MaterialSPtr MaterialFactory::CreateAnimated(const TextureSPtr& inpTex) const
        {
            Material* pMaterial = new Material();
            pMaterial->SetShaderProgram(ShaderPass::k_ambient, GetShaderManager()->GetShaderFromFile(Core::StorageLocation::k_package, "Core/Animated"));
            pMaterial->SetActiveShaderProgram(ShaderPass::k_ambient);
            pMaterial->SetTexture(inpTex);
            pMaterial->SetTransparent(false);
            pMaterial->SetCullFace(CullFace::k_front);
            pMaterial->SetCullingEnabled(true);
			return MaterialSPtr(pMaterial);
        }
        //---------------------------------------------------
        /// Create Animated Ambient
        //---------------------------------------------------
        MaterialSPtr MaterialFactory::CreateAnimatedAmbient(const TextureSPtr& inpTex) const
        {
            Material* pMaterial = new Material();
            pMaterial->SetShaderProgram(ShaderPass::k_ambient, GetShaderManager()->GetShaderFromFile(Core::StorageLocation::k_package, "Core/AnimatedAmbient"));
            pMaterial->SetActiveShaderProgram(ShaderPass::k_ambient);
            pMaterial->SetTexture(inpTex);
            pMaterial->SetTransparent(false);
            pMaterial->SetCullFace(CullFace::k_front);
            pMaterial->SetCullingEnabled(true);
			return MaterialSPtr(pMaterial);
        }
        //---------------------------------------------------
        /// Create Animated Blinn
        //---------------------------------------------------
        MaterialSPtr MaterialFactory::CreateAnimatedBlinn(const TextureSPtr& inpTex) const
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
			return MaterialSPtr(pMaterial);
        }
        //---------------------------------------------------
        /// Create Animated Blinn Shadowed
        //---------------------------------------------------
        MaterialSPtr MaterialFactory::CreateAnimatedBlinnShadowed(const TextureSPtr& inpTex) const
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
			return MaterialSPtr(pMaterial);
        }
        //---------------------------------------------------
        /// Create Animated Blinn Per Vertex
        //---------------------------------------------------
        MaterialSPtr MaterialFactory::CreateAnimatedBlinnPerVertex(const TextureSPtr& inpTex) const
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
			return MaterialSPtr(pMaterial);
        }
        //---------------------------------------------------
        /// Create Animated Blinn Per Vertex
        //---------------------------------------------------
        MaterialSPtr MaterialFactory::CreateAnimatedBlinnPerVertexShadowed(const TextureSPtr& inpTex) const
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
			return MaterialSPtr(pMaterial);
        }
        //---------------------------------------------------
        /// Create Animated Directional Shadow Map
        //---------------------------------------------------
        MaterialSPtr MaterialFactory::CreateAnimatedDirectionalShadowMap() const
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
            
			return MaterialSPtr(pMaterial);
        }
        //---------------------------------------------------
        /// Get Texture Manager
        //---------------------------------------------------
        TextureManager* MaterialFactory::GetTextureManager() const
        {
            if(mpTextureManager == nullptr)
            {
                mpTextureManager = GET_RESOURCE_MANAGER(TextureManager);
            }
            
            return mpTextureManager;
        }
        //---------------------------------------------------
        /// Get Shader Manager
        //---------------------------------------------------
        ShaderManager* MaterialFactory::GetShaderManager() const
        {
            if(mpShaderManager == nullptr)
            {
                mpShaderManager = GET_RESOURCE_MANAGER(ShaderManager);
            }
            
            return mpShaderManager;
        }
        //---------------------------------------------------
        /// Get Cubemap Manager
        //---------------------------------------------------
        CubemapManager* MaterialFactory::GetCubemapManager() const
        {
            if(mpCubemapManager == nullptr)
            {
                mpCubemapManager = GET_RESOURCE_MANAGER(CubemapManager);
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
        		mpRenderCapabilities = Core::Application::GetSystemImplementing<RenderCapabilities>();
        		CS_ASSERT(mpRenderCapabilities, "Material Factor is missing required system: Render Capabilities.");
			}

			return mpRenderCapabilities;
        }
    }
}
