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

#ifndef _CHILLISOURCE_RENDERING_MODEL_SMALLMESHBATCHER_H_
#define _CHILLISOURCE_RENDERING_MODEL_SMALLMESHBATCHER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Rendering/Model/IndexFormat.h>
#include <ChilliSource/Rendering/Model/PolygonType.h>
#include <ChilliSource/Rendering/Model/RenderMeshBatch.h>
#include <ChilliSource/Rendering/Model/VertexFormat.h>

#include <vector>

namespace ChilliSource
{
    /// Manages the batching of small objects into a single mesh to reduce the number of draw calls
    /// that are performed. This is primarily for sprites and UI elements, but it will also batch
    /// static meshes under a certain vertex count.
    ///
    /// This works by converting combining all of the meshes together into a single vertex and index
    /// buffer. The vertex data will be converted into world space, and the index data will be offset
    /// to the updated position of the vertex data in the buffer. This is an expensive operation, so
    /// it should only be applied to small meshes.
    ///
    /// Only objects of the same mesh type can be batched. When a new mesh type is encountered the
    /// current mesh batch is flushed and a new batch started. Flushing applies the dynamic mesh and
    /// then sends a render instance command. A flush is also required before the current material
    /// changes else the render instance command will occur when the new material is bound. This has
    /// to be handled manually by calling Flush().
    ///
    /// This is not thread-safe and each instance should only be accessed from one thread at a time.
    ///
    class SmallMeshBatcher final
    {
    public:
        /// Evaluates whether or not the given render pass object can be batched. This is based on the type
        /// of mesh, and the number of vertices in the mesh. This should be checked prior to calling batch.
        ///
        /// @param renderPassObject
        ///     The render pass object which should be checked.
        ///
        /// @return Whether or not the render pass object can be batched.
        ///
        static bool CanBatch(const RenderPassObject& renderPassObject) noexcept;
        
        /// Creates a new instance with the given render command list. This is the list that all generated
        /// commands will be added to.
        ///
        /// @param renderCommandList
        ///     The render command list commands should be added to.
        ///
        SmallMeshBatcher(RenderCommandList* renderCommandList) noexcept;
        
        /// Adds the given render pass object to the batch. If the object contains a different mesh type
        /// than the current batch, the batch will first be flushed. If the render pass object cannot be
        /// batched this will assert.
        ///
        /// @param renderPassObject
        ///     The render pass object which should be batched.
        ///
        void Batch(const RenderPassObject& renderPassObject) noexcept;
        
        /// Flushes the batch, adding the appropriate commands to the renderCommandList.
        ///
        void Flush() noexcept;
        
        ~SmallMeshBatcher() noexcept;
        
    private:
        /// Checks the current state versus the the state described by the given render pass object. If
        /// the state is different then the internal state is updated.
        ///
        /// @return renderPassObject
        ///     The render pass object whose state should be checked.
        ///
        /// @return Whether or not the state has changed.
        ///
        bool TryUpdateRenderState(const RenderPassObject& renderPassObject) noexcept;
        
        /// Adds the given object to the batch. Will flush the batch if there is no room left in the buffer.
        ///
        /// @param renderPassObject
        ///     The render pass object which should be added.
        ///
        void AddToBatch(const RenderPassObject& renderPassObject) noexcept;
        
        RenderCommandList* m_renderCommandList;
        
        PolygonType m_currentPolygonType = PolygonType::k_triangle;
        VertexFormat m_currentVertexFormat = VertexFormat::k_staticMesh;
        IndexFormat m_currentIndexFormat = IndexFormat::k_short;
        bool m_hasIndices = false;
        std::vector<RenderMeshBatch::Mesh> m_currentMeshes;
        u32 m_currentVertexDataSize = 0;
        u32 m_currentIndexDataSize = 0;
    };
}

#endif