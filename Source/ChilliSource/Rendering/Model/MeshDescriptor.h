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
#include <ChilliSource/Core/Math/Matrix4.h>
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
			std::vector<ChilliSource::Core::Matrix4> mInverseBindPoseMatrices;
			PrimitiveType ePrimitiveType;            
		};
        //-------------------------------------------------------------------------
		/// Container for information required to build a skeleton.
		//-------------------------------------------------------------------------
		struct SkeletonDescriptor
		{
            std::vector<std::string> m_nodeNames;
            std::vector<s32> m_parentNodeIndices;
            std::vector<s32> m_jointIndices;
		};
		//-------------------------------------------------------------------------
		/// Container for information required to build a mesh.
		//-------------------------------------------------------------------------
		struct MeshDescriptor
		{
			MeshDescriptor();
			
            SkeletonDescriptor m_skeletonDesc;
			MeshFeatureDescriptor mFeatures;
            std::vector<SubMeshDescriptor> mMeshes;
			VertexDeclaration mVertexDeclaration;
			Core::Vector3 mvMinBounds;
			Core::Vector3 mvMaxBounds;
            u32 mudwIndexSize;
		};
	}
}

#endif