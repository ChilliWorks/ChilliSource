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

#ifndef _CHILLISOURCE_RENDERING_BASE_RENDERPASS_H_
#define _CHILLISOURCE_RENDERING_BASE_RENDERPASS_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/Colour.h>
#include <ChilliSource/Rendering/Base/RenderPassObject.h>
#include <ChilliSource/Rendering/ForwardDeclarations.h>
#include <ChilliSource/Rendering/Lighting/AmbientRenderLight.h>
#include <ChilliSource/Rendering/Lighting/DirectionalRenderLight.h>
#include <ChilliSource/Rendering/Lighting/PointRenderLight.h>

#include <vector>

namespace ChilliSource
{
    /// Provides the ability to store a number of render pass objecs to render
    /// with a single light source.
    ///
    /// This is immutable and therefore thread-safe
    ///
    class RenderPass final
    {
    public:
        CS_DECLARE_NOCOPY(RenderPass);
        
        /// Enum describing the light types a pass can hold
        ///
        enum class LightType
        {
            k_none,
            k_ambient,
            k_directional,
            k_point
        };
        
        RenderPass() noexcept;
        RenderPass(RenderPass&&) = default;
        RenderPass& operator=(RenderPass&&) = default;
        
        /// @param renderPassObjects
        ///     The list of render pass objects for this pass. Should be moved.
        ///
        RenderPass(std::vector<RenderPassObject> renderPassObjects) noexcept;
        
        /// @param light
        ///     The ambient light to use for this pass
        /// @param renderPassObjects
        ///     The list of render pass objects for this pass. Should be moved.
        ///
        RenderPass(const AmbientRenderLight& light, std::vector<RenderPassObject> renderPassObjects) noexcept;
        
        /// @param light
        ///     The point light to use for this pass
        /// @param renderPassObjects
        ///     The list of render pass objects for this pass. Should be moved.
        ///
        RenderPass(const PointRenderLight& light, std::vector<RenderPassObject> renderPassObjects) noexcept;
        
        /// @param light
        ///     The directional light to use for this pass
        /// @param renderPassObjects
        ///     The list of render pass objects for this pass. Should be moved.
        ///
        RenderPass(const DirectionalRenderLight& light, std::vector<RenderPassObject> renderPassObjects) noexcept;
        
        /// @return The light type set for this pass
        ///
        LightType GetLightType() const noexcept { return m_lightType; }
        
        /// Return the ambient light for this pass, if set. GetLightType() should be used
        /// to determine if this is set, if not this function will assert
        ///
        /// @return The ambient light to use for this pass
        ///
        const AmbientRenderLight& GetAmbientLight() const noexcept;
        
        /// Return the point light for this pass, if set. GetLightType() should be used
        /// to determine if this is set, if not this function will assert
        ///
        /// @return The ambient light to use for this pass
        ///
        const PointRenderLight& GetPointLight() const noexcept;
        
        /// Return the directional light for this pass, if set. GetLightType() should be used
        /// to determine if this is set, if not this function will assert
        ///
        /// @return The ambient light to use for this pass
        ///
        const DirectionalRenderLight& GetDirectionalLight() const noexcept;
        
        /// @return The list of render pass objects for this pass
        ///
        const std::vector<RenderPassObject>& GetRenderPassObjects() const noexcept { return m_renderPassObjects; }
        
    private:
        std::vector<RenderPassObject> m_renderPassObjects;
        LightType m_lightType;
        
        union
        {
            AmbientRenderLight m_ambientLight;
            PointRenderLight m_pointLight;
            DirectionalRenderLight m_directionalLight;
        };
    };
}
#endif
