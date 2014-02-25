//
//  HorizontalList.cpp
//  MoshiMonsters
//
//  Created by Scott Downie on 14/06/2012.
//  Copyright (c) 2012 Tag Games. All rights reserved.
//

#include <ChilliSource/GUI/Container/HorizontalList.h>

#include <ChilliSource/Core/Base/Screen.h>

namespace ChilliSource
{
    namespace GUI
    {
        DEFINE_META_CLASS(CHorizontalList);
        DEFINE_PROPERTY(AbsoluteSpacing);
        DEFINE_PROPERTY(RelativeSpacing);
        DEFINE_PROPERTY(Justifiction);
        DEFINE_PROPERTY(VerticalJustifiction);
        
        //---------------------------------------------------------
        /// Constructor
        ///
        /// Default
        //---------------------------------------------------------
        CHorizontalList::CHorizontalList()
        : HorizontalJustifiction(ItemHorizontalJustification::k_left), AbsoluteSpacing(0.0f), RelativeSpacing(0.0f), meVerticalAlignmentAnchor(Core::AlignmentAnchor::k_middleLeft)
        {
            
        }
        //---------------------------------------------------------
        /// Constructor
        ///
        /// @param Dictionary of params
        //---------------------------------------------------------
        CHorizontalList::CHorizontalList(const Core::ParamDictionary& insParams)
        : CGUIView(insParams), HorizontalJustifiction(ItemHorizontalJustification::k_left), AbsoluteSpacing(0.0f), meVerticalAlignmentAnchor(Core::AlignmentAnchor::k_middleLeft), RelativeSpacing(0.0f)
        {
            std::string strValue;
            
            //---Set Justification
            if(insParams.TryGetValue("HorizontalJustification", strValue))
            {
                if(strValue == "Centre")
                {
                    SetHorizontalJustification(ItemHorizontalJustification::k_centre);
                }
                else if(strValue == "Left")
                {
                    SetHorizontalJustification(ItemHorizontalJustification::k_left);
                }
                else if(strValue == "Right")
                {
                    SetHorizontalJustification(ItemHorizontalJustification::k_right);
                }
            }
            //---Set Justification
            if(insParams.TryGetValue("VerticalJustification", strValue))
            {
                if(strValue == "Top")
                {
                    SetVerticalJustification(ItemVerticalJustification::k_top);
                }
                else if(strValue == "Middle")
                {
                    SetVerticalJustification(ItemVerticalJustification::k_middle);
                }
                else if(strValue == "Bottom")
                {
                    SetVerticalJustification(ItemVerticalJustification::k_bottom);
                }
            }
            //---Set Spacing
            if(insParams.TryGetValue("AbsoluteSpacing", strValue))
            {
                AbsoluteSpacing = Core::CStringConverter::ParseFloat(strValue);
            }
            
            if(insParams.TryGetValue("RelativeSpacing", strValue))
            {
                RelativeSpacing = Core::CStringConverter::ParseFloat(strValue);
            }
        }
        //---------------------------------------------------------
        /// Set Relative Spacing
        ///
        /// @param Space between items in the list relative to
        /// the size of the container
        //---------------------------------------------------------
        void CHorizontalList::SetRelativeSpacing(f32 infSpacing)
        {
            RelativeSpacing = infSpacing;
        }
        //---------------------------------------------------------
        /// Set Absolute Spacing
        ///
        /// @param Space between items in the list
        //---------------------------------------------------------
        void CHorizontalList::SetAbsoluteSpacing(f32 infSpacing)
        {
            AbsoluteSpacing = infSpacing;
        }
        //---------------------------------------------------------
        /// Get Absolute Spacing
        ///
        /// @return Space between items in the list
        //---------------------------------------------------------
        f32 CHorizontalList::GetAbsoluteSpacing() const
        {
            return AbsoluteSpacing;
        }
        //---------------------------------------------------------
        /// Get Relative Spacing
        ///
        /// @return Space between items in the list relative to the
        /// size of the container
        //---------------------------------------------------------
        f32 CHorizontalList::GetRelativeSpacing() const
        {
            return RelativeSpacing;
        }
        //---------------------------------------------------------
        /// Set Horizontal Justification
        ///
        /// @param Justification which will align the items within the list
        //---------------------------------------------------------
        void CHorizontalList::SetHorizontalJustification(ItemHorizontalJustification ineJustification)
        {
            HorizontalJustifiction = ineJustification;
        }
        //---------------------------------------------------------
        /// Get Horizontal Justification
        ///
        /// @return Justification which will align the items within the list
        //---------------------------------------------------------
        CHorizontalList::ItemHorizontalJustification CHorizontalList::GetHorizontalJustification() const
        {
            return HorizontalJustifiction;
        }
        //---------------------------------------------------------
        /// Set Vertical Justification
        ///
        /// @param Justification which will align the items within the list
        //---------------------------------------------------------
        void CHorizontalList::SetVerticalJustification(ItemVerticalJustification ineJustification)
        {
            VerticalJustifiction = ineJustification;
            
            switch(VerticalJustifiction)
            {
                case ItemVerticalJustification::k_top:
                    meVerticalAlignmentAnchor = Core::AlignmentAnchor::k_topLeft;
                    break;
                case ItemVerticalJustification::k_middle:
                    meVerticalAlignmentAnchor = Core::AlignmentAnchor::k_middleLeft;
                    break;
                case ItemVerticalJustification::k_bottom:
                    meVerticalAlignmentAnchor = Core::AlignmentAnchor::k_bottomLeft;
                    break;
            }
            
        }
        //---------------------------------------------------------
        /// GetVertical Justification
        ///
        /// @return Vertical Justification which will align the items within the list
        //---------------------------------------------------------
        CHorizontalList::ItemVerticalJustification CHorizontalList::GetVerticalJustification() const
        {
            return VerticalJustifiction;
        }
        //-----------------------------------------------------
        /// Add Subview
        ///
        /// Add a view to the hierarchy
        ///
        /// @param GUIView shared pointer
        //-----------------------------------------------------
        void CHorizontalList::AddSubview(const GUIViewPtr& inpSubview)
        {
            CGUIView::AddSubview(inpSubview);
            maListSubviews.push_back(inpSubview);
        }
        //-----------------------------------------------------
        /// Remove Subview (Internal)
        ///
        /// Remove a view from our hierarchy
        ///
        /// @param GUIView pointer
        //-----------------------------------------------------
        void CHorizontalList::RemoveSubview(CGUIView* inpSubview)
        {
            CGUIView::RemoveSubview(inpSubview);
            for(CGUIView::Subviews::iterator it = maListSubviews.begin(); it != maListSubviews.end(); ++it)
            {
                if((*it).get() == inpSubview)
                {
                    maListSubviews.erase(it);
                    return;
                }
            }
            
        }
		//-----------------------------------------------------
		/// Remove Subview (Internal)
		///
		/// Remove a view from our hierarchy
		///
		/// @param GUIView pointer
		//-----------------------------------------------------			
		void CHorizontalList::RemoveAllSubviews()
		{
			CGUIView::RemoveAllSubviews();
            maListSubviews.clear();
		}
        //-------------------------------------------------------
        /// Draw
        ///
        /// Draw all our subviews in neat rows and columns. Each
        /// cell is based on the size of the largest content 
        ///
        /// @param Canvas renderer pointer
        //-------------------------------------------------------
        void CHorizontalList::Draw(Rendering::CCanvasRenderer* inpCanvas)
        {
            //This is a container view and has no renderables
            //We will need to render our subviews though
            if(Visible)
            {
				//Check if this is on screen
				Core::CVector2 vTopRight = GetAbsoluteScreenSpaceAnchorPoint(Core::AlignmentAnchor::k_topRight);
				Core::CVector2 vBottomLeft = GetAbsoluteScreenSpaceAnchorPoint(Core::AlignmentAnchor::k_bottomLeft);
				
				if(vTopRight.y < 0 || vBottomLeft.y > Core::CScreen::GetOrientedHeight() || vTopRight.x < 0 || vBottomLeft.x > Core::CScreen::GetOrientedWidth())
				{
					//Offscreen
					return;
				}
				
                //Arrange everything!
                LayoutContent();
				
                CGUIView::Draw(inpCanvas);
            }
        }
        //-------------------------------------------------------
        /// Layout Content
        ///
        /// Loop through the subviews and position them correctly
        /// in the list 
        //-------------------------------------------------------
        void CHorizontalList::LayoutContent()
        {
            f32 fContainerWidth = GetAbsoluteSize().x;
            f32 fSpacing = (fContainerWidth * RelativeSpacing) + AbsoluteSpacing;
            
            f32 fX = 0.0f;
            for(Subviews::iterator pView = maListSubviews.begin(); pView != maListSubviews.end(); ++pView)
            {
                (*pView)->EnableAlignmentToParent(true);
                (*pView)->SetAlignmentToParent(meVerticalAlignmentAnchor);
                (*pView)->SetLocalAlignment(meVerticalAlignmentAnchor);
                
                (*pView)->SetOffsetFromParentAlignment(0.0f, 0.0f, fX, 0.0f);
                fX += ((*pView)->GetAbsoluteSize().x + fSpacing);
            }
            
            fX -= fSpacing;
            
            switch(HorizontalJustifiction)
            {
                case ItemHorizontalJustification::k_left:
                default:
                    break;
                case ItemHorizontalJustification::k_right:
                {
                    f32 fOffset = GetAbsoluteSize().x - fX;
                    for(Subviews::iterator pView = maListSubviews.begin(); pView != maListSubviews.end(); ++pView)
                    {
                        (*pView)->SetOffsetFromParentAlignment(0.0f, 0.0f, fOffset + (*pView)->GetOffsetFromParentAlignment().vAbsolute.x, 0.0f);
                    }
                    break;
                }
                case ItemHorizontalJustification::k_centre:
                {
                    f32 fHalfOffset = (GetAbsoluteSize().x * 0.5f) - (fX * 0.5f);
                    for(Subviews::iterator pView = maListSubviews.begin(); pView != maListSubviews.end(); ++pView)
                    {
                        (*pView)->SetOffsetFromParentAlignment(0.0f, 0.0f, fHalfOffset + (*pView)->GetOffsetFromParentAlignment().vAbsolute.x, 0.0f);
                    }
                    break;
                }
            }
        }
    }
}

