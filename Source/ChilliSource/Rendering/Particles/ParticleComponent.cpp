/*
 *  ParticleComponent.cpp
 *  moFloTest
 *
 *  Created by Scott Downie on 07/01/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

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
		bool ParticleComponent::IsA(ChilliSource::Core::InterfaceIDType inInterfaceID) const
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