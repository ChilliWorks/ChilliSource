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
			mbHasTexture = false;
			mbHasMaterial = false;
			mbHasAnimationData = false;
		}
		//-------------------------------------------------------------------------
		/// Container for information required to build a sub-mesh. 
		//-------------------------------------------------------------------------
		SubMeshDescriptor::SubMeshDescriptor()
		{
			mudwNumVertices = 0;
			mudwNumIndices = 0;
			mvMinBounds = ChilliSource::Core::CVector3(0.0f, 0.0f, 0.0f);
			mvMaxBounds = ChilliSource::Core::CVector3(0.0f, 0.0f, 0.0f);
			mpVertexData = NULL;
			mpIndexData = NULL;
            ePrimitiveType = PrimitiveType::k_tri;
			meMaterialStorageLocation = Core::StorageLocation::k_none;
		}
		//-------------------------------------------------------------------------
		/// Container for information required to build a mesh.
		//-------------------------------------------------------------------------
		MeshDescriptor::MeshDescriptor()
		{
			mudwIndexSize = 0;
			mvMinBounds = ChilliSource::Core::CVector3(0.0f, 0.0f, 0.0f);
			mvMaxBounds = ChilliSource::Core::CVector3(0.0f, 0.0f, 0.0f);
		}
	}
}