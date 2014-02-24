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
		CEntityAnimationController::CEntityAnimationController()
        : mbPaused(false)
		{
		}
		
		CEntityAnimationController::~CEntityAnimationController()
		{
		}

        void CEntityAnimationController::ResetAnimData()
        {
            mmapAnimDataToEntityName.clear();
        }
		
		void CEntityAnimationController::LoadDataFromSceneAnimDesc(const CSceneAnimationDesc & insSceneDesc, bool inbClearExistingData)
        {
			// Reset the existing animation data
            if (inbClearExistingData)
				ResetAnimData();
			
			for (u32 nAnim = 0; nAnim < insSceneDesc.asEntityAnimations.size(); nAnim++) 
            {
				const CSceneAnimationDesc::EntityAnimationDesc & sDesc = insSceneDesc.asEntityAnimations[nAnim];
				
				EntityAnimationData sData;
				sData.afKeyframeTimes = sDesc.afKeyframeTimes;
				sData.aKeyframeValues = sDesc.aKeyframeValues;
                sData.udwKeyframeCount = sDesc.udwKeyframeCount;
				
				mmapAnimDataToEntityName.insert(std::make_pair(sDesc.strName, sData));
			}
		}
		
		const EntityAnimationData* CEntityAnimationController::GetEntityAnimationDataWithName(const std::string & instrName) const 
		{
			HASH_MAP<std::string, EntityAnimationData>::const_iterator pEntry = mmapAnimDataToEntityName.find(instrName);
			if (pEntry == mmapAnimDataToEntityName.end())
				return NULL;
			else 
				return &(pEntry->second);
		}
		
		void CEntityAnimationController::ReleaseAnimations()
		{
			masPlayingAnimations.clear();
		}
		
		EntityAnimationPtr CEntityAnimationController::CreateAnimation(const std::string & instrAnimName, CEntity* inpAnimTarget, u32 inudwQueryFlags)
        {
			const EntityAnimationData* pData = GetEntityAnimationDataWithName(instrAnimName);
			assert(pData);
			
			EntityAnimationPtr pResult = EntityAnimationPtr(new CEntityAnimation(pData, inpAnimTarget, inudwQueryFlags));
			assert(pResult);
			
			masPlayingAnimations.push_back(EntityAnimContext(pResult, false));
			return pResult;			
		}
		
		EntityAnimationPtr CEntityAnimationController::CreateAndPlayAnimation(const std::string & instrAnimName, CEntity* inpAnimTarget, AnimationPlayMode inePlayMode)
		{
			const EntityAnimationData* pData = GetEntityAnimationDataWithName(instrAnimName);
			assert(pData);
			
			EntityAnimationPtr pResult = EntityAnimationPtr(new CEntityAnimation(pData, inpAnimTarget));
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
        void CEntityAnimationController::Pause()
        {
            mbPaused = true;
        }
        //----------------------------------------------
        /// Resume
        ///
        /// Stop the animation timer
        //----------------------------------------------
        void CEntityAnimationController::Resume()
        {
            mbPaused = false;
        }
        //----------------------------------------------
        /// Restart
        ///
        /// Put all the animations back to the first frame
        //----------------------------------------------
		void CEntityAnimationController::Restart()
		{
			for	(u32 nAnim = 0; nAnim < masPlayingAnimations.size(); ++nAnim)
			{
				masPlayingAnimations[nAnim].pAnimActual->ToFrame(0);
			}
		}
		
		void CEntityAnimationController::Update(f32 infDT)
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
                    mAnimationCompletionEvent.Invoke(this);
                else if(bLooped)
                    mAnimationLoopedEvent.Invoke(this);
            }
		}
		
		void CEntityAnimationController::ApplyAnimationToEntity(const CSceneAnimationDesc& insSceneAnimDesc, const EntityPtr& inpcEntity, AnimationPlayMode inePlayMode)
		{
			// Now setup a temporary animation for this entity
			LoadDataFromSceneAnimDesc(insSceneAnimDesc, true);
			
			// Loop through each individual animation and queue its animation
			for (u32 nAnim = 0; nAnim < insSceneAnimDesc.asEntityAnimations.size(); nAnim++) 
			{
                const CSceneAnimationDesc::EntityAnimationDesc & sCurrentAnimation(insSceneAnimDesc.asEntityAnimations[nAnim]);
//				string strName = sCurrentAnimation.strTargetPath;
				CEntity* pEntity = inpcEntity->FindChildEntityWithName(sCurrentAnimation.strTargetPath);
				if(pEntity)
                {
					CreateAndPlayAnimation(sCurrentAnimation.strName, pEntity, inePlayMode);
                }
			}
		}
	
		bool CEntityAnimationController::GetIsFinished() const
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