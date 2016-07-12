//
//  The MIT License (MIT)
//
//  Copyright (c) 2016 Tag Games Limited
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

#ifndef _CHILLISOURCE_RENDERING_MODEL_RENDERSKINNEDANIMATION_H_
#define _CHILLISOURCE_RENDERING_MODEL_RENDERSKINNEDANIMATION_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Math/Vector4.h>
#include <ChilliSource/Core/Memory/SharedPtr.h>
#include <ChilliSource/Core/Memory/UniquePtr.h>
#include <ChilliSource/Rendering/Model/VertexFormat.h>

namespace ChilliSource
{
    /// A standard-layout container for information which the renderer needs pertaining to a
    /// single skinned animation.
    ///
    /// This is immutable and therefore thread-safe.
    ///
    class RenderSkinnedAnimation final
    {
    public:
        CS_DECLARE_NOCOPY(RenderSkinnedAnimation);
        
        /// Creates a new instance with the given skinned animation joint data.
        ///
        /// @param jointData
        ///     The combined inverse bind pose and joint frame data matrixes. The matrices are
        ///     broken up into a collection of Vector4, so the final column, which is always
        ///     [0, 0, 0, 1] can be omitted.
        /// @param jointDataSize
        ///     The number of Vector4's that comprise the joint data.
        ///
        RenderSkinnedAnimation(UniquePtr<Vector4[]> jointData, u32 jointDataSize) noexcept;
        
        /// @return The combined inverse bind pose and joint frame data matrixes. The matrices are
        ///     broken up into a collection of Vector4, so the final column, which is always
        ///     [0, 0, 0, 1] can be omitted.
        ///
        const Vector4* GetJointData() const noexcept { return m_jointData.get(); }
        
        /// @return The number of Vector4's that comprise the joint data.
        ///
        u32 GetJointDataSize() const noexcept { return m_jointDataSize; }
        
    private:
        friend class RenderMeshManager;

        UniquePtr<Vector4[]> m_jointData;
        u32 m_jointDataSize;
    };
    
    using RenderSkinnedAnimationAUPtr = UniquePtr<RenderSkinnedAnimation>;
    using RenderSkinnedAnimationASPtr = SharedPtr<RenderSkinnedAnimation>;
}

#endif
