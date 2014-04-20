//
//  HorizontalList.cpp
//  MoshiMonsters
//
//  Created by Scott Downie on 14/06/2012.
//  Copyright (c) 2012 Tag Games. All rights reserved.
//

#include <ChilliSource/GUI/Container/HorizontalList.h>

#include <ChilliSource/Core/Base/Screen.h>
#include <ChilliSource/Core/String/StringParser.h>

namespace ChilliSource
{
    namespace GUI
    {
        DEFINE_META_CLASS(HorizontalList);
        DEFINE_PROPERTY(AbsoluteSpacing);
        DEFINE_PROPERTY(RelativeSpacing);
        DEFINE_PROPERTY(Justifiction);
        DEFINE_PROPERTY(VerticalJustifiction);
        
        //---------------------------------------------------------
        /// Constructor
        ///
        /// Default
        //---------------------------------------------------------
        HorizontalList::HorizontalList()
        : HorizontalJustifiction(ItemHorizontalJustification::k_left), AbsoluteSpacing(0.0f), RelativeSpacing(0.0f), meVerticalAlignmentAnchor(Rendering::AlignmentAnchor::k_middleLeft)
        {
            
        }
        //---------------------------------------------------------
        /// Constructor
        ///
        /// @param Dictionary of params
        //---------------------------------------------------------
        HorizontalList::HorizontalList(const Core::ParamDictionary& insParams)
        : GUIView(insParams), HorizontalJustifiction(ItemHorizontalJustification::k_left), AbsoluteSpacing(0.0f), meVerticalAlignmentAnchor(Rendering::AlignmentAnchor::k_middleLeft), RelativeSpacing(0.0f)
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
                AbsoluteSpacing = Core::ParseF32(strValue);
            }
            
            if(insParams.TryGetValue("RelativeSpacing", strValue))
            {
                RelativeSpacing = Core::ParseF32(strValue);
            }
        }
        //---------------------------------------------------------
        /// Set Relative Spacing
        ///
        /// @param Space between items in the list relative to
        /// the size of the container
        //---------------------------------------------------------
        void HorizontalList::SetRelativeSpacing(f32 infSpacing)
        {
            RelativeSpacing = infSpacing;
        }
        //---------------------------------------------------------
        /// Set Absolute Spacing
        ///
        /// @param Space between items in the list
        //---------------------------------------------------------
        void HorizontalList::SetAbsoluteSpacing(f32 infSpacing)
        {
            AbsoluteSpacing = infSpacing;
        }
        //---------------------------------------------------------
        /// Get Absolute Spacing
        ///
        /// @return Space between items in the list
        //---------------------------------------------------------
        f32 HorizontalList::GetAbsoluteSpacing() const
        {
            return AbsoluteSpacing;
        }
        //---------------------------------------------------------
        /// Get Relative Spacing
        ///
        /// @return Space between items in the list relative to the
        /// size of the container
        //---------------------------------------------------------
        f32 HorizontalList::GetRelativeSpacing() const
        {
            return RelativeSpacing;
        }
        //---------------------------------------------------------
        /// Set Horizontal Justification
        ///
        /// @param Justification which will align the items within the list
        //---------------------------------------------------------
        void HorizontalList::SetHorizontalJustification(ItemHorizontalJustification ineJustification)
        {
            HorizontalJustifiction = ineJustification;
        }
        //---------------------------------------------------------
        /// Get Horizontal Justification
        ///
        /// @return Justification which will align the items within the list
        //---------------------------------------------------------
        HorizontalList::ItemHorizontalJustification HorizontalList::GetHorizontalJustification() const
        {
            return HorizontalJustifiction;
        }
        //---------------------------------------------------------
        /// Set Vertical Justification
        ///
        /// @param Justification which will align the items within the list
        //---------------------------------------------------------
        void HorizontalList::SetVerticalJustification(ItemVerticalJustification ineJustification)
        {
            VerticalJustifiction = ineJustification;
            
            switch(VerticalJustifiction)
            {
                case ItemVerticalJustification::k_top:
                    meVerticalAlignmentAnchor = Rendering::AlignmentAnchor::k_topLeft;
                    break;
                case ItemVerticalJustification::k_middle:
                    meVerticalAlignmentAnchor = Rendering::AlignmentAnchor::k_middleLeft;
                    break;
                case ItemVerticalJustification::k_bottom:
                    meVerticalAlignmentAnchor = Rendering::AlignmentAnchor::k_bottomLeft;
                    break;
            }
            
        }
        //---------------------------------------------------------
        /// GetVertical Justification
        ///
        /// @return Vertical Justification which will align the items within the list
        //---------------------------------------------------------
        HorizontalList::ItemVerticalJustification HorizontalList::GetVerticalJustification() const
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
        void HorizontalList::AddSubview(const GUIViewSPtr& inpSubview)
        {
            GUIView::AddSubview(inpSubview);
            maListSubviews.push_back(inpSubview);
        }
        //-----------------------------------------------------
        /// Remove Subview (Internal)
        ///
        /// Remove a view from our hierarchy
        ///
        /// @param GUIView pointer
        //-----------------------------------------------------
        void HorizontalList::RemoveSubview(GUIView* inpSubview)
        {
            GUIView::RemoveSubview(inpSubview);
            for(GUIView::Subviews::iterator it = maListSubviews.begin(); it != maListSubviews.end(); ++it)
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
		void HorizontalList::RemoveAllSubviews()
		{
			GUIView::RemoveAllSubviews();
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
        void HorizontalList::Draw(Rendering::CanvasRenderer* inpCanvas)
        {
            //This is a container view and has no renderables
            //We will need to render our subviews though
            if(Visible)
            {
				//Check if this is on screen
				Core::Vector2Old vTopRight = GetAbsoluteScreenSpaceAnchorPoint(Rendering::AlignmentAnchor::k_topRight);
				Core::Vector2Old vBottomLeft = GetAbsoluteScreenSpaceAnchorPoint(Rendering::AlignmentAnchor::k_bottomLeft);
				
				if(vTopRight.y < 0 || vBottomLeft.y > Core::Screen::GetOrientedHeight() || vTopRight.x < 0 || vBottomLeft.x > Core::Screen::GetOrientedWidth())
				{
					//Offscreen
					return;
				}
				
                //Arrange everything!
                LayoutContent();
				
                GUIView::Draw(inpCanvas);
            }
        }
        //-------------------------------------------------------
        /// Layout Content
        ///
        /// Loop through the subviews and position them correctly
        /// in the list 
        //-------------------------------------------------------
        void HorizontalList::LayoutContent()
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

