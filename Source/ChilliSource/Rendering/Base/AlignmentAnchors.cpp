//
//  AlignmentAnchors.cpp
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

#include <ChilliSource/Rendering/Base/AlignmentAnchors.h>

namespace ChilliSource
{
    namespace Rendering
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
        void Align(AlignmentAnchor ineAlignment, const Core::Vector2& invHalfSize, Core::Vector2& outvAlignmentPoint)
        {
            //Vertical alignment
			switch (ineAlignment) 
            {
				case AlignmentAnchor::k_topLeft:
				case AlignmentAnchor::k_topCentre:
				case AlignmentAnchor::k_topRight:
					outvAlignmentPoint.y = -invHalfSize.y;
					break;
				case AlignmentAnchor::k_middleLeft:
				case AlignmentAnchor::k_middleCentre:
				case AlignmentAnchor::k_middleRight:
					outvAlignmentPoint.y = 0;
					break;				
				case AlignmentAnchor::k_bottomLeft:
				case AlignmentAnchor::k_bottomCentre:
				case AlignmentAnchor::k_bottomRight:
					outvAlignmentPoint.y = invHalfSize.y;
					break;
                    
                default:
                    break;
			}
			
			//Horizontal alignment
			switch (ineAlignment) 
            {
				case AlignmentAnchor::k_topLeft:
				case AlignmentAnchor::k_middleLeft:
				case AlignmentAnchor::k_bottomLeft:
					outvAlignmentPoint.x = invHalfSize.x;
					break;
					
				case AlignmentAnchor::k_topCentre:
				case AlignmentAnchor::k_middleCentre:
				case AlignmentAnchor::k_bottomCentre:
					outvAlignmentPoint.x = 0;
					break;	
					
				case AlignmentAnchor::k_topRight:
				case AlignmentAnchor::k_middleRight:
				case AlignmentAnchor::k_bottomRight:
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
        void GetAnchorPoint(AlignmentAnchor ineAlignment, const Core::Vector2& invHalfSize, Core::Vector2& outvAlignmentPoint)
        {
            //Vertical alignment
			switch (ineAlignment) 
            {
				case AlignmentAnchor::k_topLeft:
				case AlignmentAnchor::k_topCentre:
				case AlignmentAnchor::k_topRight:
					outvAlignmentPoint.y = invHalfSize.y;
					break;
				case AlignmentAnchor::k_middleLeft:
				case AlignmentAnchor::k_middleCentre:
				case AlignmentAnchor::k_middleRight:
					outvAlignmentPoint.y = 0;
					break;				
				case AlignmentAnchor::k_bottomLeft:
				case AlignmentAnchor::k_bottomCentre:
				case AlignmentAnchor::k_bottomRight:
					outvAlignmentPoint.y = -invHalfSize.y;
					break;
                default:
                    break;
			}
			
			//Horizontal alignment
			switch (ineAlignment) 
            {
				case AlignmentAnchor::k_topLeft:
				case AlignmentAnchor::k_middleLeft:
				case AlignmentAnchor::k_bottomLeft:
					outvAlignmentPoint.x = -invHalfSize.x;
					break;
					
				case AlignmentAnchor::k_topCentre:
				case AlignmentAnchor::k_middleCentre:
				case AlignmentAnchor::k_bottomCentre:
					outvAlignmentPoint.x = 0;
					break;	
					
				case AlignmentAnchor::k_topRight:
				case AlignmentAnchor::k_middleRight:
				case AlignmentAnchor::k_bottomRight:
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
		AlignmentAnchor ParseAlignmentAnchor(const std::string& instrString)
		{
			if(instrString == "MiddleCentre")
			{
				return AlignmentAnchor::k_middleCentre;
			}
			else if(instrString == "TopLeft")
			{
				return AlignmentAnchor::k_topLeft;
			}
			else if(instrString == "TopRight")
			{
				return AlignmentAnchor::k_topRight;
			}
			else if(instrString == "BottomLeft")
			{
				return AlignmentAnchor::k_bottomLeft;
			}
			else if(instrString == "BottomRight")
			{
				return AlignmentAnchor::k_bottomRight;
			}
			else if(instrString == "MiddleLeft")
			{
				return AlignmentAnchor::k_middleLeft;
			}
			else if(instrString == "MiddleRight")
			{
				return AlignmentAnchor::k_middleRight;
			}
			else if(instrString == "TopCentre")
			{
				return AlignmentAnchor::k_topCentre;
			}
			else if(instrString == "BottomCentre")
			{
				return AlignmentAnchor::k_bottomCentre;
			}
			else
			{
				CS_LOG_ERROR("Alignment Anchor: Invalid string " + instrString);
				return AlignmentAnchor::k_middleCentre;
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
			case AlignmentAnchor::k_topLeft:
				return "TopLeft";
			case AlignmentAnchor::k_topRight:
				return "TopRight";
			case AlignmentAnchor::k_topCentre:
				return "TopCentre";
			case AlignmentAnchor::k_middleLeft:
				return "MiddleLeft";
			case AlignmentAnchor::k_middleRight:
				return "MiddleRight";
			case AlignmentAnchor::k_middleCentre:
				return "MiddleCentre";
			case AlignmentAnchor::k_bottomLeft:
				return "BottomLeft";
			case AlignmentAnchor::k_bottomRight:
				return "BottomRight";
			case AlignmentAnchor::k_bottomCentre:
				return "BottomCentre";
			default:
				return "";
			}
		}
    }
}
