/*
 *  ParticleComponent.cpp
 *  moFloTest
 *
 *  Created by Scott Downie on 07/01/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Rendering/Particles/ParticleComponent.h>

#include <ChilliSource/Core/Base/FastDelegate.h>
#include <ChilliSource/Rendering/Particles/ParticleSystem.h>
#include <ChilliSource/Rendering/Particles/Effectors/ParticleEffector.h>

namespace ChilliSource
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
		CParticleComponent::CParticleComponent(): mpOwningSystem(nullptr), mfUpdateScaleFactor(1.0f), mbEmittersFinished(false)
		{
            SetCullingEnabled(false);
		}
		//----------------------------------------------------------
		/// Is A
		//----------------------------------------------------------
		bool CParticleComponent::IsA(ChilliSource::Core::InterfaceIDType inInterfaceID) const
		{
			return (inInterfaceID == CParticleComponent::InterfaceID) || 
            (inInterfaceID == RenderComponent::InterfaceID);
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
		/// Returns the emitter at the given index or nullptr
		//---------------------------------------------------
		CParticleEmitter* CParticleComponent::GetEmitter(u32 inudwIndex){
			
			if (inudwIndex < mEmitters.size())
				return mEmitters[inudwIndex];
				
			return nullptr;
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
			
            for(std::vector<CParticleEmitter*>::iterator it = mEmitters.begin(); it != mEmitters.end(); ++it)
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
            
			for(std::vector<CParticleEmitter*>::iterator it = mEmitters.begin(); it != mEmitters.end(); ++it)
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
        void CParticleComponent::Render(RenderSystem* inpRenderSystem, CameraComponent* inpCam, ShaderPass ineShaderPass)
        {
            if (ineShaderPass == ShaderPass::k_ambient)
            {
                for(std::vector<CParticleEmitter*>::iterator it = mEmitters.begin(); it != mEmitters.end(); ++it)
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
            for(std::vector<CParticleEmitter*>::iterator it = mEmitters.begin(); it != mEmitters.end(); ++it)
			{
				(*it)->StartEmitting();
			}
        }
		//---------------------------------------------------
		/// Emit Once
		//---------------------------------------------------
        void CParticleComponent::EmitBurst()
        {
            for(std::vector<CParticleEmitter*>::iterator it = mEmitters.begin(); it != mEmitters.end(); ++it)
			{
				(*it)->EmitBurst();
			}
        }
        //---------------------------------------------------
        /// Stop Emitting
        //---------------------------------------------------
        void CParticleComponent::StopEmitting()
        {
            for(std::vector<CParticleEmitter*>::iterator it = mEmitters.begin(); it != mEmitters.end(); ++it)
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
            for(std::vector<CParticleEmitter*>::iterator it = mEmitters.begin(); it != mEmitters.end(); ++it)
			{
				CS_SAFE_DELETE(*it);
			}
			
			for(std::vector<IParticleEffector*>::iterator itEffector = mEffectors.begin(); itEffector != mEffectors.end(); ++itEffector)
            {
                CS_SAFE_DELETE(*itEffector);
            }
			
            RemoveFromWorldSystem();
		}
	}
}