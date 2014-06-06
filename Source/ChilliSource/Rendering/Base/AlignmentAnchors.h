//
//  AlignmentAnchors.h
//  Chilli Source
//  Created by Scott Downie on 22/04/2011.
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


#ifndef _MO_FLO_CORE_ALIGNMENT_ANCHORS_H_
#define _MO_FLO_CORE_ALIGNMENT_ANCHORS_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Math/Vector2.h>

namespace ChilliSource
{
    namespace Rendering
    {
        //---Anchor points for view alignment
        enum class AlignmentAnchor
        {
            k_topLeft,
            k_topRight,
            k_topCentre,
            k_middleLeft,
            k_middleRight,
            k_middleCentre,
            k_bottomLeft,
            k_bottomRight,
            k_bottomCentre
        };
        
        //--------------------------------------------------------------
        /// Align
        ///
        /// Calculate the alignment anchor point based on the anchor
        /// and the size
        ///
        /// @param Anchor type 
        /// @param Half size
        /// @param Out anchor point
        //---------------------------------------------------------------
        void Align(AlignmentAnchor ineAlignment, const Core::Vector2& invHalfSize, Core::Vector2& outvAlignmentPoint);
        //---------------------------------------------------------------
        /// Alignment Anchor From String
        ///
        /// @param String
        /// @return Alignment anchor type corresponding to given string
        //---------------------------------------------------------------
        AlignmentAnchor AlignmentAnchorFromString(const std::string& instrString);
        //---------------------------------------------------------------
        /// String From Alignment Anchor
        ///
        /// @param Alignment Anchor
        /// @return String.
        //---------------------------------------------------------------
        std::string StringFromAlignmentAnchor(AlignmentAnchor ineAlignmentAnchor);
        //--------------------------------------------------------------
        /// Get Anchor Point
        ///
        /// Calculate the alignment anchor point based on the anchor
        /// and the size
        ///
        /// @param Anchor type 
        /// @param Half size
        /// @param Out anchor point
        //---------------------------------------------------------------
        void GetAnchorPoint(AlignmentAnchor ineAlignment, const Core::Vector2& invHalfSize, Core::Vector2& outvAlignmentPoint);
    }
}

#endif
