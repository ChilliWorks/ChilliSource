/*
 *  Mesh.h
 *  moFlo
 *
 *  Created by Scott Downie on 06/10/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#ifndef _MO_FLO_RENDERING_MESH_H_
#define _MO_FLO_RENDERING_MESH_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Rendering/Base/MeshBuffer.h>
#include <ChilliSource/Core/Resource/Resource.h>
#include <ChilliSource/Core/Base/Colour.h>
#include <ChilliSource/Core/Math/Vector2.h>
#include <ChilliSource/Core/Math/Vector3.h>
#include <ChilliSource/Core/Math/Vector4.h>
#include <ChilliSource/Core/Math/Matrix4x4.h>
#include <ChilliSource/Core/Math/Geometry/Shapes.h>
#include <ChilliSource/Rendering/Model/Skeleton.h>

namespace ChilliSource
{
	namespace Rendering
	{
		class Mesh : public Core::Resource
		{
		public:
			
			virtual ~Mesh();
			
			CS_DECLARE_NAMEDTYPE(Mesh);
			//---------------------------------------------------------------------
            /// Factory method for creating mesh resource instance
            ///
            /// @author S Downie
            ///
            /// @return Ownership of new instance
            //---------------------------------------------------------------------
            static MeshUPtr Create();
			//---------------------------------------------------------------------
			/// Is A
			///
			/// @return Whether this object is of given type
			//---------------------------------------------------------------------
			bool IsA(Core::InterfaceIDType inInterfaceID) const override;
            //----------------------------------------------------------------------------
            /// Build the mesh from the given description. This will build the mesh buffers
            /// and sub meshes
            ///
            /// @author I Copland
            ///
            /// @param Mesh descriptor
            //----------------------------------------------------------------------------
            bool Build(const MeshDescriptor& in_meshDesc);
			//-----------------------------------------------------------------
			/// Get AABB
			///
			/// @return AABB
			//-----------------------------------------------------------------
			const Core::AABB& GetAABB() const;
            //-----------------------------------------------------------------
            /// Get Number of Vertices
            ///
            /// @return Number of verts in this meshes sub-meshes.
            //-----------------------------------------------------------------
            u32 GetNumVerts() const;
            //-----------------------------------------------------------------
            /// Get Number of Indices
            ///
            /// @return Number of indices in this this meshes sub-meshes.
            //-----------------------------------------------------------------
            u32 GetNumIndices() const;
			//-----------------------------------------------------------------
			/// Get Skeleton
			///
			/// @return The skeleton
			//-----------------------------------------------------------------
			const SkeletonSPtr& GetSkeletonPtr() const;
			//-----------------------------------------------------------------
            /// Get Number of Sub Meshes
            ///
            /// @return Number of sub meshes.
            //-----------------------------------------------------------------
            u32 GetNumSubMeshes() const;
			//-----------------------------------------------------------------
			/// Get Sub-mesh at Index
			///
			/// @param Index
			/// @return Handle to submesh
			//-----------------------------------------------------------------
			SubMeshCSPtr GetSubMeshAtIndex(u32 inIndex) const;
			//-----------------------------------------------------------------
			/// Get Sub-mesh by Name
			///
			/// @param Index
			/// @return Handle to submesh
			//-----------------------------------------------------------------
			SubMeshCSPtr GetSubMeshByName(const std::string& instrName) const;
            //-----------------------------------------------------------------
			/// Get Submesh Index by Name
			///
			/// @param name
			/// @return The index. This will return -1 if there is no mesh by the
            /// given name.
			//-----------------------------------------------------------------
			s32 GetSubMeshIndexByName(const std::string& instrName) const;
			//-----------------------------------------------------------------
			/// Render
			///
			/// Render the contents of any sub meshes
			///
			/// @param the render system.
			/// @param World transform matrix
			/// @param The array of materials.
			//-----------------------------------------------------------------
			void Render(RenderSystem* inpRenderSystem, const Core::Matrix4x4 &inmatWorld, const std::vector<MaterialSPtr>& inMaterials, 
                        const SkinnedAnimationGroupSPtr& inpAnimationGroup = SkinnedAnimationGroupSPtr()) const;
		private:
			//Only the pool can create this
			Mesh();
			
			//-----------------------------------------------------------------
			/// Create Sub Mesh
			///
			/// Creates a new sub mesh with the given name.
			///
			/// @param the name of the mesh.
			/// @return The newly created sub-mesh.
			//-----------------------------------------------------------------
			SubMeshSPtr CreateSubMesh(const std::string& instrName);
			//-----------------------------------------------------------------
			/// Remove Sub Mesh By Name
			///
			/// Removed a sub mesh using its name.
			///
			/// @param the name of the mesh.
			//-----------------------------------------------------------------
			void RemoveSubMeshByName(const std::string& instrName);
			//-----------------------------------------------------------------
			/// Set Bounds
			///
			/// Creates the AABB for this mesh from minimum and maximum bounds
			/// vectors
			///
			/// @param the minimum bounds
			/// @param the maximum bounds.
			//-----------------------------------------------------------------
			void SetBounds(const ChilliSource::Core::Vector3& invMinBounds, const ChilliSource::Core::Vector3& invMaxBounds);
			//-----------------------------------------------------------------
			/// Calc Vertex And Index Counts
			///
			/// Calculates the internal vertex and index counts. This should be
			/// called after filling all of the submeshes with data.
			//-----------------------------------------------------------------
			void CalcVertexAndIndexCounts();
			
			//Only model loaders can alter the mesh construct
			friend class CSModelProvider;
			friend class MeshBatch;
		private:
			
			std::vector<SubMeshSPtr> mSubMeshes;
			SkeletonSPtr mpSkeleton;
			
			Core::AABB mBoundingBox;
			u32 mudwTotalVerts;
			u32 mudwTotalIndices;
		};
	}
}

#endif