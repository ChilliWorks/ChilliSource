//
//  GUIView.cpp
//  Chilli Source
//  Created by Scott Downie on 21/04/2011.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2011 Tag Games Limited
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//

#include <ChilliSource/GUI/Base/GUIView.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/Screen.h>
#include <ChilliSource/Core/Base/Utils.h>
#include <ChilliSource/Core/Math/MathUtils.h>
#include <ChilliSource/Core/String/StringParser.h>
#include <ChilliSource/Rendering/Base/CanvasRenderer.h>

#include <algorithm>
#include <limits>

namespace ChilliSource
{
    namespace GUI
    {
        static GUIViewSPtr pGUINull;

		DEFINE_META_CLASS(GUIView)

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
        GUIView::GUIView() : mpParentView(nullptr), mpRootWindow(nullptr), mbIsBeingDragged(false), mudwCacheValidaters(0), UnifiedPosition(0.5f, 0.5f, 0.0f, 0.0f), UnifiedSize(1.0f, 1.0f, 0.0f, 0.0f),
        Rotation(0.0f), Opacity(1.0f), LocalAlignment(Rendering::AlignmentAnchor::k_middleCentre), ParentalAlignment(Rendering::AlignmentAnchor::k_bottomLeft), AlignedWithParent(false),
        Scale(Core::Vector2::k_one), ClipSubviews(false), InheritColour(true), Visible(true), Movable(false), UserInteraction(true), ConsumesTouches((u8)TouchType::k_all),
        AcceptTouchesOutsideOfBounds(false), InheritOpacity(true), RotatedWithParent(true), InheritScale(false), ClipOffScreen(true)
        {
            m_screen = Core::Application::Get()->GetSystem<Core::Screen>();
        }
        //------------------------------------------------------
        /// Constructor
        ///
        /// Construct view from given key values
        ///
        /// @param Param dictionary
        //------------------------------------------------------
        GUIView::GUIView(const Core::ParamDictionary& insParams) 
		: mpParentView(nullptr), mpRootWindow(nullptr), mbIsBeingDragged(false), mudwCacheValidaters(0), UnifiedPosition(0.5f, 0.5f, 0.0f, 0.0f), UnifiedSize(1.0f, 1.0f, 0.0f, 0.0f),
        Rotation(0.0f), Opacity(1.0f), LocalAlignment(Rendering::AlignmentAnchor::k_middleCentre), ParentalAlignment(Rendering::AlignmentAnchor::k_bottomLeft), AlignedWithParent(false),
        Scale(Core::Vector2::k_one), ClipSubviews(false), InheritColour(true), Visible(true), Movable(false), UserInteraction(true), ConsumesTouches((u8)TouchType::k_all),
        AcceptTouchesOutsideOfBounds(false), InheritOpacity(true), RotatedWithParent(true), InheritScale(false), ClipOffScreen(true)
        {
            m_screen = Core::Application::Get()->GetSystem<Core::Screen>();
            
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
                Core::Vector4 vRawPosition = Core::ParseVector4(strValue);
                SetPosition(Core::UnifiedVector2(vRawPosition.x, vRawPosition.y, vRawPosition.z, vRawPosition.w));
            }
            //---Unified size
            if(insParams.TryGetValue("UnifiedSize", strValue))
            {
                //Convert this to a vector4 that we can then conver to unified vector2
                Core::Vector4 vRawSize = Core::ParseVector4(strValue);
                SetSize(Core::UnifiedVector2(vRawSize.x, vRawSize.y, vRawSize.z, vRawSize.w));
            }
            //---Unified scale
            if(insParams.TryGetValue("Scale", strValue))
            {
                //Convert this to a vector4 that we can then conver to unified vector2
                Core::Vector2 vScale = Core::ParseVector2(strValue);
                ScaleTo(vScale);
            }
            //---Scale Inheritance
			if (insParams.TryGetValue("InheritScale", strValue))
			{
				InheritScale = Core::ParseBool(strValue);
			}
            //---Rotation
            if(insParams.TryGetValue("Rotation", strValue))
            {
                RotateTo(Core::ParseF32(strValue));
            }
            //---Is rotated with parent
            if(insParams.TryGetValue("RotatedWithParent", strValue))
            {
                RotatedWithParent = Core::ParseBool(strValue);
            }
            //---Is aligned with parent
            if(insParams.TryGetValue("AlignedWithParent", strValue))
            {
                AlignedWithParent = Core::ParseBool(strValue);
            }
			//---Enable Clipping Off Screen
			if(insParams.TryGetValue("ClipOffScreen", strValue))
			{
				ClipOffScreen = Core::ParseBool(strValue);
			}
            //---Enable clipping
            if(insParams.TryGetValue("ClipSubviews", strValue))
            {
                ClipSubviews = Core::ParseBool(strValue);
            }
            //---Local alignment
            if(insParams.TryGetValue("LocalAlignment", strValue))
            {
                LocalAlignment = Rendering::AlignmentAnchorFromString(strValue);
            }
            //---Parental alignment
            if(insParams.TryGetValue("ParentalAlignment", strValue))
            {
                ParentalAlignment = Rendering::AlignmentAnchorFromString(strValue);
            }
            //---Unified parental offset
            if(insParams.TryGetValue("UnifiedParentalOffset", strValue))
            {
                //Convert this to a vector4 that we can then conver to unified vector2
                Core::Vector4 vRawOffset = Core::ParseVector4(strValue);
                SetOffsetFromParentAlignment(Core::UnifiedVector2(vRawOffset.x, vRawOffset.y, vRawOffset.z, vRawOffset.w));
            }
            //---Unified alignment offset
            if(insParams.TryGetValue("UnifiedPositionOffset", strValue))
            {
                //Convert this to a vector4 that we can then conver to unified vector2
                Core::Vector4 vRawOffset = Core::ParseVector4(strValue);
                SetOffsetFromPosition(Core::UnifiedVector2(vRawOffset.x, vRawOffset.y, vRawOffset.z, vRawOffset.w));
            }
            //---Enable user interaction
            if(insParams.TryGetValue("UserInteraction", strValue))
            {
                UserInteraction = Core::ParseBool(strValue);
            }
            //---Enable touch consumption
            if(insParams.TryGetValue("ConsumesTouches", strValue))
            {
				EnableTouchConsumption(Core::ParseBool(strValue));
            }
            //---Enable touch consumption
            if(insParams.TryGetValue("ConsumesTouchesBegan", strValue))
            {
				EnableTouchConsumption(Core::ParseBool(strValue), TouchType::k_began);
            }
            //---Enable touch consumption
            if(insParams.TryGetValue("ConsumesTouchesMoved", strValue))
            {
				EnableTouchConsumption(Core::ParseBool(strValue), TouchType::k_moved);
            }
            //---Accept Touches Outside of Bounds
            if(insParams.TryGetValue("AcceptTouchesOutsideOfBounds", strValue))
            {
                EnableAcceptTouchesOutsideOfBounds(Core::ParseBool(strValue));
            }
            //---Colour
            if(insParams.TryGetValue("Colour", strValue))
            {
                SetColour(Core::ParseColour(strValue));
            }
            //---Colour Inheritance
			if (insParams.TryGetValue("InheritColour", strValue))
			{
				InheritColour = Core::ParseBool(strValue);
			}
            //---Movable
            if(insParams.TryGetValue("Movable", strValue))
            {
                SetMovable(Core::ParseBool(strValue));
            }
			//---Visible
            if(insParams.TryGetValue("Visible", strValue))
			{
				Visible = Core::ParseBool(strValue);
			}
			//---Inherit Opacity
			if (insParams.TryGetValue("InheritOpacity", strValue))
			{
				InheritOpacity = Core::ParseBool(strValue);
			}
			//---Opacity
			if (insParams.TryGetValue("Opacity", strValue))
			{
				Opacity = Core::ParseF32(strValue);
			}
        }
		//-----------------------------------------------------
		/// On Transform Changed
		///
		/// Dirty the transform and force it to be recalculated
		//-----------------------------------------------------
		void GUIView::OnTransformChanged(u32 inudwInvalidFlags)
		{
            Core::Utils::BitmapClear(mudwCacheValidaters, inudwInvalidFlags);
			
			for(GUIView::Subviews::iterator it = mSubviews.begin(); it != mSubviews.end(); ++it)
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
        void GUIView::AddSubview(const GUIViewSPtr& inpSubview)
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
        void GUIView::RemoveSubview(GUIView* inpSubview)
        {
            for(GUIView::Subviews::iterator it = mSubviews.begin(); it != mSubviews.end(); ++it)
            {
                if((*it).get() == inpSubview)
                {
                    inpSubview->SetParentView(nullptr);
                    inpSubview->SetRootWindow(nullptr);
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
		void GUIView::RemoveAllSubviews()
		{
			for(GUIView::Subviews::iterator it = mSubviews.begin(); it != mSubviews.end(); ++it)
            {
				(*it)->SetParentView(nullptr);
				(*it)->SetRootWindow(nullptr);
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
        void GUIView::RemoveFromParentView()
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
        void GUIView::LayoutContent()
        {
            
        }
        
        //-----------------------------------------------------
        /// Bring Forward
        ///
        /// Move this view a single layer towards the screen
        //-----------------------------------------------------
        void GUIView::BringForward()
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
        void GUIView::SendBackward()
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
        void GUIView::BringToFront()
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
        void GUIView::SendToBack()
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
        void GUIView::BringSubviewForward(GUIView* inpView)
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
        void GUIView::SendSubviewBackward(GUIView* inpView)
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
        void GUIView::BringSubviewToFront(GUIView* inpView)
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
        void GUIView::SendSubviewToBack(GUIView* inpView)
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
        GUIView* GUIView::GetParentViewPtr() const
        {
            return mpParentView;
        }
		//-----------------------------------------------------
		/// Get Parent With Name
		///
		/// @param Name
		/// @return Parent in hierarchy with given name
		//-----------------------------------------------------
		GUIView* GUIView::GetParentWithName(const std::string& instrName) const
		{
			if(mpParentView)
			{
				if(mpParentView->GetName() == instrName)
				{
					return mpParentView;
				}
				return mpParentView->GetParentWithName(instrName);
			}
			
			return nullptr;
		}
        //-----------------------------------------------------
        /// Set Parent View 
        ///
        /// @param The parent the view is attached to
        //-----------------------------------------------------
        void GUIView::SetParentView(GUIView* inpParentView)
        {
			OnTransformChanged((u32)TransformCache::k_transform|(u32)TransformCache::k_absSize|(u32)TransformCache::k_absPos);
            mpParentView = inpParentView;
        }
		//-----------------------------------------------------
		/// Get Input Events
		///
		/// @return The input events of the view
		//-----------------------------------------------------
		InputEvents* GUIView::GetInputEvents()
		{
			return &mInputEvents;
		}
        //-----------------------------------------------------
        /// Get Root Window Pointer
        ///
        /// @return Window that the view is attached to
        //-----------------------------------------------------
        Window* GUIView::GetRootWindowPtr() const
        {
            return mpRootWindow;
        }
        //-----------------------------------------------------
        /// Set Root Window 
        ///
        /// @param The window the view is attached to
        //-----------------------------------------------------
        void GUIView::SetRootWindow(Window* inpWindow)
        {
            mpRootWindow = inpWindow;
			
			for(GUIView::Subviews::iterator it = mSubviews.begin(); it != mSubviews.end(); ++it)
            {
				(*it)->SetRootWindow(inpWindow);
			}
        }
        //-----------------------------------------------------------
        //-----------------------------------------------------------
        Core::IConnectableEvent<Input::PointerSystem::PointerDownDelegate>& GUIView::GetPointerDownEvent()
        {
            return m_pointerDownEvent;
        }
        //-----------------------------------------------------------
        //-----------------------------------------------------------
        Core::IConnectableEvent<Input::PointerSystem::PointerMovedDelegate>& GUIView::GetPointerMovedEvent()
        {
            return m_pointerMovedEvent;
        }
        //-----------------------------------------------------------
        //-----------------------------------------------------------
        Core::IConnectableEvent<Input::PointerSystem::PointerUpDelegate>& GUIView::GetPointerUpEvent()
        {
            return m_pointerUpEvent;
        }
        //-----------------------------------------------------
        /// Get Subviews
        ///
        /// @return Vector of shared GUI view pointers
        //-----------------------------------------------------
        const GUIView::Subviews& GUIView::GetSubviews() const
        {
            return mSubviews;
        }
		//-----------------------------------------------------
		/// Get Subviews
		///
		/// @return Vector of shared GUI view pointers
		//-----------------------------------------------------
		GUIView::Subviews& GUIView::GetSubviews()
		{
			return mSubviews;
		}
        //-----------------------------------------------------
        /// Get Subview At Index
        ///
        /// @return Subview at the given index in the hierarchy
        //-----------------------------------------------------
        const GUIViewSPtr& GUIView::GetSubviewAtIndex(u32 inudwIndex) const
        {
            return mSubviews[inudwIndex];
        }
        //-----------------------------------------------------
        /// Get Subview With Name
        ///
        /// @param Name
        /// @return Subview in hierarchy with given name
        //-----------------------------------------------------
        const GUIViewSPtr& GUIView::GetSubviewWithName(const std::string& instrName) const
        {
            for(GUIView::Subviews::const_iterator it = mSubviews.begin(); it != mSubviews.end(); ++it)
            {
                //If the subview has the name then return it
                if((*it)->GetName() == instrName)
                {
                    return (*it);
                }
                else
                {
                    //Check if it lives inside the subviews subview
                    const GUIViewSPtr& pView = (*it)->GetSubviewWithName(instrName);
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
		void GUIView::GetSubviewsWithName(const std::string& instrName, std::vector<GUIViewSPtr>& outSubviews) const
        {
            for(GUIView::Subviews::const_iterator it = mSubviews.begin(); it != mSubviews.end(); ++it)
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
        void GUIView::SetLocalAlignment(Rendering::AlignmentAnchor ineAlignment)
        {
			OnTransformChanged((u32)TransformCache::k_transform|(u32)TransformCache::k_absPos);
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
        Rendering::AlignmentAnchor GUIView::GetLocalAlignment() const
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
        void GUIView::EnableRotationWithParent(bool inbEnable)
        {
			OnTransformChanged((u32)TransformCache::k_transform|(u32)TransformCache::k_absPos);
            RotatedWithParent = inbEnable;
        }
		//-----------------------------------------------------
		/// Is Rotated With Parent
		///
		/// @param Whether the view rotates with it's parent
		//-----------------------------------------------------
		bool GUIView::IsRotatedWithParent() const
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
        void GUIView::EnableAlignmentToParent(bool inbEnable)
        {
			OnTransformChanged((u32)TransformCache::k_transform|(u32)TransformCache::k_absPos);
            AlignedWithParent = inbEnable;
        }
        //-----------------------------------------------------
        /// Is Aligned To Parent
        ///
        /// @param Whether the view is aligned with it's
        //-----------------------------------------------------
        bool GUIView::IsAlignedToParent() const
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
        void GUIView::SetAlignmentToParent(Rendering::AlignmentAnchor ineAlignment)
        {
			OnTransformChanged((u32)TransformCache::k_transform|(u32)TransformCache::k_absPos);
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
        Rendering::AlignmentAnchor GUIView::GetAlignmentWithParent() const
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
        void GUIView::SetOffsetFromParentAlignment(const Core::UnifiedVector2& invOffset)
        {
			OnTransformChanged((u32)TransformCache::k_transform|(u32)TransformCache::k_absPos);
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
        void GUIView::SetOffsetFromParentAlignment(f32 infRx, f32 infRy, f32 infAx, f32 infAy)
        {
			OnTransformChanged((u32)TransformCache::k_transform|(u32)TransformCache::k_absPos);
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
        const Core::UnifiedVector2& GUIView::GetOffsetFromParentAlignment() const
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
        const Core::Vector2& GUIView::GetAbsoluteOffsetFromParentAlignment() const
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
        void GUIView::SetOffsetFromPosition(const Core::UnifiedVector2& invOffset)
        {
			OnTransformChanged((u32)TransformCache::k_transform|(u32)TransformCache::k_absPos);
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
        void GUIView::SetOffsetFromPosition(f32 infRx, f32 infRy, f32 infAx, f32 infAy)
        {
			OnTransformChanged((u32)TransformCache::k_transform|(u32)TransformCache::k_absPos);
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
        const Core::UnifiedVector2& GUIView::GetOffsetFromPosition() const
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
        const Core::Vector2& GUIView::GetAbsoluteOffsetFromPosition() const
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
		void GUIView::EnableClipOffScreen(bool inbEnable)
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
		bool GUIView::IsClippedOffScreenEnabled() const
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
        void GUIView::EnableSubviewClipping(bool inbEnable)
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
        bool GUIView::IsSubviewClippingEnabled() const
        {
            return ClipSubviews;
        }
        //-----------------------------------------------------
        /// Set Name
        ///
        /// @param Name of view instance
        //-----------------------------------------------------
        void GUIView::SetName(const std::string& instrName)
        {
            Name = instrName;
        }
        //-----------------------------------------------------
        /// Get Name
        ///
        /// @return Name of view instance
        //-----------------------------------------------------
        const std::string& GUIView::GetName() const
        {
            return Name;
        }
        //-----------------------------------------------------
        /// Set Visible
        ///
        /// @param Toggle whether the view and subviews are
        /// visible or not
        //-----------------------------------------------------
        void GUIView::SetVisible(bool inbVisible)
        {
            Visible = inbVisible;
        }
		//-----------------------------------------------------
		/// Is Visible
		///
		/// @return
		//-----------------------------------------------------
		bool GUIView::IsVisible() const
		{
			return Visible;
		}
		//-----------------------------------------------------
		/// Is Visible In Hierarchy
		///
		/// @return If the entity is visible, checking the ancestors
		//-----------------------------------------------------
		bool GUIView::IsVisibleInHierarchy() const
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
        bool GUIView::IsRendered() const
        {
            bool bIsDrawn = IsVisible();
            if(!bIsDrawn)
                return false;
            
            GUIView * pParentView = GetParentViewPtr();
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
        void GUIView::EnableUserInteraction(bool inbEnabled)
        {
			UserInteraction = inbEnabled;
		}
		//-----------------------------------------------------
		/// Is User Interaction Enabled
		///
		/// @return whether the view receives user input
		//-----------------------------------------------------
		bool GUIView::IsUserInteractionEnabled() const
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
		void GUIView::SetMovable(bool inbMovable)
		{
			Movable = inbMovable;
		}
		//-----------------------------------------------------
		/// Is Movable
		///
		/// @return Whether view is draggable
		//-----------------------------------------------------
		bool GUIView::IsMovable() const
		{
			return Movable;
		}
		//-----------------------------------------------------
		/// Enable Touch Consumption
		///
		/// @param Override to force a view to consome or route touches
		/// @param The type of touch concerned (BEGAN, MOVED or ALL)
        //-----------------------------------------------------
        void GUIView::EnableTouchConsumption(bool inbEnabled, TouchType ineType)
        {
			if(inbEnabled)
			{
				
				ConsumesTouches |= (u32)ineType;
			}
			else
			{
				
				ConsumesTouches &= ~(u32)ineType;
			}
		}
		//-----------------------------------------------------
		/// Is Touch Consumption Enabled
		///
		/// @param The type of touch concerned (BEGAN, MOVED or ALL)
		/// @return Override to force a view to consome or route
		/// touches
		//-----------------------------------------------------
		bool GUIView::IsTouchConsumptionEnabled(TouchType ineType) const
		{
			return ((u32)ineType & ConsumesTouches) != 0;
		}
        //-----------------------------------------------------
        /// Enable Inherited Scale
        ///
        /// @param Whether the parents scale affects the
        /// child
        //-----------------------------------------------------
        void GUIView::EnableInheritedScale(bool inbEnabled)
		{
			InheritScale = inbEnabled;
		}
		//-----------------------------------------------------
		/// Is Inherited Scale Enabled
		///
		/// @return Whether this view inherits scale from its parent
		//-----------------------------------------------------
		bool GUIView::IsInheritedScaleEnabled() const
		{
			return InheritScale;
		}
        //-----------------------------------------------------
        /// Enable Inherited Colour
        ///
        /// @param Whether the parents colour affects the
        /// child
        //-----------------------------------------------------
        void GUIView::EnableInheritedColour(bool inbEnabled)
		{
			InheritColour = inbEnabled;
		}
		//-----------------------------------------------------
		/// Is Inherited Colour Enabled
		///
		/// @return Whether this view inherits colour from its parent
		//-----------------------------------------------------
		bool GUIView::IsInheritedColourEnabled() const
		{
			return InheritColour;
		}
		//-----------------------------------------------------
		/// Enable Inherited Opacity
		///
		/// @param Whether this view inherits opacity from its parent
		//-----------------------------------------------------
		void GUIView::EnableInheritedOpacity(bool inbEnabled)
		{
			InheritOpacity = inbEnabled;
		}
		//-----------------------------------------------------
		/// Is Inherited Opacity Enabled
		///
		/// @return Whether this view inherits opacity from its parent
		//-----------------------------------------------------
		bool GUIView::IsInheritedOpacityEnabled() const
		{
			return InheritOpacity;
		}
        //-----------------------------------------------------
        /// Set Colour
        ///
        /// @param The colour applied to the view when drawn
        //-----------------------------------------------------
        void GUIView::SetColour(const Core::Colour& inColour)
        {
            Colour = inColour;
        }
		//-----------------------------------------------------
		/// Get Colour
		///
		/// @return The colour applied to the view when drawn
		//-----------------------------------------------------
		const Core::Colour& GUIView::GetColour() const
		{
			return Colour;
		}
        //-----------------------------------------------------
        /// Get Absolute Colour
        ///
        /// @param The inherited colour of the view
        //-----------------------------------------------------
        const Core::Colour& GUIView::GetAbsoluteColour()
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
		void GUIView::SetOpacity(f32 infValue)
		{
			Opacity = infValue;
		}
		//-----------------------------------------------------
		/// GetOpacity
		///
		/// @param Returns the opacity of this view.
		//-----------------------------------------------------
		f32 GUIView::GetOpacity() const
		{
			return Opacity;
		}
		//-----------------------------------------------------
		/// GetInheritedOpacity
		///
		/// @return Returns this view's opacity multiplied by the inherited opacity of any parent it may have
		//-----------------------------------------------------
		f32 GUIView::GetInheritedOpacity() const
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
        void GUIView::SetPosition(const Core::UnifiedVector2& invPosition)
        {
			OnTransformChanged((u32)TransformCache::k_transform|(u32)TransformCache::k_absPos);
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
        void GUIView::SetPosition(f32 fRx, f32 fRy, f32 fAx, f32 fAy)
        {
			OnTransformChanged((u32)TransformCache::k_transform|(u32)TransformCache::k_absPos);
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
        void GUIView::MoveBy(const Core::UnifiedVector2& invPosition)
        {
			OnTransformChanged((u32)TransformCache::k_transform|(u32)TransformCache::k_absPos);
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
        void GUIView::MoveBy(f32 fRx, f32 fRy, f32 fAx, f32 fAy)
        {
			OnTransformChanged((u32)TransformCache::k_transform|(u32)TransformCache::k_absPos);
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
        void GUIView::RotateTo(f32 infAngleInRadians)
        {
			OnTransformChanged((u32)TransformCache::k_transform);
            Rotation = infAngleInRadians;
        }
        //------------------------------------------------------
        /// Rotate By
        ///
        /// Rotate the object by the given angle
        ///
        /// @param Angle in radians
        //------------------------------------------------------
        void GUIView::RotateBy(f32 infAngleInRadians)
        {
			OnTransformChanged((u32)TransformCache::k_transform);
            Rotation += infAngleInRadians;
        }
        //------------------------------------------------------
        /// Set Size
        ///
        /// Set the dimensions of the object
        ///
        /// @param Unified co-ordinates for scale factor
        //------------------------------------------------------
        void GUIView::SetSize(const Core::UnifiedVector2& invSize)
        {
			OnTransformChanged((u32)TransformCache::k_transform|(u32)TransformCache::k_absSize);
            UnifiedSize = invSize;
        }
        //------------------------------------------------------
        /// Set Size
        ///
        /// Set the dimensions of the object
        ///
        /// @param Unified co-ordinates Rx, Ry, Ax, Ay 
        //------------------------------------------------------
        void GUIView::SetSize(f32 fRx, f32 fRy, f32 fAx, f32 fAy)
        {
			OnTransformChanged((u32)TransformCache::k_transform|(u32)TransformCache::k_absSize);
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
        void GUIView::ScaleTo(const Core::Vector2& invScale)
        {
			OnTransformChanged((u32)TransformCache::k_transform|(u32)TransformCache::k_absSize);
            Scale = invScale;
        }
        //------------------------------------------------------
        /// Scale To
        ///
        /// Set the scale of the object
        ///
        /// @param Unified co-ordinates fScaleX, fScaleY
        //------------------------------------------------------
        void GUIView::ScaleTo(f32 fScaleX, f32 fScaleY)
        {
			OnTransformChanged((u32)TransformCache::k_transform|(u32)TransformCache::k_absSize);
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
        void GUIView::ScaleTo(f32 fScale)
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
        const Core::UnifiedVector2& GUIView::GetPosition() const
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
        f32 GUIView::GetRotation() const
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
        const Core::UnifiedVector2& GUIView::GetSize() const
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
        const Core::Vector2& GUIView::GetScale() const
        {
            return Scale;
        }
        //-----------------------------------------------------
        /// Get Absolute Scale
        ///
        /// @param The inherited scale of the view
        //-----------------------------------------------------
        const Core::Vector2& GUIView::GetAbsoluteScale() const
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
		const f32 GUIView::GetAbsoluteRotation() const
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
        const Core::Vector2& GUIView::GetAbsolutePosition() const
        {
			if(!Core::Utils::BitmapCheck(mudwCacheValidaters, (u32)TransformCache::k_absPos))
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
						Core::Vector2 vParentSize = mpParentView->GetAbsoluteSize();
						Core::Vector2 vParentHalfSize = vParentSize * 0.5f;
						
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
				Core::Vector2 vAlignment;
				Rendering::Align(LocalAlignment, GetAbsoluteSize() * 0.5f, vAlignment);
				mvAbsolutePosition += vAlignment;
				
				//BUG:
				//BITMASK_SET(mudwCacheValidaters, (u32)TransformCache::k_absPos);
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
        const Core::Vector2& GUIView::GetAbsoluteScreenSpacePosition() const
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
        const Core::Vector2& GUIView::GetAbsoluteSize() const
        {
            //The absolute size is based on the absolute size of our parent 
            //and the relative size of us
            if(mpParentView)
            {
				if(!Core::Utils::BitmapCheck(mudwCacheValidaters, (u32)TransformCache::k_absSize))
				{
					mvAbsoluteSize = ((mpParentView->GetAbsoluteSize() * GetSize().GetRelative()) + GetSize().GetAbsolute()) * GetAbsoluteScale();
                    Core::Utils::BitmapSet(mudwCacheValidaters, (u32)TransformCache::k_absSize);
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
        Core::Vector2 GUIView::GetAbsoluteAnchorPoint(Rendering::AlignmentAnchor ineAlignment) const
        {
			Core::Vector2 vSize = GetAbsoluteSize();
			Core::Vector2 vHalfSize(vSize.x * 0.5f, vSize.y * 0.5f);
			Core::Vector2 vPos;
			Rendering::GetAnchorPoint(ineAlignment, vHalfSize, vPos);

            return vPos;
        }
        //-----------------------------------------------------
        /// Get Absolute Screen Space Anchor Point
        ///
        /// @param Anchor point type
        /// @return Anchor point value as absolute
        //-----------------------------------------------------
        Core::Vector2 GUIView::GetAbsoluteScreenSpaceAnchorPoint(Rendering::AlignmentAnchor ineAlignment) const
        {
			Core::Vector2 vSize = GetAbsoluteSize();
			Core::Vector2 vHalfSize(vSize.x * 0.5f, vSize.y * 0.5f);
			Core::Vector2 vPos;
		
			Rendering::GetAnchorPoint(ineAlignment, vHalfSize, vPos);
				
			return vPos + GetAbsoluteScreenSpacePosition();
        }
        //-----------------------------------------------------
        //-----------------------------------------------------
        Core::Screen* GUIView::GetScreen() const
        {
            return m_screen;
        }
        //-----------------------------------------------------
        /// Get Transform
        ///
        /// @return Transformation matrix
        //-----------------------------------------------------
        const Core::Matrix3& GUIView::GetTransform() const
        {
			if(!Core::Utils::BitmapCheck(mudwCacheValidaters, (u32)TransformCache::k_transform))
			{
				if(mpParentView)
				{
					//Create our transform without respect to our parent
					Core::Matrix3 matTrans = Core::Matrix3::CreateTransform(GetAbsolutePosition(), Core::Vector2::k_one, GetAbsoluteRotation());
					
					//Apply our parents transform
					mmatTransform = matTrans * mpParentView->GetTransform();
				}
				else
				{
					mmatTransform = Core::Matrix3::CreateTransform(GetAbsolutePosition(), Core::Vector2::k_one, Rotation);
				}
				
				Core::Utils::BitmapSet(mudwCacheValidaters, (u32)TransformCache::k_transform);
			}
            
            return mmatTransform;
        }
        //-----------------------------------------------------
        /// Is Onscreen
        ///
        /// @return Whether any part of the view is within
        /// the bounds of the screen
        //-----------------------------------------------------
        bool GUIView::IsOnscreen() const
        {
            //Check if this is on screen
            Core::Vector2 vTopRight = GetAbsoluteScreenSpaceAnchorPoint(Rendering::AlignmentAnchor::k_topRight);
            Core::Vector2 vBottomLeft = GetAbsoluteScreenSpaceAnchorPoint(Rendering::AlignmentAnchor::k_bottomLeft);
        
            return (vTopRight.y >= 0 && vBottomLeft.y <= m_screen->GetResolution().y && vTopRight.x >= 0 && vBottomLeft.x <= m_screen->GetResolution().x);
        }
        
        //---Functional overrides
        //-----------------------------------------------------
        /// Update
        ///
        /// @param Time between frames
        //-----------------------------------------------------
        void GUIView::Update(f32 infDt)
        {
            if(Visible)
            {
				mSubviewsCopy = mSubviews;
                for(GUIView::Subviews::iterator it = mSubviewsCopy.begin(); it != mSubviewsCopy.end(); ++it)
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
        void GUIView::Draw(Rendering::CanvasRenderer * inpCanvas)
        {
			if(Visible && (!ClipOffScreen || IsOnscreen()))
			{
				//This is a container view and has no renderables
				//We will need to render our subviews though
				
				//Check if we force clip our children 
				if(ClipSubviews)
				{
                    Core::Vector2 vBottomLeft = GetAbsoluteScreenSpaceAnchorPoint(Rendering::AlignmentAnchor::k_bottomLeft);
					inpCanvas->PushClipBounds(vBottomLeft, GetAbsoluteSize());
				}
				
				for(GUIView::Subviews::iterator it = mSubviews.begin(); it != mSubviews.end(); ++it)
				{
					if ((*it)->IsVisible())
					{
						(*it)->Draw(inpCanvas);
					}
				}
				
				if(ClipSubviews)
				{
					inpCanvas->PopClipBounds();
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
        bool GUIView::Contains(const Core::Vector2& invPoint) const
        {
            Core::Rectangle sAbsoluteAABB(GetAbsoluteScreenSpaceAnchorPoint(Rendering::AlignmentAnchor::k_middleCentre), GetAbsoluteSize());
            return sAbsoluteAABB.Contains(invPoint);
        }
        //-----------------------------------------------------------
        /// Enable Accept Touches Outside of Bounds
        ///
		/// @param Whether view will listen for touches outwith
		/// it's bounds
        //-----------------------------------------------------------		
		void GUIView::EnableAcceptTouchesOutsideOfBounds(bool inbEnable)
		{
			AcceptTouchesOutsideOfBounds = inbEnable;
		}
		//-----------------------------------------------------------
        /// Is Accept Touches Outside of Bounds Enabled
        ///
		/// @return Whether view will listen for touches outwith
		/// it's bounds
        //-----------------------------------------------------------
		bool GUIView::IsAcceptTouchesOutsideOfBoundsEnabled() const
		{
			return AcceptTouchesOutsideOfBounds;
		}
		//-----------------------------------------------------------
		//-----------------------------------------------------------
		void GUIView::OnScreenResolutionChanged()
		{
			OnTransformChanged((u32)TransformCache::k_transform|(u32)TransformCache::k_absSize|(u32)TransformCache::k_absPos);

			for(GUIView::Subviews::iterator it = mSubviews.begin(); it != mSubviews.end(); ++it)
			{
				(*it)->OnScreenResolutionChanged();
			}
		}
        //---Touch Delegates
        //-----------------------------------------------------------
        //-----------------------------------------------------------
        bool GUIView::OnPointerDown(const Input::PointerSystem::Pointer& in_pointer, f64 in_timestamp, Input::PointerSystem::InputType in_inputType)
        {
            if(UserInteraction)
            {
				if(Movable)
				{
					mbIsBeingDragged = true;
				}

				mSubviewsCopy = mSubviews;

                //Loop through all our children
                for(GUIView::Subviews::reverse_iterator it = mSubviewsCopy.rbegin(); it != mSubviewsCopy.rend(); ++it)
                {
					if((*it)->UserInteraction)
					{
						bool bContains = (*it)->Contains(in_pointer.m_location);
						if((*it)->IsAcceptTouchesOutsideOfBoundsEnabled() || bContains)
						{
							if((*it)->OnPointerDown(in_pointer, in_timestamp, in_inputType))
							{
                                mSubviewsCopy.clear();
								return true;
							}
						}
					}
                }
                //Check for input events
				bool bContains = Contains(in_pointer.m_location);
				if(IsAcceptTouchesOutsideOfBoundsEnabled() || bContains)
				{
					mInputEvents.OnPointerDown(this, in_pointer, bContains);
                    m_pointerDownEvent.NotifyConnections(in_pointer, in_timestamp, in_inputType);
                    
					//We consume this touch as it is within us
					if(IsTouchConsumptionEnabled(TouchType::k_began) && bContains)
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
        //-----------------------------------------------------------
        bool GUIView::OnPointerMoved(const Input::PointerSystem::Pointer& in_pointer, f64 in_timestamp)
        {
            if(UserInteraction)
            {
				if(mbIsBeingDragged)
				{
					if(!AlignedWithParent)
					{
						SetPosition(Core::UnifiedVector2(Core::Vector2::k_zero, in_pointer.m_location));
					}
					else
					{
						SetOffsetFromParentAlignment(Core::UnifiedVector2(Core::Vector2::k_zero, in_pointer.m_location));
					}
				}

				mSubviewsCopy = mSubviews;

                for(GUIView::Subviews::reverse_iterator it = mSubviewsCopy.rbegin(); it != mSubviewsCopy.rend(); ++it)
                {
                    if((*it)->OnPointerMoved(in_pointer, in_timestamp))
                    {
                        mSubviewsCopy.clear();
                        return true;
                    }
                }
                
                //Check for input events
                //If we contain this touch we can consume it
                if(mInputEvents.OnPointerMoved(this, in_pointer))
                {
                    m_pointerMovedEvent.NotifyConnections(in_pointer, in_timestamp);
                    if(IsTouchConsumptionEnabled(TouchType::k_moved))
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
        //-----------------------------------------------------------
        void GUIView::OnPointerUp(const Input::PointerSystem::Pointer& in_pointer, f64 in_timestamp, Input::PointerSystem::InputType in_inputType)
        {
			mbIsBeingDragged = false;

            if(UserInteraction)
            {
				mSubviewsCopy = mSubviews;
				
                for(GUIView::Subviews::reverse_iterator it = mSubviewsCopy.rbegin(); it != mSubviewsCopy.rend(); ++it)
                {
                    (*it)->OnPointerUp(in_pointer, in_timestamp, in_inputType);
                }
                
                m_pointerUpEvent.NotifyConnections(in_pointer, in_timestamp, in_inputType);
                mInputEvents.OnPointerUp(this, in_pointer);
            }
            mSubviewsCopy.clear();
        }
        
        //-----------------------------------------------------
        /// Layout Children's Content
        ///
        /// Calls LayoutContent on its subviews. Must be
        /// attached to root window.
        //-----------------------------------------------------
        void GUIView::LayoutChildrensContent()
        {
            Subviews aSubviewsCopy = mSubviews;
            for(GUIView::Subviews::iterator it = aSubviewsCopy.begin(); it != aSubviewsCopy.end(); ++it)
            {
                (*it)->LayoutContent();
                (*it)->LayoutChildrensContent();
            }
        }
		//-----------------------------------------------------
		/// Destructor
		//-----------------------------------------------------
		GUIView::~GUIView()
		{
			RemoveAllSubviews();
		}
    }
}
