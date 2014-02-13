//
//  Alignment.h
//  moFloTest
//
//  Created by Scott Downie on 22/04/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//


#ifndef _MO_FLO_CORE_ALIGNMENT_ANCHORS_H_
#define _MO_FLO_CORE_ALIGNMENT_ANCHORS_H_

#include <moFlo/Core/Math/Vector2.h>

namespace moFlo
{
    namespace Core
    {
        //---Anchor points for view alignment
        enum AlignmentAnchor
        {
            ALIGN_TOP_LEFT,
            ALIGN_TOP_RIGHT,
            ALIGN_TOP_CENTRE,
            ALIGN_MIDDLE_LEFT,
            ALIGN_MIDDLE_RIGHT,
            ALIGN_MIDDLE_CENTRE,
            ALIGN_BOTTOM_LEFT,
            ALIGN_BOTTOM_RIGHT,
            ALIGN_BOTTOM_CENTRE,
			
            ALIGN_BOTTOM_TOTAL,
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
        void Align(AlignmentAnchor ineAlignment, const CVector2& invHalfSize, CVector2& outvAlignmentPoint);
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
        void GetAnchorPoint(AlignmentAnchor ineAlignment, const CVector2& invHalfSize, CVector2& outvAlignmentPoint);
    }
}

#endif
