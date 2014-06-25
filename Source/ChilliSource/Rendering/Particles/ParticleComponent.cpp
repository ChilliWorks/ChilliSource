//
//  ParticleComponent.cpp
//  Chilli Source
//  Created by Scott Downie on 07/01/2011.
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

#include <ChilliSource/Rendering/Particles/ParticleComponent.h>

#include <ChilliSource/Rendering/Particles/ParticleSystem.h>
#include <ChilliSource/Rendering/Particles/Affectors/ParticleAffector.h>

namespace ChilliSource
{
	namespace Rendering
	{
        CS_DEFINE_NAMEDTYPE(ParticleComponent);
		//=====================================================
		/// Particle Component
		///
		/// Entire particle system
		//=====================================================
		//-----------------------------------------------------
		/// Constructor 
		//-----------------------------------------------------
		ParticleComponent::ParticleComponent(): mpOwningSystem(nullptr), mfUpdateScaleFactor(1.0f), mbEmittersFinished(false)
		{
            SetCullingEnabled(false);
		}
		//----------------------------------------------------------
		/// Is A
		//----------------------------------------------------------
		bool ParticleComponent::IsA(CSCore::InterfaceIDType inInterfaceID) const
		{
			return (inInterfaceID == ParticleComponent::InterfaceID) || 
            (inInterfaceID == RenderComponent::InterfaceID);
		}
		//-------------------------------------------------
		/// Set Owning System
		//-------------------------------------------------
		void ParticleComponent::SetOwningSystem(ParticleSystem* inpSystem)
		{
			mpOwningSystem = inpSystem;
		}
		//-------------------------------------------------
		/// Remove From World System
		//-------------------------------------------------
		void ParticleComponent::RemoveFromWorldSystem()
		{
			mpOwningSystem->RemoveParticleComponent(this);
		}
		//---------------------------------------------------
		/// Add Emitter
		//---------------------------------------------------
		void ParticleComponent::AddEmitter(ParticleEmitterUPtr inpEmitter)
		{
            if(inpEmitter)
            {
                mEmitters.push_back(std::move(inpEmitter));
            }
		}
		//---------------------------------------------------
		/// Get number of emitters in this system
		//---------------------------------------------------
		u32 ParticleComponent::GetNumEmitters() const
        {
			return mEmitters.size();
		}
		//---------------------------------------------------
		/// Returns the emitter at the given index or nullptr
		//---------------------------------------------------
		ParticleEmitter* ParticleComponent::GetEmitter(u32 inudwIndex)
        {
			if (inudwIndex < mEmitters.size())
				return mEmitters[inudwIndex].get();
				
			return nullptr;
		}
        //---------------------------------------------------
		/// Add Affector
		//---------------------------------------------------
		void ParticleComponent::AddAffector(ParticleAffectorUPtr inpAffector)
		{
            if(!inpAffector)
            {
                return;
            }
			
            for(std::vector<ParticleEmitterUPtr>::iterator it = mEmitters.begin(); it != mEmitters.end(); ++it)
			{
				(*it)->AddAffector(inpAffector.get());
			}
            
            mAffectors.push_back(std::move(inpAffector));
		}
		//-------------------------------------------------
		/// Update
		//-------------------------------------------------
		void ParticleComponent::Update(f32 infDt)
		{
            bool bEmittingFinished = true;
            
			for(std::vector<ParticleEmitterUPtr>::iterator it = mEmitters.begin(); it != mEmitters.end(); ++it)
			{
				(*it)->Update(infDt * mfUpdateScaleFactor);
                
                if((*it)->GetIsEmittingFinished() == false)
                {
                    bEmittingFinished = false;
                }
			}
            
            if(bEmittingFinished)
            {
                mbEmittersFinished = true;
            }
		}
        //-------------------------------------------------
        /// Render
        //-------------------------------------------------
        void ParticleComponent::Render(RenderSystem* inpRenderSystem, CameraComponent* inpCam, ShaderPass ineShaderPass)
        {
            if (ineShaderPass == ShaderPass::k_ambient)
            {
                for(std::vector<ParticleEmitterUPtr>::iterator it = mEmitters.begin(); it != mEmitters.end(); ++it)
                {
                    (*it)->Render(inpRenderSystem, inpCam);
                }
            }
        }
        //---------------------------------------------------
        /// Start Emitting
        //---------------------------------------------------
        void ParticleComponent::StartEmitting()
        {
            for(std::vector<ParticleEmitterUPtr>::iterator it = mEmitters.begin(); it != mEmitters.end(); ++it)
			{
				(*it)->StartEmitting();
			}
        }
		//---------------------------------------------------
		/// Emit Once
		//---------------------------------------------------
        void ParticleComponent::EmitBurst()
        {
            for(std::vector<ParticleEmitterUPtr>::iterator it = mEmitters.begin(); it != mEmitters.end(); ++it)
			{
				(*it)->EmitBurst();
			}
        }
        //---------------------------------------------------
        /// Stop Emitting
        //---------------------------------------------------
        void ParticleComponent::StopEmitting()
        {
            for(std::vector<ParticleEmitterUPtr>::iterator it = mEmitters.begin(); it != mEmitters.end(); ++it)
			{
				(*it)->StopEmitting();
			}
        }
        //---------------------------------------------------
        /// Set Update Scale Factor
        //---------------------------------------------------
        void ParticleComponent::SetUpdateScaleFactor(f32 infScale)
        {
            mfUpdateScaleFactor = infScale;
        }
        //-----------------------------------------------------
		/// Destructor 
		//-----------------------------------------------------
		ParticleComponent::~ParticleComponent()
		{
            RemoveFromWorldSystem();
		}
	}
}