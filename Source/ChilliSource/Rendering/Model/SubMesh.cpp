/*
 *  SubMesh.cpp
 *  moFlo
 *
 *  Created by Scott Downie on 08/10/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Rendering/Model/SubMesh.h>

#include <ChilliSource/Rendering/Base/RenderSystem.h>
#include <ChilliSource/Rendering/Base/VertexLayouts.h>
#include <ChilliSource/Rendering/Material/Material.h>
#include <ChilliSource/Rendering/Model/SkinnedAnimation.h>
#include <ChilliSource/Rendering/Model/SkinnedAnimationGroup.h>
#include <ChilliSource/Rendering/Texture/Texture.h>

#ifdef CS_ENABLE_DEBUGSTATS
#include <ChilliSource/Debugging/Base/DebugStats.h>
#endif

#include <limits>

namespace ChilliSource
{
	namespace Rendering
	{
		//--------------------------------------------------------------------
		/// Constructor
		//--------------------------------------------------------------------
		SubMesh::SubMesh(std::string instrName) : mstrName(instrName), mpMeshBuffer(nullptr), mpRenderSystem(nullptr)
		{
			
		}
		//-----------------------------------------------------------------
		/// Get Default Material Name
		//-----------------------------------------------------------------
		const std::string& SubMesh::GetDefaultMaterialName()
		{
			return mstrDefaultMaterialName;
		}
        //-----------------------------------------------------------------
        /// Get Default Material Storage Location
        //-----------------------------------------------------------------
        Core::StorageLocation SubMesh::GetDefaultMaterialStorageLocation()
        {
            return meDefaultStorageLocation;
        }
		//-----------------------------------------------------------------
		/// Get Internal Mesh Buffer
		//-----------------------------------------------------------------
		MeshBuffer* SubMesh::GetInternalMeshBuffer() const
		{
			return mpMeshBuffer;
		}
		//-----------------------------------------------------------------
		/// Get AABB
		//-----------------------------------------------------------------
		const Core::AABB& SubMesh::GetAABB()
		{
			return mBoundingBox;
		}
		//-----------------------------------------------------------------
		/// Get Name
		//-----------------------------------------------------------------
		const std::string& SubMesh::GetName()
		{
			return mstrName;
		}
		//-----------------------------------------------------------------
		/// Get Number of Vertices
		//-----------------------------------------------------------------
		u32 SubMesh::GetNumVerts()
		{
			if (mpMeshBuffer != nullptr)
				return mpMeshBuffer->GetVertexCount();
			else
				return 0;
		}
		//-----------------------------------------------------------------
		/// Get Number of Indices
		//-----------------------------------------------------------------
		u32 SubMesh::GetNumIndices()
		{
			if (mpMeshBuffer != nullptr)
				return mpMeshBuffer->GetIndexCount();
			else
				return 0;
		}
		//-----------------------------------------------------------------
		/// Prepare
		//-----------------------------------------------------------------
		void SubMesh::Prepare(RenderSystem* inpRenderSystem, const VertexDeclaration& inVertexDeclaration, u32 inudwIndexSizeInBytes,
							   u32 inudwVertexCapacityInBytes, u32 inudwIndexCapacityInBytes, BufferAccess inAccessFlag, PrimitiveType inPrimativeType)
		{
			mpRenderSystem = inpRenderSystem;
			
			BufferDescription desc;
			desc.eUsageFlag = BufferUsage::k_static;
			desc.VertexDataCapacity = inudwVertexCapacityInBytes; 
			desc.IndexDataCapacity  = inudwIndexCapacityInBytes;
			desc.ePrimitiveType = inPrimativeType;
			desc.eAccessFlag = inAccessFlag;
			desc.VertexLayout = inVertexDeclaration;
			desc.IndexSize = inudwIndexSizeInBytes;
			
			mpMeshBuffer = mpRenderSystem->CreateBuffer(desc);
			mpMeshBuffer->SetVertexCount(0);
			mpMeshBuffer->SetIndexCount(0); 
		}
		//-----------------------------------------------------------------
		/// Alter Buffer Declaration
		//-----------------------------------------------------------------
		void SubMesh::AlterBufferDeclaration(const VertexDeclaration& inVertexDeclaration, u32 inudwIndexSizeInBytes)
		{
			BufferDescription desc;
			desc.eUsageFlag = BufferUsage::k_static;
			desc.VertexDataCapacity = mpMeshBuffer->GetVertexCapacity(); 
			desc.IndexDataCapacity  = mpMeshBuffer->GetIndexCapacity();
			desc.ePrimitiveType = mpMeshBuffer->GetPrimitiveType();
			desc.eAccessFlag = mpMeshBuffer->GetBufferDescription().eAccessFlag;
			desc.VertexLayout = inVertexDeclaration;
			desc.IndexSize = inudwIndexSizeInBytes;
			
			mpMeshBuffer->SetBufferDescription(desc);
			mpMeshBuffer->SetVertexCount(0);
			mpMeshBuffer->SetIndexCount(0);
		}
		//-----------------------------------------------------------------
		/// Build
		//-----------------------------------------------------------------
		void SubMesh::Build(void* inpVertexData, void* inpIndexData, u32 inudwNumVertices, u32 indwNumIndices, Core::Vector3 invMin, Core::Vector3 invMax)
		{
			mpMeshBuffer->SetVertexCount(inudwNumVertices);
			mpMeshBuffer->SetIndexCount(indwNumIndices);
			
			//get the new capacities for the mesh
			u32 udwVertexDataCapacity = inudwNumVertices * mpMeshBuffer->GetVertexDeclaration().GetTotalSize(); 
			u32 udwIndexDataCapacity  = indwNumIndices * mpMeshBuffer->GetBufferDescription().IndexSize;
			
			//Bind both buffers
			mpMeshBuffer->Bind();
			
			//---Vertex Mapping
			u8* pVBuffer = nullptr;
			mpMeshBuffer->LockVertex((f32**)&pVBuffer, 0, 0);
			
			//Map the data from the cache into the buffer
			memcpy(pVBuffer, inpVertexData, udwVertexDataCapacity);
			
			//---End mapping - Vertex
			mpMeshBuffer->UnlockVertex();
			
			//---Index Mapping
            if(udwIndexDataCapacity != 0)
            {
                u16* pIBuffer = nullptr;
                mpMeshBuffer->LockIndex(&pIBuffer, 0, 0);
                
                memcpy(pIBuffer, inpIndexData, udwIndexDataCapacity);
                
                //---End mapping - Index
                mpMeshBuffer->UnlockIndex();
			}
			//Calculate the size of this meshes bounding box
			Core::Vector3 vSize = invMax - invMin;
			
			//Build our bounding box based on the size of all our sub-meshes
			mBoundingBox = Core::AABB((invMax + invMin) * 0.5f, vSize);
			
		}
		//-----------------------------------------------------------------
		/// Set Default Material Name
		//-----------------------------------------------------------------
		void SubMesh::SetDefaultMaterialName(const std::string& instrMaterialName)
		{
			mstrDefaultMaterialName = instrMaterialName;
		}
        //-----------------------------------------------------------------
        /// Set Default Material Storage Location
        //-----------------------------------------------------------------
        void SubMesh::SetDefaultMaterialStorageLocation(Core::StorageLocation ineStorageLocation)
        {
            meDefaultStorageLocation = ineStorageLocation;
        }
		//-----------------------------------------------------------------
		/// Set Skeleton Controller
		//-----------------------------------------------------------------
		void SubMesh::SetInverseBindPose(const InverseBindPosePtr& inpInverseBindPose)
		{
			mpInverseBindPose = inpInverseBindPose;
		}
		//-----------------------------------------------------------------
		/// Render
		//-----------------------------------------------------------------
		void SubMesh::Render(RenderSystem* inpRenderSystem, const Core::Matrix4x4 &inmatWorld, const MaterialSPtr& inpMaterial, const SkinnedAnimationGroupSPtr& inpAnimationGroup) const
		{
            CS_ASSERT(mpMeshBuffer->GetVertexCount() > 0, "Cannot render Sub Mesh without vertices");
            CS_ASSERT(inpMaterial.get() && inpMaterial->GetActiveShaderProgram(), "Cannot render Sub Mesh without a material or active shader.");
            
            inpRenderSystem->ApplyMaterial(*inpMaterial.get());
            
            if (inpAnimationGroup != nullptr)
            {
                //Apply inverse bind pose matrix.
                std::vector<Core::Matrix4x4> combinedMatrices;
                inpAnimationGroup->ApplyInverseBindPose(mpInverseBindPose->mInverseBindPoseMatrices, combinedMatrices);
                inpRenderSystem->ApplyJoints(combinedMatrices);
            }
			
			mpMeshBuffer->Bind();
#ifdef CS_ENABLE_DEBUGSTATS
            DebugStats::AddToEvent("Verts", mpMeshBuffer->GetVertexCount()); // Guess that indices use all verts
#endif
            if(mpMeshBuffer->GetIndexCount() > 0)
            {
                inpRenderSystem->RenderBuffer(mpMeshBuffer, 0, mpMeshBuffer->GetIndexCount(), inmatWorld);
            }
            else
            {
                inpRenderSystem->RenderVertexBuffer(mpMeshBuffer, 0, mpMeshBuffer->GetVertexCount(), inmatWorld);
            }
		}
		//-----------------------------------------------------------------
		/// Destructor
		//-----------------------------------------------------------------
		SubMesh::~SubMesh()
		{
			CS_SAFEDELETE(mpMeshBuffer);
		}
	}
}
