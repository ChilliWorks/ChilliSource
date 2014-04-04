/*
 *  Mesh.cpp
 *  moFlo
 *
 *  Created by Scott Downie on 06/10/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Rendering/Model/Mesh.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Math/Matrix4x4.h>
#include <ChilliSource/Rendering/Base/RenderSystem.h>
#include <ChilliSource/Rendering/Base/VertexLayouts.h>
#include <ChilliSource/Rendering/Material/Material.h>
#include <ChilliSource/Rendering/Model/MeshDescriptor.h>
#include <ChilliSource/Rendering/Model/SubMesh.h>

#ifdef CS_ENABLE_DEBUGSTATS
#include <ChilliSource/Debugging/Base/DebugStats.h>
#endif

#include <limits>

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
		: mudwTotalVerts(0), mudwTotalIndices(0), mpSkeleton(new Skeleton())
		{
		}
		//---------------------------------------------------------------------
		/// Is A
		//---------------------------------------------------------------------
		bool Mesh::IsA(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == Mesh::InterfaceID;
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
                mpSkeleton = in_meshDesc.mpSkeleton;
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
		/// Get AABB
		//-----------------------------------------------------------------
		const Core::AABB& Mesh::GetAABB() const
		{
			return mBoundingBox;
		}
		//-----------------------------------------------------------------
        /// Get Number of Vertices
        //-----------------------------------------------------------------
        u32 Mesh::GetNumVerts() const
        {
            return mudwTotalVerts;
        }
        //-----------------------------------------------------------------
        /// Get Number of Indices
        //-----------------------------------------------------------------
        u32 Mesh::GetNumIndices() const
        {
            return mudwTotalIndices;
        }
		//-----------------------------------------------------------------
		/// Get Skeleton
		//-----------------------------------------------------------------
		const SkeletonSPtr& Mesh::GetSkeletonPtr() const
		{
			return mpSkeleton;
		}
		//-----------------------------------------------------------------
		/// Get Number of Sub Meshes
		//-----------------------------------------------------------------
		u32 Mesh::GetNumSubMeshes() const
		{
			return mSubMeshes.size();
		}
		//-----------------------------------------------------------------
		/// Get Sub-mesh at Index
		//-----------------------------------------------------------------
		SubMeshCSPtr Mesh::GetSubMeshAtIndex(u32 inIndex) const
		{
            CS_ASSERT(inIndex < mSubMeshes.size(), "Sub mesh index out of bounds");
			return mSubMeshes[inIndex];
		}
		//-----------------------------------------------------------------
		/// Get Sub-mesh by Name
		//-----------------------------------------------------------------
		SubMeshCSPtr Mesh::GetSubMeshByName(const std::string& instrName) const
		{
			for (std::vector<SubMeshSPtr>::const_iterator it = mSubMeshes.begin(); it != mSubMeshes.end(); ++it)
			{
				if ((*it)->GetName() == instrName)
					return (*it);
			}
			
			return SubMeshCSPtr();
		}
        //-----------------------------------------------------------------
		/// Get Sub-mesh Index by Name
		//-----------------------------------------------------------------
		s32 Mesh::GetSubMeshIndexByName(const std::string& instrName) const
		{
            u32 dwCount = 0;
			for (std::vector<SubMeshSPtr>::const_iterator it = mSubMeshes.begin(); it != mSubMeshes.end(); ++it)
			{
				if ((*it)->GetName() == instrName)
					return dwCount;
                dwCount++;
			}
			
			return -1;
		}
        //-----------------------------------------------------------------
		/// Create Sub Mesh
		//-----------------------------------------------------------------
		SubMeshSPtr Mesh::CreateSubMesh(const std::string& instrName)
		{
			SubMeshSPtr newMesh(new SubMesh(instrName));
			mSubMeshes.push_back(newMesh);
			return newMesh;
		}
		//-----------------------------------------------------------------
		/// Remove Sub Mesh By Name
		//-----------------------------------------------------------------
		void Mesh::RemoveSubMeshByName(const std::string& instrName)
		{
			int index = -1;
			int currentIndex = 0;
			for (std::vector<SubMeshSPtr>::iterator it = mSubMeshes.begin(); it != mSubMeshes.end(); ++it)
			{
				if ((*it)->GetName() == instrName)
					index = currentIndex;
				currentIndex++;
			}
			
			if (index != -1)
			{
				mSubMeshes.erase(mSubMeshes.begin() + index);
			}
		}
		//-----------------------------------------------------------------
		/// Set Bounds
		//-----------------------------------------------------------------
		void Mesh::SetBounds(const ChilliSource::Core::Vector3& invMinBounds, const ChilliSource::Core::Vector3& invMaxBounds)
		{
			//Calculate the size of this meshes bounding box
			Core::Vector3 vSize = invMaxBounds - invMinBounds;
			
			//Build our bounding box based on the size of all our sub-meshes
			mBoundingBox = Core::AABB((invMaxBounds + invMinBounds) * 0.5f, vSize);
		}
		//-----------------------------------------------------------------
		/// Render
		//-----------------------------------------------------------------
		void Mesh::Render(RenderSystem* inpRenderSystem, const Core::Matrix4x4 &inmatWorld, const std::vector<MaterialSPtr>& inMaterials, const SkinnedAnimationGroupSPtr& inpAnimationGroup) const
		{
            CS_ASSERT(inMaterials.size() > 0, "Must have at least one material to render");

            std::vector<SubMesh*> aOpaqueSubMeshes;
            aOpaqueSubMeshes.reserve(mSubMeshes.size());
            
            std::vector<SubMesh*> aTransparentSubMeshes;
            aTransparentSubMeshes.reserve(mSubMeshes.size());
            
            //render all opaque stuff first
			u32 udwCurrMaterial = 0;
			for(std::vector<SubMeshSPtr>::const_iterator it = mSubMeshes.begin(); it != mSubMeshes.end(); ++it)
			{
                const MaterialSPtr& pMaterial = inMaterials[udwCurrMaterial];
                ++udwCurrMaterial;
                udwCurrMaterial = (u32)std::min(udwCurrMaterial, (u32)inMaterials.size()-1);
                
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
			for(std::vector<SubMesh*>::const_iterator it = aOpaqueSubMeshes.begin(); it != aOpaqueSubMeshes.end(); ++it)
			{
                const MaterialSPtr& pMaterial = inMaterials[udwCurrMaterial];
                ++udwCurrMaterial;
                udwCurrMaterial = std::min(udwCurrMaterial, (u32)inMaterials.size()-1);
					
#ifdef CS_ENABLE_DEBUGSTATS
                DebugStats::AddToEvent("Meshes", 1);
#endif
                (*it)->Render(inpRenderSystem, inmatWorld, pMaterial, inpAnimationGroup);
			}
			
			//then transparent stuff
			udwCurrMaterial = 0;
			for(std::vector<SubMesh*>::const_iterator it = aTransparentSubMeshes.begin(); it != aTransparentSubMeshes.end(); ++it)
			{
                const MaterialSPtr& pMaterial = inMaterials[udwCurrMaterial];
                ++udwCurrMaterial;
                udwCurrMaterial = (u32)std::min(udwCurrMaterial, (u32)inMaterials.size()-1);
				
#ifdef CS_ENABLE_DEBUGSTATS
                DebugStats::AddToEvent("Meshes_Trans", 1);
#endif
                (*it)->Render(inpRenderSystem, inmatWorld, pMaterial, inpAnimationGroup);
			}
		}
		//-----------------------------------------------------------------
		/// Calc Vertex And Index Counts
		//-----------------------------------------------------------------
		void Mesh::CalcVertexAndIndexCounts()
		{
			mudwTotalVerts = 0;
			mudwTotalIndices = 0;
			
			for(std::vector<SubMeshSPtr>::const_iterator it = mSubMeshes.begin(); it != mSubMeshes.end(); ++it)
			{
				mudwTotalVerts += (*it)->GetNumVerts();
				mudwTotalIndices += (*it)->GetNumIndices();
			}
		}
		//-----------------------------------------------------------------
		/// Destructor
		//-----------------------------------------------------------------
		Mesh::~Mesh()
		{
			mSubMeshes.clear();
		}
	}
}
