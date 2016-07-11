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

#ifndef _CHILLISOURCE_RENDERING_RENDERCOMMAND_COMMANDS_BEGINWITHTARGETGROUPRENDERCOMMAND_H_
#define _CHILLISOURCE_RENDERING_RENDERCOMMAND_COMMANDS_BEGINWITHTARGETGROUPRENDERCOMMAND_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/Colour.h>
#include <ChilliSource/Rendering/RenderCommand/RenderCommand.h>

namespace ChilliSource
{
    /// A render command for beginning rendering to the the given target group with the given
    /// clear colour.
    ///
    /// This must be instantiated via a RenderCommandList.
    ///
    /// This is immutable and therefore thread-safe.
    ///
    class BeginWithTargetGroupRenderCommand final : public RenderCommand
    {
    public:
        /// @return The render target group that
        ///
        const RenderTargetGroup* GetRenderTargetGroup() const noexcept { return m_renderTargetGroup; };
        
        /// @return The clear colour.
        ///
        const Colour& GetClearColour() const noexcept { return m_clearColour; };
        
    private:
        friend class RenderCommandList;
        
        /// Creates a new instance with the given RenderTargetGroup and clear colour.
        ///
        /// @param renderTargetGroup
        ///     The render target group to render into.
        /// @param clearColour
        ///     The clear colour.
        ///
        BeginWithTargetGroupRenderCommand(const RenderTargetGroup* renderTargetGroup, const Colour& clearColour) noexcept;
        
        const RenderTargetGroup* m_renderTargetGroup;
        Colour m_clearColour;
    };
}

#endif
