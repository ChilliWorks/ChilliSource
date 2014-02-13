/*
 *  ForwardDeclarations.h
 *  moFlo
 *
 *  Created by Stuart McGaw on 23/09/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#ifndef _MOFLO_RENDERING_FORWARDDECLARATIONS_H_
#define _MOFLO_RENDERING_FORWARDDECLARATIONS_H_

namespace moFlo
{
	namespace Rendering
	{
		class CVertexDeclaration;					//Defines the layout and components for a vertex
		
		class CMesh;							//A container for vertex data and indexes describing geometry
		class CSubMesh;
		class IMeshBuffer;				//A rendersystem specific hardware accelerated buffer containing vertex data
		class CMeshManager;
		
		class CSkinnedAnimation;			//a single skeletal animation for use with a CMesh.
		class CSkinnedAnimationManager;
        class CSkinnedAnimationGroup;
		class CSkeleton;
		struct SkinnedAnimationFrame;
        struct SkeletonNode;
		
        class CFont;
		class CFontLoader;
		class IFontManager;
		
		class ITexture;					//Texture that can be bound, have filters applied to it (implemented by a subclass per IRenderSystem)
		class ITextureManager;
        
        class ICubemap;
        class ICubemapManager;
		
		class CSpriteSheet;
		class CSpriteSheetLoader;
		class ISpriteSheetManager;
		
		class IShader; 
		class IShaderManager;
		
		class CMaterial;				//Describes how a surface should be drawn using lighting, textures and shaders
		class IMaterialManager;
        class CMaterialFactory;
		
		class IRenderTarget;
		
		class CRenderer;				//Class responsible for drawing the contents of a scene graph to screen
        class CCanvasRenderer;
		class CRendererSortPredicate;
		typedef SHARED_PTR<CRendererSortPredicate> RendererSortPredicatePtr;
        class ICullingPredicate;
        typedef SHARED_PTR<ICullingPredicate> CullingPredicatePtr;

		class CSpriteBatch;
		class CDynamicSpriteBatch;
		class CMeshBatch;
		
		class IRenderSystem;			//A specialisation of ISystem which provides facilities for rendering Scenes to screen
        class IRenderable;
		
		class CRenderComponentFactory;	
		
		class IRenderComponent;				//Base class for anything that can be drawn by an IRenderSystem.
		class CStaticMeshComponent;			//A 3D mesh which cannot be animated (though it can be moved)
		class CAnimatedMeshComponent;
		class CStaticMeshBatchComponent;
		class CParticleSystemComponent;		//A particle system
		class CSpriteComponent;				//A quad placed in world space
		class CStaticSpriteBatchComponent;	
		
		//Other Rendering subclasses of IComponent
		class CCameraComponent;
		class ILightComponent;
        class CAmbientLightComponent;
		class CDirectionalLightComponent;
        class CPointLightComponent;
        
        class CAnimatedMeshComponentUpdater;
		
		class CParticleSystem;
		class CParticleComponent;
		class CParticleEmitter;
		struct Particle;
		class CParticleComponentFactory;
        class CParticleEffectorFactory;
        class CParticleEmitterFactory;
		class IParticleEffector;
		
        class IRenderCapabilities;
        
		//---Light components
		typedef SHARED_PTR<ILightComponent> LightComponentPtr;
		typedef WEAK_PTR<ILightComponent> LightComponentWeakPtr;
        
        typedef SHARED_PTR<CAmbientLightComponent> AmbientLightComponentPtr;
		typedef WEAK_PTR<CAmbientLightComponent> AmbientLightComponentWeakPtr;
		
		typedef SHARED_PTR<CDirectionalLightComponent> DirectionalLightComponentPtr;
		typedef WEAK_PTR<CDirectionalLightComponent> DirectionalLightComponentWeakPtr;
        
        typedef SHARED_PTR<CPointLightComponent> PointLightComponentPtr;
		typedef WEAK_PTR<CPointLightComponent> PointLightComponentWeakPtr;
		
		//---Render components
		typedef SHARED_PTR<IRenderComponent> RenderComponentPtr;
		typedef WEAK_PTR<IRenderComponent> RenderComponentWeakPtr;
		
		typedef SHARED_PTR<CSpriteComponent> SpriteComponentPtr;
		typedef WEAK_PTR<CSpriteComponent> SpriteComponentWeakPtr;
		
		typedef SHARED_PTR<CStaticMeshComponent> StaticMeshComponentPtr;
		typedef WEAK_PTR<CStaticMeshComponent> StaticMeshComponentWeakPtr;
		
		typedef SHARED_PTR<CAnimatedMeshComponent> AnimatedMeshComponentPtr;
		typedef WEAK_PTR<CAnimatedMeshComponent> AnimatedMeshComponentWeakPtr;
		
		typedef SHARED_PTR<CParticleComponent> ParticleComponentPtr;
		typedef WEAK_PTR<CParticleComponent> ParticleComponentWeakPtr;
		
		typedef SHARED_PTR<CCameraComponent> CameraComponentPtr;
		typedef WEAK_PTR<CCameraComponent> CameraComponentWeakPtr;

		//---Resources
        typedef SHARED_PTR<CFont> FontPtr;
		typedef WEAK_PTR<CFont> FontWeakPtr;

		typedef SHARED_PTR<CMaterial> MaterialPtr;
		typedef WEAK_PTR<CMaterial> MaterialWeakPtr;
		
		typedef SHARED_PTR<CMesh> MeshPtr;
		typedef WEAK_PTR<CMesh> MeshWeakPtr;
		
		typedef SHARED_PTR<CSkinnedAnimation> SkinnedAnimationPtr;
		typedef WEAK_PTR<CSkinnedAnimation> SkinnedAnimationWeakPtr;
		
		typedef SHARED_PTR<CSubMesh> SubMeshPtr;
		typedef WEAK_PTR<CSubMesh> SubMeshWeakPtr;
		
		typedef SHARED_PTR<IShader> ShaderPtr;
		typedef WEAK_PTR<IShader> ShaderWeakPtr;
		
		typedef SHARED_PTR<CSpriteSheet> SpriteSheetPtr;
		typedef WEAK_PTR<CSpriteSheet> SpriteSheetWeakPtr;
		
		typedef SHARED_PTR<ITexture> TexturePtr;
		typedef WEAK_PTR<ITexture> TextureWeakPtr;
        
        typedef SHARED_PTR<ICubemap> CubemapPtr;
		typedef WEAK_PTR<ICubemap> CubemapWeakPtr;
		
		//----Other
		typedef SHARED_PTR<SkinnedAnimationFrame> SkinnedAnimationFramePtr;
		typedef WEAK_PTR<SkinnedAnimationFrame> SkinnedAnimationFrameWeakPtr;
        
        typedef SHARED_PTR<SkeletonNode> SkeletonNodePtr;
		typedef WEAK_PTR<SkeletonNode> SkeletonNodeWeakPtr;
		
        typedef SHARED_PTR<CSkinnedAnimationGroup> SkinnedAnimationGroupPtr;
		typedef WEAK_PTR<CSkinnedAnimationGroup> SkinnedAnimationWeakGroupPtr;
        
		typedef SHARED_PTR<CSkeleton> SkeletonPtr;
		typedef WEAK_PTR<CSkeleton> SkeletonWeakPtr;
	}
}

#endif
