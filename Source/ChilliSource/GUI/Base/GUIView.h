//
//  GUIView.h
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

//===============================================================================================
/// GUI View
///
/// A base view class from which all other GUI elements inherit. Can act as a empty 
/// container view for subviews. Houses the main UI functionality.
//===============================================================================================

#ifndef _CHILLISOURCE_GUI_GUI_VIEW_H_
#define _CHILLISOURCE_GUI_GUI_VIEW_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/GUI/Base/InputEvents.h>
#include <ChilliSource/Rendering/Base/AlignmentAnchors.h>
#include <ChilliSource/Core/Container/ParamDictionary.h>
#include <ChilliSource/Core/Math/Matrix3.h>
#include <ChilliSource/Core/Math/UnifiedCoordinates.h>
#include <ChilliSource/Core/Reflection/Reflection.h>
#include <ChilliSource/Core/Base/Colour.h>
#include <ChilliSource/Input/Pointer/PointerSystem.h>

namespace ChilliSource
{
	namespace GUI
	{
		class GUIView
		{
		protected:
            
			enum class TransformCache
			{
                k_transform = (1 << 0),
                k_absSize = (1 << 1),
                k_absPos = (1 << 2)
			};

			mutable u32 mudwCacheValidaters;
			
		public:
            
			enum class TouchType
			{
                k_began = (1 << 0),
                k_moved = (1 << 1),
                k_all = k_began | k_moved
			};

		public:

			DECLARE_META_CLASS(GUIView)

			typedef std::vector<GUIViewSPtr> Subviews;

			GUIView();
			GUIView(const Core::ParamDictionary& insParams);
			virtual ~GUIView();

			//---Hierarchy Manipulation
			//-----------------------------------------------------
			/// Add Subview
			///
			/// Add a view to the hierarchy
			///
			/// @param GUIView shared pointer
			//-----------------------------------------------------
			virtual void AddSubview(const GUIViewSPtr& inpSubview);
			//-----------------------------------------------------
			/// Remove Subview (Internal)
			///
			/// Remove a view from our hierarchy
			///
			/// @param GUIView pointer
			//-----------------------------------------------------
			virtual void RemoveSubview(GUIView* inpSubview);

			//-----------------------------------------------------
			/// Remove Subview (Internal)
			///
			/// Remove a view from our hierarchy
			///
			/// @param GUIView pointer
			//-----------------------------------------------------			
			virtual void RemoveAllSubviews();
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
			void RemoveFromParentView();

			//-----------------------------------------------------
			/// Layout Content
			///
			/// Function used to layout GUI content when absolute
			/// sizes are needed. Must be attached to root window.
			//-----------------------------------------------------
			virtual void LayoutContent();

			//-----------------------------------------------------
			/// Get Parent View Pointer
			///
			/// @return Raw GUI view pointer
			//-----------------------------------------------------
			GUIView* GetParentViewPtr() const;
			//-----------------------------------------------------
			/// Get Parent With Name
			///
			/// @param Name
			/// @return Parent in hierarchy with given name
			//-----------------------------------------------------
			GUIView* GetParentWithName(const std::string& instrName) const;
			//-----------------------------------------------------
			/// Get Root Window Pointer
			///
			/// @return Window that the view is attached to
			//-----------------------------------------------------
			Window* GetRootWindowPtr() const;
			//-----------------------------------------------------
			/// Get Subviews
			///
			/// @return Vector of shared GUI view pointers
			//-----------------------------------------------------
			const Subviews& GetSubviews() const;
			//-----------------------------------------------------
			/// Get Subviews
			///
			/// @return Vector of shared GUI view pointers
			//-----------------------------------------------------
			Subviews& GetSubviews();
			//-----------------------------------------------------
			/// Get Subview At Index
			///
			/// @param Index
			/// @return Subview at the given index in the hierarchy
			//-----------------------------------------------------
			const GUIViewSPtr& GetSubviewAtIndex(u32 inudwIndex) const;
			//-----------------------------------------------------
			/// Get Subview With Name
			///
			/// @param Name
			/// @return Subview in hierarchy with given name
			//-----------------------------------------------------
			const GUIViewSPtr& GetSubviewWithName(const std::string& instrName) const;
			//-----------------------------------------------------
			/// Get Subviews With Name
			///
			/// @param Name
			/// @param out List of subviews in hierarchy with given name
			//-----------------------------------------------------
			void GetSubviewsWithName(const std::string& instrName, std::vector<GUIViewSPtr>& outSubviews) const;
			//-----------------------------------------------------
			/// Bring Forward
			///
			/// Move this view a single layer towards the screen
			//-----------------------------------------------------
			void BringForward();
			//-----------------------------------------------------
			/// Send Backward
			///
			/// Move this view a layer away from the screen
			//-----------------------------------------------------
			void SendBackward();
			//-----------------------------------------------------
			/// Bring To Front
			///
			/// Move this view to the top of the view hierarchy
			//-----------------------------------------------------
			void BringToFront();
			//-----------------------------------------------------
			/// Send To Back
			///
			/// Move this view to the end of the view hierarchy
			//-----------------------------------------------------
			void SendToBack();
			//-----------------------------------------------------
			/// Bring Subview Forward
			///
			/// Move this view a single layer towards the screen
			//-----------------------------------------------------
			void BringSubviewForward(GUIView* inpView);
			//-----------------------------------------------------
			/// Send Subview Backward
			///
			/// Move this view a layer away from the screen
			//-----------------------------------------------------
			void SendSubviewBackward(GUIView* inpView);
			//-----------------------------------------------------
			/// Bring Subview To Front
			///
			/// Move this view to the top of the view hierarchy
			//-----------------------------------------------------
			void BringSubviewToFront(GUIView* inpView);
			//-----------------------------------------------------
			/// Send Subview To Back
			///
			/// Move this view to the end of the view hierarchy
			//-----------------------------------------------------
			void SendSubviewToBack(GUIView* inpView);

			//---View properties
			//-----------------------------------------------------
			/// Set Name
			///
			/// @param Name of view instance
			//-----------------------------------------------------
			void SetName(const std::string& instrName);
			//-----------------------------------------------------
			/// Get Name
			///
			/// @return Name of view instance
			//-----------------------------------------------------
			const std::string& GetName() const;
			//-----------------------------------------------------
			/// Set Colour
			///
			/// @param The colour applied to the view when drawn
			//-----------------------------------------------------
			void SetColour(const Core::Colour& inColour);
			//-----------------------------------------------------
			/// Get Colour
			///
			/// @return The colour applied to the view when drawn
			//-----------------------------------------------------
			const Core::Colour& GetColour() const;
			//-----------------------------------------------------
			/// Get Absolute Colour
			///
			/// @param The inherited colour of the view
			//-----------------------------------------------------
			const Core::Colour& GetAbsoluteColour();
			//-----------------------------------------------------
			/// Set Opacity
			///
			/// @param Fractional opacity to set for this view (0 - 1)
			//-----------------------------------------------------
			void SetOpacity(f32 infValue);
			//-----------------------------------------------------
			/// GetOpacity
			///
			/// @param Returns the opacity of this view.
			//-----------------------------------------------------
			f32 GetOpacity() const;
			//-----------------------------------------------------
			/// GetInheritedOpacity
			///
			/// @return Returns this view's opacity multiplied by the inherited opacity of any parent it may have
			//-----------------------------------------------------
			f32 GetInheritedOpacity() const;
			
			//-----------------------------------------------------
			/// Enable Clip Off Screen
			///
			/// Toggle whether the view is clipped when exceeding
			/// the boundary of the screen
			///
			/// @param Enable/disable
			//-----------------------------------------------------
			void EnableClipOffScreen(bool inbEnable);
			//-----------------------------------------------------
			/// Is Clipped Off Screen Enabled
			///
			/// Toggle whether the view is clipped when exceeding
			/// the boundary of the screen
			///
			/// @return Enable/disable
			//-----------------------------------------------------
			bool IsClippedOffScreenEnabled() const;
			//-----------------------------------------------------
			/// Enable Subview Clipping
			///
			/// Toggle whether the view is clipped when exceeding
			/// the boundary of it's parent view
			///
			/// @param Enable/disable
			//-----------------------------------------------------
			void EnableSubviewClipping(bool inbEnable);
			//-----------------------------------------------------
			/// Is Subview Clipping Enabled
			///
			/// Toggle whether the view is clipped when exceeding
			/// the boundary of it's parent view
			///
			/// @return Enable/disable
			//-----------------------------------------------------
			bool IsSubviewClippingEnabled() const;
			//-----------------------------------------------------
			/// Set Visible
			///
			/// @param Toggle whether the view and subviews are
			/// visible or not
			//-----------------------------------------------------
			void SetVisible(bool inbVisible);
			//-----------------------------------------------------
			/// Is Visible
			///
			/// @return
			//-----------------------------------------------------
			bool IsVisible() const;
			//-----------------------------------------------------
			/// Is Visible In Hierarchy
			///
			/// @return If the entity is visible, checking the ancestors
			//-----------------------------------------------------
			bool IsVisibleInHierarchy() const;
            //-----------------------------------------------------
            /// Is Rendered
            ///
            /// Determines if view is being drawn to screen
            /// takes into consideration if any of it's parents
            /// visibility is set to false.
            ///
            /// @returns If being drawn
            //-----------------------------------------------------
            bool IsRendered() const;
			//-----------------------------------------------------
			/// Enable User Interaction
			///
			/// Toggle whether the view receives user input
			///
			/// @param Enable/disable
			//-----------------------------------------------------
			void EnableUserInteraction(bool inbEnabled);
			//-----------------------------------------------------
			/// Is User Interaction Enabled
			///
			/// @return whether the view receives user input
			//-----------------------------------------------------
			bool IsUserInteractionEnabled() const;
			//-----------------------------------------------------
			/// Set Movable
			///
			/// Toggle whether the user can move the UI
			///
			/// @param Enable/disable
			//-----------------------------------------------------
			void SetMovable(bool inbMovable);
			//-----------------------------------------------------
			/// Is Movable
			///
			/// @return Whether view is draggable
			//-----------------------------------------------------
			bool IsMovable() const;
			//-----------------------------------------------------
			/// Enable Touch Consumption
			///
			/// @param Override to force a view to consome or route
			/// touches
			/// @param The type of touch concerned (BEGAN, MOVED or ALL)
			//-----------------------------------------------------
			void EnableTouchConsumption(bool inbEnabled, TouchType ineTouch = TouchType::k_all);
			//-----------------------------------------------------
			/// Is Touch Consumption Enabled
			///
			/// @param The type of touch concerned (BEGAN, MOVED or ALL)
			/// @return Override to force a view to consome or route
			/// touches
			//-----------------------------------------------------
			bool IsTouchConsumptionEnabled(TouchType ineTouch = TouchType::k_all) const;
			//-----------------------------------------------------
			/// Enable Inherited Scale
			///
			/// @param Whether the parents scale affects the
			/// child
			//-----------------------------------------------------
			void EnableInheritedScale(bool inbEnabled);
			//-----------------------------------------------------
			/// Is Inherited Scale Enabled
			///
			/// @return Whether this view inherits scale from its parent
			//-----------------------------------------------------
			bool IsInheritedScaleEnabled() const;
			//-----------------------------------------------------
			/// Enable Inherited Colour
			///
			/// @param Whether the parents colour affects the
			/// child
			//-----------------------------------------------------
			void EnableInheritedColour(bool inbEnabled);
			//-----------------------------------------------------
			/// Is Inherited Colour Enabled
			///
			/// @return Whether this view inherits colour from its parent
			//-----------------------------------------------------
			bool IsInheritedColourEnabled() const;
			//-----------------------------------------------------
			/// Enable Inherited Opacity
			///
			/// @param Whether this view inherits opacity from its parent
			//-----------------------------------------------------
			void EnableInheritedOpacity(bool inbEnabled);
			//-----------------------------------------------------
			/// Is Inherited Opacity Enabled
			///
			/// @return Whether this view inherits opacity from its parent
			//-----------------------------------------------------
			bool IsInheritedOpacityEnabled() const;

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
			void SetLocalAlignment(Rendering::AlignmentAnchor ineAlignment);
			//-----------------------------------------------------
			/// Get Local Alignment
			///
			/// Get the pivot point of the view. This is the point
			/// about which all transforms are applied and from 
			/// which any renderable content is drawn
			///
			/// @return Alignment type
			//-----------------------------------------------------
			Rendering::AlignmentAnchor GetLocalAlignment() const;
			//-----------------------------------------------------
			/// Enable Rotation With Parent
			///
			/// Toggle whether the view rotates with it's
			/// parent
			///
			/// @param Enable/disable
			//-----------------------------------------------------
			void EnableRotationWithParent(bool inbEnable);
			//-----------------------------------------------------
			/// Is Rotated With Parent
			///
			/// @param Whether the view rotates with it's parent
			//-----------------------------------------------------
			bool IsRotatedWithParent() const;
			//-----------------------------------------------------
			/// Enable Alignment To Parent
			///
			/// Toggle whether the view is aligned with it's
			/// parent
			///
			/// @param Enable/disable
			//-----------------------------------------------------
			void EnableAlignmentToParent(bool inbEnable);
			//-----------------------------------------------------
			/// Is Aligned To Parent
			///
			/// @param Whether the view is aligned with it's parent
			//-----------------------------------------------------
			bool IsAlignedToParent() const;
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
			void SetAlignmentToParent(Rendering::AlignmentAnchor ineAlignment);
			//-----------------------------------------------------
			/// Get Alignment with Parent
			///
			/// Get the anchor point about which the child view
			/// aligns to it's parent
			///
			/// @return Alignment type
			//-----------------------------------------------------
			Rendering::AlignmentAnchor GetAlignmentWithParent() const;
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
			void SetOffsetFromParentAlignment(const Core::UnifiedVector2& invOffset);
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
			void SetOffsetFromParentAlignment(f32 infRx, f32 infRy, f32 infAx, f32 infAy);
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
			const Core::UnifiedVector2& GetOffsetFromParentAlignment() const;
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
			const Core::Vector2& GetAbsoluteOffsetFromParentAlignment() const;
			//-----------------------------------------------------
			/// Set Offset From Alignment
			///
			/// If alignment is disabled then the view will be 
			/// offset by the given co-ords from the anchor point
			///
			/// @param Unified co-ordinates
			//-----------------------------------------------------
			void SetOffsetFromPosition(const Core::UnifiedVector2& invOffset);
			//-----------------------------------------------------
			/// Set Offset From Position
			///
			/// If alignment is disabled then the view will be 
			/// offset by the given co-ords from the anchor point
			///
			/// @param Unified co-ordinates
			//-----------------------------------------------------
			void SetOffsetFromPosition(f32 infRx, f32 infRy, f32 infAx, f32 infAy);
			//-----------------------------------------------------
			/// Get Offset From Position
			///
			/// If alignment is disabled then the view will be 
			/// offset by the given co-ords from the anchor point
			///
			/// @return Unified co-ordinates
			//-----------------------------------------------------
			const Core::UnifiedVector2& GetOffsetFromPosition() const;
			//-----------------------------------------------------
			/// Get Absolute Offset From Position
			///
			/// If alignment is disabled then the view will be 
			/// offset by the given co-ords from the anchor point
			///
			/// @return Unified co-ordinates converted to real
			/// co-ordinates
			//-----------------------------------------------------
			const Core::Vector2& GetAbsoluteOffsetFromPosition() const;
			//-----------------------------------------------------
			/// Get Absolute Anchor Point
			///
			/// @param Anchor point type
			/// @return Anchor point value as absolute
			//-----------------------------------------------------
			Core::Vector2 GetAbsoluteAnchorPoint(Rendering::AlignmentAnchor ineAlignment) const;
			//-----------------------------------------------------
			/// Get Absolute Screen Space Anchor Point
			///
			/// @param Anchor point type
			/// @return Anchor point value as absolute
			//-----------------------------------------------------
			Core::Vector2 GetAbsoluteScreenSpaceAnchorPoint(Rendering::AlignmentAnchor ineAlignment) const;
            //-----------------------------------------------------
            /// Is Onscreen
            ///
            /// @return Whether any part of the view is within
            /// the bounds of the screen
            //-----------------------------------------------------
            bool IsOnscreen() const;

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
			void SetPosition(const Core::UnifiedVector2& invPosition);
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
			void SetPosition(f32 fRx, f32 fRy, f32 fAx, f32 fAy);
			//------------------------------------------------------
			/// Move By
			///
			/// Offset the current accumulated position by the
			/// given co-ordinates
			///
			/// @param co-ordinates
			//------------------------------------------------------
			void MoveBy(const Core::UnifiedVector2& invPosition);
			//------------------------------------------------------
			/// Move By
			///
			/// Offset the current accumulated position by the
			/// given co-ordinates
			///
			/// @param Unified co-ordinates Rx, Ry, Ax, Ay 
			//------------------------------------------------------
			void MoveBy(f32 fRx, f32 fRy, f32 fAx, f32 fAy);
			//------------------------------------------------------
			/// Rotate To
			///
			/// Rotate the object to the given angle with respect
			/// to the parents rotation
			///
			/// @param Angle in radians
			//------------------------------------------------------
			void RotateTo(f32 infAngleInRadians);
			//------------------------------------------------------
			/// Rotate By
			///
			/// Rotate the object by the given angle
			///
			/// @param Angle in radians
			//------------------------------------------------------
			void RotateBy(f32 infAngleInRadians);
			//------------------------------------------------------
			/// Set Size
			///
			/// Set the dimensions of the object
			///
			/// @param Unified co-ordinates 
			//------------------------------------------------------
			void SetSize(const Core::UnifiedVector2& invSize);
			//------------------------------------------------------
			/// Set Size
			///
			/// Set the dimensions of the object
			///
			/// @param Unified co-ordinates Rx, Ry, Ax, Ay 
			//------------------------------------------------------
			void SetSize(f32 fRx, f32 fRy, f32 fAx, f32 fAy);
			//------------------------------------------------------
			/// Scale To
			///
			/// Set the scale of the object
			///
			/// @param Unified co-ordinates 
			//------------------------------------------------------
			void ScaleTo(const Core::Vector2& invScale);
			//------------------------------------------------------
			/// Scale To
			///
			/// Set the scale of the object
			///
			/// @param Scale co-ordinates fScaleX, fScaleY
			//------------------------------------------------------
			void ScaleTo(f32 fScaleX, f32 fScaleY);
			//------------------------------------------------------
			/// Scale To
			///
			/// Set the uniform scale of the object
			///
			/// @param Scale uniform co-ordinate fScale
			//------------------------------------------------------
			void ScaleTo(f32 fScale);
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
			const Core::UnifiedVector2& GetPosition() const;
			//------------------------------------------------------
			/// Get Rotation
			///
			/// Rotation of object in radians
			///
			/// @return Angle in radians
			//------------------------------------------------------
			f32 GetRotation() const;
			//------------------------------------------------------
			/// Get Size
			///
			/// Get the dimensions of the object
			///
			/// @return Unified co-ordinates for dimensions
			//------------------------------------------------------
			const Core::UnifiedVector2& GetSize() const;
			//------------------------------------------------------
			/// Get Scale
			///
			/// Get the scale of the object
			///
			/// @return Unified co-ordinates for scale
			//------------------------------------------------------
			const Core::Vector2& GetScale() const;
			//-----------------------------------------------------
			/// Get Absolute Scale
			///
			/// @param The inherited scale of the view
			//-----------------------------------------------------
			const Core::Vector2& GetAbsoluteScale() const;
			//-----------------------------------------------------
			/// Get Absolute Screen Space Position
			///
			/// The position of the view after all parental
			/// transforms have been applied. This is it's final
			/// render position on screen
			///
			/// @return Position combined absolute and relative
			//------------------------------------------------------
			const Core::Vector2& GetAbsoluteScreenSpacePosition() const;
			//-----------------------------------------------------
			/// Get Absolute Rotation
			///
			/// Rotation in parent views space. This is the cumulated
			/// rotation of the view hierarchy.
			///
			/// @return Cumulated rotation of the parent hierarchy
			//------------------------------------------------------
			const f32 GetAbsoluteRotation() const;
			//-----------------------------------------------------
			/// Get Absolute Position
			///
			/// Position in parent views space. This is the unified
			/// co-ordinates transformed into absolute
			///
			/// @return Position combined absolute and relative
			//------------------------------------------------------
			const Core::Vector2& GetAbsolutePosition() const;
			//------------------------------------------------------
			/// Get Absolute Size
			///
			/// Get the real dimensions based by combining absolute 
			/// and relative
			///
			/// @return Size combined absolute and relative
			//------------------------------------------------------
			const Core::Vector2& GetAbsoluteSize() const;

			//---Functional overrides
			//-----------------------------------------------------
			/// Update
			///
			/// @param Time between frames
			//-----------------------------------------------------
			virtual void Update(f32 infDt);
			//-----------------------------------------------------
			/// Draw
			///
			/// Override this implementation to draw the GUI
			/// using the primitves within canvas renderer i.e.
			/// DrawBox, DrawString etc
			///
			/// @param Canvas renderer
			//-----------------------------------------------------
			virtual void Draw(Rendering::CanvasRenderer * inpCanvas);
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
			virtual bool Contains(const Core::Vector2& invPoint) const;
			//-----------------------------------------------------------
			/// Enable Accept Touches Outside of Bounds
			///
			/// When set to true, the view will receive touch information
			/// and pass it on to its children regardless of whether the
			/// touch is inside its bounding box
			//-----------------------------------------------------------
			void EnableAcceptTouchesOutsideOfBounds(bool inbEnable);
			//-----------------------------------------------------------
			/// Is Accept Touches Outside of Bounds Enabled
			///
			/// @return Whether view will listen for touches outwith
			/// it's bounds
			//-----------------------------------------------------------
			bool IsAcceptTouchesOutsideOfBoundsEnabled() const;
			//-----------------------------------------------------------
			/// On Screen Resolution Changed
			///
			/// Triggered if the screen resolution changes so we can
			/// resize ourself
			//-----------------------------------------------------------
			virtual void OnScreenResolutionChanged();

			//---Touch Delegates
			//-----------------------------------------------------------
			/// Called when the window receives cursor/touch input
			///
            /// @author S Downie
            ///
			/// @param The pointer
            /// @param The timestamp
            /// @param The press type.
			///
            /// @return Whether touch has been consumed
			//-----------------------------------------------------------
			virtual bool OnPointerDown(const Input::PointerSystem::Pointer& in_pointer, f64 in_timestamp, Input::PointerSystem::InputType in_inputType);
			//-----------------------------------------------------------
			/// Called when the window receives cursor/touch input
			///
            /// @author S Downie
            ///
			/// @param Touch data
            /// @param The timestamp
            ///
			/// @return Whether touch has been consumed
			//-----------------------------------------------------------
			virtual bool OnPointerMoved(const Input::PointerSystem::Pointer& in_pointer, f64 in_timestamp);
			//-----------------------------------------------------------
			/// Called when the window stops receiving cursor/touch input
			///
            /// @author S Downie
            ///
			/// @param Touch data
            /// @param The timestamp
            /// @param The press type.
			//-----------------------------------------------------------
			virtual void OnPointerUp(const Input::PointerSystem::Pointer& in_pointer, f64 in_timestamp, Input::PointerSystem::InputType in_inputType);
			//-----------------------------------------------------
			/// Set Parent View 
			///
			/// @param The parent the view is attached to
			//-----------------------------------------------------
			void SetParentView(GUIView* inpParentView);
			//-----------------------------------------------------
			/// Get Input Events
			///
			/// @return The input events of the view
			//-----------------------------------------------------
			InputEvents* GetInputEvents();
			//-----------------------------------------------------
			/// Set Root Window 
			///
			/// @param The window the view is attached to
			//-----------------------------------------------------
			void SetRootWindow(Window* inpWindow);
            //-----------------------------------------------------------
            /// Use this to register for touch notifications that
            /// are not consumed by the GUI.
            ///
            /// @author S Downie
            ///
            /// @return Touch began event
            //-----------------------------------------------------------
            Core::IConnectableEvent<Input::PointerSystem::PointerDownDelegate>& GetPointerDownEvent();
            //-----------------------------------------------------------
            /// Use this to register for touch notifications that
            /// are not consumed by the GUI
            ///
            /// @author S Downie
            ///
            /// @return Touch moved event
            //-----------------------------------------------------------
            Core::IConnectableEvent<Input::PointerSystem::PointerMovedDelegate>& GetPointerMovedEvent();
            //-----------------------------------------------------------
            /// Use this to register for touch notifications that
            /// are not consumed by the GUI
            ///
            /// @author S Downie
            ///
            /// @return Touch end event
            //-----------------------------------------------------------
            Core::IConnectableEvent<Input::PointerSystem::PointerUpDelegate>& GetPointerUpEvent();
            
		protected:
			//-----------------------------------------------------
			/// @author Ian Copland
            ///
            /// @return The screen.
			//-----------------------------------------------------
            Core::Screen* GetScreen() const;
			//-----------------------------------------------------
			/// Get Transform
			///
			/// @return Transformation matrix
			//-----------------------------------------------------
			const Core::Matrix3& GetTransform() const;
			//-----------------------------------------------------
			/// On Transform Changed
			///
			/// Dirty the transform and force it to be recalculated
			//-----------------------------------------------------
			virtual void OnTransformChanged(u32 inudwInvalidFlags);
			
			//-----------------------------------------------------
			/// On Subview Added
			///
			/// Called whenever a subview is added
			///
			/// @param GUIView shared pointer
			//-----------------------------------------------------
			virtual void OnSubviewAdded(const GUIViewSPtr& inpSubview) { }
			//-----------------------------------------------------
			/// Layout Children's Content
			///
			/// Calls LayoutContent on its subviews. Must be
			/// attached to root window.
			//-----------------------------------------------------
			void LayoutChildrensContent();
            //-----------------------------------------------------
            /// Get Absolute Rotation
			///
			/// Rotation in parent views space. This is the cumulated
			/// rotation of the view hierarchy.
			///
			/// @return Cumulated rotation of the parent hierarchy
            //------------------------------------------------------
            const f32 GetParentSpaceRotation() const;
		private:


			mutable Core::Matrix3 mmatTransform;

			Core::Colour mAbsoluteColour;
			mutable Core::Vector2 mvAbsolutePosition;
			mutable Core::Vector2 mvAbsoluteScale;
			mutable Core::Vector2 mvAbsoluteSize;
			mutable Core::Vector2 mvAbsoluteParentalOffset;
			mutable Core::Vector2 mvAbsolutePositionOffset;

            Core::Screen* m_screen;
            
		protected:
            
            Core::Event<Input::PointerSystem::PointerDownDelegate> m_pointerDownEvent;
            Core::Event<Input::PointerSystem::PointerMovedDelegate> m_pointerMovedEvent;
            Core::Event<Input::PointerSystem::PointerUpDelegate> m_pointerUpEvent;
            
			Subviews mSubviews;
			Subviews mSubviewsCopy;

			InputEvents mInputEvents;

			GUIView* mpParentView;
			Window* mpRootWindow;

			bool mbIsBeingDragged;
			
			u8 ConsumesTouches;

			//---Properties
			DECLARE_PROPERTY_A(std::string, Name, SetName, GetName);
			DECLARE_PROPERTY_A(Core::UnifiedVector2, UnifiedPosition, SetPosition, GetPosition);
			DECLARE_PROPERTY_A(Core::UnifiedVector2, UnifiedSize, SetSize, GetSize);
			DECLARE_PROPERTY_A(Core::UnifiedVector2, UnifiedParentalOffset, SetOffsetFromParentAlignment, GetOffsetFromParentAlignment);
			DECLARE_PROPERTY_A(Core::UnifiedVector2, UnifiedPositionOffset, SetOffsetFromPosition, GetOffsetFromPosition);
			DECLARE_PROPERTY_A(Core::Vector2, Scale, ScaleTo, GetScale);
			DECLARE_PROPERTY_A(f32, Rotation, RotateTo, GetRotation);
			DECLARE_PROPERTY_A(f32, Opacity, SetOpacity, GetOpacity);
			DECLARE_PROPERTY_A(Rendering::AlignmentAnchor, LocalAlignment, SetLocalAlignment, GetLocalAlignment);
			DECLARE_PROPERTY_A(Rendering::AlignmentAnchor, ParentalAlignment, SetAlignmentToParent, GetAlignmentWithParent);
			DECLARE_PROPERTY_A(bool, AlignedWithParent, EnableAlignmentToParent, IsAlignedToParent);
			DECLARE_PROPERTY_A(bool, RotatedWithParent, EnableRotationWithParent, IsRotatedWithParent);
			DECLARE_PROPERTY_A(bool, ClipOffScreen, EnableClipOffScreen, IsClippedOffScreenEnabled);
			DECLARE_PROPERTY_A(bool, ClipSubviews, EnableSubviewClipping, IsSubviewClippingEnabled);
			DECLARE_PROPERTY_A(bool, InheritScale, EnableInheritedScale, IsInheritedScaleEnabled);
			DECLARE_PROPERTY_A(bool, InheritColour, EnableInheritedColour, IsInheritedColourEnabled);
			DECLARE_PROPERTY_A(bool, Visible, SetVisible, IsVisible);
			DECLARE_PROPERTY_A(bool, Movable, SetMovable, IsMovable);
			DECLARE_PROPERTY_A(bool, UserInteraction, EnableUserInteraction, IsUserInteractionEnabled);
			DECLARE_PROPERTY_A(bool, AcceptTouchesOutsideOfBounds, EnableAcceptTouchesOutsideOfBounds, IsAcceptTouchesOutsideOfBoundsEnabled);
			DECLARE_PROPERTY_A(bool, InheritOpacity, EnableInheritedOpacity, IsInheritedOpacityEnabled);
			DECLARE_PROPERTY_A(Core::Colour, Colour, SetColour, GetColour);
		};
	}
}

#endif