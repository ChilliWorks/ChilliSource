//
//  VerticalList.h
//  MoFlow
//
//  Created by Scott Downie on 1/07/2013.
//  Copyright (c) 2013 Tag Games. All rights reserved.
//

#ifndef _MOFLOW_GUI_VERTICAL_LIST_H_
#define _MOFLOW_GUI_VERTICAL_LIST_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/GUI/Base/GUIView.h>

namespace ChilliSource
{
    namespace GUI
    {
        class CVerticalList : public CGUIView
        {
        public:
            
            DECLARE_META_CLASS(CVerticalList)
            
            CVerticalList();
            CVerticalList(const Core::ParamDictionary& insParams);
            
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
            /// Set Vertical Justification
            ///
            /// @param Justification which will align the items within the list
            //---------------------------------------------------------
            void SetVerticalJustification(ItemVerticalJustification ineJustification);
            //---------------------------------------------------------
            /// Get Vertical Justification
            ///
            /// @return Justification which will align the items within the list
            //---------------------------------------------------------
            ItemVerticalJustification GetVerticalJustification() const;
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
            DECLARE_PROPERTY_A(ItemVerticalJustification, VerticalJustifiction, SetVerticalJustification, GetVerticalJustification);
            DECLARE_PROPERTY_A(ItemHorizontalJustification, HorizontalJustifiction, SetHorizontalJustification, GetHorizontalJustification);
        };
    }
}

#endif

