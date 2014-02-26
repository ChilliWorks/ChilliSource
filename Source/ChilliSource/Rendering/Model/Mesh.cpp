/*
 *  Mesh.cpp
 *  moFlo
 *
 *  Created by Scott Downie on 06/10/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Rendering/Model/Mesh.h>
#include <ChilliSource/Rendering/Model/SubMesh.h>
#include <ChilliSource/Rendering/Base/VertexLayouts.h>
#include <ChilliSource/Rendering/Base/RenderSystem.h>
#include <limits>
#include <ChilliSource/Core/Math/Matrix4x4.h>

#ifdef DEBUG_STATS
#include <ChilliSource/Debugging/Base/DebugStats.h>
#endif

namespace ChilliSource
{
	namespace Rendering
	{
        DEFINE_NAMED_INTERFACE(Mesh);
		//--------------------------------------------------------------------
		/// Constructor
		//--------------------------------------------------------------------
		Mesh::Mesh() 
		: mpRenderSystem(nullptr), mudwTotalVerts(0), mudwTotalIndices(0), mpSkeleton(new Skeleton())
		{
		}
		//---------------------------------------------------------------------
		/// Is A
		//---------------------------------------------------------------------
		bool Mesh::IsA(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == Mesh::InterfaceID;
		}
		//-----------------------------------------------------------------
		/// Get AABB
		//-----------------------------------------------------------------
		const Core::AABB& Mesh::GetAABB()
		{
			return mBoundingBox;
		}
		//-----------------------------------------------------------------
        /// Get Number of Vertices
        //-----------------------------------------------------------------
        u32 Mesh::GetNumVerts()
        {
            return mudwTotalVerts;
        }
        //-----------------------------------------------------------------
        /// Get Number of Indices
        //-----------------------------------------------------------------
        u32 Mesh::GetNumIndices()
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
		SubMeshSPtr Mesh::GetSubMeshAtIndex(u32 inIndex) const
		{
			if(inIndex > mSubMeshes.size())
			{
				return SubMeshSPtr();
			}
			
			return mSubMeshes[inIndex];
		}
		//-----------------------------------------------------------------
		/// Get Sub-mesh by Name
		//-----------------------------------------------------------------
		SubMeshSPtr Mesh::GetSubMeshByName(const std::string& instrName) const
		{
			for (std::vector<SubMeshSPtr>::const_iterator it = mSubMeshes.begin(); it != mSubMeshes.end(); ++it)
			{
				if ((*it)->GetName() == instrName)
					return (*it);
			}
			
			return SubMeshSPtr();
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
		void Mesh::SetBounds(const ChilliSource::Core::CVector3& invMinBounds, const ChilliSource::Core::CVector3& invMaxBounds)
		{
			//Calculate the size of this meshes bounding box
			Core::CVector3 vSize = invMaxBounds - invMinBounds;
			
			//Build our bounding box based on the size of all our sub-meshes
			mBoundingBox = Core::AABB((invMaxBounds + invMinBounds) * 0.5f, vSize);
		}
		//-----------------------------------------------------------------
		/// Render
		//-----------------------------------------------------------------
		void Mesh::Render(RenderSystem* inpRenderSystem, const Core::CMatrix4x4 &inmatWorld, const std::vector<MaterialSPtr>& inMaterials, const SkinnedAnimationGroupSPtr& inpAnimationGroup) const
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
                udwCurrMaterial = std::min(++udwCurrMaterial, (u32)inMaterials.size()-1);
                
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
                udwCurrMaterial = std::min(++udwCurrMaterial, (u32)inMaterials.size()-1);
					
#ifdef DEBUG_STATS
                DebugStats::AddToEvent("Meshes", 1);
#endif
                (*it)->Render(inpRenderSystem, inmatWorld, pMaterial, inpAnimationGroup);
			}
			
			//then transparent stuff
			udwCurrMaterial = 0;
			for(std::vector<SubMesh*>::const_iterator it = aTransparentSubMeshes.begin(); it != aTransparentSubMeshes.end(); ++it)
			{
                const MaterialSPtr& pMaterial = inMaterials[udwCurrMaterial];
                udwCurrMaterial = std::min(++udwCurrMaterial, (u32)inMaterials.size()-1);
				
#ifdef DEBUG_STATS
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