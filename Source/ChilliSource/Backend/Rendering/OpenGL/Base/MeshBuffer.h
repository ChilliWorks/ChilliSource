/*
 *  MeshBuffer.h
 *  moFlo
 *
 *  Created by Scott Downie on 01/10/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#ifndef _MOFLOW_OPENGL_MESHBUFFER_H_
#define _MOFLOW_OPENGL_MESHBUFFER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Rendering/Base/MeshBuffer.h>
#include <ChilliSource/Backend/Rendering/OpenGL/ForwardDeclarations.h>
#include <ChilliSource/Backend/Rendering/OpenGL/Base/GLIncludes.h>

namespace ChilliSource
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
		class CMeshBuffer : public ChilliSource::Rendering::MeshBuffer
		{
		public:
			CMeshBuffer(ChilliSource::Rendering::BufferDescription &inBuffDesc);
			virtual ~CMeshBuffer();
			
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
			/// Set Map Buffer Available
			///
			/// @param Whether the GL implementation supports
			/// the map buffer extension
			//-----------------------------------------------------
			void SetMapBufferAvailable(bool inbEnabled);
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
			void SetOwningRenderSystem(CRenderSystem* inpSystem);
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

            CRenderSystem* mpRenderSystem;

            bool mbMapBufferAvailable;
            bool mbCacheValid;
            
            static CMeshBuffer* pCurrentlyBoundBuffer;
		};
	}
}

#endif
