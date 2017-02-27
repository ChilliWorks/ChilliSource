//
//  ForwardDeclarations.h
//  ChilliSource
//  Created by Ian Copland on 26/02/2014.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2014 Tag Games Limited
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

#ifndef _CHILLISOURCE_RENDERING_FORWARDDECLARATIONS_H_
#define _CHILLISOURCE_RENDERING_FORWARDDECLARATIONS_H_

#include <ChilliSource/Core/Base/StandardMacros.h>

#include <memory>

namespace ChilliSource
{
    //------------------------------------------------------------
    /// Base
    //------------------------------------------------------------
    CS_FORWARDDECLARE_CLASS(CanvasMaterialPool);
    CS_FORWARDDECLARE_CLASS(CanvasRenderer);
    CS_FORWARDDECLARE_CLASS(IRenderCommandProcessor);
    CS_FORWARDDECLARE_CLASS(IRenderPassCompiler);
    CS_FORWARDDECLARE_CLASS(ForwardRenderPassCompiler);
    CS_FORWARDDECLARE_CLASS(FrameAllocatorQueue);
    CS_FORWARDDECLARE_CLASS(RenderCapabilities);
    CS_FORWARDDECLARE_CLASS(Renderer);
    CS_FORWARDDECLARE_CLASS(RenderFrame);
    CS_FORWARDDECLARE_CLASS(RenderFrameData);
    CS_FORWARDDECLARE_CLASS(RenderObject);
    CS_FORWARDDECLARE_CLASS(RenderPass);
    CS_FORWARDDECLARE_CLASS(RenderPassObject);
    CS_FORWARDDECLARE_CLASS(RenderSnapshot);
    CS_FORWARDDECLARE_CLASS(TargetRenderPassGroup);
    CS_FORWARDDECLARE_CLASS(CameraRenderPassGroup);
    enum class AlignmentAnchor;
    enum class BlendEqn;
    enum class BlendMode;
    enum class CanvasDrawMode;
    enum class CullFace;
    enum class DepthTestComparison;
    enum class HorizontalTextJustification;
    enum class RenderLayer;
    enum class RenderPasses;
    enum class SizePolicy;
    enum class StencilOp;
    enum class SurfaceFormat;
    enum class TestFunc;
    enum class TargetType;
    enum class VerticalTextJustification;
    //------------------------------------------------------------
    /// Camera
    //------------------------------------------------------------
    CS_FORWARDDECLARE_CLASS(CameraComponent);
    CS_FORWARDDECLARE_CLASS(OrthographicCameraComponent);
    CS_FORWARDDECLARE_CLASS(PerspectiveCameraComponent);
    CS_FORWARDDECLARE_CLASS(RenderCamera);
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
    CS_FORWARDDECLARE_CLASS(AmbientRenderLight);
    CS_FORWARDDECLARE_CLASS(DirectionalRenderLight);
    CS_FORWARDDECLARE_CLASS(PointRenderLight);
    //------------------------------------------------------------
    /// Material
    //------------------------------------------------------------
    CS_FORWARDDECLARE_CLASS(ForwardRenderMaterialGroupManager);
    CS_FORWARDDECLARE_CLASS(Material);
    CS_FORWARDDECLARE_CLASS(MaterialFactory);
    CS_FORWARDDECLARE_CLASS(MaterialProvider);
    CS_FORWARDDECLARE_CLASS(RenderMaterial);
    CS_FORWARDDECLARE_CLASS(RenderMaterialGroup);
    CS_FORWARDDECLARE_CLASS(RenderMaterialGroupManager);
    enum class MaterialShadingType;
    //------------------------------------------------------------
    /// Model
    //------------------------------------------------------------
    CS_FORWARDDECLARE_CLASS(AnimatedModelComponent);
    CS_FORWARDDECLARE_CLASS(CSAnimProvider);
    CS_FORWARDDECLARE_CLASS(CSModelProvider);
    CS_FORWARDDECLARE_CLASS(MeshDesc);
    CS_FORWARDDECLARE_CLASS(Model);
    CS_FORWARDDECLARE_CLASS(ModelDesc);
    CS_FORWARDDECLARE_CLASS(PrimitiveModelFactory);
    CS_FORWARDDECLARE_CLASS(RenderDynamicMesh);
    CS_FORWARDDECLARE_CLASS(RenderMesh);
    CS_FORWARDDECLARE_CLASS(RenderMeshBatch);
    CS_FORWARDDECLARE_CLASS(RenderMeshManager);
    CS_FORWARDDECLARE_CLASS(RenderSkinnedAnimation);
    CS_FORWARDDECLARE_CLASS(Skeleton);
    CS_FORWARDDECLARE_CLASS(SkeletonDesc);
    CS_FORWARDDECLARE_STRUCT(SkeletonNode);
    CS_FORWARDDECLARE_CLASS(SkinnedAnimation);
    CS_FORWARDDECLARE_CLASS(SkinnedAnimationGroup);
    CS_FORWARDDECLARE_CLASS(SmallMeshBatcher);
    CS_FORWARDDECLARE_CLASS(StaticModelComponent);
    CS_FORWARDDECLARE_CLASS(VertexFormat);
    enum class IndexFormat;
    enum class PolygonType;
    //------------------------------------------------------------
    /// Particle
    //------------------------------------------------------------
    CS_FORWARDDECLARE_CLASS(ConcurrentParticleData);
    CS_FORWARDDECLARE_CLASS(CSParticleProvider);
    CS_FORWARDDECLARE_CLASS(ParticleEffect);
    CS_FORWARDDECLARE_CLASS(ParticleEffectComponent);
    CS_FORWARDDECLARE_STRUCT(Particle);
    CS_FORWARDDECLARE_CLASS(ParticleDrawable);
    CS_FORWARDDECLARE_CLASS(ParticleDrawableDef);
    CS_FORWARDDECLARE_CLASS(ParticleDrawableDefFactory);
    CS_FORWARDDECLARE_CLASS(ParticleEmitter);
    CS_FORWARDDECLARE_CLASS(ParticleEmitterDef);
    CS_FORWARDDECLARE_CLASS(ParticleEmitterDefFactory);
    CS_FORWARDDECLARE_CLASS(ParticleAffector);
    CS_FORWARDDECLARE_CLASS(ParticleAffectorDef);
    CS_FORWARDDECLARE_CLASS(ParticleAffectorDefFactory);
    CS_FORWARDDECLARE_CLASS(StaticBillboardParticleDrawable);
    CS_FORWARDDECLARE_CLASS(StaticBillboardParticleDrawableDef);
    CS_FORWARDDECLARE_CLASS(AccelerationParticleAffector);
    CS_FORWARDDECLARE_CLASS(AccelerationParticleAffectorDef);
    CS_FORWARDDECLARE_CLASS(AngularAccelerationParticleAffector);
    CS_FORWARDDECLARE_CLASS(AngularAccelerationParticleAffectorDef);
    CS_FORWARDDECLARE_CLASS(ColourOverLifetimeParticleAffector);
    CS_FORWARDDECLARE_CLASS(ColourOverLifetimeParticleAffectorDef);
    CS_FORWARDDECLARE_CLASS(ScaleOverLifetimeParticleAffector);
    CS_FORWARDDECLARE_CLASS(ScaleOverLifetimeParticleAffectorDef);
    CS_FORWARDDECLARE_CLASS(SphereParticleEmitter);
    CS_FORWARDDECLARE_CLASS(SphereParticleEmitterDef);
    CS_FORWARDDECLARE_CLASS(CircleParticleEmitter);
    CS_FORWARDDECLARE_CLASS(CircleParticleEmitterDef);
    CS_FORWARDDECLARE_CLASS(PointParticleEmitter);
    CS_FORWARDDECLARE_CLASS(PointParticleEmitterDef);
    CS_FORWARDDECLARE_CLASS(ConeParticleEmitter);
    CS_FORWARDDECLARE_CLASS(ConeParticleEmitterDef);
    CS_FORWARDDECLARE_CLASS(Cone2DParticleEmitter);
    CS_FORWARDDECLARE_CLASS(Cone2DParticleEmitterDef);
    template <typename TPropertyType> class ParticleProperty;
    template <typename TPropertyType> using ParticlePropertyUPtr = std::unique_ptr<ParticleProperty<TPropertyType>>;
    template <typename TPropertyType> using ParticlePropertyCUPtr = std::unique_ptr<const ParticleProperty<TPropertyType>>;
    template <typename TPropertyType> class ConstantParticleProperty;
    template <typename TPropertyType> class RandomConstantParticleProperty;
    template <typename TPropertyType> class ComponentwiseRandomConstantParticleProperty;
    template <typename TPropertyType> class CurveParticleProperty;
    template <typename TPropertyType> class RandomCurveParticleProperty;
    template <typename TPropertyType> class ComponentwiseRandomCurveParticleProperty;
    //------------------------------------------------------------
    /// Render Command
    //------------------------------------------------------------
    CS_FORWARDDECLARE_CLASS(ApplyAmbientLightRenderCommand);
    CS_FORWARDDECLARE_CLASS(ApplyCameraRenderCommand);
    CS_FORWARDDECLARE_CLASS(ApplyDirectionalLightRenderCommand);
    CS_FORWARDDECLARE_CLASS(ApplyDynamicMeshRenderCommand);
    CS_FORWARDDECLARE_CLASS(ApplyMaterialRenderCommand);
    CS_FORWARDDECLARE_CLASS(ApplyMeshRenderCommand);
    CS_FORWARDDECLARE_CLASS(ApplyMeshBatchRenderCommand);
    CS_FORWARDDECLARE_CLASS(ApplyPointLightRenderCommand);
    CS_FORWARDDECLARE_CLASS(ApplySkinnedAnimationRenderCommand);
    CS_FORWARDDECLARE_CLASS(BeginRenderCommand);
    CS_FORWARDDECLARE_CLASS(BeginWithTargetGroupRenderCommand);
    CS_FORWARDDECLARE_CLASS(EndRenderCommand);
    CS_FORWARDDECLARE_CLASS(LoadMaterialGroupRenderCommand);
    CS_FORWARDDECLARE_CLASS(LoadMeshRenderCommand);
    CS_FORWARDDECLARE_CLASS(LoadShaderRenderCommand);
    CS_FORWARDDECLARE_CLASS(LoadTargetGroupRenderCommand);
    CS_FORWARDDECLARE_CLASS(LoadTextureRenderCommand);
    CS_FORWARDDECLARE_CLASS(LoadCubemapRenderCommand);
    CS_FORWARDDECLARE_CLASS(RestoreMeshRenderCommand);
    CS_FORWARDDECLARE_CLASS(RestoreRenderTargetGroupCommand);
    CS_FORWARDDECLARE_CLASS(RestoreTextureRenderCommand);
    CS_FORWARDDECLARE_CLASS(RestoreCubemapRenderCommand);
    CS_FORWARDDECLARE_CLASS(RenderCommand);
    CS_FORWARDDECLARE_CLASS(RenderCommandBuffer);
    CS_FORWARDDECLARE_CLASS(RenderCommandBufferManager);
    CS_FORWARDDECLARE_CLASS(RenderCommandList);
    CS_FORWARDDECLARE_CLASS(RenderInstanceRenderCommand);
    CS_FORWARDDECLARE_CLASS(UnloadMaterialGroupRenderCommand);
    CS_FORWARDDECLARE_CLASS(UnloadMeshRenderCommand);
    CS_FORWARDDECLARE_CLASS(UnloadShaderRenderCommand);
    CS_FORWARDDECLARE_CLASS(UnloadTargetGroupRenderCommand);
    CS_FORWARDDECLARE_CLASS(UnloadTextureRenderCommand);
    CS_FORWARDDECLARE_CLASS(UnloadCubemapRenderCommand);
    //------------------------------------------------------------
    /// Shader
    //------------------------------------------------------------
    CS_FORWARDDECLARE_CLASS(CSShaderProvider);
    CS_FORWARDDECLARE_CLASS(RenderShader);
    CS_FORWARDDECLARE_CLASS(RenderShaderManager);
    CS_FORWARDDECLARE_CLASS(RenderShaderVariables);
    CS_FORWARDDECLARE_CLASS(Shader);
    //------------------------------------------------------------
    /// Skybox
    //------------------------------------------------------------
    CS_FORWARDDECLARE_CLASS(SkyboxComponent);
    //------------------------------------------------------------
    /// Sprite
    //------------------------------------------------------------
    CS_FORWARDDECLARE_CLASS(SpriteComponent);
    //------------------------------------------------------------
    /// Target
    //------------------------------------------------------------
    CS_FORWARDDECLARE_CLASS(RenderTargetGroup);
    CS_FORWARDDECLARE_CLASS(RenderTargetGroupManager);
    CS_FORWARDDECLARE_CLASS(TargetGroup);
    enum class RenderTargetGroupType;
    //------------------------------------------------------------
    /// Texture
    //------------------------------------------------------------
    CS_FORWARDDECLARE_CLASS(Cubemap);
    CS_FORWARDDECLARE_CLASS(CubemapProvider);
    CS_FORWARDDECLARE_CLASS(RenderTexture);
    CS_FORWARDDECLARE_CLASS(RenderTextureManager);
    CS_FORWARDDECLARE_CLASS(Texture);
    CS_FORWARDDECLARE_CLASS(TextureAtlas);
    CS_FORWARDDECLARE_CLASS(TextureAtlasProvider);
    CS_FORWARDDECLARE_CLASS(TextureDesc);
    CS_FORWARDDECLARE_CLASS(TextureProvider);
    CS_FORWARDDECLARE_CLASS(UVs);
    enum class TextureFilterMode;
    enum class TextureType;
    enum class TextureWrapMode;
}

#endif
