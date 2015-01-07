//
//  ComponentwiseRandomConstantParticleProperty.h
//  Chilli Source
//  Created by Ian Copland on 29/12/2014.
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

#ifndef _CHILLISOURCE_RENDERING_PARTICLE_PROPERTY_COMPONENTWISERANDOMCONSTANTPARTICLEPROPERTY_H_
#define _CHILLISOURCE_RENDERING_PARTICLE_PROPERTY_COMPONENTWISERANDOMCONSTANTPARTICLEPROPERTY_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Math/Random.h>
#include <ChilliSource/Rendering/Particle/Property/ParticleProperty.h>

namespace ChilliSource
{
    namespace Rendering
    {
        //------------------------------------------------------------------------------
        /// A particle property for getting a random value between the given lower and
        /// upper value. If the value has multiple components (i.e Vector2, Matrix4,
        /// Colour, etc) then each component will be randomised individually.
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------------
        template <typename TPropertyType> class ComponentwiseRandomConstantParticleProperty final : public ParticleProperty<TPropertyType>
        {
        public:
            //------------------------------------------------------------------------------
            /// Constructor.
            ///
            /// @author Ian Copland
            ///
            /// @param The lower value.
            /// @param The upper value.
            //------------------------------------------------------------------------------
            ComponentwiseRandomConstantParticleProperty(TPropertyType in_lowerValue, TPropertyType in_upperValue);
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param The normalised (0.0 - 1.0) particle effect playback progress. This is
            /// ignored for a random property.
            ///
            /// @return a random value between the lower and upper values the property was
            /// created with.
            //------------------------------------------------------------------------------
            TPropertyType GenerateValue(f32 in_playbackProgress) const override;
            
        private:
            TPropertyType m_lowerValue;
            TPropertyType m_upperValue;
        };
        
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename TPropertyType> ComponentwiseRandomConstantParticleProperty<TPropertyType>::ComponentwiseRandomConstantParticleProperty(TPropertyType in_lowerValue, TPropertyType in_upperValue)
            : m_lowerValue(in_lowerValue), m_upperValue(in_upperValue)
        {
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename TPropertyType> TPropertyType ComponentwiseRandomConstantParticleProperty<TPropertyType>::GenerateValue(f32 in_playbackProgress) const
        {
            return Core::Random::GenerateComponentwise(m_lowerValue, m_upperValue);
        }
    }
}

#endif