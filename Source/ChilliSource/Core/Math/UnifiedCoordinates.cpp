//
//  UnifiedCoordinates.cpp
//  ChilliSource
//  Created by Scott Downie on 22/04/2011.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2011 Tag Games Limited
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

#include <ChilliSource/Core/Math/UnifiedCoordinates.h>

namespace ChilliSource
{
    const UnifiedVector2 UnifiedVector2::ZERO(Vector2::k_zero, Vector2::k_zero);
    
    //------------------------------------------------------
    /// Get Absolute
    ///
    /// @return Absolute vector
    //------------------------------------------------------
    const Vector2& UnifiedVector2::GetAbsolute() const
    {
        return vAbsolute;
    }
    //------------------------------------------------------
    /// Get Relative
    ///
    /// @return Relative vector
    //------------------------------------------------------
    const Vector2& UnifiedVector2::GetRelative() const
    {
        return vRelative;
    }
}
