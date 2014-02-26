//
//  ForwardDeclarations.h
//  Chilli Source
//
//  Created by Ian Copland on 26/02/2014.
//  Copyright 2014 Tag Games. All rights reserved.
//


#ifndef _CHILLISOURCE_RENDERING_FORWARDDECLARATIONS_H_
#define _CHILLISOURCE_RENDERING_FORWARDDECLARATIONS_H_

#include <ChilliSource/ChilliSource.h>

namespace ChilliSource
{
	namespace Rendering
	{
        //------------------------------------------------------------
        /// Base
        //------------------------------------------------------------
        CS_FORWARD_DECLARE_CLASS(CanvasRenderer);
        CS_FORWARD_DECLARE_CLASS(ICullingPredicate);
        CS_FORWARD_DECLARE_CLASS(ViewportCullPredicate);
        CS_FORWARD_DECLARE_CLASS(FrustumCullPredicate);
        CS_FORWARD_DECLARE_CLASS(MeshBatch);
        CS_FORWARD_DECLARE_CLASS(MeshBuffer);
        CS_FORWARD_DECLARE_CLASS(RenderCapabilities);
        CS_FORWARD_DECLARE_CLASS(RenderComponent);
        CS_FORWARD_DECLARE_CLASS(RenderComponentFactory);
        CS_FORWARD_DECLARE_CLASS(Renderer);
        CS_FORWARD_DECLARE_CLASS(RendererSortPredicate);
        CS_FORWARD_DECLARE_CLASS(NullSortPredicate);
        CS_FORWARD_DECLARE_CLASS(BackToFrontSortPredicate);
        CS_FORWARD_DECLARE_CLASS(MaterialSortPredicate);
        CS_FORWARD_DECLARE_CLASS(RenderSystem);
        CS_FORWARD_DECLARE_CLASS(RenderTarget);
        CS_FORWARD_DECLARE_CLASS(VertexDeclaration);
        //------------------------------------------------------------
        /// Camera
        //------------------------------------------------------------
        CS_FORWARD_DECLARE_CLASS(CameraComponent);
        //------------------------------------------------------------
        /// Font
        //------------------------------------------------------------
        CS_FORWARD_DECLARE_CLASS(Font);
        CS_FORWARD_DECLARE_CLASS(FontLoader);
        CS_FORWARD_DECLARE_CLASS(FontManager);
        //------------------------------------------------------------
        /// Lighting
        //------------------------------------------------------------
        CS_FORWARD_DECLARE_CLASS(CAmbientLightComponent);
        CS_FORWARD_DECLARE_CLASS(CDirectionalLightComponent);
        CS_FORWARD_DECLARE_CLASS(ILightComponent);
        CS_FORWARD_DECLARE_CLASS(CPointLightComponent);
        //------------------------------------------------------------
        /// Material
        //------------------------------------------------------------
        CS_FORWARD_DECLARE_CLASS(CMaterial);
        CS_FORWARD_DECLARE_CLASS(CMaterialFactory);
        CS_FORWARD_DECLARE_CLASS(CMaterialLoader);
        CS_FORWARD_DECLARE_CLASS(IMaterialManager);
        //------------------------------------------------------------
        /// Model
        //------------------------------------------------------------
        CS_FORWARD_DECLARE_CLASS(CAnimatedMeshComponent);
        CS_FORWARD_DECLARE_CLASS(CAnimatedMeshComponentUpdater);
        CS_FORWARD_DECLARE_CLASS(CMesh);
        CS_FORWARD_DECLARE_CLASS(MeshFeatureDescriptor);
        CS_FORWARD_DECLARE_CLASS(SubMeshDescriptor);
        CS_FORWARD_DECLARE_CLASS(MeshDescriptor);
        CS_FORWARD_DECLARE_CLASS(CMeshManager);
        CS_FORWARD_DECLARE_CLASS(CMoAnimLoader);
        CS_FORWARD_DECLARE_CLASS(CMoModelLoader);
        CS_FORWARD_DECLARE_CLASS(CSkeleton);
        CS_FORWARD_DECLARE_CLASS(CSkinnedAnimation);
        CS_FORWARD_DECLARE_CLASS(SkinnedAnimationFrame);
        CS_FORWARD_DECLARE_CLASS(SkeletonNode);
        CS_FORWARD_DECLARE_CLASS(CSkinnedAnimationGroup);
        CS_FORWARD_DECLARE_CLASS(CSkinnedAnimationManager);
        CS_FORWARD_DECLARE_CLASS(CStaticMeshComponent);
        CS_FORWARD_DECLARE_CLASS(CSubMesh);
        //------------------------------------------------------------
        /// Particles
        //------------------------------------------------------------
        CS_FORWARD_DECLARE_CLASS(CParticleComponent);
        CS_FORWARD_DECLARE_CLASS(CParticleComponentFactory);
        CS_FORWARD_DECLARE_CLASS(CParticleSystem);
        CS_FORWARD_DECLARE_CLASS(CColourChangerParticleEffector);
        CS_FORWARD_DECLARE_CLASS(CLinearForceParticleEffector);
        CS_FORWARD_DECLARE_CLASS(IParticleEffector);
        CS_FORWARD_DECLARE_CLASS(CParticleEffectorFactory);
        CS_FORWARD_DECLARE_CLASS(CScalerParticleEffector);
        CS_FORWARD_DECLARE_CLASS(CSpinnerParticleEffector);
        CS_FORWARD_DECLARE_CLASS(CConeParticleEmitter);
        CS_FORWARD_DECLARE_CLASS(CParticleEmitter);
        CS_FORWARD_DECLARE_CLASS(CParticleEmitterFactory);
        CS_FORWARD_DECLARE_CLASS(CPointParticleEmitter);
        CS_FORWARD_DECLARE_CLASS(CRingParticleEmitter);
        //------------------------------------------------------------
        /// Shader
        //------------------------------------------------------------
        CS_FORWARD_DECLARE_CLASS(IShader);
        CS_FORWARD_DECLARE_CLASS(IShaderManager);
        //------------------------------------------------------------
        /// Sprite
        //------------------------------------------------------------
        CS_FORWARD_DECLARE_CLASS(CDynamicSpriteBatch);
        CS_FORWARD_DECLARE_CLASS(CSpriteBatch);
        CS_FORWARD_DECLARE_CLASS(CSpriteComponent);
        CS_FORWARD_DECLARE_CLASS(CSpriteSheet);
        CS_FORWARD_DECLARE_CLASS(CSpriteSheetLoader);
        CS_FORWARD_DECLARE_CLASS(ISpriteSheetManager);
        CS_FORWARD_DECLARE_CLASS(CXMLSpriteSheetLoader);
        //------------------------------------------------------------
        /// Texture
        //------------------------------------------------------------
        CS_FORWARD_DECLARE_CLASS(ICubemap);
        CS_FORWARD_DECLARE_CLASS(ICubemapManager);
        CS_FORWARD_DECLARE_CLASS(ITexture);
        CS_FORWARD_DECLARE_CLASS(ITextureManager);
	}
}

#endif
