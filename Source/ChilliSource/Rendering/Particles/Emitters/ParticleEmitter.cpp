//
//  ParticleEmitter.cpp
//  moFloTest
//
//  Created by Scott Downie on 07/04/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#include <ChilliSource/Rendering/Particles/Emitters/ParticleEmitter.h>
#include <ChilliSource/Rendering/Particles/Effectors/ParticleEffector.h>
#include <ChilliSource/Rendering/Particles/ParticleComponent.h>
#include <ChilliSource/Rendering/Base/RenderSystem.h>
#include <ChilliSource/Rendering/Sprite/DynamicSpriteBatcher.h>
#include <ChilliSource/Rendering/Camera/CameraComponent.h>

#include <ChilliSource/Core/Entity/Entity.h>
#include <ChilliSource/Core/String/StringParser.h>
#include <ChilliSource/Core/Math/MathUtils.h>

namespace ChilliSource
{
    namespace Rendering
    {
        //-------------------------------------------------------------
        /// Constructor
        ///
        /// @param Param Dictionary
        //-------------------------------------------------------------
        ParticleEmitter::ParticleEmitter(const Core::ParamDictionary& inParams, const MaterialSPtr &inpMaterial, ParticleComponent* inpComponent)
        : mudwMaxNumParticles(100), mudwMaxNumParticlesPerEmission(1), mfEmissionFreq(0.5f), mfCurrentTime(0.0f), mfLastEmissionTime(0.0f), mfTimeToLive(1.0f), mvInitialScale(1.0f, 1.0f), mbShouldLoop(true)
        ,mfEnergyLoss(1.0f/mfTimeToLive), mpOwningComponent(inpComponent), mbIsEmitting(true), mudwNumUsed(0), mpMaterial(inpMaterial),msParticleUVs(Core::Vector2::ZERO, Core::Vector2(1.0f,1.0f))
		,mudwBurstCounter(0), mbIsEmittingFinished(false)
        ,mbIsGlobalSpace(true)
        {
            std::string strTemp;
            
            //Frequency
            if(inParams.TryGetValue("Frequency", strTemp))
            {
                mfEmissionFreq = Core::ParseF32(strTemp);
                CS_ASSERT(mfEmissionFreq > 0.0f, "Frequency cannot be zero or less");
                
                mfCurrentTime = mfEmissionFreq;
            }
            //Looping
            if(inParams.TryGetValue("IsLooping", strTemp))
            {
                mbShouldLoop = Core::ParseBool(strTemp);
            }
            //Lifetime in seconds
            if(inParams.TryGetValue("Lifetime", strTemp))
            {
                f32 fTimeToLive = Core::ParseF32(strTemp);
                SetLifetime(fTimeToLive);
            }
            //Colour
            if(inParams.TryGetValue("InitColour", strTemp))
            {
                mInitialColour = Core::ParseColour(strTemp);
            }
            //Velocity
            if(inParams.TryGetValue("InitVelocity", strTemp))
            {
                mfInitialVelocity = Core::ParseF32(strTemp);
                mfMinInitialVelocity = mfInitialVelocity;
            }
            //Min Init Velocity
            if(inParams.TryGetValue("MinInitVelocity", strTemp))
            {
                mfMinInitialVelocity = Core::ParseF32(strTemp);
            }
            //Scale
            if(inParams.TryGetValue("InitScale", strTemp))
            {
                mvInitialScale = Core::ParseVector2(strTemp);
            }
            //Num per emission
            if(inParams.TryGetValue("ParticlesPerEmit", strTemp))
            {
                mudwMaxNumParticlesPerEmission = Core::ParseU32(strTemp);
            }
            // Global space
            if(inParams.TryGetValue("GlobalSpace", strTemp))
            {
                mbIsGlobalSpace = Core::ParseBool(strTemp);
            }
            
			mudwMaxNumParticles = (u32)(std::ceil(mfTimeToLive / mfEmissionFreq) * mudwMaxNumParticlesPerEmission);
            
            // Allocate all the memory upfront
            m_particles = new Particle[mudwMaxNumParticles];
            
            // Initialise Energy array to 0
            for(u32 i=0; i<mudwMaxNumParticles; ++i)
            {
                m_particles[i].m_energy = 0.0f;
                m_particles[i].m_angularRotation = 0.0f;
            }
        }
        //-----------------------------------------------------
        /// Update
        ///
        /// Update all the active particles with respect to
        /// time
        ///
        /// @param Time between frames
        //-----------------------------------------------------
        void ParticleEmitter::Update(f32 infDT)
        {
            mfCurrentTime += infDT;
            
            const Core::Vector3& vCurrentPos = mpOwningComponent->GetEntity()->GetTransform().GetWorldPosition();
            const f32 kfTimeSinceLastEmission = Core::MathUtils::Min(mfCurrentTime - mfLastEmissionTime, mfTimeToLive);
            u32 udwNumEmits = std::ceil(kfTimeSinceLastEmission/mfEmissionFreq);
            
            //If we force a burst emit then we override the frequency
            if(mudwBurstCounter > 0)
            {
                udwNumEmits = 1;
            }
            
            if(mudwNumUsed == 0)
            {
                mvLastEmissionPos = vCurrentPos;
            }
            
            u32 udwNumParticleToEmit = mudwMaxNumParticlesPerEmission * udwNumEmits;
            u32 udwNum_particlesEmittedThisStep = 0;
            f32 fEmissionStep = 0.0f;
            bool bParticlesActive = false;
            
            for(u32 i=0; i<mudwMaxNumParticles; ++i)
            {
                // allow active particle to die if time elapsed so it can be recycled
                if(m_particles[i].m_energy > 0.0f)
                {
                    UpdateParticle(m_particles[i], infDT);
                    mudwNumUsed = (m_particles[i].m_energy <= 0.0f) ? mudwNumUsed-1 : mudwNumUsed;
                }
                
                //If time to emit and we have some left in our quota...or we are a looping emitter then we need to emit
                bool bEmit = (mbIsEmitting && (mudwNumUsed < mudwMaxNumParticles) && udwNumParticleToEmit > 0 && m_particles[i].m_energy <= 0.0f);
                if(bEmit)
                {
                    mudwNumUsed++;
                    udwNumParticleToEmit--;
                    udwNum_particlesEmittedThisStep++;
                    
                    f32 fLerpFactor = fEmissionStep/kfTimeSinceLastEmission;
                    Core::Vector3 vPosition = Core::MathUtils::Lerp(fLerpFactor, mvLastEmissionPos, vCurrentPos) - vCurrentPos;
                    if(udwNum_particlesEmittedThisStep >= mudwMaxNumParticlesPerEmission)
                    {
                        fEmissionStep += mfEmissionFreq;
                        udwNum_particlesEmittedThisStep = 0;
                    }
                    Core::Quaternion qOrientation;
                    Core::Vector3 vScale(Core::Vector3::ONE);
                    
                    if(mbIsGlobalSpace)
                    {
                        vPosition = vCurrentPos + vPosition;
                        qOrientation = mpOwningComponent->GetEntity()->GetTransform().GetWorldOrientation();
                        vScale = mpOwningComponent->GetEntity()->GetTransform().GetWorldScale();
                    }
                    
                    m_particles[i].m_energy = 1.0f;
                    m_particles[i].m_colour = mInitialColour;
                    m_particles[i].m_translation = vPosition;
                    m_particles[i].m_scale = vScale;
                    m_particles[i].m_angularRotation = 0.0f;
                    
                    //We will emit a particle and pass it through the effector
                    Emit(m_particles[i]);
                    
                    // We need to rotate our velocity by our emmiters orientation if in global space
                    if(mbIsGlobalSpace)
                    {
                        m_particles[i].m_velocity = qOrientation * m_particles[i].m_velocity;
                    }
                    
                    for(std::vector<ParticleEffector*>::iterator itEffector = mEffectors.begin(); itEffector != mEffectors.end(); ++itEffector)
                    {
                        (*itEffector)->Init(m_particles[i]);
                    }
                    
                    UpdateParticle(m_particles[i], kfTimeSinceLastEmission - fEmissionStep);
                    mudwNumUsed = (m_particles[i].m_energy <= 0.0f) ? mudwNumUsed-1 : mudwNumUsed;
                }
                // test particles for activity
                if(m_particles[i].m_energy > 0.0f)
                {
                    bParticlesActive = true;
                }
            }
            
            if(udwNumEmits > 0)
            {
                mvLastEmissionPos = vCurrentPos;
                mfLastEmissionTime = mfCurrentTime;
            }
            
            if(mudwBurstCounter > 0)
            {
                mudwBurstCounter--;
                
                if(mudwBurstCounter == 0)
                {
                    mudwNumUsed = 0;
                    mbIsEmitting = false;
                }
            }
            
            //If we have used our quota and we are not looping then stop emitting and reset the system
            if(mudwNumUsed >= mudwMaxNumParticles)
            {
                if(mbShouldLoop == false)
                {
                    mbIsEmitting = false;
                }
                else
                {
                    mudwNumUsed = 0;
                }
            }
            
            //There are no more alive particles and we are no longer emitting
            if(!bParticlesActive && !mbIsEmitting)
            {
                mbIsEmittingFinished = true;
            }
        }
        //-----------------------------------------------------
        /// Update Particle
        ///
        /// Update an indivdual particle with the effectors
        ///
        /// @param Particle array index
        /// @param DT
        //-----------------------------------------------------
        void ParticleEmitter::UpdateParticle(Particle& in_particle, f32 infDT)
        {
            //Apply the effector to each particle
            for(std::vector<ParticleEffector*>::iterator itEffector = mEffectors.begin(); itEffector != mEffectors.end(); ++itEffector)
            {
                if(in_particle.m_energy <= (*itEffector)->GetActiveEnergyLevel())
                {
                    (*itEffector)->Apply(in_particle, infDT);
                }
            }
            
            in_particle.m_energy -= (mfEnergyLoss * infDT);
            in_particle.m_translation += (in_particle.m_velocity * infDT);
        }
        //-----------------------------------------------------
        /// Render
        ///
        /// Render the active particles based on their
        /// renderable type
        ///
        /// @param Render system
        //-----------------------------------------------------
        void ParticleEmitter::Render(RenderSystem* inpRenderSystem, CameraComponent* inpCam)
        {
            SpriteComponent::SpriteData sData;
            // Get world matrix
            const Core::Matrix4x4 & matTrans = mpOwningComponent->GetEntity()->GetTransform().GetWorldTransform();
            
            // Get quaternion to particle space
            Core::Quaternion qParticleRot = Core::Quaternion(matTrans).Conjugate();
            
            const Core::Matrix4x4 & matCamWorld = inpCam->GetEntity()->GetTransform().GetWorldTransform();
            // Get cameras up and right vectors in particle space
            
            Core::Vector3 vRight = matCamWorld.Right();
            Core::Vector3 vUp = matCamWorld.Up();
            Core::Vector3 vForward = matCamWorld.Forward();
            
            const Core::Matrix4x4 * pTransform = nullptr;
            if(mbIsGlobalSpace == false)
            {
                pTransform = &matTrans;
                vRight = qParticleRot * vRight;
                vUp = qParticleRot * vUp;
                vForward = qParticleRot * vForward;
            }

            for(u32 i=0; i<mudwMaxNumParticles; ++i)
            {
                if(m_particles[i].m_energy > 0.0f && m_particles[i].m_colour.a > 0.0f)
                {
                    // Rotate per particle
                    Core::Quaternion qRot(vForward, m_particles[i].m_angularRotation);

                    UpdateSpriteData(m_particles[i].m_translation, m_particles[i].m_colour, sData, qRot * vRight, qRot * vUp, m_particles[i].m_scale);
                    
                    if (sData.pMaterial->GetActiveShaderProgram() != nullptr)
                    {
                        inpRenderSystem->GetDynamicSpriteBatchPtr()->Render(inpRenderSystem, sData, pTransform);
                    }
                }
            }
        }
        //---------------------------------------------------
        /// Start Emitting
        ///
        /// Particle emitters will resume emitting
        //---------------------------------------------------
        void ParticleEmitter::StartEmitting()
        {
            mbIsEmitting = true;
        }
        //---------------------------------------------------
        /// Emit Once
        ///
        /// Particle emitters will emit particles once
        //---------------------------------------------------
        void ParticleEmitter::EmitBurst()
        {
			StartEmitting();
			mudwBurstCounter++;
        }
        //---------------------------------------------------
        /// Stop Emitting
        ///
        /// No new particles will be emitted once the current
        /// ones die
        //---------------------------------------------------
        void ParticleEmitter::StopEmitting()
        {
            mbIsEmitting = false;
        }
        //-----------------------------------------------------
        /// Add Effector
        ///
        /// Add a new effector to apply to the particles
        ///
        /// @param Particle effector
        //-----------------------------------------------------
        void ParticleEmitter::AddEffector(ParticleEffector* inpEffector)
        {
            mEffectors.push_back(inpEffector);
        }
        //-----------------------------------------------------
        /// Remove Effector
        ///
        /// Remove the effector so it no longer applies to
        /// the system
        ///
        /// @param Particle effector
        //-----------------------------------------------------
        void ParticleEmitter::RemoveEffector(ParticleEffector* inpEffector)
        {
            for(std::vector<ParticleEffector*>::iterator itEffector = mEffectors.begin(); itEffector != mEffectors.end(); ++itEffector)
            {
                if(*itEffector == inpEffector)
                {
                    CS_SAFEDELETE(*itEffector);
                    mEffectors.erase(itEffector);
                    return;
                }
            }
        }
        //-----------------------------------------------------
        /// Set Velocity
        ///
        /// Set the initial velocity of the particles leaving
        /// the emitter
        ///
        /// @param Velocity vector
        //-----------------------------------------------------
        void ParticleEmitter::SetVelocity(f32 infVelocity)
        {
            mfInitialVelocity = infVelocity;
        }
        //-----------------------------------------------------
        /// Set Colour
        ///
        /// Set the initial colour of the particles
        ///
        /// @param Colour
        //-----------------------------------------------------
        void ParticleEmitter::SetColour(const Core::Colour inColour)
        {
            mInitialColour = inColour;
        }
        //-----------------------------------------------------
        /// Set Number of Particles Per Emission
        ///
        /// Sets the number of particles that are emitted
        /// at any given time. This is a maximum if there are
        /// no particles available no new ones will be created
        ///
        /// @param Number of particles
        //-----------------------------------------------------
        void ParticleEmitter::SetNumParticlesPerEmission(u32 inudwNumParticles)
        {
            mudwMaxNumParticlesPerEmission = inudwNumParticles;
        }
        //-----------------------------------------------------
        /// Set Material
        ///
        /// The material that each particle will use
        ///
        /// @param Material ptr
        //-----------------------------------------------------
        void ParticleEmitter::SetMaterial(const MaterialSPtr& inpMaterial)
        {
            mpMaterial = inpMaterial;
        }
        //-----------------------------------------------------
        /// Set Emission Frequency
        ///
        /// After what period in seconds should particles be
        /// emitted
        ///
        /// @param Frequency in seconds
        //-----------------------------------------------------
        void ParticleEmitter::SetEmissionFrequency(f32 infFreq)
        {
            mfEmissionFreq = infFreq;
        }
		//-------------------------------------------------
		/// Sets the UVs used for a particle
		//-------------------------------------------------
		void ParticleEmitter::SetParticleUVs(const Core::Rectangle & insUVs)
        {
			msParticleUVs = insUVs;
		}
        //-----------------------------------------------------
        /// Set Lifetime
        ///
        /// Set the lifetime of the particles in second
        ///
        /// @param Lifetime in seconds
        //-----------------------------------------------------
        void ParticleEmitter::SetLifetime(f32 infLifetime)
        {
            mfTimeToLive = infLifetime;
            
            if(mfTimeToLive == 0.0f)
            {
                mfEnergyLoss = 0.0f;
            }
            else
            {
                mfEnergyLoss = 1.0f/mfTimeToLive;
            }
        }
        //-----------------------------------------------------
        /// Set Looping
        ///
        /// Set whether the emitter is continuous
        ///
        /// @param Toggle on/off
        //-----------------------------------------------------
        void ParticleEmitter::SetLooping(bool inbLooping)
        {
            mbShouldLoop = inbLooping;
        }
		//-----------------------------------------------------
		/// Update Sprite Data
		///
		/// Rebuild the sprite data
		//-----------------------------------------------------
		void ParticleEmitter::UpdateSpriteData(const Core::Vector3& invPos, const Core::Colour & insTintColour, SpriteComponent::SpriteData& outsData,
                                                const Core::Vector3& invRight, const Core::Vector3& invUp, const Core::Vector3& invScale)
		{
			Core::Colour::ByteColour Col = Core::Colour::ColourToByteColour(insTintColour);
			
			outsData.sVerts[(u32)SpriteComponent::Verts::k_topLeft].Col = Col;
            outsData.sVerts[(u32)SpriteComponent::Verts::k_bottomLeft].Col = Col;
            outsData.sVerts[(u32)SpriteComponent::Verts::k_topRight].Col = Col;
            outsData.sVerts[(u32)SpriteComponent::Verts::k_bottomRight].Col = Col;
			
			outsData.sVerts[(u32)SpriteComponent::Verts::k_topLeft].vTex = msParticleUVs.TopLeft();
			outsData.sVerts[(u32)SpriteComponent::Verts::k_bottomLeft].vTex = msParticleUVs.BottomLeft();
			outsData.sVerts[(u32)SpriteComponent::Verts::k_topRight].vTex = msParticleUVs.TopRight();
			outsData.sVerts[(u32)SpriteComponent::Verts::k_bottomRight].vTex = msParticleUVs.BottomRight();
			
			outsData.pMaterial = mpMaterial;
            outsData.pMaterial->SetActiveShaderProgram(ShaderPass::k_ambient);
			
            Core::Vector3 vHalfRight = (0.5f * mvInitialScale.x * invScale.x) * invRight;
            Core::Vector3 vHalfUp = (0.5f * mvInitialScale.y * invScale.y) * invUp;
            
            Core::Vector4 vTemp(vHalfUp.x - vHalfRight.x, vHalfUp.y - vHalfRight.y, vHalfUp.z - vHalfRight.z, 1.0f);
            outsData.sVerts[(u32)SpriteComponent::Verts::k_topLeft].vPos = invPos + vTemp;
            
            vTemp.x = vHalfUp.x + vHalfRight.x; vTemp.y = vHalfUp.y + vHalfRight.y; vTemp.z = vHalfUp.z + vHalfRight.z;
            outsData.sVerts[(u32)SpriteComponent::Verts::k_topRight].vPos = invPos + vTemp;
            
            vTemp.x = -vHalfUp.x - vHalfRight.x; vTemp.y = -vHalfUp.y - vHalfRight.y; vTemp.z = -vHalfUp.z - vHalfRight.z;
            outsData.sVerts[(u32)SpriteComponent::Verts::k_bottomLeft].vPos = invPos + vTemp;
            
            vTemp.x = -vHalfUp.x + vHalfRight.x; vTemp.y = -vHalfUp.y + vHalfRight.y; vTemp.z = -vHalfUp.z + vHalfRight.z;
            outsData.sVerts[(u32)SpriteComponent::Verts::k_bottomRight].vPos = invPos + vTemp;
		}
        //-------------------------------------------------------
        /// Destructor
        //-------------------------------------------------------
        ParticleEmitter::~ParticleEmitter()
        {
            delete[] m_particles;
        }
    }
}