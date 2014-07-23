//
//  BlendMode.h
//  Chilli Source
//  Created by Scott Downie on 09/04/2014.
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

#ifndef _CHILLISOURCE_RENDERING_BASE_BLENDMODE_H_
#define _CHILLISOURCE_RENDERING_BASE_BLENDMODE_H_

namespace ChilliSource
{
	namespace Rendering
	{
        //---------------------------------------------
        /// The blend modes that are used to describe
        /// a blend function for rendering. The blend
        /// function takes 2 modes - the source and
        /// the destination.
        ///
        /// @author S Downie
        //---------------------------------------------
        enum class BlendMode
        {
            k_zero,
            k_one,
            k_sourceCol,
            k_oneMinusSourceCol,
            k_sourceAlpha,
            k_oneMinusSourceAlpha,
            k_destAlpha,
            k_oneMinusDestAlpha
        };
	}
}

#endif
