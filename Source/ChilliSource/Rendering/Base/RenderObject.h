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
            k_dynamic
        };
        
        /// Creates a new instance with the given material group, mesh and transform data.
        ///
        /// @param renderMaterialGroup
        ///     The material group that should be used when rendering this object.
        /// @param renderMesh
        ///     The mesh that should be used when rendering this object.
        /// @param worldMatrix
        ///     The world matrix describing the transform of the object.
        ///
        RenderObject(const RenderMaterialGroup* renderMaterialGroup, const RenderMesh* renderMesh, const Matrix4& worldMatrix) noexcept;
        
        /// Creates a new instance with the given material group, mesh and transform data.
        ///
        /// @param renderMaterialGroup
        ///     The material group that should be used when rendering this object.
        /// @param renderDynamicMesh
        ///     The dynamic mesh that should be used when rendering this object.
        /// @param worldMatrix
        ///     The world matrix describing the transform of the object.
        ///
        RenderObject(const RenderMaterialGroup* renderMaterialGroup, const RenderDynamicMesh* renderDynamicMesh, const Matrix4& worldMatrix) noexcept;
        
        /// @return The type of object this describes.
        ///
        Type GetType() const { return m_type; }
        
        /// @return The material group that should be used when rendering this object.
        ///
        const RenderMaterialGroup* GetRenderMaterialGroup() const noexcept { return m_renderMaterialGroup; }
        
        /// @return The mesh that should be used when rendering this object.
        ///
        const RenderMesh* GetRenderMesh() const noexcept { return m_renderMesh; }
        
        /// @return The mesh that should be used when rendering this object.
        ///
        const RenderDynamicMesh* GetRenderDynamicMesh() noexcept { return m_renderDynamicMesh; }
        
        /// @return The world matrix describing the transform of the object.
        ///
        const Matrix4& GetWorldMatrix() const noexcept { return m_worldMatrix; }
        
    private:
        Type m_type;
        const RenderMaterialGroup* m_renderMaterialGroup;
        const RenderMesh* m_renderMesh = nullptr;
        const RenderDynamicMesh* m_renderDynamicMesh = nullptr;
        Matrix4 m_worldMatrix;
    };
}

#endif
