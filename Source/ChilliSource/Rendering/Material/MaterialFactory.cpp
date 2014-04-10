//
//  MaterialFactory.cpp
//  Chilli Source
//
//  Created by Scott Downie on 05/08/2013.
//  Copyright (c) 2013 Tag Games Ltd. All rights reserved.
//

#include <ChilliSource/Rendering/Material/MaterialFactory.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Resource/ResourcePool.h>
#include <ChilliSource/Rendering/Base/CullFace.h>
#include <ChilliSource/Rendering/Base/RenderCapabilities.h>
#include <ChilliSource/Rendering/Base/ShaderPass.h>
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
        bool MaterialFactory::IsA(Core::InterfaceIDType in_interfaceId) const
        {
            return in_interfaceId == MaterialFactory::InterfaceID;
        }
        //---------------------------------------------------
        //---------------------------------------------------
        MaterialSPtr MaterialFactory::CreateCustom(const std::string& in_uniqueId) const
        {
            Core::ResourcePool* resourcePool = Core::Application::Get()->GetResourcePool();
            return resourcePool->CreateResource<Material>(in_uniqueId);
        }
        //---------------------------------------------------
        //---------------------------------------------------
        MaterialSPtr MaterialFactory::CreateGUI(const std::string& in_uniqueId) const
        {
            MaterialSPtr material(CreateCustom(in_uniqueId));
            
            material->SetShader(ShaderPass::k_ambient, m_shaderManager->GetShaderFromFile(Core::StorageLocation::k_package, "Core/GUI"));
            material->SetTransparencyEnabled(true);
            material->SetCullFace(CullFace::k_front);
            material->SetFaceCullingEnabled(false);
            
			return material;
        }
        //---------------------------------------------------
        //---------------------------------------------------
        MaterialSPtr MaterialFactory::CreateGUIDistanceFont(const std::string& in_uniqueId) const
        {
            MaterialSPtr material(CreateCustom(in_uniqueId));
            
            material->SetShader(ShaderPass::k_ambient, m_shaderManager->GetShaderFromFile(Core::StorageLocation::k_package, "Core/GUIDistanceFont"));
            material->SetTransparencyEnabled(true);
            material->SetCullFace(CullFace::k_front);
            material->SetFaceCullingEnabled(false);
            
			return material;
        }
        //---------------------------------------------------
        //---------------------------------------------------
        MaterialSPtr MaterialFactory::CreateSprite(const std::string& in_uniqueId, const TextureSPtr& in_texture) const
        {
            MaterialSPtr material(CreateCustom(in_uniqueId));
            material->SetShader(ShaderPass::k_ambient, m_shaderManager->GetShaderFromFile(Core::StorageLocation::k_package, "Core/Sprite"));
            material->AddTexture(in_texture);
            material->SetTransparencyEnabled(false);
            material->SetCullFace(CullFace::k_front);
            material->SetFaceCullingEnabled(false);
			return material;
        }
        //---------------------------------------------------
        //---------------------------------------------------
        MaterialSPtr MaterialFactory::CreateStatic(const std::string& in_uniqueId, const TextureSPtr& in_texture) const
        {
            MaterialSPtr material(CreateCustom(in_uniqueId));
            material->SetShader(ShaderPass::k_ambient, m_shaderManager->GetShaderFromFile(Core::StorageLocation::k_package, "Core/Static"));
            material->AddTexture(in_texture);
            material->SetTransparencyEnabled(false);
            material->SetCullFace(CullFace::k_front);
            material->SetFaceCullingEnabled(true);
			return material;
        }
        //---------------------------------------------------
        //---------------------------------------------------
        MaterialSPtr MaterialFactory::CreateStaticAmbient(const std::string& in_uniqueId, const TextureSPtr& in_texture) const
        {
            MaterialSPtr material(CreateCustom(in_uniqueId));
            material->SetShader(ShaderPass::k_ambient, m_shaderManager->GetShaderFromFile(Core::StorageLocation::k_package, "Core/StaticAmbient"));
            material->AddTexture(in_texture);
            material->SetTransparencyEnabled(false);
            material->SetCullFace(CullFace::k_front);
            material->SetFaceCullingEnabled(true);
			return material;
        }
        //---------------------------------------------------
        //---------------------------------------------------
        MaterialSPtr MaterialFactory::CreateStaticBlinn(const std::string& in_uniqueId, const TextureSPtr& in_texture) const
        {
            MaterialSPtr material(CreateCustom(in_uniqueId));
            material->SetShader(ShaderPass::k_ambient, m_shaderManager->GetShaderFromFile(Core::StorageLocation::k_package, "Core/StaticAmbient"));
            material->SetShader(ShaderPass::k_directional, m_shaderManager->GetShaderFromFile(Core::StorageLocation::k_package, "Core/StaticBlinnDirectional"));
            material->SetShader(ShaderPass::k_point, m_shaderManager->GetShaderFromFile(Core::StorageLocation::k_package, "Core/StaticBlinnPoint"));
            material->AddTexture(in_texture);
            material->SetTransparencyEnabled(false);
            material->SetCullFace(CullFace::k_front);
            material->SetFaceCullingEnabled(true);
			return material;
        }
        //---------------------------------------------------
        //---------------------------------------------------
        MaterialSPtr MaterialFactory::CreateStaticBlinnShadowed(const std::string& in_uniqueId, const TextureSPtr& in_texture) const
        {
        	if (m_renderCapabilities->IsShadowMappingSupported() == false)
			{
				return CreateStaticBlinn(in_uniqueId, in_texture);
			}

            MaterialSPtr material(CreateCustom(in_uniqueId));
            material->SetShader(ShaderPass::k_ambient, m_shaderManager->GetShaderFromFile(Core::StorageLocation::k_package, "Core/StaticAmbient"));
            material->SetShader(ShaderPass::k_directional, m_shaderManager->GetShaderFromFile(Core::StorageLocation::k_package, "Core/StaticBlinnShadowedDirectional"));
            material->SetShader(ShaderPass::k_point, m_shaderManager->GetShaderFromFile(Core::StorageLocation::k_package, "Core/StaticBlinnPoint"));
            material->AddTexture(in_texture);
            material->SetTransparencyEnabled(false);
            material->SetCullFace(CullFace::k_front);
            material->SetFaceCullingEnabled(true);
			return material;
        }
        //---------------------------------------------------
        //---------------------------------------------------
        MaterialSPtr MaterialFactory::CreateStaticBlinnPerVertex(const std::string& in_uniqueId, const TextureSPtr& in_texture) const
        {
            MaterialSPtr material(CreateCustom(in_uniqueId));
            material->SetShader(ShaderPass::k_ambient, m_shaderManager->GetShaderFromFile(Core::StorageLocation::k_package, "Core/StaticAmbient"));
            material->SetShader(ShaderPass::k_directional, m_shaderManager->GetShaderFromFile(Core::StorageLocation::k_package, "Core/StaticBlinnPerVertexDirectional"));
            material->SetShader(ShaderPass::k_point, m_shaderManager->GetShaderFromFile(Core::StorageLocation::k_package, "Core/StaticBlinnPerVertexPoint"));
            material->AddTexture(in_texture);
            material->SetTransparencyEnabled(false);
            material->SetCullFace(CullFace::k_front);
            material->SetFaceCullingEnabled(true);
			return material;
        }
        //---------------------------------------------------
        //---------------------------------------------------
        MaterialSPtr MaterialFactory::CreateStaticBlinnPerVertexShadowed(const std::string& in_uniqueId, const TextureSPtr& in_texture) const
        {
        	if (m_renderCapabilities->IsShadowMappingSupported() == false)
			{
				return CreateStaticBlinnPerVertex(in_uniqueId, in_texture);
			}

            MaterialSPtr material(CreateCustom(in_uniqueId));
            material->SetShader(ShaderPass::k_ambient, m_shaderManager->GetShaderFromFile(Core::StorageLocation::k_package, "Core/StaticAmbient"));
            material->SetShader(ShaderPass::k_directional, m_shaderManager->GetShaderFromFile(Core::StorageLocation::k_package, "Core/StaticBlinnPerVertexShadowedDirectional"));
            material->SetShader(ShaderPass::k_point, m_shaderManager->GetShaderFromFile(Core::StorageLocation::k_package, "Core/StaticBlinnPerVertexPoint"));
            material->AddTexture(in_texture);
            material->SetTransparencyEnabled(false);
            material->SetCullFace(CullFace::k_front);
            material->SetFaceCullingEnabled(true);
			return material;
        }
        //---------------------------------------------------
        //---------------------------------------------------
        MaterialSPtr MaterialFactory::CreateStaticDirectionalShadowMap(const std::string& in_uniqueId) const
        {
            MaterialSPtr material(CreateCustom(in_uniqueId));
            material->SetShader(ShaderPass::k_ambient, m_shaderManager->GetShaderFromFile(Core::StorageLocation::k_package, "Core/StaticDirectionalShadowMap"));
            material->SetTransparencyEnabled(false);
            material->SetCullFace(CullFace::k_front);
            material->SetFaceCullingEnabled(true);
            
#ifdef CS_ENABLE_DEBUGSHADOW
            material->SetColourWriteEnabled(true);
#else
            material->SetColourWriteEnabled(false);
#endif
            
			return material;
        }
        //---------------------------------------------------
        //---------------------------------------------------
        MaterialSPtr MaterialFactory::CreateAnimated(const std::string& in_uniqueId, const TextureSPtr& in_texture) const
        {
            MaterialSPtr material(CreateCustom(in_uniqueId));
            material->SetShader(ShaderPass::k_ambient, m_shaderManager->GetShaderFromFile(Core::StorageLocation::k_package, "Core/Animated"));
            material->AddTexture(in_texture);
            material->SetTransparencyEnabled(false);
            material->SetCullFace(CullFace::k_front);
            material->SetFaceCullingEnabled(true);
			return material;
        }
        //---------------------------------------------------
        //---------------------------------------------------
        MaterialSPtr MaterialFactory::CreateAnimatedAmbient(const std::string& in_uniqueId, const TextureSPtr& in_texture) const
        {
            MaterialSPtr material(CreateCustom(in_uniqueId));
            material->SetShader(ShaderPass::k_ambient, m_shaderManager->GetShaderFromFile(Core::StorageLocation::k_package, "Core/AnimatedAmbient"));
            material->AddTexture(in_texture);
            material->SetTransparencyEnabled(false);
            material->SetCullFace(CullFace::k_front);
            material->SetFaceCullingEnabled(true);
			return material;
        }
        //---------------------------------------------------
        //---------------------------------------------------
        MaterialSPtr MaterialFactory::CreateAnimatedBlinn(const std::string& in_uniqueId, const TextureSPtr& in_texture) const
        {
            MaterialSPtr material(CreateCustom(in_uniqueId));
            material->SetShader(ShaderPass::k_ambient, m_shaderManager->GetShaderFromFile(Core::StorageLocation::k_package, "Core/AnimatedAmbient"));
            material->SetShader(ShaderPass::k_directional, m_shaderManager->GetShaderFromFile(Core::StorageLocation::k_package, "Core/AnimatedBlinnDirectional"));
            material->SetShader(ShaderPass::k_point, m_shaderManager->GetShaderFromFile(Core::StorageLocation::k_package, "Core/AnimatedBlinnPoint"));
            material->AddTexture(in_texture);
            material->SetTransparencyEnabled(false);
            material->SetCullFace(CullFace::k_front);
            material->SetFaceCullingEnabled(true);
			return material;
        }
        //---------------------------------------------------
        //---------------------------------------------------
        MaterialSPtr MaterialFactory::CreateAnimatedBlinnShadowed(const std::string& in_uniqueId, const TextureSPtr& in_texture) const
        {
        	if (m_renderCapabilities->IsShadowMappingSupported() == false)
			{
				return CreateAnimatedBlinn(in_uniqueId, in_texture);
			}

            MaterialSPtr material(CreateCustom(in_uniqueId));
            material->SetShader(ShaderPass::k_ambient, m_shaderManager->GetShaderFromFile(Core::StorageLocation::k_package, "Core/AnimatedAmbient"));
            material->SetShader(ShaderPass::k_directional, m_shaderManager->GetShaderFromFile(Core::StorageLocation::k_package, "Core/AnimatedBlinnShadowedDirectional"));
            material->SetShader(ShaderPass::k_point, m_shaderManager->GetShaderFromFile(Core::StorageLocation::k_package, "Core/AnimatedBlinnPoint"));
            material->AddTexture(in_texture);
            material->SetTransparencyEnabled(false);
            material->SetCullFace(CullFace::k_front);
            material->SetFaceCullingEnabled(true);
			return material;
        }
        //---------------------------------------------------
        //---------------------------------------------------
        MaterialSPtr MaterialFactory::CreateAnimatedBlinnPerVertex(const std::string& in_uniqueId, const TextureSPtr& in_texture) const
        {
            MaterialSPtr material(CreateCustom(in_uniqueId));
            material->SetShader(ShaderPass::k_ambient, m_shaderManager->GetShaderFromFile(Core::StorageLocation::k_package, "Core/AnimatedAmbient"));
            material->SetShader(ShaderPass::k_directional, m_shaderManager->GetShaderFromFile(Core::StorageLocation::k_package, "Core/AnimatedBlinnPerVertexDirectional"));
            material->SetShader(ShaderPass::k_point, m_shaderManager->GetShaderFromFile(Core::StorageLocation::k_package, "Core/AnimatedBlinnPerVertexPoint"));
            material->AddTexture(in_texture);
            material->SetTransparencyEnabled(false);
            material->SetCullFace(CullFace::k_front);
            material->SetFaceCullingEnabled(true);
			return material;
        }
        //---------------------------------------------------
        //---------------------------------------------------
        MaterialSPtr MaterialFactory::CreateAnimatedBlinnPerVertexShadowed(const std::string& in_uniqueId, const TextureSPtr& in_texture) const
        {
        	if (m_renderCapabilities->IsShadowMappingSupported() == false)
        	{
        		return CreateAnimatedBlinnPerVertex(in_uniqueId, in_texture);
        	}

            MaterialSPtr material(CreateCustom(in_uniqueId));
            material->SetShader(ShaderPass::k_ambient, m_shaderManager->GetShaderFromFile(Core::StorageLocation::k_package, "Core/AnimatedAmbient"));
            material->SetShader(ShaderPass::k_directional, m_shaderManager->GetShaderFromFile(Core::StorageLocation::k_package, "Core/AnimatedBlinnPerVertexShadowedDirectional"));
            material->SetShader(ShaderPass::k_point, m_shaderManager->GetShaderFromFile(Core::StorageLocation::k_package, "Core/AnimatedBlinnPerVertexPoint"));
            material->AddTexture(in_texture);
            material->SetTransparencyEnabled(false);
            material->SetCullFace(CullFace::k_front);
            material->SetFaceCullingEnabled(true);
			return material;
        }
        //---------------------------------------------------
        //---------------------------------------------------
        MaterialSPtr MaterialFactory::CreateAnimatedDirectionalShadowMap(const std::string& in_uniqueId) const
        {
            MaterialSPtr material(CreateCustom(in_uniqueId));
            material->SetShader(ShaderPass::k_ambient, m_shaderManager->GetShaderFromFile(Core::StorageLocation::k_package, "Core/AnimatedDirectionalShadowMap"));
            material->SetTransparencyEnabled(false);
            material->SetCullFace(CullFace::k_front);
            material->SetFaceCullingEnabled(true);
            
#ifdef CS_ENABLE_DEBUGSHADOW
            material->SetColourWriteEnabled(true);
#else
            material->SetColourWriteEnabled(false);
#endif
            
			return material;
        }
    }
}
