//
//  HorizontalList.cpp
//  MoshiMonsters
//
//  Created by Scott Downie on 14/06/2012.
//  Copyright (c) 2012 Tag Games. All rights reserved.
//

#include <ChilliSource/Rendering/GUI/HorizontalList.h>

#include <ChilliSource/Core/Base/Screen.h>

namespace moFlo
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
        : Justifiction(JUSTIFY_LEFT), AbsoluteSpacing(0.0f), RelativeSpacing(0.0f), meVerticalAlignmentAnchor(Core::ALIGN_MIDDLE_LEFT)
        {
            
        }
        //---------------------------------------------------------
        /// Constructor
        ///
        /// @param Dictionary of params
        //---------------------------------------------------------
        CHorizontalList::CHorizontalList(const Core::ParamDictionary& insParams)
        : CGUIView(insParams), Justifiction(JUSTIFY_LEFT), AbsoluteSpacing(0.0f), meVerticalAlignmentAnchor(Core::ALIGN_MIDDLE_LEFT), RelativeSpacing(0.0f)
        {
            std::string strValue;
            
            //---Set Justification
            if(insParams.TryGetValue("Justification", strValue))
            {
                if(strValue == "Centre")
                {
                    Justifiction = JUSTIFY_CENTRE;
                }
                else if(strValue == "Left")
                {
                    Justifiction = JUSTIFY_LEFT;
                }
                else if(strValue == "Right")
                {
                    Justifiction = JUSTIFY_RIGHT;
                }
            }
            //---Set Justification
            if(insParams.TryGetValue("VerticalJustification", strValue))
            {
                if(strValue == "Top")
                {
                    SetVerticalJustification( JUSTIFY_TOP );
                }
                else if(strValue == "Middle")
                {
                    SetVerticalJustification( JUSTIFY_MIDDLE );
                }
                else if(strValue == "Bottom")
                {
                    SetVerticalJustification( JUSTIFY_BOTTOM );
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
        /// Set Justification
        ///
        /// @param Justification which will align the items within the list
        //---------------------------------------------------------
        void CHorizontalList::SetJustification(ItemJustification ineJustification)
        {
            Justifiction = ineJustification;
        }
        //---------------------------------------------------------
        /// Get Justification
        ///
        /// @return Justification which will align the items within the list
        //---------------------------------------------------------
        CHorizontalList::ItemJustification CHorizontalList::GetJustification() const
        {
            return Justifiction;
        }
        //---------------------------------------------------------
        /// Set Vertical Justification
        ///
        /// @param Justification which will align the items within the list
        //---------------------------------------------------------
        void CHorizontalList::SetVerticalJustification(ItemVerticalJustification ineJustification)
        {
            VerticalJustifiction = ineJustification;
            
            switch( VerticalJustifiction )
            {
                case JUSTIFY_TOP:
                    meVerticalAlignmentAnchor = Core::ALIGN_TOP_LEFT;
                    break;
                case JUSTIFY_MIDDLE:
                    meVerticalAlignmentAnchor = Core::ALIGN_MIDDLE_LEFT;
                    break;
                case JUSTIFY_BOTTOM:
                    meVerticalAlignmentAnchor = Core::ALIGN_BOTTOM_LEFT;
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
				Core::CVector2 vTopRight = GetAbsoluteScreenSpaceAnchorPoint(Core::ALIGN_TOP_RIGHT);
				Core::CVector2 vBottomLeft = GetAbsoluteScreenSpaceAnchorPoint(Core::ALIGN_BOTTOM_LEFT);
				
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
            
            switch(Justifiction)
            {
                case JUSTIFY_LEFT:
                default:
                    break;
                case JUSTIFY_RIGHT:
                {
                    f32 fOffset = GetAbsoluteSize().x - fX;
                    for(Subviews::iterator pView = maListSubviews.begin(); pView != maListSubviews.end(); ++pView)
                    {
                        (*pView)->SetOffsetFromParentAlignment(0.0f, 0.0f, fOffset + (*pView)->GetOffsetFromParentAlignment().vAbsolute.x, 0.0f);
                    }
                    break;
                }
                case JUSTIFY_CENTRE:
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

