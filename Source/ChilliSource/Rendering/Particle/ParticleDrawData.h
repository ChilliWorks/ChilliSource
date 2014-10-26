//
//  ParticleDrawData.h
//  Chilli Source
//  Created by Ian Copland on 20/10/2014.
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

#ifndef _CHILLISOURCE_RENDERING_PARTICLE_PARTICLEDRAWDATA_H_
#define _CHILLISOURCE_RENDERING_PARTICLE_PARTICLEDRAWDATA_H_

#include <ChilliSource/ChilliSource.h>

#include <ChilliSource/Core/Math/Vector3.h>
#include <ChilliSource/Core/Math/Vector2.h>
#include <ChilliSource/Core/Math/Quaternion.h>
#include <ChilliSource/Core/Base/Colour.h>

namespace ChilliSource
{
	namespace Rendering
	{
		//-----------------------------------------------------------------------
		/// A struct containing just the information required for drawing a
		/// particle.
		///
		/// @author Ian Copland
		//-----------------------------------------------------------------------
		struct ParticleDrawData final
		{
			bool m_isActive = false;
			CSCore::Vector3 m_position;
			CSCore::Vector2 m_scale = CSCore::Vector2(1.0f, 1.0f);
			f32 m_rotation = 0.0f;
			CSCore::Colour m_colour = CSCore::Colour::k_white;
		};
	}
}

#endif