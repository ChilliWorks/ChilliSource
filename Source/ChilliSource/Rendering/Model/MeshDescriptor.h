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

#include <ChilliSource/Rendering/ForwardDeclarations.h>
#include <ChilliSource/Rendering/Base/MeshBuffer.h>
#include <ChilliSource/Rendering/Base/VertexDeclaration.h>
#include <ChilliSource/Core/Math/Matrix4x4.h>
#include <ChilliSource/Core/File/FileSystem.h>

namespace moFlo
{
	namespace Rendering
	{
		
		//-------------------------------------------------------------------------
		/// Describes the features used in this mesh.
		//-------------------------------------------------------------------------
		struct MeshFeatureDescriptor
		{
			MeshFeatureDescriptor();
			
			bool mbHasTexture;
			bool mbHasMaterial;
			bool mbHasAnimationData;
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
			moFlo::Core::CVector3 mvMinBounds;
			moFlo::Core::CVector3 mvMaxBounds;
			std::string mstrTextureName;
			std::string mstrMaterialName;
            Core::StorageLocation meMaterialStorageLocation;
			u8* mpVertexData;
			u8* mpIndexData;
			DYNAMIC_ARRAY<moFlo::Core::CMatrix4x4> mInverseBindPoseMatrices;
			PrimitiveType ePrimitiveType;            
		};
		//-------------------------------------------------------------------------
		/// Container for information required to build a mesh.
		//-------------------------------------------------------------------------
		struct MeshDescriptor
		{
			MeshDescriptor();
			
			MeshFeatureDescriptor mFeatures;
			CVertexDeclaration mVertexDeclaration;
			u32 mudwIndexSize;
			moFlo::Core::CVector3 mvMinBounds;
			moFlo::Core::CVector3 mvMaxBounds;
			SkeletonPtr mpSkeleton;
			DYNAMIC_ARRAY<SubMeshDescriptor> mMeshes;
		};
	}
}

#endif