/*
 *  GLRenderTarget.cpp
 *  moFlo
 *
 *  Created by Scott Downie on 05/10/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Backend/Rendering/OpenGL/Base/RenderTarget.h>

#include <ChilliSource/Backend/Rendering/OpenGL/Base/GLError.h>
#include <ChilliSource/Backend/Rendering/OpenGL/Texture/Texture.h>

#ifdef CS_TARGETPLATFORM_IOS
#import <ChilliSource/Backend/Platform/iOS/Core/Base/CSAppDelegate.h>
#import <ChilliSource/Backend/Platform/iOS/Core/Base/CSGLViewController.h>
#endif

namespace ChilliSource
{
	namespace OpenGL
	{
        RenderTarget* RenderTarget::pCurrentlyBoundTarget = nullptr;
        
        GLint gCurrentlyBoundFrameBuffer = -1;
        GLint gCurrentlyBoundRenderBuffer = -1;
        
        //------------------------------------------------------
        /// Bind Frame Buffer
        ///
        /// @param Frame buffer ID
        //------------------------------------------------------
        inline void BindFrameBuffer(GLuint inBuffer)
        {
            if(gCurrentlyBoundFrameBuffer != inBuffer)
            {
                glBindFramebuffer(GL_FRAMEBUFFER, inBuffer);
                gCurrentlyBoundFrameBuffer = inBuffer;
            }
            
            CS_OPENGL_ASSERT("An OpenGL error occurred while binding frame buffer.");
        }
        //------------------------------------------------------
        /// Bind Render Buffer
        ///
        /// @param Render buffer ID
        //------------------------------------------------------
        inline void BindRenderBuffer(GLuint inBuffer)
        {
            if(gCurrentlyBoundRenderBuffer != inBuffer)
            {
                glBindRenderbuffer(GL_RENDERBUFFER, inBuffer);
                gCurrentlyBoundRenderBuffer = inBuffer;
            }
            
            CS_OPENGL_ASSERT("An OpenGL error occurred while binding render buffer.");
        }
        //------------------------------------------------------
        /// Create Frame Buffer
        ///
        /// @param Frame buffer ID
        //------------------------------------------------------
        inline void CreateFrameBuffer(GLuint* inBuffer)
        {
            glGenFramebuffers(1, inBuffer);
            CS_OPENGL_ASSERT("An OpenGL error occurred while creating frame buffer.");
            
            BindFrameBuffer(*inBuffer);
        }
        //------------------------------------------------------
        /// Create Render Buffer
        ///
        /// @param Render buffer ID
        //------------------------------------------------------
        inline void CreateRenderBuffer(GLuint* inBuffer)
        {
            glGenRenderbuffers(1, inBuffer);
            CS_OPENGL_ASSERT("An OpenGL error occurred while creating render buffer.");
            
            BindRenderBuffer(*inBuffer);
        }
        //------------------------------------------------------
        /// Delete Frame Buffer
        ///
        /// @param Frame buffer ID
        //------------------------------------------------------
        inline void DeleteFrameBuffer(GLuint* inBuffer)
        {
            if(*inBuffer)
			{
                if(gCurrentlyBoundFrameBuffer == *inBuffer)
                {
                    gCurrentlyBoundFrameBuffer = -1;
                }
                
				glDeleteFramebuffers(1, inBuffer);
				*inBuffer = 0;
			}
            
            CS_OPENGL_ASSERT("An OpenGL error occurred while deleting frame buffer.");
        }
        //------------------------------------------------------
        /// Delete Render Buffer
        ///
        /// @param Render buffer ID
        //------------------------------------------------------
        inline void DeleteRenderBuffer(GLuint* inBuffer)
        {
            if(*inBuffer)
			{
                if(gCurrentlyBoundRenderBuffer == *inBuffer)
                {
                    gCurrentlyBoundRenderBuffer = -1;
                }
                
				glDeleteRenderbuffers(1, inBuffer);
				*inBuffer = 0;
			}
            
            CS_OPENGL_ASSERT("An OpenGL error occurred while deleting render buffer.");
        }
        //--------------------------------------------------
        /// Clear Cache
        ///
        /// Flush the currently bound texture cache
        //--------------------------------------------------
        void RenderTarget::ClearCache()
        {
            RenderTarget::pCurrentlyBoundTarget = nullptr;
            gCurrentlyBoundFrameBuffer = -1;
            gCurrentlyBoundRenderBuffer = -1;
        }
		//------------------------------------------------------
		/// Constructor
		///
		/// Default
		//------------------------------------------------------
        RenderTarget::RenderTarget() 
        : mFrameBuffer(0), mRenderBuffer(0), mDepthBuffer(0)
		{
		}
		//------------------------------------------------------
		/// Init
		///
		/// @param Width of the render target
		/// @param Height of the render target
		//------------------------------------------------------
		void RenderTarget::Init(u32 inudwWidth, u32 inudwHeight)
		{
			mudwWidth = inudwWidth;
			mudwHeight = inudwHeight;
			
            CreateFrameBuffer(&mFrameBuffer);
		}
        //------------------------------------------------------
        /// Set Target Textures
        //------------------------------------------------------
        void RenderTarget::SetTargetTextures(const Rendering::TextureSPtr& inpColourTexture, const Rendering::TextureSPtr& inpDepthTexture)
        {
            DeleteRenderBuffer(&mRenderBuffer);
            DeleteRenderBuffer(&mDepthBuffer);
            BindFrameBuffer(mFrameBuffer);
			
            if (inpColourTexture != nullptr)
            {
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, std::static_pointer_cast<Texture>(inpColourTexture)->GetTextureHandle(), 0);
            }
            
            if (inpDepthTexture != nullptr)
            {
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, std::static_pointer_cast<Texture>(inpDepthTexture)->GetTextureHandle(), 0);
            }
            else
            {
                if (CreateAndAttachDepthBuffer() == false)
                {
                    CS_LOG_ERROR("Failed to attach Depth Buffer to render target.");
                }
            }
            
#ifdef CS_ENABLE_DEBUG
            GLenum eStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
            switch(eStatus)
            {
                case GL_FRAMEBUFFER_COMPLETE:
                    break;
                case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
                    CS_LOG_ERROR("Render Target's frame buffer has incomplete attachment!");
                    break;
#ifdef CS_OPENGLVERSION_ES
                case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:
                    CS_LOG_ERROR("Render Target's frame buffer has incomplete dimensions!");
                    break;
#endif
                case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
                    CS_LOG_ERROR("Render Target's frame buffer has missing attachment!");
                    break;
                case GL_FRAMEBUFFER_UNSUPPORTED:
                    CS_LOG_ERROR("Render Target's frame buffer is unsupported!");
                    break;
            }
#endif
            
            CS_OPENGL_ASSERT("An OpenGL error occurred while setting render target textures.");
        }
        //------------------------------------------------------
		/// Create and Attach Depth Buffer
		///
		/// Instantiate a render buffer and bind it to the
		/// frame buffer object
		//------------------------------------------------------
		bool RenderTarget::CreateAndAttachDepthBuffer()
		{
            BindFrameBuffer(mFrameBuffer);
            CreateRenderBuffer(&mDepthBuffer);
			
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, mudwWidth, mudwHeight);
            
			//Attach the depth buffer to the framebuffer
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mDepthBuffer);
			
            bool success = true;
#ifdef CS_ENABLE_DEBUG
            //Check it has worked
            GLint Depth = 0;
			glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_DEPTH_SIZE,  (GLint*)&Depth);
			success = ((glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) && Depth != 0);
#endif
            
            CS_OPENGL_ASSERT("An OpenGL error occurred while creating and attaching depth buffer.");
            return success;
		}
		//------------------------------------------------------
		/// Bind
		//------------------------------------------------------
		void RenderTarget::Bind()
		{ 
            if(RenderTarget::pCurrentlyBoundTarget != this)
            {
                BindFrameBuffer(mFrameBuffer);
                glViewport(0, 0, mudwWidth, mudwHeight);
                RenderTarget::pCurrentlyBoundTarget = this;
            }
            
            CS_OPENGL_ASSERT("An OpenGL error occurred while binding render target.");
		}
        //------------------------------------------------------
        /// Discard
        //------------------------------------------------------
        void RenderTarget::Discard()
        {
#ifdef CS_TARGETPLATFORM_IOS
            GLenum Attachments[] = {GL_COLOR_ATTACHMENT0, GL_DEPTH_ATTACHMENT};
            BindFrameBuffer(mFrameBuffer);
            glDiscardFramebufferEXT(GL_FRAMEBUFFER, 2, Attachments);
            CS_OPENGL_ASSERT("An OpenGL error occurred while discarding the render target.");
#endif
        }
		//------------------------------------------------------
		/// Destroy
		//------------------------------------------------------
		void RenderTarget::Destroy()
		{
            Bind();
            DeleteRenderBuffer(&mRenderBuffer);
            DeleteRenderBuffer(&mDepthBuffer);
            
            if(RenderTarget::pCurrentlyBoundTarget == this)
            {
#ifdef CS_TARGETPLATFORM_IOS
                GLKView* glView = (GLKView*)[CSAppDelegate sharedInstance].viewController.view;
                [glView bindDrawable];
#else
                BindFrameBuffer(0);
#endif
                RenderTarget::pCurrentlyBoundTarget = nullptr;
            }
            
            DeleteFrameBuffer(&mFrameBuffer);
		}
		//------------------------------------------------------
		/// Destructor
		//------------------------------------------------------
		RenderTarget::~RenderTarget()
		{
			Destroy();
		}
	}
}
