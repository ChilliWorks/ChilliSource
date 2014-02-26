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
        CS_FORWARD_DECLARE_CLASS(CAlignmentAnchors);
        CS_FORWARD_DECLARE_CLASS(CCanvasRenderer);
        CS_FORWARD_DECLARE_CLASS(ICullingPredicate);
        CS_FORWARD_DECLARE_CLASS(CViewportCullPredicate);
        CS_FORWARD_DECLARE_CLASS(CFrustumCullPredicate);
        CS_FORWARD_DECLARE_CLASS(CMeshBatch);
        CS_FORWARD_DECLARE_CLASS(IMeshBuffer);
        CS_FORWARD_DECLARE_CLASS(IRenderCapabilities);
        CS_FORWARD_DECLARE_CLASS(IRenderComponent);
        CS_FORWARD_DECLARE_CLASS(CRenderComponentFactory);
        CS_FORWARD_DECLARE_CLASS(CRenderer);
        CS_FORWARD_DECLARE_CLASS(CRendererSortPredicate);
        CS_FORWARD_DECLARE_CLASS(CnullptrSortPredicate);
        CS_FORWARD_DECLARE_CLASS(CBackToFrontSortPredicate);
        CS_FORWARD_DECLARE_CLASS(CMaterialSortPredicate);
        CS_FORWARD_DECLARE_CLASS(IRenderSystem);
        CS_FORWARD_DECLARE_CLASS(IRenderTarget);
        CS_FORWARD_DECLARE_CLASS(CVertexDeclaration);
        //------------------------------------------------------------
        /// Camera
        //------------------------------------------------------------
        CS_FORWARD_DECLARE_CLASS(CCameraComponent);
        //------------------------------------------------------------
        /// Font
        //------------------------------------------------------------
        CS_FORWARD_DECLARE_CLASS(CFont);
        CS_FORWARD_DECLARE_CLASS(CFontLoader);
        CS_FORWARD_DECLARE_CLASS(IFontManager);
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
