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
        SkinnedAnimationGroup::SkinnedAnimationGroup(const SkeletonSPtr& inpSkeleton)
        : mpSkeleton(inpSkeleton), mbAnimationLengthDirty(true), mfAnimationLength(0.0f), mbPrepared(false)
        {
            for (u32 i = 0; i < mpSkeleton->GetNumNodes(); ++i)
            {
                mCurrentAnimationMatrices.push_back(Core::Matrix4x4());
            }
        }
        //----------------------------------------------------------
        /// Attach Animation
        //----------------------------------------------------------
        void SkinnedAnimationGroup::AttachAnimation(const SkinnedAnimationSPtr& inpAnimation, f32 infBlendlinePosition)
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
        void SkinnedAnimationGroup::DetatchAnimation(const SkinnedAnimationSPtr& inpAnimation)
        {
            mbAnimationLengthDirty = true;
            for (std::vector<AnimationItemPtr>::iterator it = mAnimations.begin(); it != mAnimations.end(); ++it)
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
        void SkinnedAnimationGroup::ClearAnimations()
        {
            mAnimations.clear();
        }
        //----------------------------------------------------------
        /// Build Animation Data
        //----------------------------------------------------------
        void SkinnedAnimationGroup::BuildAnimationData(AnimationBlendType ineBlendType, f32 infPlaybackPosition, f32 infBlendlinePosition)
        {
            //check how many animations we have. if we only have 1 then dont try and blend. If we have none then error.
            if (mAnimations.size() > 1)
            {
                //find which two animations should be blended together
                AnimationItemPtr pAnimItem1;
                AnimationItemPtr pAnimItem2;
                for (std::vector<AnimationItemPtr>::iterator it = mAnimations.begin(); it != mAnimations.end(); ++it)
                {
                    f32 fBlendlinePosition = (*it)->fBlendlinePosition;
                    if (fBlendlinePosition <= infBlendlinePosition && (pAnimItem1 == nullptr || fBlendlinePosition > pAnimItem1->fBlendlinePosition))
                    {
                        pAnimItem1 = (*it);
                    }
                    if (fBlendlinePosition >= infBlendlinePosition && (pAnimItem2 == nullptr || fBlendlinePosition < pAnimItem2->fBlendlinePosition))
                    {
                        pAnimItem2 = (*it);
                    }
                }
                
                //get the animation frames
                SkinnedAnimationFrameSPtr pFrame1;
                if (pAnimItem1 != nullptr)
                    pFrame1 = CalculateAnimationFrame(pAnimItem1->pSkinnedAnimation, infPlaybackPosition);
                
                SkinnedAnimationFrameSPtr pFrame2;
                if (pAnimItem2 != nullptr)
                    pFrame2 = CalculateAnimationFrame(pAnimItem2->pSkinnedAnimation, infPlaybackPosition);
                
                //check that we do indeed have two animations to blend. if not, just return the frame we do have.
                if (pFrame1 != nullptr && pFrame2 != nullptr && pAnimItem1.get() != pAnimItem2.get())
                {
                    //get the interpolation factor and then apply the requested blend to the two frames.
                    f32 fFactor = (infBlendlinePosition - pAnimItem1->fBlendlinePosition) / (pAnimItem2->fBlendlinePosition - pAnimItem1->fBlendlinePosition);
                    switch (ineBlendType)
                    {
                        case AnimationBlendType::k_linear:
                            mCurrentAnimationData = LerpBetweenFrames(pFrame1, pFrame2, fFactor);
                            break;
                        default:
                            CS_LOG_ERROR("Invalid animation blend type given.");
                            mCurrentAnimationData = pFrame1;
                            break;
                    }
                }
                else if (pFrame1 != nullptr)
                {
                    mCurrentAnimationData = pFrame1;
                }
                else if (pFrame2 != nullptr)
                {
                    mCurrentAnimationData = pFrame2;
                }
                else 
                {
                    CS_LOG_ERROR("Something has gone wrong when blending animations.");
                    return;
                }
                mbPrepared = true;
            }
            else if (mAnimations.size() > 0) 
            {
                SkinnedAnimationSPtr pAnim = mAnimations[0]->pSkinnedAnimation;
                mCurrentAnimationData = CalculateAnimationFrame(pAnim, infPlaybackPosition);
                mbPrepared = true;
            }
            else
            {
                CS_LOG_ERROR("No animations attached to the animation group!");
				return;
            }
        }
        //----------------------------------------------------------
        /// Blend Group
        //----------------------------------------------------------
        void SkinnedAnimationGroup::BlendGroup(AnimationBlendType ineBlendType, const SkinnedAnimationGroupSPtr& inpAnimationGroup, f32 infBlendFactor)
        {
            switch (ineBlendType)
            {
                case AnimationBlendType::k_linear:
                    mCurrentAnimationData = LerpBetweenFrames(mCurrentAnimationData, inpAnimationGroup->mCurrentAnimationData, infBlendFactor);
                    break;
                default:
                    CS_LOG_ERROR("Invalid animation blend type given.");
                    break;
            }
        }
        //----------------------------------------------------------
        /// Build Matrices
        //----------------------------------------------------------
        void SkinnedAnimationGroup::BuildMatrices(s32 indwCurrentParent, const Core::Matrix4x4& inParentMatrix)
        {
            const std::vector<SkeletonNodeSPtr>& nodes = mpSkeleton->GetNodes();
			u32 currIndex = 0;
			for (std::vector<SkeletonNodeSPtr>::const_iterator it = nodes.begin(); it != nodes.end(); ++it)
			{
				if ((*it)->mdwParentIndex == indwCurrentParent)
				{
					//get the world translation and orientation
					Core::Matrix4x4 localMat;
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
        const Core::Matrix4x4& SkinnedAnimationGroup::GetMatrixAtIndex(s32 indwIndex) const
        {
            if (indwIndex < mCurrentAnimationMatrices.size())
            {
                return mCurrentAnimationMatrices[indwIndex];
            }
            return Core::Matrix4x4::IDENTITY;
        }
        //----------------------------------------------------------
        /// Apply Inverse Bind Pose
        //----------------------------------------------------------
        void SkinnedAnimationGroup::ApplyInverseBindPose(const std::vector<Core::Matrix4x4>& inInverseBindPoseMatrices, std::vector<Core::Matrix4x4>& outCombinedMatrices)
        {
            const std::vector<s32>& kadwJoints = mpSkeleton->GetJointIndices();
            
            outCombinedMatrices.clear();
            for (u32 i = 0; i < kadwJoints.size(); ++i)
            {
                outCombinedMatrices.push_back(Core::Matrix4x4());
            }
            
            //check that they have the same number of joints
			if (kadwJoints.size() != inInverseBindPoseMatrices.size())
			{
				CS_LOG_ERROR("Cannot apply bind pose matrices to joint matrices, becuase they are not from the same skeleton.");
			}
			
			//iterate through and multiply together to get the new array
			s32 count = 0;
			std::vector<s32>::const_iterator joint = kadwJoints.begin();
			for (std::vector<Core::Matrix4x4>::const_iterator ibp = inInverseBindPoseMatrices.begin(); 
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
        f32 SkinnedAnimationGroup::GetAnimationLength()
        {
            CalculateAnimationLength();
            return mfAnimationLength;
        }
        //----------------------------------------------------------
        /// Get Animation Count
        //----------------------------------------------------------
        u32 SkinnedAnimationGroup::GetAnimationCount() const
        {
            return mAnimations.size();
        }
        //----------------------------------------------------------
        /// Is Prepared
        //----------------------------------------------------------
        bool SkinnedAnimationGroup::IsPrepared() const
        {
            return mbPrepared;
        }
        //----------------------------------------------------------
        /// Get Animation
        //----------------------------------------------------------
        void SkinnedAnimationGroup::GetAnimations(std::vector<SkinnedAnimationSPtr>& outapSkinnedAnimationList)
        {
            for (std::vector<AnimationItemPtr>::iterator it = mAnimations.begin(); it != mAnimations.end(); ++it)
            {
                outapSkinnedAnimationList.push_back((*it)->pSkinnedAnimation);
            }
        }
        //----------------------------------------------------------
        /// Calculate Animation Length
        //----------------------------------------------------------
        void SkinnedAnimationGroup::CalculateAnimationLength()
        {
            if (mbAnimationLengthDirty)
            {
                mfAnimationLength = 0.0f;
                
                for (std::vector<AnimationItemPtr>::iterator it = mAnimations.begin(); it != mAnimations.end(); ++it)
                {
                    SkinnedAnimationSPtr pAnim = (*it)->pSkinnedAnimation;
                    f32 fAnimationLength = pAnim->GetFrameTime() * ((f32)(pAnim->GetNumFrames() - 1));
                    
                    if (mfAnimationLength != 0.0f && mfAnimationLength != fAnimationLength)
                    {
                        CS_LOG_ERROR("All grouped animations must have the same length!");
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
        SkinnedAnimationGroup::~SkinnedAnimationGroup()
        {
        }
        //----------------------------------------------------------
        /// Calculate Animation Frame
        //----------------------------------------------------------
        SkinnedAnimationFrameSPtr SkinnedAnimationGroup::CalculateAnimationFrame(const SkinnedAnimationSPtr& inpAnimation, f32 infPlaybackPosition)
        {
            //report errors if the playback position provided does not make sense
            if (infPlaybackPosition < 0.0f)
            {
                CS_LOG_ERROR("A playback position below 0 does not make sense.");
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
			SkinnedAnimationFrameSPtr frameA = inpAnimation->GetFrameAtIndex(dwFrameAIndex);
			SkinnedAnimationFrameSPtr frameB = inpAnimation->GetFrameAtIndex(dwFrameBIndex);
			
			//get the ratio of one frame to the next
			f32 interpFactor = (infPlaybackPosition - (dwFrameAIndex * inpAnimation->GetFrameTime())) / inpAnimation->GetFrameTime();
			
			//blend between frames
            return LerpBetweenFrames(frameA, frameB, interpFactor);
        }
        //--------------------------------------------------------------
        /// Lerp Between Frames
        //--------------------------------------------------------------
        SkinnedAnimationFrameSPtr SkinnedAnimationGroup::LerpBetweenFrames(const SkinnedAnimationFrameSPtr& inFrameA, const SkinnedAnimationFrameSPtr& inFrameB, f32 infInterpFactor)
        {
            SkinnedAnimationFrameSPtr outFrame(new SkinnedAnimationFrame());
			
            if(inFrameA != nullptr && inFrameB != nullptr)
            {
                //iterate through each translation
                outFrame->mNodeTranslations.reserve(inFrameB->mNodeTranslations.size());
                std::vector<Core::Vector3>::const_iterator transAIt = inFrameA->mNodeTranslations.begin();
                for (std::vector<Core::Vector3>::const_iterator transBIt = inFrameB->mNodeTranslations.begin(); 
                     transAIt != inFrameA->mNodeTranslations.end() && transBIt != inFrameB->mNodeTranslations.end();)
                {
                    //lerp
                    Core::Vector3 newTrans = Core::MathUtils::Lerp(infInterpFactor, *transAIt, *transBIt);
                    
                    //add to frame
                    outFrame->mNodeTranslations.push_back(newTrans);
                    
                    //incriment the iterators
                    ++transAIt;
                    ++transBIt;
                }
                
                //iterate through each orientation
                outFrame->mNodeOrientations.reserve(inFrameB->mNodeOrientations.size());
                std::vector<Core::Quaternion>::const_iterator orientAIt = inFrameA->mNodeOrientations.begin();
                for (std::vector<Core::Quaternion>::const_iterator orientBIt = inFrameB->mNodeOrientations.begin();
                     orientAIt != inFrameA->mNodeOrientations.end() && orientBIt != inFrameB->mNodeOrientations.end();)
                {
                    //lerp
                    Core::Quaternion newOrient = Core::Quaternion::Slerp(*orientAIt, *orientBIt, infInterpFactor);
                    
                    //add to frame
                    outFrame->mNodeOrientations.push_back(newOrient);
                    
                    //incriment the iterators
                    ++orientAIt;
                    ++orientBIt;
                }
                
                //iterate through each scale
                outFrame->mNodeScalings.reserve(inFrameB->mNodeScalings.size());
                std::vector<Core::Vector3>::const_iterator scaleAIt = inFrameA->mNodeScalings.begin();
                for (std::vector<Core::Vector3>::const_iterator scaleBIt = inFrameB->mNodeScalings.begin();
                     scaleAIt != inFrameA->mNodeScalings.end() && scaleBIt != inFrameB->mNodeScalings.end();)
                {
                    //lerp
                    Core::Vector3 newScale = Core::MathUtils::Lerp(infInterpFactor, *scaleAIt, *scaleBIt);
                    
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

