//
//  ColourUtils.h
//  Chilli Source
//
//  Created by Scott Downie on 03/04/2014.
//  Copyright 2014 Tag Games. All rights reserved.
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
        }
	}
}

#endif