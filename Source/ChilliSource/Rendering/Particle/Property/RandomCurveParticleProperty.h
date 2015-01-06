//
//  RandomCurveParticleProperty.h
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

#ifndef _CHILLISOURCE_RENDERING_PARTICLE_PROPERTY_RANDOMCURVEPARTICLEPROPERTY_H_
#define _CHILLISOURCE_RENDERING_PARTICLE_PROPERTY_RANDOMCURVEPARTICLEPROPERTY_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Math/Random.h>
#include <ChilliSource/Rendering/Particle/Property/ParticleProperty.h>

namespace ChilliSource
{
    namespace Rendering
    {
        //------------------------------------------------------------------------------
        /// A particle property describing a value which is randomly generated between
        /// bounds. The bounds changes over the lifetime of the particle effect. The
        /// curve function is described by a delegate provided in the constructor.
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------------
        template <typename TPropertyType> class RandomCurveParticleProperty final : public ParticleProperty<TPropertyType>
        {
        public:
            //------------------------------------------------------------------------------
            /// A delegate for a function which describes the curve. This must accept values
            /// in the range 0.0 - 1.0 and output values in the range 0.0 - 1.0.
            ///
            /// @author Ian Copland
            ///
            /// @param The current normalised (0.0 - 1.0) particle effect progress.
            ///
            /// @return The interpolation factor.
            //------------------------------------------------------------------------------
            using CurveFunction = std::function<f32(f32)>;
            //------------------------------------------------------------------------------
            /// Constructor.
            ///
            /// @author Ian Copland
            ///
            /// @param The start lower value.
            /// @param The start upper value.
            /// @param The end lower value.
            /// @param The end upper value.
            /// @param The curve function.
            //------------------------------------------------------------------------------
            RandomCurveParticleProperty(TPropertyType in_startLowerValue, TPropertyType in_startUpperValue, TPropertyType in_endLowerValue, TPropertyType in_endUpperValue, const CurveFunction& in_curveFunction);
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param The normalised (0.0 - 1.0) particle effect playback progress.
            ///
            /// @return The generated value.
            //------------------------------------------------------------------------------
            TPropertyType GenerateValue(f32 in_playbackProgress) const override;
            
        private:
            TPropertyType m_startLowerValue;
            TPropertyType m_startUpperValue;
            TPropertyType m_endLowerValue;
            TPropertyType m_endUpperValue;
            CurveFunction m_curveFunction;
        };
        
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename TPropertyType> RandomCurveParticleProperty<TPropertyType>::RandomCurveParticleProperty(TPropertyType in_startLowerValue, TPropertyType in_startUpperValue, TPropertyType in_endLowerValue,
                                                                                                                  TPropertyType in_endUpperValue, const CurveFunction& in_curveFunction)
            : m_startLowerValue(in_startLowerValue), m_startUpperValue(in_startUpperValue), m_endLowerValue(in_endLowerValue), m_endUpperValue(in_endUpperValue), m_curveFunction(in_curveFunction)
        {
            CS_ASSERT(m_curveFunction != nullptr, "A curve function must be provided.");
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        template <typename TPropertyType> TPropertyType RandomCurveParticleProperty<TPropertyType>::GenerateValue(f32 in_playbackProgress) const
        {
            CS_ASSERT(in_playbackProgress >= 0.0f && in_playbackProgress <= 1.0f, "Playback progress must be in the range 0.0 to 1.0.");
            
            f32 interpolationFactor = m_curveFunction(in_playbackProgress);
            
            TPropertyType lowerBound = m_startLowerValue + (m_endLowerValue - m_startLowerValue) * interpolationFactor;
            TPropertyType upperBound = m_startUpperValue + (m_endUpperValue - m_startUpperValue) * interpolationFactor;
            
            return Core::Random::Generate(lowerBound, upperBound);
        }
    }
}

#endif