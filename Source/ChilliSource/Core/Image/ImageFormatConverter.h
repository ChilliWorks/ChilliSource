//
//  ImageFormatConverter.h
//  moFlow
//
//  Created by Ian Copland on 10/04/2013.
//  Copyright (c) 2013 Tag Games Ltd. All rights reserved.
//

#ifndef MOFLOW_CORE_IMAGEFORMATCONVERTER_H
#define MOFLOW_CORE_IMAGEFORMATCONVERTER_H

#include <ChilliSource/Core/ForwardDeclarations.h>

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
            void RGBA8888ToRGB888(Core::CImage* inpImage);
            //---------------------------------------------------
            /// RGBA8888 To RGBA4444
            ///
            /// Converts an Image in format RGBA8888 to RGBA4444
            /// format.
            ///
            /// @param The image pointer.
            //---------------------------------------------------
            void RGBA8888ToRGBA4444(Core::CImage* inpImage);
            //---------------------------------------------------
            /// RGBA8888 To RGB565
            ///
            /// Converts an Image in format RGBA8888 to RGBA4444
            /// format.
            ///
            /// @param The image pointer.
            //---------------------------------------------------
            void RGBA8888ToRGB565(Core::CImage* inpImage);
            //---------------------------------------------------
            /// RGBA8888 To LUMA88
            ///
            /// Converts an Image in format RGBA8888 to LUMA88
            /// format.
            ///
            /// @param The image pointer.
            //---------------------------------------------------
            void RGBA8888ToLUMA88(Core::CImage* inpImage);
            //---------------------------------------------------
            /// RGBA8888 To LUM8
            ///
            /// Converts an Image in format RGBA8888 to LUMA88
            /// format.
            ///
            /// @param The image pointer.
            //---------------------------------------------------
            void RGBA8888ToLUM8(Core::CImage* inpImage);
            //---------------------------------------------------
            /// RGBA8888 To Depth 32
            ///
            /// Converts an Image in format RGBA8888 to Depth 32
            /// format
            ///
            /// @param The image pointer.
            //---------------------------------------------------
            void RGBA8888ToDepth32(Core::CImage* inpImage);
            //---------------------------------------------------
            /// RGBA8888 To Depth 16
            ///
            /// Converts an Image in format RGBA8888 to Depth 16
            /// format
            ///
            /// @param The image pointer.
            //---------------------------------------------------
            void RGBA8888ToDepth16(Core::CImage* inpImage);
        }
    }
}

#endif
