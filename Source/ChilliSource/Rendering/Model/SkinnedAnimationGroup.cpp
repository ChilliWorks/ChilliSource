//
//  SkinnedAnimationGroup.cpp
//  moFlow
//
//  Created by Ian Copland on 08/06/2012.
//  Copyright (c) 2012 Tag Games Ltd. All rights reserved.
//

#include <ChilliSource/Rendering/Model/SkinnedAnimationGroup.h>
#include <ChilliSource/Core/Math/Quaternion.h>
#include <ChilliSource/Core/Math/Vector3.h>
#include <ChilliSource/Core/Math/MathUtils.h>
#include <ChilliSource/Rendering/Model/SkinnedAnimation.h>
#include <ChilliSource/Rendering/Model/Skeleton.h>

namespace ChilliSource
{
    namespace Rendering
    {
        //-----------------------------------------------------------
        /// Constructor
        //-----------------------------------------------------------
        CSkinnedAnimationGroup::CSkinnedAnimationGroup(const SkeletonPtr& inpSkeleton)
        : mpSkeleton(inpSkeleton), mbAnimationLengthDirty(true), mfAnimationLength(0.0f), mbPrepared(false)
        {
            for (u32 i = 0; i < mpSkeleton->GetNumNodes(); ++i)
            {
                mCurrentAnimationMatrices.push_back(Core::CMatrix4x4());
            }
        }
        //----------------------------------------------------------
        /// Attach Animation
        //----------------------------------------------------------
        void CSkinnedAnimationGroup::AttachAnimation(const SkinnedAnimationPtr& inpAnimation, f32 infBlendlinePosition)
        {
            mbAnimationLengthDirty = true;
            AnimationItemPtr pItem(new AnimationItem());
            pItem->pSkinnedAnimation = inpAnimation;
            pItem->fBlendlinePosition = infBlendlinePosition;
            mAnimations.push_back(pItem);
        }
        //----------------------------------------------------------
        /// Detatch Animation
        //----------------------------------------------------------
        void CSkinnedAnimationGroup::DetatchAnimation(const SkinnedAnimationPtr& inpAnimation)
        {
            mbAnimationLengthDirty = true;
            for (DYNAMIC_ARRAY<AnimationItemPtr>::iterator it = mAnimations.begin(); it != mAnimations.end(); ++it)
            {
                if ((*it)->pSkinnedAnimation.get() == inpAnimation.get())
                {
                    mAnimations.erase(it);
                }
                break;
            }
        }
        //----------------------------------------------------------
        /// Clear Animations
        //----------------------------------------------------------
        void CSkinnedAnimationGroup::ClearAnimations()
        {
            mAnimations.clear();
        }
        //----------------------------------------------------------
        /// Build Animation Data
        //----------------------------------------------------------
        void CSkinnedAnimationGroup::BuildAnimationData(AnimationBlendType ineBlendType, f32 infPlaybackPosition, f32 infBlendlinePosition)
        {
            //check how many animations we have. if we only have 1 then dont try and blend. If we have none then error.
            if (mAnimations.size() > 1)
            {
                //find which two animations should be blended together
                AnimationItemPtr pAnimItem1;
                AnimationItemPtr pAnimItem2;
                for (DYNAMIC_ARRAY<AnimationItemPtr>::iterator it = mAnimations.begin(); it != mAnimations.end(); ++it)
                {
                    f32 fBlendlinePosition = (*it)->fBlendlinePosition;
                    if (fBlendlinePosition <= infBlendlinePosition && (pAnimItem1 == NULL || fBlendlinePosition > pAnimItem1->fBlendlinePosition))
                    {
                        pAnimItem1 = (*it);
                    }
                    if (fBlendlinePosition >= infBlendlinePosition && (pAnimItem2 == NULL || fBlendlinePosition < pAnimItem2->fBlendlinePosition))
                    {
                        pAnimItem2 = (*it);
                    }
                }
                
                //get the animation frames
                SkinnedAnimationFramePtr pFrame1;
                if (pAnimItem1 != NULL)
                    pFrame1 = CalculateAnimationFrame(pAnimItem1->pSkinnedAnimation, infPlaybackPosition);
                
                SkinnedAnimationFramePtr pFrame2;
                if (pAnimItem2 != NULL)
                    pFrame2 = CalculateAnimationFrame(pAnimItem2->pSkinnedAnimation, infPlaybackPosition);
                
                //check that we do indeed have two animations to blend. if not, just return the frame we do have.
                if (pFrame1 != NULL && pFrame2 != NULL && pAnimItem1.get() != pAnimItem2.get())
                {
                    //get the interpolation factor and then apply the requested blend to the two frames.
                    f32 fFactor = (infBlendlinePosition - pAnimItem1->fBlendlinePosition) / (pAnimItem2->fBlendlinePosition - pAnimItem1->fBlendlinePosition);
                    switch (ineBlendType)
                    {
                        case AnimationBlendType::k_linear:
                            mCurrentAnimationData = LerpBetweenFrames(pFrame1, pFrame2, fFactor);
                            break;
                        default:
                            ERROR_LOG("Invalid animation blend type given.");
                            mCurrentAnimationData = pFrame1;
                            break;
                    }
                }
                else if (pFrame1 != NULL)
                {
                    mCurrentAnimationData = pFrame1;
                }
                else if (pFrame2 != NULL)
                {
                    mCurrentAnimationData = pFrame2;
                }
                else 
                {
                    ERROR_LOG("Something has gone wrong when blending animations.");
                    return;
                }
                mbPrepared = true;
            }
            else if (mAnimations.size() > 0) 
            {
                SkinnedAnimationPtr pAnim = mAnimations[0]->pSkinnedAnimation;
                mCurrentAnimationData = CalculateAnimationFrame(pAnim, infPlaybackPosition);
                mbPrepared = true;
            }
            else
            {
                ERROR_LOG("No animations attached to the animation group!");
				return;
            }
        }
        //----------------------------------------------------------
        /// Blend Group
        //----------------------------------------------------------
        void CSkinnedAnimationGroup::BlendGroup(AnimationBlendType ineBlendType, const SkinnedAnimationGroupPtr& inpAnimationGroup, f32 infBlendFactor)
        {
            switch (ineBlendType)
            {
                case AnimationBlendType::k_linear:
                    mCurrentAnimationData = LerpBetweenFrames(mCurrentAnimationData, inpAnimationGroup->mCurrentAnimationData, infBlendFactor);
                    break;
                default:
                    ERROR_LOG("Invalid animation blend type given.");
                    break;
            }
        }
        //----------------------------------------------------------
        /// Build Matrices
        //----------------------------------------------------------
        void CSkinnedAnimationGroup::BuildMatrices(s32 indwCurrentParent, const Core::CMatrix4x4& inParentMatrix)
        {
            const DYNAMIC_ARRAY<SkeletonNodePtr>& nodes = mpSkeleton->GetNodes();
			u32 currIndex = 0;
			for (DYNAMIC_ARRAY<SkeletonNodePtr>::const_iterator it = nodes.begin(); it != nodes.end(); ++it)
			{
				if ((*it)->mdwParentIndex == indwCurrentParent)
				{
					//get the world translation and orientation
					Core::CMatrix4x4 localMat;
                    if(mCurrentAnimationData->mNodeTranslations.empty() == false)
                    {
                        localMat.SetTransform(mCurrentAnimationData->mNodeTranslations[currIndex], mCurrentAnimationData->mNodeScalings[currIndex], mCurrentAnimationData->mNodeOrientations[currIndex]);
                    }
					
					//convert to matrix and store
					mCurrentAnimationMatrices[currIndex] =  localMat * inParentMatrix;
					
					//Apply to all children
					BuildMatrices(currIndex, mCurrentAnimationMatrices[currIndex]);
				}
                
				currIndex++;
			}
        }
        //----------------------------------------------------------
        /// Get Matrix At Index
        //----------------------------------------------------------
        const Core::CMatrix4x4& CSkinnedAnimationGroup::GetMatrixAtIndex(s32 indwIndex) const
        {
            if (indwIndex < mCurrentAnimationMatrices.size())
            {
                return mCurrentAnimationMatrices[indwIndex];
            }
            return Core::CMatrix4x4::IDENTITY;
        }
        //----------------------------------------------------------
        /// Apply Inverse Bind Pose
        //----------------------------------------------------------
        void CSkinnedAnimationGroup::ApplyInverseBindPose(const DYNAMIC_ARRAY<Core::CMatrix4x4>& inInverseBindPoseMatrices, DYNAMIC_ARRAY<Core::CMatrix4x4>& outCombinedMatrices)
        {
            const DYNAMIC_ARRAY<s32>& kadwJoints = mpSkeleton->GetJointIndices();
            
            outCombinedMatrices.clear();
            for (u32 i = 0; i < kadwJoints.size(); ++i)
            {
                outCombinedMatrices.push_back(Core::CMatrix4x4());
            }
            
            //check that they have the same number of joints
			if (kadwJoints.size() != inInverseBindPoseMatrices.size())
			{
				ERROR_LOG("Cannot apply bind pose matrices to joint matrices, becuase they are not from the same skeleton.");
			}
			
			//iterate through and multiply together to get the new array
			s32 count = 0;
			DYNAMIC_ARRAY<s32>::const_iterator joint = kadwJoints.begin();
			for (DYNAMIC_ARRAY<Core::CMatrix4x4>::const_iterator ibp = inInverseBindPoseMatrices.begin(); 
				 joint != kadwJoints.end() && ibp != inInverseBindPoseMatrices.end();)
			{
				//multiply together
				outCombinedMatrices[count] = ((*ibp) * (mCurrentAnimationMatrices[*joint]));
				
				//incriment the iterators
				++joint;
				++ibp;
				count++;
			}
        }
        //----------------------------------------------------------
        /// Get Animation Length
        //----------------------------------------------------------
        f32 CSkinnedAnimationGroup::GetAnimationLength()
        {
            CalculateAnimationLength();
            return mfAnimationLength;
        }
        //----------------------------------------------------------
        /// Get Animation Count
        //----------------------------------------------------------
        u32 CSkinnedAnimationGroup::GetAnimationCount() const
        {
            return mAnimations.size();
        }
        //----------------------------------------------------------
        /// Is Prepared
        //----------------------------------------------------------
        bool CSkinnedAnimationGroup::IsPrepared() const
        {
            return mbPrepared;
        }
        //----------------------------------------------------------
        /// Get Animation
        //----------------------------------------------------------
        void CSkinnedAnimationGroup::GetAnimations(DYNAMIC_ARRAY<SkinnedAnimationPtr>& outapSkinnedAnimationList)
        {
            for (DYNAMIC_ARRAY<AnimationItemPtr>::iterator it = mAnimations.begin(); it != mAnimations.end(); ++it)
            {
                outapSkinnedAnimationList.push_back((*it)->pSkinnedAnimation);
            }
        }
        //----------------------------------------------------------
        /// Calculate Animation Length
        //----------------------------------------------------------
        void CSkinnedAnimationGroup::CalculateAnimationLength()
        {
            if (mbAnimationLengthDirty)
            {
                mfAnimationLength = 0.0f;
                
                for (DYNAMIC_ARRAY<AnimationItemPtr>::iterator it = mAnimations.begin(); it != mAnimations.end(); ++it)
                {
                    SkinnedAnimationPtr pAnim = (*it)->pSkinnedAnimation;
                    f32 fAnimationLength = pAnim->GetFrameTime() * ((f32)(pAnim->GetNumFrames() - 1));
                    
                    if (mfAnimationLength != 0.0f && mfAnimationLength != fAnimationLength)
                    {
                        ERROR_LOG("All grouped animations must have the same length!");
                        mfAnimationLength = 0.0f;
                        return;
                    }
                    
                    mfAnimationLength = fAnimationLength;
                }
                
                mbAnimationLengthDirty = false;
            }
        }
        //-----------------------------------------------------------
        /// Destructor
        //-----------------------------------------------------------
        CSkinnedAnimationGroup::~CSkinnedAnimationGroup()
        {
        }
        //----------------------------------------------------------
        /// Calculate Animation Frame
        //----------------------------------------------------------
        SkinnedAnimationFramePtr CSkinnedAnimationGroup::CalculateAnimationFrame(const SkinnedAnimationPtr& inpAnimation, f32 infPlaybackPosition)
        {
            //report errors if the playback position provided does not make sense
            if (infPlaybackPosition < 0.0f)
            {
                ERROR_LOG("A playback position below 0 does not make sense.");
            }
            
            //calculate the two frame indices this is between
			f32 frames = infPlaybackPosition / inpAnimation->GetFrameTime();
			s32 dwFrameAIndex = (s32)floorf(frames);
			s32 dwFrameBIndex = (s32)ceilf(frames);
			
			//ensure the frame numbers make sense
            if(dwFrameAIndex < 0)
            {
                dwFrameAIndex = 0;
            }
            
            if(dwFrameAIndex >= inpAnimation->GetNumFrames())
            {
                dwFrameAIndex = inpAnimation->GetNumFrames() - 1;
            }
            
            if(dwFrameBIndex < 0)
            {
                dwFrameBIndex = 0;
            }
            
            if(dwFrameBIndex >= inpAnimation->GetNumFrames())
            {
                dwFrameBIndex = inpAnimation->GetNumFrames() - 1;
            }
            
			//get the frames
			SkinnedAnimationFramePtr frameA = inpAnimation->GetFrameAtIndex(dwFrameAIndex);
			SkinnedAnimationFramePtr frameB = inpAnimation->GetFrameAtIndex(dwFrameBIndex);
			
			//get the ratio of one frame to the next
			f32 interpFactor = (infPlaybackPosition - (dwFrameAIndex * inpAnimation->GetFrameTime())) / inpAnimation->GetFrameTime();
			
			//blend between frames
            return LerpBetweenFrames(frameA, frameB, interpFactor);
        }
        //--------------------------------------------------------------
        /// Lerp Between Frames
        //--------------------------------------------------------------
        SkinnedAnimationFramePtr CSkinnedAnimationGroup::LerpBetweenFrames(const SkinnedAnimationFramePtr& inFrameA, const SkinnedAnimationFramePtr& inFrameB, f32 infInterpFactor)
        {
            SkinnedAnimationFramePtr outFrame(new SkinnedAnimationFrame());
			
            if(inFrameA != NULL && inFrameB != NULL)
            {
                //iterate through each translation
                outFrame->mNodeTranslations.reserve(inFrameB->mNodeTranslations.size());
                DYNAMIC_ARRAY<Core::CVector3>::const_iterator transAIt = inFrameA->mNodeTranslations.begin();
                for (DYNAMIC_ARRAY<Core::CVector3>::const_iterator transBIt = inFrameB->mNodeTranslations.begin(); 
                     transAIt != inFrameA->mNodeTranslations.end() && transBIt != inFrameB->mNodeTranslations.end();)
                {
                    //lerp
                    Core::CVector3 newTrans = Core::CMathUtils::Lerp(infInterpFactor, *transAIt, *transBIt);
                    
                    //add to frame
                    outFrame->mNodeTranslations.push_back(newTrans);
                    
                    //incriment the iterators
                    ++transAIt;
                    ++transBIt;
                }
                
                //iterate through each orientation
                outFrame->mNodeOrientations.reserve(inFrameB->mNodeOrientations.size());
                DYNAMIC_ARRAY<Core::CQuaternion>::const_iterator orientAIt = inFrameA->mNodeOrientations.begin();
                for (DYNAMIC_ARRAY<Core::CQuaternion>::const_iterator orientBIt = inFrameB->mNodeOrientations.begin();
                     orientAIt != inFrameA->mNodeOrientations.end() && orientBIt != inFrameB->mNodeOrientations.end();)
                {
                    //lerp
                    Core::CQuaternion newOrient = Core::CQuaternion::Slerp(*orientAIt, *orientBIt, infInterpFactor);
                    
                    //add to frame
                    outFrame->mNodeOrientations.push_back(newOrient);
                    
                    //incriment the iterators
                    ++orientAIt;
                    ++orientBIt;
                }
                
                //iterate through each scale
                outFrame->mNodeScalings.reserve(inFrameB->mNodeScalings.size());
                DYNAMIC_ARRAY<Core::CVector3>::const_iterator scaleAIt = inFrameA->mNodeScalings.begin();
                for (DYNAMIC_ARRAY<Core::CVector3>::const_iterator scaleBIt = inFrameB->mNodeScalings.begin();
                     scaleAIt != inFrameA->mNodeScalings.end() && scaleBIt != inFrameB->mNodeScalings.end();)
                {
                    //lerp
                    Core::CVector3 newScale = Core::CMathUtils::Lerp(infInterpFactor, *scaleAIt, *scaleBIt);
                    
                    //add to frame
                    outFrame->mNodeScalings.push_back(newScale);
                    
                    //incriment the iterators
                    ++scaleAIt;
                    ++scaleBIt;
                }
            }
			
			return outFrame;
        }
    }
}

