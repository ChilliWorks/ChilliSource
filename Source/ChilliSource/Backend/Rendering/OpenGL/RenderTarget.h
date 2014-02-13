/*
 *  GLRenderTarget.h
 *  moFlo
 *
 *  Created by Scott Downie on 05/10/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#ifndef _MOFLOW_OPENGL_RENDERTARGET_H_
#define _MOFLOW_OPENGL_RENDERTARGET_H_

#include <ChilliSource/Rendering/RenderTarget.h>
#include <ChilliSource/Rendering/ForwardDeclarations.h>
#include <ChilliSource/RenderingPlatform/OpenGL/GLIncludes.h>

#ifdef TARGET_OS_IPHONE
@class EAGLContext;
#endif

namespace moFlo
{
	namespace OpenGL
	{
		class CRenderTarget : public moFlo::Rendering::IRenderTarget
		{
		public:
#ifdef TARGET_OS_IPHONE
            //------------------------------------------------------
            /// Create Default Render Target
            ///
            /// iOS does not use an implicit onscreen render target
            /// instead a render target with the surface view
            /// attached as the render buffer must be provided.
            ///
            /// @param The iOS EAGLView
            /// @param The width of the render target.
            /// @param the height of the render target.
            //------------------------------------------------------
			static CRenderTarget* CreateDefaultRenderTarget(EAGLContext* inpContext, u32 inudwWidth, u32 inudwHeight);
            //------------------------------------------------------
            /// Present Default Render Target
            ///
            /// Presents the default iOS render target.
            ///
            /// @param The iOS EAGLView
            /// @param the render target to present
            //------------------------------------------------------
			static void PresentDefaultRenderTarget(EAGLContext* inpContext, CRenderTarget* inpRenderTarget);
            //------------------------------------------------------
            /// Destroy Default Render Target
            ///
            /// Destroys the default iOS render target.
            ///
            /// @param The iOS EAGLView
            /// @param the render target to destroy
            //------------------------------------------------------
			static void DestroyDefaultRenderTarget(EAGLContext* inpContext, CRenderTarget* inpRenderTarget);
#endif
            //--------------------------------------------------
			/// Clear Cache
			///
			/// Flush the currently bound texture cache
			//--------------------------------------------------
			static void ClearCache();
            
            CRenderTarget();
			virtual ~CRenderTarget();
            
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
            /// depth texture is NULL a depth buffer will be
            /// created. If the colour texture is NULL only the
            /// depth texture will be attached.
            ///
            /// @param The colour texture.
            /// @param The depth texture.
            //------------------------------------------------------
			void SetTargetTextures(const Rendering::TexturePtr& inpColourTexture, const Rendering::TexturePtr& inpDepthTexture);
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
            
			GLuint mFrameBuffer;
			GLuint mRenderBuffer;
			GLuint mDepthBuffer;
            
            static CRenderTarget* pCurrentlyBoundTarget;
        
        };
	}
}

#endif
