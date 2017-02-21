//
//  ParticleAffectorDefFactory.cpp
//  ChilliSource
//  Created by Ian Copland on 08/11/2014.
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

#include <ChilliSource/Rendering/Particle/Affector/ParticleAffectorDefFactory.h>

#include <ChilliSource/Rendering/Particle/Affector/AccelerationParticleAffectorDef.h>
#include <ChilliSource/Rendering/Particle/Affector/AngularAccelerationParticleAffectorDef.h>
#include <ChilliSource/Rendering/Particle/Affector/ColourOverLifetimeParticleAffectorDef.h>
#include <ChilliSource/Rendering/Particle/Affector/ScaleOverLifetimeParticleAffectorDef.h>

namespace ChilliSource
{
    CS_DEFINE_NAMEDTYPE(ParticleAffectorDefFactory);
    //-----------------------------------------------------------------
    //-----------------------------------------------------------------
    ParticleAffectorDefFactoryUPtr ParticleAffectorDefFactory::Create()
    {
        return ParticleAffectorDefFactoryUPtr(new ParticleAffectorDefFactory());
    }
    //-----------------------------------------------------------------
    //-----------------------------------------------------------------
    bool ParticleAffectorDefFactory::IsA(InterfaceIDType in_interfaceId) const
    {
        return (ParticleAffectorDefFactory::InterfaceID == in_interfaceId);
    }
    //-----------------------------------------------------------------
    //-----------------------------------------------------------------
    void ParticleAffectorDefFactory::RegisterDefaults()
    {
        Register<AccelerationParticleAffectorDef>("Acceleration");
        Register<AngularAccelerationParticleAffectorDef>("AngularAcceleration");
        Register<ColourOverLifetimeParticleAffectorDef>("ColourOverLifetime");
        Register<ScaleOverLifetimeParticleAffectorDef>("ScaleOverLifetime");
    }
}
