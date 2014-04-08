/*
 *  MeshBatch.cpp
 *  moFloTest
 *
 *  Created by Scott Downie on 10/12/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Rendering/Model/StaticMeshComponent.h>
#include <ChilliSource/Rendering/Model/SubMesh.h>
#include <ChilliSource/Rendering/Base/MeshBatch.h>
#include <ChilliSource/Rendering/Base/MeshBuffer.h>
#include <ChilliSource/Rendering/Base/RenderSystem.h>

#ifdef CS_ENABLE_DEBUGSTATS
#include <ChilliSource/Debugging/Base/DebugStats.h>
#endif

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
		void MeshBatch::AddMesh(const StaticMeshComponentSPtr &inpMesh, const Core::Matrix4x4& inmatTransform)
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
				Core::Matrix4x4 NormalMatrix = it->second;
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
                        Core::Matrix4x4::Multiply(&sTempVert.Pos, &it->second, &sTempVert.Pos);
                        Core::Matrix4x4::Multiply(&sTempVert.Norm, &NormalMatrix, &sTempVert.Norm);
                        
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
		/// Render
		///
		/// Draw the contents of the mesh buffer
		///
		/// @param Active render system
		//------------------------------------------------------
		void MeshBatch::Render(RenderSystem* inpRenderSystem) const
		{
            CS_ASSERT(mpMaterial && mpMaterial->GetActiveShaderProgram(), "Cannot render a mesh batch without a material or active shader.");
            
			//If we own the mesh buffer then the batcher won't be calling bind for us.
			mpMeshBuffer->Bind();
		
			//Tell the render system to draw the contents of the buffer
			inpRenderSystem->ApplyMaterial(*mpMaterial.get());
#ifdef CS_ENABLE_DEBUGSTATS
            DebugStats::AddToEvent("Verts", mpMeshBuffer->GetVertexCount()); // Guess that indices use all verts
#endif
			inpRenderSystem->RenderBuffer(mpMeshBuffer, 0, mpMeshBuffer->GetIndexCount(), Core::Matrix4x4::IDENTITY);
		}
		//------------------------------------------------------
		/// Get Material
		///
		/// @return Material
		//------------------------------------------------------
		const MaterialSPtr& MeshBatch::GetMaterial() const
		{
			return mpMaterial;
		}
		//------------------------------------------------------
		/// Set Material
		///
		/// @param Material
		//------------------------------------------------------
		void MeshBatch::SetMaterial(MaterialSPtr inpMaterial)
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