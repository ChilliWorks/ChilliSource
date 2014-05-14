//
//  ParticleEffect.cpp
//  Chilli Source
//  Created by S Downie on 30/04/2014.
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

#include <ChilliSource/Rendering/Particles/ParticleEffect.h>

#include <ChilliSource/Core/Cryptographic/HashCRC32.h>

namespace ChilliSource
{
	namespace Rendering
	{
        CS_DEFINE_NAMEDTYPE(ParticleEffect);
        
        //---------------------------------------------------------------------
        //---------------------------------------------------------------------
        ParticleEffectUPtr ParticleEffect::Create()
        {
            return ParticleEffectUPtr(new ParticleEffect());
        }
		//---------------------------------------------------------------------
		//---------------------------------------------------------------------
		bool ParticleEffect::IsA(Core::InterfaceIDType in_interfaceId) const
		{
			return in_interfaceId == ParticleEffect::InterfaceID;
		}
        //---------------------------------------------------------------------
        //---------------------------------------------------------------------
        void ParticleEffect::Build(std::vector<Core::ParamDictionary>&& in_emitterDescs, std::vector<Core::ParamDictionary>&& in_affectorDescs, const MaterialCSPtr& in_material,
                                   const TextureAtlasCSPtr& in_atlas, std::string&& in_atlasId)
        {
            m_emitterDescs = std::move(in_emitterDescs);
            m_affectorDescs = std::move(in_affectorDescs);
            m_material = in_material;
            m_atlas = in_atlas;
            m_atlasId = std::move(in_atlasId);
        }
        //---------------------------------------------------------------------
        //---------------------------------------------------------------------
        const MaterialCSPtr& ParticleEffect::GetMaterial() const
        {
            return m_material;
        }
        //---------------------------------------------------------------------
        //---------------------------------------------------------------------
        const TextureAtlasCSPtr& ParticleEffect::GetAtlas() const
        {
            return m_atlas;
        }
        //---------------------------------------------------------------------
        //---------------------------------------------------------------------
        const std::string& ParticleEffect::GetAtlasId() const
        {
            return m_atlasId;
        }
        //---------------------------------------------------------------------
        //---------------------------------------------------------------------
        u32 ParticleEffect::GetNumEmitters() const
        {
            return m_emitterDescs.size();
        }
        //---------------------------------------------------------------------
        //---------------------------------------------------------------------
        const Core::ParamDictionary& ParticleEffect::GetEmitterDesc(u32 in_index) const
        {
            CS_ASSERT(in_index < GetNumEmitters(), "Emitter index out of bounds");
            
            return m_emitterDescs[in_index];
        }
        //---------------------------------------------------------------------
        //---------------------------------------------------------------------
        u32 ParticleEffect::GetNumAffectors() const
        {
            return m_affectorDescs.size();
        }
        //---------------------------------------------------------------------
        //---------------------------------------------------------------------
        const Core::ParamDictionary& ParticleEffect::GetAffectorDesc(u32 in_index) const
        {
            CS_ASSERT(in_index < GetNumAffectors(), "Affector index out of bounds");
            
            return m_affectorDescs[in_index];
        }
	}
}
