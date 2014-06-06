//
//  ParticleEmitterFactory.cpp
//  Chilli Source
//  Created by S Downie on 11/04/2011.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2011 Tag Games Limited
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

#include <ChilliSource/Rendering/Particles/Emitters/ParticleEmitterFactory.h>

#include <ChilliSource/Core/Delegate/MakeDelegate.h>
#include <ChilliSource/Rendering/Particles/Emitters/ConeParticleEmitter.h>
#include <ChilliSource/Rendering/Particles/Emitters/PointParticleEmitter.h>
#include <ChilliSource/Rendering/Particles/Emitters/RingParticleEmitter.h>

namespace ChilliSource
{
    namespace Rendering
    {
        CS_DEFINE_NAMEDTYPE(ParticleEmitterFactory);
        
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        ParticleEmitterFactoryUPtr ParticleEmitterFactory::Create()
        {
            return ParticleEmitterFactoryUPtr(new ParticleEmitterFactory());
        }
        //------------------------------------------------------------------
        //------------------------------------------------------------------
        bool ParticleEmitterFactory::IsA(Core::InterfaceIDType in_interfaceId) const
        {
            return in_interfaceId == ParticleEmitterFactory::InterfaceID;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void ParticleEmitterFactory::OnInit()
        {
            AddCreator("Point", Core::MakeDelegate(&PointParticleEmitter::Create));
            AddCreator("Ring", Core::MakeDelegate(&RingParticleEmitter::Create));
            AddCreator("Cone", Core::MakeDelegate(&ConeParticleEmitter::Create));
        }
        //------------------------------------------------------------------
        //------------------------------------------------------------------
        void ParticleEmitterFactory::AddCreator(const std::string& in_type, const EmitterCreateDelegate& in_delegate)
        {
            m_creators.insert(std::make_pair(in_type, in_delegate));
        }
        //------------------------------------------------------------------
        //------------------------------------------------------------------
        void ParticleEmitterFactory::RemoveAllCreators()
        {
            m_creators.clear();
        }
        //------------------------------------------------------------------
        //-------------------------------------------------------------------
        ParticleEmitterUPtr ParticleEmitterFactory::CreateParticleEmitter(const std::string& in_type, const Core::ParamDictionary& in_properties, const MaterialCSPtr& in_material, ParticleComponent* in_owner) const
        {
            auto it = m_creators.find(in_type);
            
            if(it != m_creators.end())
            {
                return (it->second)(in_properties, in_material, in_owner);
            }
            
            CS_LOG_ERROR("Cannot create particle emitter of type: " + in_type);
            return nullptr;
        }
        //-------------------------------------------------------------------
        //-------------------------------------------------------------------
        void ParticleEmitterFactory::OnDestroy()
        {
            RemoveAllCreators();
        }
    }
}
