//
//  DrawableUtils.cpp
//  Chilli Source
//  Created by Scott Downie on 03/10/2014.
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

#include <ChilliSource/UI/Drawable/DrawableUtils.h>

#include <ChilliSource/Rendering/Texture/Texture.h>
#include <ChilliSource/Rendering/Texture/UVs.h>

namespace ChilliSource
{
    namespace UI
    {
        namespace DrawableUtils
        {
            //----------------------------------------------------------------------------
            //----------------------------------------------------------------------------
            Rendering::TextureAtlas::Frame BuildFrame(const Rendering::Texture* in_texture, const Rendering::TextureAtlas* in_textureAtlas, const std::string& in_atlasId, const Rendering::UVs& in_virtualUVs)
            {
                Rendering::TextureAtlas::Frame outputFrame;
                
                if (in_textureAtlas != nullptr && in_atlasId.empty() == false)
                {
                    outputFrame = in_textureAtlas->GetFrame(in_atlasId);
                    outputFrame.m_uvs.m_u += (in_virtualUVs.m_u * outputFrame.m_uvs.m_s);
                    outputFrame.m_uvs.m_v += (in_virtualUVs.m_v * outputFrame.m_uvs.m_t);
                    outputFrame.m_uvs.m_s *= in_virtualUVs.m_s;
                    outputFrame.m_uvs.m_t *= in_virtualUVs.m_t;
                }
                else
                {
                    Core::Vector2 texSize;
                    if(in_texture != nullptr)
                    {
                        texSize.x = (f32)in_texture->GetWidth() * in_virtualUVs.m_s;
                        texSize.y = (f32)in_texture->GetHeight() * in_virtualUVs.m_t;
                    }
                    
                    outputFrame.m_croppedSize = texSize;
                    outputFrame.m_originalSize = texSize;
                    outputFrame.m_uvs = in_virtualUVs;
                    outputFrame.m_offset = Core::Vector2::k_zero;
                }
                
                return outputFrame;
            }
        }
    }
}