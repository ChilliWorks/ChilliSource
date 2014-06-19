/*
 *  GLRenderTarget.h
 *  moFlo
 *
 *  Created by Scott Downie on 05/10/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#ifndef _CHILLISOURCE_OPENGL_RENDERTARGET_H_
#define _CHILLISOURCE_OPENGL_RENDERTARGET_H_

#include <ChilliSource/ChilliSource.h>
#include <CSBackend/Rendering/OpenGL/ForwardDeclarations.h>
#include <CSBackend/Rendering/OpenGL/Base/GLIncludes.h>
#include <ChilliSource/Rendering/Base/RenderTarget.h>

namespace CSBackend
{
	namespace OpenGL
	{
		class RenderTarget final : public CSRendering::RenderTarget
		{
		public:
            //--------------------------------------------------
			/// Clear Cache
			///
			/// Flush the currently bound texture cache
			//--------------------------------------------------
			static void ClearCache();
            
            RenderTarget();
            ~RenderTarget();
            
            //------------------------------------------------------
            /// Init
            ///
            /// @param Width of the render target
            /// @param Height of the render target
            //------------------------------------------------------
			void Init(u32 inudwWidth, u32 inudwHeight);
            //------------------------------------------------------
            /// Set Target Textures
            ///
            /// Sets the textures which will be rendered into. If the
            /// depth texture is nullptr a depth buffer will be
            /// created. If the colour texture is nullptr only the
            /// depth texture will be attached.
            ///
            /// @param The colour texture.
            /// @param The depth texture.
            //------------------------------------------------------
			void SetTargetTextures(const CSRendering::TextureSPtr& inpColourTexture, const CSRendering::TextureSPtr& inpDepthTexture);
            //------------------------------------------------------
            /// Bind
            ///
            /// Bind the frame buffer to the GLES context
            //------------------------------------------------------
			void Bind();
            //------------------------------------------------------
            /// Discard
            ///
            /// Hint to GL that the colour buffer and depth buffer
            /// can be discarded. This prevents load and store
            /// operations.
            //------------------------------------------------------
            void Discard();
            //------------------------------------------------------
            /// Destroy
            ///
            /// Release the buffers
            //------------------------------------------------------
			void Destroy();

		private:
            //------------------------------------------------------
            /// Create and Attach Depth Buffer
            ///
            /// Instantiate a render buffer and bind it to the
            /// frame buffer object
            //------------------------------------------------------
			bool CreateAndAttachDepthBuffer();
			//------------------------------------------------------
			/// Create and Attach Colour Buffer
			///
			/// Instantiate a render buffer and bind it to the
			/// frame buffer object
			//------------------------------------------------------
			bool CreateAndAttachColourBuffer();
            
			GLuint mFrameBuffer;
			GLuint mRenderBuffer;
			GLuint mDepthBuffer;
            
            static RenderTarget* pCurrentlyBoundTarget;
        
        };
	}
}

#endif
