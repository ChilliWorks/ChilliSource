/*
 *  VertexLayouts.h
 *  moFloTest
 *
 *  Created by Scott Downie on 04/10/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

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