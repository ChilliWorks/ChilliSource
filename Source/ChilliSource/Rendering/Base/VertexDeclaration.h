//
//  VertexDeclaration.h
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

#ifndef _CHILLISOURCE_RENDERING_VERTEX_DECLARATION_H_
#define _CHILLISOURCE_RENDERING_VERTEX_DECLARATION_H_

#include <ChilliSource/ChilliSource.h>

#include <vector>

namespace ChilliSource
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
			
			bool operator== (const VertexElement& inOther) const
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
		class VertexDeclaration
		{
		public:
			VertexDeclaration();
			VertexDeclaration(u32 nElements, const VertexElement* inpElements);
			
			const u32 GetNumElements() const;
			const VertexElement& GetElementAtIndex(const u32 inIndex) const;
			const u32 GetSizeOfElement(const VertexElement &inElement) const;
			const u32 GetElementOffset(const VertexElement &inElement) const;
			const u32 GetTotalSize() const;
			const u32 GetTotalNumValues() const;
            const std::vector<VertexElement>& GetElements() const;
			
			bool operator== (const VertexDeclaration& inOther) const;
			
		private:
			
			std::vector<VertexElement> mElements;
			
			u32 mTotalSize;
			u32 mTotalNumValues;
		};
	}
}

#endif