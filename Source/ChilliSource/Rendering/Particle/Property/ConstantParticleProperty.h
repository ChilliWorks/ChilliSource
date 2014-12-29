//
//  ConstantParticleProperty.h
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

#ifndef _CHILLISOURCE_RENDERING_PARTICLE_PROPERTY_CONSTANTPARTICLEPROPERTY_H_
#define _CHILLISOURCE_RENDERING_PARTICLE_PROPERTY_CONSTANTPARTICLEPROPERTY_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Rendering/Particle/Property/ParticleProperty.h>

namespace ChilliSource
{
	namespace Rendering
	{
		//------------------------------------------------------------------------------
		/// A simple particle property for containing a value that remains constant over
        /// the lifetime of a particle effect.
		///
		/// @author Ian Copland
		//------------------------------------------------------------------------------
		template <typename TPropertyType> class ConstantParticleProperty final : public ParticleProperty<TPropertyType>
		{
		public:
			//------------------------------------------------------------------------------
			/// Constructor.
			///
			/// @author Ian Copland
			///
			/// @param The static value.
			//------------------------------------------------------------------------------
			ConstantParticleProperty(TPropertyType in_value);
			//------------------------------------------------------------------------------
			/// @author Ian Copland
			///
			/// @param The normalised (0.0 - 1.0) particle effect playback progress.
			///
			/// @return simply returns the static value.
			//------------------------------------------------------------------------------
			TPropertyType GenerateValue(f32 in_playbackProgress) const override;
            
		private:
			TPropertyType m_value;
		};
        
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		template <typename TPropertyType> ConstantParticleProperty<TPropertyType>::ConstantParticleProperty(TPropertyType in_value)
			: m_value(in_value)
		{
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		template <typename TPropertyType> TPropertyType ConstantParticleProperty<TPropertyType>::GenerateValue(f32 in_playbackProgress) const
		{
			return m_value;
		}
	}
}

#endif