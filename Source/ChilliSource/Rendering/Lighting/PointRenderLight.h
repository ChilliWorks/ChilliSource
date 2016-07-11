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

#ifndef _CHILLISOURCE_RENDERING_LIGHTING_RENDERPOINTLIGHT_H_
#define _CHILLISOURCE_RENDERING_LIGHTING_RENDERPOINTLIGHT_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/Colour.h>
#include <ChilliSource/Core/Math/Vector3.h>

namespace ChilliSource
{
    /// A standard-layout container for data the renderer needs which pertains to a single
    /// point light, such as the colour and position.
    ///
    class PointRenderLight final
    {
    public:
        
        /// Creates a new instance of the container with default black colour with
        /// position and attenuation zeroed.
        ///
        PointRenderLight() noexcept;
        
        /// Creates a new instance of the container with the given light colour and position.
        ///
        /// @param colour
        ///     The colour of the light.
        /// @param position
        ///     The position of the light.
        /// @param attenuation
        ///     The attenuation of the light.
        /// @param rangeOfInfluence
        ///     The range of influence of the light.
        ///
        PointRenderLight(const Colour& colour, const Vector3& position, const Vector3& attenuation, f32 rangeOfInfluence) noexcept;
        
        /// @return The colour of the light.
        ///
        const Colour& GetColour() const noexcept { return m_colour; }
        
        /// @return The position of the light.
        ///
        const Vector3& GetPosition() const noexcept { return m_position; }
        
        /// @return The attenuation of the light.
        ///
        const Vector3& GetAttenuation() const noexcept { return m_attenuation; }
        
        /// @return The range of influence of the light.
        ///
        f32 GetRangeOfInfluence() const noexcept { return m_rangeOfInfluence; }
        
    private:
        Colour m_colour;
        Vector3 m_position;
        Vector3 m_attenuation;
        f32 m_rangeOfInfluence;
    };
}

#endif
