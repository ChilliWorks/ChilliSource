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
                return ColourToByteColour(in_colour.r, in_colour.g, in_colour.b, in_colour.a);
            }
            //-----------------------------------------------------------
            //-----------------------------------------------------------
            ByteColour ColourToByteColour(f32 in_red, f32 in_green, f32 in_blue, f32 in_alpha)
            {
                CS_ASSERT(in_red >= 0.0f && in_red <= 1.0f, "Colour must be in range 0.0 - 1.0");
                CS_ASSERT(in_green >= 0.0f && in_green <= 1.0f, "Colour must be in range 0.0 - 1.0");
                CS_ASSERT(in_blue >= 0.0f && in_blue <= 1.0f, "Colour must be in range 0.0 - 1.0");
                CS_ASSERT(in_alpha >= 0.0f && in_alpha <= 1.0f, "Colour must be in range 0.0 - 1.0");
                
                ByteColour result;
                
                result.r = MathUtils::Round(in_red * 255.0f);
                result.g = MathUtils::Round(in_green * 255.0f);
                result.b = MathUtils::Round(in_blue * 255.0f);
                result.a = MathUtils::Round(in_alpha * 255.0f);
                
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
