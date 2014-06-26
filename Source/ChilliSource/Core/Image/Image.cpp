//
//  Image.cpp
//  Chilli Source
//  Created by Scott Downie on 01/10/2010.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2010 Tag Games Limited
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

#include <ChilliSource/Core/Image/Image.h>

#include <ChilliSource/Core/Image/ImageCompression.h>
#include <ChilliSource/Core/Image/ImageFormat.h>

namespace ChilliSource
{
	namespace Core
	{
		CS_DEFINE_NAMEDTYPE(Image);
    
        //----------------------------------------------------------------
        //----------------------------------------------------------------
        ImageUPtr Image::Create()
        {
            return ImageUPtr(new Image());
        }
        //----------------------------------------------------------------
		//----------------------------------------------------------------
		bool Image::IsA(Core::InterfaceIDType in_interfaceId) const
		{
			return in_interfaceId == Image::InterfaceID;
		}
        //----------------------------------------------------------------
        //----------------------------------------------------------------
        void Image::Build(const Descriptor& in_desc, ImageDataUPtr in_imageData)
        {
            m_dataDesc = in_desc;
            m_imageData = std::move(in_imageData);
        }
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		ImageFormat Image::GetFormat() const
		{
			return m_dataDesc.m_format;
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		u32 Image::GetWidth() const
		{
			return m_dataDesc.m_width;
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		u32 Image::GetHeight() const
		{
			return m_dataDesc.m_height;
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		const u8* Image::GetData() const
		{
			return m_imageData.get();
		}
        //----------------------------------------------------------------
        //----------------------------------------------------------------
        Image::ImageDataUPtr&& Image::MoveData()
        {
            return std::move(m_imageData);
        }
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		ImageCompression Image::GetCompression() const
		{
			return m_dataDesc.m_compression;
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		u32 Image::GetDataSize() const
		{
			return m_dataDesc.m_dataSize;
		}
	}
}
