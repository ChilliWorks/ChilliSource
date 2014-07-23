//
//  VertexLayouts.h
//  Chilli Source
//  Created by Scott Downie on 04/10/2010.
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

#ifndef _CHILLISOURCE_RENDERING_BASE_VERTEXLAYOUTS_H_
#define _CHILLISOURCE_RENDERING_BASE_VERTEXLAYOUTS_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Rendering/Base/VertexDeclaration.h>

namespace ChilliSource
{
	namespace Rendering
	{
		namespace VertexLayout
		{
            static const VertexElement kMeshElements[]	 = {{VertexDataType::k_float4, VertexDataSemantic::k_position}, {VertexDataType::k_float3, VertexDataSemantic::k_normal}, {VertexDataType::k_float2, VertexDataSemantic::k_uv}};
			static const VertexElement kSpriteElements[] = {{VertexDataType::k_float4, VertexDataSemantic::k_position}, {VertexDataType::k_float2, VertexDataSemantic::k_uv},     {VertexDataType::k_byte4, VertexDataSemantic::k_colour}};
			static const VertexElement kPosColElement[]	 = {{VertexDataType::k_float4, VertexDataSemantic::k_position}, {VertexDataType::k_byte4, VertexDataSemantic::k_colour}};
			
			//static const VertexDeclaration kMesh(4,kMeshElements);
            static const VertexDeclaration kMesh(3,kMeshElements);
			static const VertexDeclaration kSprite(3,kSpriteElements);
			static const VertexDeclaration kPosCol(2,kPosColElement);
		}
	}
}

#endif