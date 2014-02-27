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

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Entity/Entity.h>
#include <ChilliSource/Core/Entity/EntityAnimation.h>

namespace ChilliSource
{
	namespace Core
    {
		class EntityAnimationController 
        {
		public:	
			//----------------------------------------------
			/// EntityAnimationController
			///
			//----------------------------------------------
			EntityAnimationController();
			virtual ~EntityAnimationController();
			
			//----------------------------------------------
			/// LoadDataFromSceneDesc
			///
			/// Causes the controller to fill its internal hashmap of animation names to EntityAnimationData using the contents of a SceneDesc.
			/// The hashmap is first cleared before the new data is retrieved.
			///
			/// @param SceneDesc to clone data
			//----------------------------------------------
			void LoadDataFromSceneAnimDesc(const SceneAnimationDesc & insSceneAnimDesc, bool inbClearExistingData = true);
			
			//----------------------------------------------
			/// ReleaseAnimations
			///
			/// The controller will release all its std::shared_ptrs to EntityAnimations it created. Although EntityAnimations which
			/// have been held elsewhere will still be valid they will no longer be updated automatically.
			//----------------------------------------------
			void ReleaseAnimations();
			
			//----------------------------------------------
			/// CreateAnimation
			///
			/// Creates and retains an EntityAnimation object with animdata with the given name and target entity.
			/// Will return an empty pointer if it cannot find animdata with the given name or the target entity is nullptr.
			//----------------------------------------------
			EntityAnimationSPtr CreateAnimation(const std::string & instrAnimName, ChilliSource::Core::Entity* inpAnimTarget, u32 inudwQueryFlags = 0);
			
			//----------------------------------------------
			/// CreateAnimation
			///
			/// Creates and retains an EntityAnimation object with animdata with the given name and target entity.
			/// Will return an empty pointer if it cannot find animdata with the given name or the target entity is nullptr.
			//----------------------------------------------
			EntityAnimationSPtr CreateAndPlayAnimation(const std::string & instrAnimName, ChilliSource::Core::Entity* inpAnimTarget, AnimationPlayMode inePlayMode);
            
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
			void ApplyAnimationToEntity(const SceneAnimationDesc& insSceneAnimDesc, const EntitySPtr& inpcEntity, AnimationPlayMode inePlayMode);

            //----------------------------------------------
            /// GetEventCompletion
            ///
            /// Return event to subscribe to for animation completion events
			///
            //----------------------------------------------
            typedef std::function<void(EntityAnimationController*)> EventAnimationCompletion;
            Core::IConnectableEvent<EventAnimationCompletion> & GetEventCompletion() { return mAnimationCompletionEvent; }
			
            //----------------------------------------------
            /// Get Event Looped
            ///
            /// Return event to subscribe to for animation looped events
			///
            //----------------------------------------------
            Core::IConnectableEvent<EventAnimationCompletion> & GetEventLooped() { return mAnimationLoopedEvent; }
            
			bool GetIsFinished() const;
		protected:
			struct EntityAnimContext
			{
				EntityAnimContext()
				{
					bDisposeOnComplete = false;
				}
				
				EntityAnimContext(EntityAnimationSPtr inpAnimActual, bool inbDisposeOnComplete)
				:pAnimActual(inpAnimActual)
				,bDisposeOnComplete(inbDisposeOnComplete)
				{
				}
				
				EntityAnimationSPtr	pAnimActual;
				bool				bDisposeOnComplete;
			};


			const EntityAnimationData* GetEntityAnimationDataWithName(const std::string & instrName) const;


			std::unordered_map<std::string, EntityAnimationData> mmapAnimDataToEntityName;
			std::vector<EntityAnimContext> masPlayingAnimations;
            Event<EventAnimationCompletion> mAnimationCompletionEvent;
            Event<EventAnimationCompletion> mAnimationLoopedEvent;
            
            bool mbPaused;
		};
	}
}

#endif
