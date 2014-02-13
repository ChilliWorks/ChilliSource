//
//  HorizontalList.h
//  MoFlow
//
//  Created by Scott Downie on 14/06/2012.
//  Copyright (c) 2012 Tag Games. All rights reserved.
//

#ifndef _MOFLOW_GUI_HORIZONTAL_LIST_H_
#define _MOFLOW_GUI_HORIZONTAL_LIST_H_

#include <ChilliSource/GUI/GUIView.h>

namespace moFlo
{
    namespace GUI
    {
        class CHorizontalList : public moFlo::GUI::CGUIView
        {
        public:
            
            DECLARE_META_CLASS(CHorizontalList)
            
            CHorizontalList();
            CHorizontalList(const Core::ParamDictionary& insParams);
            
            enum ItemJustification
            {
                JUSTIFY_LEFT,
                JUSTIFY_RIGHT,
                JUSTIFY_CENTRE
            };
            
            enum ItemVerticalJustification
            {
                JUSTIFY_TOP,
                JUSTIFY_MIDDLE,
                JUSTIFY_BOTTOM
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
            /// Set Justification
            ///
            /// @param Justification which will align the items within the list
            //---------------------------------------------------------
            void SetJustification(ItemJustification ineJustification);
            //---------------------------------------------------------
            /// Get Justification
            ///
            /// @return Justification which will align the items within the list
            //---------------------------------------------------------
            ItemJustification GetJustification() const;
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
            void AddSubview(const GUIViewPtr& inpSubview);
            //-----------------------------------------------------
            /// Remove Subview (Internal)
            ///
            /// Remove a view from our hierarchy
            ///
            /// @param GUIView pointer
            //-----------------------------------------------------
            void RemoveSubview(CGUIView* inpSubview);
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
            void Draw(Rendering::CCanvasRenderer* inpCanvas);
            //-------------------------------------------------------
            /// Layout Content
            ///
            /// Loop through the subviews and position them correctly
            /// in the list 
            //-------------------------------------------------------
            void LayoutContent();
            
        private:
            
            CGUIView::Subviews maListSubviews;
            
            Core::AlignmentAnchor meVerticalAlignmentAnchor;
        
            DECLARE_PROPERTY_A(f32, AbsoluteSpacing, SetAbsoluteSpacing, GetAbsoluteSpacing);
            DECLARE_PROPERTY_A(f32, RelativeSpacing, SetRelativeSpacing, GetRelativeSpacing);
            DECLARE_PROPERTY_A(ItemJustification, Justifiction, SetJustification, GetJustification);
            DECLARE_PROPERTY_A(ItemVerticalJustification, VerticalJustifiction, SetVerticalJustification, GetVerticalJustification);
        };
    }
}

#endif

