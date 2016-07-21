//
//  The MIT License (MIT)
//
//  Copyright (c) 2014 Tag Games Limited
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

#ifndef _CHILLISOURCE_RENDERING_AMBIENTLIGHTCOMPONENT_H_
#define _CHILLISOURCE_RENDERING_AMBIENTLIGHTCOMPONENT_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/Colour.h>
#include <ChilliSource/Core/Entity/Component.h>

namespace ChilliSource
{
    /// A component which describes an ambient light. While this is in the scene, all
    /// lit objects will have the light colour descibed applied to it.
    ///
    /// This is not thread-safe and should only be accessed from the main thread.
    ///
    class AmbientLightComponent final : public Component
    {
    public:
        CS_DECLARE_NAMEDTYPE(AmbientLightComponent);
        
        /// Creates a new ambient light with the given colour and intensity.
        ///
        /// @param colour
        ///     The colour of the ambient light.
        /// @param intensity
        ///     (Optional) The intensity of the ambient light. This defaults to 1.0.
        ///
        AmbientLightComponent(const Colour& colour, f32 intensity = 1.0f) noexcept;
        
        /// Allows querying of whether or not this system implements the interface described by the
        /// given interface Id. Typically this is not called directly as the templated equivalent
        /// IsA<Interface>() is preferred.
        ///
        /// @param interfaceId
        ///     The Id of the interface.
        ///
        /// @return Whether or not the interface is implemented.
        ///
        bool IsA(InterfaceIDType interfaceId) const noexcept override;
        
        /// @return The colour of the ambient light.
        ///
        void SetColour(const Colour& colour) noexcept { m_colour = colour; }
        
        /// @return The intensity of the ambient light.
        ///
        void SetIntensity(f32 intensity) noexcept { m_intensity = intensity; }
        
        /// @return The colour of the ambient light.
        ///
        const Colour& GetColour() const noexcept { return m_colour; }
        
        /// @return The intensity of the ambient light.
        ///
        f32 GetIntensity() const noexcept { return m_intensity; }
        
        /// @return The colour of the ambient light with the intesity applied to it.
        ///
        Colour GetFinalColour() const noexcept { return m_colour * m_intensity; }
        
    private:
        /// This is called during the render snapshot stage of the render pipeline to collect a copy of
        /// the current state of the renderable portion of the scene. This will add all relevant light
        /// data to this snapshot.
        ///
        /// @param renderSnapshot
        ///     The snapshot that the light data will be added to.
        /// @param frameAllocator
        ///     Allocate any render memory for this frame from here
        ///
        void OnRenderSnapshot(RenderSnapshot& renderSnapshot, IAllocator* frameAllocator) noexcept override;
        
        Colour m_colour;
        f32 m_intensity;
    };
}

#endif