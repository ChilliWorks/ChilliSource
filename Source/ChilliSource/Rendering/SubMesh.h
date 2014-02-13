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

#include <moFlo/Rendering/MeshBuffer.h>
#include <moFlo/Rendering/Mesh.h>

#include <moFlo/Core/Math/Vector2.h>
#include <moFlo/Core/Math/Vector3.h>
#include <moFlo/Core/Math/Vector4.h>
#include <moFlo/Core/Math/Matrix4x4.h>
#include <moFlo/Core/Math/Shapes.h>
#include <moFlo/Core/FileIO/FileSystem.h>

namespace moFlo
{
	namespace Rendering
	{
		//===============================================
		/// Inverse Bind Pose
		//===============================================
		struct InverseBindPose
		{
			DYNAMIC_ARRAY<moFlo::Core::CMatrix4x4> mInverseBindPoseMatrices;
		};
		typedef SHARED_PTR<InverseBindPose> InverseBindPosePtr;
		
		class CSubMesh
		{
		public:
			
			virtual ~CSubMesh();
			
			//-----------------------------------------------------------------
			/// Get Default Material Name
			///
			/// @param the name(filepath) of the default material.
			//-----------------------------------------------------------------
			const std::string& GetDefaultMaterialName();
            //-----------------------------------------------------------------
			/// Get Default Material Storage Location
			///
			/// @param the storage location of the default material.
			//-----------------------------------------------------------------
            Core::STORAGE_LOCATION GetDefaultMaterialStorageLocation();
			//-----------------------------------------------------------------
			/// Get Internal Mesh Buffer
			///
			/// @return Mesh buffer containing the vertex data
			//-----------------------------------------------------------------
			IMeshBuffer* GetInternalMeshBuffer() const;
			//-----------------------------------------------------------------
			/// Get AABB
			///
			/// @return AABB
			//-----------------------------------------------------------------
			const Core::AABB& GetAABB();
			//-----------------------------------------------------------------
			/// Get Name
			///
			/// @return The meshes name
			//-----------------------------------------------------------------
			const std::string& GetName();
			//-----------------------------------------------------------------
            /// Get Number of Vertices
            ///
            /// @return Number of verts in this sub-mesh
            //-----------------------------------------------------------------
            u32 GetNumVerts();
            //-----------------------------------------------------------------
            /// Get Number of Indices
            ///
            /// @return Number of indices in this sub-mesh
            //-----------------------------------------------------------------
            u32 GetNumIndices();
			
		private:
			//Only the mesh can create this
			CSubMesh(std::string instrName);
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
			void Prepare(IRenderSystem* inpRenderSystem, CVertexDeclaration& inVertexDeclaration, u32 inudwIndexSizeInBytes,
						 u32 inudwVertexCapacityInBytes, u32 inudwIndexCapacityInBytes, BufferAccess inAccessFlag = READ, 
						 PrimitiveType inPrimativeType = TRIS);
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
			void AlterBufferDeclaration(CVertexDeclaration& inVertexDeclaration, u32 inudwIndexSizeInBytes);
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
			void Build(void* inpVertexData, void*inpIndexData, u32 inudwNumVertices, u32 indwNumIndices, Core::CVector3 invMin, Core::CVector3 invMax);
			//-----------------------------------------------------------------
			/// Set Default Material Name
			/// 
			/// @param the material name
			//-----------------------------------------------------------------
			void SetDefaultMaterialName(const std::string& instrMaterialName);
            //-----------------------------------------------------------------
			/// Set Default Material Storage Location
			/// 
			/// @param the material storage location
			//-----------------------------------------------------------------
			void SetDefaultMaterialStorageLocation(Core::STORAGE_LOCATION ineStorageLocation);
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
			void Render(IRenderSystem* inpRenderSystem, const Core::CMatrix4x4 &inmatWorld, const MaterialPtr& inpMaterial, const SkinnedAnimationGroupPtr& inpAnimationGroup) const;
			
			//Only model loaders can alter the mesh construct
			friend class CMeshManager;
			friend class CMesh;
			
		private:
			
			std::string mstrName;
			std::string mstrDefaultMaterialName;
            Core::STORAGE_LOCATION meDefaultStorageLocation;
			Core::AABB mBoundingBox;
			
			//Used to create hardware buffers and render
			IRenderSystem* mpRenderSystem;
			
			//We own this!!!
			IMeshBuffer* mpMeshBuffer;
			
			InverseBindPosePtr mpInverseBindPose;
		};
	}
}

#endif