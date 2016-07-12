//
//  The MIT License (MIT)
//
//  Copyright (c) 2016 Tag Games Limited
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

#ifndef _CHILLISOURCE_RENDERING_BASE_RENDERPASSOBJECT_H_
#define _CHILLISOURCE_RENDERING_BASE_RENDERPASSOBJECT_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Math/Geometry/Shapes.h>
#include <ChilliSource/Core/Math/Matrix4.h>
#include <ChilliSource/Rendering/ForwardDeclarations.h>

namespace ChilliSource
{
    /// A standard-layout container for data which the renderer requires that relates to a single render
    /// pass for a single object in the scene, such as mesh data, material and transform data.
    /// While a RenderObject contains a material group that has materials for all render passes, this
    /// only contains a single material for the pass this object is be created for.
    ///
    /// This is immutable and therefore thread-safe
    ///
    class RenderPassObject final
    {
    public:
        /// An enum describing the type of mesh this object represents.
        ///
        enum class Type
        {
            k_static,
            k_staticAnimated,
            k_dynamic,
            k_dynamicAnimated
        };
        
        /// Creates a new static mesh render pass object.
        ///
        /// @param renderMaterial
        ///     The render material that should be used when rendering this object.
        /// @param renderMesh
        ///     The static mesh that should be used when rendering this object.
        /// @param worldMatrix
        ///     The world matrix describing the transform of the object.
        /// @param boundingSphere
        ///     The world space bounding sphere of the object. This should be build using the local mesh
        ///     bounding sphere.
        /// @param priority
        ///     The order priority of the render object; lower values will be rendered first. This may or
        ///     may not be used depending on the sort algorithm used, as is the order of objects with the
        ///     same priority.
        ///
        RenderPassObject(const RenderMaterial* renderMaterial, const RenderMesh* renderMesh, const Matrix4& worldMatrix, const Sphere& boundingSphere, u32 priority) noexcept;
        
        /// Creates a new static mesh render pass object with skinned animation
        ///
        /// @param renderMaterial
        ///     The render material that should be used when rendering this object.
        /// @param renderMesh
        ///     The static mesh that should be used when rendering this object.
        /// @param renderSkinnedAnimation
        ///     The skinned animation data.
        /// @param worldMatrix
        ///     The world matrix describing the transform of the object.
        /// @param boundingSphere
        ///     The world space bounding sphere of the object. This should be build using the local mesh
        ///     bounding sphere.
        /// @param priority
        ///     The order priority of the render object; lower values will be rendered first. This may or
        ///     may not be used depending on the sort algorithm used, as is the order of objects with the
        ///     same priority.
        ///
        RenderPassObject(const RenderMaterial* renderMaterial, const RenderMesh* renderMesh, const RenderSkinnedAnimation* renderSkinnedAnimation, const Matrix4& worldMatrix,
                         const Sphere& boundingSphere, u32 priority) noexcept;
        
        /// Creates a new dynamic mesh render pass object.
        ///
        /// @param renderMaterial
        ///     The render material that should be used when rendering this object.
        /// @param renderDynamicMesh
        ///     The dynamic mesh that should be used when rendering this object.
        /// @param worldMatrix
        ///     The world matrix describing the transform of the object.
        /// @param boundingSphere
        ///     The world space bounding sphere of the object. This should be build using the local mesh
        ///     bounding sphere.
        /// @param priority
        ///     The order priority of the render object; lower values will be rendered first. This may or
        ///     may not be used depending on the sort algorithm used, as is the order of objects with the
        ///     same priority.
        ///
        RenderPassObject(const RenderMaterial* renderMaterial, const RenderDynamicMesh* renderDynamicMesh, const Matrix4& worldMatrix, const Sphere& boundingSphere, u32 priority) noexcept;
        
        /// Creates a new dynamic mesh render pass object.
        ///
        /// @param renderMaterial
        ///     The render material that should be used when rendering this object.
        /// @param renderDynamicMesh
        ///     The dynamic mesh that should be used when rendering this object.
        /// @param renderSkinnedAnimation
        ///     The skinned animation data.
        /// @param worldMatrix
        ///     The world matrix describing the transform of the object.
        /// @param boundingSphere
        ///     The world space bounding sphere of the object. This should be build using the local mesh
        ///     bounding sphere.
        /// @param priority
        ///     The order priority of the render object; lower values will be rendered first. This may or
        ///     may not be used depending on the sort algorithm used, as is the order of objects with the
        ///     same priority.
        ///
        RenderPassObject(const RenderMaterial* renderMaterial, const RenderDynamicMesh* renderDynamicMesh, const RenderSkinnedAnimation* renderSkinnedAnimation, const Matrix4& worldMatrix,
                         const Sphere& boundingSphere, u32 priority) noexcept;
        
        /// @return The type of object this describes.
        ///
        Type GetType() const noexcept { return m_type; }
        
        /// @return The render material that should be used when rendering this object.
        ///
        const RenderMaterial* GetRenderMaterial() const noexcept { return m_renderMaterial; }
        
        /// @return The static mesh that should be used when rendering this object if this is a
        ///     static mesh object, otherwise this will return nullptr.
        ///
        const RenderMesh* GetRenderMesh() const noexcept { return m_renderMesh; }
        
        /// @return The dynamic mesh that should be used when rendering this object if this is
        ///     a dynamic mesh object, otherwise this will return nullptr.
        ///
        const RenderDynamicMesh* GetRenderDynamicMesh() const noexcept { return m_renderDynamicMesh; }
        
        /// @return The skinned animation data for this object. Will be null if the object is not
        ///     animated.
        ///
        const RenderSkinnedAnimation* GetRenderSkinnedAnimation() const noexcept { return m_renderSkinnedAnimation; }
        
        /// @return The world matrix describing the transform of the object.
        ///
        const Matrix4& GetWorldMatrix() const noexcept { return m_worldMatrix; }
        
        /// @return The world space bounding sphere of the object.
        ///
        const Sphere& GetBoundingSphere() const noexcept { return m_boundingSphere; }
        
        /// @return The order priority of the render object; lower values will be rendered first.
        ///     This may or may not be used depending on the sort algorithm used, as is the order
        ///     of objects with the same priority.
        ///
        u32 GetPriority() const noexcept { return m_priority; }
        
    private:
        Type m_type;
        const RenderMaterial* m_renderMaterial;
        const RenderMesh* m_renderMesh = nullptr;
        const RenderDynamicMesh* m_renderDynamicMesh = nullptr;
        const RenderSkinnedAnimation* m_renderSkinnedAnimation = nullptr;
        Matrix4 m_worldMatrix;
        Sphere m_boundingSphere;
        u32 m_priority;
    };
}

#endif
