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

#ifndef _CHILLISOURCE_RENDERING_BASE_RENDERPASSUTILS_H_
#define _CHILLISOURCE_RENDERING_BASE_RENDERPASSUTILS_H_

#include <ChilliSource/ChilliSource.h>

#include <ChilliSource/Rendering/Base/RenderPass.h>
#include <ChilliSource/Rendering/Base/RenderPassObjectSorter.h>

#include <vector>

namespace ChilliSource
{
    /// Collection of functions for render pass objects
    ///
    namespace RenderPassVisibilityChecker
    {
        /// Parses a collection of RenderObjects and generates a list of them that are considered
        /// to be visible and within the passed camera's view frustrum.
        ///
        /// @param taskContext
        ///     Context to manage any spawned tasks
        /// @param camera
        ///     The camera who will decide the objects visibility
        /// @param renderObjects
        ///     The collection of RenderObjects whos visibility is to be checked
        ///
        /// @return Collection of visible RenderObjects
        ///
        std::vector<RenderObject> CalculateVisibleObjects(const TaskContext& taskContext, const RenderCamera& camera, const std::vector<RenderObject>& renderObjects) noexcept;
    }
}

#endif
