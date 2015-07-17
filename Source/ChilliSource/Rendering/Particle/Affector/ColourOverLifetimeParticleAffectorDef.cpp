//
//  ColourOverLifetimeParticleAffectorDef.cpp
//  Chilli Source
//  Created by Ian Copland on 02/11/2014.
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

#include <ChilliSource/Rendering/Particle/Affector/ColourOverLifetimeParticleAffectorDef.h>

#include <ChilliSource/Rendering/Particle/Affector/ColourOverLifetimeParticleAffector.h>
#include <ChilliSource/Rendering/Particle/Property/ParticlePropertyFactory.h>

namespace ChilliSource
{
	namespace Rendering
	{
        namespace
        {
            const char k_intermediateColours[] = "IntermediateColours";
            const char k_targetColourProperty[] = "TargetColourProperty";
            const char k_colourProperty[] = "ColourProperty";
            const char k_timeProperty[] = "TimeProperty";
        }
        
		CS_DEFINE_NAMEDTYPE(ColourOverLifetimeParticleAffectorDef);
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		ColourOverLifetimeParticleAffectorDef::ColourOverLifetimeParticleAffectorDef(ParticlePropertyUPtr<Core::Colour> in_targetColour)
			: m_targetColourProperty(std::move(in_targetColour))
		{
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		ColourOverLifetimeParticleAffectorDef::ColourOverLifetimeParticleAffectorDef(const Json::Value& in_paramsJson, const LoadedDelegate& in_asyncDelegate)
		{
			// Target Colour
			const auto& jsonValue = in_paramsJson.get(k_targetColourProperty, Json::nullValue);
			CS_ASSERT(jsonValue.isNull() == false, "No target colour property provided.");
			m_targetColourProperty = ParticlePropertyFactory::CreateProperty<Core::Colour>(jsonValue);
            
            // Intermediate Colours
            u32 index = 0;
            const auto& jsonArray = in_paramsJson.get(k_intermediateColours, Json::nullValue);
            for(const auto& jsonIntermediateColour : jsonArray)
            {
                CS_ASSERT(index < 2, "Can't handle more than 2 intermediate colours");
                
                const auto& jsonColour = jsonIntermediateColour.get(k_colourProperty, Json::nullValue);
                m_intermediateColours[index].m_colourProperty = ParticlePropertyFactory::CreateProperty<Core::Colour>(jsonColour);
                
                const auto& jsonTime = jsonIntermediateColour.get(k_timeProperty, Json::nullValue);
                m_intermediateColours[index].m_timeProperty = ParticlePropertyFactory::CreateProperty<f32>(jsonTime);
                
                ++index;
            }
            
            // TODO: ask Ian
            // Complete unfilled colours
            while(index < 2)
            {
                m_intermediateColours[index].m_colourProperty = ParticlePropertyFactory::CreateProperty<Core::Colour>("0.0 0.0 0.0 0.0");
                m_intermediateColours[index].m_timeProperty = ParticlePropertyFactory::CreateProperty<f32>("0.0");
                ++index;
            }
            
            // Curve
            m_curveName = in_paramsJson.get("Curve", "Linear").asString();

			//call the loaded delegate if required.
			if (in_asyncDelegate != nullptr)
			{
				in_asyncDelegate(this);
			}
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		bool ColourOverLifetimeParticleAffectorDef::IsA(Core::InterfaceIDType in_interfaceId) const
		{
			return (ParticleAffectorDef::InterfaceID == in_interfaceId || ColourOverLifetimeParticleAffectorDef::InterfaceID == in_interfaceId);
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		ParticleAffectorUPtr ColourOverLifetimeParticleAffectorDef::CreateInstance(Core::dynamic_array<Particle>* in_particleArray) const
		{
			return ParticleAffectorUPtr(new ColourOverLifetimeParticleAffector(this, in_particleArray));
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		const ParticleProperty<Core::Colour>* ColourOverLifetimeParticleAffectorDef::GetTargetColourProperty() const
		{
			return m_targetColourProperty.get();
        }
        //----------------------------------------------------------------
        //----------------------------------------------------------------
        const std::string& ColourOverLifetimeParticleAffectorDef::GetCurveName() const
        {
            return m_curveName;
        }
        //----------------------------------------------------------------
        //----------------------------------------------------------------
        const ColourOverLifetimeParticleAffectorDef::IntermediateColour* ColourOverLifetimeParticleAffectorDef::GetIntermediateColours() const
        {
            return m_intermediateColours;
        }
	}
}