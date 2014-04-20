/*
 *  MeshDescriptor.cpp
 *  MoFlowSkeleton
 *
 *  Created by Ian Copland on 14/10/2011.
 *  Copyright 2011 Tag Games Ltd. All rights reserved.
 *
 */

#include <ChilliSource/Rendering/Model/MeshDescriptor.h>

namespace ChilliSource
{
	namespace Rendering
	{
		//-------------------------------------------------------------------------
		/// Describes the features used in this mesh.
		//-------------------------------------------------------------------------
		MeshFeatureDescriptor::MeshFeatureDescriptor()
		{
			mbHasAnimationData = false;
            mbHasMaterial = false;
            mbHasTexture = false;
		}
		//-------------------------------------------------------------------------
		/// Container for information required to build a sub-mesh. 
		//-------------------------------------------------------------------------
		SubMeshDescriptor::SubMeshDescriptor()
		{
			mudwNumVertices = 0;
			mudwNumIndices = 0;
			mvMinBounds = ChilliSource::Core::Vector3Old(0.0f, 0.0f, 0.0f);
			mvMaxBounds = ChilliSource::Core::Vector3Old(0.0f, 0.0f, 0.0f);
			mpVertexData = nullptr;
			mpIndexData = nullptr;
            ePrimitiveType = PrimitiveType::k_tri;
		}
		//-------------------------------------------------------------------------
		/// Container for information required to build a mesh.
		//-------------------------------------------------------------------------
		MeshDescriptor::MeshDescriptor()
		{
			mudwIndexSize = 0;
		}
	}
}