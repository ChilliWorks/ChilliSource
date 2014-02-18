/*
 *  VertexDeclaration.h
 *  moFlo
 *
 *  Created by Scott Downie on 01/10/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#ifndef _MO_FLO_RENDERING_VERTEX_DECLARATION_H_
#define _MO_FLO_RENDERING_VERTEX_DECLARATION_H_

namespace moFlo
{
	namespace Rendering
	{
		enum class VertexDataType {k_float2, k_float3, k_float4, k_byte4};
		enum class VertexDataSemantic {k_position, k_normal, k_uv, k_colour, k_weight, k_jointIndex};
		
		struct VertexElement
		{
			VertexDataType eType;
			VertexDataSemantic eSemantic;
			
			const u16 Size() const 
			{
				switch(eType)
				{
					case VertexDataType::k_float2:
						return sizeof(f32) * 2;
					case VertexDataType::k_float3:
						return sizeof(f32) * 3;
					case VertexDataType::k_float4:
						return sizeof(f32) * 4;
                    case VertexDataType::k_byte4:
                        return sizeof(u8) * 4;
					default:
						return 0;
				};
			}
			const u16 NumDataTypesPerType() const
			{
				switch(eType)
				{
					case VertexDataType::k_float2:
						return 2;
					case VertexDataType::k_float3:
						return 3;
					case VertexDataType::k_float4:
						return 4;
                    case VertexDataType::k_byte4:
                        return 4;
					default:
						return 0;
				};
			}
			
			bool operator== (const VertexElement& inOther)
			{
				if (eType == inOther.eType && eSemantic == inOther.eSemantic)
					return true;
				return false;
			}
		};
		
		//==============================================================
		/// Description:
		///
		/// Holds the vertex layouts to be used by the render buffer
		//==============================================================
		class CVertexDeclaration
		{
		public:
			CVertexDeclaration();
			CVertexDeclaration(u16 nElements, const VertexElement* inpElements);
			
			const u16 GetNumElements();
			const VertexElement& GetElementAtIndex(const u16 inIndex);
			const u16 GetSizeOfElement(VertexElement &inElement);
			const u16 GetElementOffset(const VertexElement &inElement);
			const u16 GetTotalSize();
			const u32 GetTotalNumValues();
            const DYNAMIC_ARRAY<VertexElement>& GetElements() const;
			
			bool operator== (const CVertexDeclaration& inOther);
			
		private:
			
			DYNAMIC_ARRAY<VertexElement> mElements;
			
			u16 mTotalSize;
			u32 mTotalNumValues;
		};
	}
}

#endif