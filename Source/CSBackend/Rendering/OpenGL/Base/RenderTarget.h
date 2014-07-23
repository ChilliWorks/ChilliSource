//
//  RenderTarget.h
//  Chilli Source
//  Created by Scott Downie on 05/10/2010.
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
            /// @author Ian Copland
            ///
            /// @return The colour texture.
            //------------------------------------------------------
			const CSRendering::TextureSPtr& GetColourTexture() const;
            //------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The depth texture.
            //------------------------------------------------------
			const CSRendering::TextureSPtr& GetDepthTexture() const;
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
            
            CSRendering::TextureSPtr m_colourTexture;
            CSRendering::TextureSPtr m_depthTexture;
            
            static RenderTarget* pCurrentlyBoundTarget;
        
        };
	}
}

#endif
