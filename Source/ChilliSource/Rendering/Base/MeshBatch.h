//
//  MeshBatch.h
//  Chilli Source
//  Created by Scott Downie on 10/12/2010.
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

#ifndef _CHILLISOURCE_RENDERING_MESH_BATCH_H_
#define _CHILLISOURCE_RENDERING_MESH_BATCH_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Rendering/Base/VertexLayouts.h>

namespace ChilliSource
{
	namespace Rendering
	{
		//--------------------------------------------------------------
		/// Vertex format for mesh batches. Mesh batches must be in this
		/// format.
		//--------------------------------------------------------------
		struct MeshBatchVertex
		{
			MeshBatchVertex(){}
			MeshBatchVertex(const Core::Vector4 &invPos, const Core::Vector3 &invNorm, const Core::Vector2 &invTexCoord)
			: Pos(invPos), Norm(invNorm), Tex(invTexCoord)
            {
				
            }
			Core::Vector4 Pos;
			Core::Vector3 Norm;
			Core::Vector2 Tex;
		};
		
		
		class MeshBatch
		{
		public:
			MeshBatch();
			~MeshBatch();
			//------------------------------------------------------
			/// Add Mesh
			///
			/// Insert a mesh into this batch.
			///
			/// @param Static mesh component
			/// @param Transform
			//------------------------------------------------------
			void AddMesh(const StaticMeshComponentSPtr &inpSprite, const Core::Matrix4& inmatTransform);
			//------------------------------------------------------
			/// Set Material
			///
			/// @param Material
			//------------------------------------------------------
			void SetMaterial(const MaterialCSPtr& inpMaterial);
			//------------------------------------------------------
			/// Get Material
			///
			/// @return Material
			//------------------------------------------------------
			const MaterialCSPtr& GetMaterial() const;
			//------------------------------------------------------
			/// Render
			///
			/// Draw the contents of the mesh buffer
			///
			/// @param Active render system
            /// @param Shader pass
			//------------------------------------------------------
			void Render(RenderSystem* inpRenderSystem, ShaderPass in_shaderPass) const;
			//------------------------------------------------------
			/// Get Tag
			///
			/// Associated data usually used to determine layer
			//------------------------------------------------------
			s32 GetTag() const;
			//------------------------------------------------------
			/// Set Tag
			///
			/// Associated data usually used to determine layer
			//------------------------------------------------------
			void SetTag(s32 indwValue);
			//------------------------------------------------------
			/// Create Static Buffer
			///
			/// Get the render system to build a static mesh
			/// buffer.
			///
			/// @param Render system
			//------------------------------------------------------
			void CreateStaticBuffer(RenderSystem* inpRenderSystem);
			//------------------------------------------------------
			/// Build
			///
			/// Finalise the batch. Nothing can be changed once
			/// the batch is builtı
			//------------------------------------------------------
			void Build();

		private:
			
			s32 mdwTag;
			
			u32 mudwVertexCount;
			u32 mudwIndexCount;
			
			//The shared material
			MaterialCSPtr mpMaterial;
			
			//---Render Buffer
			MeshBuffer* mpMeshBuffer;
			
			//List of sprites in the batch
			typedef std::unordered_map<StaticMeshComponentSPtr, Core::Matrix4> MapMeshToTransform;
			MapMeshToTransform mmapMeshCache;
		};
	}
}

#endif