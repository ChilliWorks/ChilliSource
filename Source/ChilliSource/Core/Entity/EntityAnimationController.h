/*
 *  EntityAnimationController.h
 *  SceneLoader
 *
 *  Created by Stuart McGaw on 22/02/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#ifndef _MOFLO_CORE_ENTITYANIMATIONCONTROLLER_H_
#define _MOFLO_CORE_ENTITYANIMATIONCONTROLLER_H_

#include <ChilliSource/Core/ForwardDeclarations.h>
#include <ChilliSource/Core/Entity/Entity.h>
#include <ChilliSource/Core/Entity/EntityAnimation.h>

namespace moFlo
{
	namespace Core
    {
		class CEntityAnimationController 
        {
		public:	
			//----------------------------------------------
			/// CEntityAnimationController
			///
			//----------------------------------------------
			CEntityAnimationController();
			virtual ~CEntityAnimationController();
			
			//----------------------------------------------
			/// LoadDataFromSceneDesc
			///
			/// Causes the controller to fill its internal hashmap of animation names to EntityAnimationData using the contents of a SceneDesc.
			/// The hashmap is first cleared before the new data is retrieved.
			///
			/// @param SceneDesc to clone data
			//----------------------------------------------
			void LoadDataFromSceneAnimDesc(const CSceneAnimationDesc & insSceneAnimDesc, bool inbClearExistingData = true);
			
			//----------------------------------------------
			/// ReleaseAnimations
			///
			/// The controller will release all its shared_ptrs to EntityAnimations it created. Although EntityAnimations which
			/// have been held elsewhere will still be valid they will no longer be updated automatically.
			//----------------------------------------------
			void ReleaseAnimations();
			
			//----------------------------------------------
			/// CreateAnimation
			///
			/// Creates and retains an EntityAnimation object with animdata with the given name and target entity.
			/// Will return an empty pointer if it cannot find animdata with the given name or the target entity is NULL.
			//----------------------------------------------
			EntityAnimationPtr CreateAnimation(const std::string & instrAnimName, moFlo::Core::CEntity* inpAnimTarget, u32 inudwQueryFlags = 0);
			
			//----------------------------------------------
			/// CreateAnimation
			///
			/// Creates and retains an EntityAnimation object with animdata with the given name and target entity.
			/// Will return an empty pointer if it cannot find animdata with the given name or the target entity is NULL.
			//----------------------------------------------
			EntityAnimationPtr CreateAndPlayAnimation(const std::string & instrAnimName, moFlo::Core::CEntity* inpAnimTarget, AnimationPlayMode inePlayMode);
            
            //----------------------------------------------
            /// Pause
            ///
            /// Stop the animation timer
            //----------------------------------------------
			void Pause();
            
            //----------------------------------------------
            /// Resume
            ///
            /// Stop the animation timer
            //----------------------------------------------
            void Resume();
            
            //----------------------------------------------
            /// Restart
            ///
            /// Put all the animations back to the first frame
            //----------------------------------------------
            void Restart();
            
			//----------------------------------------------
			/// Update
			///
			/// Causes the controller to update all of the animations it has created.
			//----------------------------------------------
			void Update(f32 infDT);
            
            //----------------------------------------------
            /// Reset Anim Data
            ///
            /// Reset the hash map
            //----------------------------------------------
            void ResetAnimData();
			
            //----------------------------------------------
            /// ApplyAnimationToEntity
            ///
            /// Given the animation held in insSceneDesc, apply it to the entity hierarchy specified by inpcEntity
			///
            //----------------------------------------------
			void ApplyAnimationToEntity(const CSceneAnimationDesc& insSceneAnimDesc, const EntityPtr& inpcEntity, AnimationPlayMode inePlayMode);

            //----------------------------------------------
            /// GetEventCompletion
            ///
            /// Return event to subscribe to for animation completion events
			///
            //----------------------------------------------
            typedef fastdelegate::FastDelegate1<CEntityAnimationController*> EventAnimationCompletion;
            moFlo::IEvent<EventAnimationCompletion> & GetEventCompletion() { return mAnimationCompletionEvent; }
			
            //----------------------------------------------
            /// Get Event Looped
            ///
            /// Return event to subscribe to for animation looped events
			///
            //----------------------------------------------
            moFlo::IEvent<EventAnimationCompletion> & GetEventLooped() { return mAnimationLoopedEvent; }
            
			bool GetIsFinished() const;
		protected:
			struct EntityAnimContext
			{
				EntityAnimContext()
				{
					bDisposeOnComplete = false;
				}
				
				EntityAnimContext(EntityAnimationPtr inpAnimActual, bool inbDisposeOnComplete)
				:pAnimActual(inpAnimActual)
				,bDisposeOnComplete(inbDisposeOnComplete)
				{
				}
				
				EntityAnimationPtr	pAnimActual;
				bool				bDisposeOnComplete;
			};


			const EntityAnimationData* GetEntityAnimationDataWithName(const std::string & instrName) const;


			HASH_MAP<std::string, EntityAnimationData>	mmapAnimDataToEntityName;
			DYNAMIC_ARRAY<EntityAnimContext>			masPlayingAnimations;
            CEvent1<EventAnimationCompletion>           mAnimationCompletionEvent;
            CEvent1<EventAnimationCompletion>           mAnimationLoopedEvent;
            
            bool mbPaused;
		};
		
		typedef SHARED_PTR<CEntityAnimationController> EntityAnimationControllerPtr;
	}
}

#endif
