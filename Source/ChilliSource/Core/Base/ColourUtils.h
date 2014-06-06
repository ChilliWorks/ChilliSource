//
//  ColourUtils.h
//  Chilli Source
//  Created by Scott Downie on 03/04/2014.
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

#ifndef _CHILLISOURCE_CORE_BASE_COLOURUTILS_H_
#define _CHILLISOURCE_CORE_BASE_COLOURUTILS_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/ByteColour.h>
#include <ChilliSource/Core/Base/Colour.h>

namespace ChilliSource
{
	namespace Core
	{
        namespace ColourUtils
        {
            //-----------------------------------------------------------
            /// Convert a colour with components in the range 0 - 255
            /// into a normalised colour container in the range 0.0 - 1.0
            ///
            /// @author S Downie
            ///
            /// @param Red 0 - 255
            /// @param Green 0 - 255
            /// @param Blue 0 - 255
            /// @param Alpha 0 - 255
            ///
            /// @return Normalised colour 0.0 - 1.0
            //-----------------------------------------------------------
            inline Colour ByteColourToColour(u8 in_byteR, u8 in_byteG, u8 in_byteB, u8 in_byteA)
            {
                return Colour((f32)in_byteR/255.0f, (f32)in_byteG/255.0f, (f32)in_byteB/255.0f, (f32)in_byteA/255.0f);
            }
            //-----------------------------------------------------------
            /// Convert a colour with components in the range 0 - 255
            /// into a normalised colour container in the range 0.0 - 1.0
            ///
            /// @author S Downie
            ///
            /// @param Byte colour 0 - 255
            ///
            /// @return Normalised colour 0.0 - 1.0
            //-----------------------------------------------------------
            inline Colour ByteColourToColour(ByteColour in_colour)
            {
                return Colour((f32)in_colour.r/255.0f, (f32)in_colour.g/255.0f, (f32)in_colour.b/255.0f, (f32)in_colour.a/255.0f);
            }
            //-----------------------------------------------------------
            /// Convert a colour with components in the range 0 - 255
            /// that are packed into a single u32 into a normalised colour
            /// container in the range 0.0 - 1.0
            ///
            /// @author S Downie
            ///
            /// @param RGBA
            ///
            /// @return Normalised colour 0.0 - 1.0
            //-----------------------------------------------------------
            Colour PackedRGBAToColour(u32 in_rgba);
            //-----------------------------------------------------------
            /// Convert a colour container with normalised range
            /// into a byte colour with range 0 - 255
            ///
            /// @author S Downie
            ///
            /// @param Normalised colour 0.0 - 1.0
            ///
            /// @return Byte colour 0 - 255
            //-----------------------------------------------------------
            ByteColour ColourToByteColour(const Colour& in_colour);
            //-----------------------------------------------------------
            /// Convert a colour container with normalised range
            /// into a byte colour with range 0 - 255
            ///
            /// @author S Downie
            ///
            /// @param Normalised red 0.0 - 1.0
            /// @param Normalised green 0.0 - 1.0
            /// @param Normalised blue 0.0 - 1.0
            /// @param Normalised alpha 0.0 - 1.0
            ///
            /// @return Byte colour 0 - 255
            //-----------------------------------------------------------
            ByteColour ColourToByteColour(f32 in_red, f32 in_green, f32 in_blue, f32 in_alpha);
        }
	}
}

#endif