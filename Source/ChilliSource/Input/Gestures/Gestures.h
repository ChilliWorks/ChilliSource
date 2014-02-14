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

#include <ChilliSource/Core/Math/Vector2.h>
#include <ChilliSource/Core/Time/Timer.h>
#include <ChilliSource/Core/ForwardDeclarations.h>
#include <ChilliSource/Core/Base/FastDelegate.h>
#include <ChilliSource/Core/Base/Surface.h>
#include <ChilliSource/Core/Math/Geometry/Shapes.h>

#include <ChilliSource/Input/ForwardDeclarations.h>

namespace moFlo
{
	namespace Input
	{
		//================================================
		/// Gesture
		///
		/// Base gesture class.
		//================================================
		class CGesture
		{
		public:
			typedef fastdelegate::FastDelegate1<const CGesture&> GestureEventDelegate;
			
            CGesture(ISurface* inpSurface);
            CGesture(ITouchScreen* inpTouchDevice);
			virtual ~CGesture();
			
			//----------------------------------------------------
			/// Register Gesture Delegate
			///
			/// Triggered when the delegate of object type is
			/// recognized.
			/// @param Event listener
			//----------------------------------------------------
			void RegisterGestureDelegate(GestureEventDelegate inEventDelegate);
			//----------------------------------------------------
			/// Deregister Gesture Delegate
			///
			/// Usubscribe the listener for event notifications
			/// @param Event listener
			//----------------------------------------------------
			void DeregisterGestureDelegate(GestureEventDelegate inEventDelegate);
			
			//---Touch Delegates
			//----------------------------------------------------
			/// On Touch Delegates
			///
			/// Triggered by the input manager when the device
			/// receives touch notifications
			//----------------------------------------------------
			virtual void OnTouchBegan(const TouchInfo &Info) = 0;
			virtual void OnTouchMoved(const TouchInfo &Info) = 0;
			virtual void OnTouchEnded(const TouchInfo &Info) = 0;
            
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
			
			typedef DYNAMIC_ARRAY<GestureEventDelegate> GestureDelegatesList;
			typedef GestureDelegatesList::iterator GestureDelegatesListItr;
			
			u16 mNumContactPointsRequired;	
			bool mbIsGestureInvalid;
			Core::CTimer mTimer;
			ISurface* mpSurface;
            ITouchScreen* mpTouchDevice;
            
        private:
            
            GestureDelegatesList mGestureDelegates;
		};
		
		//================================================
		/// Swipe Gesture
		///
		/// A swift movement with one or more contact
		/// points over a large range within a short time
		//================================================
		class CSwipeGesture : public CGesture
		{
		public:
            CSwipeGesture(ISurface* inpSurface);
            CSwipeGesture(ITouchScreen* inpTouchDevice);
			
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
            moFlo::Core::CVector2 GetStartPosition() const { return mvStartPos; }
            
            //----------------------------------------------------
			/// Get Swipe end position
			///
			/// Return end swipe position
			//----------------------------------------------------
            moFlo::Core::CVector2 GetEndPosition() const { return mvEndPos; }
            
		private:
			
			//---Touch Delegates
			//----------------------------------------------------
			/// On Touch Delegates
			///
			/// Triggered by the input manager when the device
			/// receives touch notifications
			//----------------------------------------------------
			void OnTouchBegan(const TouchInfo &Info);
			void OnTouchMoved(const TouchInfo &Info){}
			void OnTouchEnded(const TouchInfo &Info);
			
		public:
			
			Core::CVector2 mvVelocity;
			
		private:
			
			u32 mMinDistanceRequiredSqrd;
			u16 mNumContactPointsRequired;
			u16 mCurrentNumContactPoints;
			u16 mMaxNumContactPoints;
			
			f32 mfMaximumSwipeDuration;
			
			Core::CVector2 mvStartPos;
			Core::CVector2 mvEndPos;
		};		
		//================================================
		/// Pinch Gesture (Continuous)
		///
		/// A movement in which the distance between
		/// two contact points decreases
		//================================================
		class CPinchGesture : public CGesture
		{
		public:
            CPinchGesture(ISurface* inpSurface);
            CPinchGesture(ITouchScreen* inpTouchDevice);
			
			//----------------------------------------------------
			/// Register Gesture Delegate
			///
			/// Triggered when the delegate of object type is
			/// recognized.
			/// @param Event listener
			//----------------------------------------------------
			void RegisterGestureBeganDelegate(GestureEventDelegate inEventDelegate);
			//----------------------------------------------------
			/// Deregister Gesture Delegate
			///
			/// Usubscribe the listener for event notifications
			/// @param Event listener
			//----------------------------------------------------
			void DeregisterGestureBeganDelegate(GestureEventDelegate inEventDelegate);
			//----------------------------------------------------
			/// Register Gesture Delegate
			///
			/// Triggered when the delegate of object type is
			/// recognized.
			/// @param Event listener
			//----------------------------------------------------
			void RegisterGestureEndedDelegate(GestureEventDelegate inEventDelegate);
			//----------------------------------------------------
			/// Deregister Gesture Delegate
			///
			/// Usubscribe the listener for event notifications
			/// @param Event listener
			//----------------------------------------------------
			void DeregisterGestureEndedDelegate(GestureEventDelegate inEventDelegate);
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
			void PopulateStartPositions(moFlo::Core::CVector2& outvFirstPosition, moFlo::Core::CVector2& outvSecondPosition) const;
            //----------------------------------------------------
			/// Get Current Position
			//----------------------------------------------------
			void PopulateCurrentPositions(moFlo::Core::CVector2& outvFirstPosition, moFlo::Core::CVector2& outvSecondPosition) const;
            
			
		private:
			
			//---Touch Delegates
			//----------------------------------------------------
			/// On Touch Delegates
			///
			/// Triggered by the input manager when the device
			/// receives touch notifications
			//----------------------------------------------------
			void OnTouchBegan(const TouchInfo &Info);
			void OnTouchMoved(const TouchInfo &Info);
			void OnTouchEnded(const TouchInfo &Info);
			
		public:
			f32				mfRatio;				// The ratio of mfCurrentDisplacement / mfStartDisplacement
			f32				mfDAngle;
			
		private:
			
			u32 mMinDistanceRequiredSqrd;
			u32 mCurrentTouches;
			
			u32 mTouchID1;
			u32 mTouchID2;
			
			
			bool mbFirstTouchBegan, mbSecondTouchBegan;
			
			Core::CVector2	mvStartPos1;			// Touch positions at start of gesture
			Core::CVector2	mvStartPos2;			
			f32				mfStartDisplacement;	// Cached sqrt(|mvStartPos2 - mvStartPos1|)
			f32				mfStartAngle;
			Core::CVector2	mvCurrentPos1;			// Touch positions of end of gesture
			Core::CVector2	mvCurrentPos2;
			f32				mfCurrentDisplacement;	// Cached sqrt(|mvCurrentPos2 - mvCurrentPos1|)
			f32				mfCurrentAngle;
			
			GestureDelegatesList mGestureBeganDelegates;
			GestureDelegatesList mGestureEndedDelegates;
		};
		//================================================
		/// Tap Gesture
		///
		/// One or more contact points registered for
		/// a short period of time. Handles multiple taps
		//================================================
		class CTapGesture : public CGesture
		{
		public:
            CTapGesture(ISurface* inpSurface);
            CTapGesture(ITouchScreen* inpTouchDevice);
			
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
			
			Core::CVector2 mvLocation;
			
		private:
			
			//---Touch Delegates
			//----------------------------------------------------
			/// On Touch Delegates
			///
			/// Triggered by the input manager when the device
			/// receives touch notifications
			//----------------------------------------------------
			void OnTouchBegan(const TouchInfo &Info);
			void OnTouchMoved(const TouchInfo &Info){}
			void OnTouchEnded(const TouchInfo &Info);
			
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
			
			Core::CVector2 mvStartPos;
		};
		//================================================
		/// Drag Gesture (Continuous)
		///
		/// One or more contact points moving perceptibly
		/// over any range
		//================================================
		class CDragGesture : public CGesture
		{
		public:
			CDragGesture(ISurface* inpSurface);
            CDragGesture(ITouchScreen* inpTouchDevice);
			
			//----------------------------------------------------
			/// RegisterGestureBeganDelegate
			///
			/// Triggered when the delegate of object type is
			/// begun.
			/// @param Event listener
			//----------------------------------------------------
			void RegisterGestureBeganDelegate(GestureEventDelegate inEventDelegate);			
			//----------------------------------------------------
			/// DeregisterGestureBeganDelegate
			///
			/// Usubscribe the listener for event notifications
			/// @param Event listener
			//----------------------------------------------------
			void DeregisterGestureBeganDelegate(GestureEventDelegate inEventDelegate);
			//----------------------------------------------------
			/// Register Gesture Delegate
			///
			/// Triggered when the delegate of object type is
			/// recognized.
			/// @param Event listener
			//----------------------------------------------------
			void RegisterGestureEndedDelegate(GestureEventDelegate inEventDelegate);
			//----------------------------------------------------
			/// Deregister Gesture Delegate
			///
			/// Usubscribe the listener for event notifications
			/// @param Event listener
			//----------------------------------------------------
			void DeregisterGestureEndedDelegate(GestureEventDelegate inEventDelegate);
            //----------------------------------------------------
			/// Get Swipe start position
			///
			/// Return start swipe position
			//----------------------------------------------------
            moFlo::Core::CVector2 GetStartPosition() const { return mvStartPos; }
            //----------------------------------------------------
			/// Get Touch ID
			///
			/// Return the touch ID
			//----------------------------------------------------
            u32 GetTouchID() const { return mCurrentID; }
            
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
			void OnTouchBegan(const TouchInfo &Info);
			void OnTouchMoved(const TouchInfo &Info);
			void OnTouchEnded(const TouchInfo &Info);
			
		public:
			
			Core::CVector2 mvLocation;
            Core::CVector2 mvPreviousLocation;
			
		private:
			
			Core::CVector2 mvStartPos;
			
			f32 mMinDistanceRequiredSqrd;
            u32 mCurrentID;
            u32 mCurrentTouches;
			
			bool mbFirstRun;
			bool mbIsGestureActive;		// true if the gesture is valid - if it becomes invalid it can never validate again, until we go back to a first touch
			f32 mfInitialHoldDuration;
			
			GestureDelegatesList mGestureBeganDelegates;
			GestureDelegatesList mGestureEndedDelegates;
		};
		//================================================
		/// Hold Gesture
		///
		/// One contact point registered for a short 
		/// period of time.
		//================================================
		class CHoldGesture : public CGesture
		{
		public:
			CHoldGesture(ISurface* inpSurface);
            CHoldGesture(ITouchScreen* inpTouchDevice);
			
			//----------------------------------------------------
			/// Register Gesture Delegate
			///
			/// Triggered when the delegate of object type is
			/// recognized.
			/// @param Event listener
			//----------------------------------------------------
			void RegisterGestureBeganDelegate(GestureEventDelegate inEventDelegate);
			//----------------------------------------------------
			/// Deregister Gesture Delegate
			///
			/// Usubscribe the listener for event notifications
			/// @param Event listener
			//----------------------------------------------------
			void DeregisterGestureBeganDelegate(GestureEventDelegate inEventDelegate);
			//----------------------------------------------------
			/// Register Gesture Delegate
			///
			/// Triggered when the delegate of object type is
			/// recognized.
			/// @param Event listener
			//----------------------------------------------------
			void RegisterGestureEndedDelegate(GestureEventDelegate inEventDelegate);
			//----------------------------------------------------
			/// Deregister Gesture Delegate
			///
			/// Usubscribe the listener for event notifications
			/// @param Event listener
			//----------------------------------------------------
			void DeregisterGestureEndedDelegate(GestureEventDelegate inEventDelegate);
            
            //----------------------------------------------------
			/// Register Gesture Delegate
			///
			/// Triggered when the Hold gesture is aborted before completion is
			/// recognized.
			/// @param Event listener
			//----------------------------------------------------
			void RegisterGestureCancelledDelegate(GestureEventDelegate inEventDelegate);
			//----------------------------------------------------
			/// Deregister Gesture Delegate
			///
			/// Usubscribe the listener for event notifications
			/// @param Event listener
			//----------------------------------------------------
			void DeregisterGestureCancelledDelegate(GestureEventDelegate inEventDelegate);
            
			
			void OnGestureUpdate(f32 infDT);
			
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
			
			Core::CVector2 mvLocation;
			
		private:
			
			//---Touch Delegates
			//----------------------------------------------------
			/// On Touch Delegates
			///
			/// Triggered by the input manager when the device
			/// receives touch notifications
			//----------------------------------------------------
			void OnTouchBegan(const TouchInfo &Info);
			void OnTouchMoved(const TouchInfo &Info);
			void OnTouchEnded(const TouchInfo &Info);
			
			void CancelGesture();
			void EndGesture();
            
		private:
			
			//Core::CVector2 mvStartPos;
			
			f32 mfMaxDistanceAllowedSqrd;
			f32 mfHoldDuration;
			f32 mfInitialHoldTime;
			u32 mudwNumberOfTouch;
			bool mbIsGestureActive;		// true if the gesture is valid - if it becomes invalid it can never validate again, until we go back to a first touch
			bool mbIsGestureStarted;    // true if the gesture has begun, i.e. onTouchBegan was called. This is used instead of in the touch moved to cancel the gesture 
			GestureDelegatesList mGestureBeganDelegates;
			GestureDelegatesList mGestureEndedDelegates;
            GestureDelegatesList mGestureCancelledDelegates;
		};
		
		typedef SHARED_PTR<CPinchGesture> PinchGesturePtr;
		typedef SHARED_PTR<CSwipeGesture> SwipeGesturePtr;
		typedef SHARED_PTR<CTapGesture> TapGesturePtr;
		typedef SHARED_PTR<CDragGesture> DragGesturePtr;
		typedef SHARED_PTR<CHoldGesture> HoldGesturePtr;
		typedef SHARED_PTR<CGesture> GesturePtr;

	}
}


#endif
