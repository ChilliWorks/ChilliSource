//
//  MeshDescriptor.h
//  Chilli Source
//  Created by Ian Copland on 14/10/2011.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2011 Tag Games Limited
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

#ifndef _CHILLISOURCE_RENDERING_MESHDESCRIPTOR_H_
#define _CHILLISOURCE_RENDERING_MESHDESCRIPTOR_H_

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
			CSCore::Vector3 mvMinBounds;
			CSCore::Vector3 mvMaxBounds;
			u8* mpVertexData;
			u8* mpIndexData;
			std::vector<CSCore::Matrix4> mInverseBindPoseMatrices;
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