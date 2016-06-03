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
        
        /// @param renderMaterial
        ///     The material to use
        /// @param renderMesh
        ///     The mesh to render
        /// @param worldMatrix
        ///     The transform to render at
        ///
        RenderPassObject(const RenderMaterial* renderMaterial, const RenderMesh* renderMesh, const Matrix4& worldMatrix) noexcept;
        
        /// @return The RenderMaterial to use
        ///
        const RenderMaterial* GetRenderMaterial() const noexcept { return m_renderMaterial; }
        
        /// @return The RenderMesh to use
        ///
        const RenderMesh* GetRenderMesh() const noexcept { return m_renderMesh; }
        
        /// @return The WorldMatrix to use
        ///
        const Matrix4& GetWorldMatrix() const noexcept { return m_worldMatrix; }
        
    private:
        const RenderMaterial* m_renderMaterial;
        const RenderMesh* m_renderMesh;
        Matrix4 m_worldMatrix;
    };
}

#endif
