//
//  VerticalList.cpp
//  MoFlow
//
//  Created by Scott Downie on 1/07/2013.
//  Copyright (c) 2013 Tag Games. All rights reserved.
//

#include <ChilliSource/Rendering/GUI/VerticalList.h>

#include <ChilliSource/Core/Base/Screen.h>

namespace ChilliSource
{
    namespace Rendering
    {
        DEFINE_META_CLASS(CVerticalList);
        
        DEFINE_PROPERTY(AbsoluteSpacing);
        DEFINE_PROPERTY(RelativeSpacing);
        DEFINE_PROPERTY(Justifiction);
        DEFINE_PROPERTY(HorizontalJustifiction);
        
        //---------------------------------------------------------
        /// Constructor
        ///
        /// Default
        //---------------------------------------------------------
        CVerticalList::CVerticalList()
        : VerticalJustifiction(ItemVerticalJustification::k_top), AbsoluteSpacing(0.0f), RelativeSpacing(0.0f), meHorizontalAlignmentAnchor(Core::AlignmentAnchor::k_topCentre)
        {
            
        }
        //---------------------------------------------------------
        /// Constructor
        ///
        /// @param Dictionary of params
        //---------------------------------------------------------
        CVerticalList::CVerticalList(const Core::ParamDictionary& insParams)
        : CGUIView(insParams), VerticalJustifiction(ItemVerticalJustification::k_top), AbsoluteSpacing(0.0f), meHorizontalAlignmentAnchor(Core::AlignmentAnchor::k_topCentre), RelativeSpacing(0.0f)
        {
            std::string strValue;
            
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
        void CVerticalList::SetRelativeSpacing(f32 infSpacing)
        {
            RelativeSpacing = infSpacing;
        }
        //---------------------------------------------------------
        /// Set Absolute Spacing
        ///
        /// @param Space between items in the list
        //---------------------------------------------------------
        void CVerticalList::SetAbsoluteSpacing(f32 infSpacing)
        {
            AbsoluteSpacing = infSpacing;
        }
        //---------------------------------------------------------
        /// Get Absolute Spacing
        ///
        /// @return Space between items in the list
        //---------------------------------------------------------
        f32 CVerticalList::GetAbsoluteSpacing() const
        {
            return AbsoluteSpacing;
        }
        //---------------------------------------------------------
        /// Get Relative Spacing
        ///
        /// @return Space between items in the list relative to the
        /// size of the container
        //---------------------------------------------------------
        f32 CVerticalList::GetRelativeSpacing() const
        {
            return RelativeSpacing;
        }
        //---------------------------------------------------------
        /// Set Vertical Justification
        ///
        /// @param Justification which will align the items within the list
        //---------------------------------------------------------
        void CVerticalList::SetVerticalJustification(ItemVerticalJustification ineJustification)
        {
            VerticalJustifiction = ineJustification;
        }
        //---------------------------------------------------------
        /// Get Vertical Justification
        ///
        /// @return Justification which will align the items within the list
        //---------------------------------------------------------
        CVerticalList::ItemVerticalJustification CVerticalList::GetVerticalJustification() const
        {
            return VerticalJustifiction;
        }
        //---------------------------------------------------------
        /// Set Horizontal Justification
        ///
        /// @param Justification which will align the items within the list
        //---------------------------------------------------------
        void CVerticalList::SetHorizontalJustification(ItemHorizontalJustification ineJustification)
        {
            HorizontalJustifiction = ineJustification;
            
            switch(HorizontalJustifiction)
            {
                case ItemHorizontalJustification::k_left:
                    meHorizontalAlignmentAnchor = Core::AlignmentAnchor::k_topLeft;
                    break;
                case ItemHorizontalJustification::k_centre:
                    meHorizontalAlignmentAnchor = Core::AlignmentAnchor::k_topCentre;
                    break;
                case ItemHorizontalJustification::k_right:
                    meHorizontalAlignmentAnchor = Core::AlignmentAnchor::k_topRight;
                    break;
            }
            
        }
        //---------------------------------------------------------
        /// Get Horizontal Justification
        ///
        /// @return Horizontal Justification which will align the items within the list
        //---------------------------------------------------------
        CVerticalList::ItemHorizontalJustification CVerticalList::GetHorizontalJustification() const
        {
            return HorizontalJustifiction;
        }
        //-----------------------------------------------------
        /// Add Subview
        ///
        /// Add a view to the hierarchy
        ///
        /// @param GUIView shared pointer
        //-----------------------------------------------------
        void CVerticalList::AddSubview(const GUIViewPtr& inpSubview)
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
        void CVerticalList::RemoveSubview(CGUIView* inpSubview)
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
		void CVerticalList::RemoveAllSubviews()
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
        void CVerticalList::Draw(Rendering::CCanvasRenderer* inpCanvas)
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
        void CVerticalList::LayoutContent()
        {
            f32 fContainerHeight = GetAbsoluteSize().y;
            f32 fSpacing = (fContainerHeight * RelativeSpacing) + AbsoluteSpacing;
            
            f32 fY = 0.0f;
            for(Subviews::iterator pView = maListSubviews.begin(); pView != maListSubviews.end(); ++pView)
            {
                (*pView)->EnableAlignmentToParent(true);
                (*pView)->SetAlignmentToParent(meHorizontalAlignmentAnchor);
                (*pView)->SetLocalAlignment(meHorizontalAlignmentAnchor);
                
                (*pView)->SetOffsetFromParentAlignment(0.0f, 0.0f, 0.0f, fY);
                fY -= ((*pView)->GetAbsoluteSize().y + fSpacing);
            }
            
            fY -= fSpacing;
            
            switch(VerticalJustifiction)
            {
                case ItemVerticalJustification::k_top:
                default:
                    break;
                case ItemVerticalJustification::k_bottom:
                {
                    f32 fOffset = -(GetAbsoluteSize().y + fY);
                    for(Subviews::iterator pView = maListSubviews.begin(); pView != maListSubviews.end(); ++pView)
                    {
                        (*pView)->SetOffsetFromParentAlignment(0.0f, 0.0f, 0.0f, fOffset + (*pView)->GetOffsetFromParentAlignment().vAbsolute.y);
                    }
                    break;
                }
                case ItemVerticalJustification::k_middle:
                {
                    f32 fHalfOffset = (GetAbsoluteSize().y + fY) * -0.5f;
                    for(Subviews::iterator pView = maListSubviews.begin(); pView != maListSubviews.end(); ++pView)
                    {
                        (*pView)->SetOffsetFromParentAlignment(0.0f, 0.0f, 0.0f, fHalfOffset + (*pView)->GetOffsetFromParentAlignment().vAbsolute.y);
                    }
                    break;
                }
            }
        }
    }
}

