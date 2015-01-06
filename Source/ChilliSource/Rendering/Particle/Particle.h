//
//  Particle.h
//  Chilli Source
//  Created by Ian Copland on 07/10/2014.
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

#ifndef _CHILLISOURCE_RENDERING_PARTICLE_PARTICLE_H_
#define _CHILLISOURCE_RENDERING_PARTICLE_PARTICLE_H_

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
		/// A struct containing the properties for a single particle.
		///
		/// @author Ian Copland
		//-----------------------------------------------------------------------
		struct Particle final
		{
			bool m_isActive = false;
			f32 m_lifetime = 0.0f;
			f32 m_energy = 0.0f;
			Core::Vector3 m_position;
            Core::Vector2 m_scale = Core::Vector2::k_one;
			f32 m_rotation = 0.0f;
			Core::Colour m_colour = Core::Colour::k_white;
			Core::Vector3 m_velocity;
			f32 m_angularVelocity = 0.0f;
		};
	}
}

#endif