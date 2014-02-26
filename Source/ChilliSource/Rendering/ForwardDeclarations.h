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

namespace ChilliSource
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
		typedef std::shared_ptr<CRendererSortPredicate> RendererSortPredicatePtr;
        class ICullingPredicate;
        typedef std::shared_ptr<ICullingPredicate> CullingPredicatePtr;

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
		typedef std::shared_ptr<ILightComponent> LightComponentPtr;
		typedef std::weak_ptr<ILightComponent> LightComponentWeakPtr;
        
        typedef std::shared_ptr<CAmbientLightComponent> AmbientLightComponentPtr;
		typedef std::weak_ptr<CAmbientLightComponent> AmbientLightComponentWeakPtr;
		
		typedef std::shared_ptr<CDirectionalLightComponent> DirectionalLightComponentPtr;
		typedef std::weak_ptr<CDirectionalLightComponent> DirectionalLightComponentWeakPtr;
        
        typedef std::shared_ptr<CPointLightComponent> PointLightComponentPtr;
		typedef std::weak_ptr<CPointLightComponent> PointLightComponentWeakPtr;
		
		//---Render components
		typedef std::shared_ptr<IRenderComponent> RenderComponentPtr;
		typedef std::weak_ptr<IRenderComponent> RenderComponentWeakPtr;
		
		typedef std::shared_ptr<CSpriteComponent> SpriteComponentPtr;
		typedef std::weak_ptr<CSpriteComponent> SpriteComponentWeakPtr;
		
		typedef std::shared_ptr<CStaticMeshComponent> StaticMeshComponentPtr;
		typedef std::weak_ptr<CStaticMeshComponent> StaticMeshComponentWeakPtr;
		
		typedef std::shared_ptr<CAnimatedMeshComponent> AnimatedMeshComponentPtr;
		typedef std::weak_ptr<CAnimatedMeshComponent> AnimatedMeshComponentWeakPtr;
		
		typedef std::shared_ptr<CParticleComponent> ParticleComponentPtr;
		typedef std::weak_ptr<CParticleComponent> ParticleComponentWeakPtr;
		
		typedef std::shared_ptr<CCameraComponent> CameraComponentPtr;
		typedef std::weak_ptr<CCameraComponent> CameraComponentWeakPtr;

		//---Resources
        typedef std::shared_ptr<CFont> FontPtr;
		typedef std::weak_ptr<CFont> FontWeakPtr;

		typedef std::shared_ptr<CMaterial> MaterialPtr;
		typedef std::weak_ptr<CMaterial> MaterialWeakPtr;
		
		typedef std::shared_ptr<CMesh> MeshPtr;
		typedef std::weak_ptr<CMesh> MeshWeakPtr;
		
		typedef std::shared_ptr<CSkinnedAnimation> SkinnedAnimationPtr;
		typedef std::weak_ptr<CSkinnedAnimation> SkinnedAnimationWeakPtr;
		
		typedef std::shared_ptr<CSubMesh> SubMeshPtr;
		typedef std::weak_ptr<CSubMesh> SubMeshWeakPtr;
		
		typedef std::shared_ptr<IShader> ShaderPtr;
		typedef std::weak_ptr<IShader> ShaderWeakPtr;
		
		typedef std::shared_ptr<CSpriteSheet> SpriteSheetPtr;
		typedef std::weak_ptr<CSpriteSheet> SpriteSheetWeakPtr;
		
		typedef std::shared_ptr<ITexture> TexturePtr;
		typedef std::weak_ptr<ITexture> TextureWeakPtr;
        
        typedef std::shared_ptr<ICubemap> CubemapPtr;
		typedef std::weak_ptr<ICubemap> CubemapWeakPtr;
		
		//----Other
		typedef std::shared_ptr<SkinnedAnimationFrame> SkinnedAnimationFramePtr;
		typedef std::weak_ptr<SkinnedAnimationFrame> SkinnedAnimationFrameWeakPtr;
        
        typedef std::shared_ptr<SkeletonNode> SkeletonNodePtr;
		typedef std::weak_ptr<SkeletonNode> SkeletonNodeWeakPtr;
		
        typedef std::shared_ptr<CSkinnedAnimationGroup> SkinnedAnimationGroupPtr;
		typedef std::weak_ptr<CSkinnedAnimationGroup> SkinnedAnimationWeakGroupPtr;
        
		typedef std::shared_ptr<CSkeleton> SkeletonPtr;
		typedef std::weak_ptr<CSkeleton> SkeletonWeakPtr;
	}
}

#endif
