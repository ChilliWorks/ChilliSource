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
#include <ChilliSource/Rendering/Particles/Effectors/ParticleEffector.h>

namespace moFlo
{
	namespace Rendering
	{
        DEFINE_NAMED_INTERFACE(CParticleComponent);
		//=====================================================
		/// Particle Component
		///
		/// Entire particle system
		//=====================================================
		//-----------------------------------------------------
		/// Constructor 
		//-----------------------------------------------------
		CParticleComponent::CParticleComponent(): mpOwningSystem(NULL), mfUpdateScaleFactor(1.0f), mbEmittersFinished(false)
		{
            SetCullingEnabled(false);
		}
		//----------------------------------------------------------
		/// Is A
		//----------------------------------------------------------
		bool CParticleComponent::IsA(moFlo::Core::InterfaceIDType inInterfaceID) const
		{
			return (inInterfaceID == CParticleComponent::InterfaceID) || 
            (inInterfaceID == IRenderComponent::InterfaceID);
		}
		//-------------------------------------------------
		/// Set Owning System
		//-------------------------------------------------
		void CParticleComponent::SetOwningSystem(CParticleSystem* inpSystem)
		{
			mpOwningSystem = inpSystem;
		}
		//-------------------------------------------------
		/// Remove From World System
		//-------------------------------------------------
		void CParticleComponent::RemoveFromWorldSystem()
		{
			mpOwningSystem->RemoveParticleComponent(this);
		}
		//---------------------------------------------------
		/// Add Emitter
		//---------------------------------------------------
		void CParticleComponent::AddEmitter(CParticleEmitter* inpEmitter)
		{
            if(inpEmitter)
            {
                mEmitters.push_back(inpEmitter);
            }
		}
		//---------------------------------------------------
		/// Get number of emitters in this system
		//---------------------------------------------------
		u32 CParticleComponent::GetNumEmitters() const
        {
			return mEmitters.size();
		}
		//---------------------------------------------------
		/// Returns the emitter at the given index or NULL
		//---------------------------------------------------
		CParticleEmitter* CParticleComponent::GetEmitter(u32 inudwIndex){
			
			if (inudwIndex < mEmitters.size())
				return mEmitters[inudwIndex];
				
			return NULL;
		}
        //---------------------------------------------------
		/// Add Effector
		//---------------------------------------------------
		void CParticleComponent::AddEffector(IParticleEffector* inpEffector)
		{
            if(!inpEffector)
            {
                return;
            }
            
			mEffectors.push_back(inpEffector);
			
            for(DYNAMIC_ARRAY<CParticleEmitter*>::iterator it = mEmitters.begin(); it != mEmitters.end(); ++it)
			{
				(*it)->AddEffector(inpEffector);
			}
		}
		//-------------------------------------------------
		/// Update
		//-------------------------------------------------
		void CParticleComponent::Update(f32 infDt)
		{
            bool bEmittingFinished = true;
            
			for(DYNAMIC_ARRAY<CParticleEmitter*>::iterator it = mEmitters.begin(); it != mEmitters.end(); ++it)
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
        void CParticleComponent::Render(IRenderSystem* inpRenderSystem, CCameraComponent* inpCam, ShaderPass ineShaderPass)
        {
            if (ineShaderPass == ShaderPass::k_ambient)
            {
                for(DYNAMIC_ARRAY<CParticleEmitter*>::iterator it = mEmitters.begin(); it != mEmitters.end(); ++it)
                {
                    (*it)->Render(inpRenderSystem, inpCam);
                }
            }
        }
        //---------------------------------------------------
        /// Start Emitting
        //---------------------------------------------------
        void CParticleComponent::StartEmitting()
        {
            for(DYNAMIC_ARRAY<CParticleEmitter*>::iterator it = mEmitters.begin(); it != mEmitters.end(); ++it)
			{
				(*it)->StartEmitting();
			}
        }
		//---------------------------------------------------
		/// Emit Once
		//---------------------------------------------------
        void CParticleComponent::EmitBurst()
        {
            for(DYNAMIC_ARRAY<CParticleEmitter*>::iterator it = mEmitters.begin(); it != mEmitters.end(); ++it)
			{
				(*it)->EmitBurst();
			}
        }
        //---------------------------------------------------
        /// Stop Emitting
        //---------------------------------------------------
        void CParticleComponent::StopEmitting()
        {
            for(DYNAMIC_ARRAY<CParticleEmitter*>::iterator it = mEmitters.begin(); it != mEmitters.end(); ++it)
			{
				(*it)->StopEmitting();
			}
        }
        //---------------------------------------------------
        /// Set Update Scale Factor
        //---------------------------------------------------
        void CParticleComponent::SetUpdateScaleFactor(f32 infScale)
        {
            mfUpdateScaleFactor = infScale;
        }
        //-----------------------------------------------------
		/// Destructor 
		//-----------------------------------------------------
		CParticleComponent::~CParticleComponent()
		{
            for(DYNAMIC_ARRAY<CParticleEmitter*>::iterator it = mEmitters.begin(); it != mEmitters.end(); ++it)
			{
				SAFE_DELETE(*it);
			}
			
			for(DYNAMIC_ARRAY<IParticleEffector*>::iterator itEffector = mEffectors.begin(); itEffector != mEffectors.end(); ++itEffector)
            {
                SAFE_DELETE(*itEffector);
            }
			
            RemoveFromWorldSystem();
		}
	}
}