//
//  RenderComponentFactory.cpp
//  Chilli Source
//  Created by S Downie on 29/09/2010.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2010 Tag Games Limited
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//


#include <ChilliSource/Rendering/Base/RenderComponentFactory.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/Screen.h>
#include <ChilliSource/Core/Container/ParamDictionary.h>
#include <ChilliSource/Core/Image/ImageCompression.h>
#include <ChilliSource/Core/Image/ImageFormat.h>
#include <ChilliSource/Core/Resource/ResourcePool.h>
#include <ChilliSource/Rendering/Base/RenderCapabilities.h>
#include <ChilliSource/Rendering/Camera/CameraComponent.h>
#include <ChilliSource/Rendering/Lighting/AmbientLightComponent.h>
#include <ChilliSource/Rendering/Lighting/DirectionalLightComponent.h>
#include <ChilliSource/Rendering/Lighting/PointLightComponent.h>
#include <ChilliSource/Rendering/Model/AnimatedMeshComponent.h>
#include <ChilliSource/Rendering/Model/StaticMeshComponent.h>
#include <ChilliSource/Rendering/Particles/ParticleComponent.h>
#include <ChilliSource/Rendering/Particles/ParticleEffect.h>
#include <ChilliSource/Rendering/Particles/ParticleSystem.h>
#include <ChilliSource/Rendering/Particles/Affectors/ParticleAffector.h>
#include <ChilliSource/Rendering/Particles/Affectors/ParticleAffectorFactory.h>
#include <ChilliSource/Rendering/Particles/Emitters/ParticleEmitter.h>
#include <ChilliSource/Rendering/Particles/Emitters/ParticleEmitterFactory.h>
#include <ChilliSource/Rendering/Sprite/SpriteComponent.h>
#include <ChilliSource/Rendering/Texture/Texture.h>
#include <ChilliSource/Rendering/Texture/TextureAtlas.h>

namespace ChilliSource
{
	namespace Rendering
	{
        namespace
        {
            //--------------------------------------------------------
            /// Calculate the size with the maximum dimension of 1.0
            /// such that the aspect ratio of the given size is maintained
            ///
            /// @author S Downie
            ///
            /// @param Original size
            ///
            /// @return Normalised size maintaining aspect
            //--------------------------------------------------------
            Core::Vector2 CalculateNormalisedSizeMaintainingAspect(const Core::Vector2& in_originalSize)
            {
                CS_ASSERT(in_originalSize.x != 0.0f && in_originalSize.y != 0.0f, "Cannot have ZERO original size");
                Core::Vector2 normSize;
                
                if(in_originalSize.x >= in_originalSize.y)
                {
                    normSize.x = 1.0f;
                    normSize.y = in_originalSize.y/in_originalSize.x;
                }
                else
                {
                    normSize.x = in_originalSize.x/in_originalSize.y;
                    normSize.y = 1.0f;
                }
                
                return normSize;
            }
        }
        
		CS_DEFINE_NAMEDTYPE(RenderComponentFactory);
        
        //--------------------------------------------------------
        //--------------------------------------------------------
        RenderComponentFactoryUPtr RenderComponentFactory::Create()
        {
            return RenderComponentFactoryUPtr(new RenderComponentFactory());
        }
        //--------------------------------------------------------
        //--------------------------------------------------------
        void RenderComponentFactory::OnInit()
        {
            m_resourcePool = Core::Application::Get()->GetResourcePool();
            CS_ASSERT(m_resourcePool, "Render component factory is missing required system: Resource Pool");
            
            m_renderCapabilities = Core::Application::Get()->GetSystem<RenderCapabilities>();
            CS_ASSERT(m_renderCapabilities, "Render Component Factory is missing required system: Render Capabilities.");
            
            m_screen = Core::Application::Get()->GetSystem<Core::Screen>();
            CS_ASSERT(m_screen, "Render Component Factory is missing required system: Screen.");
            
            m_particleSystem = Core::Application::Get()->GetSystem<ParticleSystem>();
            CS_ASSERT(m_particleSystem, "Render Component Factory is missing required system: Particle System.");
            
            m_emitterFactory = Core::Application::Get()->GetSystem<ParticleEmitterFactory>();
            CS_ASSERT(m_emitterFactory, "Render Component Factory is missing required system: Emitter Factory.");
            
            m_affectorFactory = Core::Application::Get()->GetSystem<ParticleAffectorFactory>();
            CS_ASSERT(m_affectorFactory, "Render Component Factory is missing required system: Affector Factory.");
        }
        //--------------------------------------------------------
        //--------------------------------------------------------
		bool RenderComponentFactory::IsA(Core::InterfaceIDType in_interfaceId) const
        {
			return in_interfaceId == RenderComponentFactory::InterfaceID;
		}
        //---------------------------------------------------------------------------
        //---------------------------------------------------------------------------
        SpriteComponentUPtr RenderComponentFactory::CreateSpriteComponent(const Core::Vector2& in_size, const MaterialCSPtr& in_material)
        {
            SpriteComponentUPtr pSprite(new SpriteComponent());
            pSprite->SetDimensions(in_size);
            pSprite->SetMaterial(in_material);
            return pSprite;
        }
        //---------------------------------------------------------------------------
        //---------------------------------------------------------------------------
        SpriteComponentUPtr RenderComponentFactory::CreateSpriteComponent(const TextureAtlasCSPtr& in_textureAtlas, const std::string& in_textureId, const MaterialCSPtr& in_material, SpriteSizePolicy in_sizePolicy)
        {
            SpriteComponentUPtr pSprite(new SpriteComponent());
            pSprite->SetMaterial(in_material);
            pSprite->SetUVs(in_textureAtlas->GetFrameUVs(in_textureId));
            
            switch(in_sizePolicy)
            {
                case SpriteSizePolicy::k_useImageSize:
                    pSprite->SetDimensions(in_textureAtlas->GetFrameSize(in_textureId));
                    break;
                case SpriteSizePolicy::k_normaliseMaintainingAspect:
                    Core::Vector2 frameSize(in_textureAtlas->GetFrameSize(in_textureId));
                    pSprite->SetDimensions(CalculateNormalisedSizeMaintainingAspect(frameSize));
                    break;
            }
            
            return pSprite;
        }
        //---------------------------------------------------------------------------
        //---------------------------------------------------------------------------
		StaticMeshComponentUPtr RenderComponentFactory::CreateStaticMeshComponent(const MeshCSPtr& in_model, const MaterialCSPtr& in_material)
		{
			StaticMeshComponentUPtr pResult(new StaticMeshComponent());
			pResult->AttachMesh(in_model, in_material);
			return pResult;
		}
        //---------------------------------------------------------------------------
        //---------------------------------------------------------------------------
		AnimatedMeshComponentUPtr RenderComponentFactory::CreateAnimatedMeshComponent(const MeshCSPtr& in_model, const MaterialCSPtr& in_material)
		{
			AnimatedMeshComponentUPtr pResult(new AnimatedMeshComponent());
			pResult->AttachMesh(in_model, in_material);
			return pResult;
		}
		//---------------------------------------------------------------------------
		//---------------------------------------------------------------------------
		CameraComponentUPtr RenderComponentFactory::CreatePerspectiveCameraComponent(f32 in_FOV, f32 in_near, f32 in_far)
		{
			CameraDescription desc;
			desc.vViewSize = m_screen->GetResolution();
			desc.fAspect = (desc.vViewSize.x/desc.vViewSize.y);
			desc.fFOV = in_FOV;
			desc.fNearClipping = in_near;
			desc.fFarClipping = in_far;
			desc.ClearCol = Core::Colour::k_white;
			desc.m_type = CameraType::k_perspective;
			desc.bShouldResizeToScreen = true;
			
			CameraComponentUPtr pCamera(new CameraComponent(desc));
			return pCamera;
		}
        //---------------------------------------------------------------------------
		//---------------------------------------------------------------------------
		CameraComponentUPtr RenderComponentFactory::CreateOrthographicCameraComponent(f32 in_FOV, f32 in_near, f32 in_far)
		{
			CameraDescription desc;
			desc.vViewSize = m_screen->GetResolution();
			desc.fAspect = (desc.vViewSize.x/desc.vViewSize.y);
			desc.fFOV = in_FOV;
			desc.fNearClipping = in_near;
			desc.fFarClipping = in_far;
			desc.ClearCol = Core::Colour::k_white;
			desc.m_type = CameraType::k_orthographic;
			desc.bShouldResizeToScreen = true;
			
			CameraComponentUPtr pCamera(new CameraComponent(desc));
			return pCamera;
		}
        //---------------------------------------------------------------------------
		//---------------------------------------------------------------------------
        AmbientLightComponentUPtr RenderComponentFactory::CreateAmbientLightComponent() const
        {
            AmbientLightComponentUPtr pLight(new AmbientLightComponent());
            return pLight;
        }
		//---------------------------------------------------------------------------
		//---------------------------------------------------------------------------
		DirectionalLightComponentUPtr RenderComponentFactory::CreateDirectionalLightComponent(u32 in_shadowMapRes) const
		{
            static u32 s_shadowMapCount = 0;
            
            TextureSPtr pShadowMap;
            TextureSPtr pShadowMapDebug;
            
            if(m_renderCapabilities->IsShadowMappingSupported() == true && in_shadowMapRes > 0)
            {
                pShadowMap = m_resourcePool->CreateResource<Rendering::Texture>("_ShadowMap" + Core::ToString(s_shadowMapCount));
                Texture::Descriptor desc;
                desc.m_width = in_shadowMapRes;
                desc.m_height = in_shadowMapRes;
                desc.m_format = Core::ImageFormat::k_Depth16;
                desc.m_compression = Core::ImageCompression::k_none;
                desc.m_dataSize = 0;
                pShadowMap->Build(desc, nullptr);
        
#ifdef CS_ENABLE_DEBUGSHADOW
                pShadowMapDebug = m_resourcePool->CreateResource<Rendering::Texture>("_ShadowMapDebug" + Core::ToString(s_shadowMapCount));
                desc.m_width = in_shadowMapRes;
                desc.m_height = in_shadowMapRes;
                desc.m_format = Core::ImageFormat::k_RGB888;
                desc.m_compression = Core::ImageCompression::k_none;
                desc.m_dataSize = 0;
                pShadowMapDebug->Build(desc, nullptr);
#endif
                
                s_shadowMapCount++;
            }
            
            DirectionalLightComponentUPtr pLight(new DirectionalLightComponent(pShadowMap, pShadowMapDebug));
			return pLight;
		}
        //---------------------------------------------------------------------------
		//---------------------------------------------------------------------------
        PointLightComponentUPtr RenderComponentFactory::CreatePointLightComponent() const
        {
            PointLightComponentUPtr pLight(new PointLightComponent());
            return pLight;
        }
        //--------------------------------------------------------
        //--------------------------------------------------------
        ParticleComponentUPtr RenderComponentFactory::CreateParticleComponent()
		{
			ParticleComponentUPtr particleComp(new ParticleComponent());
			m_particleSystem->AddParticleComponent(particleComp.get());
			return particleComp;
		}
        //--------------------------------------------------------
        //--------------------------------------------------------
        ParticleComponentUPtr RenderComponentFactory::CreateParticleComponent(const ParticleEffectCSPtr& in_effect)
		{
			ParticleComponent* particleComp(new ParticleComponent());
            
            const MaterialCSPtr& material = in_effect->GetMaterial();
            particleComp->SetMaterial(material);
            
            u32 numEmitters = in_effect->GetNumEmitters();
            for(u32 i=0; i<numEmitters; ++i)
            {
                const auto& properties = in_effect->GetEmitterDesc(i);
                const std::string& type = properties.ValueForKey("Type");
                auto emitter = m_emitterFactory->CreateParticleEmitter(type, properties, material, particleComp);
                CS_ASSERT(emitter != nullptr, "Cannot create particle emitter of type " + type);
                emitter->SetTextureAtlas(in_effect->GetAtlas());
                emitter->SetTextureAtlasId(in_effect->GetAtlasId());
                particleComp->AddEmitter(std::move(emitter));
            }
            
            u32 numAffectors = in_effect->GetNumAffectors();
            for(u32 i=0; i<numAffectors; ++i)
            {
                const auto& properties = in_effect->GetAffectorDesc(i);
                const std::string& type = properties.ValueForKey("Type");
                auto affector = m_affectorFactory->CreateParticleAffector(type, properties);
                CS_ASSERT(affector != nullptr, "Cannot create particle affector of type " + type);
                particleComp->AddAffector(std::move(affector));
            }
            
            m_particleSystem->AddParticleComponent(particleComp);
			return ParticleComponentUPtr(particleComp);
		}
	}
}
