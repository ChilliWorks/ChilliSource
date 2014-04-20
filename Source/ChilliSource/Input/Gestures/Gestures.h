/*
 * File: Gestures.h
 * Date: 18/10/2010 2010 
 * Description: Holds the recognized gestures for the application 
 */

/*
 * Author: Scott Downie
 * Version: v 1.0
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#ifndef _MO_FLO_INPUT_GESTURES_H_
#define _MO_FLO_INPUT_GESTURES_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Event/Event.h>
#include <ChilliSource/Core/Math/Vector2Old.h>
#include <ChilliSource/Core/Math/Geometry/Shapes.h>
#include <ChilliSource/Core/Time/Timer.h>
#include <ChilliSource/GUI/Base/GUIView.h>

#include <functional>

namespace ChilliSource
{
	namespace Input
	{
		//================================================
		/// Gesture
		///
		/// Base gesture class.
		//================================================
		class Gesture
		{
		public:
			typedef std::function<void(const Gesture*)> GestureEventDelegate;
			
            Gesture(GUI::GUIView* inpView);
            Gesture(PointerSystem* in_pointerSystem);
			virtual ~Gesture(){}
			
			//----------------------------------------------------
            /// @author S Downie
            ///
            /// @return Event that is triggered when the gesture
            /// occurs
			//----------------------------------------------------
            inline Core::IConnectableEvent<GestureEventDelegate>& GetGestureEvent()
            {
                return m_gestureEvent;
            }
			
			//---Touch Delegates
			//----------------------------------------------------
			/// On Touch Delegates
			///
			/// Triggered by the input manager when the device
			/// receives touch notifications
			//----------------------------------------------------
			virtual void OnPointerDown(const PointerSystem::Pointer& in_pointer, f64 in_timestamp, PointerSystem::InputType in_inputType) = 0;
			virtual void OnPointerMoved(const PointerSystem::Pointer& in_pointer, f64 in_timestamp) = 0;
			virtual void OnPointerUp(const PointerSystem::Pointer& in_pointer, f64 in_timestamp, PointerSystem::InputType in_inputType) = 0;
            
            //----------------------------------------------------
            /// Surface Destroyed
            ///
            /// The surface we are listenting too is now gone
            //----------------------------------------------------
            void SurfaceDestroyed();
            
        protected:
            
            //----------------------------------------------------
            /// Notify Gesture Triggered
            ///
            /// Notify listeners that a gesture action has
            /// occurred
            //----------------------------------------------------
            void NotifyGestureTriggered();
			
		protected:
			
			u16 mNumContactPointsRequired;	
			bool mbIsGestureInvalid;
			Core::Timer mTimer;
            GUI::GUIView* mpView;
            PointerSystem* m_pointerSystem;
            
            Core::ConnectionUPtr m_pointerDownConnection;
            Core::ConnectionUPtr m_pointerMovedConnection;
            Core::ConnectionUPtr m_pointerUpConnection;
            
        private:
            
            Core::Event<GestureEventDelegate> m_gestureEvent;
		};
		
		//================================================
		/// Swipe Gesture
		///
		/// A swift movement with one or more contact
		/// points over a large range within a short time
		//================================================
		class SwipeGesture : public Gesture
		{
		public:
            SwipeGesture(GUI::GUIView* inpSurface);
            SwipeGesture(PointerSystem* in_pointerSystem);
			
			//----------------------------------------------------
			/// Set Number of Contact Points Required
			///
			/// The number of contacts before the gesture is recognized.
			/// @param Number of points required
			//----------------------------------------------------
			void SetNumContactPointsRequired(const u32 inNumContacts);
			
			//----------------------------------------------------
			/// SetMinimumDistance
			///
			/// Set the minimum distance in pixels touches must move to trigger the gesture
			/// @param Minimum Distance
			//----------------------------------------------------
			void SetMinimumDistance(u32 inudwMinDistance);
			
			//----------------------------------------------------
			/// Set Maximum Swipe Duration
			///
			/// The maximum duration during which the swipe will be recognised.
			/// @param Max duration of the swipe gesture
			//----------------------------------------------------
			void SetMaximumSwipeDuration(const f32 infMaximumSwipeDuration);
            
            //----------------------------------------------------
			/// Get Swipe start position
			///
			/// Return start swipe position
			//----------------------------------------------------
            ChilliSource::Core::Vector2Old GetStartPosition() const { return mvStartPos; }
            
            //----------------------------------------------------
			/// Get Swipe end position
			///
			/// Return end swipe position
			//----------------------------------------------------
            ChilliSource::Core::Vector2Old GetEndPosition() const { return mvEndPos; }
            
		private:
			
			//---Touch Delegates
			//----------------------------------------------------
			/// On Touch Delegates
			///
			/// Triggered by the input manager when the device
			/// receives touch notifications
			//----------------------------------------------------
			void OnPointerDown(const PointerSystem::Pointer& in_pointer, f64 in_timestamp, PointerSystem::InputType in_inputType) override;
			void OnPointerMoved(const PointerSystem::Pointer& in_pointer, f64 in_timestamp) override {}
			void OnPointerUp(const PointerSystem::Pointer& in_pointer, f64 in_timestamp, PointerSystem::InputType in_inputType) override;
			
		public:
			
			Core::Vector2Old mvVelocity;
			
		private:
			
			u32 mMinDistanceRequiredSqrd;
			u16 mNumContactPointsRequired;
			u16 mCurrentNumContactPoints;
			u16 mMaxNumContactPoints;
			
			f32 mfMaximumSwipeDuration;
			
			Core::Vector2Old mvStartPos;
			Core::Vector2Old mvEndPos;
		};		
		//================================================
		/// Pinch Gesture (Continuous)
		///
		/// A movement in which the distance between
		/// two contact points decreases
		//================================================
		class PinchGesture : public Gesture
		{
		public:
            PinchGesture(GUI::GUIView* inpSurface);
            PinchGesture(PointerSystem* in_pointerSystem);
			
            //----------------------------------------------------
            /// @author S Downie
            ///
            /// @return Event that is triggered when the gesture
            /// begins
			//----------------------------------------------------
            inline Core::IConnectableEvent<GestureEventDelegate>& GetGestureBeganEvent()
            {
                return m_gestureBeganEvent;
            }
            //----------------------------------------------------
            /// @author S Downie
            ///
            /// @return Event that is triggered when the gesture
            /// ends
			//----------------------------------------------------
            inline Core::IConnectableEvent<GestureEventDelegate>& GetGestureEndedEvent()
            {
                return m_gestureEndedEvent;
            }
			//----------------------------------------------------
			/// Get Start Displacement
			///
			/// @return Start Displacement
			//----------------------------------------------------
			const f32 GetStartDisplacement() const;
			//----------------------------------------------------
			/// Get Current Displacement
			///
			/// @return Current Displacement
			//----------------------------------------------------
			const f32 GetCurrentDisplacement() const;
            //----------------------------------------------------
			/// Populate Start Positions For First Touch
			//----------------------------------------------------
			void PopulateStartPositions(ChilliSource::Core::Vector2Old& outvFirstPosition, ChilliSource::Core::Vector2Old& outvSecondPosition) const;
            //----------------------------------------------------
			/// Get Current Position
			//----------------------------------------------------
			void PopulateCurrentPositions(ChilliSource::Core::Vector2Old& outvFirstPosition, ChilliSource::Core::Vector2Old& outvSecondPosition) const;
            
			
		private:
			
			//---Touch Delegates
			//----------------------------------------------------
			/// On Touch Delegates
			///
			/// Triggered by the input manager when the device
			/// receives touch notifications
			//----------------------------------------------------
			void OnPointerDown(const PointerSystem::Pointer& in_pointer, f64 in_timestamp, PointerSystem::InputType in_inputType) override;
			void OnPointerMoved(const PointerSystem::Pointer& in_pointer, f64 in_timestamp) override;
			void OnPointerUp(const PointerSystem::Pointer& in_pointer, f64 in_timestamp, PointerSystem::InputType in_inputType) override;
			
		public:
			f32				mfRatio;				// The ratio of mfCurrentDisplacement / mfStartDisplacement
			f32				mfDAngle;
			
		private:
			
			u32 mMinDistanceRequiredSqrd;
			u32 mCurrentTouches;
			
			PointerSystem::PointerId mTouchID1;
			PointerSystem::PointerId mTouchID2;
			
			
			bool mbFirstTouchBegan, mbSecondTouchBegan;
			
			Core::Vector2Old	mvStartPos1;			// Touch positions at start of gesture
			Core::Vector2Old	mvStartPos2;			
			f32				mfStartDisplacement;	// Cached sqrt(|mvStartPos2 - mvStartPos1|)
			f32				mfStartAngle;
			Core::Vector2Old	mvCurrentPos1;			// Touch positions of end of gesture
			Core::Vector2Old	mvCurrentPos2;
			f32				mfCurrentDisplacement;	// Cached sqrt(|mvCurrentPos2 - mvCurrentPos1|)
			f32				mfCurrentAngle;
			
            Core::Event<GestureEventDelegate> m_gestureBeganEvent;
            Core::Event<GestureEventDelegate> m_gestureEndedEvent;
		};
		//================================================
		/// Tap Gesture
		///
		/// One or more contact points registered for
		/// a short period of time. Handles multiple taps
		//================================================
		class TapGesture : public Gesture
		{
		public:
			TapGesture(GUI::GUIView* inpSurface);
			TapGesture(PointerSystem* in_pointerSystem);
			
			//----------------------------------------------------
			/// Set Number of Taps Required
			///
			/// The number of taps before the gesture is recognized.
			/// Use this for double tapping etc
			/// @param Number of taps required
			//----------------------------------------------------
			void SetNumTapsRequired(const u32 inNumTaps);
            //----------------------------------------------------
			/// Set Maximum Tap Duration
			///
			/// The time the tap can be held before it is not
            /// classes as a tap anymore
			/// @param Duration
			//----------------------------------------------------
            void SetMaximumTapDuration(const f32 infMaximumTapDuration);
            //----------------------------------------------------
			/// Set Maximum Distance Allowed Squared
			///
			/// The maximum squared distance from the original
            /// touch that will be accepted as a tap
			/// @param Distance Squared
			//----------------------------------------------------
            void SetMaxDistanceAllowedSqrd(const u32 inudwMaxDistanceAllowedSqrd);
		public:
			
			Core::Vector2Old mvLocation;
			
		private:
			
			//---Touch Delegates
			//----------------------------------------------------
			/// On Touch Delegates
			///
			/// Triggered by the input manager when the device
			/// receives touch notifications
			//----------------------------------------------------
			void OnPointerDown(const PointerSystem::Pointer& in_pointer, f64 in_timestamp, PointerSystem::InputType in_inputType) override;
			void OnPointerMoved(const PointerSystem::Pointer& in_pointer, f64 in_timestamp) override {};
			void OnPointerUp(const PointerSystem::Pointer& in_pointer, f64 in_timestamp, PointerSystem::InputType in_inputType) override;
			
			//----------------------------------------------------
			/// Check For Tap
			/// @return Whether a single tap has been recognized
			//----------------------------------------------------
			bool CheckForTap();
			//----------------------------------------------------------------------
			/// Check For Multi-Tap
			/// @return Whether a the most recent tap was part of the last sequence
			//----------------------------------------------------------------------
			bool CheckForMultiTap();
			
		private:
			
			u16 mNumTapsRequired;
			u16 mCurrentNumTaps;
			
			u32 mudwMaxDistAllowedSqrd;
			
			f32 mfMaximumTapDuration;
			f32 mfMaxTimeBetweenTaps;
			f32 mfLastTapTime;
			f32 mfLastBeganTime;
			
			Core::Vector2Old mvStartPos;
		};
		//================================================
		/// Drag Gesture (Continuous)
		///
		/// One or more contact points moving perceptibly
		/// over any range
		//================================================
		class DragGesture : public Gesture
		{
		public:
			DragGesture(GUI::GUIView* inpSurface);
            DragGesture(PointerSystem* in_pointerSystem);
			
            //----------------------------------------------------
            /// @author S Downie
            ///
            /// @return Event that is triggered when the gesture
            /// begins
			//----------------------------------------------------
            inline Core::IConnectableEvent<GestureEventDelegate>& GetGestureBeganEvent()
            {
                return m_gestureBeganEvent;
            }
            //----------------------------------------------------
            /// @author S Downie
            ///
            /// @return Event that is triggered when the gesture
            /// ends
			//----------------------------------------------------
            inline Core::IConnectableEvent<GestureEventDelegate>& GetGestureEndedEvent()
            {
                return m_gestureEndedEvent;
            }
            //----------------------------------------------------
			/// Get Swipe start position
			///
			/// Return start swipe position
			//----------------------------------------------------
            ChilliSource::Core::Vector2Old GetStartPosition() const { return mvStartPos; }
            //----------------------------------------------------
			/// Get Touch ID
			///
			/// Return the touch ID
			//----------------------------------------------------
            PointerSystem::PointerId GetTouchID() const { return mCurrentID; }
            
			void SetMinDistanceRequiredSqrd(f32 fMinDistanceRequiredSqrd) { mMinDistanceRequiredSqrd = fMinDistanceRequiredSqrd; }
			void SetInitialHoldDuration(f32 fInitialHoldDuration) { mfInitialHoldDuration = fInitialHoldDuration; }
			
		private:
			
			//---Touch Delegates
			//----------------------------------------------------
			/// On Touch Delegates
			///
			/// Triggered by the input manager when the device
			/// receives touch notifications
			//----------------------------------------------------
			void OnPointerDown(const PointerSystem::Pointer& in_pointer, f64 in_timestamp, PointerSystem::InputType in_inputType) override;
			void OnPointerMoved(const PointerSystem::Pointer& in_pointer, f64 in_timestamp) override;
			void OnPointerUp(const PointerSystem::Pointer& in_pointer, f64 in_timestamp, PointerSystem::InputType in_inputType) override;
			
		public:
			
			Core::Vector2Old mvLocation;
            Core::Vector2Old mvPreviousLocation;
			
		private:
			
			Core::Vector2Old mvStartPos;
			
			f32 mMinDistanceRequiredSqrd;
			PointerSystem::PointerId mCurrentID;
            u32 mCurrentTouches;
			
			bool mbFirstRun;
			bool mbIsGestureActive;		// true if the gesture is valid - if it becomes invalid it can never validate again, until we go back to a first touch
			f32 mfInitialHoldDuration;
			
            Core::Event<GestureEventDelegate> m_gestureBeganEvent;
            Core::Event<GestureEventDelegate> m_gestureEndedEvent;
		};
		//================================================
		/// Hold Gesture
		///
		/// One contact point registered for a short 
		/// period of time.
		//================================================
		class HoldGesture : public Gesture
		{
		public:
			HoldGesture(GUI::GUIView* inpSurface);
            HoldGesture(PointerSystem* in_pointerSystem);
			
            //----------------------------------------------------
            /// @author S Downie
            ///
            /// @return Event that is triggered when the gesture
            /// begins
			//----------------------------------------------------
            inline Core::IConnectableEvent<GestureEventDelegate>& GetGestureBeganEvent()
            {
                return m_gestureBeganEvent;
            }
            //----------------------------------------------------
            /// @author S Downie
            ///
            /// @return Event that is triggered when the gesture
            /// ends
			//----------------------------------------------------
            inline Core::IConnectableEvent<GestureEventDelegate>& GetGestureEndedEvent()
            {
                return m_gestureEndedEvent;
            }
            //----------------------------------------------------
            /// @author S Downie
            ///
            /// @return Event that is triggered when the gesture
            /// is cancelled
			//----------------------------------------------------
            inline Core::IConnectableEvent<GestureEventDelegate>& GetGestureCancelledEvent()
            {
                return m_gestureCancelledEvent;
            }
			
			void OnGestureUpdate();
			
			void SetMaxDistanceAllowedSqrd(f32 fMaxDistanceAllowedSqrd) { mfMaxDistanceAllowedSqrd = fMaxDistanceAllowedSqrd; }
			void SetHoldDuration(f32 fHoldDuration) { mfHoldDuration = fHoldDuration; }
            
            //----------------------------------------------------
			/// ResetGesture
			///
			/// Resets gesture state
			//----------------------------------------------------
            void ResetGesture();
            
            //----------------------------------------------------
			/// SetInitialHoldTime
			///
			/// Set the time required before the OnHoldBegin is recognised ( Helps avoid conflict with onTap gesture )
			/// @param Event listener
			//----------------------------------------------------
            void SetInitialHoldTime(f32 infHoldTime) { mfInitialHoldTime = infHoldTime; }
			
		public:
			
			Core::Vector2Old mvLocation;
			
		private:
			
			//---Touch Delegates
			//----------------------------------------------------
			/// On Touch Delegates
			///
			/// Triggered by the input manager when the device
			/// receives touch notifications
			//----------------------------------------------------
			void OnPointerDown(const PointerSystem::Pointer& in_pointer, f64 in_timestamp, PointerSystem::InputType in_inputType) override;
			void OnPointerMoved(const PointerSystem::Pointer& in_pointer, f64 in_timestamp) override;
			void OnPointerUp(const PointerSystem::Pointer& in_pointer, f64 in_timestamp, PointerSystem::InputType in_inputType) override;
			
			void CancelGesture();
			void EndGesture();
            
		private:
			
			//Core::Vector2Old mvStartPos;
			
			f32 mfMaxDistanceAllowedSqrd;
			f32 mfHoldDuration;
			f32 mfInitialHoldTime;
			u32 mudwNumberOfTouch;
			bool mbIsGestureActive;		// true if the gesture is valid - if it becomes invalid it can never validate again, until we go back to a first touch
			bool mbIsGestureStarted;    // true if the gesture has begun, i.e. onTouchBegan was called. This is used instead of in the touch moved to cancel the gesture
            
            Core::Event<GestureEventDelegate> m_gestureBeganEvent;
            Core::Event<GestureEventDelegate> m_gestureEndedEvent;
            Core::Event<GestureEventDelegate> m_gestureCancelledEvent;
            
            Core::ConnectionUPtr m_periodicTimerConnection;
		};
	}
}


#endif
