/*
 *  EntityAnimationController.cpp
 *  SceneLoader
 *
 *  Created by Stuart McGaw on 22/02/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

// INCLUDED FILES
#include <string>
#include <ChilliSource/Core/Entity/EntityAnimationController.h>

using namespace std;

// CLASSES
namespace ChilliSource 
{
	namespace Core 
	{
		EntityAnimationController::EntityAnimationController()
        : mbPaused(false)
		{
		}
		
		EntityAnimationController::~EntityAnimationController()
		{
		}

        void EntityAnimationController::ResetAnimData()
        {
            mmapAnimDataToEntityName.clear();
        }
		
		void EntityAnimationController::LoadDataFromSceneAnimDesc(const SceneAnimationDesc & insSceneDesc, bool inbClearExistingData)
        {
			// Reset the existing animation data
            if (inbClearExistingData)
				ResetAnimData();
			
			for (u32 nAnim = 0; nAnim < insSceneDesc.asEntityAnimations.size(); nAnim++) 
            {
				const SceneAnimationDesc::EntityAnimationDesc & sDesc = insSceneDesc.asEntityAnimations[nAnim];
				
				EntityAnimationData sData;
				sData.afKeyframeTimes = sDesc.afKeyframeTimes;
				sData.aKeyframeValues = sDesc.aKeyframeValues;
                sData.udwKeyframeCount = sDesc.udwKeyframeCount;
				
				mmapAnimDataToEntityName.insert(std::make_pair(sDesc.strName, sData));
			}
		}
		
		const EntityAnimationData* EntityAnimationController::GetEntityAnimationDataWithName(const std::string & instrName) const 
		{
			std::unordered_map<std::string, EntityAnimationData>::const_iterator pEntry = mmapAnimDataToEntityName.find(instrName);
			if (pEntry == mmapAnimDataToEntityName.end())
				return nullptr;
			else 
				return &(pEntry->second);
		}
		
		void EntityAnimationController::ReleaseAnimations()
		{
			masPlayingAnimations.clear();
		}
		
		EntityAnimationSPtr EntityAnimationController::CreateAnimation(const std::string & instrAnimName, Entity* inpAnimTarget, u32 inudwQueryFlags)
        {
			const EntityAnimationData* pData = GetEntityAnimationDataWithName(instrAnimName);
			assert(pData);
			
			EntityAnimationSPtr pResult = EntityAnimationSPtr(new EntityAnimation(pData, inpAnimTarget, inudwQueryFlags));
			assert(pResult);
			
			masPlayingAnimations.push_back(EntityAnimContext(pResult, false));
			return pResult;			
		}
		
		EntityAnimationSPtr EntityAnimationController::CreateAndPlayAnimation(const std::string & instrAnimName, Entity* inpAnimTarget, AnimationPlayMode inePlayMode)
		{
			const EntityAnimationData* pData = GetEntityAnimationDataWithName(instrAnimName);
			assert(pData);
			
			EntityAnimationSPtr pResult = EntityAnimationSPtr(new EntityAnimation(pData, inpAnimTarget));
			assert(pResult);
			
			masPlayingAnimations.push_back(EntityAnimContext(pResult, false));
			pResult->Play(inePlayMode);
			return pResult;
		}
        
        //----------------------------------------------
        /// Pause
        ///
        /// Stop the animation timer
        //----------------------------------------------
        void EntityAnimationController::Pause()
        {
            mbPaused = true;
        }
        //----------------------------------------------
        /// Resume
        ///
        /// Stop the animation timer
        //----------------------------------------------
        void EntityAnimationController::Resume()
        {
            mbPaused = false;
        }
        //----------------------------------------------
        /// Restart
        ///
        /// Put all the animations back to the first frame
        //----------------------------------------------
		void EntityAnimationController::Restart()
		{
			for	(u32 nAnim = 0; nAnim < masPlayingAnimations.size(); ++nAnim)
			{
				masPlayingAnimations[nAnim].pAnimActual->ToFrame(0);
			}
		}
		
		void EntityAnimationController::Update(f32 infDT)
		{
            if(!mbPaused)
            {
                bool bFinished = masPlayingAnimations.size() != 0;
                bool bLooped = masPlayingAnimations.size() != 0;
                for	(u32 nAnim = 0; nAnim < masPlayingAnimations.size(); nAnim++)
                {
                    masPlayingAnimations[nAnim].pAnimActual->Update(infDT);
                    bFinished = bFinished && masPlayingAnimations[nAnim].pAnimActual->IsFinished();
                    bLooped = bLooped && masPlayingAnimations[nAnim].pAnimActual->IsLooped();
                }
                if(bFinished)
                    mAnimationCompletionEvent.NotifyConnections(this);
                else if(bLooped)
                    mAnimationLoopedEvent.NotifyConnections(this);
            }
		}
		
		void EntityAnimationController::ApplyAnimationToEntity(const SceneAnimationDesc& insSceneAnimDesc, const EntitySPtr& inpcEntity, AnimationPlayMode inePlayMode)
		{
			// Now setup a temporary animation for this entity
			LoadDataFromSceneAnimDesc(insSceneAnimDesc, true);
			
			// Loop through each individual animation and queue its animation
			for (u32 nAnim = 0; nAnim < insSceneAnimDesc.asEntityAnimations.size(); nAnim++) 
			{
                const SceneAnimationDesc::EntityAnimationDesc & sCurrentAnimation(insSceneAnimDesc.asEntityAnimations[nAnim]);

				Entity* pEntity = inpcEntity->GetChildEntityWithNameRecursive(sCurrentAnimation.strTargetPath).get();
				if(pEntity)
                {
					CreateAndPlayAnimation(sCurrentAnimation.strName, pEntity, inePlayMode);
                }
			}
		}
	
		bool EntityAnimationController::GetIsFinished() const
		{
			bool bFinished = true;
			
			// Any animations not finished?
			for (u32 i = 0; i < masPlayingAnimations.size() && bFinished; ++i)
			{
				if (!masPlayingAnimations[i].pAnimActual->IsFinished())
					bFinished = false;
			}
				
			return bFinished;
		}
	
	}
}