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

#ifndef _CHILLISOURCE_RENDERING_RENDERCOMMAND_RENDERCOMMAND_H_
#define _CHILLISOURCE_RENDERING_RENDERCOMMAND_RENDERCOMMAND_H_

#include <ChilliSource/ChilliSource.h>

namespace ChilliSource
{

    /// The base class for render commands. This is required for type erasure so that render
    /// disparate render commands can be stored in the same container. A render command contains
    /// a type, which can be used to safely cast down to the concrete type, without the need for
    /// virtual calls or RTTI.
    ///
    /// Render commands should be instantiated within a RenderCommandList.
    ///
    /// A render command should be immutable and therefore thread safe.
    ///
    class RenderCommand
    {
    public:
        /// An enum which describes the various different types of render command.
        ///
        enum class Type
        {
            k_loadTexture,
            k_loadCubemap,
            k_loadShader,
            k_loadMaterialGroup,
            k_loadMesh,
            k_restoreTexture,
            k_restoreCubemap,
            k_restoreMesh,
            k_restoreRenderTargetGroup,
            k_loadTargetGroup,
            k_begin,
            k_beginWithTargetGroup,
            k_applyCamera,
            k_applyAmbientLight,
            k_applyDirectionalLight,
            k_applyPointLight,
            k_applyMaterial,
            k_applyMesh,
            k_applyDynamicMesh,
            k_applyMeshBatch,
            k_applySkinnedAnimation,
            k_renderInstance,
            k_end,
            k_unloadTargetGroup,
            k_unloadMesh,
            k_unloadMaterialGroup,
            k_unloadShader,
            k_unloadTexture,
            k_unloadCubemap
        };
        
        /// @return The type of render command that this is.
        ///
        Type GetType() const noexcept { return m_type; }
        
        virtual ~RenderCommand() noexcept {}
        
    protected:
        /// Constructs the RenderCommand with the given type.
        ///
        /// @param type
        ///     The type of render command that this describes.
        ///
        RenderCommand(Type type) noexcept;
        
    private:
        Type m_type;
    };
}

#endif
