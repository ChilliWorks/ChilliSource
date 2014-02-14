//
//  GUIView.cpp
//  moFloTest
//
//  Created by Scott Downie on 21/04/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#include <ChilliSource/Rendering/GUI/GUIView.h>

#include <ChilliSource/Core/String/StringConverter.h>
#include <ChilliSource/Core/Math/MathUtils.h>
#include <ChilliSource/Core/Main/Screen.h>
#include <ChilliSource/Core/Main/BitOps.h>

#include <ChilliSource/Rendering/Main/CanvasRenderer.h>

#include <algorithm>
#include <limits>

namespace moFlo
{
    namespace GUI
    {
        static GUIViewPtr pGUINull;

		DEFINE_META_CLASS(CGUIView)

		//---Properties
		DEFINE_PROPERTY(Name);
		DEFINE_PROPERTY(UnifiedPosition);
		DEFINE_PROPERTY(UnifiedSize);
		DEFINE_PROPERTY(UnifiedParentalOffset);
		DEFINE_PROPERTY(UnifiedAlignmentOffset);
		DEFINE_PROPERTY(Scale);
		DEFINE_PROPERTY(Rotation);
		DEFINE_PROPERTY(Opacity);
		DEFINE_PROPERTY(LocalAlignment);
		DEFINE_PROPERTY(ParentalAlignment);
		DEFINE_PROPERTY(AlignedWithParent);
		DEFINE_PROPERTY(RotatedWithParent);
		DEFINE_PROPERTY(ClipOffScreen);
		DEFINE_PROPERTY(ClipSubviews);
		DEFINE_PROPERTY(InheritScale);
		DEFINE_PROPERTY(InheritColour);
		DEFINE_PROPERTY(Visible);
		DEFINE_PROPERTY(Movable);
		DEFINE_PROPERTY(UserInteraction);
		DEFINE_PROPERTY(AcceptTouchesOutsideOfBounds);
		DEFINE_PROPERTY(InheritOpacity);
		DEFINE_PROPERTY(Colour);

        //-----------------------------------------------------
        /// Constructor
        ///
        /// Default
        //-----------------------------------------------------
        CGUIView::CGUIView() : mpParentView(NULL), mpRootWindow(NULL), mbIsBeingDragged(false), mudwCacheValidaters(0), UnifiedPosition(0.5f, 0.5f, 0.0f, 0.0f), UnifiedSize(1.0f, 1.0f, 0.0f, 0.0f),
			Rotation(0.0f), Opacity(1.0f), LocalAlignment(Core::ALIGN_MIDDLE_CENTRE), ParentalAlignment(Core::ALIGN_BOTTOM_LEFT), AlignedWithParent(false),
			Scale(Core::CVector2::ONE), ClipSubviews(false), InheritColour(true), Visible(true), Movable(false), UserInteraction(true), ConsumesTouches(TOUCH_TYPE_ALL),
			AcceptTouchesOutsideOfBounds(false), InheritOpacity(true), RotatedWithParent(true), InheritScale(false), ClipOffScreen(true)
        {

        }
        //------------------------------------------------------
        /// Constructor
        ///
        /// Construct view from given key values
        ///
        /// @param Param dictionary
        //------------------------------------------------------
        CGUIView::CGUIView(const Core::ParamDictionary& insParams) 
		: mpParentView(NULL), mpRootWindow(NULL), mbIsBeingDragged(false), mudwCacheValidaters(0), UnifiedPosition(0.5f, 0.5f, 0.0f, 0.0f), UnifiedSize(1.0f, 1.0f, 0.0f, 0.0f),
			Rotation(0.0f), Opacity(1.0f), LocalAlignment(Core::ALIGN_MIDDLE_CENTRE), ParentalAlignment(Core::ALIGN_BOTTOM_LEFT), AlignedWithParent(false),
			Scale(Core::CVector2::ONE), ClipSubviews(false), InheritColour(true), Visible(true), Movable(false), UserInteraction(true), ConsumesTouches(TOUCH_TYPE_ALL),
			AcceptTouchesOutsideOfBounds(false), InheritOpacity(true), RotatedWithParent(true), InheritScale(false), ClipOffScreen(true)
        {
            std::string strValue;
            
            //---Name
            if(insParams.TryGetValue("Name", strValue))
            {
                SetName(strValue);
            }
            //---Unified position
            if(insParams.TryGetValue("UnifiedPosition", strValue))
            {
                //Convert this to a vector4 that we can then conver to unified vector2
                Core::CVector4 vRawPosition = Core::CStringConverter::ParseVector4(strValue);
                SetPosition(UnifiedVector2(vRawPosition.x, vRawPosition.y, vRawPosition.z, vRawPosition.w));
            }
            //---Unified size
            if(insParams.TryGetValue("UnifiedSize", strValue))
            {
                //Convert this to a vector4 that we can then conver to unified vector2
                Core::CVector4 vRawSize = Core::CStringConverter::ParseVector4(strValue);
                SetSize(UnifiedVector2(vRawSize.x, vRawSize.y, vRawSize.z, vRawSize.w));
            }
            //---Unified scale
            if(insParams.TryGetValue("Scale", strValue))
            {
                //Convert this to a vector4 that we can then conver to unified vector2
                Core::CVector2 vScale = Core::CStringConverter::ParseVector2(strValue);
                ScaleTo(vScale);
            }
            //---Scale Inheritance
			if (insParams.TryGetValue("InheritScale", strValue))
			{
				InheritScale = Core::CStringConverter::ParseBool(strValue);
			}
            //---Rotation
            if(insParams.TryGetValue("Rotation", strValue))
            {
                RotateTo(Core::CStringConverter::ParseFloat(strValue));
            }
            //---Is rotated with parent
            if(insParams.TryGetValue("RotatedWithParent", strValue))
            {
                RotatedWithParent = Core::CStringConverter::ParseBool(strValue);
            }
            //---Is aligned with parent
            if(insParams.TryGetValue("AlignedWithParent", strValue))
            {
                AlignedWithParent = Core::CStringConverter::ParseBool(strValue);
            }
			//---Enable Clipping Off Screen
			if(insParams.TryGetValue("ClipOffScreen", strValue))
			{
				ClipOffScreen = Core::CStringConverter::ParseBool(strValue);
			}
            //---Enable clipping
            if(insParams.TryGetValue("ClipSubviews", strValue))
            {
                ClipSubviews = Core::CStringConverter::ParseBool(strValue);
            }
            //---Local alignment
            if(insParams.TryGetValue("LocalAlignment", strValue))
            {
                LocalAlignment = Core::AlignmentAnchorFromString(strValue);
            }
            //---Parental alignment
            if(insParams.TryGetValue("ParentalAlignment", strValue))
            {
                ParentalAlignment = Core::AlignmentAnchorFromString(strValue);
            }
            //---Unified parental offset
            if(insParams.TryGetValue("UnifiedParentalOffset", strValue))
            {
                //Convert this to a vector4 that we can then conver to unified vector2
                Core::CVector4 vRawOffset = Core::CStringConverter::ParseVector4(strValue);
                SetOffsetFromParentAlignment(UnifiedVector2(vRawOffset.x, vRawOffset.y, vRawOffset.z, vRawOffset.w));
            }
            //---Unified alignment offset
            if(insParams.TryGetValue("UnifiedPositionOffset", strValue))
            {
                //Convert this to a vector4 that we can then conver to unified vector2
                Core::CVector4 vRawOffset = Core::CStringConverter::ParseVector4(strValue);
                SetOffsetFromPosition(UnifiedVector2(vRawOffset.x, vRawOffset.y, vRawOffset.z, vRawOffset.w));
            }
            //---Enable user interaction
            if(insParams.TryGetValue("UserInteraction", strValue))
            {
                UserInteraction = Core::CStringConverter::ParseBool(strValue);
            }
            //---Enable touch consumption
            if(insParams.TryGetValue("ConsumesTouches", strValue))
            {
				EnableTouchConsumption(Core::CStringConverter::ParseBool(strValue));
            }
            //---Enable touch consumption
            if(insParams.TryGetValue("ConsumesTouchesBegan", strValue))
            {
				EnableTouchConsumption(Core::CStringConverter::ParseBool(strValue), TOUCH_TYPE_BEGAN);
            }
            //---Enable touch consumption
            if(insParams.TryGetValue("ConsumesTouchesMoved", strValue))
            {
				EnableTouchConsumption(Core::CStringConverter::ParseBool(strValue), TOUCH_TYPE_MOVED);
            }
            //---Accept Touches Outside of Bounds
            if(insParams.TryGetValue("AcceptTouchesOutsideOfBounds", strValue))
            {
                EnableAcceptTouchesOutsideOfBounds(Core::CStringConverter::ParseBool(strValue));
            }
            //---Colour
            if(insParams.TryGetValue("Colour", strValue))
            {
                SetColour(Core::CStringConverter::ParseColourValue(strValue));
            }
            //---Colour Inheritance
			if (insParams.TryGetValue("InheritColour", strValue))
			{
				InheritColour = Core::CStringConverter::ParseBool(strValue);
			}
            //---Movable
            if(insParams.TryGetValue("Movable", strValue))
            {
                SetMovable(Core::CStringConverter::ParseBool(strValue));
            }
			//---Visible
            if(insParams.TryGetValue("Visible", strValue))
			{
				Visible = Core::CStringConverter::ParseBool(strValue);
			}
			//---Inherit Opacity
			if (insParams.TryGetValue("InheritOpacity", strValue))
			{
				InheritOpacity = Core::CStringConverter::ParseBool(strValue);
			}
			//---Opacity
			if (insParams.TryGetValue("Opacity", strValue))
			{
				Opacity = Core::CStringConverter::ParseFloat(strValue);
			}
        }
		//-----------------------------------------------------
		/// On Transform Changed
		///
		/// Dirty the transform and force it to be recalculated
		//-----------------------------------------------------
		void CGUIView::OnTransformChanged(u32 inudwInvalidFlags)
		{
			BITMASK_CLEAR(mudwCacheValidaters, inudwInvalidFlags);
			
			for(CGUIView::Subviews::iterator it = mSubviews.begin(); it != mSubviews.end(); ++it)
			{
				(*it)->OnTransformChanged(inudwInvalidFlags);
			}
		}
        //---Hierarchy Manipulation
        //-----------------------------------------------------
        /// Add Subview
        ///
        /// Add a view to the hierarchy
        ///
        /// @param GUIView shared pointer
        //-----------------------------------------------------
        void CGUIView::AddSubview(const GUIViewPtr& inpSubview)
        {
			if(inpSubview->GetParentViewPtr() == this)
				return;
				
            mSubviews.push_back(inpSubview);
			
			if(inpSubview->GetParentViewPtr())
			{
				inpSubview->RemoveFromParentView();
			}
			
            inpSubview->SetParentView(this);
			
			if(mpRootWindow)
			{
				inpSubview->SetRootWindow(mpRootWindow);
                inpSubview->LayoutContent();
                inpSubview->LayoutChildrensContent();
			}
			
			OnSubviewAdded(inpSubview);
        }
        //-----------------------------------------------------
        /// Remove Subview (Internal)
        ///
        /// Remove a view from our hierarchy
        ///
        /// @param GUIView pointer
        //-----------------------------------------------------
        void CGUIView::RemoveSubview(CGUIView* inpSubview)
        {
            for(CGUIView::Subviews::iterator it = mSubviews.begin(); it != mSubviews.end(); ++it)
            {
                if((*it).get() == inpSubview)
                {
                    inpSubview->SetParentView(NULL);
                    inpSubview->SetRootWindow(NULL);
                    mSubviews.erase(it);
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
		void CGUIView::RemoveAllSubviews()
		{
			for(CGUIView::Subviews::iterator it = mSubviews.begin(); it != mSubviews.end(); ++it)
            {
				(*it)->SetParentView(NULL);
				(*it)->SetRootWindow(NULL);
            }
			
			mSubviews.clear();
		}
        //-----------------------------------------------------
        /// Remove From Parent View
        ///
        /// Remove ourself from our current parent
        ///
        /// WARNING: If the parent is the only object
        /// holding a reference to this object then it will
        /// be destroyed (i.e. Don't use this function to
        /// remove yourself within an event)
        //-----------------------------------------------------
        void CGUIView::RemoveFromParentView()
        {
            if(mpParentView)
            {
                mpParentView->RemoveSubview(this);
            }
        }
        
        //-----------------------------------------------------
        /// Layout Content
        ///
        /// Function used to layout GUI content when absolute
        /// sizes are needed. Must be attached to root window.
        //-----------------------------------------------------
        void CGUIView::LayoutContent()
        {
            
        }
        
        //-----------------------------------------------------
        /// Bring Forward
        ///
        /// Move this view a single layer towards the screen
        //-----------------------------------------------------
        void CGUIView::BringForward()
        {
            if(mpParentView)
            {
                mpParentView->BringSubviewForward(this);
            }
        }
        //-----------------------------------------------------
        /// Send Backward
        ///
        /// Move this view a layer away from the screen
        //-----------------------------------------------------
        void CGUIView::SendBackward()
        {
            if(mpParentView)
            {
                mpParentView->SendSubviewBackward(this);
            }
        }
        //-----------------------------------------------------
        /// Bring To Front
        ///
        /// Move this view to the top of the view hierarchy
        //-----------------------------------------------------
        void CGUIView::BringToFront()
        {
            if(mpParentView)
            {
                mpParentView->BringSubviewToFront(this);
            }
        }
        //-----------------------------------------------------
        /// Send To Back
        ///
        /// Move this view to the end of the view hierarchy
        //-----------------------------------------------------
        void CGUIView::SendToBack()
        {
            if(mpParentView)
            {
                mpParentView->SendSubviewToBack(this);
            }
        }
        //-----------------------------------------------------
        /// Bring Subview Forward
        ///
        /// Move this view a single layer towards the screen
        //-----------------------------------------------------
        void CGUIView::BringSubviewForward(CGUIView* inpView)
        {
            u32 udwNumSubviews = mSubviews.size();
            for(u32 i=0; i<udwNumSubviews; ++i)
            {
                if(mSubviews[i].get() == inpView && i != (udwNumSubviews - 1))
                {
                    std::swap(mSubviews[i], mSubviews[i+1]);
                    return;
                }
            }
        }
        //-----------------------------------------------------
        /// Send Subview Backward
        ///
        /// Move this view a layer away from the screen
        //-----------------------------------------------------
        void CGUIView::SendSubviewBackward(CGUIView* inpView)
        {
            u32 udwNumSubviews = mSubviews.size();
            for(u32 i=0; i<udwNumSubviews; ++i)
            {
                if(mSubviews[i].get() == inpView && i > 0)
                {
                    std::swap(mSubviews[i], mSubviews[i-1]);
                    return;
                }
            }
        }
        //-----------------------------------------------------
        /// Bring Subview To Front
        ///
        /// Move this view to the top of the view hierarchy
        //-----------------------------------------------------
        void CGUIView::BringSubviewToFront(CGUIView* inpView)
        {
            u32 udwNumSubviews = mSubviews.size();
            for(u32 i = 1; i < udwNumSubviews; ++i)
            {
                if(mSubviews[i - 1].get() == inpView)
                {
                    std::swap(mSubviews[i - 1], mSubviews[i]);
                }
            }
        }
        //-----------------------------------------------------
        /// Send Subview To Back
        ///
        /// Move this view to the end of the view hierarchy
        //-----------------------------------------------------
        void CGUIView::SendSubviewToBack(CGUIView* inpView)
        {
            u32 udwNumSubviews = mSubviews.size();
            for(u32 i = udwNumSubviews - 1; i > 0; --i)
            {
                if(mSubviews[i].get() == inpView)
                {
                    std::swap(mSubviews[i], mSubviews[i-1]);
                }
            }
        }
        //-----------------------------------------------------
        /// Get Parent View Pointer
        ///
        /// @return Raw GUI view pointer
        //-----------------------------------------------------
        CGUIView* CGUIView::GetParentViewPtr() const
        {
            return mpParentView;
        }
		//-----------------------------------------------------
		/// Get Parent With Name
		///
		/// @param Name
		/// @return Parent in hierarchy with given name
		//-----------------------------------------------------
		CGUIView* CGUIView::GetParentWithName(const std::string& instrName) const
		{
			if(mpParentView)
			{
				if(mpParentView->GetName() == instrName)
				{
					return mpParentView;
				}
				return mpParentView->GetParentWithName(instrName);
			}
			
			return NULL;
		}
        //-----------------------------------------------------
        /// Set Parent View 
        ///
        /// @param The parent the view is attached to
        //-----------------------------------------------------
        void CGUIView::SetParentView(CGUIView* inpParentView)
        {
			OnTransformChanged(TRANSFORM_CACHE|ABS_SIZE_CACHE|ABS_POS_CACHE);
            mpParentView = inpParentView;
        }
		//-----------------------------------------------------
		/// Get Input Events
		///
		/// @return The input events of the view
		//-----------------------------------------------------
		CInputEvents* CGUIView::GetInputEvents()
		{
			return &mInputEvents;
		}
        //-----------------------------------------------------
        /// Get Root Window Pointer
        ///
        /// @return Window that the view is attached to
        //-----------------------------------------------------
        CWindow* CGUIView::GetRootWindowPtr() const
        {
            return mpRootWindow;
        }
        //-----------------------------------------------------
        /// Set Root Window 
        ///
        /// @param The window the view is attached to
        //-----------------------------------------------------
        void CGUIView::SetRootWindow(CWindow* inpWindow)
        {
            mpRootWindow = inpWindow;
			
			for(CGUIView::Subviews::iterator it = mSubviews.begin(); it != mSubviews.end(); ++it)
            {
				(*it)->SetRootWindow(inpWindow);
			}
        }
        //-----------------------------------------------------
        /// Get Subviews
        ///
        /// @return Vector of shared GUI view pointers
        //-----------------------------------------------------
        const CGUIView::Subviews& CGUIView::GetSubviews() const
        {
            return mSubviews;
        }
		//-----------------------------------------------------
		/// Get Subviews
		///
		/// @return Vector of shared GUI view pointers
		//-----------------------------------------------------
		CGUIView::Subviews& CGUIView::GetSubviews()
		{
			return mSubviews;
		}
        //-----------------------------------------------------
        /// Get Subview At Index
        ///
        /// @return Subview at the given index in the hierarchy
        //-----------------------------------------------------
        const GUIViewPtr& CGUIView::GetSubviewAtIndex(u32 inudwIndex) const
        {
            return mSubviews[inudwIndex];
        }
        //-----------------------------------------------------
        /// Get Subview With Name
        ///
        /// @param Name
        /// @return Subview in hierarchy with given name
        //-----------------------------------------------------
        const GUIViewPtr& CGUIView::GetSubviewWithName(const std::string& instrName) const
        {
            for(CGUIView::Subviews::const_iterator it = mSubviews.begin(); it != mSubviews.end(); ++it)
            {
                //If the subview has the name then return it
                if((*it)->GetName() == instrName)
                {
                    return (*it);
                }
                else
                {
                    //Check if it lives inside the subviews subview
                    const GUIViewPtr& pView = (*it)->GetSubviewWithName(instrName);
					if(pView)
					{
						return pView;
					}
                }
            }
            
            return pGUINull;
        }
		//-----------------------------------------------------
		/// Get Subviews With Name
		///
		/// @param Name
		/// @param out List of subviews in hierarchy with given name
		//-----------------------------------------------------
		void CGUIView::GetSubviewsWithName(const std::string& instrName, DYNAMIC_ARRAY<GUIViewPtr>& outSubviews) const
        {
            for(CGUIView::Subviews::const_iterator it = mSubviews.begin(); it != mSubviews.end(); ++it)
            {
                // If the subview has the name then add it
                if((*it)->GetName() == instrName)
                {
                    outSubviews.push_back(*it);
                }
                else
                {
                    // Check if it lives inside the subviews subview
					(*it)->GetSubviewsWithName(instrName, outSubviews);
                }
            }
        }

        //---Alignment
        //-----------------------------------------------------
        /// Set Local Alignment
        ///
        /// Set the pivot point of the view. This is the point
        /// about which all transforms are applied and from 
        /// which any renderable content is drawn
        ///
        /// @param Alignment type
        //-----------------------------------------------------
        void CGUIView::SetLocalAlignment(Core::AlignmentAnchor ineAlignment)
        {
			OnTransformChanged(TRANSFORM_CACHE|ABS_POS_CACHE);
            LocalAlignment = ineAlignment;
        }
        //-----------------------------------------------------
        /// Get Local Alignment
        ///
        /// Get the pivot point of the view. This is the point
        /// about which all transforms are applied and from 
        /// which any renderable content is drawn
        ///
        /// @return Alignment type
        //-----------------------------------------------------
        Core::AlignmentAnchor CGUIView::GetLocalAlignment() const
        {
            return LocalAlignment;
        }
        //-----------------------------------------------------
        /// Enable Rotation With Parent
        ///
        /// Toggle whether the view rotates with it's
        /// parent
        ///
        /// @param Enable/disable
        //-----------------------------------------------------
        void CGUIView::EnableRotationWithParent(bool inbEnable)
        {
			OnTransformChanged(TRANSFORM_CACHE|ABS_POS_CACHE);
            RotatedWithParent = inbEnable;
        }
		//-----------------------------------------------------
		/// Is Rotated With Parent
		///
		/// @param Whether the view rotates with it's parent
		//-----------------------------------------------------
		bool CGUIView::IsRotatedWithParent() const
		{
			return RotatedWithParent;
		}
        //-----------------------------------------------------
        /// Enable Alignment To Parent
        ///
        /// Toggle whether the view is aligned with it's
        /// parent
        ///
        /// @param Enable/disable
        //-----------------------------------------------------
        void CGUIView::EnableAlignmentToParent(bool inbEnable)
        {
			OnTransformChanged(TRANSFORM_CACHE|ABS_POS_CACHE);
            AlignedWithParent = inbEnable;
        }
        //-----------------------------------------------------
        /// Is Aligned To Parent
        ///
        /// @param Whether the view is aligned with it's
        //-----------------------------------------------------
        bool CGUIView::IsAlignedToParent() const
        {
            return AlignedWithParent;
        }
        //-----------------------------------------------------
        /// Set Alignment To Parent
        ///
        /// If alignment is enabled then the view will snap
        /// to the anchor point on it's parent view. 
        /// The view will be aligned to the anchor point
        /// regardless of parent transformation
        ///
        /// @param Alignment type
        //-----------------------------------------------------
        void CGUIView::SetAlignmentToParent(Core::AlignmentAnchor ineAlignment)
        {
			OnTransformChanged(TRANSFORM_CACHE|ABS_POS_CACHE);
            ParentalAlignment = ineAlignment;
        }
        //-----------------------------------------------------
        /// Get Alignment with Parent
        ///
        /// Get the anchor point about which the child view
        /// aligns to it's parent
        ///
        /// @return Alignment type
        //-----------------------------------------------------
        Core::AlignmentAnchor CGUIView::GetAlignmentWithParent() const
        {
            return ParentalAlignment;
        }
        //-----------------------------------------------------
        /// Set Offset From Parent Alignment
        ///
        /// If alignment is enabled then the view will snap
        /// to the anchor point on it's parent view. 
        /// The view will then be offset by the given co-ords
        /// from the anchor point
        ///
        /// @param Unified co-ordinates
        //-----------------------------------------------------
        void CGUIView::SetOffsetFromParentAlignment(const UnifiedVector2& invOffset)
        {
			OnTransformChanged(TRANSFORM_CACHE|ABS_POS_CACHE);
            UnifiedParentalOffset = invOffset;
        }
        //-----------------------------------------------------
        /// Set Offset From Parent Alignment
        ///
        /// If alignment is enabled then the view will snap
        /// to the anchor point on it's parent view. 
        /// The view will then be offset by the given co-ords
        /// from the anchor point
        ///
        /// @param Unified co-ordinates
        //-----------------------------------------------------
        void CGUIView::SetOffsetFromParentAlignment(f32 infRx, f32 infRy, f32 infAx, f32 infAy)
        {
			OnTransformChanged(TRANSFORM_CACHE|ABS_POS_CACHE);
            UnifiedParentalOffset.vRelative.x = infRx;
            UnifiedParentalOffset.vRelative.y = infRy;
            UnifiedParentalOffset.vAbsolute.x = infAx;
            UnifiedParentalOffset.vAbsolute.y = infAy;
        }
        //-----------------------------------------------------
        /// Get Offset From Parent Alignment
        ///
        /// If alignment is enabled then the view will snap
        /// to the anchor point on it's parent view. 
        /// The view will then be offset by the given co-ords
        /// from the anchor point
        ///
        /// @return Unified co-ordinates
        //-----------------------------------------------------
        const UnifiedVector2& CGUIView::GetOffsetFromParentAlignment() const
        {
            return UnifiedParentalOffset;
        }
        //-----------------------------------------------------
        /// Get Absolute Offset From Parent Alignment
        ///
        /// If alignment is enabled then the view will snap
        /// to the anchor point on it's parent view. 
        /// The view will then be offset by the given co-ords
        /// from the anchor point
        ///
        /// @return Unified co-ordinates converted to real
        /// co-ordinates
        //-----------------------------------------------------
        const Core::CVector2& CGUIView::GetAbsoluteOffsetFromParentAlignment() const
        {
            mvAbsoluteParentalOffset = (mpParentView->GetAbsoluteSize() * GetOffsetFromParentAlignment().GetRelative()) + GetOffsetFromParentAlignment().GetAbsolute();
            return mvAbsoluteParentalOffset;
        }
        //-----------------------------------------------------
        /// Set Offset From Position
        ///
        /// If alignment is disabled then the view will be 
		/// offset by the given co-ords from the anchor point
        ///
        /// @param Unified co-ordinates
        //-----------------------------------------------------
        void CGUIView::SetOffsetFromPosition(const UnifiedVector2& invOffset)
        {
			OnTransformChanged(TRANSFORM_CACHE|ABS_POS_CACHE);
            UnifiedPositionOffset = invOffset;
        }
        //-----------------------------------------------------
        /// Set Offset From Position
        ///
        /// If alignment is disabled then the view will be 
		/// offset by the given co-ords from the anchor point
        ///
        /// @param Unified co-ordinates
        //-----------------------------------------------------
        void CGUIView::SetOffsetFromPosition(f32 infRx, f32 infRy, f32 infAx, f32 infAy)
        {
			OnTransformChanged(TRANSFORM_CACHE|ABS_POS_CACHE);
            UnifiedPositionOffset.vRelative.x = infRx;
            UnifiedPositionOffset.vRelative.y = infRy;
            UnifiedPositionOffset.vAbsolute.x = infAx;
            UnifiedPositionOffset.vAbsolute.y = infAy;
        }
        //-----------------------------------------------------
        /// Get Offset From Position
        ///
        /// If alignment is disabled then the view will be 
		/// offset by the given co-ords from the anchor point
        ///
        /// @return Unified co-ordinates
        //-----------------------------------------------------
        const UnifiedVector2& CGUIView::GetOffsetFromPosition() const
        {
            return UnifiedPositionOffset;
        }
        //-----------------------------------------------------
        /// Get Absolute Offset From Position
        ///
        /// If alignment is disabled then the view will be 
		/// offset by the given co-ords from the anchor point
        ///
        /// @return Unified co-ordinates converted to real
        /// co-ordinates
        //-----------------------------------------------------
        const Core::CVector2& CGUIView::GetAbsoluteOffsetFromPosition() const
        {
            mvAbsolutePositionOffset = GetOffsetFromPosition().GetAbsolute();
			
			if (mpParentView && AlignedWithParent)
			{
				mvAbsolutePositionOffset += mpParentView->GetAbsoluteSize() * GetOffsetFromPosition().GetRelative();
			}
			
            return mvAbsolutePositionOffset;
        }
        
        //---View properties
		//-----------------------------------------------------
		/// Enable Clip Off Screen
		///
		/// Toggle whether the view is clipped when exceeding
		/// the boundary of the screen
		///
		/// @param Enable/disable
		//-----------------------------------------------------
		void CGUIView::EnableClipOffScreen(bool inbEnable)
        {
            ClipOffScreen = inbEnable;
        }
		//-----------------------------------------------------
		/// Is Clipped Off Screen Enabled
		///
		/// Toggle whether the view is clipped when exceeding
		/// the boundary of the screen
		///
		/// @return Enable/disable
		//-----------------------------------------------------
		bool CGUIView::IsClippedOffScreenEnabled() const
		{
			return ClipOffScreen;
		}
        //-----------------------------------------------------
        /// Enable Subview Clipping
        ///
        /// Toggle whether the view is clipped when exceeding
        /// the boundary of it's parent view
        ///
        /// @param Enable/disable
        //-----------------------------------------------------
        void CGUIView::EnableSubviewClipping(bool inbEnable)
        {
            ClipSubviews = inbEnable;
        }
        //-----------------------------------------------------
        /// Is Subview Clipping Enabled
        ///
        /// Toggle whether the view is clipped when exceeding
        /// the boundary of it's parent view
        ///
        /// @return Enable/disable
        //-----------------------------------------------------
        bool CGUIView::IsSubviewClippingEnabled() const
        {
            return ClipSubviews;
        }
        //-----------------------------------------------------
        /// Set Name
        ///
        /// @param Name of view instance
        //-----------------------------------------------------
        void CGUIView::SetName(const std::string& instrName)
        {
            Name = instrName;
        }
        //-----------------------------------------------------
        /// Get Name
        ///
        /// @return Name of view instance
        //-----------------------------------------------------
        const std::string& CGUIView::GetName() const
        {
            return Name;
        }
        //-----------------------------------------------------
        /// Set Visible
        ///
        /// @param Toggle whether the view and subviews are
        /// visible or not
        //-----------------------------------------------------
        void CGUIView::SetVisible(bool inbVisible)
        {
            Visible = inbVisible;
        }
		//-----------------------------------------------------
		/// Is Visible
		///
		/// @return
		//-----------------------------------------------------
		bool CGUIView::IsVisible() const
		{
			return Visible;
		}
		//-----------------------------------------------------
		/// Is Visible In Hierarchy
		///
		/// @return If the entity is visible, checking the ancestors
		//-----------------------------------------------------
		bool CGUIView::IsVisibleInHierarchy() const
		{
			if (Visible && GetParentViewPtr())
			{
				return GetParentViewPtr()->IsVisibleInHierarchy();
			}
			return Visible;
		}
        //-----------------------------------------------------
        /// Is Rendered
        ///
        /// Determines if view is being drawn to screen
        /// takes into consideration if any of it's parents
        /// visibility is set to false.
        ///
        /// @returns If being drawn
        //-----------------------------------------------------
        bool CGUIView::IsRendered() const
        {
            bool bIsDrawn = IsVisible();
            if(!bIsDrawn)
                return false;
            
            CGUIView * pParentView = GetParentViewPtr();
            while(pParentView)
            {
                if(!pParentView->IsVisible())
                    return false;
                
                pParentView = pParentView->GetParentViewPtr();
            }
            return true;
        }
        //-----------------------------------------------------
        /// Enable User Interaction
        ///
        /// Toggle whether the view receives user input
        ///
        /// @param Enable/disable
        //-----------------------------------------------------
        void CGUIView::EnableUserInteraction(bool inbEnabled)
        {
			UserInteraction = inbEnabled;
		}
		//-----------------------------------------------------
		/// Is User Interaction Enabled
		///
		/// @return whether the view receives user input
		//-----------------------------------------------------
		bool CGUIView::IsUserInteractionEnabled() const
		{
			return UserInteraction;
		}
		//-----------------------------------------------------
        /// Set Movable
        ///
        /// Toggle whether the user can move the UI
        ///
		/// @param Enable/disable
		//-----------------------------------------------------
		void CGUIView::SetMovable(bool inbMovable)
		{
			Movable = inbMovable;
		}
		//-----------------------------------------------------
		/// Is Movable
		///
		/// @return Whether view is draggable
		//-----------------------------------------------------
		bool CGUIView::IsMovable() const
		{
			return Movable;
		}
		//-----------------------------------------------------
		/// Enable Touch Consumption
		///
		/// @param Override to force a view to consome or route touches
		/// @param The type of touch concerned (BEGAN, MOVED or ALL)
        //-----------------------------------------------------
        void CGUIView::EnableTouchConsumption(bool inbEnabled, TouchType ineType)
        {
			if(inbEnabled)
			{
				
				ConsumesTouches |= ineType;
			}
			else
			{
				
				ConsumesTouches &= ~ineType;
			}
		}
		//-----------------------------------------------------
		/// Is Touch Consumption Enabled
		///
		/// @param The type of touch concerned (BEGAN, MOVED or ALL)
		/// @return Override to force a view to consome or route
		/// touches
		//-----------------------------------------------------
		bool CGUIView::IsTouchConsumptionEnabled(TouchType ineType) const
		{
			return ineType & ConsumesTouches;
		}
        //-----------------------------------------------------
        /// Enable Inherited Scale
        ///
        /// @param Whether the parents scale affects the
        /// child
        //-----------------------------------------------------
        void CGUIView::EnableInheritedScale(bool inbEnabled)
		{
			InheritScale = inbEnabled;
		}
		//-----------------------------------------------------
		/// Is Inherited Scale Enabled
		///
		/// @return Whether this view inherits scale from its parent
		//-----------------------------------------------------
		bool CGUIView::IsInheritedScaleEnabled() const
		{
			return InheritScale;
		}
        //-----------------------------------------------------
        /// Enable Inherited Colour
        ///
        /// @param Whether the parents colour affects the
        /// child
        //-----------------------------------------------------
        void CGUIView::EnableInheritedColour(bool inbEnabled)
		{
			InheritColour = inbEnabled;
		}
		//-----------------------------------------------------
		/// Is Inherited Colour Enabled
		///
		/// @return Whether this view inherits colour from its parent
		//-----------------------------------------------------
		bool CGUIView::IsInheritedColourEnabled() const
		{
			return InheritColour;
		}
		//-----------------------------------------------------
		/// Enable Inherited Opacity
		///
		/// @param Whether this view inherits opacity from its parent
		//-----------------------------------------------------
		void CGUIView::EnableInheritedOpacity(bool inbEnabled)
		{
			InheritOpacity = inbEnabled;
		}
		//-----------------------------------------------------
		/// Is Inherited Opacity Enabled
		///
		/// @return Whether this view inherits opacity from its parent
		//-----------------------------------------------------
		bool CGUIView::IsInheritedOpacityEnabled() const
		{
			return InheritOpacity;
		}
        //-----------------------------------------------------
        /// Set Colour
        ///
        /// @param The colour applied to the view when drawn
        //-----------------------------------------------------
        void CGUIView::SetColour(const Core::CColour& inColour)
        {
            Colour = inColour;
        }
		//-----------------------------------------------------
		/// Get Colour
		///
		/// @return The colour applied to the view when drawn
		//-----------------------------------------------------
		const Core::CColour& CGUIView::GetColour() const
		{
			return Colour;
		}
        //-----------------------------------------------------
        /// Get Absolute Colour
        ///
        /// @param The inherited colour of the view
        //-----------------------------------------------------
        const Core::CColour& CGUIView::GetAbsoluteColour()
        {
            mAbsoluteColour = Colour;
            
            if(mpParentView && InheritColour)
            {
                mAbsoluteColour *= mpParentView->GetAbsoluteColour();
            }
            
			mAbsoluteColour *= GetInheritedOpacity();
			
            return mAbsoluteColour;
        }
		//-----------------------------------------------------
		/// Set Opacity
		///
		/// @param Fractional opacity to set for this view (0 - 1)
		//-----------------------------------------------------
		void CGUIView::SetOpacity(f32 infValue)
		{
			Opacity = infValue;
		}
		//-----------------------------------------------------
		/// GetOpacity
		///
		/// @param Returns the opacity of this view.
		//-----------------------------------------------------
		f32 CGUIView::GetOpacity() const
		{
			return Opacity;
		}
		//-----------------------------------------------------
		/// GetInheritedOpacity
		///
		/// @return Returns this view's opacity multiplied by the inherited opacity of any parent it may have
		//-----------------------------------------------------
		f32 CGUIView::GetInheritedOpacity() const
		{
			if (mpParentView && InheritOpacity)
				return Opacity * mpParentView->GetInheritedOpacity();
			
			return Opacity;
		}

        //---Transforms
        //-----------------------------------------------------
        /// Set Position
        ///
        /// Position the view based on it's local alignment
        /// and if alignment to parent is enabled then
        /// the position will be relative to the anchor point
        /// and the current offset (i.e. accumulative position)
        ///
        /// @param Unified co-ordinates
        //------------------------------------------------------
        void CGUIView::SetPosition(const UnifiedVector2& invPosition)
        {
			OnTransformChanged(TRANSFORM_CACHE|ABS_POS_CACHE);
            UnifiedPosition = invPosition;
        }
        //-----------------------------------------------------
        /// Set Position
        ///
        /// Position the view based on it's local alignment
        /// and if alignment to parent is enabled then
        /// the position will be relative to the anchor point
        /// and the current offset (i.e. accumulative position)
        ///
        /// @param Unified co-ordinates Rx, Ry, Ax, Ay
        //------------------------------------------------------
        void CGUIView::SetPosition(f32 fRx, f32 fRy, f32 fAx, f32 fAy)
        {
			OnTransformChanged(TRANSFORM_CACHE|ABS_POS_CACHE);
            UnifiedPosition.vRelative.x = fRx;
            UnifiedPosition.vRelative.y = fRy;
            UnifiedPosition.vAbsolute.x = fAx;
            UnifiedPosition.vAbsolute.y = fAy;
        }
        //------------------------------------------------------
        /// Move By
        ///
        /// Offset the current accumulated position by the
        /// given co-ordinates
        ///
        /// @param co-ordinates
        //------------------------------------------------------
        void CGUIView::MoveBy(const UnifiedVector2& invPosition)
        {
			OnTransformChanged(TRANSFORM_CACHE|ABS_POS_CACHE);
            if(!AlignedWithParent)
            {
                UnifiedPosition += invPosition;
            }
            else
            {
                UnifiedParentalOffset += invPosition;
            }
        }
        //------------------------------------------------------
        /// Move By
        ///
        /// Offset the current accumulated position by the
        /// given co-ordinates
        ///
        /// @param Unified co-ordinates Rx, Ry, Ax, Ay 
        //------------------------------------------------------
        void CGUIView::MoveBy(f32 fRx, f32 fRy, f32 fAx, f32 fAy)
        {
			OnTransformChanged(TRANSFORM_CACHE|ABS_POS_CACHE);
            if(!AlignedWithParent)
            {
                UnifiedPosition.vRelative.x += fRx;
                UnifiedPosition.vRelative.y += fRy; 
                UnifiedPosition.vAbsolute.x += fAx;
                UnifiedPosition.vAbsolute.y += fAy;
            }
            else
            {
                UnifiedParentalOffset.vRelative.x += fRx;
                UnifiedParentalOffset.vRelative.y += fRy; 
                UnifiedParentalOffset.vAbsolute.x += fAx;
                UnifiedParentalOffset.vAbsolute.y += fAy;
            }
        }
        //------------------------------------------------------
        /// Rotate To
        ///
        /// Rotate the object to the given angle with respect
        /// to the parents rotation
        ///
        /// @param Angle in radians
        //------------------------------------------------------
        void CGUIView::RotateTo(f32 infAngleInRadians)
        {
			OnTransformChanged(TRANSFORM_CACHE);
            Rotation = infAngleInRadians;
        }
        //------------------------------------------------------
        /// Rotate By
        ///
        /// Rotate the object by the given angle
        ///
        /// @param Angle in radians
        //------------------------------------------------------
        void CGUIView::RotateBy(f32 infAngleInRadians)
        {
			OnTransformChanged(TRANSFORM_CACHE);
            Rotation += infAngleInRadians;
        }
        //------------------------------------------------------
        /// Set Size
        ///
        /// Set the dimensions of the object
        ///
        /// @param Unified co-ordinates for scale factor
        //------------------------------------------------------
        void CGUIView::SetSize(const UnifiedVector2& invSize)
        {
			OnTransformChanged(TRANSFORM_CACHE|ABS_SIZE_CACHE);
            UnifiedSize = invSize;
        }
        //------------------------------------------------------
        /// Set Size
        ///
        /// Set the dimensions of the object
        ///
        /// @param Unified co-ordinates Rx, Ry, Ax, Ay 
        //------------------------------------------------------
        void CGUIView::SetSize(f32 fRx, f32 fRy, f32 fAx, f32 fAy)
        {
			OnTransformChanged(TRANSFORM_CACHE|ABS_SIZE_CACHE);
            UnifiedSize.vRelative.x = fRx;
            UnifiedSize.vRelative.y = fRy;
            UnifiedSize.vAbsolute.x = fAx;
            UnifiedSize.vAbsolute.y = fAy;
        }
        //------------------------------------------------------
        /// Scale To
        ///
        /// Set the scale of the object
        ///
        /// @param Scale co-ordinates for scale factor
        //------------------------------------------------------
        void CGUIView::ScaleTo(const Core::CVector2& invScale)
        {
			OnTransformChanged(TRANSFORM_CACHE|ABS_SIZE_CACHE);
            Scale = invScale;
        }
        //------------------------------------------------------
        /// Scale To
        ///
        /// Set the scale of the object
        ///
        /// @param Unified co-ordinates fScaleX, fScaleY
        //------------------------------------------------------
        void CGUIView::ScaleTo(f32 fScaleX, f32 fScaleY)
        {
			OnTransformChanged(TRANSFORM_CACHE|ABS_SIZE_CACHE);
            Scale.x = fScaleX;
            Scale.y = fScaleY;
        }
		//------------------------------------------------------
		/// Scale To
		///
		/// Set the uniform scale of the object
		///
		/// @param Scale uniform co-ordinate fScale
		//------------------------------------------------------
        void CGUIView::ScaleTo(f32 fScale)
        {
            ScaleTo(fScale, fScale);
        }
        //-----------------------------------------------------
        /// Get Position
        ///
        /// Position based on it's local alignment
        /// and if alignment to parent is enabled then
        /// the position will be relative to the anchor point
        /// and the current offset (i.e. accumulative position)
        ///
        /// @return Unified co-ordinates
        //------------------------------------------------------
        const UnifiedVector2& CGUIView::GetPosition() const
        {
            return UnifiedPosition;
        }
        //------------------------------------------------------
        /// Get Rotation
        ///
        /// Rotation of object in radians
        ///
        /// @return Angle in radians
        //------------------------------------------------------
        f32 CGUIView::GetRotation() const
        {
            return Rotation;
        }
        //------------------------------------------------------
        /// Get Size
        ///
        /// Get the dimensions of the object
        ///
        /// @return Unified co-ordinates for dimensions
        //------------------------------------------------------
        const UnifiedVector2& CGUIView::GetSize() const
        {
            return UnifiedSize;
        }
        //------------------------------------------------------
        /// Get Scale
        ///
        /// Get the scale of the object
        ///
        /// @return Unified co-ordinates for dimensions
        //------------------------------------------------------
        const Core::CVector2& CGUIView::GetScale() const
        {
            return Scale;
        }
        //-----------------------------------------------------
        /// Get Absolute Scale
        ///
        /// @param The inherited scale of the view
        //-----------------------------------------------------
        const Core::CVector2& CGUIView::GetAbsoluteScale() const
        {
            mvAbsoluteScale = Scale;
            
            if(mpParentView && InheritScale)
            {
                mvAbsoluteScale *= mpParentView->GetAbsoluteScale();
            }
			
            return mvAbsoluteScale;
        }
		//-----------------------------------------------------
		/// Get Absolute Rotation
		///
		/// Rotation in parent views space. This is the cumulated
		/// rotation of the view hierarchy.
		///
		/// @return Cumulated rotation of the parent hierarchy
		//------------------------------------------------------
		const f32 CGUIView::GetAbsoluteRotation() const
		{
			if (mpParentView && !RotatedWithParent)
			{
				return Rotation - mpParentView->GetAbsoluteRotation();
			}
			
			return Rotation;
		}
        //-----------------------------------------------------
        /// Get Absolute Position
        ///
        /// Position in parent views space. This is the unified
        /// co-ordinates transformed into absolute
        ///
        /// @return Position combined absolute and relative
        //------------------------------------------------------
        const Core::CVector2& CGUIView::GetAbsolutePosition() const
        {
			if(!BITMASK_CHECK(mudwCacheValidaters, ABS_POS_CACHE))
			{
				//The absolute position is the position on screen and
				//therefore must take out parents position into account 
				if(mpParentView)
				{
					if(AlignedWithParent)
					{
						//If we are aligned to the anchor point of our parent then we 
						//center ourself around this point and add on our offset
						mvAbsolutePosition = mpParentView->GetAbsoluteAnchorPoint(ParentalAlignment) + GetAbsoluteOffsetFromParentAlignment() + GetAbsoluteOffsetFromPosition();
					}
					else
					{
						//The relative position is based on the size of our parent
						Core::CVector2 vParentSize = mpParentView->GetAbsoluteSize();
						Core::CVector2 vParentHalfSize = vParentSize * 0.5f;
						
						//Get our centre point relative to the parent
						mvAbsolutePosition = (vParentSize * (GetPosition().GetRelative() + GetOffsetFromPosition().GetRelative())) + GetPosition().GetAbsolute() - vParentHalfSize + GetOffsetFromPosition().GetAbsolute();
					}
				}
				else
				{
					//We have no parent and therefore we are the window and
					//can just return our absolute position
					mvAbsolutePosition = GetPosition().GetAbsolute() + GetOffsetFromPosition().GetAbsolute();
				}
				
				//Take into account local alignment which will offset the absolute point from our centre point
				Core::CVector2 vAlignment;
				Core::Align(LocalAlignment, GetAbsoluteSize() * 0.5f, vAlignment);
				mvAbsolutePosition += vAlignment;
				
				//BUG:
				//BITMASK_SET(mudwCacheValidaters, ABS_POS_CACHE);
			}
            
            return mvAbsolutePosition;
        }
        //-----------------------------------------------------
        /// Get Absolute Screen Space Position
        ///
        /// The position of the view after all parental
        /// transforms have been applied. This is it's final
        /// render position on screen
        ///
        /// @return Position combined absolute and relative
        //------------------------------------------------------
        const Core::CVector2& CGUIView::GetAbsoluteScreenSpacePosition() const
        {
            //Return the translation component of our transform
			GetTransform();
			mvAbsolutePosition.x = mmatTransform.m[6];
			mvAbsolutePosition.y = mmatTransform.m[7];
				
            return mvAbsolutePosition;
        }
        //------------------------------------------------------
        /// Get Absolute Size
        ///
        /// Get the real dimensions based by combining absoulte 
        /// and relative
        ///
        /// @return Size combined absolute and relative
        //------------------------------------------------------
        const Core::CVector2& CGUIView::GetAbsoluteSize() const
        {
            //The absolute size is based on the absolute size of our parent 
            //and the relative size of us
            if(mpParentView)
            {
				if(!BITMASK_CHECK(mudwCacheValidaters, ABS_SIZE_CACHE))
				{
					mvAbsoluteSize = ((mpParentView->GetAbsoluteSize() * GetSize().GetRelative()) + GetSize().GetAbsolute()) * GetAbsoluteScale();
					BITMASK_SET(mudwCacheValidaters, ABS_SIZE_CACHE);
				}
				
                return mvAbsoluteSize;
            }
            else
            {
				mvAbsoluteSize = GetSize().GetAbsolute() * GetAbsoluteScale();
				return mvAbsoluteSize;
            }
        }
        //-----------------------------------------------------
        /// Get Absolute Anchor Point
        ///
        /// @param Anchor point type
        /// @return Anchor point value as absolute
        //-----------------------------------------------------
        Core::CVector2 CGUIView::GetAbsoluteAnchorPoint(Core::AlignmentAnchor ineAlignment) const
        {
			Core::CVector2 vSize = GetAbsoluteSize();
			Core::CVector2 vHalfSize(vSize.x * 0.5f, vSize.y * 0.5f);
			Core::CVector2 vPos;
			Core::GetAnchorPoint(ineAlignment, vHalfSize, vPos);

            return vPos;
        }
        //-----------------------------------------------------
        /// Get Absolute Screen Space Anchor Point
        ///
        /// @param Anchor point type
        /// @return Anchor point value as absolute
        //-----------------------------------------------------
        Core::CVector2 CGUIView::GetAbsoluteScreenSpaceAnchorPoint(Core::AlignmentAnchor ineAlignment) const
        {
			Core::CVector2 vSize = GetAbsoluteSize();
			Core::CVector2 vHalfSize(vSize.x * 0.5f, vSize.y * 0.5f);
			Core::CVector2 vPos;
		
			Core::GetAnchorPoint(ineAlignment, vHalfSize, vPos);
				
			return vPos + GetAbsoluteScreenSpacePosition();
        }
        //-----------------------------------------------------
        /// Get Transform
        ///
        /// @return Transformation matrix
        //-----------------------------------------------------
        const Core::CMatrix3x3& CGUIView::GetTransform() const
        {
			if(!BITMASK_CHECK(mudwCacheValidaters, TRANSFORM_CACHE))
			{
				if(mpParentView)
				{
					//Create our transform without respect to our parent
					Core::CMatrix3x3 matTrans;
					matTrans.SetTransform(GetAbsolutePosition(), Core::CVector2::ONE, GetAbsoluteRotation());
					
					//Apply our parents transform
					Core::CMatrix3x3::Multiply(&matTrans, &mpParentView->GetTransform(), &mmatTransform);
				}
				else
				{
					mmatTransform.SetTransform(GetAbsolutePosition(), Core::CVector2::ONE, Rotation);
				}
				
				BITMASK_SET(mudwCacheValidaters, TRANSFORM_CACHE);
			}
            
            return mmatTransform;
        }
        //-----------------------------------------------------
        /// Is Onscreen
        ///
        /// @return Whether any part of the view is within
        /// the bounds of the screen
        //-----------------------------------------------------
        bool CGUIView::IsOnscreen() const
        {
            //Check if this is on screen
            Core::CVector2 vTopRight = GetAbsoluteScreenSpaceAnchorPoint(Core::ALIGN_TOP_RIGHT);
            Core::CVector2 vBottomLeft = GetAbsoluteScreenSpaceAnchorPoint(Core::ALIGN_BOTTOM_LEFT);
        
            return (vTopRight.y >= 0 && vBottomLeft.y <= Core::CScreen::GetOrientedHeight() && vTopRight.x >= 0 && vBottomLeft.x <= Core::CScreen::GetOrientedWidth());
        }
        
        //---Functional overrides
        //-----------------------------------------------------
        /// Update
        ///
        /// @param Time between frames
        //-----------------------------------------------------
        void CGUIView::Update(f32 infDt)
        {
            if(Visible)
            {
				mSubviewsCopy = mSubviews;
                for(CGUIView::Subviews::iterator it = mSubviewsCopy.begin(); it != mSubviewsCopy.end(); ++it)
                {
                    (*it)->Update(infDt);
                }
                
                mSubviewsCopy.clear();
            }
        }
        //-----------------------------------------------------
        /// Draw
        ///
        /// Override this implementation to draw the GUI
        /// using the primitves within canvas renderer i.e.
        /// DrawBox, DrawString etc
        ///
        /// @param Canvas renderer
        //----------------------------------------------------
        void CGUIView::Draw(Rendering::CCanvasRenderer * inpCanvas)
        {
			if(Visible && (!ClipOffScreen || IsOnscreen()))
			{
				//This is a container view and has no renderables
				//We will need to render our subviews though
				
				//Check if we force clip our children 
				if(ClipSubviews)
				{
                    Core::CVector2 vBottomLeft = GetAbsoluteScreenSpaceAnchorPoint(Core::ALIGN_BOTTOM_LEFT);
					inpCanvas->EnableClippingToBounds(vBottomLeft, GetAbsoluteSize());
				}
				
				for(CGUIView::Subviews::iterator it = mSubviews.begin(); it != mSubviews.end(); ++it)
				{
					if ((*it)->IsVisible())
					{
						(*it)->Draw(inpCanvas);
					}
				}
				
				if(ClipSubviews)
				{
					inpCanvas->DisableClippingToBounds();
				}
			}
        }
        //----------------------------------------------------
        /// Contains
        ///
        /// Override this implementation to calculate whether
        /// the given point lies within the view. If you have
        /// subviews that extend the bounds of the main view
        /// for instance
        ///
        /// @param Point vector
        /// @return Whether it lies within the view
        //----------------------------------------------------
        bool CGUIView::Contains(const Core::CVector2& invPoint) const
        {
            Core::Rectangle sAbsoluteAABB(GetAbsoluteScreenSpaceAnchorPoint(Core::ALIGN_BOTTOM_LEFT), GetAbsoluteSize());
            return sAbsoluteAABB.Contains(invPoint);
        }
        //-----------------------------------------------------------
        /// Enable Accept Touches Outside of Bounds
        ///
		/// @param Whether view will listen for touches outwith
		/// it's bounds
        //-----------------------------------------------------------		
		void CGUIView::EnableAcceptTouchesOutsideOfBounds(bool inbEnable)
		{
			AcceptTouchesOutsideOfBounds = inbEnable;
		}
		//-----------------------------------------------------------
        /// Is Accept Touches Outside of Bounds Enabled
        ///
		/// @return Whether view will listen for touches outwith
		/// it's bounds
        //-----------------------------------------------------------
		bool CGUIView::IsAcceptTouchesOutsideOfBoundsEnabled() const
		{
			return AcceptTouchesOutsideOfBounds;
		}
		//-----------------------------------------------------------
		/// On Screen Orientation Changed
		///
		/// Triggered if the screen orientation changes so we can
		/// resize ourself
		//-----------------------------------------------------------
		void CGUIView::OnScreenOrientationChanged()
		{
			OnTransformChanged(TRANSFORM_CACHE|ABS_SIZE_CACHE|ABS_POS_CACHE);

			for(CGUIView::Subviews::iterator it = mSubviews.begin(); it != mSubviews.end(); ++it)
			{
				(*it)->OnScreenOrientationChanged();
			}
		}
        //---Touch Delegates
        //-----------------------------------------------------------
        /// On Touch Began
        ///
        /// Called when the window receives cursor/touch input
        ///
        /// @param Touch data
        /// @return Whether touch has been consumed
        //-----------------------------------------------------------
        bool CGUIView::OnTouchBegan(const Input::TouchInfo & insTouchInfo)
        {
            if(UserInteraction)
            {
				if(Movable)
				{
					mbIsBeingDragged = true;
				}

				mSubviewsCopy = mSubviews;

                //Loop through all our children
                for(CGUIView::Subviews::reverse_iterator it = mSubviewsCopy.rbegin(); it != mSubviewsCopy.rend(); ++it)
                {
					if((*it)->UserInteraction)
					{
						bool bContains = (*it)->Contains(insTouchInfo.vLocation);
						if((*it)->IsAcceptTouchesOutsideOfBoundsEnabled() || bContains)
						{
							if((*it)->OnTouchBegan(insTouchInfo))
							{
                                mSubviewsCopy.clear();
								return true;
							}
						}
					}
                }
                //Check for input events
				bool bContains = Contains(insTouchInfo.vLocation);
				if(IsAcceptTouchesOutsideOfBoundsEnabled() || bContains)
				{
					mInputEvents.OnTouchBegan(this, insTouchInfo);
                    mTouchBeganEvent.Invoke(insTouchInfo);
					//We consume this touch as it is within us
					if(IsTouchConsumptionEnabled(TOUCH_TYPE_BEGAN) && bContains)
					{
                        mSubviewsCopy.clear();
						return true;
					}
                }

            }
            
            //We have no user interaction enabled - let this touch pass
            mSubviewsCopy.clear();
            return false;
        }
        //-----------------------------------------------------------
        /// On Touch Moved
        ///
        /// Called when the window receives cursor/touch input
        ///
        /// @param Touch data
        /// @return Whether touch has been consumed
        //-----------------------------------------------------------
        bool CGUIView::OnTouchMoved(const Input::TouchInfo & insTouchInfo)
        {
            if(UserInteraction)
            {
				if(mbIsBeingDragged)
				{
					if(!AlignedWithParent)
					{
						SetPosition(UnifiedVector2(Core::CVector2::ZERO, insTouchInfo.vLocation));
					}
					else
					{
						SetOffsetFromParentAlignment(UnifiedVector2(Core::CVector2::ZERO, insTouchInfo.vLocation));
					}
				}

				mSubviewsCopy = mSubviews;

                for(CGUIView::Subviews::reverse_iterator it = mSubviewsCopy.rbegin(); it != mSubviewsCopy.rend(); ++it)
                {
                    if((*it)->OnTouchMoved(insTouchInfo))
                    {
                        mSubviewsCopy.clear();
                        return true;
                    }
                }
                
                //Check for input events
                //If we contain this touch we can consume it
                if(mInputEvents.OnTouchMoved(this, insTouchInfo))
                {
                    mTouchMovedEvent.Invoke(insTouchInfo);
                    if(IsTouchConsumptionEnabled(TOUCH_TYPE_MOVED))
                    {
                        mSubviewsCopy.clear();
                        return true;
                    }
                }
            }
            
            //We have no user interaction enabled
            mSubviewsCopy.clear();
            return false;
        }
        //-----------------------------------------------------------
        /// On Touch Ended
        ///
        /// Called when the window stops receiving cursor/touch input
        ///
        /// @param Touch data
        /// @return Whether touch has been consumed
        //-----------------------------------------------------------
        void CGUIView::OnTouchEnded(const Input::TouchInfo & insTouchInfo)
        {
			mbIsBeingDragged = false;

            if(UserInteraction)
            {
				mSubviewsCopy = mSubviews;
				
                for(CGUIView::Subviews::reverse_iterator it = mSubviewsCopy.rbegin(); it != mSubviewsCopy.rend(); ++it)
                {
                    (*it)->OnTouchEnded(insTouchInfo);
                }
                
                mTouchEndedEvent.Invoke(insTouchInfo);
                mInputEvents.OnTouchEnded(this, insTouchInfo);
            }
            mSubviewsCopy.clear();
        }
        
        //-----------------------------------------------------
        /// Layout Children's Content
        ///
        /// Calls LayoutContent on its subviews. Must be
        /// attached to root window.
        //-----------------------------------------------------
        void CGUIView::LayoutChildrensContent()
        {
            Subviews aSubviewsCopy = mSubviews;
            for(CGUIView::Subviews::iterator it = aSubviewsCopy.begin(); it != aSubviewsCopy.end(); ++it)
            {
                (*it)->LayoutContent();
                (*it)->LayoutChildrensContent();
            }
        }
		//-----------------------------------------------------
		/// Destructor
		//-----------------------------------------------------
		CGUIView::~CGUIView()
		{
			RemoveAllSubviews();
		}
    }
}
