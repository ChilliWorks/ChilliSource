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

#ifndef _CHILLISOURCE_RENDERING_MODEL_MODEL_H_
#define _CHILLISOURCE_RENDERING_MODEL_MODEL_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Math/Geometry/Shapes.h>
#include <ChilliSource/Core/Memory/UniquePtr.h>
#include <ChilliSource/Core/Resource/Resource.h>
#include <ChilliSource/Rendering/Model/Skeleton.h>

namespace ChilliSource
{
    /// A single renderable model which is comprised of one or more meshes.
    ///
    /// This is not thread safe and should only be accessed from one thread at a time.
    ///
    class Model final : public Resource
    {
    public:
        CS_DECLARE_NAMEDTYPE(Model);
        
        /// Allows querying of whether or not this resource implements the interface described by the
        /// given interface Id. Typically this is not called directly as the templated equivalent
        /// IsA<Interface>() is preferred.
        ///
        /// @param interfaceId
        ///     The Id of the interface.
        ///
        /// @return Whether or not the interface is implemented.
        ///
        bool IsA(InterfaceIDType interfaceId) const noexcept override;
        
        /// Build the model from the given description, creating all required meshes and a skeleton
        /// if required. The model must not already be in the loaded state.
        ///
        /// The description must be passed by move, rather than copy to avoid duplicating mesh data.
        ///
        /// @param modelDesc
        ///     The model description. Can only be used once to build a model.
        ///
        void Build(ModelDesc modelDesc) noexcept;
        
        /// This must not be called until the model is built and loaded.
        ///
        /// @return The local space axis-aligned bounding box of the Model.
        ///
        const AABB& GetAABB() const noexcept;
        
        /// This must not be called until the model is built and loaded.
        ///
        /// @return The local space bounding sphere of the Model.
        ///
        const Sphere& GetBoundingSphere() const noexcept;
        
        /// This must not be called until the model is built and loaded.
        ///
        /// @return The skeleton.
        ///
        const Skeleton& GetSkeleton() const noexcept;
        
        /// This must not be called until the model is built and loaded.
        ///
        /// @return The number of meshes that make up the model.
        ///
        u32 GetNumMeshes() const noexcept;
        
        /// Looks up the name of the mesh with the given index. If the index is out of bounds, this will
        /// assert.
        ///
        /// This must not be called until the model is built and loaded.
        ///
        /// @param index
        ///     The index of the mesh.
        ///
        /// @return The name of the mesh.
        ///
        const std::string& GetMeshName(u32 index) const noexcept;
        
        /// Looks up the index of the mesh with the given name. If the name doesn't exist, this will assert.
        ///
        /// This must not be called until the model is built and loaded.
        ///
        /// @param name
        ///     The name of the mesh.
        ///
        /// @return The index of the mesh with the given name.
        ///
        const u32 GetMeshIndex(const std::string& name) const noexcept;
        
        /// Looks up the RenderMesh of the mesh with the given index. If the index is out of bounds, this will
        /// assert.
        ///
        /// This must not be called until the model is built and loaded.
        ///
        /// @param index
        ///     The index of the mesh.
        ///
        /// @return The render mesh.
        ///
        const RenderMesh* GetRenderMesh(u32 index) const noexcept;
        
        ~Model() noexcept;
        
    private:
        friend class ResourcePool;
        friend class CSModelProvider;
        
        /// A factory method for creating new, empty instances of the resource. This must only be
        /// called by ResourcePool.
        ///
        /// @return The new instance of the resource.
        ///
        static ModelUPtr Create() noexcept;
        
        /// Destroys the RenderMeshes if there currently are any
        ///
        void DestroyRenderMeshes() noexcept;
        
        Model() = default;

        std::vector<std::string> m_meshNames;
        std::vector<UniquePtr<RenderMesh>> m_renderMeshes;
        Skeleton m_skeleton;
        AABB m_aabb;
        Sphere m_boundingSphere;
    };
}

#endif
