//
//  Mesh.cpp
//  Chilli Source
//
//  Created by Scott Downie on 06/10/2010.
//  Copyright 2010 Tag Games. All rights reserved.
//

#include <ChilliSource/Rendering/Model/Mesh.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Rendering/Base/MeshBuffer.h>
#include <ChilliSource/Rendering/Base/RenderSystem.h>
#include <ChilliSource/Rendering/Material/Material.h>
#include <ChilliSource/Rendering/Model/MeshDescriptor.h>
#include <ChilliSource/Rendering/Model/SubMesh.h>
#include <ChilliSource/Rendering/Model/Skeleton.h>

#ifdef CS_ENABLE_DEBUGSTATS
#include <ChilliSource/Debugging/Base/DebugStats.h>
#endif

namespace ChilliSource
{
	namespace Rendering
	{
        CS_DEFINE_NAMEDTYPE(Mesh);
        //---------------------------------------------------------------------
        //---------------------------------------------------------------------
        MeshUPtr Mesh::Create()
        {
            return MeshUPtr(new Mesh());
        }
		//--------------------------------------------------------------------
		//--------------------------------------------------------------------
		Mesh::Mesh()
		: m_totalNumVerts(0), m_totalNumIndices(0), m_skeleton(new Skeleton())
		{
		}
		//---------------------------------------------------------------------
		//---------------------------------------------------------------------
		bool Mesh::IsA(Core::InterfaceIDType in_interfaceId) const
		{
			return in_interfaceId == Mesh::InterfaceID;
		}
        //----------------------------------------------------------------------------
        //----------------------------------------------------------------------------
        bool Mesh::Build(const MeshDescriptor& in_meshDesc)
        {
            bool bSuccess = true;
            
            //set the bounds
            SetBounds(in_meshDesc.mvMinBounds, in_meshDesc.mvMaxBounds);
            
            if (in_meshDesc.mFeatures.mbHasAnimationData == true)
            {
                m_skeleton = in_meshDesc.mpSkeleton;
            }
            
            //iterate through each mesh
            int count = 0;
            for (auto it = in_meshDesc.mMeshes.begin(); it != in_meshDesc.mMeshes.end(); ++it)
            {
                //caclulate the mesh capacities
                u32 udwVertexDataCapacity = it->mudwNumVertices * in_meshDesc.mVertexDeclaration.GetTotalSize();
                u32 udwIndexDataCapacity  = it->mudwNumIndices * in_meshDesc.mudwIndexSize;
                
                //prepare the mesh if it needs it, otherwise just update the vertex and index declarations.
                SubMeshSPtr	newSubMesh = CreateSubMesh(it->mstrName);
                newSubMesh->Prepare(Core::Application::Get()->GetRenderSystem(), in_meshDesc.mVertexDeclaration, in_meshDesc.mudwIndexSize, udwVertexDataCapacity, udwIndexDataCapacity, BufferAccess::k_read, it->ePrimitiveType);
                
                //check that the buffers are big enough to hold this data. if not throw an error.
                if (udwVertexDataCapacity <= newSubMesh->GetInternalMeshBuffer()->GetVertexCapacity() &&
                    udwIndexDataCapacity <= newSubMesh->GetInternalMeshBuffer()->GetIndexCapacity())
                {
                    newSubMesh->Build(it->mpVertexData, it->mpIndexData, it->mudwNumVertices, it->mudwNumIndices, it->mvMinBounds, it->mvMaxBounds);
                }
                else
                {
                    CS_LOG_ERROR("Sub mesh data exceeds its buffer capacity. Mesh will return empty!");
                    bSuccess = false;
                }
                
                //add the skeleton controller
                if (in_meshDesc.mFeatures.mbHasAnimationData == true)
                {
                    InverseBindPosePtr ibp(new InverseBindPose());
                    ibp->mInverseBindPoseMatrices = it->mInverseBindPoseMatrices;
                    newSubMesh->SetInverseBindPose(ibp);
                }
                
                count++;
            }
            
            CalcVertexAndIndexCounts();
            
            //return success
            return bSuccess;
        }
		//-----------------------------------------------------------------
		//-----------------------------------------------------------------
		const Core::AABB& Mesh::GetAABB() const
		{
			return m_aabb;
		}
		//-----------------------------------------------------------------
        //-----------------------------------------------------------------
        u32 Mesh::GetNumVerts() const
        {
            return m_totalNumVerts;
        }
        //-----------------------------------------------------------------
        //-----------------------------------------------------------------
        u32 Mesh::GetNumIndices() const
        {
            return m_totalNumIndices;
        }
		//-----------------------------------------------------------------
		//-----------------------------------------------------------------
		const SkeletonSPtr& Mesh::GetSkeletonPtr() const
		{
			return m_skeleton;
		}
		//-----------------------------------------------------------------
		//-----------------------------------------------------------------
		u32 Mesh::GetNumSubMeshes() const
		{
			return m_subMeshes.size();
		}
		//-----------------------------------------------------------------
		//-----------------------------------------------------------------
		SubMeshCSPtr Mesh::GetSubMeshAtIndex(u32 in_index) const
		{
            CS_ASSERT(in_index < m_subMeshes.size(), "Sub mesh index out of bounds");
			return m_subMeshes[in_index];
		}
		//-----------------------------------------------------------------
		//-----------------------------------------------------------------
		SubMeshCSPtr Mesh::GetSubMeshByName(const std::string& in_name) const
		{
			for (auto it = m_subMeshes.begin(); it != m_subMeshes.end(); ++it)
			{
				if ((*it)->GetName() == in_name)
					return (*it);
			}
			
			return SubMeshCSPtr();
		}
        //-----------------------------------------------------------------
		//-----------------------------------------------------------------
		s32 Mesh::GetSubMeshIndexByName(const std::string& in_name) const
		{
            for(u32 i=0; i<m_subMeshes.size(); ++i)
            {
                if(m_subMeshes[i]->GetName() == in_name)
                {
                    return i;
                }
            }
            
            return -1;
		}
        //-----------------------------------------------------------------
		//-----------------------------------------------------------------
		SubMeshSPtr Mesh::CreateSubMesh(const std::string& in_name)
		{
			SubMeshSPtr newMesh(new SubMesh(in_name));
			m_subMeshes.push_back(newMesh);
			return newMesh;
		}
		//-----------------------------------------------------------------
		//-----------------------------------------------------------------
		void Mesh::RemoveSubMeshByName(const std::string& in_name)
		{
			for (auto it = m_subMeshes.begin(); it != m_subMeshes.end(); ++it)
			{
				if ((*it)->GetName() == in_name)
                {
                    m_subMeshes.erase(it);
                    return;
                }
			}
		}
		//-----------------------------------------------------------------
		//-----------------------------------------------------------------
		void Mesh::SetBounds(const Core::Vector3& in_minBounds, const Core::Vector3& in_maxBounds)
		{
			//Calculate the size of this meshes bounding box
			Core::Vector3 vSize = in_maxBounds - in_minBounds;
			
			//Build our bounding box based on the size of all our sub-meshes
			m_aabb = Core::AABB((in_maxBounds + in_minBounds) * 0.5f, vSize);
		}
		//-----------------------------------------------------------------
		//-----------------------------------------------------------------
		void Mesh::Render(RenderSystem* in_renderSystem, const Core::Matrix4x4& in_worldMat, const std::vector<MaterialSPtr>& in_materials, const SkinnedAnimationGroupSPtr& in_animGroup) const
		{
            CS_ASSERT(in_materials.size() > 0, "Must have at least one material to render");

            std::vector<SubMesh*> aOpaqueSubMeshes;
            aOpaqueSubMeshes.reserve(m_subMeshes.size());
            
            std::vector<SubMesh*> aTransparentSubMeshes;
            aTransparentSubMeshes.reserve(m_subMeshes.size());
            
            //render all opaque stuff first
			u32 udwCurrMaterial = 0;
			for(auto it = m_subMeshes.begin(); it != m_subMeshes.end(); ++it)
			{
                const MaterialSPtr& pMaterial = in_materials[udwCurrMaterial];
                ++udwCurrMaterial;
                udwCurrMaterial = (u32)std::min(udwCurrMaterial, (u32)in_materials.size()-1);
                
				if (pMaterial->GetActiveShaderProgram() != nullptr)
                {
                    if(pMaterial->IsTransparent() == false)
                    {
                        aOpaqueSubMeshes.push_back(it->get());
                    }
                    else
                    {
                        aTransparentSubMeshes.push_back(it->get());
                    }
                }
			}
            
			//render all opaque stuff first
            udwCurrMaterial = 0;
			for(auto it = aOpaqueSubMeshes.begin(); it != aOpaqueSubMeshes.end(); ++it)
			{
                const MaterialSPtr& pMaterial = in_materials[udwCurrMaterial];
                ++udwCurrMaterial;
                udwCurrMaterial = std::min(udwCurrMaterial, (u32)in_materials.size()-1);
					
#ifdef CS_ENABLE_DEBUGSTATS
                DebugStats::AddToEvent("Meshes", 1);
#endif
                (*it)->Render(in_renderSystem, in_worldMat, pMaterial, in_animGroup);
			}
			
			//then transparent stuff
			udwCurrMaterial = 0;
			for(auto it = aTransparentSubMeshes.begin(); it != aTransparentSubMeshes.end(); ++it)
			{
                const MaterialSPtr& pMaterial = in_materials[udwCurrMaterial];
                ++udwCurrMaterial;
                udwCurrMaterial = (u32)std::min(udwCurrMaterial, (u32)in_materials.size()-1);
				
#ifdef CS_ENABLE_DEBUGSTATS
                DebugStats::AddToEvent("Meshes_Trans", 1);
#endif
                (*it)->Render(in_renderSystem, in_worldMat, pMaterial, in_animGroup);
			}
		}
		//-----------------------------------------------------------------
		//-----------------------------------------------------------------
		void Mesh::CalcVertexAndIndexCounts()
		{
			m_totalNumVerts = 0;
			m_totalNumIndices = 0;
			
			for(auto it = m_subMeshes.begin(); it != m_subMeshes.end(); ++it)
			{
				m_totalNumVerts += (*it)->GetNumVerts();
				m_totalNumIndices += (*it)->GetNumIndices();
			}
		}
	}
}
