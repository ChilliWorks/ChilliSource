//
//  MeshBatch.cpp
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

#include <ChilliSource/Rendering/Model/StaticMeshComponent.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Rendering/Base/MeshBatch.h>
#include <ChilliSource/Rendering/Base/MeshBuffer.h>
#include <ChilliSource/Rendering/Base/RenderSystem.h>
#include <ChilliSource/Rendering/Model/SubMesh.h>

namespace ChilliSource
{
	namespace Rendering
	{
		//------------------------------------------------------
		/// Constructor
		///
		/// Default
		//------------------------------------------------------
		MeshBatch::MeshBatch() : mpMeshBuffer(nullptr), mudwVertexCount(0), mudwIndexCount(0), mdwTag(0)
		{

		}
		//------------------------------------------------------
		/// Create Static Buffer
		///
		/// Get the render system to build a static mesh
		/// buffer.
		///
		/// @param Render system
		//------------------------------------------------------
		void MeshBatch::CreateStaticBuffer(RenderSystem* inpRenderSystem)
		{
			//Sum up the total number of vertices
			u32 VBufferSize	= 0;
			u32 IBufferSize	= 0;
			
			for(MapMeshToTransform::const_iterator it = mmapMeshCache.begin(); it != mmapMeshCache.end(); ++it)
			{
				MeshCSPtr pMesh = it->first->GetMesh();
				
				for (u32 i = 0; i < pMesh->GetNumSubMeshes(); i++)
				{
					const SubMesh* pSubMesh = pMesh->GetSubMeshAtIndex(i);
					VBufferSize += pSubMesh->GetInternalMeshBuffer()->GetVertexCapacity();
					IBufferSize += pSubMesh->GetInternalMeshBuffer()->GetIndexCapacity();
					
					//insure this is using the correct vertex declaration.
					VertexDeclaration declaration = pSubMesh->GetInternalMeshBuffer()->GetVertexDeclaration();
					if (!(declaration == VertexLayout::kMesh))
						CS_LOG_ERROR("Mesh in mesh batch is not using the correct vertex layout!");
				}
                
                mudwVertexCount += pMesh->GetNumVerts();
                mudwIndexCount += pMesh->GetNumIndices();
			}
			
			BufferDescription desc;
			desc.eUsageFlag = BufferUsage::k_static;
			desc.VertexDataCapacity = VBufferSize; 
			desc.IndexDataCapacity  = IBufferSize;
			desc.ePrimitiveType = PrimitiveType::k_tri;
			desc.eAccessFlag = BufferAccess::k_read;
			desc.VertexLayout = VertexLayout::kMesh;
			desc.IndexSize = 2;
			
			mpMeshBuffer = inpRenderSystem->CreateBuffer(desc);
			
			//Fill the mesh buffer with the vertex data
			Build();
		}
		//------------------------------------------------------
		/// Add Mesh
		///
		/// Insert a mesh into this batch.
		///
		/// @param Static mesh component
		/// @param Transform
		//------------------------------------------------------
		void MeshBatch::AddMesh(const StaticMeshComponentSPtr &inpMesh, const Core::Matrix4& inmatTransform)
		{
			mmapMeshCache.insert(std::make_pair(inpMesh, inmatTransform));
		}
		//------------------------------------------------------
		/// Build
		///
		/// Finalise the batch. Nothing can be changed once
		/// the batch is built
		//------------------------------------------------------
		void MeshBatch::Build()
		{
			//Sanity check
			if(mmapMeshCache.empty() || !mpMeshBuffer) return;
			
			mpMeshBuffer->Bind();
			
			mpMeshBuffer->SetVertexCount(mudwVertexCount);
			mpMeshBuffer->SetIndexCount(mudwIndexCount);
			
			//Get the buffer locations
			MeshBatchVertex* pVBatchBuffer = nullptr;
			mpMeshBuffer->LockVertex((f32**)&pVBatchBuffer, 0, 0);
			u16* pIBatchBuffer = nullptr;
			mpMeshBuffer->LockIndex(&pIBatchBuffer, 0, 0);
            
            MeshBatchVertex sTempVert;
			u32 VertexStride = sizeof(MeshBatchVertex);
            u32 IndexStride = sizeof(u16);
            
            u32 udwIndexOffset = 0;
            
            //---------------------------------------------------
            // Meshes
            //---------------------------------------------------
			//Load the mesh data into the buffer and offset the indices
			for(MapMeshToTransform::const_iterator it = mmapMeshCache.begin(); it != mmapMeshCache.end(); ++it)
			{
				//build the normal matrix. NOTE: This normal matrix will NOT work if there is a scale component to the transform.
				Core::Matrix4 NormalMatrix = it->second;
				NormalMatrix.m[12] = 0.0f;
				NormalMatrix.m[13] = 0.0f;
				NormalMatrix.m[14] = 0.0f;
				
				
				//---------------------------------------------------
                // Sub-Meshes
                //---------------------------------------------------
				for(auto jt = it->first->GetMesh()->m_subMeshes.begin(); jt != it->first->GetMesh()->m_subMeshes.end(); ++jt)
				{
					MeshBuffer* pSubBuffer = (*jt)->GetInternalMeshBuffer();
					pSubBuffer->Bind();
                    
                    //----------------------------------
                    // Vertices
                    //----------------------------------
                    //Loop round each vertex in the mesh and store it in the temp so we can manipulate it
                    u32 NumVerts = pSubBuffer->GetVertexCount();
                    
					//Copy the vertex data into the buffer
					MeshBatchVertex* _pVSubBuffer = nullptr;
					pSubBuffer->LockVertex((f32**)&_pVSubBuffer, 0, 0);
                    
                    //Get the vertex data and transform it before we load it into the batch buffer
                    for(u32 i=0; i<NumVerts; ++i)
                    {
                        //Grab the vertex
                        memcpy(&sTempVert, _pVSubBuffer + i, VertexStride);
                        
                        //Transform the vertex
						sTempVert.Pos = sTempVert.Pos * it->second;
						sTempVert.Norm = sTempVert.Norm * NormalMatrix;
                        
                        //Copy the vertex into our new buffer
                        memcpy(pVBatchBuffer, &sTempVert, VertexStride);
                        pVBatchBuffer++;
                    }
                    
					pSubBuffer->UnlockVertex();
                    //----------------------------------
                    //----------------------------------
					
                    //----------------------------------
                    // Indices
                    //----------------------------------
                    //Loop round each index in the mesh and store it in the temp so we can manipulate it
                    u32 NumIndices = pSubBuffer->GetIndexCount();
                    
                    //Lock the data we need to copy
                    u16* _pISubBuffer = nullptr;
                    pSubBuffer->LockIndex(&_pISubBuffer, 0, 0);
                    
                    //Get the index data and offset it before we load it into the batch buffer
                    for(u32 i=0; i<NumIndices; ++i)
                    {
                        u16 Index = 0;
                        
                        //Grab the index
                        memcpy(&Index, _pISubBuffer +  i, IndexStride);
                        
                        //Offset the index
                        Index += udwIndexOffset;
                        
                        //Copy the index into our new buffer
                        memcpy(pIBatchBuffer, &Index, IndexStride);
                        pIBatchBuffer++;
                    }
                    
                    udwIndexOffset += NumVerts;
                    
                    //Unlock and offset the new buffer
                    pSubBuffer->UnlockIndex();
                    //----------------------------------
                    //----------------------------------
				}
			}
			
			//---End mapping - Vertex
			mpMeshBuffer->UnlockVertex();
			
			//---End mapping - Index
			mpMeshBuffer->UnlockIndex();
			
			//We can now ditch our local meshes
			mmapMeshCache.clear();
		}
		
		//------------------------------------------------------
		//------------------------------------------------------
		void MeshBatch::Render(RenderSystem* inpRenderSystem, ShaderPass in_shaderPass) const
		{
			//If we own the mesh buffer then the batcher won't be calling bind for us.
			mpMeshBuffer->Bind();
		
			//Tell the render system to draw the contents of the buffer
			inpRenderSystem->ApplyMaterial(mpMaterial, in_shaderPass);

			inpRenderSystem->RenderBuffer(mpMeshBuffer, 0, mpMeshBuffer->GetIndexCount(), Core::Matrix4::k_identity);
		}
		//------------------------------------------------------
		/// Get Material
		///
		/// @return Material
		//------------------------------------------------------
		const MaterialCSPtr& MeshBatch::GetMaterial() const
		{
			return mpMaterial;
		}
		//------------------------------------------------------
		/// Set Material
		///
		/// @param Material
		//------------------------------------------------------
		void MeshBatch::SetMaterial(const MaterialCSPtr& inpMaterial)
		{
			mpMaterial = inpMaterial;
		}
		//------------------------------------------------------
		/// Get Tag
		///
		/// Associated data usually used to determine layer
		//------------------------------------------------------
		s32 MeshBatch::GetTag() const
		{
			return mdwTag;
		}
		//------------------------------------------------------
		/// Set Tag
		///
		/// Associated data usually used to determine layer
		//------------------------------------------------------
		void MeshBatch::SetTag(s32 indwValue)
		{
			mdwTag = indwValue;
		}
		//------------------------------------------------------
		/// Destructor
		///
		//------------------------------------------------------
		MeshBatch::~MeshBatch() 
		{
			CS_SAFEDELETE(mpMeshBuffer)
		}
	}
}
