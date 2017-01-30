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
#include <ChilliSource/Rendering/Camera/CameraComponent.h>
#include <ChilliSource/Rendering/Camera/OrthographicCameraComponent.h>
#include <ChilliSource/Rendering/Camera/PerspectiveCameraComponent.h>
#include <ChilliSource/Rendering/Lighting/AmbientLightComponent.h>
#include <ChilliSource/Rendering/Lighting/DirectionalLightComponent.h>
#include <ChilliSource/Rendering/Lighting/PointLightComponent.h>
#include <ChilliSource/Rendering/Model/AnimatedModelComponent.h>
#include <ChilliSource/Rendering/Model/StaticModelComponent.h>
#include <ChilliSource/Rendering/Particle/ParticleEffectComponent.h>
#include <ChilliSource/Rendering/Sprite/SpriteComponent.h>
#include <ChilliSource/Rendering/Skybox/SkyboxComponent.h>
#include <ChilliSource/Rendering/Texture/Texture.h>
#include <ChilliSource/Rendering/Texture/TextureAtlas.h>

namespace ChilliSource
{
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
        m_screen = Application::Get()->GetSystem<Screen>();
        CS_ASSERT(m_screen, "Render Component Factory is missing required system: Screen.");
    }
    //--------------------------------------------------------
    //--------------------------------------------------------
    bool RenderComponentFactory::IsA(InterfaceIDType in_interfaceId) const
    {
        return in_interfaceId == RenderComponentFactory::InterfaceID;
    }
    //---------------------------------------------------------------------------
    //---------------------------------------------------------------------------
    SpriteComponentUPtr RenderComponentFactory::CreateSpriteComponent(const Vector2& in_size, const MaterialCSPtr& in_material, SizePolicy in_sizePolicy)
    {
        SpriteComponentUPtr pSprite(new SpriteComponent());
        pSprite->SetMaterial(in_material);
        pSprite->SetSizePolicy(in_sizePolicy);
        pSprite->SetSize(in_size);
        return pSprite;
    }
    //---------------------------------------------------------------------------
    //---------------------------------------------------------------------------
    SpriteComponentUPtr RenderComponentFactory::CreateSpriteComponent(const Vector2& in_size, const TextureAtlasCSPtr& in_textureAtlas, const std::string& in_textureId, const MaterialCSPtr& in_material, SizePolicy in_sizePolicy)
    {
        SpriteComponentUPtr pSprite(new SpriteComponent());
        pSprite->SetMaterial(in_material);
        pSprite->SetTextureAtlas(in_textureAtlas);
        pSprite->SetTextureAtlasId(in_textureId);
        pSprite->SetSizePolicy(in_sizePolicy);
        pSprite->SetSize(in_size);
        
        return pSprite;
    }
    //---------------------------------------------------------------------------
    //---------------------------------------------------------------------------
    StaticModelComponentUPtr RenderComponentFactory::CreateStaticModelComponent(const ModelCSPtr& in_model, const MaterialCSPtr& in_material)
    {
        return StaticModelComponentUPtr(new StaticModelComponent(in_model, in_material));
    }
    //---------------------------------------------------------------------------
    //---------------------------------------------------------------------------
    SkyboxComponentUPtr RenderComponentFactory::CreateSkyboxComponent(const ModelCSPtr& model, const MaterialCSPtr& material)
    {
        return SkyboxComponentUPtr(new SkyboxComponent(model, material));
    }
    //---------------------------------------------------------------------------
    //---------------------------------------------------------------------------
    AnimatedModelComponentUPtr RenderComponentFactory::CreateAnimatedModelComponent(const ModelCSPtr& in_model, const MaterialCSPtr& in_material, const SkinnedAnimationCSPtr& in_animation,
                                                                                    AnimatedModelComponent::PlaybackType in_playbackType) noexcept
    {
        return AnimatedModelComponentUPtr(new AnimatedModelComponent(in_model, in_material, in_animation, in_playbackType));
    }
    //---------------------------------------------------------------------------
    //---------------------------------------------------------------------------
    PerspectiveCameraComponentUPtr RenderComponentFactory::CreatePerspectiveCameraComponent(f32 in_fov, f32 in_near, f32 in_far)
    {
        auto screenSize = m_screen->GetResolution();
        return PerspectiveCameraComponentUPtr(new PerspectiveCameraComponent(screenSize.x / screenSize.y, in_fov, CameraComponent::ViewportResizePolicy::k_scaleWithScreen, in_near, in_far));
    }
    //---------------------------------------------------------------------------
    //---------------------------------------------------------------------------
    OrthographicCameraComponentUPtr RenderComponentFactory::CreateOrthographicCameraComponent(const Vector2& in_viewportSize, f32 in_near, f32 in_far)
    {
        return OrthographicCameraComponentUPtr(new OrthographicCameraComponent(in_viewportSize, CameraComponent::ViewportResizePolicy::k_scaleWithScreen, in_near, in_far));
    }
    //---------------------------------------------------------------------------
    //---------------------------------------------------------------------------
    OrthographicCameraComponentUPtr RenderComponentFactory::CreateOrthographicCameraComponent(f32 in_near, f32 in_far)
    {
        return CreateOrthographicCameraComponent(m_screen->GetResolution(), in_near, in_far);
    }
    //---------------------------------------------------------------------------
    //---------------------------------------------------------------------------
    AmbientLightComponentUPtr RenderComponentFactory::CreateAmbientLightComponent(const Colour& in_colour, f32 in_intensity) const noexcept
    {
        return AmbientLightComponentUPtr(new AmbientLightComponent(in_colour, in_intensity));
    }
    //---------------------------------------------------------------------------
    //---------------------------------------------------------------------------
    DirectionalLightComponentUPtr RenderComponentFactory::CreateDirectionalLightComponent(const Colour& in_colour, f32 in_intensity) const noexcept
    {
        return DirectionalLightComponentUPtr(new DirectionalLightComponent(in_colour, in_intensity));
    }
    //---------------------------------------------------------------------------
    //---------------------------------------------------------------------------
    DirectionalLightComponentUPtr RenderComponentFactory::CreateDirectionalLightComponent(DirectionalLightComponent::ShadowQuality in_shadowQuality, const Colour& in_colour, f32 in_intensity) const noexcept
    {
        return DirectionalLightComponentUPtr(new DirectionalLightComponent(in_shadowQuality, in_colour, in_intensity));
    }
    //---------------------------------------------------------------------------
    //---------------------------------------------------------------------------
    PointLightComponentUPtr RenderComponentFactory::CreatePointLightComponent(const Colour& in_colour, f32 in_radius, f32 in_intensity) const noexcept
    {
        return PointLightComponentUPtr(new PointLightComponent(in_colour, in_radius, in_intensity));
    }
    //---------------------------------------------------------------------------
    //---------------------------------------------------------------------------
    ParticleEffectComponentUPtr RenderComponentFactory::CreateParticleEffectComponent(const ParticleEffectCSPtr& in_particleEffect) const
    {
        return ParticleEffectComponentUPtr(new ParticleEffectComponent(in_particleEffect));
    }
}
