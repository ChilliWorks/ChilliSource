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

#ifndef _CHILLISOURCE_RENDERING_MATERIAL_RENDERMATERIALGROUP_H_
#define _CHILLISOURCE_RENDERING_MATERIAL_RENDERMATERIALGROUP_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Memory/UniquePtr.h>
#include <ChilliSource/Rendering/Material/RenderMaterial.h>
#include <ChilliSource/Rendering/Model/VertexFormat.h>

#include <array>
#include <vector>

namespace ChilliSource
{
    /// A series of RenderMaterials. There are multiple collections of render materials, one for each
    /// type of vertex format. Each of these sets contain a list of RenderMaterials which pertain
    /// to different passes in the render system, the combination of which describes the complete
    /// material of an object.
    ///
    /// If the render material for a pass is null, this indicates that RenderObjects with the
    /// RenderMaterialGroup should not be included in the pass.
    ///
    /// If there are no materials for a given vertex format then the format isn't supported and this
    /// will assert.
    ///
    /// This is immutable and therefore thread safe.
    ///
    class RenderMaterialGroup final
    {
    public:
        CS_DECLARE_NOCOPY(RenderMaterialGroup);
        
        static constexpr u32 k_numMaterialSlots = 7;
        
        /// A collection of RenderMaterials for a single vertex format.
        ///
        /// This is immutable and therefore thread safe.
        ///
        class Collection final
        {
        public:
            /// Creates a new instance with the given vertex format and series of RenderMaterials
            ///
            /// @param vertexFormat
            ///     The vertex format.
            /// @param renderMaterials
            ///     The render materials for each pass.
            ///
            Collection(const VertexFormat& vertexFormat, const std::array<const RenderMaterial*, k_numMaterialSlots>& renderMaterials) noexcept;
            
            /// @return The vertex format.
            ///
            const VertexFormat& GetVertexFormat() const noexcept { return m_vertexFormat; }
            
            /// Looks up a render material for the given pass index.
            ///
            /// @param passIndex
            ///     The pass index to look up.
            ///
            /// @return The render materials array
            ///
            const RenderMaterial* GetRenderMaterial(u32 passIndex) const noexcept;
            
        private:
            VertexFormat m_vertexFormat;
            std::array<const RenderMaterial*, k_numMaterialSlots> m_renderMaterials;
        };
        
        /// Creates a new instance with the given collections of RenderMaterials.
        ///
        /// SHOULD ONLY BE CREATED VIA RenderMaterialGroupManager
        ///
        /// @param renderMaterials
        ///     The list of all materials in the group.
        /// @param collections
        ///     The list of material collections. Should be moved
        ///
        RenderMaterialGroup(std::vector<UniquePtr<RenderMaterial>> renderMaterials, std::vector<Collection> collections) noexcept;
        
        /// Evaluates whether or not the given RenderMaterial is part of this group.
        ///
        /// @param renderMaterial
        ///     The RenderMaterial which should be checked.
        ///
        /// @return Whether or not the RenderMaterial is part of this group.
        ///
        bool Contains(const RenderMaterial* renderMaterial) const noexcept;
        
        /// Looks up the render material for the given vertex format and pass index.
        ///
        /// @param vertexFormat
        ///     The vertex format for which to get a render material.
        /// @param passIndex
        ///     The pass to look up.
        ///
        /// @return The static mesh render material. May be null.
        ///
        const RenderMaterial* GetRenderMaterial(const VertexFormat& vertexFormat, u32 passIndex) const noexcept;
        
        /// Exposes the render materials so their extra data can be set during loading
        /// and unloading.
        ///
        /// @return An unsorted list of all RenderMaterials in the group.
        ///
        const std::vector<RenderMaterial*>& GetRenderMaterials() noexcept { return m_renderMaterialsRaw; }
        
    private:
        
        std::vector<UniquePtr<RenderMaterial>> m_renderMaterials;
        std::vector<RenderMaterial*> m_renderMaterialsRaw;
        std::vector<Collection> m_collections;
    };
}

#endif
