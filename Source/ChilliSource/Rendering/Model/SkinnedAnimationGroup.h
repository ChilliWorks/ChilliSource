//
//  SkinnedAnimationGroup.h
//  moFlow
//
//  Created by Ian Copland on 08/06/2012.
//  Copyright (c) 2012 Tag Games Ltd. All rights reserved.
//

#ifndef MOFLOW_RENDERING_SKINNEDANIMATIONGROUP_H_
#define MOFLOW_RENDERING_SKINNEDANIMATIONGROUP_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/File/FileSystem.h>
#include <ChilliSource/Core/Math/Matrix4x4.h>

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
        class SkinnedAnimationGroup
        {
        public:
            //-----------------------------------------------------------
            /// Constructor
            //-----------------------------------------------------------
            SkinnedAnimationGroup(const SkeletonPtr& inpSkeleton);
            //----------------------------------------------------------
			/// Attach Animation
			///
			/// Attaches an animation.
			///
            /// @param The animation pointer.
            /// @param the position of the animation on the blendline.
			//----------------------------------------------------------
			void AttachAnimation(const SkinnedAnimationPtr& inpAnimation, f32 infBlendlinePosition);
            //----------------------------------------------------------
			/// Detatch Animation
			///
			/// Removes an animation.
			///
            /// @param The animation pointer.
			//----------------------------------------------------------
			void DetatchAnimation(const SkinnedAnimationPtr& inpAnimation);
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
			void BlendGroup(AnimationBlendType ineBlendType, const SkinnedAnimationGroupPtr& inpAnimationGroup, f32 infBlendFactor);
            //----------------------------------------------------------
			/// Build Matrices
            ///
            /// Builds the animation matrix data from the current
            /// animation data.
            ///
            /// @param the current parent.
            /// @param the parent matrix.
			//----------------------------------------------------------
			void BuildMatrices(s32 indwCurrentParent = -1, const Core::CMatrix4x4& inParentMatrix = Core::CMatrix4x4());
            //----------------------------------------------------------
			/// Get Matrix At Index
            ///
            /// @param the index of the required matrix.
            /// @return the matrix.
			//----------------------------------------------------------
            const Core::CMatrix4x4& GetMatrixAtIndex(s32 indwIndex) const;
            //----------------------------------------------------------
			/// Apply Inverse Bind Pose
            ///
            /// outputs a copy of the current animation matrix data with
            /// the inverse bind pose matrices applied.
            ///
            /// @param the inverse bind pose matrices.
            /// @param OUT: The combined matrices.
			//----------------------------------------------------------
			void ApplyInverseBindPose(const std::vector<Core::CMatrix4x4>& inInverseBindPoseMatrices, std::vector<Core::CMatrix4x4>& outCombinedMatrices);
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
			void GetAnimations(std::vector<SkinnedAnimationPtr>& outapSkinnedAnimationList);
            //-----------------------------------------------------------
            /// Destructor
            //-----------------------------------------------------------
            ~SkinnedAnimationGroup();
        private:
            //----------------------------------------------------------
            /// Animation Item
            ///
            /// A single animation item for containing in an animation group.
            //----------------------------------------------------------
            struct AnimationItem
            {
                SkinnedAnimationPtr pSkinnedAnimation;
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
			SkinnedAnimationFramePtr CalculateAnimationFrame(const SkinnedAnimationPtr& inpAnimation, f32 infPlaybackPosition);
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
			SkinnedAnimationFramePtr LerpBetweenFrames(const SkinnedAnimationFramePtr& inFrameA, const SkinnedAnimationFramePtr& inFrameB, f32 infInterpFactor);
            
            SkeletonPtr mpSkeleton;
            std::vector<AnimationItemPtr> mAnimations;
            SkinnedAnimationFramePtr mCurrentAnimationData;
            std::vector<Core::CMatrix4x4> mCurrentAnimationMatrices;
            bool mbAnimationLengthDirty;
            f32 mfAnimationLength;
            bool mbPrepared;
        };
    }
}

#endif
