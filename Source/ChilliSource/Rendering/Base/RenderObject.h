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

#ifndef _CHILLISOURCE_RENDERING_BASE_RENDEROBJECT_H_
#define _CHILLISOURCE_RENDERING_BASE_RENDEROBJECT_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Math/Matrix4.h>
#include <ChilliSource/Core/Math/Geometry/Shapes.h>
#include <ChilliSource/Rendering/Base/RenderLayer.h>
#include <ChilliSource/Rendering/Material/RenderMaterialGroup.h>
#include <ChilliSource/Rendering/Model/RenderMesh.h>

namespace ChilliSource
{
    /// A standard-layout container for data which the renderer requires that relates to a single
    /// object in the scene, such as mesh data, materials and transform data.
    ///
    /// This is immutable and therefore thread safe.
    ///
    class RenderObject final
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
        
        /// Creates a new instance with the given material group, mesh and transform data.
        ///
        /// @param renderMaterialGroup
        ///     The material group that should be used when rendering this object.
        /// @param renderMesh
        ///     The mesh that should be used when rendering this object.
        /// @param worldMatrix
        ///     The world matrix describing the transform of the object.
        /// @param boundingSphere
        ///     The world space bounding sphere of the object. This should be build using the local mesh
        ///     bounding sphere.
        /// @param shouldCastShadows
        ///     Whether or not the render object will try to cast shadows. Whether or not a shadow is
        ///     actually cast is determined by the objects materials, and the lights in the scene.
        /// @param renderLayer
        ///     The layer the object should be rendered into.
        /// @param priority
        ///     (Optional) The order priority of the render object; lower values will be rendered first.
        ///     This may or may not be used depending on the render pass the object will be included in.
        ///     Objects with the same order value will return in an undefined order. Defaults to 0.
        ///
        RenderObject(const RenderMaterialGroup* renderMaterialGroup, const RenderMesh* renderMesh, const Matrix4& worldMatrix, const Sphere& boundingSphere, bool shouldCastShadows,
                     RenderLayer renderLayer, u32 priority = 0) noexcept;
        
        /// Creates a new instance with the given material group, mesh, skinned animation and transform data.
        ///
        /// @param renderMaterialGroup
        ///     The material group that should be used when rendering this object.
        /// @param renderMesh
        ///     The mesh that should be used when rendering this object.
        /// @param skinnedAnimation
        ///     The skinned animation that should be used when rendering this object.
        /// @param worldMatrix
        ///     The world matrix describing the transform of the object.
        /// @param boundingSphere
        ///     The world space bounding sphere of the object. This should be build using the local mesh
        ///     bounding sphere.
        /// @param shouldCastShadows
        ///     Whether or not the render object will try to cast shadows. Whether or not a shadow is
        ///     actually cast is determined by the objects materials, and the lights in the scene.
        /// @param renderLayer
        ///     The layer the object should be rendered into.
        /// @param priority
        ///     (Optional) The order priority of the render object; lower values will be rendered first.
        ///     This may or may not be used depending on the render pass the object will be included in.
        ///     Objects with the same order value will return in an undefined order. Defaults to 0.
        ///
        RenderObject(const RenderMaterialGroup* renderMaterialGroup, const RenderMesh* renderMesh, const RenderSkinnedAnimation* renderSkinnedAnimation, const Matrix4& worldMatrix, const Sphere& boundingSphere,
                     bool shouldCastShadows, RenderLayer renderLayer, u32 priority = 0) noexcept;
        
        /// Creates a new instance with the given material group, mesh and transform data.
        ///
        /// @param renderMaterialGroup
        ///     The material group that should be used when rendering this object.
        /// @param renderDynamicMesh
        ///     The dynamic mesh that should be used when rendering this object.
        /// @param worldMatrix
        ///     The world matrix describing the transform of the object.
        /// @param boundingSphere
        ///     The world space bounding sphere of the object. This should be build using the local mesh
        ///     bounding sphere.
        /// @param shouldCastShadows
        ///     Whether or not the render object will try to cast shadows. Whether or not a shadow is
        ///     actually cast is determined by the objects materials, and the lights in the scene.
        /// @param renderLayer
        ///     The layer the object should be rendered into.
        /// @param priority
        ///     (Optional) The order priority of the render object; lower values will be rendered first.
        ///     This may or may not be used depending on the sort algorithm used, as is the order
        ///     of objects with the same priority. Defaults to 0.
        ///
        RenderObject(const RenderMaterialGroup* renderMaterialGroup, const RenderDynamicMesh* renderDynamicMesh, const Matrix4& worldMatrix, const Sphere& boundingSphere, bool shouldCastShadows,
                     RenderLayer renderLayer, u32 priority = 0) noexcept;
        
        /// Creates a new instance with the given material group, mesh, skinned animation data,  and
        /// transform data.
        ///
        /// @param renderMaterialGroup
        ///     The material group that should be used when rendering this object.
        /// @param renderDynamicMesh
        ///     The dynamic mesh that should be used when rendering this object.
        /// @param skinnedAnimation
        ///     The skinned animation that should be used when rendering this object.
        /// @param worldMatrix
        ///     The world matrix describing the transform of the object.
        /// @param boundingSphere
        ///     The world space bounding sphere of the object. This should be build using the local mesh
        ///     bounding sphere.
        /// @param shouldCastShadows
        ///     Whether or not the render object will try to cast shadows. Whether or not a shadow is
        ///     actually cast is determined by the objects materials, and the lights in the scene.
        /// @param renderLayer
        ///     The layer the object should be rendered into.
        /// @param priority
        ///     (Optional) The order priority of the render object; lower values will be rendered first.
        ///     This may or may not be used depending on the sort algorithm used, as is the order
        ///     of objects with the same priority. Defaults to 0.
        ///
        RenderObject(const RenderMaterialGroup* renderMaterialGroup, const RenderDynamicMesh* renderDynamicMesh, const RenderSkinnedAnimation* renderSkinnedAnimation, const Matrix4& worldMatrix,
                     const Sphere& boundingSphere, bool shouldCastShadows, RenderLayer renderLayer, u32 priority = 0) noexcept;
        
        /// @return The mesh type of object this describes.
        ///
        Type GetType() const noexcept { return m_type; }
        
        /// @return The material group that should be used when rendering this object.
        ///
        const RenderMaterialGroup* GetRenderMaterialGroup() const noexcept { return m_renderMaterialGroup; }
        
        /// @return The static mesh that should be used when rendering this object if this is a static mesh
        ///     object, otherwise this will return nullptr.
        ///
        const RenderMesh* GetRenderMesh() const noexcept { return m_renderMesh; }
        
        /// @return The dynamic mesh that should be used when rendering this object if this is a dynamic mesh
        ///     object, otherwise this will return nullptr.
        ///
        const RenderDynamicMesh* GetRenderDynamicMesh() const noexcept { return m_renderDynamicMesh; }
        
        /// @return The skinned animation that should be used to animate this object. Can be null if the
        ///     object is not animated.
        ///
        const RenderSkinnedAnimation* GetRenderSkinnedAnimation() const noexcept { return m_renderSkinnedAnimation; }
        
        /// @return The world matrix describing the transform of the object.
        ///
        const Matrix4& GetWorldMatrix() const noexcept { return m_worldMatrix; }
        
        /// @return The world space bounding sphere of the object.
        ///
        const Sphere& GetBoundingSphere() const noexcept { return m_boundingSphere; }
        
        /// @return Whether or not the render object will try to cast shadows. Whether or not a shadow is
        ///     actually cast is determined by the objects materials, and the lights in the scene.
        ///
        bool ShouldCastShadows() const noexcept { return m_shouldCastShadows; }
        
        /// @return The layer the object should be rendered into.
        ///
        RenderLayer GetRenderLayer() const noexcept { return m_renderLayer; }
        
        /// @return The order priority of the render object; lower values will be rendered first. This may or
        ///     may not be used depending on the sort algorithm used, as is the order of objects with the
        ///     same priority.
        ///
        u32 GetPriority() const noexcept { return m_priority; }
        
    private:
        Type m_type;
        const RenderMaterialGroup* m_renderMaterialGroup;
        const RenderMesh* m_renderMesh = nullptr;
        const RenderDynamicMesh* m_renderDynamicMesh = nullptr;
        const RenderSkinnedAnimation* m_renderSkinnedAnimation = nullptr;
        Matrix4 m_worldMatrix;
        Sphere m_boundingSphere;
        bool m_shouldCastShadows;
        RenderLayer m_renderLayer;
        u32 m_priority;
    };
}

#endif
