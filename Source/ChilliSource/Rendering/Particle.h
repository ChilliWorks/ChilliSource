//
//  Particles.h
//  Chilli Source
//  Created by Ian Copland on 07/07/2014.
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


#ifndef _CHILLISOURCE_RENDERING_PARTICLES_H_
#define _CHILLISOURCE_RENDERING_PARTICLES_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Rendering/Particle/CSParticleEffectProvider.h>
#include <ChilliSource/Rendering/Particle/Particle.h>
#include <ChilliSource/Rendering/Particle/ParticleDrawData.h>
#include <ChilliSource/Rendering/Particle/ParticleEffect.h>
#include <ChilliSource/Rendering/Particle/ParticleEffectComponent.h>
#include <ChilliSource/Rendering/Particle/Affector/ParticleAffector.h>
#include <ChilliSource/Rendering/Particle/Affector/ParticleAffectorDef.h>
#include <ChilliSource/Rendering/Particle/Affector/ParticleAffectorDefFactory.h>
#include <ChilliSource/Rendering/Particle/Drawable/BillboardParticleDrawable.h>
#include <ChilliSource/Rendering/Particle/Drawable/BillboardParticleDrawableDef.h>
#include <ChilliSource/Rendering/Particle/Drawable/ParticleDrawable.h>
#include <ChilliSource/Rendering/Particle/Drawable/ParticleDrawableDef.h>
#include <ChilliSource/Rendering/Particle/Drawable/ParticleDrawableDefFactory.h>
#include <ChilliSource/Rendering/Particle/Emitter/ParticleEmitter.h>
#include <ChilliSource/Rendering/Particle/Emitter/ParticleEmitterDef.h>
#include <ChilliSource/Rendering/Particle/Emitter/ParticleEmitterDefFactory.h>
#include <ChilliSource/Rendering/Particle/Emitter/SphereParticleEmitter.h>
#include <ChilliSource/Rendering/Particle/Emitter/SphereParticleEmitterDef.h>
#include <ChilliSource/Rendering/Particle/Property/ParticleProperty.h>
#include <ChilliSource/Rendering/Particle/Property/ParticlePropertyValueParser.h>
#include <ChilliSource/Rendering/Particle/Property/RandomParticleProperty.h>
#include <ChilliSource/Rendering/Particle/Property/StaticParticleProperty.h>

#endif