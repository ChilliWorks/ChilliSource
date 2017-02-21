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

#ifndef _CHILLISOURCE_RENDERING_RENDERCOMMAND_COMMANDS_UNLOADMESHRENDERCOMMAND_H_
#define _CHILLISOURCE_RENDERING_RENDERCOMMAND_COMMANDS_UNLOADMESHRENDERCOMMAND_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Memory/UniquePtr.h>
#include <ChilliSource/Rendering/RenderCommand/RenderCommand.h>
#include <ChilliSource/Rendering/Model/RenderMesh.h>

namespace ChilliSource
{
    /// A render command for unloading the mesh data pertaining to a single render mesh from
    /// render memory.
    ///
    /// This must be instantiated via a RenderCommandList.
    ///
    /// This is immutable and therefore thread-safe.
    ///
    class UnloadMeshRenderCommand final : public RenderCommand
    {
    public:
        /// @return The render mesh that should be unloaded.
        ///
        const RenderMesh* GetRenderMesh() const noexcept { return m_renderMesh.get(); }
        
        /// Takes ownership of the RenderMesh from this class. This will leave
        /// this command in an unusable state and should be used with caution.
        ///
        /// @return The render mesh that should be unloaded.
        ///
        UniquePtr<RenderMesh> ClaimRenderMesh() noexcept;
        
    private:
        friend class RenderCommandList;
        
        /// Constructs a new instance with the given render mesh.
        ///
        /// @param renderMesh
        ///     The render mesh that should be unloaded.
        ///
        UnloadMeshRenderCommand(UniquePtr<RenderMesh> renderMesh) noexcept;
        
        UniquePtr<RenderMesh> m_renderMesh;
    };
}

#endif
