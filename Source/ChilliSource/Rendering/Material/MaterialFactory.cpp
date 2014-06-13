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

namespace ChilliSource
{
    namespace Rendering
    {
        CS_DEFINE_NAMEDTYPE(MaterialFactory);
        
        //-------------------------------------------------------
        //-------------------------------------------------------
        MaterialFactoryUPtr MaterialFactory::Create(RenderCapabilities* in_renderCapabilities)
        {
            return MaterialFactoryUPtr(new MaterialFactory(in_renderCapabilities));
        }
        //---------------------------------------------------
        //---------------------------------------------------
        MaterialFactory::MaterialFactory(RenderCapabilities* in_renderCapabilities)
        : m_renderCapabilities(in_renderCapabilities)
        {
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
            
            Core::ResourcePool* resourcePool = Core::Application::Get()->GetResourcePool();
            material->SetShader(ShaderPass::k_ambient, resourcePool->LoadResource<Shader>(Core::StorageLocation::k_chilliSource, "Shaders/GUI.csshader"));
            material->SetTransparencyEnabled(true);
            material->SetCullFace(CullFace::k_back);
            material->SetFaceCullingEnabled(true);
            material->SetDepthTestEnabled(false);
			return material;
        }
        //---------------------------------------------------
        //---------------------------------------------------
        MaterialSPtr MaterialFactory::CreateSprite(const std::string& in_uniqueId, const TextureCSPtr& in_texture) const
        {
            MaterialSPtr material(CreateCustom(in_uniqueId));
            Core::ResourcePool* resourcePool = Core::Application::Get()->GetResourcePool();
            material->SetShader(ShaderPass::k_ambient, resourcePool->LoadResource<Shader>(Core::StorageLocation::k_chilliSource, "Shaders/Sprite.csshader"));
            material->AddTexture(in_texture);
            material->SetTransparencyEnabled(false);
            material->SetCullFace(CullFace::k_back);
            material->SetFaceCullingEnabled(false);
			return material;
        }
        //---------------------------------------------------
        //---------------------------------------------------
        MaterialSPtr MaterialFactory::CreateStatic(const std::string& in_uniqueId, const TextureCSPtr& in_texture) const
        {
            MaterialSPtr material(CreateCustom(in_uniqueId));
            Core::ResourcePool* resourcePool = Core::Application::Get()->GetResourcePool();
            material->SetShader(ShaderPass::k_ambient, resourcePool->LoadResource<Shader>(Core::StorageLocation::k_chilliSource, "Shaders/Static.csshader"));
            material->AddTexture(in_texture);
            material->SetTransparencyEnabled(false);
            material->SetCullFace(CullFace::k_back);
            material->SetFaceCullingEnabled(true);
			return material;
        }
        //---------------------------------------------------
        //---------------------------------------------------
        MaterialSPtr MaterialFactory::CreateStaticAmbient(const std::string& in_uniqueId, const TextureCSPtr& in_texture) const
        {
            MaterialSPtr material(CreateCustom(in_uniqueId));
            Core::ResourcePool* resourcePool = Core::Application::Get()->GetResourcePool();
            material->SetShader(ShaderPass::k_ambient, resourcePool->LoadResource<Shader>(Core::StorageLocation::k_chilliSource, "Shaders/StaticAmbient.csshader"));
            material->AddTexture(in_texture);
            material->SetTransparencyEnabled(false);
            material->SetCullFace(CullFace::k_back);
            material->SetFaceCullingEnabled(true);
			return material;
        }
        //---------------------------------------------------
        //---------------------------------------------------
        MaterialSPtr MaterialFactory::CreateStaticBlinn(const std::string& in_uniqueId, const TextureCSPtr& in_texture) const
        {
            MaterialSPtr material(CreateCustom(in_uniqueId));
            Core::ResourcePool* resourcePool = Core::Application::Get()->GetResourcePool();
            material->SetShader(ShaderPass::k_ambient, resourcePool->LoadResource<Shader>(Core::StorageLocation::k_chilliSource, "Shaders/StaticAmbient.csshader"));
            material->SetShader(ShaderPass::k_directional, resourcePool->LoadResource<Shader>(Core::StorageLocation::k_chilliSource, "Shaders/StaticBlinnDirectional.csshader"));
            material->SetShader(ShaderPass::k_point, resourcePool->LoadResource<Shader>(Core::StorageLocation::k_chilliSource, "Shaders/StaticBlinnPoint.csshader"));
            material->AddTexture(in_texture);
            material->SetTransparencyEnabled(false);
            material->SetCullFace(CullFace::k_back);
            material->SetFaceCullingEnabled(true);
			return material;
        }
        //---------------------------------------------------
        //---------------------------------------------------
        MaterialSPtr MaterialFactory::CreateStaticBlinnShadowed(const std::string& in_uniqueId, const TextureCSPtr& in_texture) const
        {
        	if (m_renderCapabilities->IsShadowMappingSupported() == false)
			{
				return CreateStaticBlinn(in_uniqueId, in_texture);
			}

            MaterialSPtr material(CreateCustom(in_uniqueId));
            Core::ResourcePool* resourcePool = Core::Application::Get()->GetResourcePool();
            material->SetShader(ShaderPass::k_ambient, resourcePool->LoadResource<Shader>(Core::StorageLocation::k_chilliSource, "Shaders/StaticAmbient.csshader"));
            material->SetShader(ShaderPass::k_directional, resourcePool->LoadResource<Shader>(Core::StorageLocation::k_chilliSource, "Shaders/StaticBlinnShadowedDirectional.csshader"));
            material->SetShader(ShaderPass::k_point, resourcePool->LoadResource<Shader>(Core::StorageLocation::k_chilliSource, "Shaders/StaticBlinnPoint.csshader"));
            material->AddTexture(in_texture);
            material->SetTransparencyEnabled(false);
            material->SetCullFace(CullFace::k_back);
            material->SetFaceCullingEnabled(true);
			return material;
        }
        //---------------------------------------------------
        //---------------------------------------------------
        MaterialSPtr MaterialFactory::CreateStaticBlinnPerVertex(const std::string& in_uniqueId, const TextureCSPtr& in_texture) const
        {
            MaterialSPtr material(CreateCustom(in_uniqueId));
            Core::ResourcePool* resourcePool = Core::Application::Get()->GetResourcePool();
            material->SetShader(ShaderPass::k_ambient, resourcePool->LoadResource<Shader>(Core::StorageLocation::k_chilliSource, "Shaders/StaticAmbient.csshader"));
            material->SetShader(ShaderPass::k_directional, resourcePool->LoadResource<Shader>(Core::StorageLocation::k_chilliSource, "Shaders/StaticBlinnPerVertexDirectional.csshader"));
            material->SetShader(ShaderPass::k_point, resourcePool->LoadResource<Shader>(Core::StorageLocation::k_chilliSource, "Shaders/StaticBlinnPerVertexPoint.csshader"));
            material->AddTexture(in_texture);
            material->SetTransparencyEnabled(false);
            material->SetCullFace(CullFace::k_back);
            material->SetFaceCullingEnabled(true);
			return material;
        }
        //---------------------------------------------------
        //---------------------------------------------------
        MaterialSPtr MaterialFactory::CreateStaticBlinnPerVertexShadowed(const std::string& in_uniqueId, const TextureCSPtr& in_texture) const
        {
        	if (m_renderCapabilities->IsShadowMappingSupported() == false)
			{
				return CreateStaticBlinnPerVertex(in_uniqueId, in_texture);
			}

            MaterialSPtr material(CreateCustom(in_uniqueId));
            Core::ResourcePool* resourcePool = Core::Application::Get()->GetResourcePool();
            material->SetShader(ShaderPass::k_ambient, resourcePool->LoadResource<Shader>(Core::StorageLocation::k_chilliSource, "Shaders/StaticAmbient.csshader"));
            material->SetShader(ShaderPass::k_directional, resourcePool->LoadResource<Shader>(Core::StorageLocation::k_chilliSource, "Shaders/StaticBlinnPerVertexShadowedDirectional.csshader"));
            material->SetShader(ShaderPass::k_point, resourcePool->LoadResource<Shader>(Core::StorageLocation::k_chilliSource, "Shaders/StaticBlinnPerVertexPoint.csshader"));
            material->AddTexture(in_texture);
            material->SetTransparencyEnabled(false);
            material->SetCullFace(CullFace::k_back);
            material->SetFaceCullingEnabled(true);
			return material;
        }
        //---------------------------------------------------
        //---------------------------------------------------
        MaterialSPtr MaterialFactory::CreateStaticDirectionalShadowMap(const std::string& in_uniqueId) const
        {
            MaterialSPtr material(CreateCustom(in_uniqueId));
            Core::ResourcePool* resourcePool = Core::Application::Get()->GetResourcePool();
            material->SetShader(ShaderPass::k_ambient, resourcePool->LoadResource<Shader>(Core::StorageLocation::k_chilliSource, "Shaders/StaticDirectionalShadowMap.csshader"));
            material->SetTransparencyEnabled(false);
            material->SetCullFace(CullFace::k_back);
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
        MaterialSPtr MaterialFactory::CreateAnimated(const std::string& in_uniqueId, const TextureCSPtr& in_texture) const
        {
            MaterialSPtr material(CreateCustom(in_uniqueId));
            Core::ResourcePool* resourcePool = Core::Application::Get()->GetResourcePool();
            material->SetShader(ShaderPass::k_ambient, resourcePool->LoadResource<Shader>(Core::StorageLocation::k_chilliSource, "Shaders/Animated.csshader"));
            material->AddTexture(in_texture);
            material->SetTransparencyEnabled(false);
            material->SetCullFace(CullFace::k_back);
            material->SetFaceCullingEnabled(true);
			return material;
        }
        //---------------------------------------------------
        //---------------------------------------------------
        MaterialSPtr MaterialFactory::CreateAnimatedAmbient(const std::string& in_uniqueId, const TextureCSPtr& in_texture) const
        {
            MaterialSPtr material(CreateCustom(in_uniqueId));
            Core::ResourcePool* resourcePool = Core::Application::Get()->GetResourcePool();
            material->SetShader(ShaderPass::k_ambient, resourcePool->LoadResource<Shader>(Core::StorageLocation::k_chilliSource, "Shaders/AnimatedAmbient.csshader"));
            material->AddTexture(in_texture);
            material->SetTransparencyEnabled(false);
            material->SetCullFace(CullFace::k_back);
            material->SetFaceCullingEnabled(true);
			return material;
        }
        //---------------------------------------------------
        //---------------------------------------------------
        MaterialSPtr MaterialFactory::CreateAnimatedBlinn(const std::string& in_uniqueId, const TextureCSPtr& in_texture) const
        {
            MaterialSPtr material(CreateCustom(in_uniqueId));
            Core::ResourcePool* resourcePool = Core::Application::Get()->GetResourcePool();
            material->SetShader(ShaderPass::k_ambient, resourcePool->LoadResource<Shader>(Core::StorageLocation::k_chilliSource, "Shaders/AnimatedAmbient.csshader"));
            material->SetShader(ShaderPass::k_directional, resourcePool->LoadResource<Shader>(Core::StorageLocation::k_chilliSource, "Shaders/AnimatedBlinnDirectional.csshader"));
            material->SetShader(ShaderPass::k_point, resourcePool->LoadResource<Shader>(Core::StorageLocation::k_chilliSource, "Shaders/AnimatedBlinnPoint.csshader"));
            material->AddTexture(in_texture);
            material->SetTransparencyEnabled(false);
            material->SetCullFace(CullFace::k_back);
            material->SetFaceCullingEnabled(true);
			return material;
        }
        //---------------------------------------------------
        //---------------------------------------------------
        MaterialSPtr MaterialFactory::CreateAnimatedBlinnShadowed(const std::string& in_uniqueId, const TextureCSPtr& in_texture) const
        {
        	if (m_renderCapabilities->IsShadowMappingSupported() == false)
			{
				return CreateAnimatedBlinn(in_uniqueId, in_texture);
			}

            MaterialSPtr material(CreateCustom(in_uniqueId));
            Core::ResourcePool* resourcePool = Core::Application::Get()->GetResourcePool();
            material->SetShader(ShaderPass::k_ambient, resourcePool->LoadResource<Shader>(Core::StorageLocation::k_chilliSource, "Shaders/AnimatedAmbient.csshader"));
            material->SetShader(ShaderPass::k_directional, resourcePool->LoadResource<Shader>(Core::StorageLocation::k_chilliSource, "Shaders/AnimatedBlinnShadowedDirectional.csshader"));
            material->SetShader(ShaderPass::k_point, resourcePool->LoadResource<Shader>(Core::StorageLocation::k_chilliSource, "Shaders/AnimatedBlinnPoint.csshader"));
            material->AddTexture(in_texture);
            material->SetTransparencyEnabled(false);
            material->SetCullFace(CullFace::k_back);
            material->SetFaceCullingEnabled(true);
			return material;
        }
        //---------------------------------------------------
        //---------------------------------------------------
        MaterialSPtr MaterialFactory::CreateAnimatedBlinnPerVertex(const std::string& in_uniqueId, const TextureCSPtr& in_texture) const
        {
            MaterialSPtr material(CreateCustom(in_uniqueId));
            Core::ResourcePool* resourcePool = Core::Application::Get()->GetResourcePool();
            material->SetShader(ShaderPass::k_ambient, resourcePool->LoadResource<Shader>(Core::StorageLocation::k_chilliSource, "Shaders/AnimatedAmbient.csshader"));
            material->SetShader(ShaderPass::k_directional, resourcePool->LoadResource<Shader>(Core::StorageLocation::k_chilliSource, "Shaders/AnimatedBlinnPerVertexDirectional.csshader"));
            material->SetShader(ShaderPass::k_point, resourcePool->LoadResource<Shader>(Core::StorageLocation::k_chilliSource, "Shaders/AnimatedBlinnPerVertexPoint.csshader"));
            material->AddTexture(in_texture);
            material->SetTransparencyEnabled(false);
            material->SetCullFace(CullFace::k_back);
            material->SetFaceCullingEnabled(true);
			return material;
        }
        //---------------------------------------------------
        //---------------------------------------------------
        MaterialSPtr MaterialFactory::CreateAnimatedBlinnPerVertexShadowed(const std::string& in_uniqueId, const TextureCSPtr& in_texture) const
        {
        	if (m_renderCapabilities->IsShadowMappingSupported() == false)
        	{
        		return CreateAnimatedBlinnPerVertex(in_uniqueId, in_texture);
        	}

            MaterialSPtr material(CreateCustom(in_uniqueId));
            Core::ResourcePool* resourcePool = Core::Application::Get()->GetResourcePool();
            material->SetShader(ShaderPass::k_ambient, resourcePool->LoadResource<Shader>(Core::StorageLocation::k_chilliSource, "Shaders/AnimatedAmbient.csshader"));
            material->SetShader(ShaderPass::k_directional, resourcePool->LoadResource<Shader>(Core::StorageLocation::k_chilliSource, "Shaders/AnimatedBlinnPerVertexShadowedDirectional.csshader"));
            material->SetShader(ShaderPass::k_point, resourcePool->LoadResource<Shader>(Core::StorageLocation::k_chilliSource, "Shaders/AnimatedBlinnPerVertexPoint.csshader"));
            material->AddTexture(in_texture);
            material->SetTransparencyEnabled(false);
            material->SetCullFace(CullFace::k_back);
            material->SetFaceCullingEnabled(true);
			return material;
        }
        //---------------------------------------------------
        //---------------------------------------------------
        MaterialSPtr MaterialFactory::CreateAnimatedDirectionalShadowMap(const std::string& in_uniqueId) const
        {
            MaterialSPtr material(CreateCustom(in_uniqueId));
            Core::ResourcePool* resourcePool = Core::Application::Get()->GetResourcePool();
            material->SetShader(ShaderPass::k_ambient, resourcePool->LoadResource<Shader>(Core::StorageLocation::k_chilliSource, "Shaders/AnimatedDirectionalShadowMap.csshader"));
            material->SetTransparencyEnabled(false);
            material->SetCullFace(CullFace::k_back);
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
