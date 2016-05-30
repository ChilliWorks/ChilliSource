//
//  The MIT License (MIT)
//
//  Copyright (c) 2010 Tag Games Limited
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

#ifndef _CHILLISOURCE_RENDERING_BASE_RENDERPASS_H_
#define _CHILLISOURCE_RENDERING_BASE_RENDERPASS_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/Colour.h>

#include <vector>

namespace ChilliSource
{
    namespace Rendering
    {
        //TODO:Remove these when implemented
        class RenderLight {};
        class RenderPassObject {};

        /// Provides the ability to store a number of render pass objecs to render
        /// with a single light source.
        ///
        /// This is immutable and therefore thread-safe
        ///
        class RenderPass final
        {
        public:
            
            RenderPass() = default;
            
            ///
            /// @param light
            ///     The light to use for this pass
            /// @param renderPassObjects
            ///     The list of render pass objects for this pass
            ///
            RenderPass(const RenderLight& light, const std::vector<RenderPassObject>& renderPassObjects) noexcept;
            
            ///
            /// @return The light to use for this pass
            ///
            const RenderLight& GetLight() const noexcept { return m_light; }
            
            ///
            /// @return The list of render pass objects for this pass
            ///
            const std::vector<RenderPassObject>& GetRenderPassObjects() const noexcept { return m_renderPassObjects; }
            
        private:
            RenderLight m_light;
            std::vector<RenderPassObject> m_renderPassObjects;
        };
    }
}
#endif
