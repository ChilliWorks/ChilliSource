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

#ifndef _CHILLISOURCE_RENDERING_SPRITE_SPRITEMESHBUILDER_H_
#define _CHILLISOURCE_RENDERING_SPRITE_SPRITEMESHBUILDER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Rendering/Model/RenderDynamicMesh.h>

namespace ChilliSource
{
    /// A collection of methods for building sprite RenderDynamicMeshes.
    ///
    /// These are thread-safe.
    ///
    namespace SpriteMeshBuilder
    {
        /// Generates a new RenderDynamicMesh for a sprite with the given properties. The sprite
        /// will be generated as a plane with its normal facing down the negative z axis.
        ///
        /// All required memory will be allocated from the given allocator.
        ///
        /// @param allocator
        ///     The allocator which should be used for all allocations.
        /// @param localPosition
        ///     The local position of the sprite. This is often used for offsetting the
        ///     sprite to account for cropping during texture packing.
        /// @param localSize
        ///     The size of the sprite in local space. The rendered size may be different
        ///     depending on the world transform.
        /// @param worldTransform
        ///     The world transform of the sprite.
        /// @param uvs
        ///     The UVs of each of the four vertices of the sprite.
        /// @param colour
        ///     The colour of the sprite. This will be applied to the vertex colours.
        /// @param alignmentAchor
        ///     The alignmentAnchor of the sprite.
        ///
        RenderDynamicMeshAUPtr Build(IAllocator* allocator, const Vector3& localPosition, const Vector2& localSize, const UVs& uvs, const Colour& colour, AlignmentAnchor alignmentAnchor) noexcept;
    }
}

#endif