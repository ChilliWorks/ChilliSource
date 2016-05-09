//
//  ColourOverLifetimeParticleAffectorDef.h
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

#ifndef _CHILLISOURCE_RENDERING_PARTICLE_AFFECTOR_COLOUROVERLIFETIMEPARTICLEAFFECTORDEF_H_
#define _CHILLISOURCE_RENDERING_PARTICLE_AFFECTOR_COLOUROVERLIFETIMEPARTICLEAFFECTORDEF_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/Colour.h>
#include <ChilliSource/Rendering/Particle/Affector/ParticleAffectorDef.h>
#include <ChilliSource/Rendering/Particle/Property/ParticleProperty.h>
#include <ChilliSource/Core/Math/Interpolate.h>

#include <json/json.h>

namespace ChilliSource
{
    //------------------------------------------------------------------------------
    /// The definition for a Colour Over Lifetime particle affector. This
    /// describes a particle effector which will change the colour of a particle 
    /// from its initial colour to the defined colour over the course of the 
    /// particles life.
    ///
    /// A colour over lifetime particle affector contains the following params:
    ///
    /// "TargetColourProperty": The property describing the target colour.
    ///
    /// "IntermediateColours": The list of intermediate colours to go through
    /// before interpolating to the target colour. Each intermediate colour
    /// has the following properties:
    ///
    ///     "ColourProperty": The property describing the intermediate colour.
    ///
    ///     "TimeProperty": The property defining the target time for that
    ///     colour.
    ///
    /// "Interpolation": The interpolation curve used to transition from one
    /// colour to another.
    ///
    /// @author Nicolas Tanda
    //------------------------------------------------------------------------------
    class ColourOverLifetimeParticleAffectorDef final : public ParticleAffectorDef
    {
    public:
        CS_DECLARE_NAMEDTYPE(ColourOverLifetimeParticleAffectorDef);
        //------------------------------------------------------------------------------
        /// Definition of an intermediate colour of a single particle.
        ///
        /// @author Nicolas Tanda
        //------------------------------------------------------------------------------
        struct IntermediateColour final
        {
            ParticlePropertyUPtr<Colour> m_colourProperty;
            ParticlePropertyUPtr<f32> m_timeProperty;

            //------------------------------------------------------------------------------
            /// As the move constructor is explicit, the constructor also needs to be.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            IntermediateColour() = default;
            //------------------------------------------------------------------------------
            /// Visual C++ 12 doesn't support implicit move constructors so for now they 
            /// need to be manually specified.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            IntermediateColour(IntermediateColour&& in_toMove);
            //------------------------------------------------------------------------------
            /// Visual C++ 12 doesn't support implicit move assignment operators so for now
            /// they need to be manually specified.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            IntermediateColour& operator=(IntermediateColour&& in_toMove);
        };
        //------------------------------------------------------------------------------
        /// Constructor.
        ///
        /// @author Ian Copland
        ///
        /// @param in_targetColour - The property which describes the colour to change 
        /// to.
        /// @param in_intermediateColours - [Optional] Intermediate colours to transition
        /// to between the initial and target colours. 
        /// @param in_interpolation - [Optional] The function used for interpolation
        /// between colours. Defaults to linear interpolation.
        //------------------------------------------------------------------------------
        ColourOverLifetimeParticleAffectorDef(ParticlePropertyUPtr<Colour> in_targetColour, std::vector<IntermediateColour> in_intermediateColours = std::vector<IntermediateColour>(), 
            const std::function<f32(f32)>& in_interpolation = Interpolate::Linear);
        //------------------------------------------------------------------------------
        /// Constructor. Loads the params for the affector def from the given param 
        /// dictionary. If the async delegate is not null, then any resource loading 
        /// will occur as a background task. Once complete the delegate will be called.
        /// The parameters read from json are described in the class documentation.
        ///
        /// @author Ian Copland
        ///
        /// @param in_paramsJson - A json object describing the parameters for the 
        /// particle emitter def.
        /// @param in_asyncDelegate - The loaded delegate. If this is supplied any
        /// resources will be loaded as a background task. Once complete, this delegate 
        /// will be called.
        //------------------------------------------------------------------------------
        ColourOverLifetimeParticleAffectorDef(const Json::Value& in_paramsJson, const LoadedDelegate& in_asyncDelegate = nullptr);
        //------------------------------------------------------------------------------
        /// Allows querying of whether or not this implements the interface described 
        /// by the given Id.
        ///
        /// @author Ian Copland
        ///
        /// @param in_interfaceId - The interface Id.
        ///
        /// @return Whether or not the interface is implemented.
        //------------------------------------------------------------------------------
        bool IsA(InterfaceIDType in_interfaceId) const override;
        //------------------------------------------------------------------------------
        /// Creates an instance of the particle affector described by this.
        ///
        /// @author Ian Copland.
        ///
        /// @param in_particleArray - The particle array.
        ///
        /// @return the instance.
        //------------------------------------------------------------------------------
        ParticleAffectorUPtr CreateInstance(dynamic_array<Particle>* in_particleArray) const override;
        //------------------------------------------------------------------------------
        /// @author Ian Copland
        ///
        /// @return A property describing the target colour.
        //------------------------------------------------------------------------------
        const ParticleProperty<Colour>* GetTargetColourProperty() const;
        //------------------------------------------------------------------------------
        /// @author Ian Copland
        ///
        /// @return The interpolation function used to transition between colours.
        //------------------------------------------------------------------------------
        const std::function<f32(f32)>& GetInterpolation() const;
        //------------------------------------------------------------------------------
        /// @author Nicolas Tanda
        ///
        /// @return An array of intermediate colours
        //------------------------------------------------------------------------------
        const std::vector<IntermediateColour>& GetIntermediateColours() const;
        //------------------------------------------------------------------------------
        /// Destructor
        ///
        /// @author Ian Copland.
        //------------------------------------------------------------------------------
        virtual ~ColourOverLifetimeParticleAffectorDef() {}
        
    private:
        std::function<f32(f32)> m_interpolation;
        ParticlePropertyUPtr<Colour> m_targetColourProperty;
        
        std::vector<IntermediateColour> m_intermediateColours;
    };
}

#endif