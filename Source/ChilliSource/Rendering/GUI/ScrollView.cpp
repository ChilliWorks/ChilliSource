//
//  ScrollView.cpp
//  moFlo
//
//  Created by Scott Downie on 27/04/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#include <ChilliSource/Rendering/GUI/ScrollView.h>

#include <ChilliSource/Core/Math/MathUtils.h>

#if DEBUG_DRAWING
    #include <ChilliSource/Rendering/Base/CanvasRenderer.h>
    #include <ChilliSource/Core/Resource/ResourceManagerDispenser.h>
    #include <ChilliSource/Rendering/Texture/Texture.h>
    #include <ChilliSource/Rendering/Texture/TextureManager.h>
#endif

namespace ChilliSource
{
    namespace Rendering
    {
		DEFINE_META_CLASS(CScrollView)

		DEFINE_PROPERTY(ScrollHorizontally);
		DEFINE_PROPERTY(ScrollVertically);

        const f32 kfScrollDeceleration = 0.9f;
		const f32 kfScrollTouchTravelToIgnoreChildrenSqrd = 64.0f;
        
        //--------------------------------------------
        /// Constructor
        ///
        /// Default
        //--------------------------------------------
        CScrollView::CScrollView() 
		: ScrollHorizontally(true), ScrollVertically(true), mpContainerView(new CGUIView), mbTouchMoved(false), mbTouchActive(false), mfTouchTravel(0.0f),mbDrawDebug(false)
        {
            //A scroll view that doesn't clip is useless
            EnableSubviewClipping(true);
            
            //Lets give the scroll view an empty container that we can check bounds against
            //this container will expand to hold all it's children
            mpContainerView->SetSize(1.0f, 1.0f, 0.0f, 0.0f);
            mpContainerView->SetLocalAlignment(Core::AlignmentAnchor::k_topLeft);
            mpContainerView->EnableAlignmentToParent(true);
            mpContainerView->SetAlignmentToParent(Core::AlignmentAnchor::k_topLeft);
            mpContainerView->EnableTouchConsumption(false);
            CGUIView::AddSubview(mpContainerView);
        }
        //--------------------------------------------
        /// Constructor
        ///
        /// From param dictionary
        //--------------------------------------------
        CScrollView::CScrollView(const Core::ParamDictionary& insParams) 
		: CGUIView(insParams), ScrollHorizontally(true), ScrollVertically(true), mpContainerView(new CGUIView), mbTouchMoved(false), mbTouchActive(false), mfTouchTravel(0.0f),mbDrawDebug(false)
        {
            //A scroll view that doesn't clip is useless
            EnableSubviewClipping(true);
            
            std::string strValue;
            
            //---Enable Horizontal scrolling
            if(insParams.TryGetValue("ScrollHorizontally", strValue))
            {
                ScrollHorizontally = Core::CStringConverter::ParseBool(strValue);
            }
            //---Enable Vertical scrolling
            if(insParams.TryGetValue("ScrollVertically", strValue))
            {
                ScrollVertically = Core::CStringConverter::ParseBool(strValue);
            }
            
            //Lets give the scroll view an empty container that we can check bounds against
            //this container will expand to hold all it's children
            mpContainerView->SetSize(1.0f, 1.0f, 0.0f, 0.0f);
            mpContainerView->SetLocalAlignment(Core::AlignmentAnchor::k_topLeft);
            mpContainerView->EnableAlignmentToParent(true);
            mpContainerView->SetAlignmentToParent(Core::AlignmentAnchor::k_topLeft);
            mpContainerView->EnableTouchConsumption(false);
            CGUIView::AddSubview(mpContainerView);
        }
        //-----------------------------------------------------
        /// Add Subview
        ///
        /// Add a view to the hierarchy
        ///
        /// @param GUIView shared pointer
        //-----------------------------------------------------
        void CScrollView::AddSubview(const GUIViewPtr& inpSubview)
        {
            mpContainerView->AddSubview(inpSubview);
        }
        //-----------------------------------------------------
        /// Remove Subview (Internal)
        ///
        /// Remove a view from our hierarchy
        ///
        /// @param GUIView pointer
        //-----------------------------------------------------
        void CScrollView::RemoveSubview(CGUIView* inpSubview)
        {
            mpContainerView->RemoveSubview(inpSubview);
        }
        //-----------------------------------------------------------
        /// Enable Horizontal Scrolling
        ///
        /// @param Whether the scroll view allows sideways scrolling
        //-----------------------------------------------------------
        void CScrollView::EnableHorizontalScrolling(bool inbEnabled)
        {
            ScrollHorizontally = inbEnabled;
        }
        //-----------------------------------------------------------
        /// Enable Vertical Scrolling
        ///
        /// @param Whether the scroll view allows vertical scrolling
        //-----------------------------------------------------------
        void CScrollView::EnableVerticalScrolling(bool inbEnabled)
        {
            ScrollVertically = inbEnabled;
        }
		//-----------------------------------------------------------
		/// Is Horizontal Scrolling Enabled
		///
		/// @return Whether the scroll view allows sideways scrolling
		//-----------------------------------------------------------
		bool CScrollView::IsHorizontalScrollingEnabled() const
		{
			return ScrollHorizontally;
		}
		//-----------------------------------------------------------
		/// Is Vertical Scrolling Enabled
		///
		/// @return Whether the scroll view allows vertical scrolling
		//-----------------------------------------------------------
		bool CScrollView::IsVerticalScrollingEnabled() const
		{
			return ScrollVertically;
		}
        //-----------------------------------------------------
        /// Update
        ///
        /// @param Time between frames
        //-----------------------------------------------------
        void CScrollView::Update(f32 infDt)
        {
            if(Visible)
            {
                //Check if the container exceeds the bounds of the scroll view
				//Get edge positions
				Core::CVector2 vTopLeft = GetAbsoluteScreenSpaceAnchorPoint(Core::AlignmentAnchor::k_topLeft);
				Core::CVector2 vBottomRight = GetAbsoluteScreenSpaceAnchorPoint(Core::AlignmentAnchor::k_bottomRight);
				
				//We don't want the scrollable items to fly into oblivion we must cap them.
				//The objects can only move in a direction until the furthest object in that direction is within the scroll view
				//at this point we "bounce" the objects.
				
				Core::CVector2 vNewLeftPosition = mvVelocity + mpContainerView->GetAbsoluteScreenSpaceAnchorPoint(Core::AlignmentAnchor::k_topLeft);
				Core::CVector2 vNewRightPosition = mvVelocity + mpContainerView->GetAbsoluteScreenSpaceAnchorPoint(Core::AlignmentAnchor::k_bottomRight);
				
				Core::CVector2 vSizeOfContainer = mpContainerView->GetAbsoluteScreenSpaceAnchorPoint(Core::AlignmentAnchor::k_topRight) - mpContainerView->GetAbsoluteScreenSpaceAnchorPoint(Core::AlignmentAnchor::k_bottomLeft);
				
				if(vSizeOfContainer.x > vBottomRight.x - vTopLeft.x)
				{
					// AM: Make sure we're not going to fly past the left edge
					if(vNewLeftPosition.x >= vTopLeft.x)
					{
						mvVelocity.x = vTopLeft.x - mpContainerView->GetAbsoluteScreenSpaceAnchorPoint(Core::AlignmentAnchor::k_topLeft).x;
					}
                    // AM: Make sure we're not going to fly past the right edge
					else if(vNewRightPosition.x <= vBottomRight.x)
                    {
                        mvVelocity.x = vBottomRight.x - mpContainerView->GetAbsoluteScreenSpaceAnchorPoint(Core::AlignmentAnchor::k_bottomRight).x;
                    } 
				}
				else
                {
					mvVelocity.x = 0;
                }
				
				if(vSizeOfContainer.y > vTopLeft.y - vBottomRight.y)
				{
					// AM: Make sure we're not going to fly past the top edge
					if(vNewLeftPosition.y <= vTopLeft.y)
					{
						mvVelocity.y = vTopLeft.y - mpContainerView->GetAbsoluteScreenSpaceAnchorPoint(Core::AlignmentAnchor::k_topLeft).y;
					}
                    // AM: Make sure we're not going to fly past the bottom edge
					else if(vNewRightPosition.y >= vBottomRight.y)
                    {
                        mvVelocity.y = vBottomRight.y - mpContainerView->GetAbsoluteScreenSpaceAnchorPoint(Core::AlignmentAnchor::k_bottomRight).y;
                    }
				}
				else
                {
					mvVelocity.y = 0;
				}
                
                mpContainerView->MoveBy(0.0f, 0.0f, mvVelocity.x, mvVelocity.y);
				//Decelaration
				if(mbTouchActive && !mbTouchMoved)
				{
					mvVelocity = Core::CVector2::ZERO;
				}
				else if(!mbTouchActive)
				{
					mvVelocity *= kfScrollDeceleration;
				}
				if(mbTouchMoved)
				{
					mvRealPreviousTouchPosition = mvNextRealPreviousTouchPosition;
					mbTouchMoved = false;
				}
				
                CGUIView::Update(infDt);
			}
        }
        //-----------------------------------------------------
        /// Reset
        ///
        /// Resets the scroller back to the default
        //-----------------------------------------------------
        void CScrollView::Reset()
        {
            mvVelocity = Core::CVector2::ZERO;
            mpContainerView->SetOffsetFromParentAlignment(UnifiedVector2(Core::CVector2::ZERO, Core::CVector2::ZERO));
        }
        //-----------------------------------------------------
        /// Jump To
        ///
        /// Jumps to the given position
        ///
        /// @param The new position
        //-----------------------------------------------------
        void CScrollView::JumpTo(const UnifiedVector2& inuvPosition)
        {
            Reset();
            Core::CVector2 vAbsPos = inuvPosition.GetAbsolute() + (inuvPosition.GetRelative()*mpContainerView->GetAbsoluteSize());
            mpContainerView->MoveBy(0.0f, 0.0f, vAbsPos.x, vAbsPos.y);
        }
        //-----------------------------------------------------
        /// Set Absolute Content Size
        ///
        /// @param Content size
        //-----------------------------------------------------
        void CScrollView::SetAbsoluteContentSize(const Core::CVector2& invSize)
        {
            mpContainerView->SetSize(0.0f, 0.0f, invSize.x, invSize.y);
        }
        //-----------------------------------------------------
        /// Get Absolute Content Size
        ///
        /// @return Content size
        //-----------------------------------------------------
        Core::CVector2 CScrollView::GetAbsoluteContentSize() const
        {
            if(!mpContainerView)
                return Core::CVector2::ZERO;
            return mpContainerView->GetAbsoluteSize();
        }
        //-----------------------------------------------------------
        /// Get Absolute Content Position
        ///
        /// @return The current absolute position of the content
        /// from the top left corner of the scroll view
        //-----------------------------------------------------------
        Core::CVector2 CScrollView::GetAbsoluteContentPosition() const
        {
            if(!mpContainerView)
                return Core::CVector2::ZERO;
            return mpContainerView->GetAbsoluteScreenSpaceAnchorPoint(Core::AlignmentAnchor::k_topLeft) - GetAbsoluteScreenSpaceAnchorPoint(Core::AlignmentAnchor::k_topLeft);
        }
        //-----------------------------------------------------
        /// Set Velocity
        ///
        /// @param Velocity
        //-----------------------------------------------------
        void CScrollView::SetVelocity(const Core::CVector2& invVelocity)
        {
            mvVelocity = invVelocity;
        }
        //-----------------------------------------------------------
        /// On Touch Began
        ///
        /// Called when the window receives cursor/touch input
        ///
        /// @param Touch data
        /// @return Whether touch has been consumed
        //-----------------------------------------------------------
        bool CScrollView::OnTouchBegan(const Input::TouchInfo& insTouchInfo)
        {
            if(UserInteraction && Visible)
            {
				mbTouchActive = true;
				mvRealPreviousTouchPosition = insTouchInfo.vLocation;
				
                //Stop! Hammer time...
                mvVelocity = Core::CVector2::ZERO; 
				mfTouchTravel = 0.0f;
            }
            
            return CGUIView::OnTouchBegan(insTouchInfo);
        }
        //-----------------------------------------------------------
        /// On Touch Moved
        ///
        /// Called when the window receives cursor/touch input
        ///
        /// @param Touch data
        /// @return Whether touch has been consumed
        //-----------------------------------------------------------
        bool CScrollView::OnTouchMoved(const Input::TouchInfo& insTouchInfo)
        {
            if(UserInteraction && Visible && mbTouchActive && Contains(insTouchInfo.vLocation))
            {
                //Calculate the displacement
                mvVelocity = insTouchInfo.vLocation - mvRealPreviousTouchPosition;
				mfTouchTravel += mvVelocity.LengthSquared();
				
                if(!ScrollHorizontally)
                {
                    mvVelocity.x = 0.0f;
                }
                if(!ScrollVertically)
                {
                    mvVelocity.y = 0.0f;
                }
				
				mvNextRealPreviousTouchPosition = insTouchInfo.vLocation;
				mbTouchMoved = true;
				
				CGUIView::OnTouchMoved(insTouchInfo);
            }
            
            return false;
        }
		//-----------------------------------------------------------
        /// On Touch Ended
        ///
        /// Called when the window receives cursor/touch input
        ///
        /// @param Touch data
        //-----------------------------------------------------------
        void CScrollView::OnTouchEnded(const Input::TouchInfo& insTouchInfo)
        {
			if(UserInteraction && Visible)
			{
				mbTouchActive = false;
			}

            CGUIView::OnTouchEnded(insTouchInfo);

        }
        
        //-------------------------------------------------------
        /// Draw
        ///
        /// Draw all our subviews in neat rows and columns. Each
        /// cell is based on the size of the largest content
        ///
        /// @param Canvas renderer pointer
        //-------------------------------------------------------
        void CScrollView::Draw(CCanvasRenderer* inpCanvas)
        {
#if DEBUG_DRAWING
            if(mbDrawDebug)
            {
                ITextureManager* pMgr = (ITextureManager*)(Core::CResourceManagerDispenser::GetSingletonPtr()->GetResourceManagerForType(ITexture::InterfaceID));
                inpCanvas->DrawBox(GetTransform(), GetAbsoluteSize(), pMgr->GetDefaultTexture(), Core::Rectangle(Core::CVector2::ZERO, Core::CVector2::ZERO), Core::CColour(1.0f,0.0f,0.0f,0.5f));
            }
#endif
        
            CGUIView::Draw(inpCanvas);
        }
		//-------------------------------------------------------
		/// Sets Debug Drawing
		///
		/// Enables/Disables debug drawing
		///
		/// @param New value for this flag. DEBUG_DRAWING must be
		/// set to TRUE
		//-------------------------------------------------------
		void CScrollView::EnableDebugDrawing(bool inbValue)
		{
#if DEBUG_DRAWING
			mbDrawDebug = inbValue;
#else
			mbDrawDebug = false;
#endif
		}
        //-----------------------------------------------------------
        /// Get Container View
        ///
        /// @return The GUIView that contains all scrollable subviews
        /// within the scroll view.
        //-----------------------------------------------------------
        const GUIViewPtr& CScrollView::GetContainerView() const
        {
            return mpContainerView;
        }
    }
}
