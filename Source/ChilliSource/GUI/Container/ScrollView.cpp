//
//  ScrollView.cpp
//  moFlo
//
//  Created by Scott Downie on 27/04/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#include <ChilliSource/GUI/Container/ScrollView.h>

#include <ChilliSource/Core/Math/MathUtils.h>
#include <ChilliSource/Core/String/StringParser.h>

#if CS_ENABLE_DEBUGDRAWING
    #include <ChilliSource/Rendering/Base/CanvasRenderer.h>
    #include <ChilliSource/Core/Resource/ResourceManagerDispenser.h>
    #include <ChilliSource/Rendering/Texture/Texture.h>
    #include <ChilliSource/Rendering/Texture/TextureManager.h>
#endif

namespace ChilliSource
{
    namespace GUI
    {
		DEFINE_META_CLASS(ScrollView)

		DEFINE_PROPERTY(ScrollHorizontally);
		DEFINE_PROPERTY(ScrollVertically);

        const f32 kfScrollDeceleration = 0.9f;
        
        //--------------------------------------------
        /// Constructor
        ///
        /// Default
        //--------------------------------------------
        ScrollView::ScrollView() 
		: ScrollHorizontally(true), ScrollVertically(true), mpContainerView(new GUIView), mbTouchMoved(false), mbTouchActive(false), mfTouchTravel(0.0f),mbDrawDebug(false)
        {
            //A scroll view that doesn't clip is useless
            EnableSubviewClipping(true);
            
            //Lets give the scroll view an empty container that we can check bounds against
            //this container will expand to hold all it's children
            mpContainerView->SetSize(1.0f, 1.0f, 0.0f, 0.0f);
            mpContainerView->SetLocalAlignment(Rendering::AlignmentAnchor::k_topLeft);
            mpContainerView->EnableAlignmentToParent(true);
            mpContainerView->SetAlignmentToParent(Rendering::AlignmentAnchor::k_topLeft);
            mpContainerView->EnableTouchConsumption(false);
            GUIView::AddSubview(mpContainerView);
        }
        //--------------------------------------------
        /// Constructor
        ///
        /// From param dictionary
        //--------------------------------------------
        ScrollView::ScrollView(const Core::ParamDictionary& insParams) 
		: GUIView(insParams), ScrollHorizontally(true), ScrollVertically(true), mpContainerView(new GUIView), mbTouchMoved(false), mbTouchActive(false), mfTouchTravel(0.0f),mbDrawDebug(false)
        {
            //A scroll view that doesn't clip is useless
            EnableSubviewClipping(true);
            
            std::string strValue;
            
            //---Enable Horizontal scrolling
            if(insParams.TryGetValue("ScrollHorizontally", strValue))
            {
                ScrollHorizontally = Core::ParseBool(strValue);
            }
            //---Enable Vertical scrolling
            if(insParams.TryGetValue("ScrollVertically", strValue))
            {
                ScrollVertically = Core::ParseBool(strValue);
            }
            
            //Lets give the scroll view an empty container that we can check bounds against
            //this container will expand to hold all it's children
            mpContainerView->SetSize(1.0f, 1.0f, 0.0f, 0.0f);
            mpContainerView->SetLocalAlignment(Rendering::AlignmentAnchor::k_topLeft);
            mpContainerView->EnableAlignmentToParent(true);
            mpContainerView->SetAlignmentToParent(Rendering::AlignmentAnchor::k_topLeft);
            mpContainerView->EnableTouchConsumption(false);
            GUIView::AddSubview(mpContainerView);
        }
        //-----------------------------------------------------
        /// Add Subview
        ///
        /// Add a view to the hierarchy
        ///
        /// @param GUIView shared pointer
        //-----------------------------------------------------
        void ScrollView::AddSubview(const GUIViewSPtr& inpSubview)
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
        void ScrollView::RemoveSubview(GUIView* inpSubview)
        {
            mpContainerView->RemoveSubview(inpSubview);
        }
        //-----------------------------------------------------------
        /// Enable Horizontal Scrolling
        ///
        /// @param Whether the scroll view allows sideways scrolling
        //-----------------------------------------------------------
        void ScrollView::EnableHorizontalScrolling(bool inbEnabled)
        {
            ScrollHorizontally = inbEnabled;
        }
        //-----------------------------------------------------------
        /// Enable Vertical Scrolling
        ///
        /// @param Whether the scroll view allows vertical scrolling
        //-----------------------------------------------------------
        void ScrollView::EnableVerticalScrolling(bool inbEnabled)
        {
            ScrollVertically = inbEnabled;
        }
		//-----------------------------------------------------------
		/// Is Horizontal Scrolling Enabled
		///
		/// @return Whether the scroll view allows sideways scrolling
		//-----------------------------------------------------------
		bool ScrollView::IsHorizontalScrollingEnabled() const
		{
			return ScrollHorizontally;
		}
		//-----------------------------------------------------------
		/// Is Vertical Scrolling Enabled
		///
		/// @return Whether the scroll view allows vertical scrolling
		//-----------------------------------------------------------
		bool ScrollView::IsVerticalScrollingEnabled() const
		{
			return ScrollVertically;
		}
        //-----------------------------------------------------
        /// Update
        ///
        /// @param Time between frames
        //-----------------------------------------------------
        void ScrollView::Update(f32 infDt)
        {
            if(Visible)
            {
                //Check if the container exceeds the bounds of the scroll view
				//Get edge positions
				Core::Vector2 vTopLeft = GetAbsoluteScreenSpaceAnchorPoint(Rendering::AlignmentAnchor::k_topLeft);
				Core::Vector2 vBottomRight = GetAbsoluteScreenSpaceAnchorPoint(Rendering::AlignmentAnchor::k_bottomRight);
				
				//We don't want the scrollable items to fly into oblivion we must cap them.
				//The objects can only move in a direction until the furthest object in that direction is within the scroll view
				//at this point we "bounce" the objects.
				
				Core::Vector2 vNewLeftPosition = mvVelocity + mpContainerView->GetAbsoluteScreenSpaceAnchorPoint(Rendering::AlignmentAnchor::k_topLeft);
				Core::Vector2 vNewRightPosition = mvVelocity + mpContainerView->GetAbsoluteScreenSpaceAnchorPoint(Rendering::AlignmentAnchor::k_bottomRight);
				
				Core::Vector2 vSizeOfContainer = mpContainerView->GetAbsoluteScreenSpaceAnchorPoint(Rendering::AlignmentAnchor::k_topRight) - mpContainerView->GetAbsoluteScreenSpaceAnchorPoint(Rendering::AlignmentAnchor::k_bottomLeft);
				
				if(vSizeOfContainer.x > vBottomRight.x - vTopLeft.x)
				{
					// AM: Make sure we're not going to fly past the left edge
					if(vNewLeftPosition.x >= vTopLeft.x)
					{
						mvVelocity.x = vTopLeft.x - mpContainerView->GetAbsoluteScreenSpaceAnchorPoint(Rendering::AlignmentAnchor::k_topLeft).x;
					}
                    // AM: Make sure we're not going to fly past the right edge
					else if(vNewRightPosition.x <= vBottomRight.x)
                    {
                        mvVelocity.x = vBottomRight.x - mpContainerView->GetAbsoluteScreenSpaceAnchorPoint(Rendering::AlignmentAnchor::k_bottomRight).x;
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
						mvVelocity.y = vTopLeft.y - mpContainerView->GetAbsoluteScreenSpaceAnchorPoint(Rendering::AlignmentAnchor::k_topLeft).y;
					}
                    // AM: Make sure we're not going to fly past the bottom edge
					else if(vNewRightPosition.y >= vBottomRight.y)
                    {
                        mvVelocity.y = vBottomRight.y - mpContainerView->GetAbsoluteScreenSpaceAnchorPoint(Rendering::AlignmentAnchor::k_bottomRight).y;
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
					mvVelocity = Core::Vector2::ZERO;
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
				
                GUIView::Update(infDt);
			}
        }
        //-----------------------------------------------------
        /// Reset
        ///
        /// Resets the scroller back to the default
        //-----------------------------------------------------
        void ScrollView::Reset()
        {
            mvVelocity = Core::Vector2::ZERO;
            mpContainerView->SetOffsetFromParentAlignment(Core::UnifiedVector2(Core::Vector2::ZERO, Core::Vector2::ZERO));
        }
        //-----------------------------------------------------
        /// Jump To
        ///
        /// Jumps to the given position
        ///
        /// @param The new position
        //-----------------------------------------------------
        void ScrollView::JumpTo(const Core::UnifiedVector2& inuvPosition)
        {
            Reset();
            Core::Vector2 vAbsPos = inuvPosition.GetAbsolute() + (inuvPosition.GetRelative()*mpContainerView->GetAbsoluteSize());
            mpContainerView->MoveBy(0.0f, 0.0f, vAbsPos.x, vAbsPos.y);
        }
        //-----------------------------------------------------
        /// Set Absolute Content Size
        ///
        /// @param Content size
        //-----------------------------------------------------
        void ScrollView::SetAbsoluteContentSize(const Core::Vector2& invSize)
        {
            mpContainerView->SetSize(0.0f, 0.0f, invSize.x, invSize.y);
        }
        //-----------------------------------------------------
        /// Get Absolute Content Size
        ///
        /// @return Content size
        //-----------------------------------------------------
        Core::Vector2 ScrollView::GetAbsoluteContentSize() const
        {
            if(!mpContainerView)
                return Core::Vector2::ZERO;
            return mpContainerView->GetAbsoluteSize();
        }
        //-----------------------------------------------------------
        /// Get Absolute Content Position
        ///
        /// @return The current absolute position of the content
        /// from the top left corner of the scroll view
        //-----------------------------------------------------------
        Core::Vector2 ScrollView::GetAbsoluteContentPosition() const
        {
            if(!mpContainerView)
                return Core::Vector2::ZERO;
            return mpContainerView->GetAbsoluteScreenSpaceAnchorPoint(Rendering::AlignmentAnchor::k_topLeft) - GetAbsoluteScreenSpaceAnchorPoint(Rendering::AlignmentAnchor::k_topLeft);
        }
        //-----------------------------------------------------
        /// Set Velocity
        ///
        /// @param Velocity
        //-----------------------------------------------------
        void ScrollView::SetVelocity(const Core::Vector2& invVelocity)
        {
            mvVelocity = invVelocity;
        }
        //-----------------------------------------------------------
        //-----------------------------------------------------------
        bool ScrollView::OnPointerDown(const Input::PointerSystem::Pointer& in_pointer, f64 in_timestamp, Input::PointerSystem::PressType in_pressType)
        {
            if(UserInteraction && Visible)
            {
				mbTouchActive = true;
				mvRealPreviousTouchPosition = in_pointer.m_location;
				
                //Stop! Hammer time...
                mvVelocity = Core::Vector2::ZERO; 
				mfTouchTravel = 0.0f;
            }
            
            return GUIView::OnPointerDown(in_pointer, in_timestamp, in_pressType);
        }
        //-----------------------------------------------------------
        //-----------------------------------------------------------
        bool ScrollView::OnPointerMoved(const Input::PointerSystem::Pointer& in_pointer, f64 in_timestamp)
        {
            if(UserInteraction && Visible && mbTouchActive && Contains(in_pointer.m_location))
            {
                //Calculate the displacement
                mvVelocity = in_pointer.m_location - mvRealPreviousTouchPosition;
				mfTouchTravel += mvVelocity.LengthSquared();
				
                if(!ScrollHorizontally)
                {
                    mvVelocity.x = 0.0f;
                }
                if(!ScrollVertically)
                {
                    mvVelocity.y = 0.0f;
                }
				
				mvNextRealPreviousTouchPosition = in_pointer.m_location;
				mbTouchMoved = true;
				
				GUIView::OnPointerMoved(in_pointer, in_timestamp);
            }
            
            return false;
        }
		//-----------------------------------------------------------
        //-----------------------------------------------------------
        void ScrollView::OnPointerUp(const Input::PointerSystem::Pointer& in_pointer, f64 in_timestamp, Input::PointerSystem::PressType in_pressType)
        {
			if(UserInteraction && Visible)
			{
				mbTouchActive = false;
			}

            GUIView::OnPointerUp(in_pointer, in_timestamp, in_pressType);
        }
        //-------------------------------------------------------
        /// Draw
        ///
        /// Draw all our subviews in neat rows and columns. Each
        /// cell is based on the size of the largest content
        ///
        /// @param Canvas renderer pointer
        //-------------------------------------------------------
        void ScrollView::Draw(Rendering::CanvasRenderer* inpCanvas)
        {
#if CS_ENABLE_DEBUGDRAWING
            if(mbDrawDebug)
            {
                Rendering::TextureManager* pMgr = (Rendering::TextureManager*)(Core::ResourceManagerDispenser::GetSingletonPtr()->GetResourceManagerForType(Rendering::Texture::InterfaceID));
                inpCanvas->DrawBox(GetTransform(), GetAbsoluteSize(), pMgr->GetDefaultTexture(), Core::Rectangle(Core::Vector2::ZERO, Core::Vector2::ZERO), Core::Colour(1.0f,0.0f,0.0f,0.5f));
            }
#endif
        
            GUIView::Draw(inpCanvas);
        }
		//-------------------------------------------------------
		/// Sets Debug Drawing
		///
		/// Enables/Disables debug drawing
		///
		/// @param New value for this flag. DEBUG_DRAWING must be
		/// set to TRUE
		//-------------------------------------------------------
		void ScrollView::EnableDebugDrawing(bool inbValue)
		{
#if CS_ENABLE_DEBUGDRAWING
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
        const GUIViewSPtr& ScrollView::GetContainerView() const
        {
            return mpContainerView;
        }
    }
}
