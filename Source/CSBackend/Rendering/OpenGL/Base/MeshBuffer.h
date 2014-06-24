//
//  MeshBuffer.h
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

#ifndef _CHILLISOURCE_OPENGL_MESHBUFFER_H_
#define _CHILLISOURCE_OPENGL_MESHBUFFER_H_

#include <ChilliSource/ChilliSource.h>
#include <CSBackend/Rendering/OpenGL/ForwardDeclarations.h>
#include <CSBackend/Rendering/OpenGL/Base/GLIncludes.h>
#include <ChilliSource/Rendering/Base/MeshBuffer.h>

namespace CSBackend
{
	namespace OpenGL
	{
		typedef u32 GLBufferID;
		//==========================================================
		/// Description:
		///
		/// Concrete implementation of a vertex buffer object 
		/// for OpenGL ES
		//==========================================================
		class MeshBuffer : public CSRendering::MeshBuffer
		{
		public:
			MeshBuffer(CSRendering::BufferDescription &inBuffDesc);
			virtual ~MeshBuffer();
			
            //-----------------------------------------------------
            /// Bind
            ///
            /// Set the active buffer by binding to the context
            //-----------------------------------------------------
			void Bind();
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
			bool LockVertex(f32** outppBuffer, u32 inDataOffset, u32 inDataStride);
            //-----------------------------------------------------
            /// Unlock Vertex
            ///
            /// Releases the buffer from mapping
            /// @return If successful
            //-----------------------------------------------------
			bool UnlockVertex();
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
			bool LockIndex(u16** outppBuffer, u32 inDataOffset, u32 inDataStride);
            //-----------------------------------------------------
            /// Unlock Index
            ///
            /// Releases the buffer from mapping
            /// @return If successful
            //-----------------------------------------------------
			bool UnlockIndex();
			//-----------------------------------------------------
			/// Backup
			///
			/// Backs up the data in the mesh buffer to make
			/// sure it is not lost on context loss
			//-----------------------------------------------------
			void Backup();
			//-----------------------------------------------------
			/// Restore
			///
			/// Restore the mesh buffer data from the last
			/// backup after the context has been re-created
			//-----------------------------------------------------
			void Restore();
			//-----------------------------------------------------
			/// Set Owning Render System
			///
			/// The owning render system so we may inform it
			/// when this mesh buffer is destroyed
			///
			/// @param Pointer to render system
			//-----------------------------------------------------
			void SetOwningRenderSystem(RenderSystem* inpSystem);
			//-----------------------------------------------------
            /// Is Cache Valid
            ///
            /// The owning render system needs to know if the
            /// buffer has changed and needs to apply vertex pointers
            ///
            /// @return Has mesh buffer been modifier
            //-----------------------------------------------------
            bool IsCacheValid() const;
            //-----------------------------------------------------
            /// Set Cache Valid
            ///
            /// The owning render system has seen that the
            /// buffer has changed and applied vertex pointers
            //-----------------------------------------------------
            void SetCacheValid();
		private:
			GLBufferID mVertexBuffer;
			GLBufferID mIndexBuffer;
			
			u32 mBufferUsage;
			u32 mBufferAccess;
            
            f32* mpVertexData;
            u16* mpIndexData;

            f32* mpVertexDataBackup;
            u16* mpIndexDataBackup;

            RenderSystem* mpRenderSystem;

            bool mbMapBufferAvailable;
            bool mbCacheValid;
            
            static MeshBuffer* pCurrentlyBoundBuffer;
		};
	}
}

#endif
