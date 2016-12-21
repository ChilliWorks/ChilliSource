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

#include <ChilliSource/Rendering/Texture/CubemapResourceOptions.h>

#include <ChilliSource/Core/Cryptographic/HashCRC32.h>

namespace ChilliSource
{
    //-------------------------------------------------------
    //-------------------------------------------------------
    CubemapResourceOptions::CubemapResourceOptions(bool in_mipmaps, TextureFilterMode in_filter, TextureWrapMode in_wrapS, TextureWrapMode in_wrapT)
    {
        m_options.m_hasMipMaps = in_mipmaps;
        m_options.m_filterMode = in_filter;
        m_options.m_wrapModeS = in_wrapS;
        m_options.m_wrapModeT = in_wrapT;
    }
    //-------------------------------------------------------
    //-------------------------------------------------------
    u32 CubemapResourceOptions::GenerateHash() const
    {
        return HashCRC32::GenerateHashCode((const s8*)&m_options, sizeof(Options));
    }
    //-------------------------------------------------------
    //-------------------------------------------------------
    bool CubemapResourceOptions::IsMipMapsEnabled() const
    {
        return m_options.m_hasMipMaps;
    }
    //-------------------------------------------------------
    //-------------------------------------------------------
    TextureWrapMode CubemapResourceOptions::GetWrapModeS() const
    {
        return m_options.m_wrapModeS;
    }
    //-------------------------------------------------------
    //-------------------------------------------------------
    TextureWrapMode CubemapResourceOptions::GetWrapModeT() const
    {
        return m_options.m_wrapModeT;
    }
    //-------------------------------------------------------
    //-------------------------------------------------------
    TextureFilterMode CubemapResourceOptions::GetFilterMode() const
    {
        return m_options.m_filterMode;
    }
}

