//
//  ColourUtils.cpp
//  Chilli Source
//
//  Created by Scott Downie on 03/04/2014.
//  Copyright 2014 Tag Games. All rights reserved.
//

#include <ChilliSource/Core/Base/ColourUtils.h>

#include <ChilliSource/Core/Math/MathUtils.h>

namespace ChilliSource
{
	namespace Core
	{
        namespace ColourUtils
        {
            //-----------------------------------------------------------
            //-----------------------------------------------------------
            ByteColour ColourToByteColour(const Colour& in_colour)
            {
                CS_ASSERT(in_colour.r >= 0.0f && in_colour.r <= 1.0f, "Colour must be in range 0.0 - 1.0");
                CS_ASSERT(in_colour.g >= 0.0f && in_colour.g <= 1.0f, "Colour must be in range 0.0 - 1.0");
                CS_ASSERT(in_colour.b >= 0.0f && in_colour.b <= 1.0f, "Colour must be in range 0.0 - 1.0");
                CS_ASSERT(in_colour.a >= 0.0f && in_colour.a <= 1.0f, "Colour must be in range 0.0 - 1.0");
                
                ByteColour result;
                
                result.r = MathUtils::Round(in_colour.r * 255.0f);
                result.g = MathUtils::Round(in_colour.g * 255.0f);
                result.b = MathUtils::Round(in_colour.b * 255.0f);
                result.a = MathUtils::Round(in_colour.a * 255.0f);
                
                return result;
            }
            //-----------------------------------------------------------
            //-----------------------------------------------------------
            Colour PackedRGBAToColour(u32 in_rgba)
            {
                u8 byteR = (in_rgba >> 24) & 255;
                u8 byteG = (in_rgba >> 16) & 255;
                u8 byteB = (in_rgba >> 8) & 255;
                u8 byteA = (in_rgba) & 255;
                
                return Colour((f32)byteR/255.0f, (f32)byteG/255.0f, (f32)byteB/255.0f, (f32)byteA/255.0f);
            }
        }
	}
}
