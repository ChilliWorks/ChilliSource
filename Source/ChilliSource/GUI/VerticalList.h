//
//  VerticalList.h
//  MoFlow
//
//  Created by Scott Downie on 1/07/2013.
//  Copyright (c) 2013 Tag Games. All rights reserved.
//

#ifndef _MOFLOW_GUI_VERTICAL_LIST_H_
#define _MOFLOW_GUI_VERTICAL_LIST_H_

#include <moFlo/GUI/GUIView.h>

namespace moFlo
{
    namespace GUI
    {
        class CVerticalList : public moFlo::GUI::CGUIView
        {
        public:
            
            DECLARE_META_CLASS(CVerticalList)
            
            CVerticalList();
            CVerticalList(const Core::ParamDictionary& insParams);
            
            enum ItemHorizontalJustification
            {
                JUSTIFY_LEFT,
                JUSTIFY_RIGHT,
                JUSTIFY_CENTRE
            };
            
            enum ItemJustification
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
            /// Set Horizontal Justification
            ///
            /// @param Justification which will align the items within the list
            //---------------------------------------------------------
            void SetHorizontalJustification(ItemHorizontalJustification ineJustification);
            //---------------------------------------------------------
            /// Get Justification
            ///
            /// @return Justification which will align the items within the list
            //---------------------------------------------------------
            ItemHorizontalJustification GetHorizontalJustification() const;
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
            
            Core::AlignmentAnchor meHorizontalAlignmentAnchor;
        
            DECLARE_PROPERTY_A(f32, AbsoluteSpacing, SetAbsoluteSpacing, GetAbsoluteSpacing);
            DECLARE_PROPERTY_A(f32, RelativeSpacing, SetRelativeSpacing, GetRelativeSpacing);
            DECLARE_PROPERTY_A(ItemJustification, Justifiction, SetJustification, GetJustification);
            DECLARE_PROPERTY_A(ItemHorizontalJustification, HorizontalJustifiction, SetHorizontalJustification, GetHorizontalJustification);
        };
    }
}

#endif

