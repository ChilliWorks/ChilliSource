/*
 *  MeshDescriptor.h
 *  MoFlowSkeleton
 *
 *  Created by Ian Copland on 14/10/2011.
 *  Copyright 2011 Tag Games Ltd. All rights reserved.
 *
 */

#ifndef _MOFLOW_RENDERING_MESHDESCRIPTOR_H_
#define _MOFLOW_RENDERING_MESHDESCRIPTOR_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Rendering/Base/MeshBuffer.h>
#include <ChilliSource/Rendering/Base/VertexDeclaration.h>
#include <ChilliSource/Core/Math/Matrix4x4.h>
#include <ChilliSource/Core/File/FileSystem.h>

namespace ChilliSource
{
	namespace Rendering
	{
		
		//-------------------------------------------------------------------------
		/// Describes the features used in this mesh.
		//-------------------------------------------------------------------------
		struct MeshFeatureDescriptor
		{
			MeshFeatureDescriptor();
			
			bool mbHasAnimationData;
            bool mbHasMaterial;
            bool mbHasTexture;
		};
		//-------------------------------------------------------------------------
		/// Container for information required to build a sub-mesh. 
		//-------------------------------------------------------------------------
		struct SubMeshDescriptor
		{
			SubMeshDescriptor();
			
			std::string mstrName;
			u32 mudwNumVertices;
			u32 mudwNumIndices;
			ChilliSource::Core::Vector3 mvMinBounds;
			ChilliSource::Core::Vector3 mvMaxBounds;
			u8* mpVertexData;
			u8* mpIndexData;
			std::vector<ChilliSource::Core::Matrix4x4> mInverseBindPoseMatrices;
			PrimitiveType ePrimitiveType;            
		};
		//-------------------------------------------------------------------------
		/// Container for information required to build a mesh.
		//-------------------------------------------------------------------------
		struct MeshDescriptor
		{
			MeshDescriptor();
			
			MeshFeatureDescriptor mFeatures;
			VertexDeclaration mVertexDeclaration;
			u32 mudwIndexSize;
			ChilliSource::Core::Vector3 mvMinBounds;
			ChilliSource::Core::Vector3 mvMaxBounds;
			SkeletonSPtr mpSkeleton;
			std::vector<SubMeshDescriptor> mMeshes;
		};
	}
}

#endif