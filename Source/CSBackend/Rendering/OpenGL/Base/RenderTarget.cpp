//
//  RenderTarget.cpp
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

#include <CSBackend/Rendering/OpenGL/Base/RenderTarget.h>

#include <CSBackend/Rendering/OpenGL/Base/GLError.h>
#include <CSBackend/Rendering/OpenGL/Texture/Texture.h>

#ifdef CS_TARGETPLATFORM_IOS
#import <CSBackend/Platform/iOS/Core/Base/CSAppDelegate.h>
#import <CSBackend/Platform/iOS/Core/Base/CSGLViewController.h>
#endif

namespace CSBackend
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
            
            CS_ASSERT_NOGLERROR("An OpenGL error occurred while binding frame buffer.");
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
            
            CS_ASSERT_NOGLERROR("An OpenGL error occurred while binding render buffer.");
        }
        //------------------------------------------------------
        /// Create Frame Buffer
        ///
        /// @param Frame buffer ID
        //------------------------------------------------------
        inline void CreateFrameBuffer(GLuint* inBuffer)
        {
            glGenFramebuffers(1, inBuffer);
            CS_ASSERT_NOGLERROR("An OpenGL error occurred while creating frame buffer.");
            
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
            CS_ASSERT_NOGLERROR("An OpenGL error occurred while creating render buffer.");
            
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
            
            CS_ASSERT_NOGLERROR("An OpenGL error occurred while deleting frame buffer.");
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
            
            CS_ASSERT_NOGLERROR("An OpenGL error occurred while deleting render buffer.");
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
        void RenderTarget::SetTargetTextures(const CSRendering::TextureSPtr& inpColourTexture, const CSRendering::TextureSPtr& inpDepthTexture)
        {
            DeleteRenderBuffer(&mRenderBuffer);
            DeleteRenderBuffer(&mDepthBuffer);
            BindFrameBuffer(mFrameBuffer);
            
            m_colourTexture = inpColourTexture;
            m_depthTexture = inpDepthTexture;
			
            if (m_colourTexture != nullptr)
            {
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, std::static_pointer_cast<Texture>(m_colourTexture)->GetTextureHandle(), 0);
            }
//Regular OpenGL must always have a colour buffer bound
#ifdef CS_OPENGLVERSION_STANDARD
			else
			{
				if (CreateAndAttachColourBuffer() == false)
				{
					CS_LOG_ERROR("Failed to attach Colour Buffer to render target.");
				}
			}
#endif
            if (m_depthTexture != nullptr)
            {
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, std::static_pointer_cast<Texture>(m_depthTexture)->GetTextureHandle(), 0);
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
            
            CS_ASSERT_NOGLERROR("An OpenGL error occurred while setting render target textures.");
        }
        //------------------------------------------------------
        //------------------------------------------------------
        const CSRendering::TextureSPtr& RenderTarget::GetColourTexture() const
        {
            return m_colourTexture;
        }
        //------------------------------------------------------
        //------------------------------------------------------
        const CSRendering::TextureSPtr& RenderTarget::GetDepthTexture() const
        {
            return m_depthTexture;
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
            
            CS_ASSERT_NOGLERROR("An OpenGL error occurred while creating and attaching depth buffer.");
            return success;
		}
		//------------------------------------------------------
		/// Create and Attach Colour Buffer
		///
		/// Instantiate a render buffer and bind it to the
		/// frame buffer object
		//------------------------------------------------------
		bool RenderTarget::CreateAndAttachColourBuffer()
		{
			BindFrameBuffer(mFrameBuffer);
			CreateRenderBuffer(&mRenderBuffer);

			glRenderbufferStorage(GL_RENDERBUFFER, GL_RGB, mudwWidth, mudwHeight);

			//Attach the depth buffer to the framebuffer
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, mRenderBuffer);

			bool success = true;
#ifdef CS_ENABLE_DEBUG
			//Check it has worked
			GLint red = 0;
			glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_RED_SIZE, (GLint*)&red);
			success = ((glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) && red != 0);
#endif

			CS_ASSERT_NOGLERROR("An OpenGL error occurred while creating and attaching colour buffer.");
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
            
            CS_ASSERT_NOGLERROR("An OpenGL error occurred while binding render target.");
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
            CS_ASSERT_NOGLERROR("An OpenGL error occurred while discarding the render target.");
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
            
            m_colourTexture.reset();
            m_depthTexture.reset();
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
