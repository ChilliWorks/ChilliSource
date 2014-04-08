//
//  ForwardDeclarations.h
//  Chilli Source
//
//  Created by Ian Copland on 26/02/2014.
//  Copyright 2014 Tag Games. All rights reserved.
//


#ifndef _CHILLISOURCE_RENDERING_FORWARDDECLARATIONS_H_
#define _CHILLISOURCE_RENDERING_FORWARDDECLARATIONS_H_

#include <ChilliSource/Core/Base/StandardMacros.h>

#include <memory>

namespace ChilliSource
{
	namespace Rendering
	{
        //------------------------------------------------------------
        /// Base
        //------------------------------------------------------------
        CS_FORWARDDECLARE_CLASS(CanvasRenderer);
        CS_FORWARDDECLARE_CLASS(ICullingPredicate);
        CS_FORWARDDECLARE_CLASS(ViewportCullPredicate);
        CS_FORWARDDECLARE_CLASS(FrustumCullPredicate);
        CS_FORWARDDECLARE_CLASS(MeshBatch);
        CS_FORWARDDECLARE_CLASS(MeshBuffer);
        CS_FORWARDDECLARE_CLASS(RenderCapabilities);
        CS_FORWARDDECLARE_CLASS(RenderComponent);
        CS_FORWARDDECLARE_CLASS(RenderComponentFactory);
        CS_FORWARDDECLARE_CLASS(Renderer);
        CS_FORWARDDECLARE_CLASS(RendererSortPredicate);
        CS_FORWARDDECLARE_CLASS(NullSortPredicate);
        CS_FORWARDDECLARE_CLASS(BackToFrontSortPredicate);
        CS_FORWARDDECLARE_CLASS(MaterialSortPredicate);
        CS_FORWARDDECLARE_CLASS(RenderSystem);
        CS_FORWARDDECLARE_CLASS(RenderTarget);
        CS_FORWARDDECLARE_CLASS(VertexDeclaration);
        //------------------------------------------------------------
        /// Camera
        //------------------------------------------------------------
        CS_FORWARDDECLARE_CLASS(CameraComponent);
        //------------------------------------------------------------
        /// Font
        //------------------------------------------------------------
        CS_FORWARDDECLARE_CLASS(Font);
        CS_FORWARDDECLARE_CLASS(FontProvider);
        //------------------------------------------------------------
        /// Lighting
        //------------------------------------------------------------
        CS_FORWARDDECLARE_CLASS(AmbientLightComponent);
        CS_FORWARDDECLARE_CLASS(DirectionalLightComponent);
        CS_FORWARDDECLARE_CLASS(LightComponent);
        CS_FORWARDDECLARE_CLASS(PointLightComponent);
        //------------------------------------------------------------
        /// Material
        //------------------------------------------------------------
        CS_FORWARDDECLARE_CLASS(Material);
        CS_FORWARDDECLARE_CLASS(MaterialFactory);
        CS_FORWARDDECLARE_CLASS(MaterialProvider);
        CS_FORWARDDECLARE_CLASS(MaterialManager);
        enum class ShaderPass;
        enum class AlphaBlend;
        enum class CullFace;
        enum class DepthFunction;
        //------------------------------------------------------------
        /// Model
        //------------------------------------------------------------
        CS_FORWARDDECLARE_CLASS(AnimatedMeshComponent);
        CS_FORWARDDECLARE_CLASS(Mesh);
        CS_FORWARDDECLARE_STRUCT(MeshFeatureDescriptor);
        CS_FORWARDDECLARE_STRUCT(SubMeshDescriptor);
        CS_FORWARDDECLARE_STRUCT(MeshDescriptor);
        CS_FORWARDDECLARE_STRUCT(SkeletonDescriptor);
        CS_FORWARDDECLARE_CLASS(CSAnimProvider);
        CS_FORWARDDECLARE_CLASS(CSModelProvider);
        CS_FORWARDDECLARE_CLASS(Skeleton);
        CS_FORWARDDECLARE_CLASS(SkinnedAnimation);
        CS_FORWARDDECLARE_STRUCT(SkeletonNode);
        CS_FORWARDDECLARE_CLASS(SkinnedAnimationGroup);
        CS_FORWARDDECLARE_CLASS(StaticMeshComponent);
        CS_FORWARDDECLARE_CLASS(SubMesh);
        //------------------------------------------------------------
        /// Particles
        //------------------------------------------------------------
        CS_FORWARDDECLARE_STRUCT(Particle);
        CS_FORWARDDECLARE_CLASS(ParticleComponent);
        CS_FORWARDDECLARE_CLASS(ParticleComponentFactory);
        CS_FORWARDDECLARE_CLASS(ParticleSystem);
        CS_FORWARDDECLARE_CLASS(ColourChangerParticleEffector);
        CS_FORWARDDECLARE_CLASS(LinearForceParticleEffector);
        CS_FORWARDDECLARE_CLASS(ParticleEffector);
        CS_FORWARDDECLARE_CLASS(ParticleEffectorFactory);
        CS_FORWARDDECLARE_CLASS(ScalerParticleEffector);
        CS_FORWARDDECLARE_CLASS(SpinnerParticleEffector);
        CS_FORWARDDECLARE_CLASS(ConeParticleEmitter);
        CS_FORWARDDECLARE_CLASS(ParticleEmitter);
        CS_FORWARDDECLARE_CLASS(ParticleEmitterFactory);
        CS_FORWARDDECLARE_CLASS(PointParticleEmitter);
        CS_FORWARDDECLARE_CLASS(RingParticleEmitter);
        //------------------------------------------------------------
        /// Shader
        //------------------------------------------------------------
        CS_FORWARDDECLARE_CLASS(Shader);
        CS_FORWARDDECLARE_CLASS(ShaderManager);
        //------------------------------------------------------------
        /// Sprite
        //------------------------------------------------------------
        CS_FORWARDDECLARE_CLASS(DynamicSpriteBatch);
        CS_FORWARDDECLARE_CLASS(SpriteBatch);
        CS_FORWARDDECLARE_CLASS(SpriteComponent);
        //------------------------------------------------------------
        /// Texture
        //------------------------------------------------------------
        CS_FORWARDDECLARE_CLASS(Cubemap);
        CS_FORWARDDECLARE_CLASS(CubemapManager);
        CS_FORWARDDECLARE_CLASS(Texture);
        CS_FORWARDDECLARE_CLASS(TextureManager);
        CS_FORWARDDECLARE_CLASS(TextureAtlas);
        CS_FORWARDDECLARE_CLASS(TextureAtlasProvider);
	}
}

#endif
