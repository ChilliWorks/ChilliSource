//
//  SkinnedAnimationGroup.h
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

#ifndef _CHILLISOURCE_RENDERING_SKINNEDANIMATIONGROUP_H_
#define _CHILLISOURCE_RENDERING_SKINNEDANIMATIONGROUP_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/File/FileSystem.h>
#include <ChilliSource/Core/Math/Matrix4.h>
#include <ChilliSource/Rendering/Model/SkinnedAnimation.h>

namespace ChilliSource
{
    namespace Rendering
    {
        //===============================================================
		/// Animation Blend Type
		///
		/// Describes the different types of animation blend.
		//===============================================================
        enum class AnimationBlendType
        {
            k_linear
        };
        //===============================================================
		/// Skinned Animation Group
		///
		/// A group of animations that can be blended.
		//===============================================================
        class SkinnedAnimationGroup final
        {
        public:
            
            CS_DECLARE_NOCOPY(SkinnedAnimationGroup);
            
            //-----------------------------------------------------------
            /// Constructor
            //-----------------------------------------------------------
            SkinnedAnimationGroup(const Skeleton* inpSkeleton);
            //----------------------------------------------------------
			/// Attach Animation
			///
			/// Attaches an animation.
			///
            /// @param The animation pointer.
            /// @param the position of the animation on the blendline.
			//----------------------------------------------------------
			void AttachAnimation(const SkinnedAnimationCSPtr& inpAnimation, f32 infBlendlinePosition);
            //----------------------------------------------------------
			/// Detatch Animation
			///
			/// Removes an animation.
			///
            /// @param The animation pointer.
			//----------------------------------------------------------
			void DetatchAnimation(const SkinnedAnimationCSPtr& inpAnimation);
            //----------------------------------------------------------
			/// Clear Animations
			///
			/// Clears all animations
			//----------------------------------------------------------
			void ClearAnimations();
            //----------------------------------------------------------
			/// Build Animation Data
            ///
            /// Builds a new set of animation data with the given
            /// parameters
            ///
            /// @param The blend type.
            /// @param the playback position.
            /// @param the blendline position.
			//----------------------------------------------------------
			void BuildAnimationData(AnimationBlendType ineBlendType, f32 infPlaybackPosition, f32 infBlendlinePosition);
            //----------------------------------------------------------
			/// Blend Group
            ///
            /// Blends between another group and this.
            ///
            /// @param The fade type.
            /// @param the playback position.
            /// @param the blendline position.
			//----------------------------------------------------------
			void BlendGroup(AnimationBlendType ineBlendType, const SkinnedAnimationGroupSPtr& inpAnimationGroup, f32 infBlendFactor);
            //----------------------------------------------------------
			/// Build Matrices
            ///
            /// Builds the animation matrix data from the current
            /// animation data.
            ///
            /// @param the current parent.
            /// @param the parent matrix.
			//----------------------------------------------------------
			void BuildMatrices(s32 indwCurrentParent = -1, const Core::Matrix4& inParentMatrix = Core::Matrix4());
            //----------------------------------------------------------
			/// Get Matrix At Index
            ///
            /// @param the index of the required matrix.
            /// @return the matrix.
			//----------------------------------------------------------
            const Core::Matrix4& GetMatrixAtIndex(s32 indwIndex) const;
            //----------------------------------------------------------
			/// Apply Inverse Bind Pose
            ///
            /// outputs a copy of the current animation matrix data with
            /// the inverse bind pose matrices applied.
            ///
            /// @param the inverse bind pose matrices.
            /// @param OUT: The combined matrices.
			//----------------------------------------------------------
			void ApplyInverseBindPose(const std::vector<Core::Matrix4>& inInverseBindPoseMatrices, std::vector<Core::Matrix4>& outCombinedMatrices);
            //----------------------------------------------------------
			/// Get Animation Length
            ///
            /// @return the length of the animation in seconds.
			//----------------------------------------------------------
			f32 GetAnimationLength();
            //----------------------------------------------------------
			/// Get Animation Count
            ///
            /// @return the number of attached animations.
			//----------------------------------------------------------
			u32 GetAnimationCount() const;
            //----------------------------------------------------------
			/// Is Prepared
            ///
            /// @return whether or not the group is ready for being used
            /// in rendering;
			//----------------------------------------------------------
			bool IsPrepared() const;
            //----------------------------------------------------------
			/// Get Animations
            ///
            /// @param OUT: The list of animations.
			//----------------------------------------------------------
			void GetAnimations(std::vector<SkinnedAnimationCSPtr>& outapSkinnedAnimationList);
        private:
            //----------------------------------------------------------
            /// Animation Item
            ///
            /// A single animation item for containing in an animation group.
            //----------------------------------------------------------
            struct AnimationItem
            {
                SkinnedAnimationCSPtr pSkinnedAnimation;
                f32 fBlendlinePosition;
            };
            typedef std::shared_ptr<AnimationItem> AnimationItemPtr;
            //----------------------------------------------------------
			/// Calculate Animation Length
            ///
            /// calculates the length of the animation group and ensures
            /// all attached animations are of the same length.
			//----------------------------------------------------------
            void CalculateAnimationLength();
            //----------------------------------------------------------
			/// Calculate Animation Frame
            ///
            /// Gets the frame data from a single animation.
            ///
            /// @param The blend type.
            /// @param the animation.
            /// @param the playback position.
			//----------------------------------------------------------
            SkinnedAnimation::FrameCUPtr CalculateAnimationFrame(const SkinnedAnimationCSPtr& inpAnimation, f32 infPlaybackPosition);
            //--------------------------------------------------------------
			/// Lerp Between Frames
			///
			/// Linearly interpolates between two animation frames.
			///
			/// @param frame 1
			/// @param frame 2
			/// @param the interpolation factor
			/// @return output interpolated frame.
			//--------------------------------------------------------------
            SkinnedAnimation::FrameCUPtr LerpBetweenFrames(const SkinnedAnimation::Frame* inFrameA, const SkinnedAnimation::Frame* inFrameB, f32 infInterpFactor);
            
            const Skeleton* mpSkeleton;
            std::vector<AnimationItemPtr> mAnimations;
            SkinnedAnimation::FrameCUPtr mCurrentAnimationData;
            std::vector<Core::Matrix4> mCurrentAnimationMatrices;
            bool mbAnimationLengthDirty;
            f32 mfAnimationLength;
            bool mbPrepared;
        };
    }
}

#endif
