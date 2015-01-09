//
//  SubMesh.cpp
//  Chilli Source
//  Created by Scott Downie on 08/10/2010.
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

#include <ChilliSource/Rendering/Model/SubMesh.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Rendering/Base/RenderSystem.h>
#include <ChilliSource/Rendering/Base/VertexLayouts.h>
#include <ChilliSource/Rendering/Material/Material.h>
#include <ChilliSource/Rendering/Model/SkinnedAnimation.h>
#include <ChilliSource/Rendering/Model/SkinnedAnimationGroup.h>
#include <ChilliSource/Rendering/Texture/Texture.h>

#include <limits>

namespace ChilliSource
{
	namespace Rendering
	{
		//--------------------------------------------------------------------
		/// Constructor
		//--------------------------------------------------------------------
		SubMesh::SubMesh(const std::string& instrName) : mstrName(instrName), mpMeshBuffer(nullptr)
		{
			
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
		const Core::AABB& SubMesh::GetAABB() const
		{
			return mBoundingBox;
		}
		//-----------------------------------------------------------------
		/// Get Name
		//-----------------------------------------------------------------
		const std::string& SubMesh::GetName() const
		{
			return mstrName;
		}
		//-----------------------------------------------------------------
		/// Get Number of Vertices
		//-----------------------------------------------------------------
		u32 SubMesh::GetNumVerts() const
		{
			if (mpMeshBuffer != nullptr)
				return mpMeshBuffer->GetVertexCount();
			else
				return 0;
		}
		//-----------------------------------------------------------------
		/// Get Number of Indices
		//-----------------------------------------------------------------
		u32 SubMesh::GetNumIndices() const
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
			BufferDescription desc;
			desc.eUsageFlag = BufferUsage::k_static;
			desc.VertexDataCapacity = inudwVertexCapacityInBytes; 
			desc.IndexDataCapacity  = inudwIndexCapacityInBytes;
			desc.ePrimitiveType = inPrimativeType;
			desc.eAccessFlag = inAccessFlag;
			desc.VertexLayout = inVertexDeclaration;
			desc.IndexSize = inudwIndexSizeInBytes;
			
			mpMeshBuffer = inpRenderSystem->CreateBuffer(desc);
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
		/// Set Skeleton Controller
		//-----------------------------------------------------------------
		void SubMesh::SetInverseBindPose(const InverseBindPosePtr& inpInverseBindPose)
		{
			mpInverseBindPose = inpInverseBindPose;
		}
		//-----------------------------------------------------------------
		/// Render
		//-----------------------------------------------------------------
		void SubMesh::Render(RenderSystem* inpRenderSystem, const Core::Matrix4 &inmatWorld, const MaterialCSPtr& inpMaterial, ShaderPass in_shaderPass, const SkinnedAnimationGroupSPtr& inpAnimationGroup) const
		{
            CS_ASSERT(mpMeshBuffer->GetVertexCount() > 0, "Cannot render Sub Mesh without vertices");
            
            if (inpMaterial->GetShader(in_shaderPass) != nullptr)
            {
                inpRenderSystem->ApplyMaterial(inpMaterial, in_shaderPass);
                
                if (inpAnimationGroup != nullptr)
                {
                    //Apply inverse bind pose matrix.
                    std::vector<Core::Matrix4> combinedMatrices;
                    inpAnimationGroup->ApplyInverseBindPose(mpInverseBindPose->mInverseBindPoseMatrices, combinedMatrices);
                    inpRenderSystem->ApplyJoints(combinedMatrices);
                }
                
                mpMeshBuffer->Bind();

                if(mpMeshBuffer->GetIndexCount() > 0)
                {
                    inpRenderSystem->RenderBuffer(mpMeshBuffer, 0, mpMeshBuffer->GetIndexCount(), inmatWorld);
                }
                else
                {
                    inpRenderSystem->RenderVertexBuffer(mpMeshBuffer, 0, mpMeshBuffer->GetVertexCount(), inmatWorld);
                }
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
