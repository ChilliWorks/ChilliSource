//
//  AlignmentAnchors.cpp
//  moFloTest
//
//  Created by Scott Downie on 25/04/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#include <ChilliSource/Core/AlignmentAnchors.h>

namespace moFlo
{
    namespace Core
    {
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
        void Align(AlignmentAnchor ineAlignment, const CVector2& invHalfSize, CVector2& outvAlignmentPoint)
        {
            //Vertical alignment
			switch (ineAlignment) 
            {
				case Core::ALIGN_TOP_LEFT:
				case Core::ALIGN_TOP_CENTRE:
				case Core::ALIGN_TOP_RIGHT:
					outvAlignmentPoint.y = -invHalfSize.y;
					break;
				case Core::ALIGN_MIDDLE_LEFT:
				case Core::ALIGN_MIDDLE_CENTRE:
				case Core::ALIGN_MIDDLE_RIGHT:
					outvAlignmentPoint.y = 0;
					break;				
				case Core::ALIGN_BOTTOM_LEFT:
				case Core::ALIGN_BOTTOM_CENTRE:
				case Core::ALIGN_BOTTOM_RIGHT:
					outvAlignmentPoint.y = invHalfSize.y;
					break;
                    
                default:
                    break;
			}
			
			//Horizontal alignment
			switch (ineAlignment) 
            {
				case Core::ALIGN_TOP_LEFT:
				case Core::ALIGN_MIDDLE_LEFT:
				case Core::ALIGN_BOTTOM_LEFT:
					outvAlignmentPoint.x = invHalfSize.x;
					break;
					
				case Core::ALIGN_TOP_CENTRE:
				case Core::ALIGN_MIDDLE_CENTRE:
				case Core::ALIGN_BOTTOM_CENTRE:
					outvAlignmentPoint.x = 0;
					break;	
					
				case Core::ALIGN_TOP_RIGHT:
				case Core::ALIGN_MIDDLE_RIGHT:
				case Core::ALIGN_BOTTOM_RIGHT:
					outvAlignmentPoint.x = -invHalfSize.x;
					break;
                    
                default:
                    break;
			}
        }
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
        void GetAnchorPoint(AlignmentAnchor ineAlignment, const CVector2& invHalfSize, CVector2& outvAlignmentPoint)
        {
            //Vertical alignment
			switch (ineAlignment) 
            {
				case Core::ALIGN_TOP_LEFT:
				case Core::ALIGN_TOP_CENTRE:
				case Core::ALIGN_TOP_RIGHT:
					outvAlignmentPoint.y = invHalfSize.y;
					break;
				case Core::ALIGN_MIDDLE_LEFT:
				case Core::ALIGN_MIDDLE_CENTRE:
				case Core::ALIGN_MIDDLE_RIGHT:
					outvAlignmentPoint.y = 0;
					break;				
				case Core::ALIGN_BOTTOM_LEFT:
				case Core::ALIGN_BOTTOM_CENTRE:
				case Core::ALIGN_BOTTOM_RIGHT:
					outvAlignmentPoint.y = -invHalfSize.y;
					break;
                default:
                    break;
			}
			
			//Horizontal alignment
			switch (ineAlignment) 
            {
				case Core::ALIGN_TOP_LEFT:
				case Core::ALIGN_MIDDLE_LEFT:
				case Core::ALIGN_BOTTOM_LEFT:
					outvAlignmentPoint.x = -invHalfSize.x;
					break;
					
				case Core::ALIGN_TOP_CENTRE:
				case Core::ALIGN_MIDDLE_CENTRE:
				case Core::ALIGN_BOTTOM_CENTRE:
					outvAlignmentPoint.x = 0;
					break;	
					
				case Core::ALIGN_TOP_RIGHT:
				case Core::ALIGN_MIDDLE_RIGHT:
				case Core::ALIGN_BOTTOM_RIGHT:
					outvAlignmentPoint.x = invHalfSize.x;
					break;
                default:
                    break;
			}
        }
        //---------------------------------------------------------------
		/// Alignment Anchor From String
		///
		/// @param String
		/// @return Alignment anchor type corresponding to given string
		//---------------------------------------------------------------
		AlignmentAnchor AlignmentAnchorFromString(const std::string& instrString)
		{
			if(instrString == "MiddleCentre")
			{
				return ALIGN_MIDDLE_CENTRE;
			}
			else if(instrString == "TopLeft")
			{
				return ALIGN_TOP_LEFT;
			}
			else if(instrString == "TopRight")
			{
				return ALIGN_TOP_RIGHT;
			}
			else if(instrString == "BottomLeft")
			{
				return ALIGN_BOTTOM_LEFT;
			}
			else if(instrString == "BottomRight")
			{
				return ALIGN_BOTTOM_RIGHT;
			}
			else if(instrString == "MiddleLeft")
			{
				return ALIGN_MIDDLE_LEFT;
			}
			else if(instrString == "MiddleRight")
			{
				return ALIGN_MIDDLE_RIGHT;
			}
			else if(instrString == "TopCentre")
			{
				return ALIGN_TOP_CENTRE;
			}
			else if(instrString == "BottomCentre")
			{
				return ALIGN_BOTTOM_CENTRE;
			}
			else
			{
				ERROR_LOG("Alignment Anchor: Invalid string " + instrString);
				return ALIGN_MIDDLE_CENTRE;
			}
		}
		//---------------------------------------------------------------
		/// String From Alignment Anchor
		///
		/// @param Alignment Anchor
		/// @return String.
		//---------------------------------------------------------------
		std::string StringFromAlignmentAnchor(AlignmentAnchor ineAlignmentAnchor)
		{
			switch (ineAlignmentAnchor)
			{
			case ALIGN_TOP_LEFT:
				return "TopLeft";
			case ALIGN_TOP_RIGHT:
				return "TopRight";
			case ALIGN_TOP_CENTRE:
				return "TopCentre";
			case ALIGN_MIDDLE_LEFT:
				return "MiddleLeft";
			case ALIGN_MIDDLE_RIGHT:
				return "MiddleRight";
			case ALIGN_MIDDLE_CENTRE:
				return "MiddleCentre";
			case ALIGN_BOTTOM_LEFT:
				return "BottomLeft";
			case ALIGN_BOTTOM_RIGHT:
				return "BottomRight";
			case ALIGN_BOTTOM_CENTRE:
				return "BottomCentre";
			default:
				return "";
			}
		}
    }
}
