//
//  MeshBuffer.cpp
//  Chilli Source
//  Created by Scott Downie on 01/10/2010.
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

#include <CSBackend/Rendering/OpenGL/Base/MeshBuffer.h>

#include <CSBackend/Rendering/OpenGL/Base/GLError.h>
#include <CSBackend/Rendering/OpenGL/Base/RenderSystem.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Rendering/Base/RenderCapabilities.h>

namespace CSBackend
{
	namespace OpenGL
	{
        MeshBuffer* MeshBuffer::pCurrentlyBoundBuffer = nullptr;
        
		//-----------------------------------------------------
		/// Constructor
		///
		/// Create a vertex buffer and index buffer from
		/// the given buffer description
		/// @param Buffer desciption
		//-----------------------------------------------------
		MeshBuffer::MeshBuffer(CSRendering::BufferDescription &inBuffDesc) 
        : CSRendering::MeshBuffer(inBuffDesc), mVertexBuffer(0), mIndexBuffer(0), mBufferUsage(0), mBufferAccess(0),
          mpVertexData(nullptr), mpIndexData(nullptr), mpVertexDataBackup(nullptr), mpIndexDataBackup(nullptr), mbMapBufferAvailable(false), mbCacheValid(false)
		{
            mbMapBufferAvailable = CSCore::Application::Get()->GetSystem<CSRendering::RenderCapabilities>()->IsMapBufferSupported();
            
			glGenBuffers(1, &mVertexBuffer);
            
            if(mBufferDesc.IndexDataCapacity > 0)
                glGenBuffers(1, &mIndexBuffer);
			
			switch(mBufferDesc.eUsageFlag)
			{
				case CSRendering::BufferUsage::k_dynamic:
					mBufferUsage = GL_DYNAMIC_DRAW;
					break;
				case CSRendering::BufferUsage::k_static:
					mBufferUsage = GL_STATIC_DRAW;
					break;
			};
			switch(mBufferDesc.eAccessFlag)
			{
				case CSRendering::BufferAccess::k_write:
#ifdef CS_OPENGLVERSION_STANDARD
					mBufferAccess = GL_WRITE_ONLY;
#elif defined CS_OPENGLVERSION_ES
					mBufferAccess = GL_WRITE_ONLY_OES;
#endif
					break;
				case CSRendering::BufferAccess::k_read:
#ifdef CS_OPENGLVERSION_STANDARD
					mBufferAccess = GL_WRITE_ONLY;
#elif defined CS_OPENGLVERSION_ES
					mBufferAccess = GL_WRITE_ONLY_OES;
#endif
					break;
				case CSRendering::BufferAccess::k_readWrite:
				default:
#ifdef CS_OPENGLVERSION_STANDARD
					mBufferAccess = GL_WRITE_ONLY;
#elif defined CS_OPENGLVERSION_ES
					mBufferAccess = GL_WRITE_ONLY_OES;
#endif
					break;
			};
			
			glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
			glBufferData(GL_ARRAY_BUFFER, mBufferDesc.VertexDataCapacity, nullptr, mBufferUsage);
            
            if(mIndexBuffer != 0)
            {
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, mBufferDesc.IndexDataCapacity, nullptr, mBufferUsage);
            }
            
            MeshBuffer::pCurrentlyBoundBuffer = this;
            
            CS_ASSERT_NOGLERROR("An OpenGL error occurred while creating mesh buffer.");
		}
		//-----------------------------------------------------
		/// Bind
		///
		/// Set the active buffer by binding to the context
		//-----------------------------------------------------
		void MeshBuffer::Bind()
		{
            if(MeshBuffer::pCurrentlyBoundBuffer != this)
            {
                glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
                
                MeshBuffer::pCurrentlyBoundBuffer = this;
            }
            
            CS_ASSERT_NOGLERROR("An OpenGL error occurred while binding mesh buffer.");
		}
		//-----------------------------------------------------
		/// Lock Vertex
		///
		/// Obtain a handle to the buffer memory in order
		/// to map data
		/// @param Outputs the pointer to the buffer
		/// @param The offset to the subset of the buffer
		/// @param The vertex layout stride
		/// @return If successful
		//-----------------------------------------------------
		bool MeshBuffer::LockVertex(f32** outppBuffer, u32 inDataOffset, u32 inDataStride)
		{
			if(mbMapBufferAvailable)
			{
				if (mBufferDesc.eUsageFlag == CSRendering::BufferUsage::k_dynamic)
				{
					glBufferData(GL_ARRAY_BUFFER, mBufferDesc.VertexDataCapacity, nullptr, mBufferUsage);
				}

#ifdef CS_OPENGLVERSION_STANDARD
				(*outppBuffer) = static_cast<f32*>(glMapBuffer(GL_ARRAY_BUFFER, mBufferAccess));
#elif defined CS_OPENGLVERSION_ES
				(*outppBuffer) = static_cast<f32*>(glMapBufferOES(GL_ARRAY_BUFFER, mBufferAccess));
#endif
				mpVertexData = (*outppBuffer);
			}
			else
			{
				if(!mpVertexData)
				{
					mpVertexData = (f32*)new u8[mBufferDesc.VertexDataCapacity];
				}

				(*outppBuffer) = mpVertexData;
			}
            
            mbCacheValid = false;
            CS_ASSERT_NOGLERROR("An OpenGL error occurred while locking vertex buffer.");
			return ((*outppBuffer) != nullptr);
		}
		//-----------------------------------------------------
		/// Lock Index
		///
		/// Obtain a handle to the buffer memory in order
		/// to map data
		/// @param Outputs the pointer to the buffer
		/// @param The offset to the subset of the buffer
		/// @param The vertex layout stride
		/// @return If successful
		//-----------------------------------------------------
		bool MeshBuffer::LockIndex(u16** outppBuffer, u32 inDataOffset, u32 inDataStride)
		{
            if(!mIndexBuffer)
            {
                (*outppBuffer) = nullptr;
                return false;
            }
            
			if(mbMapBufferAvailable)
			{
#ifdef CS_OPENGLVERSION_STANDARD
				(*outppBuffer) = static_cast<u16*>(glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, mBufferAccess));
#elif defined CS_OPENGLVERSION_ES
				(*outppBuffer) = static_cast<u16*>(glMapBufferOES(GL_ELEMENT_ARRAY_BUFFER, mBufferAccess));
#endif
                mpIndexData = (*outppBuffer);
			}
			else
			{
				if(!mpIndexData)
				{
					mpIndexData = (u16*)new u8[mBufferDesc.IndexDataCapacity];
				}

				(*outppBuffer) = mpIndexData;
			}
            
            mbCacheValid = false;
            CS_ASSERT_NOGLERROR("An OpenGL error occurred while locking index buffer.");
            return ((*outppBuffer) != nullptr);
		}
		//-----------------------------------------------------
		/// Unlock Vertex
		///
		/// Releases the buffer from mapping
		/// @return If successful
		//-----------------------------------------------------
		bool MeshBuffer::UnlockVertex()
		{
			if(mbMapBufferAvailable)
			{
				mpVertexData = nullptr;
                bool success = false;
#ifdef CS_OPENGLVERSION_STANDARD
				success = (glUnmapBuffer(GL_ARRAY_BUFFER) == GL_TRUE);
#elif defined CS_OPENGLVERSION_ES
				success = (glUnmapBufferOES(GL_ARRAY_BUFFER) == GL_TRUE);
#endif
                CS_ASSERT_NOGLERROR("An OpenGL error occurred while unlocking vertex buffer.");
                return success;
			}
			else
			{
				glBufferData(GL_ARRAY_BUFFER, mBufferDesc.VertexDataCapacity, mpVertexData, mBufferUsage);
                CS_ASSERT_NOGLERROR("An OpenGL error occurred while unlocking vertex buffer.");
				return true;
			}
		}
		//-----------------------------------------------------
		/// Unlock Index
		///
		/// Releases the buffer from mapping
		/// @return If successful
		//-----------------------------------------------------
		bool MeshBuffer::UnlockIndex()
		{
            if(!mIndexBuffer)
                return false;
            
			if(mbMapBufferAvailable)
			{
				mpIndexData = nullptr;
                bool success = false;
#ifdef CS_OPENGLVERSION_STANDARD
				success = (glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER) == GL_TRUE);
#elif defined CS_OPENGLVERSION_ES
				success = (glUnmapBufferOES(GL_ELEMENT_ARRAY_BUFFER) == GL_TRUE);
#endif
                CS_ASSERT_NOGLERROR("An OpenGL error occurred while unlocking vertex buffer.");
                return success;
			}
			else
			{
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, mBufferDesc.IndexDataCapacity, mpIndexData, mBufferUsage);
                CS_ASSERT_NOGLERROR("An OpenGL error occurred while unlocking vertex buffer.");
				return true;
			}
		}
		//-----------------------------------------------------
		/// Backup
		///
		/// Backs up the data in the mesh buffer to make
		/// sure it is not lost on context loss
		//-----------------------------------------------------
		void MeshBuffer::Backup()
		{
			if(!mpIndexDataBackup)
			{
				mpIndexDataBackup = (u16*)new u8[mBufferDesc.IndexDataCapacity];
			}

			//Fill the backup
			Bind();

            
			if(!mpVertexDataBackup)
			{
				mpVertexDataBackup = (f32*)new u8[mBufferDesc.VertexDataCapacity];
			}
			f32* pVBuffer = nullptr;
			LockVertex(&pVBuffer, 0, 0);
			memcpy(mpVertexDataBackup, pVBuffer, GetVertexCapacity());
			UnlockVertex();

            if(mIndexBuffer != 0)
            {
                if(!mpIndexDataBackup)
                {
                    mpIndexDataBackup = (u16*)new u8[mBufferDesc.IndexDataCapacity];
                }
                u16* pIBuffer = nullptr;
                LockIndex(&pIBuffer, 0, 0);
                memcpy(mpIndexDataBackup, pIBuffer, GetIndexCapacity());
                UnlockIndex();
            }
            
            mVertexBuffer = 0;
			mIndexBuffer = 0;
		}
		//-----------------------------------------------------
		/// Restore
		///
		/// Restore the mesh buffer data from the last
		/// backup after the context has been re-created
		//-----------------------------------------------------
		void MeshBuffer::Restore()
		{
            //Force bind as we may have been bound from before context destruction
            MeshBuffer::pCurrentlyBoundBuffer = nullptr;
            
            if(mpVertexDataBackup == nullptr)
            {
                return;
            }
            
            //Check for backups
            if(mpVertexDataBackup)
            {
               glGenBuffers(1, &mVertexBuffer); 
            }
            if(mpIndexDataBackup)
            {
                glGenBuffers(1, &mIndexBuffer);
            }

            Bind();
            
            glBufferData(GL_ARRAY_BUFFER, mBufferDesc.VertexDataCapacity, nullptr, mBufferUsage);
			f32* pVBuffer = nullptr;
			LockVertex(&pVBuffer, 0, 0);
			memcpy(pVBuffer, mpVertexDataBackup, GetVertexCapacity());
			UnlockVertex();

            if(mIndexBuffer != 0)
            {
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, mBufferDesc.IndexDataCapacity, nullptr, mBufferUsage);
                u16* pIBuffer = nullptr;
                LockIndex(&pIBuffer, 0, 0);
                memcpy(pIBuffer, mpIndexDataBackup, GetIndexCapacity());
                UnlockIndex();
            }

            CS_SAFEDELETE_ARRAY(mpVertexDataBackup);
			CS_SAFEDELETE_ARRAY(mpIndexDataBackup);
            
            CS_ASSERT_NOGLERROR("An OpenGL error occurred while restoring mesh buffer.");
		}
		//-----------------------------------------------------
		/// Set Owning Render System
		///
		/// The owning render system so we may inform it
		/// when this mesh buffer is destroyed
		///
		/// @param Pointer to render system
		//-----------------------------------------------------
		void MeshBuffer::SetOwningRenderSystem(RenderSystem* inpSystem)
		{
			mpRenderSystem = inpSystem;
		}
        
        //-----------------------------------------------------
        /// Is Cache Valid
        ///
        /// The owning render system needs to know if the
        /// buffer has changed and needs to apply vertex pointers
        ///
        /// @return Has mesh buffer been modifier
        //-----------------------------------------------------
        bool MeshBuffer::IsCacheValid() const
        {
            return mbCacheValid;
        }
        //-----------------------------------------------------
        /// Set Cache Valid
        ///
        /// The owning render system has seen that the
        /// buffer has changed and applied vertex pointers
        //-----------------------------------------------------
        void MeshBuffer::SetCacheValid()
        {
            mbCacheValid = true;
        }
		//-----------------------------------------------------
		/// Destructor
		///
		//-----------------------------------------------------
		MeshBuffer::~MeshBuffer()
		{
			if(mpRenderSystem)
			{
				mpRenderSystem->RemoveBuffer(this);
			}

            if(MeshBuffer::pCurrentlyBoundBuffer == this)
            {
                MeshBuffer::pCurrentlyBoundBuffer = nullptr;
            }
            
			glDeleteBuffers(1, &mVertexBuffer);
            if(mIndexBuffer != 0)
            {
                glDeleteBuffers(1, &mIndexBuffer);
            }
			
			mVertexBuffer = 0;
			mIndexBuffer  = 0;

			CS_SAFEDELETE_ARRAY(mpVertexData);
			CS_SAFEDELETE_ARRAY(mpIndexData);
			CS_SAFEDELETE_ARRAY(mpVertexDataBackup);
			CS_SAFEDELETE_ARRAY(mpIndexDataBackup);
            
            //CS_ASSERT_NOGLERROR("An OpenGL error occurred while destroying mesh buffer.");
		}
	}
}
