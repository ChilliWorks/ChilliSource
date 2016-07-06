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

#ifndef _CHILLISOURCE_RENDERING_BASE_RENDERSORT_H_
#define _CHILLISOURCE_RENDERING_BASE_RENDERSORT_H_

#include <ChilliSource/ChilliSource.h>

#include <ChilliSource/Rendering/Base/RenderPassObject.h>
#include <ChilliSource/Rendering/ForwardDeclarations.h>

#include <functional>
#include <vector>

namespace ChilliSource
{
    /// Collection of sort functions for render pass objects
    ///
    namespace RenderPassObjectSorter
    {
        /// Sorts a collection of opaque RenderPassObjects based on if they share a material
        /// and then by z position (Front to back)
        ///
        /// @param camera
        ///     The camera to use to determine z-distance.
        /// @param renderPassObjects
        ///     The list of render pass objects to sort.
        ///
        void OpaqueSort(const RenderCamera& camera, std::vector<RenderPassObject>& renderPassObjects) noexcept;
        
        /// Sorts a collection of transparent RenderPassObjects based on their z position (Back to front)
        ///
        /// @param camera
        ///     The camera to use to determine z-distance
        /// @param renderPassObjects
        ///     The list of render pass objects to sort.
        ///
        void TransparentSort(const RenderCamera& camera, std::vector<RenderPassObject>& renderPassObjects) noexcept;
        
        /// Sorts a collection of RenderPassObjects based on their priority value. Lower priority values
        /// will be rendered first. If objects have the same priority then they will be ordered by material.
        ///
        /// @param renderPassObjects
        ///     The list of render pass objects to sort.
        ///
        void PrioritySort(std::vector<RenderPassObject>& renderPassObjects) noexcept;
    };
}

#endif
