//
//  Alignment.h
//  moFloTest
//
//  Created by Scott Downie on 22/04/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//


#ifndef _MO_FLO_CORE_ALIGNMENT_ANCHORS_H_
#define _MO_FLO_CORE_ALIGNMENT_ANCHORS_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Math/Vector2Old.h>

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
        void Align(AlignmentAnchor ineAlignment, const Core::Vector2Old& invHalfSize, Core::Vector2Old& outvAlignmentPoint);
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
        void GetAnchorPoint(AlignmentAnchor ineAlignment, const Core::Vector2Old& invHalfSize, Core::Vector2Old& outvAlignmentPoint);
    }
}

#endif
