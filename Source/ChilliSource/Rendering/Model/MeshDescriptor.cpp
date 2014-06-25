//
//  MeshDescriptor.cpp
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
			mvMinBounds = CSCore::Vector3(0.0f, 0.0f, 0.0f);
			mvMaxBounds = CSCore::Vector3(0.0f, 0.0f, 0.0f);
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