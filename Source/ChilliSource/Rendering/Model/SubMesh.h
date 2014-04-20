/*
 *  SubMesh.h
 *  moFlo
 *
 *  Created by Scott Downie on 08/10/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#ifndef _MO_FLO_RENDERING_SUB_MESH_H_
#define _MO_FLO_RENDERING_SUB_MESH_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Rendering/Base/MeshBuffer.h>
#include <ChilliSource/Rendering/Model/Mesh.h>
#include <ChilliSource/Core/Math/Vector2.h>
#include <ChilliSource/Core/Math/Vector3Old.h>
#include <ChilliSource/Core/Math/Vector4Old.h>
#include <ChilliSource/Core/Math/Matrix4x4Old.h>
#include <ChilliSource/Core/Math/Geometry/Shapes.h>
#include <ChilliSource/Core/File/FileSystem.h>

namespace ChilliSource
{
	namespace Rendering
	{
		//===============================================
		/// Inverse Bind Pose
		//===============================================
		struct InverseBindPose
		{
			std::vector<ChilliSource::Core::Matrix4x4Old> mInverseBindPoseMatrices;
		};
		typedef std::shared_ptr<InverseBindPose> InverseBindPosePtr;
		
		class SubMesh
		{
		public:
			
			virtual ~SubMesh();
			
			//-----------------------------------------------------------------
			/// Get Internal Mesh Buffer
			///
			/// @return Mesh buffer containing the vertex data
			//-----------------------------------------------------------------
			MeshBuffer* GetInternalMeshBuffer() const;
			//-----------------------------------------------------------------
			/// Get AABB
			///
			/// @return AABB
			//-----------------------------------------------------------------
			const Core::AABB& GetAABB() const;
			//-----------------------------------------------------------------
			/// Get Name
			///
			/// @return The meshes name
			//-----------------------------------------------------------------
			const std::string& GetName() const;
			//-----------------------------------------------------------------
            /// Get Number of Vertices
            ///
            /// @return Number of verts in this sub-mesh
            //-----------------------------------------------------------------
            u32 GetNumVerts() const;
            //-----------------------------------------------------------------
            /// Get Number of Indices
            ///
            /// @return Number of indices in this sub-mesh
            //-----------------------------------------------------------------
            u32 GetNumIndices() const;
			
		private:
			//Only the mesh can create this
			SubMesh(const std::string& instrName);
			//-----------------------------------------------------------------
			/// Prepare
			///
			/// Prepares the sub mesh for use by setting up the mesh buffer, 
			/// vertex declaration, and capacities. This must be called prior 
			/// to using the mesh or calling build.
			///
			/// @param The render system.
			/// @param the vertex declaration.
			/// @param the size of an index in byte. only 2 and 4 are supported.
			/// @param the capacity of the vertex buffer in bytes.
			/// @param the capacity of the index buffer in bytes.
			/// @param the buffer access flag. defaults to "READ".			
            /// @param the primitive type. defaults to "TRIS".
			//-----------------------------------------------------------------
			void Prepare(RenderSystem* inpRenderSystem, const VertexDeclaration& inVertexDeclaration, u32 inudwIndexSizeInBytes,
						 u32 inudwVertexCapacityInBytes, u32 inudwIndexCapacityInBytes, BufferAccess inAccessFlag = BufferAccess::k_read,
						 PrimitiveType inPrimativeType = PrimitiveType::k_tri);
			//-----------------------------------------------------------------
			/// Alter Buffer Declaration
			///
			/// Changes the vertex declaration in the mesh buffer. This should
			/// be called when reusing a sub mesh for different mesh data.
			/// Build should be called following this, with the new mesh data.
			/// 
			/// @param the new vertex declaration.
			/// @param the new size of indices.
			//-----------------------------------------------------------------
			void AlterBufferDeclaration(const VertexDeclaration& inVertexDeclaration, u32 inudwIndexSizeInBytes);
			//-----------------------------------------------------------------
			/// Build
			///
			/// Fills the sub mesh with vertex and index data. This data will
			/// be passed directly into the Mesh Buffer.
			/// 
			/// @param the pre-formatted vertex data.
			/// @param the pre-formatted index data.
			/// @param the number of vertices.
			/// @param the number of indices.
			/// @param the minimum bounds.
			/// @param the maximum bounds.
			//-----------------------------------------------------------------
			void Build(void* inpVertexData, void*inpIndexData, u32 inudwNumVertices, u32 indwNumIndices, Core::Vector3Old invMin, Core::Vector3Old invMax);
			//-----------------------------------------------------------------
			/// Set Inverse Bind Pose
			/// 
			/// @param the new InverseBindPose
			//-----------------------------------------------------------------
			void SetInverseBindPose(const InverseBindPosePtr& inpInverseBindPose);
			//-----------------------------------------------------------------
			/// Render
			///
			/// Render the contents of the mesh buffer and invoke render
			/// on any sub meshes
			///
			/// @param the render system.
			/// @param World transform matrix
			//-----------------------------------------------------------------
			void Render(RenderSystem* inpRenderSystem, const Core::Matrix4x4Old &inmatWorld, const MaterialCSPtr& inpMaterial, ShaderPass in_shaderPass, const SkinnedAnimationGroupSPtr& inpAnimationGroup) const;
			
			friend class Mesh;
			
		private:
			
			std::string mstrName;
			Core::AABB mBoundingBox;
			
			//We own this!!!
			MeshBuffer* mpMeshBuffer;
			
			InverseBindPosePtr mpInverseBindPose;
		};
	}
}

#endif