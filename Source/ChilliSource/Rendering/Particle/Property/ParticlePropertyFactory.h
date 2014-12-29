//
//  ParticlePropertyFactory.h
//  Chilli Source
//  Created by Ian Copland on 04/11/2014.
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

#ifndef _CHILLISOURCE_RENDERING_PARTICLE_PROPERTY_PARTICLEPROPERTYFACTORY_H_
#define _CHILLISOURCE_RENDERING_PARTICLE_PROPERTY_PARTICLEPROPERTYFACTORY_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/String/StringUtils.h>

#include <json/json.h>

namespace ChilliSource
{
	namespace Rendering
	{
		//------------------------------------------------------------------------------
		/// A collection of methods for creating particle properties.
		///
		/// @author Ian Copland
		//------------------------------------------------------------------------------
		namespace ParticlePropertyFactory
		{
			//------------------------------------------------------------------------------
			/// Creates a new particle property for the type described in the template
            /// parameter from the given json.
            ///
            /// The following are the different types of particle property:
            ///
            /// "Constant": A simple constant value. If the given JSON is just a string
            /// as opposed to a json object the type is assumed to be a constant. This takes
            /// only one parameter: 'Value'.
            ///
            /// "RandomConstant": A random value between two bounds. The bounds remain
            /// constant over the life of the particle effect. This takes two arguments:
            /// 'LowerValue' and 'UpperValue'.
            ///
            /// "ComponentwiseRandomConstant": A random value between two bounds. The bounds
            /// remain constant over the life of the particle effect. If the value is
            /// comprised of multiple components (i.e Vector3, Matrix4, Colour, etc) each is
            /// randomised separately. This takes two arguments: 'LowerValue' and
            /// 'UpperValue'.
            ///
            /// "Curve": A value that changes over the life time of the particle effect.
            /// Different curve types can be supplied to describe how the start and end
            /// values are interpolated. This takes three arguments: 'Curve', 'StartValue'
            /// and 'EndValue'.
            ///
            /// "RandomCurve": A random value between two bounds. The bounds change over the
            /// lifetime of the particle effect. Different curve types can be supplied to
            /// describe how the start and end values are interpolated. This takes five
            /// arguments: 'Curve', 'StartLowerValue', 'StartUpperValue', 'EndLowerValue'
            /// and 'EndUpperValue'.
            ///
            /// "ComponentwiseRandomCurve": A random value between two bounds. The bounds
            /// change over the lifetime of the particle effect. Different curve types can
            /// be supplied to describe how the start and end values are interpolated. If
            /// the value is comprised of multiple components (i.e Vector3, Matrix4, Colour,
            /// etc) each is randomised separately. This takes five arguments: 'Curve',
            /// 'StartLowerValue', 'StartUpperValue', 'EndLowerValue' and 'EndUpperValue'.
            ///
            /// The following are the different curve types available:
            ///
            /// "Linear"
            /// "EaseInQuad"
            /// "EaseOutQuad"
            /// "SmoothStep"
            /// "LinearPingPong"
            /// "EaseInQuadPingPong"
            /// "EaseOutQuadPingPong"
            /// "SmoothStepPingPong"
			///
			/// @author Ian Copland
			///
			/// @param The json value.
			///
			/// @return The output particle property.
			//------------------------------------------------------------------------------
			template <typename TType> ParticlePropertyUPtr<TType> CreateProperty(const Json::Value& in_json);
		}
	}
}

#include <ChilliSource/Rendering/Particle/Property/ParticlePropertyFactoryImpl.h>

#endif