//
//  UVs.h
//  Chilli Source
//  Created by Scott Downie on 13/06/2014.
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

#ifndef _CHILLISOURCE_RENDERING_TEXTURE_UVS_H_
#define _CHILLISOURCE_RENDERING_TEXTURE_UVS_H_

#include <ChilliSource/ChilliSource.h>

namespace ChilliSource
{
	namespace Rendering
	{
        //--------------------------------------------------------------
        /// Holds the description of UV coordinates (normalised)
        /// U-V (Top-Left) S-T (Width-Height)
        ///
        /// @author S Downie
        //--------------------------------------------------------------
        class UVs
        {
        public:
            //----------------------------------------------------------
            /// Constructor
            ///
            /// @author S Downie
            //----------------------------------------------------------
            UVs() = default;
            //----------------------------------------------------------
            /// Constructor
            ///
            /// @author S Downie
            ///
            /// @param U (normalised 0.0 - 1.0)
            /// @param V (normalised 0.0 - 1.0)
            /// @param S (normalised 0.0 - 1.0)
            /// @param T (normalised 0.0 - 1.0)
            //----------------------------------------------------------
            UVs(f32 in_u, f32 in_v, f32 in_s, f32 in_t);
            //----------------------------------------------------------
            /// @author S Downie
            ///
            /// @param UVs
            ///
            /// @return UVs flipped around the horizontal axis
            //----------------------------------------------------------
            static UVs FlipVertically(const UVs& in_uvs);
            //----------------------------------------------------------
            /// @author S Downie
            ///
            /// @param UVs
            ///
            /// @return UVs flipped around the vertical axis
            //----------------------------------------------------------
            static UVs FlipHorizontally(const UVs& in_uvs);
            //----------------------------------------------------------
            /// @author S Downie
            ///
            /// @param UVs
            ///
            /// @return UVs flipped around the vertical axis and the
            /// horizontal axis
            //----------------------------------------------------------
            static UVs FlipDiagonally(const UVs& in_uvs);
            
        public:
            
            f32 m_u = 0.0f;
            f32 m_v = 0.0f;
            f32 m_s = 1.0f;
            f32 m_t = 1.0f;
        };
	}
}

#endif