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
    namespace
    {
        const char k_intermediateColours[] = "IntermediateColours";
        const char k_targetColourProperty[] = "TargetColourProperty";
        const char k_colourProperty[] = "ColourProperty";
        const char k_timeProperty[] = "TimeProperty";
        
        const char k_interpolationKey[] = "Interpolation";
        const char k_defaultInterpolation[] = "Linear";
    }
    
    CS_DEFINE_NAMEDTYPE(ColourOverLifetimeParticleAffectorDef);
    //----------------------------------------------------------------
    //----------------------------------------------------------------
    ColourOverLifetimeParticleAffectorDef::IntermediateColour::IntermediateColour(IntermediateColour&& in_toMove)
    {
        m_colourProperty = std::move(in_toMove.m_colourProperty);
        m_timeProperty = std::move(in_toMove.m_timeProperty);
    }
    //----------------------------------------------------------------
    //----------------------------------------------------------------
    ColourOverLifetimeParticleAffectorDef::IntermediateColour& ColourOverLifetimeParticleAffectorDef::IntermediateColour::operator=(IntermediateColour&& in_toMove)
    {
        m_colourProperty = std::move(in_toMove.m_colourProperty);
        m_timeProperty = std::move(in_toMove.m_timeProperty);

        return *this;
    }
    //----------------------------------------------------------------
    //----------------------------------------------------------------
    ColourOverLifetimeParticleAffectorDef::ColourOverLifetimeParticleAffectorDef(ParticlePropertyUPtr<Colour> in_targetColour, std::vector<IntermediateColour> in_intermediateColours,
        const std::function<f32(f32)>& in_interpolation)
        : m_targetColourProperty(std::move(in_targetColour)), m_intermediateColours(std::move(in_intermediateColours)), m_interpolation(in_interpolation)
    {
    }
    //----------------------------------------------------------------
    //----------------------------------------------------------------
    ColourOverLifetimeParticleAffectorDef::ColourOverLifetimeParticleAffectorDef(const Json::Value& in_paramsJson, const LoadedDelegate& in_asyncDelegate)
    {
        // Target Colour
        const auto& jsonValue = in_paramsJson.get(k_targetColourProperty, Json::nullValue);
        CS_ASSERT(jsonValue.isNull() == false, "No target colour property provided.");
        m_targetColourProperty = ParticlePropertyFactory::CreateProperty<Colour>(jsonValue);
        
        // Intermediate Colours
        const auto& jsonArray = in_paramsJson.get(k_intermediateColours, Json::nullValue);
        m_intermediateColours.reserve(jsonArray.size());
        for(const auto& jsonIntermediateColour : jsonArray)
        {
            m_intermediateColours.push_back(IntermediateColour());
            
            const auto& jsonColour = jsonIntermediateColour.get(k_colourProperty, Json::nullValue);
            m_intermediateColours.back().m_colourProperty = ParticlePropertyFactory::CreateProperty<Colour>(jsonColour);
            
            const auto& jsonTime = jsonIntermediateColour.get(k_timeProperty, Json::nullValue);
            m_intermediateColours.back().m_timeProperty = ParticlePropertyFactory::CreateProperty<f32>(jsonTime);
        }
        
        // Curve
        auto interpolationName = in_paramsJson.get(k_interpolationKey, k_defaultInterpolation).asString();
        m_interpolation = Interpolate::GetInterpolateFunction(interpolationName);

        // Call the loaded delegate if required.
        if (in_asyncDelegate != nullptr)
        {
            in_asyncDelegate(this);
        }
    }
    //----------------------------------------------------------------
    //----------------------------------------------------------------
    bool ColourOverLifetimeParticleAffectorDef::IsA(InterfaceIDType in_interfaceId) const
    {
        return (ParticleAffectorDef::InterfaceID == in_interfaceId || ColourOverLifetimeParticleAffectorDef::InterfaceID == in_interfaceId);
    }
    //----------------------------------------------------------------
    //----------------------------------------------------------------
    ParticleAffectorUPtr ColourOverLifetimeParticleAffectorDef::CreateInstance(dynamic_array<Particle>* in_particleArray) const
    {
        return ParticleAffectorUPtr(new ColourOverLifetimeParticleAffector(this, in_particleArray));
    }
    //----------------------------------------------------------------
    //----------------------------------------------------------------
    const ParticleProperty<Colour>* ColourOverLifetimeParticleAffectorDef::GetTargetColourProperty() const
    {
        return m_targetColourProperty.get();
    }
    //----------------------------------------------------------------
    //----------------------------------------------------------------
    const std::function<f32(f32)>& ColourOverLifetimeParticleAffectorDef::GetInterpolation() const
    {
        return m_interpolation;
    }
    //----------------------------------------------------------------
    //----------------------------------------------------------------
    const std::vector<ColourOverLifetimeParticleAffectorDef::IntermediateColour>& ColourOverLifetimeParticleAffectorDef::GetIntermediateColours() const
    {
        return m_intermediateColours;
    }
}