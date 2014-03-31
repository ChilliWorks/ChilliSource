//
//  MaterialFactory.cpp
//  Chilli Source
//
//  Created by Scott Downie on 05/08/2013.
//  Copyright (c) 2013 Tag Games Ltd. All rights reserved.
//

#include <ChilliSource/Rendering/Material/MaterialFactory.h>

#include <ChilliSource/Rendering/Base/RenderCapabilities.h>
#include <ChilliSource/Rendering/Material/Material.h>
#include <ChilliSource/Rendering/Shader/Shader.h>
#include <ChilliSource/Rendering/Shader/ShaderManager.h>

namespace ChilliSource
{
    namespace Rendering
    {
        CS_DEFINE_NAMEDTYPE(MaterialFactory);
        
        //-------------------------------------------------------
        //-------------------------------------------------------
        MaterialFactoryUPtr MaterialFactory::Create(ShaderManager* in_shaderMgr, RenderCapabilities* in_renderCapabilities)
        {
            return MaterialFactoryUPtr(new MaterialFactory(in_shaderMgr, in_renderCapabilities));
        }
        //---------------------------------------------------
        //---------------------------------------------------
        MaterialFactory::MaterialFactory(ShaderManager* in_shaderMgr, RenderCapabilities* in_renderCapabilities)
        : m_shaderManager(in_shaderMgr), m_renderCapabilities(in_renderCapabilities)
        {
            CS_ASSERT(m_shaderManager, "Must have shader manager");
            CS_ASSERT(m_renderCapabilities, "Must have render caps.");
        }
        //---------------------------------------------------
        //---------------------------------------------------
        bool MaterialFactory::IsA(Core::InterfaceIDType in_interfaceID) const
        {
            return in_interfaceID == MaterialFactory::InterfaceID;
        }
        //---------------------------------------------------
        //---------------------------------------------------
        MaterialUPtr MaterialFactory::CreateCustom() const
        {
            Material* material = new Material();
			return MaterialUPtr(material);
        }
        
        //---------------------------------------------------
        //---------------------------------------------------
        MaterialUPtr MaterialFactory::CreateGUI() const
        {
            Material* material = new Material();
            
            material->SetShaderProgram(ShaderPass::k_ambient, m_shaderManager->GetShaderFromFile(Core::StorageLocation::k_package, "Core/GUI"));
            material->SetActiveShaderProgram(ShaderPass::k_ambient);
            material->SetTransparent(true);
            material->SetCullFace(CullFace::k_front);
            material->SetCullingEnabled(false);
            
			return MaterialUPtr(material);
        }
        //---------------------------------------------------
        //---------------------------------------------------
        MaterialUPtr MaterialFactory::CreateGUIDistanceFont() const
        {
            Material* material = new Material();
            
            material->SetShaderProgram(ShaderPass::k_ambient, m_shaderManager->GetShaderFromFile(Core::StorageLocation::k_package, "Core/GUIDistanceFont"));
            material->SetActiveShaderProgram(ShaderPass::k_ambient);
            material->SetTransparent(true);
            material->SetCullFace(CullFace::k_front);
            material->SetCullingEnabled(false);
            
			return MaterialUPtr(material);
        }
        //---------------------------------------------------
        //---------------------------------------------------
        MaterialUPtr MaterialFactory::CreateGUIDistanceFontOutlined() const
        {
            Material* material = new Material();
            material->SetShaderProgram(ShaderPass::k_ambient, m_shaderManager->GetShaderFromFile(Core::StorageLocation::k_package, "Core/GUIDistanceFontOutlined"));
            material->SetActiveShaderProgram(ShaderPass::k_ambient);
            material->SetTransparent(true);
            material->SetCullFace(CullFace::k_front);
            material->SetCullingEnabled(false);
			return MaterialUPtr(material);
        }
        //---------------------------------------------------
        //---------------------------------------------------
        MaterialUPtr MaterialFactory::CreateSprite(const TextureSPtr& in_texture) const
        {
            Material* material = new Material();
            material->SetShaderProgram(ShaderPass::k_ambient, m_shaderManager->GetShaderFromFile(Core::StorageLocation::k_package, "Core/Sprite"));
            material->SetActiveShaderProgram(ShaderPass::k_ambient);
            material->SetTexture(in_texture);
            material->SetTransparent(false);
            material->SetCullFace(CullFace::k_front);
            material->SetCullingEnabled(false);
			return MaterialUPtr(material);
        }
        //---------------------------------------------------
        //---------------------------------------------------
        MaterialUPtr MaterialFactory::CreateStatic(const TextureSPtr& in_texture) const
        {
            Material* material = new Material();
            material->SetShaderProgram(ShaderPass::k_ambient, m_shaderManager->GetShaderFromFile(Core::StorageLocation::k_package, "Core/Static"));
            material->SetActiveShaderProgram(ShaderPass::k_ambient);
            material->SetTexture(in_texture);
            material->SetTransparent(false);
            material->SetCullFace(CullFace::k_front);
            material->SetCullingEnabled(true);
			return MaterialUPtr(material);
        }
        //---------------------------------------------------
        //---------------------------------------------------
        MaterialUPtr MaterialFactory::CreateStaticAmbient(const TextureSPtr& in_texture) const
        {
            Material* material = new Material();
            material->SetShaderProgram(ShaderPass::k_ambient, m_shaderManager->GetShaderFromFile(Core::StorageLocation::k_package, "Core/StaticAmbient"));
            material->SetActiveShaderProgram(ShaderPass::k_ambient);
            material->SetTexture(in_texture);
            material->SetTransparent(false);
            material->SetCullFace(CullFace::k_front);
            material->SetCullingEnabled(true);
			return MaterialUPtr(material);
        }
        //---------------------------------------------------
        //---------------------------------------------------
        MaterialUPtr MaterialFactory::CreateStaticBlinn(const TextureSPtr& in_texture) const
        {
            Material* material = new Material();
            material->SetShaderProgram(ShaderPass::k_ambient, m_shaderManager->GetShaderFromFile(Core::StorageLocation::k_package, "Core/StaticAmbient"));
            material->SetShaderProgram(ShaderPass::k_directional, m_shaderManager->GetShaderFromFile(Core::StorageLocation::k_package, "Core/StaticBlinnDirectional"));
            material->SetShaderProgram(ShaderPass::k_point, m_shaderManager->GetShaderFromFile(Core::StorageLocation::k_package, "Core/StaticBlinnPoint"));
            material->SetActiveShaderProgram(ShaderPass::k_ambient);
            material->SetTexture(in_texture);
            material->SetTransparent(false);
            material->SetCullFace(CullFace::k_front);
            material->SetCullingEnabled(true);
			return MaterialUPtr(material);
        }
        //---------------------------------------------------
        //---------------------------------------------------
        MaterialUPtr MaterialFactory::CreateStaticBlinnShadowed(const TextureSPtr& in_texture) const
        {
        	if (m_renderCapabilities->IsShadowMappingSupported() == false)
			{
				return CreateStaticBlinn(in_texture);
			}

            Material* material = new Material();
            material->SetShaderProgram(ShaderPass::k_ambient, m_shaderManager->GetShaderFromFile(Core::StorageLocation::k_package, "Core/StaticAmbient"));
            material->SetShaderProgram(ShaderPass::k_directional, m_shaderManager->GetShaderFromFile(Core::StorageLocation::k_package, "Core/StaticBlinnShadowedDirectional"));
            material->SetShaderProgram(ShaderPass::k_point, m_shaderManager->GetShaderFromFile(Core::StorageLocation::k_package, "Core/StaticBlinnPoint"));
            material->SetActiveShaderProgram(ShaderPass::k_ambient);
            material->SetTexture(in_texture);
            material->SetTransparent(false);
            material->SetCullFace(CullFace::k_front);
            material->SetCullingEnabled(true);
			return MaterialUPtr(material);
        }
        //---------------------------------------------------
        //---------------------------------------------------
        MaterialUPtr MaterialFactory::CreateStaticBlinnPerVertex(const TextureSPtr& in_texture) const
        {
            Material* material = new Material();
            material->SetShaderProgram(ShaderPass::k_ambient, m_shaderManager->GetShaderFromFile(Core::StorageLocation::k_package, "Core/StaticAmbient"));
            material->SetShaderProgram(ShaderPass::k_directional, m_shaderManager->GetShaderFromFile(Core::StorageLocation::k_package, "Core/StaticBlinnPerVertexDirectional"));
            material->SetShaderProgram(ShaderPass::k_point, m_shaderManager->GetShaderFromFile(Core::StorageLocation::k_package, "Core/StaticBlinnPerVertexPoint"));
            material->SetActiveShaderProgram(ShaderPass::k_ambient);
            material->SetTexture(in_texture);
            material->SetTransparent(false);
            material->SetCullFace(CullFace::k_front);
            material->SetCullingEnabled(true);
			return MaterialUPtr(material);
        }
        //---------------------------------------------------
        //---------------------------------------------------
        MaterialUPtr MaterialFactory::CreateStaticBlinnPerVertexShadowed(const TextureSPtr& in_texture) const
        {
        	if (m_renderCapabilities->IsShadowMappingSupported() == false)
			{
				return CreateStaticBlinnPerVertex(in_texture);
			}

            Material* material = new Material();
            material->SetShaderProgram(ShaderPass::k_ambient, m_shaderManager->GetShaderFromFile(Core::StorageLocation::k_package, "Core/StaticAmbient"));
            material->SetShaderProgram(ShaderPass::k_directional, m_shaderManager->GetShaderFromFile(Core::StorageLocation::k_package, "Core/StaticBlinnPerVertexShadowedDirectional"));
            material->SetShaderProgram(ShaderPass::k_point, m_shaderManager->GetShaderFromFile(Core::StorageLocation::k_package, "Core/StaticBlinnPerVertexPoint"));
            material->SetActiveShaderProgram(ShaderPass::k_ambient);
            material->SetTexture(in_texture);
            material->SetTransparent(false);
            material->SetCullFace(CullFace::k_front);
            material->SetCullingEnabled(true);
			return MaterialUPtr(material);
        }
        //---------------------------------------------------
        //---------------------------------------------------
        MaterialUPtr MaterialFactory::CreateStaticDirectionalShadowMap() const
        {
            Material* material = new Material();
            material->SetShaderProgram(ShaderPass::k_ambient, m_shaderManager->GetShaderFromFile(Core::StorageLocation::k_package, "Core/StaticDirectionalShadowMap"));
            material->SetTransparent(false);
            material->SetCullFace(CullFace::k_front);
            material->SetCullingEnabled(true);
            
#ifdef CS_ENABLE_DEBUGSHADOW
            material->SetColourWriteEnabled(true);
#else
            material->SetColourWriteEnabled(false);
#endif
            
			return MaterialUPtr(material);
        }
        //---------------------------------------------------
        //---------------------------------------------------
        MaterialUPtr MaterialFactory::CreateAnimated(const TextureSPtr& in_texture) const
        {
            Material* material = new Material();
            material->SetShaderProgram(ShaderPass::k_ambient, m_shaderManager->GetShaderFromFile(Core::StorageLocation::k_package, "Core/Animated"));
            material->SetActiveShaderProgram(ShaderPass::k_ambient);
            material->SetTexture(in_texture);
            material->SetTransparent(false);
            material->SetCullFace(CullFace::k_front);
            material->SetCullingEnabled(true);
			return MaterialUPtr(material);
        }
        //---------------------------------------------------
        //---------------------------------------------------
        MaterialUPtr MaterialFactory::CreateAnimatedAmbient(const TextureSPtr& in_texture) const
        {
            Material* material = new Material();
            material->SetShaderProgram(ShaderPass::k_ambient, m_shaderManager->GetShaderFromFile(Core::StorageLocation::k_package, "Core/AnimatedAmbient"));
            material->SetActiveShaderProgram(ShaderPass::k_ambient);
            material->SetTexture(in_texture);
            material->SetTransparent(false);
            material->SetCullFace(CullFace::k_front);
            material->SetCullingEnabled(true);
			return MaterialUPtr(material);
        }
        //---------------------------------------------------
        //---------------------------------------------------
        MaterialUPtr MaterialFactory::CreateAnimatedBlinn(const TextureSPtr& in_texture) const
        {
            Material* material = new Material();
            material->SetShaderProgram(ShaderPass::k_ambient, m_shaderManager->GetShaderFromFile(Core::StorageLocation::k_package, "Core/AnimatedAmbient"));
            material->SetShaderProgram(ShaderPass::k_directional, m_shaderManager->GetShaderFromFile(Core::StorageLocation::k_package, "Core/AnimatedBlinnDirectional"));
            material->SetShaderProgram(ShaderPass::k_point, m_shaderManager->GetShaderFromFile(Core::StorageLocation::k_package, "Core/AnimatedBlinnPoint"));
            material->SetActiveShaderProgram(ShaderPass::k_ambient);
            material->SetTexture(in_texture);
            material->SetTransparent(false);
            material->SetCullFace(CullFace::k_front);
            material->SetCullingEnabled(true);
			return MaterialUPtr(material);
        }
        //---------------------------------------------------
        //---------------------------------------------------
        MaterialUPtr MaterialFactory::CreateAnimatedBlinnShadowed(const TextureSPtr& in_texture) const
        {
        	if (m_renderCapabilities->IsShadowMappingSupported() == false)
			{
				return CreateAnimatedBlinn(in_texture);
			}

            Material* material = new Material();
            material->SetShaderProgram(ShaderPass::k_ambient, m_shaderManager->GetShaderFromFile(Core::StorageLocation::k_package, "Core/AnimatedAmbient"));
            material->SetShaderProgram(ShaderPass::k_directional, m_shaderManager->GetShaderFromFile(Core::StorageLocation::k_package, "Core/AnimatedBlinnShadowedDirectional"));
            material->SetShaderProgram(ShaderPass::k_point, m_shaderManager->GetShaderFromFile(Core::StorageLocation::k_package, "Core/AnimatedBlinnPoint"));
            material->SetActiveShaderProgram(ShaderPass::k_ambient);
            material->SetTexture(in_texture);
            material->SetTransparent(false);
            material->SetCullFace(CullFace::k_front);
            material->SetCullingEnabled(true);
			return MaterialUPtr(material);
        }
        //---------------------------------------------------
        //---------------------------------------------------
        MaterialUPtr MaterialFactory::CreateAnimatedBlinnPerVertex(const TextureSPtr& in_texture) const
        {
            Material* material = new Material();
            material->SetShaderProgram(ShaderPass::k_ambient, m_shaderManager->GetShaderFromFile(Core::StorageLocation::k_package, "Core/AnimatedAmbient"));
            material->SetShaderProgram(ShaderPass::k_directional, m_shaderManager->GetShaderFromFile(Core::StorageLocation::k_package, "Core/AnimatedBlinnPerVertexDirectional"));
            material->SetShaderProgram(ShaderPass::k_point, m_shaderManager->GetShaderFromFile(Core::StorageLocation::k_package, "Core/AnimatedBlinnPerVertexPoint"));
            material->SetActiveShaderProgram(ShaderPass::k_ambient);
            material->SetTexture(in_texture);
            material->SetTransparent(false);
            material->SetCullFace(CullFace::k_front);
            material->SetCullingEnabled(true);
			return MaterialUPtr(material);
        }
        //---------------------------------------------------
        //---------------------------------------------------
        MaterialUPtr MaterialFactory::CreateAnimatedBlinnPerVertexShadowed(const TextureSPtr& in_texture) const
        {
        	if (m_renderCapabilities->IsShadowMappingSupported() == false)
        	{
        		return CreateAnimatedBlinnPerVertex(in_texture);
        	}

            Material* material = new Material();
            material->SetShaderProgram(ShaderPass::k_ambient, m_shaderManager->GetShaderFromFile(Core::StorageLocation::k_package, "Core/AnimatedAmbient"));
            material->SetShaderProgram(ShaderPass::k_directional, m_shaderManager->GetShaderFromFile(Core::StorageLocation::k_package, "Core/AnimatedBlinnPerVertexShadowedDirectional"));
            material->SetShaderProgram(ShaderPass::k_point, m_shaderManager->GetShaderFromFile(Core::StorageLocation::k_package, "Core/AnimatedBlinnPerVertexPoint"));
            material->SetActiveShaderProgram(ShaderPass::k_ambient);
            material->SetTexture(in_texture);
            material->SetTransparent(false);
            material->SetCullFace(CullFace::k_front);
            material->SetCullingEnabled(true);
			return MaterialUPtr(material);
        }
        //---------------------------------------------------
        //---------------------------------------------------
        MaterialUPtr MaterialFactory::CreateAnimatedDirectionalShadowMap() const
        {
            Material* material = new Material();
            material->SetShaderProgram(ShaderPass::k_ambient, m_shaderManager->GetShaderFromFile(Core::StorageLocation::k_package, "Core/AnimatedDirectionalShadowMap"));
            material->SetTransparent(false);
            material->SetCullFace(CullFace::k_front);
            material->SetCullingEnabled(true);
            
#ifdef CS_ENABLE_DEBUGSHADOW
            material->SetColourWriteEnabled(true);
#else
            material->SetColourWriteEnabled(false);
#endif
            
			return MaterialUPtr(material);
        }
    }
}
