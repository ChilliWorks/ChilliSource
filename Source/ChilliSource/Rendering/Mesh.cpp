/*
 *  Mesh.cpp
 *  moFlo
 *
 *  Created by Scott Downie on 06/10/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#include <moFlo/Rendering/Mesh.h>
#include <moFlo/Rendering/SubMesh.h>
#include <moFlo/Rendering/VertexLayouts.h>
#include <moFlo/Rendering/RenderSystem.h>
#include <limits>
#include <moFlo/Core/Math/Matrix4x4.h>

#ifdef DEBUG_STATS
#include <moFlo/Core/DebugStats.h>
#endif

namespace moFlo
{
	namespace Rendering
	{
        DEFINE_NAMED_INTERFACE(CMesh);
		//--------------------------------------------------------------------
		/// Constructor
		//--------------------------------------------------------------------
		CMesh::CMesh() 
		: mpRenderSystem(NULL), mudwTotalVerts(0), mudwTotalIndices(0), mpSkeleton(new CSkeleton())
		{
		}
		//---------------------------------------------------------------------
		/// Is A
		//---------------------------------------------------------------------
		bool CMesh::IsA(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == CMesh::InterfaceID;
		}
		//-----------------------------------------------------------------
		/// Get AABB
		//-----------------------------------------------------------------
		const Core::AABB& CMesh::GetAABB()
		{
			return mBoundingBox;
		}
		//-----------------------------------------------------------------
        /// Get Number of Vertices
        //-----------------------------------------------------------------
        u32 CMesh::GetNumVerts()
        {
            return mudwTotalVerts;
        }
        //-----------------------------------------------------------------
        /// Get Number of Indices
        //-----------------------------------------------------------------
        u32 CMesh::GetNumIndices()
        {
            return mudwTotalIndices;
        }
		//-----------------------------------------------------------------
		/// Get Skeleton
		//-----------------------------------------------------------------
		const SkeletonPtr& CMesh::GetSkeletonPtr() const
		{
			return mpSkeleton;
		}
		//-----------------------------------------------------------------
		/// Get Number of Sub Meshes
		//-----------------------------------------------------------------
		u32 CMesh::GetNumSubMeshes() const
		{
			return mSubMeshes.size();
		}
		//-----------------------------------------------------------------
		/// Get Sub-mesh at Index
		//-----------------------------------------------------------------
		SubMeshPtr CMesh::GetSubMeshAtIndex(u32 inIndex) const
		{
			if(inIndex > mSubMeshes.size())
			{
				return SubMeshPtr();
			}
			
			return mSubMeshes[inIndex];
		}
		//-----------------------------------------------------------------
		/// Get Sub-mesh by Name
		//-----------------------------------------------------------------
		SubMeshPtr CMesh::GetSubMeshByName(const std::string& instrName) const
		{
			for (DYNAMIC_ARRAY<SubMeshPtr>::const_iterator it = mSubMeshes.begin(); it != mSubMeshes.end(); ++it)
			{
				if ((*it)->GetName() == instrName)
					return (*it);
			}
			
			return SubMeshPtr();
		}
        //-----------------------------------------------------------------
		/// Get Sub-mesh Index by Name
		//-----------------------------------------------------------------
		s32 CMesh::GetSubMeshIndexByName(const std::string& instrName) const
		{
            u32 dwCount = 0;
			for (DYNAMIC_ARRAY<SubMeshPtr>::const_iterator it = mSubMeshes.begin(); it != mSubMeshes.end(); ++it)
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
		SubMeshPtr CMesh::CreateSubMesh(const std::string& instrName)
		{
			SubMeshPtr newMesh(new CSubMesh(instrName));
			mSubMeshes.push_back(newMesh);
			return newMesh;
		}
		//-----------------------------------------------------------------
		/// Remove Sub Mesh By Name
		//-----------------------------------------------------------------
		void CMesh::RemoveSubMeshByName(const std::string& instrName)
		{
			int index = -1;
			int currentIndex = 0;
			for (DYNAMIC_ARRAY<SubMeshPtr>::iterator it = mSubMeshes.begin(); it != mSubMeshes.end(); ++it)
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
		void CMesh::SetBounds(const moFlo::Core::CVector3& invMinBounds, const moFlo::Core::CVector3& invMaxBounds)
		{
			//Calculate the size of this meshes bounding box
			Core::CVector3 vSize = invMaxBounds - invMinBounds;
			
			//Build our bounding box based on the size of all our sub-meshes
			mBoundingBox = Core::AABB((invMaxBounds + invMinBounds) * 0.5f, vSize);
		}
		//-----------------------------------------------------------------
		/// Render
		//-----------------------------------------------------------------
		void CMesh::Render(IRenderSystem* inpRenderSystem, const Core::CMatrix4x4 &inmatWorld, const DYNAMIC_ARRAY<MaterialPtr>& inMaterials, const SkinnedAnimationGroupPtr& inpAnimationGroup) const
		{
            MOFLOW_ASSERT(inMaterials.size() > 0, "Must have at least one material to render");

            DYNAMIC_ARRAY<CSubMesh*> aOpaqueSubMeshes;
            aOpaqueSubMeshes.reserve(mSubMeshes.size());
            
            DYNAMIC_ARRAY<CSubMesh*> aTransparentSubMeshes;
            aTransparentSubMeshes.reserve(mSubMeshes.size());
            
            //render all opaque stuff first
			u32 udwCurrMaterial = 0;
			for(DYNAMIC_ARRAY<SubMeshPtr>::const_iterator it = mSubMeshes.begin(); it != mSubMeshes.end(); ++it)
			{
                const MaterialPtr& pMaterial = inMaterials[udwCurrMaterial];
                udwCurrMaterial = std::min(++udwCurrMaterial, (u32)inMaterials.size()-1);
                
				if (pMaterial->GetActiveShaderProgram() != NULL)
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
			for(DYNAMIC_ARRAY<CSubMesh*>::const_iterator it = aOpaqueSubMeshes.begin(); it != aOpaqueSubMeshes.end(); ++it)
			{
                const MaterialPtr& pMaterial = inMaterials[udwCurrMaterial];
                udwCurrMaterial = std::min(++udwCurrMaterial, (u32)inMaterials.size()-1);
					
#ifdef DEBUG_STATS
                CDebugStats::AddToEvent("Meshes", 1);
#endif
                (*it)->Render(inpRenderSystem, inmatWorld, pMaterial, inpAnimationGroup);
			}
			
			//then transparent stuff
			udwCurrMaterial = 0;
			for(DYNAMIC_ARRAY<CSubMesh*>::const_iterator it = aTransparentSubMeshes.begin(); it != aTransparentSubMeshes.end(); ++it)
			{
                const MaterialPtr& pMaterial = inMaterials[udwCurrMaterial];
                udwCurrMaterial = std::min(++udwCurrMaterial, (u32)inMaterials.size()-1);
				
#ifdef DEBUG_STATS
                CDebugStats::AddToEvent("Meshes_Trans", 1);
#endif
                (*it)->Render(inpRenderSystem, inmatWorld, pMaterial, inpAnimationGroup);
			}
		}
		//-----------------------------------------------------------------
		/// Calc Vertex And Index Counts
		//-----------------------------------------------------------------
		void CMesh::CalcVertexAndIndexCounts()
		{
			mudwTotalVerts = 0;
			mudwTotalIndices = 0;
			
			for(DYNAMIC_ARRAY<SubMeshPtr>::const_iterator it = mSubMeshes.begin(); it != mSubMeshes.end(); ++it)
			{
				mudwTotalVerts += (*it)->GetNumVerts();
				mudwTotalIndices += (*it)->GetNumIndices();
			}
		}
		//-----------------------------------------------------------------
		/// Destructor
		//-----------------------------------------------------------------
		CMesh::~CMesh()
		{
			mSubMeshes.clear();
		}
	}
}