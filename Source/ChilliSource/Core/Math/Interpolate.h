//
//  Interpolate.h
//  Chilli Source
//  Created by Ian Copland on 29/12/2014.
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

#ifndef _CHILLISOURCE_CORE_MATH_INTERPOLATE_H_
#define _CHILLISOURCE_CORE_MATH_INTERPOLATE_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/String/StringUtils.h>

#include <json/json.h>

#include <functional>

namespace ChilliSource
{
    //------------------------------------------------------------------------------
    /// A collection of Interpolate functions.
    ///
    /// @author Ian Copland
    //------------------------------------------------------------------------------
    namespace Interpolate
    {
        //------------------------------------------------------------------------------
        /// @author Ian Copland
        ///
        /// @param The string name of the curve.
        ///
        /// @return The associated Interpolate function.
        //------------------------------------------------------------------------------
        std::function<f32(f32)> GetInterpolateFunction(const std::string& in_name);
        //------------------------------------------------------------------------------
        /// The linear Interpolate function.
        ///
        /// @author Ian Copland
        ///
        /// @param The input x. This must be in the range 0.0 to 1.0.
        ///
        /// @return The output y. This will be in the range 0.0 to 1.0.
        //------------------------------------------------------------------------------
        f32 Linear(f32 in_x);
        //------------------------------------------------------------------------------
        /// The ease in quad Interpolate function.
        ///
        /// @author Ian Copland
        ///
        /// @param The input x. This must be in the range 0.0 to 1.0.
        ///
        /// @return The output y. This will be in the range 0.0 to 1.0.
        //------------------------------------------------------------------------------
        f32 EaseInQuad(f32 in_x);
        //------------------------------------------------------------------------------
        /// The ease out quad Interpolate function.
        ///
        /// @author Ian Copland
        ///
        /// @param The input x. This must be in the range 0.0 to 1.0.
        ///
        /// @return The output y. This will be in the range 0.0 to 1.0.
        //------------------------------------------------------------------------------
        f32 EaseOutQuad(f32 in_x);
        //------------------------------------------------------------------------------
        /// The smooth step Interpolate function.
        ///
        /// @author Ian Copland
        ///
        /// @param The input x. This must be in the range 0.0 to 1.0.
        ///
        /// @return The output y. This will be in the range 0.0 to 1.0.
        //------------------------------------------------------------------------------
        f32 SmoothStep(f32 in_x);
        //------------------------------------------------------------------------------
        /// The linear ping pong Interpolate function.
        ///
        /// @author Ian Copland
        ///
        /// @param The input x. This must be in the range 0.0 to 1.0.
        ///
        /// @return The output y. This will be in the range 0.0 to 1.0.
        //------------------------------------------------------------------------------
        f32 LinearPingPong(f32 in_x);
        //------------------------------------------------------------------------------
        /// The ease in quad ping pong Interpolate function.
        ///
        /// @author Ian Copland
        ///
        /// @param The input x. This must be in the range 0.0 to 1.0.
        ///
        /// @return The output y. This will be in the range 0.0 to 1.0.
        //------------------------------------------------------------------------------
        f32 EaseInQuadPingPong(f32 in_x);
        //------------------------------------------------------------------------------
        /// The ease out quad ping pong Interpolate function.
        ///
        /// @author Ian Copland
        ///
        /// @param The input x. This must be in the range 0.0 to 1.0.
        ///
        /// @return The output y. This will be in the range 0.0 to 1.0.
        //------------------------------------------------------------------------------
        f32 EaseOutQuadPingPong(f32 in_x);
        //------------------------------------------------------------------------------
        /// The smooth step ping pong Interpolate function.
        ///
        /// @author Ian Copland
        ///
        /// @param The input x. This must be in the range 0.0 to 1.0.
        ///
        /// @return The output y. This will be in the range 0.0 to 1.0.
        //------------------------------------------------------------------------------
        f32 SmoothStepPingPong(f32 in_x);
    }
}

#endif