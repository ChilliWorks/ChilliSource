//
//  UVs.cpp
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

#include <ChilliSource/Rendering/Texture/UVs.h>

namespace ChilliSource
{
	namespace Rendering
	{
        //----------------------------------------------------------
        //----------------------------------------------------------
        UVs::UVs(f32 in_u, f32 in_v, f32 in_s, f32 in_t)
        : m_u(in_u), m_v(in_v), m_s(in_s), m_t(in_t)
        {
            
        }
        //----------------------------------------------------------
        //----------------------------------------------------------
        UVs UVs::FlipVertically(const UVs& in_uvs)
        {
            UVs result = in_uvs;
            
            result.m_v += result.m_t;
            result.m_t *= -1;
            
            return result;
        }
        //----------------------------------------------------------
        //----------------------------------------------------------
        UVs UVs::FlipHorizontally(const UVs& in_uvs)
        {
            UVs result = in_uvs;
            
            result.m_u += result.m_s;
            result.m_s *= -1;
            
            return result;
        }
        //----------------------------------------------------------
        //----------------------------------------------------------
        UVs UVs::FlipDiagonally(const UVs& in_uvs)
        {
            UVs result = in_uvs;
            
            result.m_u += result.m_s;
            result.m_s *= -1;
            
            result.m_v += result.m_t;
            result.m_t *= -1;
            
            return result;
        }
	}
}
