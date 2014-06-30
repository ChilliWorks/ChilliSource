//
//  HorzontalList.h
//  Chilli Source
//  Created by Scott Downie on 14/6/2012.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2012 Tag Games Limited
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


#ifndef _CHILLISOURCE_GUI_HORIZONTAL_LIST_H_
#define _CHILLISOURCE_GUI_HORIZONTAL_LIST_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/GUI/Base/GUIView.h>

namespace ChilliSource
{
    namespace GUI
    {
        class HorizontalList : public GUIView
        {
        public:
            
            DECLARE_META_CLASS(HorizontalList)
            
            HorizontalList();
            HorizontalList(const Core::ParamDictionary& insParams);
            
            enum class ItemHorizontalJustification
            {
                k_left,
                k_right,
                k_centre
            };
            
            enum class ItemVerticalJustification
            {
                k_top,
                k_middle,
                k_bottom
            };
            
            //---------------------------------------------------------
            /// Set Absolute Spacing
            ///
            /// @param Space between items in the list
            //---------------------------------------------------------
            void SetAbsoluteSpacing(f32 infSpacing);
            //---------------------------------------------------------
            /// Get Absolute Spacing
            ///
            /// @return Space between items in the list
            //---------------------------------------------------------
            f32 GetAbsoluteSpacing() const;
            //---------------------------------------------------------
            /// Set Relative Spacing
            ///
            /// @param Space between items in the list relative to
            /// the size of the container
            //---------------------------------------------------------
            void SetRelativeSpacing(f32 infSpacing);
            //---------------------------------------------------------
            /// Get Relative Spacing
            ///
            /// @return Space between items in the list relative to the
            /// size of the container
            //---------------------------------------------------------
            f32 GetRelativeSpacing() const;
            //---------------------------------------------------------
            /// Set Horizontal Justification
            ///
            /// @param Justification which will align the items within the list
            //---------------------------------------------------------
            void SetHorizontalJustification(ItemHorizontalJustification ineJustification);
            //---------------------------------------------------------
            /// Get Horizontal Justification
            ///
            /// @return Justification which will align the items within the list
            //---------------------------------------------------------
            ItemHorizontalJustification GetHorizontalJustification() const;
            //---------------------------------------------------------
            /// Set Vertical Justification
            ///
            /// @param Justification which will align the items within the list
            //---------------------------------------------------------
            void SetVerticalJustification(ItemVerticalJustification ineJustification);
            //---------------------------------------------------------
            /// Get Justification
            ///
            /// @return Justification which will align the items within the list
            //---------------------------------------------------------
            ItemVerticalJustification GetVerticalJustification() const;
            //-----------------------------------------------------
            /// Add Subview
            ///
            /// Add a view to the hierarchy
            ///
            /// @param GUIView shared pointer
            //-----------------------------------------------------
            void AddSubview(const GUIViewSPtr& inpSubview);
            //-----------------------------------------------------
            /// Remove Subview (Internal)
            ///
            /// Remove a view from our hierarchy
            ///
            /// @param GUIView pointer
            //-----------------------------------------------------
            void RemoveSubview(GUIView* inpSubview);
            //-----------------------------------------------------
            /// Remove Subview (Internal)
            ///
            /// Remove a view from our hierarchy
            ///
            /// @param GUIView pointer
            //-----------------------------------------------------			
            void RemoveAllSubviews();
            //-------------------------------------------------------
            /// Draw
            ///
            /// Draw all our subviews in neat rows and columns. Each
            /// cell is based on the size of the largest content 
            ///
            /// @param Canvas renderer pointer
            //-------------------------------------------------------
            void Draw(Rendering::CanvasRenderer* inpCanvas);
            //-------------------------------------------------------
            /// Layout Content
            ///
            /// Loop through the subviews and position them correctly
            /// in the list 
            //-------------------------------------------------------
            void LayoutContent();
            
        private:
            
            GUIView::Subviews maListSubviews;
            
            Rendering::AlignmentAnchor meVerticalAlignmentAnchor;
        
            DECLARE_PROPERTY_A(f32, AbsoluteSpacing, SetAbsoluteSpacing, GetAbsoluteSpacing);
            DECLARE_PROPERTY_A(f32, RelativeSpacing, SetRelativeSpacing, GetRelativeSpacing);
            DECLARE_PROPERTY_A(ItemHorizontalJustification, HorizontalJustifiction, SetHorizontalJustification, GetHorizontalJustification);
            DECLARE_PROPERTY_A(ItemVerticalJustification, VerticalJustifiction, SetVerticalJustification, GetVerticalJustification);
        };
    }
}

#endif

