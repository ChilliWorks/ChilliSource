/*
 *  Colour.cpp
 *  moFlo
 *
 *  Created by Scott Downie on 27/09/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Core/Base/Colour.h>
#include <ChilliSource/Core/Math/MathUtils.h>

namespace ChilliSource
{
	namespace Core
	{
		
		const Colour Colour::WHITE(1.0f,1.0f,1.0f,1.0f);
		const Colour Colour::TRANSPARENT(0.0f,0.0f,0.0f,0.0f);
		const Colour Colour::BLACK(0.0f,0.0f,0.0f,1.0f);
		const Colour Colour::GREY(0.5f,0.5f,0.5f,1.0f);
		const Colour Colour::RED(1.0f,0.0f,0.0f,1.0f);
		const Colour Colour::GREEN(0.0f,1.0f,0.0f,1.0f);
		const Colour Colour::BLUE(0.0f,0.0f,1.0f,1.0f);
		const Colour Colour::MAGENTA(1.0f,0.0f,1.0f,1.0f);
		const Colour Colour::YELLOW(1.0f,1.0f,0.0f,1.0f);
		const Colour Colour::CYAN(0.0f,1.0f,1.0f,1.0f);
		const Colour Colour::CORNFLOWER_BLUE(0.4f,0.5f,0.8f,1.0f);
		const Colour Colour::LIGHTGREY(0.8f,0.8f,0.8f,1.0f);
		const Colour Colour::TURQUOISE(0.25f,0.88f,0.82f,1.0f);
		const Colour Colour::ORANGE(0.9f,0.4f,0.12f,1.0f);
		const Colour Colour::PURPLE(0.5f,0.0f,0.5f,1.0f);
		const Colour Colour::PINK(0.94f,0.36f,0.47f,1.0f);
		
        //--------------------------------------------------
		/// Constructor
		///
        /// Default
		//--------------------------------------------------
		Colour::Colour() : r(1.0f), g(1.0f), b(1.0f), a(1.0f)
		{
		}
		//--------------------------------------------------
		/// Constructor
		///
		/// Initialised a white colour container
		/// @param Red component
		/// @param Green component
		/// @param Blue component
		/// @param Alpha component
		//--------------------------------------------------
		Colour::Colour(f32 inR, f32 inG, f32 inB, f32 inA) : r(inR), g(inG), b(inB), a(inA)
		{
		}
		
		Colour::Colour(u8 inR, u8 inG, u8 inB, u8 inA)
		:r((f32)inR / 255.0f), g((f32)inG / 255.0f),	b((f32)inB / 255.0f),	a((f32)inA / 255.0f)
		{
			
		}
        
        Colour::Colour(u32 inudwRGBA)
        {
            u8 byR = (inudwRGBA >> 24) & 255;
            u8 byG = (inudwRGBA >> 16) & 255;
            u8 byB = (inudwRGBA >> 8) & 255;
            u8 byA = (inudwRGBA) & 255;
            
            r = (f32)byR/255.0f;
            g = (f32)byG/255.0f;
            b = (f32)byB/255.0f;
            a = (f32)byA/255.0f;
        }

		Colour Colour::FromByteRGBA(u8 inR, u8 inG, u8 inB, u8 inA)
        {
			return Colour(inR,inG,inB,inA);
		}
        Colour::ByteColour Colour::ColourToByteColour(const Colour& inCol)
        {
            Colour::ByteColour Col;
            
            inCol.r <= 0.0f ? Col.r = 0 : Col.r = MathUtils::Round(inCol.r * 255.0f);
            inCol.g <= 0.0f ? Col.g = 0 : Col.g = MathUtils::Round(inCol.g * 255.0f);
            inCol.b <= 0.0f ? Col.b = 0 : Col.b = MathUtils::Round(inCol.b * 255.0f);
            inCol.a <= 0.0f ? Col.a = 0 : Col.a = MathUtils::Round(inCol.a * 255.0f);

            return Col;
        }
	}
}
