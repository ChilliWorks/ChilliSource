//
//  RandomParticleProperty.h
//  Chilli Source
//  Created by Ian Copland on 26/10/2014.
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

#ifndef _CHILLISOURCE_RENDERING_PARTICLE_PROPERTY_RANDOMPARTICLEPROPERTY_H_
#define _CHILLISOURCE_RENDERING_PARTICLE_PROPERTY_RANDOMPARTICLEPROPERTY_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Math/Random.h>
#include <ChilliSource/Rendering/Particle/Property/ParticleProperty.h>
#include <ChilliSource/Rendering/Particle/Property/ParticlePropertyValueParser.h>

namespace ChilliSource
{
	namespace Rendering
	{
		//-----------------------------------------------------------------------
		/// A particle property for getting a random value between the two given
		/// values. 
		///
		/// @author Ian Copland
		//-----------------------------------------------------------------------
		template <typename TPropertyType> class RandomParticleProperty final : public ParticleProperty<TPropertyType>
		{
		public:
			//----------------------------------------------------------------
			/// Constructor.
			///
			/// @author Ian Copland
			///
			/// @param The first value.
			/// @param The second value.
			//----------------------------------------------------------------
			RandomParticleProperty(TPropertyType in_first, TPropertyType in_second);
			//----------------------------------------------------------------
			/// Constructor. This will retreive the parameters of the random
			/// property from the given param dictionary.
			///
			/// TODO: !? describe the values that will be read!
			///
			/// @author Ian Copland
			///
			/// @param The string to parse to get the static value.
			//----------------------------------------------------------------
			RandomParticleProperty(const Core::ParamDictionary& in_params);
			//----------------------------------------------------------------
			/// @author Ian Copland
			///
			/// @param The normalised (0.0 - 1.0) particle effect playback 
			/// progress or normalised life progress of a particle depending
			/// on the context.
			///
			/// @param simply returns the static value.
			//----------------------------------------------------------------
			TPropertyType GenerateValue(f32 in_playbackProgress) const override;
		private:
			TPropertyType m_first;
			TPropertyType m_second;
		};
			//--------------------------------------------------
			//--------------------------------------------------
			template <typename TPropertyType> RandomParticleProperty<TPropertyType>::RandomParticleProperty(TPropertyType in_first, TPropertyType in_second)
				: m_first(in_first), m_second(in_second)
			{
			}
			//--------------------------------------------------
			//--------------------------------------------------
			template <typename TPropertyType> RandomParticleProperty<TPropertyType>::RandomParticleProperty(const Core::ParamDictionary& in_params)
			{
				//TODO: !?
				CS_LOG_FATAL("Unimplemented: RandomParticleProperty(const Core::ParamDictionary& in_params)");
			}
			//--------------------------------------------------
			//--------------------------------------------------
			template <typename TPropertyType> TPropertyType RandomParticleProperty<TPropertyType>::GenerateValue(f32 in_playbackProgress) const
			{
				return Core::Random::GenerateInRange(m_first, m_second);
			}
	}
}

#endif