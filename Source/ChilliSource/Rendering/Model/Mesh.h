//
//  Mesh.h
//  Chilli Source
//
//  Created by Scott Downie on 06/10/2010.
//  Copyright 2010 Tag Games. All rights reserved.
//

#ifndef _CHILLISOURCE_RENDERING_MODEL_MESH_H_
#define _CHILLISOURCE_RENDERING_MODEL_MESH_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Math/Geometry/Shapes.h>
#include <ChilliSource/Core/Resource/Resource.h>

namespace ChilliSource
{
	namespace Rendering
	{
        //---------------------------------------------------------------------
        /// Holds the sub meshes and skeleton that make up a mesh resource.
        /// Provides methods for building and rendering the mesh
        ///
        /// @author S Downie
        //---------------------------------------------------------------------
		class Mesh final : public Core::Resource
		{
		public:
			
			CS_DECLARE_NAMEDTYPE(Mesh);
            
			//---------------------------------------------------------------------
			/// @author S Downie
			///
            /// @param Interface ID type
            ///
			/// @return Whether this object is of given type
			//---------------------------------------------------------------------
			bool IsA(Core::InterfaceIDType in_interfaceId) const override;
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
			/// @author S Downie
			///
			/// @return Axis-aligned bounding box
			//-----------------------------------------------------------------
			const Core::AABB& GetAABB() const;
            //-----------------------------------------------------------------
            /// @author I Copland
            ///
            /// @return Number of verts in this meshes sub-meshes.
            //-----------------------------------------------------------------
            u32 GetNumVerts() const;
            //-----------------------------------------------------------------
            /// @author I Copland
            ///
            /// @return Number of indices in this this meshes sub-meshes.
            //-----------------------------------------------------------------
            u32 GetNumIndices() const;
			//-----------------------------------------------------------------
			/// @author I Copland
			///
			/// @return The skeleton
			//-----------------------------------------------------------------
			const SkeletonSPtr& GetSkeletonPtr() const;
			//-----------------------------------------------------------------
            /// @author I Copland
            ///
            /// @return Number of sub meshes.
            //-----------------------------------------------------------------
            u32 GetNumSubMeshes() const;
			//-----------------------------------------------------------------
			/// @author I Copland
			///
			/// @param Index
            ///
			/// @return Submesh or null
			//-----------------------------------------------------------------
			SubMeshCSPtr GetSubMeshAtIndex(u32 in_index) const;
			//-----------------------------------------------------------------
			/// @author I Copland
			///
			/// @param Index
            //
			/// @return Submesh or null
			//-----------------------------------------------------------------
			SubMeshCSPtr GetSubMeshByName(const std::string& in_name) const;
            //-----------------------------------------------------------------
			/// @author I Copland
			///
			/// @param name
            ///
			/// @return The index. This will return -1 if there is no mesh by the
            /// given name.
			//-----------------------------------------------------------------
			s32 GetSubMeshIndexByName(const std::string& in_name) const;
			//-----------------------------------------------------------------
			/// Render the contents of any sub meshes
            ///
            /// @author S Downie
			///
			/// @param the render system.
			/// @param World transform matrix
			/// @param The array of materials.
            /// @param Skinned animation group
			//-----------------------------------------------------------------
			void Render(RenderSystem* in_renderSystem, const Core::Matrix4x4& in_worldMat, const std::vector<MaterialSPtr>& in_materials, const SkinnedAnimationGroupSPtr& in_animGroup = SkinnedAnimationGroupSPtr()) const;
		private:
            
            friend class Core::ResourcePool;
            //---------------------------------------------------------------------
            /// Factory method for creating mesh resource instance. Only accessed
            /// by the resource pool
            ///
            /// @author S Downie
            ///
            /// @return Ownership of new instance
            //---------------------------------------------------------------------
            static MeshUPtr Create();
            //-----------------------------------------------------------------
            /// Private constructor enforces the use of the factory method
            ///
            /// @author S Downie
            //-----------------------------------------------------------------
			Mesh();
			//-----------------------------------------------------------------
			/// Creates a new sub mesh with the given name.
            ///
            /// @author I Copland
			///
			/// @param the name of the mesh.
			/// @return The newly created sub-mesh.
			//-----------------------------------------------------------------
			SubMeshSPtr CreateSubMesh(const std::string& in_name);
			//-----------------------------------------------------------------
			/// Removed a sub mesh using its name.
            ///
            /// @author I Copland
			///
			/// @param the name of the mesh.
			//-----------------------------------------------------------------
			void RemoveSubMeshByName(const std::string& in_name);
			//-----------------------------------------------------------------
			/// Creates the AABB for this mesh from minimum and maximum bounds
			/// vectors
            ///
            /// @author I Copland
			///
			/// @param the minimum bounds
			/// @param the maximum bounds.
			//-----------------------------------------------------------------
			void SetBounds(const Core::Vector3& in_minBounds, const Core::Vector3& in_maxBounds);
			//-----------------------------------------------------------------
			/// Calculates the internal vertex and index counts. This should be
			/// called after filling all of the submeshes with data.
            ///
            /// @author I Copland
			//-----------------------------------------------------------------
			void CalcVertexAndIndexCounts();
			
			//Only model loaders can alter the mesh construct
			friend class CSModelProvider;
			friend class MeshBatch;
            
		private:
			
			std::vector<SubMeshSPtr> m_subMeshes;
			SkeletonSPtr m_skeleton;
			
			Core::AABB m_aabb;
			u32 m_totalNumVerts;
			u32 m_totalNumIndices;
		};
	}
}

#endif