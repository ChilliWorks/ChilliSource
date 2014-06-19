//
//  ParticleComponent.h
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

#ifndef _CHILLISOURCE_RENDERING_PARTICLE_COMPONENT_H_
#define _CHILLISOURCE_RENDERING_PARTICLE_COMPONENT_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Rendering/Base/RenderComponent.h>
#include <ChilliSource/Rendering/Particles/Emitters/ParticleEmitter.h>

#include <functional>

namespace ChilliSource
{
	namespace Rendering
	{
		//====================================================
		/// Particle Component
		///
		/// Multiple emitters that create the system
		//====================================================
		class ParticleComponent : public RenderComponent
		{
		public:
            
            typedef std::function<void(ParticleComponent*)> EmittersFinishedDelegate;
			
			CS_DECLARE_NAMEDTYPE(ParticleComponent);
			
			ParticleComponent();
            ~ParticleComponent();
			//----------------------------------------------------------
			/// Is A
			///
			/// Returns if it is of the type given
			/// @param Comparison Type
			/// @return Whether the class matches the comparison type
			//----------------------------------------------------------
			bool IsA(CSCore::InterfaceIDType inInterfaceID) const override;
			
			
			//---------------------------------------------------
			/// Add Emitter
			///
			/// Add a new particle emitter to the system
			///
			/// @param Emitter
			//---------------------------------------------------
			void AddEmitter(ParticleEmitterUPtr inpEmitter);
		

			//---------------------------------------------------
			/// Get number of emitters in this system
			//---------------------------------------------------
			u32 GetNumEmitters() const;
			
			//---------------------------------------------------
			/// Returns the emitter at the given index or nullptr
			///
			/// @param Index
			//---------------------------------------------------
			ParticleEmitter* GetEmitter(u32 inudwIndex);
			
            //---------------------------------------------------
            /// Add Affector
            ///
            /// Add a new particle affector to all emitters
            ///
            /// @param Affector
            //---------------------------------------------------
            void AddAffector(ParticleAffectorUPtr inpAffector);
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
            void Render(RenderSystem* inpRenderSystem, CameraComponent* inpCam, ShaderPass ineShaderPass) override;
            //-----------------------------------------------------
            /// Render Shadow Map
            ///
            /// Render the mesh to the shadow map
            ///
            /// @param Render system
            /// @param Active camera component
            /// @param Material to render static shadows with
            /// @param Material to render skinned shadows with
            //-----------------------------------------------------
            void RenderShadowMap(RenderSystem* inpRenderSystem, CameraComponent* inpCam, const MaterialCSPtr& in_staticShadowMap, const MaterialCSPtr& in_animShadowMap) override {};
			//-------------------------------------------------
			/// Set Owning System
			///
			/// @param Particle system
			//-------------------------------------------------
			void SetOwningSystem(ParticleSystem* inpSystem);
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
			
			std::vector<ParticleEmitterUPtr> mEmitters;
			std::vector<ParticleAffectorUPtr> mAffectors;
			
			ParticleSystem* mpOwningSystem;
            
            f32 mfUpdateScaleFactor;
            
            bool mbEmittersFinished;
            EmittersFinishedDelegate mEmittersFinishedDelegate;
		};
	}
}

#endif
