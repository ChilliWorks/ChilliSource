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
        //---------------------------------------------------------------
        Core::Vector2 GetAnchorPoint(AlignmentAnchor in_alignment, const Core::Vector2& in_halfSize)
        {
            Core::Vector2 output;
            
            //Vertical alignment
			switch (in_alignment)
            {
				case AlignmentAnchor::k_topLeft:
				case AlignmentAnchor::k_topCentre:
				case AlignmentAnchor::k_topRight:
					output.y = in_halfSize.y;
					break;
				case AlignmentAnchor::k_middleLeft:
				case AlignmentAnchor::k_middleCentre:
				case AlignmentAnchor::k_middleRight:
					output.y = 0;
					break;				
				case AlignmentAnchor::k_bottomLeft:
				case AlignmentAnchor::k_bottomCentre:
				case AlignmentAnchor::k_bottomRight:
					output.y = -in_halfSize.y;
					break;
                default:
                    break;
			}
			
			//Horizontal alignment
			switch (in_alignment)
            {
				case AlignmentAnchor::k_topLeft:
				case AlignmentAnchor::k_middleLeft:
				case AlignmentAnchor::k_bottomLeft:
					output.x = -in_halfSize.x;
					break;
					
				case AlignmentAnchor::k_topCentre:
				case AlignmentAnchor::k_middleCentre:
				case AlignmentAnchor::k_bottomCentre:
					output.x = 0;
					break;	
					
				case AlignmentAnchor::k_topRight:
				case AlignmentAnchor::k_middleRight:
				case AlignmentAnchor::k_bottomRight:
					output.x = in_halfSize.x;
					break;
                default:
                    break;
			}
            
            return output;
        }
        //---------------------------------------------------------------
		//---------------------------------------------------------------
		AlignmentAnchor ParseAlignmentAnchor(const std::string& in_string)
		{
			if(in_string == "MiddleCentre")
			{
				return AlignmentAnchor::k_middleCentre;
			}
			else if(in_string == "TopLeft")
			{
				return AlignmentAnchor::k_topLeft;
			}
			else if(in_string == "TopRight")
			{
				return AlignmentAnchor::k_topRight;
			}
			else if(in_string == "BottomLeft")
			{
				return AlignmentAnchor::k_bottomLeft;
			}
			else if(in_string == "BottomRight")
			{
				return AlignmentAnchor::k_bottomRight;
			}
			else if(in_string == "MiddleLeft")
			{
				return AlignmentAnchor::k_middleLeft;
			}
			else if(in_string == "MiddleRight")
			{
				return AlignmentAnchor::k_middleRight;
			}
			else if(in_string == "TopCentre")
			{
				return AlignmentAnchor::k_topCentre;
			}
			else if(in_string == "BottomCentre")
			{
				return AlignmentAnchor::k_bottomCentre;
			}
			else
			{
				CS_LOG_ERROR("Alignment Anchor: Invalid string " + in_string);
				return AlignmentAnchor::k_middleCentre;
			}
		}
		//---------------------------------------------------------------
		//---------------------------------------------------------------
		std::string StringFromAlignmentAnchor(AlignmentAnchor in_alignmentAnchor)
		{
			switch (in_alignmentAnchor)
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
                CS_LOG_ERROR("Invalid alignment anchor.");
				return "";
			}
		}
    }
}
