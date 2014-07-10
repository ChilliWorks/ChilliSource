//
//  CubemapResourceOptions.cpp
//  Chilli Source
//  Created by Scott Downie on 16/05/2014.
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
	namespace Rendering
	{
        //-------------------------------------------------------
        //-------------------------------------------------------
        CubemapResourceOptions::CubemapResourceOptions(bool in_mipmaps, Texture::FilterMode in_filter, Texture::WrapMode in_wrapS, Texture::WrapMode in_wrapT, bool in_restoreCubemapDataEnabled)
        {
            m_options.m_hasMipMaps = in_mipmaps;
            m_options.m_filterMode = in_filter;
            m_options.m_wrapModeS = in_wrapS;
            m_options.m_wrapModeT = in_wrapT;
            m_options.m_restoreCubemapDataEnabled = in_restoreCubemapDataEnabled;
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        u32 CubemapResourceOptions::GenerateHash() const
        {
            return Core::HashCRC32::GenerateHashCode((const s8*)&m_options, sizeof(Options));
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        bool CubemapResourceOptions::IsMipMapsEnabled() const
        {
            return m_options.m_hasMipMaps;
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        Texture::WrapMode CubemapResourceOptions::GetWrapModeS() const
        {
            return m_options.m_wrapModeS;
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        Texture::WrapMode CubemapResourceOptions::GetWrapModeT() const
        {
            return m_options.m_wrapModeT;
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        Texture::FilterMode CubemapResourceOptions::GetFilterMode() const
        {
            return m_options.m_filterMode;
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        bool CubemapResourceOptions::IsRestoreCubemapDataEnabled() const
        {
            return m_options.m_restoreCubemapDataEnabled;
        }
	}
}

