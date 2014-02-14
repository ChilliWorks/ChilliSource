/*
 *  VertexLayouts.h
 *  moFloTest
 *
 *  Created by Scott Downie on 04/10/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Rendering/Base/VertexDeclaration.h>

namespace moFlo
{
	namespace Rendering
	{
		namespace VertexLayout
		{
			//static const VertexElement kMeshElements[]		= {{FLOAT4,POSITION},{FLOAT3,NORMAL},{FLOAT2,UV},{BYTE4,COLOUR}};
            static const VertexElement kMeshElements[]		= {{FLOAT4,POSITION},{FLOAT3,NORMAL},{FLOAT2,UV}};
			static const VertexElement kSpriteElements[]	= {{FLOAT4,POSITION},{FLOAT2,UV},{BYTE4,COLOUR}};
			static const VertexElement kPosColElement[]		= {{FLOAT4,POSITION},{BYTE4,COLOUR}};
			
			//static const CVertexDeclaration kMesh(4,kMeshElements);
            static const CVertexDeclaration kMesh(3,kMeshElements);
			static const CVertexDeclaration kSprite(3,kSpriteElements);
			static const CVertexDeclaration kPosCol(2,kPosColElement);
		}
	}
}