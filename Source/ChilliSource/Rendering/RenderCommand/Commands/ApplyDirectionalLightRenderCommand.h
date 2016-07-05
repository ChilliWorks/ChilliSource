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

#ifndef _CHILLISOURCE_RENDERING_RENDERCOMMAND_COMMANDS_APPLYDIRECTIONALLIGHTRENDERCOMMAND_H_
#define _CHILLISOURCE_RENDERING_RENDERCOMMAND_COMMANDS_APPLYDIRECTIONALLIGHTRENDERCOMMAND_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/Colour.h>
#include <ChilliSource/Core/Math/Matrix4.h>
#include <ChilliSource/Core/Math/Vector3.h>
#include <ChilliSource/Rendering/RenderCommand/RenderCommand.h>

namespace ChilliSource
{
    /// A render command for applying the described directional light to the current context
    /// state.
    ///
    /// This must be instantiated via a RenderCommandList.
    ///
    /// This is immutable and therefore thread-safe.
    ///
    class ApplyDirectionalLightRenderCommand final : public RenderCommand
    {
    public:
        /// @return The colour of the light.
        ///
        const Colour& GetColour() const noexcept { return m_colour; }
        
        /// @return The direction of the light.
        ///
        const Vector3& GetDirection() const noexcept { return m_direction; }
        
        /// @return The view projection matrix of the light which is used as the camera when rendering
        ///     the shadow map.
        ///
        const Matrix4& GetLightViewProjection() const noexcept { return m_lightViewProjection; }
        
        /// @return The tolerence used to judge if an object is in shadow.
        ///
        f32 GetShadowTolerance() const noexcept { return m_shadowTolerance; }
        
        /// @return The render texture which should be used for the shadow map. Will be null if there is
        ///     no shadow map.
        ///
        const RenderTexture* GetShadowMapRenderTexture() const noexcept { return m_shadowMapRenderTexture; }
        
    private:
        friend class RenderCommandList;
        
        /// Creates a new instance with the given light colour.
        ///
        /// @param colour
        ///     The colour of the light.
        /// @param direction
        ///     The direction of the light.
        /// @param lightViewProjection
        ///     The view projection matrix of the light which is used as the camera when rendering
        ///     the shadow map.
        /// @param shadowTolerance
        ///     The tolerence used to judge if an object is in shadow.
        /// @param shadowMapRenderTexture
        ///     The render texture which should be used for the shadow map. Can be null if there is no
        ///     shadow map.
        ///
        ApplyDirectionalLightRenderCommand(const Colour& colour, const Vector3& direction, const Matrix4& lightViewProjection, f32 shadowTolerance, const RenderTexture* shadowMapRenderTexture) noexcept;
        
        Colour m_colour;
        Vector3 m_direction;
        Matrix4 m_lightViewProjection;
        f32 m_shadowTolerance;
        const RenderTexture* m_shadowMapRenderTexture;
    };
}

#endif
