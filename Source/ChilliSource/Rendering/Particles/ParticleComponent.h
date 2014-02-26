/*
 *  ParticleComponent.h
 *  moFloTest
 *
 *  Created by Scott Downie on 07/01/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#ifndef _MO_FLO_RENDERING_PARTICLE_COMPONENT_H_
#define _MO_FLO_RENDERING_PARTICLE_COMPONENT_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Rendering/Base/RenderComponent.h>
#include <ChilliSource/Rendering/Particles/Emitters/ParticleEmitter.h>

namespace ChilliSource
{
	namespace Rendering
	{
		//====================================================
		/// Particle Component
		///
		/// Multiple emitters that create the system
		//====================================================
		class CParticleComponent : public IRenderComponent
		{
		public:
            
            typedef fastdelegate::FastDelegate1<CParticleComponent*> EmittersFinishedDelegate;
			
			DECLARE_NAMED_INTERFACE(CParticleComponent);
			
			CParticleComponent();
            ~CParticleComponent();
			//----------------------------------------------------------
			/// Is A
			///
			/// Returns if it is of the type given
			/// @param Comparison Type
			/// @return Whether the class matches the comparison type
			//----------------------------------------------------------
			bool IsA(ChilliSource::Core::InterfaceIDType inInterfaceID) const;
			
			
			//---------------------------------------------------
			/// Add Emitter
			///
			/// Add a new particle emitter to the system
			///
			/// @param Emitter
			//---------------------------------------------------
			void AddEmitter(CParticleEmitter* inpEmitter);
		

			//---------------------------------------------------
			/// Get number of emitters in this system
			//---------------------------------------------------
			u32 GetNumEmitters() const;
			
			//---------------------------------------------------
			/// Returns the emitter at the given index or nullptr
			///
			/// @param Index
			//---------------------------------------------------
			CParticleEmitter* GetEmitter(u32 inudwIndex);
			
            //---------------------------------------------------
            /// Add Effector
            ///
            /// Add a new particle effector to all emitters
            ///
            /// @param Effector
            //---------------------------------------------------
            void AddEffector(IParticleEffector* inpEffector);
            //---------------------------------------------------
            /// Start Emitting
            ///
            /// Particle emitters will resume emitting
            //---------------------------------------------------
            void StartEmitting();
            //---------------------------------------------------
            /// Emit Once
            ///
            /// Particle emitters will fire particles once
            //---------------------------------------------------
            void EmitBurst();
            //---------------------------------------------------
            /// Stop Emitting
            ///
            /// No new particles will be emitted once the current
            /// ones die
            //---------------------------------------------------
            void StopEmitting();
            //---------------------------------------------------
            /// Set Update Scale Factor
            ///
            /// A scalar value by which the update time will
            /// be modified. This can be used to speed up and
            /// slow down particles
            //---------------------------------------------------
            void SetUpdateScaleFactor(f32 infScale);
			//-------------------------------------------------
			/// Update
			///
			/// Update all the emitters
			/// 
			/// @param Time since last frame
			//-------------------------------------------------
			void Update(f32 infDt);
            //-------------------------------------------------
            /// Render
            ///
            /// Called by the renderer so we may render all
            /// our emitters
            ///
            /// @param Render system
            /// @param Active camera component
            /// @param The current shader pass.
            //-------------------------------------------------
            void Render(IRenderSystem* inpRenderSystem, CCameraComponent* inpCam, ShaderPass ineShaderPass);
            //-----------------------------------------------------
            /// Render Shadow Map
            ///
            /// Render the mesh to the shadow map
            ///
            /// @param Render system
            /// @param Active camera component
            //-----------------------------------------------------
            void RenderShadowMap(IRenderSystem* inpRenderSystem, CCameraComponent* inpCam){};
			//-------------------------------------------------
			/// Set Owning System
			///
			/// @param Particle system
			//-------------------------------------------------
			void SetOwningSystem(CParticleSystem* inpSystem);
			//-------------------------------------------------
			/// Remove From World System
			//-------------------------------------------------
			void RemoveFromWorldSystem();
			//-------------------------------------------------
			/// Set Emitters Finished Delegate
			//-------------------------------------------------
            inline void SetEmittersFinishedDelegate(const EmittersFinishedDelegate& inDelegate) { mEmittersFinishedDelegate = inDelegate; }
			//-------------------------------------------------
			/// Are all Emitters finished? (when emitting once!)
			//-------------------------------------------------
            inline bool GetEmittersFinished() { return mbEmittersFinished; }
			//-------------------------------------------------
			/// Safely run the delegate
			//-------------------------------------------------
            void EmitterFinishedDelegate() { if(mEmittersFinishedDelegate) mEmittersFinishedDelegate(this); }
			
		private:
			
			DYNAMIC_ARRAY<CParticleEmitter*> mEmitters;
			DYNAMIC_ARRAY<IParticleEffector*> mEffectors;
			
			CParticleSystem* mpOwningSystem;
            
            f32 mfUpdateScaleFactor;
            
            bool mbEmittersFinished;
            EmittersFinishedDelegate mEmittersFinishedDelegate;
		};
	}
}

#endif