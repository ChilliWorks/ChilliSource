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
    namespace Rendering
    {
        /// Collection of functions for render pass objects
        ///
    	namespace RenderPassVisibilityChecker
    	{
            /// Parses a list of RenderObjects and generates a list of RenderPassObjects for
            /// each RenderObject that has a Base pass defined.
            ///
            /// @param renderObjects
            ///     A list of RenderObjects to parse
            ///
            /// @return A collection of RenderPassObjects, one for each RenderObject Base pass
            ///
            std::vector<RenderPassObject> GetBaseRenderPassObjects(const std::vector<RenderObject>& renderObjects) noexcept;
            
            /// Parses a list of RenderObjects and generates a list of RenderPassObjects for
            /// each RenderObject that has a Directional pass defined.
            ///
            /// @param renderObjects
            ///     A list of RenderObjects to parse
            ///
            /// @return A collection of RenderPassObjects, one for each RenderObject Directional pass
            ///
            std::vector<RenderPassObject> GetDirectionalLightRenderPassObjects(const std::vector<RenderObject>& renderObjects) noexcept;
            
            /// Parses a list of RenderObjects and generates a list of RenderPassObjects for
            /// each RenderObject that has a Transparent pass defined.
            ///
            /// @param renderObjects
            ///     A list of RenderObjects to parse
            ///
            /// @return A collection of RenderPassObjects, one for each RenderObject Transparent pass
            ///
            std::vector<RenderPassObject> GetTransparentRenderPassObjects(const std::vector<RenderObject>& renderObjects) noexcept;
            
            /// Checks if the RenderPassObject is contained in the camera view frustrum.
            ///
            /// @param camera
            ///     The camera who will decide the objects visibility
            /// @param renderPassObject
            ///     The RenderPassObject whos visibility is to be checked
            ///
            /// @return If the RenderPassObject is considered to be visible
            ///
            bool IsRenderObjectVisible(const RenderCamera& camera, const RenderPassObject& renderPassObject) noexcept;
            
            /// Parses a collection of RenderPassObjects and generates a list of them that are considered
            /// to be visible and within the passed camera's view frustrum.
            ///
            /// @param outerTaskContext
            ///     Context to manage any spawned tasks
            /// @param camera
            ///     The camera who will decide the objects visibility
            /// @param renderPassObjects
            ///     The collection of RenderPassObjects whos visibility is to be checked
            ///
            /// @return Collection of visible RenderPassObjects
            ///
            std::vector<RenderPassObject> CalculateVisibleObjects(const TaskContext& outerTaskContext, const RenderCamera& camera, const std::vector<RenderPassObject>& renderPassObjects) noexcept;
            
            /// Gather all sorted visible RenderPassObjects into a RenderPass with the passed in light source.
            ///
            /// @param outerTaskContext
            ///     Context to manage any spawned tasks
            /// @param renderFrame
            ///     Current frame data
            ///
            /// @return The list of target render pass groups
            ///
            RenderPass CompileRenderPass(const TaskContext& taskContext, const RenderCamera& camera, const RenderAmbientLight& light, const std::vector<RenderPassObject>& renderPassObjects,
                                         const RenderSortFunc& sort) noexcept;
    	}
    }
}

#endif
