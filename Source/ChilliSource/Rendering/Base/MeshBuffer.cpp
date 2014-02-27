/*
 * File: MeshBuffer.cpp
 * Date: 28/10/2010 2010 
 * Description: 
 */

/*
 * Author: Scott Downie
 * Version: v 1.0
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */


#include <ChilliSource/Rendering/Base/MeshBuffer.h>

namespace ChilliSource
{
	namespace Rendering
	{
		//------------------------------------------------
		/// Constructor
		///
		/// @param Buffer description
		//------------------------------------------------
		MeshBuffer::MeshBuffer(BufferDescription &inBuffDesc) : mBufferDesc(inBuffDesc), mVertexDecl(inBuffDesc.VertexLayout), mVertexCount(0), mIndexCount(0)
		{

		}
		//------------------------------------------------
		/// Set Buffer Description
		///
		/// @param buffer description
		//------------------------------------------------
		void MeshBuffer::SetBufferDescription(BufferDescription &inBuffDesc)
		{
			mBufferDesc = inBuffDesc;
			mVertexDecl = inBuffDesc.VertexLayout;
		}
		//------------------------------------------------
		/// Get Buffer Description
		///
		/// @return buffer description
		//------------------------------------------------
		const BufferDescription& MeshBuffer::GetBufferDescription()
		{
			return mBufferDesc;
		}
		//------------------------------------------------
		/// Get Vertex Declaration
		///
		/// @return Vertex format
		//------------------------------------------------
		VertexDeclaration& MeshBuffer::GetVertexDeclaration() 
		{
			return mVertexDecl;
		}
		//------------------------------------------------
		/// Get Vertex Count
		///
		/// @return Number of vertices in buffer
		//------------------------------------------------
		const u32 MeshBuffer::GetVertexCount() const
		{
			return mVertexCount;
		}
		//------------------------------------------------
		/// Get Index Count
		///
		/// @return Number of indices in buffer
		//------------------------------------------------
		const u32 MeshBuffer::GetIndexCount() const
		{
			return mIndexCount;
		}
		//------------------------------------------------
		/// Get Vertex Capacity
		///
		/// @return Size of of vertices in buffer
		//------------------------------------------------
		const u32 MeshBuffer::GetVertexCapacity() const
		{
			return mBufferDesc.VertexDataCapacity;
		}
		//------------------------------------------------
		/// Get Index Capacity
		///
		/// @return Size of of indices in buffer
		//------------------------------------------------
		const u32 MeshBuffer::GetIndexCapacity() const
		{
			return mBufferDesc.IndexDataCapacity;
		}
		//------------------------------------------------
		/// Set Vertex Count
		///
		/// @param Total number of vertices in buffer
		//------------------------------------------------
		void MeshBuffer::SetVertexCount(u32 inVertexCount)
		{
			mVertexCount = inVertexCount;
		}
		//------------------------------------------------
		/// Set Index Count
		///
		/// @param Total number of indices in buffer
		//------------------------------------------------
		void MeshBuffer::SetIndexCount(u32 inIndexCount)
		{
			mIndexCount = inIndexCount;
		}
		//------------------------------------------------
		/// Increment Vertex Count
		///
		/// @param Add to the total number of vertices in buffer
		//------------------------------------------------
		void MeshBuffer::IncrementVertexCount(u32 inVertexCount)
		{
			mVertexCount += inVertexCount;
		}
		//------------------------------------------------
		/// Increment Index Count
		///
		/// @param Add to the total number of indices in buffer
		//------------------------------------------------
		void MeshBuffer::IncrementIndexCount(u32 inIndexCount)
		{
			mIndexCount += inIndexCount;
		}
		//------------------------------------------------
		/// Get Primitive Type
		///
		/// @return Whether the mesh uses lines, tris etc
		//------------------------------------------------
		const PrimitiveType MeshBuffer::GetPrimitiveType() const
		{
			return mBufferDesc.ePrimitiveType;
		}
		//-------------------------------------------------------------------------
		/// Get Primitive Count
		///
		/// @return Number of primitives in the mesh based on the primitive type
		//-------------------------------------------------------------------------
		const u32 MeshBuffer::GetPrimitiveCount() const
		{
			u32 nPrimitives = 0;
			
			switch (mBufferDesc.ePrimitiveType) 
			{
				case PrimitiveType::k_tri:
					nPrimitives = mIndexCount/3;
					break;
				case PrimitiveType::k_triStrip:
					nPrimitives = (mIndexCount - 2);
					break;
				case PrimitiveType::k_line:
					nPrimitives = mIndexCount/2;
					break; 
			}
			return nPrimitives;
		}
	}
}