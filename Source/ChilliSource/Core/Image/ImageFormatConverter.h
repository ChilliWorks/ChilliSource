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

#ifndef MOFLOW_CORE_IMAGEFORMATCONVERTER_H
#define MOFLOW_CORE_IMAGEFORMATCONVERTER_H

#include <ChilliSource/ChilliSource.h>

namespace ChilliSource
{
    namespace Core
    {
        //=====================================================
        /// Image Format Converter
        ///
        /// The image format converter provides a number of
        /// method for converting from one image format to
        /// another.
        //=====================================================
        namespace ImageFormatConverter
        {
            //---------------------------------------------------
            /// RGBA8888 To RGB888
            ///
            /// Converts an Image in format RGBA8888 to RGB888
            /// format.
            ///
            /// @param The image pointer.
            //---------------------------------------------------
            void RGBA8888ToRGB888(Image* inpImage);
            //---------------------------------------------------
            /// RGBA8888 To RGBA4444
            ///
            /// Converts an Image in format RGBA8888 to RGBA4444
            /// format.
            ///
            /// @param The image pointer.
            //---------------------------------------------------
            void RGBA8888ToRGBA4444(Image* inpImage);
            //---------------------------------------------------
            /// RGBA8888 To RGB565
            ///
            /// Converts an Image in format RGBA8888 to RGBA4444
            /// format.
            ///
            /// @param The image pointer.
            //---------------------------------------------------
            void RGBA8888ToRGB565(Image* inpImage);
            //---------------------------------------------------
            /// RGBA8888 To LUMA88
            ///
            /// Converts an Image in format RGBA8888 to LUMA88
            /// format.
            ///
            /// @param The image pointer.
            //---------------------------------------------------
            void RGBA8888ToLUMA88(Image* inpImage);
            //---------------------------------------------------
            /// RGBA8888 To LUM8
            ///
            /// Converts an Image in format RGBA8888 to LUMA88
            /// format.
            ///
            /// @param The image pointer.
            //---------------------------------------------------
            void RGBA8888ToLUM8(Image* inpImage);
            //---------------------------------------------------
            /// RGBA8888 To Depth 32
            ///
            /// Converts an Image in format RGBA8888 to Depth 32
            /// format
            ///
            /// @param The image pointer.
            //---------------------------------------------------
            void RGBA8888ToDepth32(Image* inpImage);
            //---------------------------------------------------
            /// RGBA8888 To Depth 16
            ///
            /// Converts an Image in format RGBA8888 to Depth 16
            /// format
            ///
            /// @param The image pointer.
            //---------------------------------------------------
            void RGBA8888ToDepth16(Image* inpImage);
        }
    }
}

#endif
