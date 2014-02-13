//
//  GUIView.h
//  moFloTest
//
//  Created by Scott Downie on 21/04/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

//===============================================================================================
/// GUI View
///
/// A base view class from which all other GUI elements inherit. Can act as a empty 
/// container view for subviews. Houses the main UI functionality.
//===============================================================================================

#ifndef _MO_FLO_GUI_GUI_VIEW_H_
#define _MO_FLO_GUI_GUI_VIEW_H_

#include <ChilliSource/Rendering/ForwardDeclarations.h>
#include <ChilliSource/GUI/ForwardDeclarations.h>
#include <ChilliSource/GUI/InputEvents.h>

#include <ChilliSource/Rendering/ForwardDeclarations.h>

#include <ChilliSource/Core/AlignmentAnchors.h>
#include <ChilliSource/Core/ParamDictionary.h>
#include <ChilliSource/Core/Surface.h>
#include <ChilliSource/Core/Math/Matrix3x3.h>
#include <ChilliSource/Core/Math/UnifiedCoordinates.h>

#include <ChilliSource/Input/TouchScreen.h>

#include <ChilliSource/Core/Reflection/Reflection.h>

namespace moFlo
{
	namespace GUI
	{
		class CGUIView : public ISurface
		{
		protected:
			enum TransformCache
			{
				TRANSFORM_CACHE = (1 << 0),
				ABS_SIZE_CACHE = (1 << 1),
				ABS_POS_CACHE = (1 << 2)
			};

			mutable u32 mudwCacheValidaters;
			
		public:
			enum TouchType
			{
				TOUCH_TYPE_BEGAN = (1 << 0),
				TOUCH_TYPE_MOVED = (1 << 1),
				TOUCH_TYPE_ALL	 = TOUCH_TYPE_BEGAN | TOUCH_TYPE_MOVED
			};

		public:

			DECLARE_META_CLASS(CGUIView)

			typedef DYNAMIC_ARRAY<GUIViewPtr> Subviews;

			CGUIView();
			CGUIView(const Core::ParamDictionary& insParams);
			virtual ~CGUIView();

			//---Hierarchy Manipulation
			//-----------------------------------------------------
			/// Add Subview
			///
			/// Add a view to the hierarchy
			///
			/// @param GUIView shared pointer
			//-----------------------------------------------------
			virtual void AddSubview(const GUIViewPtr& inpSubview);
			//-----------------------------------------------------
			/// Remove Subview (Internal)
			///
			/// Remove a view from our hierarchy
			///
			/// @param GUIView pointer
			//-----------------------------------------------------
			virtual void RemoveSubview(CGUIView* inpSubview);

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
			CGUIView* GetParentViewPtr() const;
			//-----------------------------------------------------
			/// Get Parent With Name
			///
			/// @param Name
			/// @return Parent in hierarchy with given name
			//-----------------------------------------------------
			CGUIView* GetParentWithName(const std::string& instrName) const;
			//-----------------------------------------------------
			/// Get Root Window Pointer
			///
			/// @return Window that the view is attached to
			//-----------------------------------------------------
			CWindow* GetRootWindowPtr() const;
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
			const GUIViewPtr& GetSubviewAtIndex(u32 inudwIndex) const;
			//-----------------------------------------------------
			/// Get Subview With Name
			///
			/// @param Name
			/// @return Subview in hierarchy with given name
			//-----------------------------------------------------
			const GUIViewPtr& GetSubviewWithName(const std::string& instrName) const;
			//-----------------------------------------------------
			/// Get Subviews With Name
			///
			/// @param Name
			/// @param out List of subviews in hierarchy with given name
			//-----------------------------------------------------
			void GetSubviewsWithName(const std::string& instrName, DYNAMIC_ARRAY<GUIViewPtr>& outSubviews) const;
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
			void BringSubviewForward(CGUIView* inpView);
			//-----------------------------------------------------
			/// Send Subview Backward
			///
			/// Move this view a layer away from the screen
			//-----------------------------------------------------
			void SendSubviewBackward(CGUIView* inpView);
			//-----------------------------------------------------
			/// Bring Subview To Front
			///
			/// Move this view to the top of the view hierarchy
			//-----------------------------------------------------
			void BringSubviewToFront(CGUIView* inpView);
			//-----------------------------------------------------
			/// Send Subview To Back
			///
			/// Move this view to the end of the view hierarchy
			//-----------------------------------------------------
			void SendSubviewToBack(CGUIView* inpView);

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
			void SetColour(const Core::CColour& inColour);
			//-----------------------------------------------------
			/// Get Colour
			///
			/// @return The colour applied to the view when drawn
			//-----------------------------------------------------
			const Core::CColour& GetColour() const;
			//-----------------------------------------------------
			/// Get Absolute Colour
			///
			/// @param The inherited colour of the view
			//-----------------------------------------------------
			const Core::CColour& GetAbsoluteColour();
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
			void EnableTouchConsumption(bool inbEnabled, TouchType ineTouch = TOUCH_TYPE_ALL);
			//-----------------------------------------------------
			/// Is Touch Consumption Enabled
			///
			/// @param The type of touch concerned (BEGAN, MOVED or ALL)
			/// @return Override to force a view to consome or route
			/// touches
			//-----------------------------------------------------
			bool IsTouchConsumptionEnabled(TouchType ineTouch = TOUCH_TYPE_ALL) const;
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
			void SetLocalAlignment(Core::AlignmentAnchor ineAlignment);
			//-----------------------------------------------------
			/// Get Local Alignment
			///
			/// Get the pivot point of the view. This is the point
			/// about which all transforms are applied and from 
			/// which any renderable content is drawn
			///
			/// @return Alignment type
			//-----------------------------------------------------
			Core::AlignmentAnchor GetLocalAlignment() const;
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
			void SetAlignmentToParent(Core::AlignmentAnchor ineAlignment);
			//-----------------------------------------------------
			/// Get Alignment with Parent
			///
			/// Get the anchor point about which the child view
			/// aligns to it's parent
			///
			/// @return Alignment type
			//-----------------------------------------------------
			Core::AlignmentAnchor GetAlignmentWithParent() const;
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
			void SetOffsetFromParentAlignment(const UnifiedVector2& invOffset);
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
			const UnifiedVector2& GetOffsetFromParentAlignment() const;
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
			const Core::CVector2& GetAbsoluteOffsetFromParentAlignment() const;
			//-----------------------------------------------------
			/// Set Offset From Alignment
			///
			/// If alignment is disabled then the view will be 
			/// offset by the given co-ords from the anchor point
			///
			/// @param Unified co-ordinates
			//-----------------------------------------------------
			void SetOffsetFromPosition(const UnifiedVector2& invOffset);
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
			const UnifiedVector2& GetOffsetFromPosition() const;
			//-----------------------------------------------------
			/// Get Absolute Offset From Position
			///
			/// If alignment is disabled then the view will be 
			/// offset by the given co-ords from the anchor point
			///
			/// @return Unified co-ordinates converted to real
			/// co-ordinates
			//-----------------------------------------------------
			const Core::CVector2& GetAbsoluteOffsetFromPosition() const;
			//-----------------------------------------------------
			/// Get Absolute Anchor Point
			///
			/// @param Anchor point type
			/// @return Anchor point value as absolute
			//-----------------------------------------------------
			Core::CVector2 GetAbsoluteAnchorPoint(Core::AlignmentAnchor ineAlignment) const;
			//-----------------------------------------------------
			/// Get Absolute Screen Space Anchor Point
			///
			/// @param Anchor point type
			/// @return Anchor point value as absolute
			//-----------------------------------------------------
			Core::CVector2 GetAbsoluteScreenSpaceAnchorPoint(Core::AlignmentAnchor ineAlignment) const;
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
			void SetPosition(const UnifiedVector2& invPosition);
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
			void MoveBy(const UnifiedVector2& invPosition);
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
			void SetSize(const UnifiedVector2& invSize);
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
			void ScaleTo(const Core::CVector2& invScale);
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
			const UnifiedVector2& GetPosition() const;
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
			const UnifiedVector2& GetSize() const;
			//------------------------------------------------------
			/// Get Scale
			///
			/// Get the scale of the object
			///
			/// @return Unified co-ordinates for scale
			//------------------------------------------------------
			const Core::CVector2& GetScale() const;
			//-----------------------------------------------------
			/// Get Absolute Scale
			///
			/// @param The inherited scale of the view
			//-----------------------------------------------------
			const Core::CVector2& GetAbsoluteScale() const;
			//-----------------------------------------------------
			/// Get Absolute Screen Space Position
			///
			/// The position of the view after all parental
			/// transforms have been applied. This is it's final
			/// render position on screen
			///
			/// @return Position combined absolute and relative
			//------------------------------------------------------
			const Core::CVector2& GetAbsoluteScreenSpacePosition() const;
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
			const Core::CVector2& GetAbsolutePosition() const;
			//------------------------------------------------------
			/// Get Absolute Size
			///
			/// Get the real dimensions based by combining absolute 
			/// and relative
			///
			/// @return Size combined absolute and relative
			//------------------------------------------------------
			const Core::CVector2& GetAbsoluteSize() const;

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
			virtual void Draw(Rendering::CCanvasRenderer * inpCanvas);
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
			virtual bool Contains(const Core::CVector2& invPoint) const;
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
			/// On Screen Orientation Changed
			///
			/// Triggered if the screen orientation changes so we can
			/// resize ourself
			//-----------------------------------------------------------
			virtual void OnScreenOrientationChanged();

			//---Touch Delegates
			//-----------------------------------------------------------
			/// On Touch Began
			///
			/// Called when the window receives cursor/touch input
			///
			/// @param Touch data
			/// @return Whether touch has been consumed
			//-----------------------------------------------------------
			virtual bool OnTouchBegan(const Input::TouchInfo & insTouchInfo);
			//-----------------------------------------------------------
			/// On Touch Moved
			///
			/// Called when the window receives cursor/touch input
			///
			/// @param Touch data
			/// @return Whether touch has been consumed
			//-----------------------------------------------------------
			virtual bool OnTouchMoved(const Input::TouchInfo & insTouchInfo);
			//-----------------------------------------------------------
			/// On Touch Ended
			///
			/// Called when the window stops receiving cursor/touch input
			///
			/// @param Touch data
			//-----------------------------------------------------------
			virtual void OnTouchEnded(const Input::TouchInfo & insTouchInfo);
			//-----------------------------------------------------
			/// Set Parent View 
			///
			/// @param The parent the view is attached to
			//-----------------------------------------------------
			void SetParentView(CGUIView* inpParentView);
			//-----------------------------------------------------
			/// Get Input Events
			///
			/// @return The input events of the view
			//-----------------------------------------------------
			CInputEvents* GetInputEvents();
			//-----------------------------------------------------
			/// Set Root Window 
			///
			/// @param The window the view is attached to
			//-----------------------------------------------------
			void SetRootWindow(CWindow* inpWindow);

		protected:

			//-----------------------------------------------------
			/// Get Transform
			///
			/// @return Transformation matrix
			//-----------------------------------------------------
			const Core::CMatrix3x3& GetTransform() const;
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
			virtual void OnSubviewAdded(const GUIViewPtr& inpSubview) { }
			//-----------------------------------------------------
			/// Layout Children's Content
			///
			/// Calls LayoutContent on its subviews. Must be
			/// attached to root window.
			//-----------------------------------------------------
			void LayoutChildrensContent();

		private:


			mutable Core::CMatrix3x3 mmatTransform;

			Core::CColour mAbsoluteColour;
			mutable Core::CVector2 mvAbsolutePosition;
			mutable Core::CVector2 mvAbsoluteScale;
			mutable Core::CVector2 mvAbsoluteSize;
			mutable Core::CVector2 mvAbsoluteParentalOffset;
			mutable Core::CVector2 mvAbsolutePositionOffset;

		protected:

			Subviews mSubviews;
			Subviews mSubviewsCopy;

			CInputEvents mInputEvents;

			CGUIView* mpParentView;
			CWindow* mpRootWindow;

			bool mbIsBeingDragged;
			
			u8 ConsumesTouches;

			//---Properties
			DECLARE_PROPERTY_A(std::string, Name, SetName, GetName);
			DECLARE_PROPERTY_A(UnifiedVector2, UnifiedPosition, SetPosition, GetPosition);
			DECLARE_PROPERTY_A(UnifiedVector2, UnifiedSize, SetSize, GetSize);
			DECLARE_PROPERTY_A(UnifiedVector2, UnifiedParentalOffset, SetOffsetFromParentAlignment, GetOffsetFromParentAlignment);
			DECLARE_PROPERTY_A(UnifiedVector2, UnifiedPositionOffset, SetOffsetFromPosition, GetOffsetFromPosition);
			DECLARE_PROPERTY_A(Core::CVector2, Scale, ScaleTo, GetScale);
			DECLARE_PROPERTY_A(f32, Rotation, RotateTo, GetRotation);
			DECLARE_PROPERTY_A(f32, Opacity, SetOpacity, GetOpacity);
			DECLARE_PROPERTY_A(Core::AlignmentAnchor, LocalAlignment, SetLocalAlignment, GetLocalAlignment);
			DECLARE_PROPERTY_A(Core::AlignmentAnchor, ParentalAlignment, SetAlignmentToParent, GetAlignmentWithParent);
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
			DECLARE_PROPERTY_A(Core::CColour, Colour, SetColour, GetColour);
		};
	}
}

#endif