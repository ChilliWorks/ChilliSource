//
//  Image.h
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

#ifndef _CHILLISOURCE_CORE_IMAGE_IMAGE_H_
#define _CHILLISOURCE_CORE_IMAGE_IMAGE_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Resource/Resource.h>

namespace ChilliSource
{
	namespace Core
	{
        //----------------------------------------------------------------
        /// A resource for a loaded image in one of the formats supported
        /// in ImageFormat.h
        ///
        /// @author S Downie
        //----------------------------------------------------------------
		class Image : public Resource
		{
		public:
			CS_DECLARE_NAMEDTYPE(Image);
            
            using ImageDataUPtr = std::unique_ptr<u8[]>;
            
            //----------------------------------------------------------------
            /// Holds the information about the image data such as size,
            /// compression, etc. Used to build the image resource
            ///
            /// @author S Downie
            //----------------------------------------------------------------
            struct Descriptor
            {
                ImageCompression m_compression;
                ImageFormat m_format;
                u32 m_width;
                u32 m_height;
                u32 m_dataSize;
            };
			//----------------------------------------------------------------
			/// @author S Downie
			///
			/// @param Interface type to query
            ///
			/// @return Whether the object is of given type
			//----------------------------------------------------------------
            bool IsA(Core::InterfaceIDType in_interfaceId) const override;
            //----------------------------------------------------------------
            /// Populate the image with the given data. The image resource
            /// takes ownership of the raw image data
            ///
            /// @author S Downie
            ///
            /// @param Image data descriptor
            /// @param Image data
            //----------------------------------------------------------------
            void Build(const Descriptor& in_desc, ImageDataUPtr in_imageData);
			//----------------------------------------------------------------
			/// @author S Downie
			///
			/// @return Image format (RGB, RGBA, etc)
			//----------------------------------------------------------------
			ImageFormat GetFormat() const;
            //----------------------------------------------------------------
			/// @author S Downie
			///
			/// @return image compression type
			//----------------------------------------------------------------
			ImageCompression GetCompression() const;
			//----------------------------------------------------------------
			/// @author S Downie
			///
			/// @return Image width
			//----------------------------------------------------------------
			u32 GetWidth() const;
			//----------------------------------------------------------------
			/// @author S Downie
			///
			/// @return Image height
			//----------------------------------------------------------------
			u32 GetHeight() const;
			//----------------------------------------------------------------
			/// @author S Downie
			///
			/// @return Return size of the image in bytes
			//----------------------------------------------------------------
			u32 GetDataSize() const;
			//----------------------------------------------------------------
			/// Use datasize, width, height, format and compression
            /// to decode
            ///
            /// @author S Downie
			///
			/// @return Image data.
			//----------------------------------------------------------------
			const u8* GetData() const;
            
        private:
            friend class ResourcePool;
            friend class Rendering::TextureProvider;
            friend class Rendering::CubemapProvider;
            //----------------------------------------------------------------
            /// Factory create method. Only called by resource pool or by
            /// texture provider.
            ///
            /// @author S Downie
            ///
            /// @return An empty image resource
            //----------------------------------------------------------------
            static ImageUPtr Create();
            //----------------------------------------------------------------
            /// Private constructor to force the use of the factory create
            /// method
            ///
            /// @author S Downie
            //----------------------------------------------------------------
            Image() = default;
            //----------------------------------------------------------------
            /// @author S Downie
			///
			/// @return Ownership of image data.
			//----------------------------------------------------------------
            ImageDataUPtr&& MoveData();
            
		private:
			
            Descriptor m_dataDesc;
            
            ImageDataUPtr m_imageData;
		};
		
	}

}
#endif
