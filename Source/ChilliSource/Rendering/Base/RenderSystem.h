//
//  RenderSystem.h
//  Chilli Source
//  Created by Scott Downie on 28/09/2010.
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

#ifndef _CHILLISOURCE_RENDERING_IRENDERSYSTEM_H
#define _CHILLISOURCE_RENDERING_IRENDERSYSTEM_H

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/System/AppSystem.h>
#include <ChilliSource/Rendering/Base/MeshBuffer.h>

namespace ChilliSource
{
	namespace Rendering
	{
        class DynamicSpriteBatch;
        
		class RenderSystem : public Core::AppSystem
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
            /// @author Ian Copland
			//----------------------------------------------------------
			virtual void Suspend() = 0;
            //----------------------------------------------------------
			/// Called after system destroy
            ///
            /// @author S Downie
			//----------------------------------------------------------
            virtual void Destroy() = 0;
			
			virtual ~RenderSystem();
            
			virtual void BeginFrame(RenderTarget* inpActiveRenderTarget) = 0;
			virtual void EndFrame(RenderTarget* inpActiveRenderTarget) = 0;

			virtual void ApplyMaterial(const MaterialCSPtr& inMaterial, ShaderPass in_shaderPass) = 0;
            virtual void ApplyJoints(const std::vector<Core::Matrix4>& inaJoints) = 0;
			virtual void ApplyCamera(const Core::Vector3& invPosition, const Core::Matrix4& inmatInvView, const Core::Matrix4& inmatProj, const Core::Colour& inClearCol) = 0;
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
			virtual void RenderVertexBuffer(MeshBuffer*, u32 inudwOffset, u32 inudwStride, const Core::Matrix4&) = 0;
			virtual void RenderBuffer(MeshBuffer*, u32 inudwOffset, u32 inudwStride, const Core::Matrix4&) = 0;
			
			virtual RenderTarget* CreateRenderTarget(u32 inWidth, u32 inHeight) = 0;
			
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
            
            DynamicSpriteBatch* mpSpriteBatcher;
		};
	}
}

#endif

