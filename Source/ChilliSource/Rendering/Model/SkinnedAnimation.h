//
//  SkinnedAnimation.h
//  Chilli Source
//  Created by Ian Copland on 17/10/2011.
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

#ifndef _CHILLISOURCE_RENDERING_MODEL_SKINNEDANIMATION_H_
#define _CHILLISOURCE_RENDERING_MODEL_SKINNEDANIMATION_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Math/Matrix4.h>
#include <ChilliSource/Core/Math/Quaternion.h>
#include <ChilliSource/Core/Math/Vector3.h>
#include <ChilliSource/Core/Resource/Resource.h>

namespace ChilliSource
{
	namespace Rendering
	{
		//---------------------------------------------------------------------
		/// A resource that holds data for a single skinned animation.
        /// This largely consists of the transformations for each key frame.
        ///
        /// @author Ian Copland
		//---------------------------------------------------------------------
		class SkinnedAnimation final : public Core::Resource
		{
		public:
            
            //---------------------------------------------------------------------
            /// Contains all relevant data for a single frame of animation.
            ///
            /// @author Ian Copland
            //---------------------------------------------------------------------
            struct Frame
            {
                std::vector<Core::Vector3> m_nodeTranslations;
                std::vector<Core::Quaternion> m_nodeOrientations;
                std::vector<Core::Vector3> m_nodeScales;
            };
            using FrameUPtr = std::unique_ptr<Frame>;
            using FrameSPtr = std::shared_ptr<Frame>;
            using FrameCUPtr = std::unique_ptr<const Frame>;
            using FrameCSPtr = std::shared_ptr<const Frame>;
            
			CS_DECLARE_NAMEDTYPE(SkinnedAnimation);
			
			//---------------------------------------------------------------------
			/// @author Ian Copland
			///
            /// @param Interface ID type
            ///
			/// @return Whether this object is of given type
			//---------------------------------------------------------------------
			bool IsA(Core::InterfaceIDType in_interfaceId) const override;
			//---------------------------------------------------------------------
			/// @author Ian Copland
			///
			/// @param the index to the frame
			/// @return the AnimationFramePtr at the given index
			//---------------------------------------------------------------------
			const SkinnedAnimation::Frame* GetFrameAtIndex(u32 in_index) const;
			//---------------------------------------------------------------------
			/// @author Ian Copland
			///
			/// @return the time between frames in seconds
			//---------------------------------------------------------------------
			f32 GetFrameTime() const;
			//---------------------------------------------------------------------
			/// @author Ian Copland
			///
			/// @return the number of frames in the animation.
			//---------------------------------------------------------------------
			u32 GetNumFrames() const;
            //---------------------------------------------------------------------
			/// Adds a new frame to the end of the animation.
            ///
            /// @author Ian Copland
			///
			/// @param The new frame which is no owned by the resource
			//---------------------------------------------------------------------
			void AddFrame(SkinnedAnimation::FrameCUPtr in_frame);
			//---------------------------------------------------------------------
			/// Sets the frame rate of the animation. Do not use this for changing
			/// the speed of an animation. Instead changing the speed through
			/// the animated component.
            ///
            /// @author Ian Copland
			///
			/// @param The time between frames in seconds
			//---------------------------------------------------------------------
			void SetFrameTime(f32 in_timeBetweenFrames);
            
		private:
            
            friend class Core::ResourcePool;
            //---------------------------------------------------------------------
            /// Factory method for creating mesh resource instance. Only accessed
            /// by the resource pool
            ///
            /// @author S Downie
            ///
            /// @return Ownership of new instance
            //---------------------------------------------------------------------
            static SkinnedAnimationUPtr Create();
			//---------------------------------------------------------------------
			/// Declared private so that this can only be created via the create
            /// method
            ///
            /// @author Ian Copland
			//---------------------------------------------------------------------
			SkinnedAnimation();
            
		private:
			
			f32 m_frameTime;
			std::vector<SkinnedAnimation::FrameCUPtr> m_frames;
		};
	}
}

#endif