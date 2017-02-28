//
//  The MIT License (MIT)
//
//  Copyright (c) 2016 Tag Games Limited
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

#include <CSBackend/Rendering/OpenGL/Target/GLTargetGroup.h>

#include <CSBackend/Rendering/OpenGL/Base/GLError.h>
#include <CSBackend/Rendering/OpenGL/Texture/GLTexture.h>

#include <ChilliSource/Rendering/Target/RenderTargetGroup.h>
#include <ChilliSource/Rendering/Texture/RenderTexture.h>

#ifdef CS_OPENGLVERSION_ES
	#define GL_DEPTH24_STENCIL8 GL_DEPTH24_STENCIL8_OES
#endif

namespace CSBackend
{
    namespace OpenGL
    {
        namespace
        {
#ifdef CS_TARGETPLATFORM_WINDOWS
            /// Creates a new colour render buffer with the requested size.
            ///
            /// @param dimensions
            ///     The dimensions of the colour buffer.
            ///
            /// @return The handle of the buffer.
            ///
            GLuint CreateColourRenderBuffer(const ChilliSource::Integer2& dimensions) noexcept
            {
                GLuint handle = 0;
                glGenRenderbuffers(1, &handle);
                glBindRenderbuffer(GL_RENDERBUFFER, handle);
                glRenderbufferStorage(GL_RENDERBUFFER, GL_RGB, dimensions.x, dimensions.y);
                
                CS_ASSERT_NOGLERROR("An OpenGL error occurred while creating a colour render buffer.");
                
                return handle;
            }
#endif
            
            /// Creates a new depth render buffer with the requested size.
            ///
            /// @param dimensions
            ///     The dimensions of the colour buffer.
            /// @param format
            ///     Format of the depth buffer
            ///
            /// @return The handle of the buffer.
            ///
            GLuint CreateDepthRenderBuffer(const ChilliSource::Integer2& dimensions, GLenum format) noexcept
            {
                GLuint handle = 0;
                glGenRenderbuffers(1, &handle);
                glBindRenderbuffer(GL_RENDERBUFFER, handle);
                glRenderbufferStorage(GL_RENDERBUFFER, format, dimensions.x, dimensions.y);
                
                CS_ASSERT_NOGLERROR("An OpenGL error occurred while creating a depth render buffer.");
                
                return handle;
            }
            
            /// Validates that the current frame buffer is complete. If it isn't, a fatal error
            /// will occur.
            ///
#if CS_ENABLE_DEBUG
            void ValidateFramebuffer() noexcept
            {
                GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
                
                switch(status)
                {
                    case GL_FRAMEBUFFER_COMPLETE:
                        break;
                    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
                        CS_LOG_FATAL("Framebuffer has incomplete attachment!");
                        break;
#ifdef CS_OPENGLVERSION_ES
                    case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:
                        CS_LOG_FATAL("Framebuffer has incomplete dimensions!");
                        break;
#endif
                    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
                        CS_LOG_FATAL("Framebuffer has missing attachment!");
                        break;
                    case GL_FRAMEBUFFER_UNSUPPORTED:
                        CS_LOG_FATAL("Framebuffer is unsupported!");
                        break;
                }
            }
#endif
        }
        
        //------------------------------------------------------------------------------
        GLTargetGroup::GLTargetGroup(const ChilliSource::RenderTargetGroup* renderTargetGroup) noexcept
            : m_renderTargetGroup(renderTargetGroup)
        {
            BuildTargetGroup();
        }
        
        //------------------------------------------------------------------------------
        void GLTargetGroup::Bind() noexcept
        {
            glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferHandle);
            
             CS_ASSERT_NOGLERROR("An OpenGL error occurred while binding target group.");
        }
        
        //------------------------------------------------------------------------------
        void GLTargetGroup::Restore() noexcept
        {
            if(m_invalidData)
            {
                BuildTargetGroup();
                m_invalidData = false;
            }
        }
        
        //------------------------------------------------------------------------------
        void GLTargetGroup::BuildTargetGroup() noexcept
        {
            glGenFramebuffers(1, &m_frameBufferHandle);
            glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferHandle);
            
            if (m_renderTargetGroup->GetColourTarget())
            {
                auto glTexture = static_cast<GLTexture*>(m_renderTargetGroup->GetColourTarget()->GetExtraData());
                CS_ASSERT(!glTexture->IsDataInvalid(), "GLTexture data is invalid! Ensure targets are restored after textures.");
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, glTexture->GetHandle(), 0);
            }
            else
            {
                // OpenGL 2.0 on windows requires a colour buffer, else the frame buffer cannot be used.
#ifdef CS_TARGETPLATFORM_WINDOWS
                m_colourRenderBufferHandle = CreateColourRenderBuffer(m_renderTargetGroup->GetResolution());
                glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_colourRenderBufferHandle);
#endif
            }
            
            if (m_renderTargetGroup->GetDepthTarget())
            {
                auto glTexture = static_cast<GLTexture*>(m_renderTargetGroup->GetDepthTarget()->GetExtraData());
                CS_ASSERT(!glTexture->IsDataInvalid(), "GLTexture data is invalid! Ensure targets are restored after textures.");
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, glTexture->GetHandle(), 0);
            }
            else if (m_renderTargetGroup->ShouldUseDepthBuffer())
            {
                m_depthRenderBufferHandle = CreateDepthRenderBuffer(m_renderTargetGroup->GetResolution(), m_renderTargetGroup->ShouldUseStencilBuffer() ? GL_DEPTH24_STENCIL8 : GL_DEPTH_COMPONENT16);
                glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthRenderBufferHandle);
            }
            
#ifdef CS_ENABLE_DEBUG
            ValidateFramebuffer();
#endif
            
            CS_ASSERT_NOGLERROR("An OpenGL error occurred while creating target group.");
        }
        
        //------------------------------------------------------------------------------
        GLTargetGroup::~GLTargetGroup() noexcept
        {
            if(!m_invalidData)
            {
                if (m_colourRenderBufferHandle > 0)
                {
                    glDeleteRenderbuffers(1, &m_colourRenderBufferHandle);
                }

                if (m_depthRenderBufferHandle > 0)
                {
                    glDeleteRenderbuffers(1, &m_depthRenderBufferHandle);
                }
                
                glDeleteFramebuffers(1, &m_frameBufferHandle);

                CS_ASSERT_NOGLERROR("An OpenGL error occurred while destroying target group.");
            }
        }
    }
}
