/*
 *  RenderSystem.h
 *  MoFlow
 *
 *  Created by Tag Games on 28/09/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */


#ifndef _MOFLO_RENDERING_IRENDERSYSTEM_H
#define _MOFLO_RENDERING_IRENDERSYSTEM_H

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/System/AppSystem.h>
#include <ChilliSource/Core/System/SystemConcepts.h>
#include <ChilliSource/Rendering/Base/MeshBuffer.h>

namespace ChilliSource
{
	namespace Rendering
	{
        class DynamicSpriteBatch;
        
		class RenderSystem : public Core::AppSystem, public Core::IComponentProducer
		{
		public:
			CS_DECLARE_NAMEDTYPE(RenderSystem);
            
            //-------------------------------------------------------
            /// Create the platform dependent backend
            ///
            /// @author S Downie
            ///
            /// @param Render capabilities
            ///
            /// @return New backend instance
            //-------------------------------------------------------
            static RenderSystemUPtr Create(RenderCapabilities* in_renderCapabilities);
            //----------------------------------------------------------
            /// Called before system init
            ///
            /// @author S Downie
			//----------------------------------------------------------
			virtual void Init() = 0;
            //----------------------------------------------------------
            /// Called before system resume
            ///
            /// @author S Downie
            //----------------------------------------------------------
            virtual void Resume() = 0;
            //----------------------------------------------------------
			/// Called after system suspend
            ///
            /// @author I Copland
			//----------------------------------------------------------
			virtual void Suspend() = 0;
            //----------------------------------------------------------
			/// Called after system destroy
            ///
            /// @author S Downie
			//----------------------------------------------------------
            virtual void Destroy() = 0;
			
			virtual ~RenderSystem();
			
			virtual void OnScreenOrientationChanged(u32 inudwWidth, u32 inudwHeight) = 0;

			virtual void BeginFrame(RenderTarget* inpActiveRenderTarget) = 0;
			virtual void EndFrame(RenderTarget* inpActiveRenderTarget) = 0;

			virtual void ApplyMaterial(const MaterialCSPtr& inMaterial, ShaderPass in_shaderPass) = 0;
            virtual void ApplyJoints(const std::vector<Core::Matrix4x4>& inaJoints) = 0;
			virtual void ApplyCamera(const Core::Vector3& invPosition, const Core::Matrix4x4& inmatInvView, const Core::Matrix4x4& inmatProj, const Core::Colour& inClearCol) = 0;
            virtual void SetLight(LightComponent* inpLightComponent) = 0;
			
			virtual void EnableAlphaBlending(bool inbIsEnabled) = 0;
			virtual void EnableDepthTesting(bool inbIsEnabled) = 0;
			virtual void EnableFaceCulling(bool inbIsEnabled) = 0;
			virtual void EnableColourWriting(bool inbIsEnabled) = 0;
			virtual void EnableDepthWriting(bool inbIsEnabled) = 0;
            virtual void EnableScissorTesting(bool inbIsEnabled) = 0;
            
            virtual void LockDepthWriting() = 0;
            virtual void UnlockDepthWriting() = 0;
            virtual void LockAlphaBlending() = 0;
            virtual void UnlockAlphaBlending() = 0;
			
            virtual void LockBlendFunction() = 0;
            virtual void UnlockBlendFunction() = 0;
            
			virtual void SetBlendFunction(BlendMode ineSrcFunc, BlendMode ineDstFunc) = 0;
            virtual void SetDepthFunction(DepthTestComparison ineFunc) = 0;
            virtual void SetCullFace(CullFace ineCullFace) = 0;
            virtual void SetScissorRegion(const Core::Vector2& invPosition, const Core::Vector2& invSize) = 0;
			
			virtual MeshBuffer* CreateBuffer(BufferDescription&) = 0;
			virtual void RenderVertexBuffer(MeshBuffer*, u32 inudwOffset, u32 inudwStride, const Core::Matrix4x4&) = 0;
			virtual void RenderBuffer(MeshBuffer*, u32 inudwOffset, u32 inudwStride, const Core::Matrix4x4&) = 0;
			
			virtual RenderTarget* CreateRenderTarget(u32 inWidth, u32 inHeight) = 0;
			virtual RenderTarget* GetDefaultRenderTarget() = 0;
			
			Core::ComponentFactory* GetComponentFactoryPtr(u32 inudwIndex);
			Core::ComponentFactory& GetComponentFactory(u32 inudwIndex);
			
			//----------------------------------------------------
			/// Get Number Of Component Factories
			///
			/// @return Number of factories in this system
			//----------------------------------------------------
			u32 GetNumComponentFactories() const;
            //----------------------------------------------------
            /// Get Dynamic Sprite Batch Pointer
            ///
            /// @return Pointer to dynamic sprite batcher
            //----------------------------------------------------
            DynamicSpriteBatch* GetDynamicSpriteBatchPtr();
            
        protected:
            //-------------------------------------------------------
            /// Private constructor to force use of factory method
            ///
            /// @author S Downie
            //-------------------------------------------------------
            RenderSystem();
            
		protected:
			
			//---Render Factories
			RenderComponentFactory* mpRenderFactory;
            
            DynamicSpriteBatch* mpSpriteBatcher;
		};
	}
}

#endif

