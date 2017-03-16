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

#include <ChilliSource/Core/Base/ScreenInfo.h>

namespace ChilliSource
{
    //----------------------------------------------------------------
    ScreenInfo::ScreenInfo(const Vector2& initialResolution, f32 densityScale, f32 inverseDensityScale, const std::vector<Integer2>& supportedResolutions) noexcept
        : m_initialResolution(initialResolution), m_densityScale(densityScale), m_inverseDensityScale(inverseDensityScale), m_supportedResolutions(supportedResolutions)
    {
    }

    //----------------------------------------------------------------
    const Vector2& ScreenInfo::GetInitialResolution() const noexcept
    {
        return m_initialResolution;
    }

    //----------------------------------------------------------------
    f32 ScreenInfo::GetDensityScale() const noexcept
    {
        return m_densityScale;
    }

    //----------------------------------------------------------------
    f32 ScreenInfo::GetInverseDensityScale() const noexcept
    {
        return m_inverseDensityScale;
    }

    //----------------------------------------------------------------
    const std::vector<Integer2>& ScreenInfo::GetSupportedFullscreenResolutions() const noexcept
    {
        return m_supportedResolutions;
    }


}