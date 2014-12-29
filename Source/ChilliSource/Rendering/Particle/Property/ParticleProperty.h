//
//  ParticleProperty.h
//  Chilli Source
//  Created by Ian Copland on 06/10/2014.
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

#ifndef _CHILLISOURCE_RENDERING_PARTICLE_PROPERTY_PARTICLEPROPERTY_H_
#define _CHILLISOURCE_RENDERING_PARTICLE_PROPERTY_PARTICLEPROPERTY_H_

#include <ChilliSource/ChilliSource.h>

namespace ChilliSource
{
	namespace Rendering
	{
		//------------------------------------------------------------------------------
		/// A base class for different particle property types. This allows properties
        /// more complex than just a single value, for example a property that selects a
        /// random value within a certain range, or that changes over the life of the
        /// particle effect.
        ///
        /// The documentation for Particle Property Factory describes the various types
        /// of particle property that can be created.
		///
		/// @author Ian Copland
		//------------------------------------------------------------------------------
		template <typename TPropertyType> class ParticleProperty
		{
		public:
			//------------------------------------------------------------------------------
			/// Generates a new value within the confines of the property's settings.
			///
			/// @author Ian Copland
			///
			/// @param The normalised (0.0 - 1.0) particle effect playback progress.
			///
			/// @return The generated value.
			//------------------------------------------------------------------------------
			virtual TPropertyType GenerateValue(f32 in_playbackProgress) const = 0;
			//------------------------------------------------------------------------------
			/// Destructor.
			///
			/// @author Ian Copland
			//------------------------------------------------------------------------------
			virtual ~ParticleProperty() {};
		};
	}
}

#endif