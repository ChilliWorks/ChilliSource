//
//  ImageFormatConverter.h
//  Chilli Source
//  Created by Ian Copland on 10/04/2013.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2013 Tag Games Limited
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

#ifndef _CHILLISOURCE_CORE_IMAGE_IMAGEFORMATCONVERTER_H
#define _CHILLISOURCE_CORE_IMAGE_IMAGEFORMATCONVERTER_H

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Image/Image.h>

namespace ChilliSource
{
    namespace Core
    {
        //---------------------------------------------------------
        /// The image format converter provides a number of method
    	/// for converting from one image format to another.
    	///
    	/// @author Ian Copland
        //---------------------------------------------------------
        namespace ImageFormatConverter
        {
			//---------------------------------------------------
			/// A struct containing the buffer and size of raw
        	/// image data.
			///
			/// @author Ian Copland
			//---------------------------------------------------
        	struct ImageBuffer final
        	{
        		Image::ImageDataUPtr m_data;
        		u32 m_size = 0;

#ifdef CS_TARGETPLATFORM_WINDOWS
				//------------------------------------------------
				/// Visual C++ 12 doesn't support implicit move
				/// constructors, so this has to be manually
				/// specified.
				///
				/// @author Ian Copland
				//------------------------------------------------
				ImageBuffer() = default;
				//------------------------------------------------
				/// Visual C++ 12 doesn't support implicit move
				/// constructors, so this has to be manually
				/// specified.
				///
				/// @author Ian Copland
				///
				/// @param The image buffer to move.
				//------------------------------------------------
				ImageBuffer(ImageBuffer&& in_other);
#endif
        	};
			//---------------------------------------------------
			/// Converts an Image in format RGBA8888 to RGB888
			/// format. This will allocate a new buffer for the
        	/// target format data and release the previous
        	/// buffer.
        	///
        	/// @author Ian Copland
			///
			/// @param A pointer to the image to convert.
			//---------------------------------------------------
			void RGBA8888ToRGB888(Image* in_image);
			//---------------------------------------------------
			/// Converts an Image in format RGBA8888 to RGBA4444
			/// format. This will allocate a new buffer for the
        	/// target format data and release the previous
        	/// buffer.
        	///
        	/// @author Ian Copland
			///
			/// @param A pointer to the image to convert.
			//---------------------------------------------------
			void RGBA8888ToRGBA4444(Image* in_image);
			//---------------------------------------------------
			/// Converts an Image in format RGBA8888 to RGB565
			/// format. This will allocate a new buffer for the
        	/// target format data and release the previous
        	/// buffer.
        	///
        	/// @author Ian Copland
			///
			/// @param A pointer to the image to convert.
			//---------------------------------------------------
			void RGBA8888ToRGB565(Image* in_image);
			//---------------------------------------------------
			/// Converts an Image in format RGBA8888 to LumA88
			/// format. This will allocate a new buffer for the
        	/// target format data and release the previous
        	/// buffer.
        	///
        	/// @author Ian Copland
			///
			/// @param A pointer to the image to convert.
			//---------------------------------------------------
			void RGBA8888ToLumA88(Image* in_image);
			//---------------------------------------------------
			/// Converts an Image in format RGBA8888 to LumA88
			/// format. This will allocate a new buffer for the
        	/// target format data and release the previous
        	/// buffer.
        	///
        	/// @author Ian Copland
			///
			/// @param A pointer to the image to convert.
			//---------------------------------------------------
			void RGBA8888ToLum8(Image* in_image);
			//---------------------------------------------------
			/// Creates new RGB888 image data from RGBA8888 image
			/// data.
			///
			/// @author Ian Copland
			///
			/// @param The input RGBA8888 image data buffer.
			/// @param The size of the RGBA8888 image data buffer.
			///
			/// @return The output RGB888 image data.
			//---------------------------------------------------
			ImageBuffer RGBA8888ToRGB888(const u8* in_imageData, u32 in_imageDataSize);
			//---------------------------------------------------
			/// Creates new RGBA4444 image data from RGBA8888 image
			/// data.
			///
			/// @author Ian Copland
			///
			/// @param The input RGBA8888 image data buffer.
			/// @param The size of the RGBA8888 image data buffer.
			///
			/// @return The output RGBA4444 image data.
			//---------------------------------------------------
			ImageBuffer RGBA8888ToRGBA4444(const u8* in_imageData, u32 in_imageDataSize);
			//---------------------------------------------------
			/// Creates new RGB565 image data from RGBA8888 image
			/// data.
			///
			/// @author Ian Copland
			///
			/// @param The input RGBA8888 image data buffer.
			/// @param The size of the RGBA8888 image data buffer.
			///
			/// @return The output RGB565 image data.
			//---------------------------------------------------
			ImageBuffer RGBA8888ToRGB565(const u8* in_imageData, u32 in_imageDataSize);
			//---------------------------------------------------
			/// Creates new LumA88 image data from RGBA8888 image
			/// data.
			///
			/// @author Ian Copland
			///
			/// @param The input RGBA8888 image data buffer.
			/// @param The size of the RGBA8888 image data buffer.
			///
			/// @return The output LumA88 image data.
			//---------------------------------------------------
			ImageBuffer RGBA8888ToLumA88(const u8* in_imageData, u32 in_imageDataSize);
			//---------------------------------------------------
			/// Creates new LUM8 image data from RGBA8888 image
			/// data.
			///
			/// @author Ian Copland
			///
			/// @param The input RGBA8888 image data buffer.
			/// @param The size of the RGBA8888 image data buffer.
			///
			/// @return The output Lum8 image data.
			//---------------------------------------------------
			ImageBuffer RGBA8888ToLum8(const u8* in_imageData, u32 in_imageDataSize);
        }
    }
}

#endif
