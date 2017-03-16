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

#ifndef _CHILLISOURCE_CORE_BASE_SCREENINFO_H_
#define _CHILLISOURCE_CORE_BASE_SCREENINFO_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Math/Vector2.h>
#include <vector>

namespace ChilliSource
{
    /// This class contains all information necessary for building
    /// the Screen class.
    ///
    /// This class is immutable, and therefore thread-safe.
    ///
    class ScreenInfo final
    {
    public:
        /// Constructs the ScreenInfo class.
        ///
        /// @param initialResolution
        ///         The initial resolution for the application.
        /// @param densityScale
        ///         The density scale.
        /// @param inverseDensityScale
        ///         The inverse density scale.
        /// @param supportedResolutions
        ///         A list of resolutions supported by the display.
        ///
        ScreenInfo(const Vector2& initialResolution, f32 densityScale, f32 inverseDensityScale, const std::vector<Integer2>& supportedResolutions) noexcept;

        /// @return The initial resolution for the application.
        ///
        const Vector2& GetInitialResolution() const noexcept;

        /// @return The density scale.
        ///
        f32 GetDensityScale() const noexcept;

        /// @return The inverse density scale.
        ///
        f32 GetInverseDensityScale() const noexcept;

        /// @return A list of resolutions supported by the display.
        ///
        const std::vector<Integer2>& GetSupportedFullscreenResolutions() const noexcept;

    private:
        Vector2 m_initialResolution;
        f32 m_densityScale;
        f32 m_inverseDensityScale;
        std::vector<Integer2> m_supportedResolutions;

    };
}

#endif