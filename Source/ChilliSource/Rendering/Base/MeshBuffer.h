/*
 *  MeshBuffer.h
 *  moFlo
 *
 *  Created by Scott Downie on 30/09/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#ifndef _MO_FLO_RENDERING_MESH_BUFFER
#define _MO_FLO_RENDERING_MESH_BUFFER

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Rendering/Base/VertexDeclaration.h>

namespace ChilliSource
{
	namespace Rendering
	{
		enum class PrimitiveType	{k_tri, k_triStrip, k_line};
		enum class BufferUsage	{k_static, k_dynamic};
		enum class BufferAccess	{k_read, k_write, k_readWrite, k_writeDiscard, k_writeNoOverwrite};
		
		struct BufferDescription
		{
			u32 VertexDataCapacity;
			u32 IndexDataCapacity;
			PrimitiveType ePrimitiveType;
			BufferAccess eAccessFlag;
			BufferUsage eUsageFlag;
			CVertexDeclaration VertexLayout;
			u32 IndexSize;
		};
		
		class IMeshBuffer
		{
		public:
			IMeshBuffer(BufferDescription &inBuffDesc);
			virtual ~IMeshBuffer(){}
            //-----------------------------------------------------
            /// Bind
            ///
            /// Set the active buffer by binding to the context
            //-----------------------------------------------------
			virtual void Bind() = 0;
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
			virtual bool LockVertex(f32** outppBuffer, u32 inDataOffset, u32 inDataStride) = 0;
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
			virtual bool LockIndex(u16** outppBuffer, u32 inDataOffset, u32 inDataStride) = 0;
            //-----------------------------------------------------
            /// Unlock Vertex
            ///
            /// Releases the buffer from mapping
            /// @return If successful
            //-----------------------------------------------------
			virtual bool UnlockVertex() = 0;
            //-----------------------------------------------------
            /// Unlock Index
            ///
            /// Releases the buffer from mapping
            /// @return If successful
            //-----------------------------------------------------
			virtual bool UnlockIndex() = 0;
			
			//------------------------------------------------
			/// Set Buffer Description
			///
			/// @param buffer description
			//------------------------------------------------
			void SetBufferDescription(BufferDescription &inBuffDesc);
			//------------------------------------------------
			/// Get Buffer Description
			///
			/// @return buffer description
			//------------------------------------------------
			const BufferDescription& GetBufferDescription();
			//------------------------------------------------
			/// Get Vertex Declaration
			///
			/// @return Vertex format
			//------------------------------------------------
			CVertexDeclaration& GetVertexDeclaration();
			//------------------------------------------------
			/// Get Vertex Count
			///
			/// @return Number of vertices in buffer
			//------------------------------------------------
			const u32 GetVertexCount() const; 
			//------------------------------------------------
			/// Get Index Count
			///
			/// @return Number of indices in buffer
			//------------------------------------------------
			const u32 GetIndexCount() const;
			//------------------------------------------------
			/// Get Vertex Capacity
			///
			/// @return Size of of vertices in buffer
			//------------------------------------------------
			const u32 GetVertexCapacity() const; 
			//------------------------------------------------
			/// Get Index Capacity
			///
			/// @return Size of of indices in buffer
			//------------------------------------------------
			const u32 GetIndexCapacity() const;
			//------------------------------------------------
			/// Set Vertex Count
			///
			/// @param Total number of vertices in buffer
			//------------------------------------------------
			void SetVertexCount(u32 inVertexCount);
			//------------------------------------------------
			/// Set Index Count
			///
			/// @param Total number of indices in buffer
			//------------------------------------------------
			void SetIndexCount(u32 inIndexCount);
			//------------------------------------------------
			/// Increment Vertex Count
			///
			/// @param Add to the total number of vertices in buffer
			//------------------------------------------------
			void IncrementVertexCount(u32 inVertexCount);
			//------------------------------------------------
			/// Increment Index Count
			///
			/// @param Add to the total number of indices in buffer
			//------------------------------------------------
			void IncrementIndexCount(u32 inIndexCount);
			//------------------------------------------------
			/// Get Primitive Type
			///
			/// @return Whether the mesh uses lines, tris etc
			//------------------------------------------------
			const PrimitiveType GetPrimitiveType() const;
			//-------------------------------------------------------------------------
			/// Get Primitive Count
			///
			/// @return Number of primitives in the mesh based on the primitive type
			//-------------------------------------------------------------------------
			const u32 GetPrimitiveCount() const;

		protected:
			
			CVertexDeclaration mVertexDecl;
			BufferDescription mBufferDesc;
			
			u32 mVertexCount;
			u32 mIndexCount;
		};
	}
}

#endif