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

#include <ChilliSource/Rendering/Lighting/PointRenderLight.h>

namespace ChilliSource
{
    //------------------------------------------------------------------------------
    PointRenderLight::PointRenderLight() noexcept
        : m_colour(Colour::k_black), m_position(Vector3::k_zero), m_attenuation(1.0f, 0.0f, 0.0f), m_rangeOfInfluence(0.0f)
    {
    }
    //------------------------------------------------------------------------------
    PointRenderLight::PointRenderLight(const Colour& colour, const Vector3& position, const Vector3& attenuation, f32 rangeOfInfluence) noexcept
        : m_colour(colour), m_position(position), m_attenuation(attenuation), m_rangeOfInfluence(rangeOfInfluence)
    {
    }
}