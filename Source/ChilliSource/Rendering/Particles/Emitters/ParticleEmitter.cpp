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
        CParticleEmitter::CParticleEmitter(const Core::ParamDictionary& inParams, const MaterialPtr &inpMaterial, CParticleComponent* inpComponent)
        : mudwMaxNumParticles(100), mudwMaxNumParticlesPerEmission(1), mfEmissionFreq(0.5f), mfCurrentTime(0.0f), mfLastEmissionTime(0.0f), mfTimeToLive(1.0f), mvInitialScale(1.0f, 1.0f), mbShouldLoop(true)
        ,mfEnergyLoss(1.0f/mfTimeToLive), mpOwningComponent(inpComponent), mbIsEmitting(true), mudwNumUsed(0), mpMaterial(inpMaterial),msParticleUVs(Core::CVector2::ZERO, Core::CVector2(1.0f,1.0f))
		,mudwBurstCounter(0), mbIsEmittingFinished(false)
        ,mbIsGlobalSpace(true)
        {
            std::string strTemp;
            
            //Frequency
            if(inParams.TryGetValue("Frequency", strTemp))
            {
                mfEmissionFreq = Core::ParseF32(strTemp);
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
            
            mudwMaxNumParticles = (std::ceil(mfTimeToLive/mfEmissionFreq) * mudwMaxNumParticlesPerEmission);
            
            // Allocate all the memory upfront
			mParticles.vTranslation = new Core::CVector3[mudwMaxNumParticles];
            mParticles.vScale = new Core::CVector3[mudwMaxNumParticles];
            mParticles.vVelocity = new Core::CVector3[mudwMaxNumParticles];
			mParticles.Col = new Core::CColour[mudwMaxNumParticles];
            mParticles.fEnergy = new f32[mudwMaxNumParticles];
            mParticles.fAngularRotation = new f32[mudwMaxNumParticles];
            
            // Initialise Energy array to 0
            for(u32 i=0; i<mudwMaxNumParticles; ++i)
            {
                mParticles.fEnergy[i] = 0.0f;
                mParticles.fAngularRotation[i] = 0.0f;
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
        void CParticleEmitter::Update(f32 infDT)
        {
            mfCurrentTime += infDT;
            
            const Core::CVector3& vCurrentPos = mpOwningComponent->GetEntityOwner()->Transform().GetWorldPosition();
            const f32 kfTimeSinceLastEmission = mfCurrentTime - mfLastEmissionTime;
            u32 udwNumEmits = kfTimeSinceLastEmission/mfEmissionFreq;
            
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
            f32 fEmissionStep = mfEmissionFreq;
            bool bParticlesActive = false;
            
            for(u32 i=0; i<mudwMaxNumParticles; ++i)
            {
                //If time to emit and we have some left in our quota...or we are a looping emitter then we need to emit
                bool bEmit = (mbIsEmitting && (mudwNumUsed < mudwMaxNumParticles) && udwNumParticleToEmit > 0 && mParticles.fEnergy[i] <= 0.0f);
                if(bEmit)
                {
                    mudwNumUsed++;
                    udwNumParticleToEmit--;
                    bParticlesActive = true;
                    
                    f32 fLerpFactor = fEmissionStep/kfTimeSinceLastEmission;
                    Core::CVector3 vPosition = Core::CMathUtils::Lerp(fLerpFactor, mvLastEmissionPos, vCurrentPos) - vCurrentPos;
                    fEmissionStep += mfEmissionFreq;
                    
                    Core::CQuaternion qOrientation;
                    Core::CVector3 vScale(Core::CVector3::ONE);
                    
                    if(mbIsGlobalSpace)
                    {
                        vPosition = vCurrentPos + vPosition;
                        qOrientation = mpOwningComponent->GetEntityOwner()->Transform().GetWorldOrientation();
                        vScale = mpOwningComponent->GetEntityOwner()->Transform().GetWorldScale();
                    }
                    
                    mParticles.fEnergy[i] = 1.0f;
                    mParticles.Col[i] = mInitialColour;
                    mParticles.vTranslation[i] = vPosition;
                    mParticles.vScale[i] = vScale;
                    mParticles.fAngularRotation[0] = 0.0f;
                    
                    //We will emit a particle and pass it through the effector
                    Emit(&mParticles, i);
                    
                    // We need to rotate our velocity by our emmiters orientation if in global space
                    if(mbIsGlobalSpace)
                    {
                        mParticles.vVelocity[i] = qOrientation * mParticles.vVelocity[i];
                    }
                    
                    for(DYNAMIC_ARRAY<IParticleEffector*>::iterator itEffector = mEffectors.begin(); itEffector != mEffectors.end(); ++itEffector)
                    {
                        (*itEffector)->Init(&mParticles, i);
                    }
                    
                    for(u32 udwUpdateCount=0; udwUpdateCount<udwNumEmits; ++udwUpdateCount)
                    {
                        UpdateParticle(i, infDT);
                    }
                }
                else if(mParticles.fEnergy[i] > 0.0f)
                {
                    bParticlesActive = true;
                    UpdateParticle(i, infDT);
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
        void CParticleEmitter::UpdateParticle(u32 inudwParticleIndex, f32 infDT)
        {
            //Apply the effector to each particle
            for(DYNAMIC_ARRAY<IParticleEffector*>::iterator itEffector = mEffectors.begin(); itEffector != mEffectors.end(); ++itEffector)
            {
                if(mParticles.fEnergy[inudwParticleIndex] <= (*itEffector)->GetActiveEnergyLevel())
                {
                    (*itEffector)->Apply(&mParticles, inudwParticleIndex, infDT);
                }
            }
            
            mParticles.fEnergy[inudwParticleIndex] -= (mfEnergyLoss * infDT);
            mParticles.vTranslation[inudwParticleIndex] += (mParticles.vVelocity[inudwParticleIndex] * infDT);
        }
        //-----------------------------------------------------
        /// Render
        ///
        /// Render the active particles based on their
        /// renderable type
        ///
        /// @param Render system
        //-----------------------------------------------------
        void CParticleEmitter::Render(IRenderSystem* inpRenderSystem, CCameraComponent* inpCam)
        {
            CSpriteComponent::SpriteData sData;
            // Get world matrix
            const Core::CMatrix4x4 & matTrans = mpOwningComponent->GetEntityOwner()->Transform().GetWorldTransform();
            
            // Get quaternion to particle space
            Core::CQuaternion qParticleRot = Core::CQuaternion(matTrans).Conjugate();
            
            const Core::CMatrix4x4 & matCamWorld = inpCam->GetEntityOwner()->Transform().GetWorldTransform();
            // Get cameras up and right vectors in particle space
            
            Core::CVector3 vRight = matCamWorld.Right();
            Core::CVector3 vUp = matCamWorld.Up();
            Core::CVector3 vForward = matCamWorld.Forward();
            
            const Core::CMatrix4x4 * pTransform = NULL;
            if(mbIsGlobalSpace == false)
            {
                pTransform = &matTrans;
                vRight = qParticleRot * vRight;
                vUp = qParticleRot * vUp;
                vForward = qParticleRot * vForward;
            }

            for(u32 i=0; i<mudwMaxNumParticles; ++i)
            {
                if(mParticles.fEnergy[i] > 0.0f && mParticles.Col[i].a > 0.0f)
                {
                    // Rotate per particle
                    Core::CQuaternion qRot(vForward, mParticles.fAngularRotation[i]);

                    UpdateSpriteData(mParticles.vTranslation[i], mParticles.Col[i], sData, qRot * vRight, qRot * vUp, mParticles.vScale[i]);
                    
                    if (sData.pMaterial->GetActiveShaderProgram() != NULL)
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
        void CParticleEmitter::StartEmitting()
        {
            mbIsEmitting = true;
        }
        //---------------------------------------------------
        /// Emit Once
        ///
        /// Particle emitters will emit particles once
        //---------------------------------------------------
        void CParticleEmitter::EmitBurst()
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
        void CParticleEmitter::StopEmitting()
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
        void CParticleEmitter::AddEffector(IParticleEffector* inpEffector)
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
        void CParticleEmitter::RemoveEffector(IParticleEffector* inpEffector)
        {
            for(DYNAMIC_ARRAY<IParticleEffector*>::iterator itEffector = mEffectors.begin(); itEffector != mEffectors.end(); ++itEffector)
            {
                if(*itEffector == inpEffector)
                {
                    SAFE_DELETE(*itEffector);
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
        void CParticleEmitter::SetVelocity(f32 infVelocity)
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
        void CParticleEmitter::SetColour(const Core::CColour inColour)
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
        void CParticleEmitter::SetNumParticlesPerEmission(u32 inudwNumParticles)
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
        void CParticleEmitter::SetMaterial(const MaterialPtr& inpMaterial)
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
        void CParticleEmitter::SetEmissionFrequency(f32 infFreq)
        {
            mfEmissionFreq = infFreq;
        }
		//-------------------------------------------------
		/// Sets the UVs used for a particle
		//-------------------------------------------------
		void CParticleEmitter::SetParticleUVs(const Core::Rectangle & insUVs)
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
        void CParticleEmitter::SetLifetime(f32 infLifetime)
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
        void CParticleEmitter::SetLooping(bool inbLooping)
        {
            mbShouldLoop = inbLooping;
        }
		//-----------------------------------------------------
		/// Update Sprite Data
		///
		/// Rebuild the sprite data
		//-----------------------------------------------------
		void CParticleEmitter::UpdateSpriteData(const Core::CVector3& invPos, const Core::CColour & insTintColour, CSpriteComponent::SpriteData& outsData,
                                                const Core::CVector3& invRight, const Core::CVector3& invUp, const Core::CVector3& invScale)
		{
			Core::CColour::ByteColour Col = Core::CColour::ColourToByteColour(insTintColour);
			
			outsData.sVerts[(u32)CSpriteComponent::Verts::k_topLeft].Col = Col;
            outsData.sVerts[(u32)CSpriteComponent::Verts::k_bottomLeft].Col = Col;
            outsData.sVerts[(u32)CSpriteComponent::Verts::k_topRight].Col = Col;
            outsData.sVerts[(u32)CSpriteComponent::Verts::k_bottomRight].Col = Col;
			
			outsData.sVerts[(u32)CSpriteComponent::Verts::k_topLeft].vTex = msParticleUVs.TopLeft();
			outsData.sVerts[(u32)CSpriteComponent::Verts::k_bottomLeft].vTex = msParticleUVs.BottomLeft();
			outsData.sVerts[(u32)CSpriteComponent::Verts::k_topRight].vTex = msParticleUVs.TopRight();
			outsData.sVerts[(u32)CSpriteComponent::Verts::k_bottomRight].vTex = msParticleUVs.BottomRight();
			
			outsData.pMaterial = mpMaterial;
            outsData.pMaterial->SetActiveShaderProgram(ShaderPass::k_ambient);
			
            Core::CVector3 vHalfRight = (0.5f * mvInitialScale.x * invScale.x) * invRight;
            Core::CVector3 vHalfUp = (0.5f * mvInitialScale.y * invScale.y) * invUp;
            
            Core::CVector4 vTemp(vHalfUp.x - vHalfRight.x, vHalfUp.y - vHalfRight.y, vHalfUp.z - vHalfRight.z, 1.0f);
            outsData.sVerts[(u32)CSpriteComponent::Verts::k_topLeft].vPos = invPos + vTemp;
            
            vTemp.x = vHalfUp.x + vHalfRight.x; vTemp.y = vHalfUp.y + vHalfRight.y; vTemp.z = vHalfUp.z + vHalfRight.z;
            outsData.sVerts[(u32)CSpriteComponent::Verts::k_topRight].vPos = invPos + vTemp;
            
            vTemp.x = -vHalfUp.x - vHalfRight.x; vTemp.y = -vHalfUp.y - vHalfRight.y; vTemp.z = -vHalfUp.z - vHalfRight.z;
            outsData.sVerts[(u32)CSpriteComponent::Verts::k_bottomLeft].vPos = invPos + vTemp;
            
            vTemp.x = -vHalfUp.x + vHalfRight.x; vTemp.y = -vHalfUp.y + vHalfRight.y; vTemp.z = -vHalfUp.z + vHalfRight.z;
            outsData.sVerts[(u32)CSpriteComponent::Verts::k_bottomRight].vPos = invPos + vTemp;
		}
        //-------------------------------------------------------
        /// Destructor
        //-------------------------------------------------------
        CParticleEmitter::~CParticleEmitter()
        {
            delete[] mParticles.vTranslation;
            delete[] mParticles.vScale;
            delete[] mParticles.vVelocity;
			delete[] mParticles.Col;
            delete[] mParticles.fEnergy;
            delete[] mParticles.fAngularRotation;
        }
    }
}