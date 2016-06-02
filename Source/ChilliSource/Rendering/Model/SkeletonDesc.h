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

#ifndef _CHILLISOURCE_RENDERING_MODEL_SKELETONDESC_H_
#define _CHILLISOURCE_RENDERING_MODEL_SKELETONDESC_H_

#include <ChilliSource/ChilliSource.h>

#include <vector>

namespace ChilliSource
{
    /// A description of a skeleton.
    ///
    /// This is immutable and therefore thread safe.
    ///
    class SkeletonDesc final
    {
    public:
        SkeletonDesc() = default;
        
        /// Instantiates a new skeleton description with the given node data. Each of the
        /// lists must be identical in length.
        ///
        /// @param nodeNames
        ///     A list containing the names of each node.
        /// @param parentNodeIndices
        ///     A list containing the parent index of each node.
        /// @param jointIndices
        ///     A list containing the joint indices.
        ///
        SkeletonDesc(const std::vector<std::string>& nodeNames, const std::vector<s32>& parentNodeIndices, const std::vector<s32>& jointIndices) noexcept;
        
        /// @return A list containing the names of each node.
        ///
        const std::vector<std::string>& GetNodeNames() const noexcept { return m_nodeNames; }
        
        /// @return A list containing the parent index of each node.
        ///
        const std::vector<s32>& GetParentNodeIndices() const noexcept { return m_parentNodeIndices; }
        
        /// @return A list containing the joint indices.
        ///
        const std::vector<s32>& GetJointIndices() const noexcept { return m_jointIndices; }
        
    private:
        std::vector<std::string> m_nodeNames;
        std::vector<s32> m_parentNodeIndices;
        std::vector<s32> m_jointIndices;
    };
}

#endif
