//
//  MeshBuffer.h
//  Chilli Source
//  Created by Scott Downie on 30/09/2010.
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

#ifndef _CHILLISOURCE_RENDERING_MESH_BUFFER
#define _CHILLISOURCE_RENDERING_MESH_BUFFER

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
			VertexDeclaration VertexLayout;
			u32 IndexSize;
		};
		
		class MeshBuffer
		{
		public:
			MeshBuffer(BufferDescription &inBuffDesc);
			virtual ~MeshBuffer(){}
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
			VertexDeclaration& GetVertexDeclaration();
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
			
			VertexDeclaration mVertexDecl;
			BufferDescription mBufferDesc;
			
			u32 mVertexCount;
			u32 mIndexCount;
		};
	}
}

#endif