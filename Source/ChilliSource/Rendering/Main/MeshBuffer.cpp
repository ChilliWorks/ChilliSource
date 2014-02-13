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


#include <ChilliSource/Rendering/MeshBuffer.h>

namespace moFlo
{
	namespace Rendering
	{
		//------------------------------------------------
		/// Constructor
		///
		/// @param Buffer description
		//------------------------------------------------
		IMeshBuffer::IMeshBuffer(BufferDescription &inBuffDesc) : mBufferDesc(inBuffDesc), mVertexDecl(inBuffDesc.VertexLayout), mVertexCount(0), mIndexCount(0)
		{

		}
		//------------------------------------------------
		/// Set Buffer Description
		///
		/// @param buffer description
		//------------------------------------------------
		void IMeshBuffer::SetBufferDescription(BufferDescription &inBuffDesc)
		{
			mBufferDesc = inBuffDesc;
			mVertexDecl = inBuffDesc.VertexLayout;
		}
		//------------------------------------------------
		/// Get Buffer Description
		///
		/// @return buffer description
		//------------------------------------------------
		const BufferDescription& IMeshBuffer::GetBufferDescription()
		{
			return mBufferDesc;
		}
		//------------------------------------------------
		/// Get Vertex Declaration
		///
		/// @return Vertex format
		//------------------------------------------------
		CVertexDeclaration& IMeshBuffer::GetVertexDeclaration() 
		{
			return mVertexDecl;
		}
		//------------------------------------------------
		/// Get Vertex Count
		///
		/// @return Number of vertices in buffer
		//------------------------------------------------
		const u32 IMeshBuffer::GetVertexCount() const
		{
			return mVertexCount;
		}
		//------------------------------------------------
		/// Get Index Count
		///
		/// @return Number of indices in buffer
		//------------------------------------------------
		const u32 IMeshBuffer::GetIndexCount() const
		{
			return mIndexCount;
		}
		//------------------------------------------------
		/// Get Vertex Capacity
		///
		/// @return Size of of vertices in buffer
		//------------------------------------------------
		const u32 IMeshBuffer::GetVertexCapacity() const
		{
			return mBufferDesc.VertexDataCapacity;
		}
		//------------------------------------------------
		/// Get Index Capacity
		///
		/// @return Size of of indices in buffer
		//------------------------------------------------
		const u32 IMeshBuffer::GetIndexCapacity() const
		{
			return mBufferDesc.IndexDataCapacity;
		}
		//------------------------------------------------
		/// Set Vertex Count
		///
		/// @param Total number of vertices in buffer
		//------------------------------------------------
		void IMeshBuffer::SetVertexCount(u32 inVertexCount)
		{
			mVertexCount = inVertexCount;
		}
		//------------------------------------------------
		/// Set Index Count
		///
		/// @param Total number of indices in buffer
		//------------------------------------------------
		void IMeshBuffer::SetIndexCount(u32 inIndexCount)
		{
			mIndexCount = inIndexCount;
		}
		//------------------------------------------------
		/// Increment Vertex Count
		///
		/// @param Add to the total number of vertices in buffer
		//------------------------------------------------
		void IMeshBuffer::IncrementVertexCount(u32 inVertexCount)
		{
			mVertexCount += inVertexCount;
		}
		//------------------------------------------------
		/// Increment Index Count
		///
		/// @param Add to the total number of indices in buffer
		//------------------------------------------------
		void IMeshBuffer::IncrementIndexCount(u32 inIndexCount)
		{
			mIndexCount += inIndexCount;
		}
		//------------------------------------------------
		/// Get Primitive Type
		///
		/// @return Whether the mesh uses lines, tris etc
		//------------------------------------------------
		const PrimitiveType IMeshBuffer::GetPrimitiveType() const
		{
			return mBufferDesc.ePrimitiveType;
		}
		//-------------------------------------------------------------------------
		/// Get Primitive Count
		///
		/// @return Number of primitives in the mesh based on the primitive type
		//-------------------------------------------------------------------------
		const u32 IMeshBuffer::GetPrimitiveCount() const
		{
			u32 nPrimitives = 0;
			
			switch (mBufferDesc.ePrimitiveType) 
			{
				case TRIS:
					nPrimitives = mIndexCount/3;
					break;
				case TRI_STRIP:
					nPrimitives = (mIndexCount - 2);
					break;
				case LINE:
					nPrimitives = mIndexCount/2;
					break; 
			}
			return nPrimitives;
		}
	}
}