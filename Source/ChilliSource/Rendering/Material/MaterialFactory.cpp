//
//  MaterialFactory.cpp
//  MoFlow
//
//  Created by Scott Downie on 05/08/2013.
//  Copyright (c) 2013 Tag Games Ltd. All rights reserved.
//

#include <ChilliSource/Rendering/MaterialFactory.h>
#include <ChilliSource/Rendering/Material/Material.h>
#include <ChilliSource/Rendering/Shader/Shader.h>
#include <ChilliSource/Rendering/Shader/ShaderManager.h>
#include <ChilliSource/Rendering/Texture/Texture.h>
#include <ChilliSource/Rendering/Texture/TextureManager.h>
#include <ChilliSource/Rendering/Texture/Cubemap.h>
#include <ChilliSource/Rendering/Texture/CubemapManager.h>
#include <ChilliSource/Rendering/Main/RenderCapabilities.h>
#include <ChilliSource/Core/Resource/ResourceManagerDispenser.h>
#include <ChilliSource/Core/Main/Application.h>

namespace moFlo
{
    namespace Rendering
    {
        DEFINE_NAMED_INTERFACE(CMaterialFactory);
        
        //---------------------------------------------------
        /// Constructor
        //---------------------------------------------------
        CMaterialFactory::CMaterialFactory()
        : mpTextureManager(NULL), mpShaderManager(NULL), mpCubemapManager(NULL), mpRenderCapabilities(NULL)
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
            
            pMaterial->SetShaderProgram(SP_AMBIENT, GetShaderManager()->GetShaderFromFile(Core::SL_PACKAGE, "Core/GUI"));
            pMaterial->SetActiveShaderProgram(SP_AMBIENT);
            pMaterial->SetTransparent(true);
            pMaterial->SetCullFace(CF_FRONT);
            pMaterial->SetCullingEnabled(false);
            
			return MaterialPtr(pMaterial);
        }
        //---------------------------------------------------
        /// Create GUI Distance Font
        //---------------------------------------------------
        MaterialPtr CMaterialFactory::CreateGUIDistanceFont() const
        {
            CMaterial* pMaterial = new CMaterial();
            
            pMaterial->SetShaderProgram(SP_AMBIENT, GetShaderManager()->GetShaderFromFile(Core::SL_PACKAGE, "Core/GUIDistanceFont"));
            pMaterial->SetActiveShaderProgram(SP_AMBIENT);
            pMaterial->SetTransparent(true);
            pMaterial->SetCullFace(CF_FRONT);
            pMaterial->SetCullingEnabled(false);
            
			return MaterialPtr(pMaterial);
        }
        //---------------------------------------------------
        /// Create GUI Distance Font Outlined
        //---------------------------------------------------
        MaterialPtr CMaterialFactory::CreateGUIDistanceFontOutlined() const
        {
            CMaterial* pMaterial = new CMaterial();
            pMaterial->SetShaderProgram(SP_AMBIENT, GetShaderManager()->GetShaderFromFile(Core::SL_PACKAGE, "Core/GUIDistanceFontOutlined"));
            pMaterial->SetActiveShaderProgram(SP_AMBIENT);
            pMaterial->SetTransparent(true);
            pMaterial->SetCullFace(CF_FRONT);
            pMaterial->SetCullingEnabled(false);
			return MaterialPtr(pMaterial);
        }
        //---------------------------------------------------
        /// Create Sprite
        //---------------------------------------------------
        MaterialPtr CMaterialFactory::CreateSprite(const TexturePtr& inpTex) const
        {
            CMaterial* pMaterial = new CMaterial();
            pMaterial->SetShaderProgram(SP_AMBIENT, GetShaderManager()->GetShaderFromFile(Core::SL_PACKAGE, "Core/Sprite"));
            pMaterial->SetActiveShaderProgram(SP_AMBIENT);
            pMaterial->SetTexture(inpTex);
            pMaterial->SetTransparent(false);
            pMaterial->SetCullFace(CF_FRONT);
            pMaterial->SetCullingEnabled(false);
			return MaterialPtr(pMaterial);
        }
        //---------------------------------------------------
        /// Create Static
        //---------------------------------------------------
        MaterialPtr CMaterialFactory::CreateStatic(const TexturePtr& inpTex) const
        {
            CMaterial* pMaterial = new CMaterial();
            pMaterial->SetShaderProgram(SP_AMBIENT, GetShaderManager()->GetShaderFromFile(Core::SL_PACKAGE, "Core/Static"));
            pMaterial->SetActiveShaderProgram(SP_AMBIENT);
            pMaterial->SetTexture(inpTex);
            pMaterial->SetTransparent(false);
            pMaterial->SetCullFace(CF_FRONT);
            pMaterial->SetCullingEnabled(true);
			return MaterialPtr(pMaterial);
        }
        //---------------------------------------------------
        /// Create Static
        //---------------------------------------------------
        MaterialPtr CMaterialFactory::CreateStaticAmbient(const TexturePtr& inpTex) const
        {
            CMaterial* pMaterial = new CMaterial();
            pMaterial->SetShaderProgram(SP_AMBIENT, GetShaderManager()->GetShaderFromFile(Core::SL_PACKAGE, "Core/StaticAmbient"));
            pMaterial->SetActiveShaderProgram(SP_AMBIENT);
            pMaterial->SetTexture(inpTex);
            pMaterial->SetTransparent(false);
            pMaterial->SetCullFace(CF_FRONT);
            pMaterial->SetCullingEnabled(true);
			return MaterialPtr(pMaterial);
        }
        //---------------------------------------------------
        /// Create Static Blinn
        //---------------------------------------------------
        MaterialPtr CMaterialFactory::CreateStaticBlinn(const TexturePtr& inpTex) const
        {
            CMaterial* pMaterial = new CMaterial();
            pMaterial->SetShaderProgram(SP_AMBIENT, GetShaderManager()->GetShaderFromFile(Core::SL_PACKAGE, "Core/StaticAmbient"));
            pMaterial->SetShaderProgram(SP_DIRECTIONAL, GetShaderManager()->GetShaderFromFile(Core::SL_PACKAGE, "Core/StaticBlinnDirectional"));
            pMaterial->SetShaderProgram(SP_POINT, GetShaderManager()->GetShaderFromFile(Core::SL_PACKAGE, "Core/StaticBlinnPoint"));
            pMaterial->SetActiveShaderProgram(SP_AMBIENT);
            pMaterial->SetTexture(inpTex);
            pMaterial->SetTransparent(false);
            pMaterial->SetCullFace(CF_FRONT);
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
            pMaterial->SetShaderProgram(SP_AMBIENT, GetShaderManager()->GetShaderFromFile(Core::SL_PACKAGE, "Core/StaticAmbient"));
            pMaterial->SetShaderProgram(SP_DIRECTIONAL, GetShaderManager()->GetShaderFromFile(Core::SL_PACKAGE, "Core/StaticBlinnShadowedDirectional"));
            pMaterial->SetShaderProgram(SP_POINT, GetShaderManager()->GetShaderFromFile(Core::SL_PACKAGE, "Core/StaticBlinnPoint"));
            pMaterial->SetActiveShaderProgram(SP_AMBIENT);
            pMaterial->SetTexture(inpTex);
            pMaterial->SetTransparent(false);
            pMaterial->SetCullFace(CF_FRONT);
            pMaterial->SetCullingEnabled(true);
			return MaterialPtr(pMaterial);
        }
        //---------------------------------------------------
        /// Create Static Blinn Per Vertex
        //---------------------------------------------------
        MaterialPtr CMaterialFactory::CreateStaticBlinnPerVertex(const TexturePtr& inpTex) const
        {
            CMaterial* pMaterial = new CMaterial();
            pMaterial->SetShaderProgram(SP_AMBIENT, GetShaderManager()->GetShaderFromFile(Core::SL_PACKAGE, "Core/StaticAmbient"));
            pMaterial->SetShaderProgram(SP_DIRECTIONAL, GetShaderManager()->GetShaderFromFile(Core::SL_PACKAGE, "Core/StaticBlinnPerVertexDirectional"));
            pMaterial->SetShaderProgram(SP_POINT, GetShaderManager()->GetShaderFromFile(Core::SL_PACKAGE, "Core/StaticBlinnPerVertexPoint"));
            pMaterial->SetActiveShaderProgram(SP_AMBIENT);
            pMaterial->SetTexture(inpTex);
            pMaterial->SetTransparent(false);
            pMaterial->SetCullFace(CF_FRONT);
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
            pMaterial->SetShaderProgram(SP_AMBIENT, GetShaderManager()->GetShaderFromFile(Core::SL_PACKAGE, "Core/StaticAmbient"));
            pMaterial->SetShaderProgram(SP_DIRECTIONAL, GetShaderManager()->GetShaderFromFile(Core::SL_PACKAGE, "Core/StaticBlinnPerVertexShadowedDirectional"));
            pMaterial->SetShaderProgram(SP_POINT, GetShaderManager()->GetShaderFromFile(Core::SL_PACKAGE, "Core/StaticBlinnPerVertexPoint"));
            pMaterial->SetActiveShaderProgram(SP_AMBIENT);
            pMaterial->SetTexture(inpTex);
            pMaterial->SetTransparent(false);
            pMaterial->SetCullFace(CF_FRONT);
            pMaterial->SetCullingEnabled(true);
			return MaterialPtr(pMaterial);
        }
        //---------------------------------------------------
        /// Create Static Directional Shadow Map
        //---------------------------------------------------
        MaterialPtr CMaterialFactory::CreateStaticDirectionalShadowMap() const
        {
            CMaterial* pMaterial = new CMaterial();
            pMaterial->SetShaderProgram(SP_AMBIENT, GetShaderManager()->GetShaderFromFile(Core::SL_PACKAGE, "Core/StaticDirectionalShadowMap"));
            pMaterial->SetTransparent(false);
            pMaterial->SetCullFace(CF_FRONT);
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
            pMaterial->SetShaderProgram(SP_AMBIENT, GetShaderManager()->GetShaderFromFile(Core::SL_PACKAGE, "Core/Animated"));
            pMaterial->SetActiveShaderProgram(SP_AMBIENT);
            pMaterial->SetTexture(inpTex);
            pMaterial->SetTransparent(false);
            pMaterial->SetCullFace(CF_FRONT);
            pMaterial->SetCullingEnabled(true);
			return MaterialPtr(pMaterial);
        }
        //---------------------------------------------------
        /// Create Animated Ambient
        //---------------------------------------------------
        MaterialPtr CMaterialFactory::CreateAnimatedAmbient(const TexturePtr& inpTex) const
        {
            CMaterial* pMaterial = new CMaterial();
            pMaterial->SetShaderProgram(SP_AMBIENT, GetShaderManager()->GetShaderFromFile(Core::SL_PACKAGE, "Core/AnimatedAmbient"));
            pMaterial->SetActiveShaderProgram(SP_AMBIENT);
            pMaterial->SetTexture(inpTex);
            pMaterial->SetTransparent(false);
            pMaterial->SetCullFace(CF_FRONT);
            pMaterial->SetCullingEnabled(true);
			return MaterialPtr(pMaterial);
        }
        //---------------------------------------------------
        /// Create Animated Blinn
        //---------------------------------------------------
        MaterialPtr CMaterialFactory::CreateAnimatedBlinn(const TexturePtr& inpTex) const
        {
            CMaterial* pMaterial = new CMaterial();
            pMaterial->SetShaderProgram(SP_AMBIENT, GetShaderManager()->GetShaderFromFile(Core::SL_PACKAGE, "Core/AnimatedAmbient"));
            pMaterial->SetShaderProgram(SP_DIRECTIONAL, GetShaderManager()->GetShaderFromFile(Core::SL_PACKAGE, "Core/AnimatedBlinnDirectional"));
            pMaterial->SetShaderProgram(SP_POINT, GetShaderManager()->GetShaderFromFile(Core::SL_PACKAGE, "Core/AnimatedBlinnPoint"));
            pMaterial->SetActiveShaderProgram(SP_AMBIENT);
            pMaterial->SetTexture(inpTex);
            pMaterial->SetTransparent(false);
            pMaterial->SetCullFace(CF_FRONT);
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
            pMaterial->SetShaderProgram(SP_AMBIENT, GetShaderManager()->GetShaderFromFile(Core::SL_PACKAGE, "Core/AnimatedAmbient"));
            pMaterial->SetShaderProgram(SP_DIRECTIONAL, GetShaderManager()->GetShaderFromFile(Core::SL_PACKAGE, "Core/AnimatedBlinnShadowedDirectional"));
            pMaterial->SetShaderProgram(SP_POINT, GetShaderManager()->GetShaderFromFile(Core::SL_PACKAGE, "Core/AnimatedBlinnPoint"));
            pMaterial->SetActiveShaderProgram(SP_AMBIENT);
            pMaterial->SetTexture(inpTex);
            pMaterial->SetTransparent(false);
            pMaterial->SetCullFace(CF_FRONT);
            pMaterial->SetCullingEnabled(true);
			return MaterialPtr(pMaterial);
        }
        //---------------------------------------------------
        /// Create Animated Blinn Per Vertex
        //---------------------------------------------------
        MaterialPtr CMaterialFactory::CreateAnimatedBlinnPerVertex(const TexturePtr& inpTex) const
        {
            CMaterial* pMaterial = new CMaterial();
            pMaterial->SetShaderProgram(SP_AMBIENT, GetShaderManager()->GetShaderFromFile(Core::SL_PACKAGE, "Core/AnimatedAmbient"));
            pMaterial->SetShaderProgram(SP_DIRECTIONAL, GetShaderManager()->GetShaderFromFile(Core::SL_PACKAGE, "Core/AnimatedBlinnPerVertexDirectional"));
            pMaterial->SetShaderProgram(SP_POINT, GetShaderManager()->GetShaderFromFile(Core::SL_PACKAGE, "Core/AnimatedBlinnPerVertexPoint"));
            pMaterial->SetActiveShaderProgram(SP_AMBIENT);
            pMaterial->SetTexture(inpTex);
            pMaterial->SetTransparent(false);
            pMaterial->SetCullFace(CF_FRONT);
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
            pMaterial->SetShaderProgram(SP_AMBIENT, GetShaderManager()->GetShaderFromFile(Core::SL_PACKAGE, "Core/AnimatedAmbient"));
            pMaterial->SetShaderProgram(SP_DIRECTIONAL, GetShaderManager()->GetShaderFromFile(Core::SL_PACKAGE, "Core/AnimatedBlinnPerVertexShadowedDirectional"));
            pMaterial->SetShaderProgram(SP_POINT, GetShaderManager()->GetShaderFromFile(Core::SL_PACKAGE, "Core/AnimatedBlinnPerVertexPoint"));
            pMaterial->SetActiveShaderProgram(SP_AMBIENT);
            pMaterial->SetTexture(inpTex);
            pMaterial->SetTransparent(false);
            pMaterial->SetCullFace(CF_FRONT);
            pMaterial->SetCullingEnabled(true);
			return MaterialPtr(pMaterial);
        }
        //---------------------------------------------------
        /// Create Animated Directional Shadow Map
        //---------------------------------------------------
        MaterialPtr CMaterialFactory::CreateAnimatedDirectionalShadowMap() const
        {
            CMaterial* pMaterial = new CMaterial();
            pMaterial->SetShaderProgram(SP_AMBIENT, GetShaderManager()->GetShaderFromFile(Core::SL_PACKAGE, "Core/AnimatedDirectionalShadowMap"));
            pMaterial->SetTransparent(false);
            pMaterial->SetCullFace(CF_FRONT);
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
            if(mpTextureManager == NULL)
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
            if(mpShaderManager == NULL)
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
            if(mpCubemapManager == NULL)
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
        	if(mpRenderCapabilities == NULL)
			{
        		mpRenderCapabilities = Core::CApplication::GetSystemImplementing<IRenderCapabilities>();
        		MOFLOW_ASSERT(mpRenderCapabilities, "Material Factor is missing required system: Render Capabilities.");
			}

			return mpRenderCapabilities;
        }
    }
}
