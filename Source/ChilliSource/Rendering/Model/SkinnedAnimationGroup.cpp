//
//  SkinnedAnimationGroup.cpp
//  Chilli Source
//  Created by Ian Copland on 08/06/2012.
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
        SkinnedAnimationGroup::SkinnedAnimationGroup(const Skeleton* inpSkeleton)
        : mpSkeleton(inpSkeleton), mbAnimationLengthDirty(true), mfAnimationLength(0.0f), mbPrepared(false)
        {
            for (s32 i = 0; i < mpSkeleton->GetNumNodes(); ++i)
            {
                mCurrentAnimationMatrices.push_back(Core::Matrix4());
            }
        }
        //----------------------------------------------------------
        /// Attach Animation
        //----------------------------------------------------------
        void SkinnedAnimationGroup::AttachAnimation(const SkinnedAnimationCSPtr& inpAnimation, f32 infBlendlinePosition)
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
        void SkinnedAnimationGroup::DetatchAnimation(const SkinnedAnimationCSPtr& inpAnimation)
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
                SkinnedAnimation::FrameCUPtr pFrame1;
                if (pAnimItem1 != nullptr)
                    pFrame1 = CalculateAnimationFrame(pAnimItem1->pSkinnedAnimation, infPlaybackPosition);
                
                SkinnedAnimation::FrameCUPtr pFrame2;
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
                            mCurrentAnimationData = LerpBetweenFrames(pFrame1.get(), pFrame2.get(), fFactor);
                            break;
                        default:
                            CS_LOG_ERROR("Invalid animation blend type given.");
                            mCurrentAnimationData = std::move(pFrame1);
                            break;
                    }
                }
                else if (pFrame1 != nullptr)
                {
                    mCurrentAnimationData = std::move(pFrame1);
                }
                else if (pFrame2 != nullptr)
                {
                    mCurrentAnimationData = std::move(pFrame2);
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
                const SkinnedAnimationCSPtr& pAnim = mAnimations[0]->pSkinnedAnimation;
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
                    mCurrentAnimationData = LerpBetweenFrames(mCurrentAnimationData.get(), inpAnimationGroup->mCurrentAnimationData.get(), infBlendFactor);
                    break;
                default:
                    CS_LOG_ERROR("Invalid animation blend type given.");
                    break;
            }
        }
        //----------------------------------------------------------
        /// Build Matrices
        //----------------------------------------------------------
        void SkinnedAnimationGroup::BuildMatrices(s32 indwCurrentParent, const Core::Matrix4& inParentMatrix)
        {
            const std::vector<SkeletonNodeCUPtr>& nodes = mpSkeleton->GetNodes();
			u32 currIndex = 0;
			for (auto it = nodes.begin(); it != nodes.end(); ++it)
			{
				if ((*it)->mdwParentIndex == indwCurrentParent)
				{
					//get the world translation and orientation
					Core::Matrix4 localMat;
                    if(mCurrentAnimationData->m_nodeTranslations.empty() == false)
                    {
						localMat = Core::Matrix4::CreateTransform(mCurrentAnimationData->m_nodeTranslations[currIndex], mCurrentAnimationData->m_nodeScales[currIndex], mCurrentAnimationData->m_nodeOrientations[currIndex]);
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
        const Core::Matrix4& SkinnedAnimationGroup::GetMatrixAtIndex(s32 indwIndex) const
        {
            if (indwIndex < (s32)mCurrentAnimationMatrices.size())
            {
                return mCurrentAnimationMatrices[indwIndex];
            }
            return Core::Matrix4::k_identity;
        }
        //----------------------------------------------------------
        /// Apply Inverse Bind Pose
        //----------------------------------------------------------
        void SkinnedAnimationGroup::ApplyInverseBindPose(const std::vector<Core::Matrix4>& inInverseBindPoseMatrices, std::vector<Core::Matrix4>& outCombinedMatrices)
        {
            const std::vector<s32>& kadwJoints = mpSkeleton->GetJointIndices();
            
            outCombinedMatrices.clear();
            for (u32 i = 0; i < kadwJoints.size(); ++i)
            {
                outCombinedMatrices.push_back(Core::Matrix4());
            }
            
            //check that they have the same number of joints
			if (kadwJoints.size() != inInverseBindPoseMatrices.size())
			{
				CS_LOG_ERROR("Cannot apply bind pose matrices to joint matrices, because they are not from the same skeleton.");
			}
			
			//iterate through and multiply together to get the new array
			s32 count = 0;
			std::vector<s32>::const_iterator joint = kadwJoints.begin();
			for (std::vector<Core::Matrix4>::const_iterator ibp = inInverseBindPoseMatrices.begin(); 
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
        void SkinnedAnimationGroup::GetAnimations(std::vector<SkinnedAnimationCSPtr>& outapSkinnedAnimationList)
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
                    const SkinnedAnimationCSPtr& pAnim = (*it)->pSkinnedAnimation;
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
        //----------------------------------------------------------
        /// Calculate Animation Frame
        //----------------------------------------------------------
        SkinnedAnimation::FrameCUPtr SkinnedAnimationGroup::CalculateAnimationFrame(const SkinnedAnimationCSPtr& inpAnimation, f32 infPlaybackPosition)
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
            
            if(dwFrameAIndex >= (s32)inpAnimation->GetNumFrames())
            {
                dwFrameAIndex = inpAnimation->GetNumFrames() - 1;
            }
            
            if(dwFrameBIndex < 0)
            {
                dwFrameBIndex = 0;
            }
            
			if (dwFrameBIndex >= (s32)inpAnimation->GetNumFrames())
            {
                dwFrameBIndex = inpAnimation->GetNumFrames() - 1;
            }
            
			//get the frames
			const SkinnedAnimation::Frame* frameA = inpAnimation->GetFrameAtIndex(dwFrameAIndex);
			const SkinnedAnimation::Frame* frameB = inpAnimation->GetFrameAtIndex(dwFrameBIndex);
			
			//get the ratio of one frame to the next
			f32 interpFactor = (infPlaybackPosition - (dwFrameAIndex * inpAnimation->GetFrameTime())) / inpAnimation->GetFrameTime();
			
			//blend between frames
            return LerpBetweenFrames(frameA, frameB, interpFactor);
        }
        //--------------------------------------------------------------
        /// Lerp Between Frames
        //--------------------------------------------------------------
        SkinnedAnimation::FrameCUPtr SkinnedAnimationGroup::LerpBetweenFrames(const SkinnedAnimation::Frame* inFrameA, const SkinnedAnimation::Frame* inFrameB, f32 infInterpFactor)
        {
            SkinnedAnimation::FrameUPtr outFrame(new SkinnedAnimation::Frame());
			
            if(inFrameA != nullptr && inFrameB != nullptr)
            {
                //iterate through each translation
                outFrame->m_nodeTranslations.reserve(inFrameB->m_nodeTranslations.size());
                std::vector<Core::Vector3>::const_iterator transAIt = inFrameA->m_nodeTranslations.begin();
                for (std::vector<Core::Vector3>::const_iterator transBIt = inFrameB->m_nodeTranslations.begin();
                     transAIt != inFrameA->m_nodeTranslations.end() && transBIt != inFrameB->m_nodeTranslations.end();)
                {
                    //lerp
                    Core::Vector3 newTrans = Core::MathUtils::Lerp(infInterpFactor, *transAIt, *transBIt);
                    
                    //add to frame
                    outFrame->m_nodeTranslations.push_back(newTrans);
                    
                    //incriment the iterators
                    ++transAIt;
                    ++transBIt;
                }
                
                //iterate through each orientation
                outFrame->m_nodeOrientations.reserve(inFrameB->m_nodeOrientations.size());
                std::vector<Core::Quaternion>::const_iterator orientAIt = inFrameA->m_nodeOrientations.begin();
                for (std::vector<Core::Quaternion>::const_iterator orientBIt = inFrameB->m_nodeOrientations.begin();
                     orientAIt != inFrameA->m_nodeOrientations.end() && orientBIt != inFrameB->m_nodeOrientations.end();)
                {
                    //lerp
                    Core::Quaternion newOrient = Core::Quaternion::Slerp(*orientAIt, *orientBIt, infInterpFactor);
                    
                    //add to frame
                    outFrame->m_nodeOrientations.push_back(newOrient);
                    
                    //incriment the iterators
                    ++orientAIt;
                    ++orientBIt;
                }
                
                //iterate through each scale
                outFrame->m_nodeScales.reserve(inFrameB->m_nodeScales.size());
                std::vector<Core::Vector3>::const_iterator scaleAIt = inFrameA->m_nodeScales.begin();
                for (std::vector<Core::Vector3>::const_iterator scaleBIt = inFrameB->m_nodeScales.begin();
                     scaleAIt != inFrameA->m_nodeScales.end() && scaleBIt != inFrameB->m_nodeScales.end();)
                {
                    //lerp
                    Core::Vector3 newScale = Core::MathUtils::Lerp(infInterpFactor, *scaleAIt, *scaleBIt);
                    
                    //add to frame
                    outFrame->m_nodeScales.push_back(newScale);
                    
                    //incriment the iterators
                    ++scaleAIt;
                    ++scaleBIt;
                }
            }
			
			return SkinnedAnimation::FrameCUPtr(std::move(outFrame));
        }
    }
}

