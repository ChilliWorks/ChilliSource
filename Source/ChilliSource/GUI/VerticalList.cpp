//
//  VerticalList.cpp
//  MoFlow
//
//  Created by Scott Downie on 1/07/2013.
//  Copyright (c) 2013 Tag Games. All rights reserved.
//

#include <moFlo/GUI/VerticalList.h>

#include <moFlo/Core/Screen.h>

namespace moFlo
{
    namespace GUI
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
        : Justifiction(JUSTIFY_TOP), AbsoluteSpacing(0.0f), RelativeSpacing(0.0f), meHorizontalAlignmentAnchor(Core::ALIGN_TOP_CENTRE)
        {
            
        }
        //---------------------------------------------------------
        /// Constructor
        ///
        /// @param Dictionary of params
        //---------------------------------------------------------
        CVerticalList::CVerticalList(const Core::ParamDictionary& insParams)
        : CGUIView(insParams), Justifiction(JUSTIFY_TOP), AbsoluteSpacing(0.0f), meHorizontalAlignmentAnchor(Core::ALIGN_TOP_CENTRE), RelativeSpacing(0.0f)
        {
            std::string strValue;
            
            //---Set Justification
            if(insParams.TryGetValue("Justification", strValue))
            {
                if(strValue == "Top")
                {
                    SetJustification(JUSTIFY_TOP);
                }
                else if(strValue == "Middle")
                {
                    SetJustification(JUSTIFY_MIDDLE);
                }
                else if(strValue == "Bottom")
                {
                    SetJustification(JUSTIFY_BOTTOM);
                }
            }
            
            //---Set Justification
            if(insParams.TryGetValue("HorizontalJustification", strValue))
            {
                if(strValue == "Centre")
                {
                    SetHorizontalJustification(JUSTIFY_CENTRE);
                }
                else if(strValue == "Left")
                {
                    SetHorizontalJustification(JUSTIFY_LEFT);
                }
                else if(strValue == "Right")
                {
                    SetHorizontalJustification(JUSTIFY_RIGHT);
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
        /// Set Justification
        ///
        /// @param Justification which will align the items within the list
        //---------------------------------------------------------
        void CVerticalList::SetJustification(ItemJustification ineJustification)
        {
            Justifiction = ineJustification;
        }
        //---------------------------------------------------------
        /// Get Justification
        ///
        /// @return Justification which will align the items within the list
        //---------------------------------------------------------
        CVerticalList::ItemJustification CVerticalList::GetJustification() const
        {
            return Justifiction;
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
                case JUSTIFY_LEFT:
                    meHorizontalAlignmentAnchor = Core::ALIGN_TOP_LEFT;
                    break;
                case JUSTIFY_CENTRE:
                    meHorizontalAlignmentAnchor = Core::ALIGN_TOP_CENTRE;
                    break;
                case JUSTIFY_RIGHT:
                    meHorizontalAlignmentAnchor = Core::ALIGN_TOP_RIGHT;
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
            
            switch(Justifiction)
            {
                case JUSTIFY_TOP:
                default:
                    break;
                case JUSTIFY_BOTTOM:
                {
                    f32 fOffset = -(GetAbsoluteSize().y + fY);
                    for(Subviews::iterator pView = maListSubviews.begin(); pView != maListSubviews.end(); ++pView)
                    {
                        (*pView)->SetOffsetFromParentAlignment(0.0f, 0.0f, 0.0f, fOffset + (*pView)->GetOffsetFromParentAlignment().vAbsolute.y);
                    }
                    break;
                }
                case JUSTIFY_MIDDLE:
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

