/*
 * File: Gestures.cpp
 * Date: 18/10/2010 2010 
 * Description: 
 */

/*
 * Author: Scott Downie
 * Version: v 1.0
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#include <ChilliSource/Input/Gestures/Gestures.h>
#include <ChilliSource/Input/Base/InputSystem.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/MakeDelegate.h>

namespace ChilliSource
{
	namespace Input
	{
		//=========================================================
		/// Gesture - Base
		//=========================================================
		//----------------------------------------------------
		/// Constructor
		///
		/// @param Touch screen device
		/// @param Active gesture bounds in screen space
		//----------------------------------------------------
		Gesture::Gesture(TouchScreen* inpTouchDevice) : mbIsGestureInvalid(false), mpView(nullptr), mpTouchDevice(inpTouchDevice)
		{
			//Register for touch notifications
			m_touchBeganConnection = inpTouchDevice->GetTouchBeganEvent().OpenConnection(Core::MakeDelegate(this, &Gesture::OnTouchBegan));
			m_touchMoveConnection = inpTouchDevice->GetTouchMovedEvent().OpenConnection(Core::MakeDelegate(this, &Gesture::OnTouchMoved));
			m_touchEndConnection = inpTouchDevice->GetTouchEndEvent().OpenConnection(Core::MakeDelegate(this, &Gesture::OnTouchEnded));
		}
        //----------------------------------------------------
		/// Constructor
		///
		/// @param Surface
		/// @param Active gesture bounds in screen space
		//----------------------------------------------------
		Gesture::Gesture(GUI::GUIView* inpView) : mbIsGestureInvalid(false), mpView(inpView), mpTouchDevice(nullptr)
		{
			//Register for touch notifications
			m_touchBeganConnection = mpView->GetTouchBeganEvent().OpenConnection(Core::MakeDelegate(this, &Gesture::OnTouchBegan));
			m_touchMoveConnection = mpView->GetTouchMovedEvent().OpenConnection(Core::MakeDelegate(this, &Gesture::OnTouchMoved));
			m_touchEndConnection = mpView->GetTouchEndEvent().OpenConnection(Core::MakeDelegate(this, &Gesture::OnTouchEnded));
		}
        //----------------------------------------------------
		/// Surface Destroyed
		///
        /// The surface we are listenting too is now gone
		//----------------------------------------------------
		void Gesture::SurfaceDestroyed()
        {
            mpView = nullptr;
            mpTouchDevice = nullptr;
        }
        //----------------------------------------------------
		/// Notify Gesture Triggered
		///
        /// Notify listeners that a gesture action has
        /// occurred
		//----------------------------------------------------
        void Gesture::NotifyGestureTriggered()
        {
            m_gestureEvent.NotifyConnections(this);
        }
		//================================================
		/// Swipe Gesture
		///
		/// A swift movement with one or more contact
		/// points over a large range within a short time
		//================================================
		//----------------------------------------------------
		/// Constructor
		///
		/// @param Surface
		/// @param Active gesture bounds in screen space
		//----------------------------------------------------
		SwipeGesture::SwipeGesture(GUI::GUIView* inpView)
		: Gesture(inpView), mMinDistanceRequiredSqrd(4000), mNumContactPointsRequired(1), mCurrentNumContactPoints(0), mMaxNumContactPoints(0), mfMaximumSwipeDuration(0.5f)
		{
		}
        //----------------------------------------------------
		/// Constructor
		///
		/// @param Touch screen device
		/// @param Active gesture bounds in screen space
		//----------------------------------------------------
		SwipeGesture::SwipeGesture(TouchScreen* inpTouchDevice)
		: Gesture(inpTouchDevice), mMinDistanceRequiredSqrd(4000), mNumContactPointsRequired(1), mCurrentNumContactPoints(0), mMaxNumContactPoints(0), mfMaximumSwipeDuration(0.5f)
		{
		}
		//----------------------------------------------------
		/// Set Number of Contact Points Required
		///
		/// The number of contacts before the gesture is recognized.
		/// @param Number of points required
		//----------------------------------------------------
		void SwipeGesture::SetNumContactPointsRequired(const u32 inNumContacts)
		{
			mNumContactPointsRequired = inNumContacts;
		}
		//----------------------------------------------------
		/// Set Maximum Swipe Duration
		///
		/// The maximum duration during which the swipe will be recognised.
		/// @param Max duration of the swipe gesture
		//----------------------------------------------------
		void SwipeGesture::SetMaximumSwipeDuration(const f32 infMaximumSwipeDuration)
		{
			mfMaximumSwipeDuration = infMaximumSwipeDuration;
		}
		//----------------------------------------------------
		/// SetMinimumDistance
		///
		/// Set the minimum distance touches must move to trigger the gesture
		/// @param Minimum Distance
		//----------------------------------------------------
		void SwipeGesture::SetMinimumDistance(u32 inudwMinDistance)
		{
			mMinDistanceRequiredSqrd = inudwMinDistance * inudwMinDistance;
		}
		//---Touch Delegates
		//----------------------------------------------------
		/// On Touch Delegates
		///
		/// Triggered by the input manager when the device
		/// receives touch notifications
		//----------------------------------------------------
		void SwipeGesture::OnTouchBegan(const TouchInfo &Info)
		{
			if(++mCurrentNumContactPoints == 1)
			{
				//Clean the pipes
				mTimer.Reset();
				//Start the timer. If we let go within the max duration and have covered the given distance we have swiped. Easy!
				mTimer.Start();
				
                mMaxNumContactPoints = 0;
                
				mbIsGestureInvalid = false;
			}
			mMaxNumContactPoints++;
			
			//Get it's initial position so we can work out it's velocity later
			mvStartPos = Info.vLocation;
		}
		void SwipeGesture::OnTouchEnded(const TouchInfo &Info)
		{
			if (mCurrentNumContactPoints > 0)
				mCurrentNumContactPoints--;
			
			//Don't process subsequent touches in the sequence if the gesture is invalid
			if(mbIsGestureInvalid)
			{
				mMaxNumContactPoints = 0;
				return;
			}
			
			//Contact has ended without the correct number of contact
			//points being applied. This gesture is not a swipe!
			if(mMaxNumContactPoints != mNumContactPointsRequired)
			{
				mbIsGestureInvalid = true;
				mMaxNumContactPoints = 0;
				return;
			}
			
			//Calculate the velocity based on the released position
			mvEndPos = Info.vLocation;
			//Average all the contact point velocities and us that to determine if we have swiped
			mvVelocity = mvEndPos - mvStartPos;
			
			if(mCurrentNumContactPoints == 0 && mTimer.GetTimeElapsed() < mfMaximumSwipeDuration && mvVelocity.LengthSquared() > mMinDistanceRequiredSqrd)
			{
				//Fire off the delegates
                NotifyGestureTriggered();
				
				mMaxNumContactPoints = 0;
			}
		}
        
		//================================================
		/// Pinch Gesture
		///
		/// A movement in which the distance between
		/// two contact points decreases or increases
		//================================================
		//----------------------------------------------------
		/// Constructor
		///
		/// @param Surface
		/// @param Active gesture bounds in screen space
		//----------------------------------------------------
		PinchGesture::PinchGesture(GUI::GUIView* inpView)
		: Gesture(inpView), mMinDistanceRequiredSqrd(6000), mfStartDisplacement(0.0f), mfCurrentDisplacement(0.0f), mbFirstTouchBegan(false), mbSecondTouchBegan(false), mCurrentTouches(0), mTouchID1(0), mfRatio(0.0f),
		mTouchID2(0)
		{
		}
        //----------------------------------------------------
		/// Constructor
		///
		/// @param Touch screen device
		/// @param Active gesture bounds in screen space
		//----------------------------------------------------
		PinchGesture::PinchGesture(TouchScreen* inpTouchDevice) 
		: Gesture(inpTouchDevice), mMinDistanceRequiredSqrd(6000), mfStartDisplacement(0.0f), mfCurrentDisplacement(0.0f), mbFirstTouchBegan(false), mbSecondTouchBegan(false), mCurrentTouches(0), mTouchID1(0), mfRatio(0.0f),
		mTouchID2(0)
		{
		}
		//----------------------------------------------------
		/// Get Start Displacement
		///
		/// @return Start Displacement
		//----------------------------------------------------
		const f32 PinchGesture::GetStartDisplacement() const
		{
			return mfStartDisplacement;
		}
		//----------------------------------------------------
		/// Get Current Displacement
		///
		/// @return Current Displacement
		//----------------------------------------------------
		const f32 PinchGesture::GetCurrentDisplacement() const
		{
			return mfCurrentDisplacement;
		}
		//---Touch Delegates
		//----------------------------------------------------
		/// On Touch Delegates
		///
		/// Triggered by the input manager when the device
		/// receives touch notifications
		//----------------------------------------------------
		void PinchGesture::OnTouchBegan(const TouchInfo &Info)
		{
			mCurrentTouches++;
			
			if(!mbFirstTouchBegan)
			{
				mbIsGestureInvalid = true;
				mbFirstTouchBegan = true;
				mvStartPos1 = mvCurrentPos1 = Info.vLocation;
				mTouchID1 = Info.ID;
			}
			else if(!mbSecondTouchBegan)
			{
				// Need to set the first touch to the current first touch position
                TouchScreen* pTouchScreen = mpTouchDevice;
                
                if(mpView)
                {
                    pTouchScreen = Core::Application::GetInputSystemPtr()->GetTouchScreenPtr();
                }
                
				TouchScreen::TouchList& cTouchList = pTouchScreen->GetTouchList();
				for (u32 i = 0; i < cTouchList.size(); ++i)
				{
					if (cTouchList[i].ID == mTouchID1)
					{
						mvStartPos1 = cTouchList[i].vLocation;
						break;
					}
				}
				
				mbSecondTouchBegan = true;
				mvStartPos2 = mvCurrentPos2 = Info.vLocation;
				mTouchID2 = Info.ID;
				
				// Now generate the distance between all this stuff
				mfStartDisplacement = mfCurrentDisplacement = (mvStartPos2 - mvStartPos1).Length();
				mfRatio = 1.0f;
				
				// Determine the start angle
				Core::Vector2 cV = mvStartPos2 - mvStartPos1;
				mfStartAngle = atan2f(cV.y, cV.x);
				mfDAngle = 0.0f;
				
                m_gestureBeganEvent.NotifyConnections(this);
			}
		}
		void PinchGesture::OnTouchMoved(const TouchInfo &Info)
		{
			//Get the initial distance between the touches
			//Once that distance has changed sufficiently then we better recognize!
			if (mbFirstTouchBegan && mbSecondTouchBegan)
			{
				//Update the velocity
				if (Info.ID == mTouchID1)
					mvCurrentPos1 = Info.vLocation;
				else if (Info.ID == mTouchID2)
					mvCurrentPos2 = Info.vLocation;
				
				mfCurrentDisplacement = (mvCurrentPos2 - mvCurrentPos1).Length();
				if (mfStartDisplacement)
					mfRatio = mfCurrentDisplacement / mfStartDisplacement;
				else
					mfRatio = 0.0f;
				
				// Determine the start angle
				Core::Vector2 cV = mvCurrentPos2 - mvCurrentPos1;
				mfCurrentAngle = atan2f(cV.y, cV.x);
				mfDAngle = mfCurrentAngle - mfStartAngle;
				
				if (!mbIsGestureInvalid || (u32)abs(mfCurrentDisplacement * mfCurrentDisplacement) > mMinDistanceRequiredSqrd)
				{
					mbIsGestureInvalid = false;
					
                    NotifyGestureTriggered();
				}
			}
		}
		void PinchGesture::OnTouchEnded(const TouchInfo &Info)
		{
            if(mCurrentTouches > 0)
            {
                mCurrentTouches--;
                
                if (mbFirstTouchBegan && mbSecondTouchBegan && !mbIsGestureInvalid)
                {
                    //Update the velocity
                    if (Info.ID == mTouchID1)
                        mvCurrentPos1 = Info.vLocation;
                    else if (Info.ID == mTouchID2)
                        mvCurrentPos2 = Info.vLocation;
                    
                    mfCurrentDisplacement = (mvCurrentPos2 - mvCurrentPos1).Length();
                    if (mfStartDisplacement)
                        mfRatio = mfCurrentDisplacement / mfStartDisplacement;
                    else
                        mfRatio = 0.0f;
                    
                    m_gestureEndedEvent.NotifyConnections(this);
                }
                
                mbFirstTouchBegan = mbSecondTouchBegan = false;
            }
		}
        
        void PinchGesture::PopulateStartPositions(ChilliSource::Core::Vector2& outvFirstPosition, ChilliSource::Core::Vector2& outvSecondPosition) const
        {
            outvFirstPosition = mvStartPos1;
            outvSecondPosition = mvStartPos2;
        }
        
        void PinchGesture::PopulateCurrentPositions(ChilliSource::Core::Vector2& outvFirstPosition, ChilliSource::Core::Vector2& outvSecondPosition) const
        {
            outvFirstPosition = mvCurrentPos1;
            outvSecondPosition = mvCurrentPos2;
        }

		//================================================
		/// Tap Gesture
		///
		/// One or more contact points registered for
		/// a short period of time. Handles multiple taps
		//================================================
		//----------------------------------------------------
		/// Constructor
		///
		/// @param Surface
		/// @param Active gesture bounds in screen space
		//----------------------------------------------------
		TapCSwipeGestureGesture::TapCSwipeGestureGesture(GUI::GUIView* inpView)
		: Gesture(inpView), mNumTapsRequired(1), mfMaximumTapDuration(0.15f), mfMaxTimeBetweenTaps(0.25f), mCurrentNumTaps(0), mfLastTapTime(0.0f), mfLastBeganTime(0.0f), mudwMaxDistAllowedSqrd(1000)
		{
		}
        //----------------------------------------------------
		/// Constructor
		///
		/// @param Touch screen device
		/// @param Active gesture bounds in screen space
		//----------------------------------------------------
		TapCSwipeGestureGesture::TapCSwipeGestureGesture(TouchScreen* inpTouchDevice) 
		: Gesture(inpTouchDevice), mNumTapsRequired(1), mfMaximumTapDuration(0.15f), mfMaxTimeBetweenTaps(0.25f), mCurrentNumTaps(0), mfLastTapTime(0.0f), mfLastBeganTime(0.0f), mudwMaxDistAllowedSqrd(1000)
		{
		}
		//----------------------------------------------------
		/// Set Number of Taps Required
		///
		/// The number of taps before the gesture is recognized.
		/// Use this for double tapping etc
		/// @param Number of taps required
		//----------------------------------------------------
		void TapCSwipeGestureGesture::SetNumTapsRequired(const u32 inNumTaps)
		{
			mNumTapsRequired = inNumTaps;
		}
        //----------------------------------------------------
        /// Set Maximum Tap Duration
        ///
        /// The time the tap can be held before it is not
        /// classes as a tap anymore
        /// @param Duration
        //----------------------------------------------------
        void TapCSwipeGestureGesture::SetMaximumTapDuration(const f32 infMaximumTapDuration)
        {
            mfMaximumTapDuration = infMaximumTapDuration;
        }
        //----------------------------------------------------
        /// Set Maximum Distance Allowed Squared
        ///
        /// The maximum squared distance from the original
        /// touch that will be accepted as a tap
        /// @param Duration
        //----------------------------------------------------
        void TapCSwipeGestureGesture::SetMaxDistanceAllowedSqrd(const u32 inudwMaxDistanceAllowedSqrd)
        {
            mudwMaxDistAllowedSqrd = inudwMaxDistanceAllowedSqrd;
        }
		//---Touch Delegates
		//----------------------------------------------------
		/// On Touch Delegates
		///
		/// Triggered by the input manager when the device
		/// receives touch notifications
		//----------------------------------------------------
		void TapCSwipeGestureGesture::OnTouchBegan(const TouchInfo &Info)
		{
			if(mCurrentNumTaps == 0)
			{
				//Clean the pipes
				mTimer.Reset();
				//Start the timer. If we let go within the max duration then we have tapped. Easy!
				mTimer.Start();
				mvStartPos = Info.vLocation;
			}
			
			mfLastBeganTime = mTimer.GetTimeElapsed();
		}
		void TapCSwipeGestureGesture::OnTouchEnded(const TouchInfo &Info)
		{
			//Ok we know by now whether we had a single tap (gesture will be valid)! Let's check if we have met the multi tap criteria
			//That is that the taps happen within the given time
			Core::Vector2 v = (Info.vLocation - mvStartPos);
			if(v.LengthSquared() < mudwMaxDistAllowedSqrd && CheckForTap() && ((mNumTapsRequired == 1) || CheckForMultiTap()))
			{				
				mCurrentNumTaps++;
			}
			else 
			{
				mCurrentNumTaps = 0;
				
				if (mNumTapsRequired > 1)
				{
					OnTouchBegan(Info);
					mCurrentNumTaps = 1;
				}
			}
			
			//Have we amassed enough taps quickly enough
			if(mCurrentNumTaps >= mNumTapsRequired)
			{
				mvLocation = Info.vLocation;
				
				//Fire off the delegates
                NotifyGestureTriggered();
				
				mCurrentNumTaps = 0;
			}
		}
		//----------------------------------------------------
		/// Check For Tap
		/// @return Whether a single tap has been recognized
		//----------------------------------------------------
		bool TapCSwipeGestureGesture::CheckForTap()
		{
			f32 fCurrentTapTime = mTimer.GetTimeElapsed();
			
            // check if the timer has been activated!
			// It is only a tap if it lasted less time than the max duration allowed and if the correct amount of contact points were applied
			if(mTimer.IsTimerActive() && (fCurrentTapTime - mfLastBeganTime) < mfMaximumTapDuration)
			{
				return true;
			}

			return false;
		}
		//----------------------------------------------------------------------
		/// Check For Multi-Tap
		/// @return Whether a the most recent tap was part of the last sequence
		//----------------------------------------------------------------------
		bool TapCSwipeGestureGesture::CheckForMultiTap()
		{
			f32 fCurrentTapTime = mTimer.GetTimeElapsed();
			
			//The first tap will always be part of the sequence
			if(mCurrentNumTaps == 0)
			{
				//Save tap timestamp
				mfLastTapTime = fCurrentTapTime;
				return true;
			}
			//If another tap is registered within the time limit then that becomes
			//part of this sequence
			if((fCurrentTapTime - mfLastTapTime) <= mfMaxTimeBetweenTaps)
			{
				mfLastTapTime = fCurrentTapTime;
				return true;
			}
			
			return false;
		}
		//================================================
		/// Drag Gesture
		///
		/// One or more contact points moving perceptibly
		/// over any range
		//================================================
		//----------------------------------------------------
		/// Constructor
		///
		/// @param Surface
		/// @param Active gesture bounds in screen space
		//-----------------------------------------------------
		DragGesture::DragGesture(GUI::GUIView* inpView)
		: Gesture(inpView), mMinDistanceRequiredSqrd(2000), mfInitialHoldDuration(0.8f), mbFirstRun(true), mCurrentTouches(0), mbIsGestureActive(false), mCurrentID(0)
		{
		}
        //----------------------------------------------------
		/// Constructor
		///
		/// @param Touch screen device
		/// @param Active gesture bounds in screen space
		//-----------------------------------------------------
		DragGesture::DragGesture(TouchScreen* inpTouchDevice) 
		: Gesture(inpTouchDevice), mMinDistanceRequiredSqrd(2000), mfInitialHoldDuration(0.8f), mbFirstRun(true), mCurrentTouches(0), mbIsGestureActive(false), mCurrentID(0)
		{
		}
		//---Touch Delegates
		//----------------------------------------------------
		/// On Touch Delegates
		///
		/// Triggered by the input manager when the device
		/// receives touch notifications
		//----------------------------------------------------
		void DragGesture::OnTouchBegan(const TouchInfo &Info)
		{
			mbIsGestureInvalid = true;
			mCurrentTouches++;
			
			// Gesture becomes active when we have but 1 touch
			if (mCurrentTouches == 1)
            {
				mbIsGestureActive = true;

				// Keep track of the touch that started the gesture
                mCurrentID = Info.ID;
            }
		}
		
		void DragGesture::OnTouchMoved(const TouchInfo &Info)
		{
			// Only recognise the touch that started the gesture
			if (mbIsGestureActive && mCurrentID == Info.ID)
			{
				//Get it's initial position so we can work out it's velocity later
				if(mbFirstRun)
				{
					//Clean the pipes
					mTimer.Reset();
					
					//Start the timer. If we move after the initial hold duration and have covered the given distance we have dragged. Easy!
					mTimer.Start();
					
					mvStartPos = Info.vLocation;
					mvLocation = mvStartPos;
                    mvPreviousLocation = Info.vPreviousLocation;
					
					mbFirstRun = false;
					
                    m_gestureBeganEvent.NotifyConnections(this);
				}			
				
				//Make sure we are holding down before we declare a drag gesture recognized
				if (mTimer.GetTimeElapsed() > mfInitialHoldDuration && mCurrentTouches == 1)
				{
					// Gesture is now valid
					mbIsGestureInvalid = false;
					
					//Update location from touch
					mvLocation = Info.vLocation;
                    mvPreviousLocation = Info.vPreviousLocation;
					
					//Calculate the distance travelled
					Core::Vector2 vVelocity = mvLocation - mvStartPos;
					
					if(vVelocity.LengthSquared() > mMinDistanceRequiredSqrd)
					{
                        NotifyGestureTriggered();
						return;
					}
				}
				else if(mCurrentTouches > 1)
				{
					mbIsGestureInvalid = true;
					mbIsGestureActive = false;	// Now deactivate the gesture
	                mCurrentID = 0;
				}
				
				mbIsGestureInvalid = true;
			}
		}
		
		void DragGesture::OnTouchEnded(const TouchInfo &Info)
		{
            if(mCurrentTouches > 0)//if(mbIsGestureActive && mCurrentTouches > 0)
            {
                if(--mCurrentTouches == 0)
                {
                    //Update the location of the drag
                    mvLocation = Info.vLocation;
                    mvPreviousLocation = Info.vPreviousLocation;
                        
                    //Fire off the delegates
                    m_gestureEndedEvent.NotifyConnections(this);
                    
                    if(mCurrentTouches == 0)
                    {
                        mbFirstRun = true;
                    }
                    
                    mTimer.Reset();
                }
                
                mbIsGestureActive = false;
                mCurrentID = 0;
            }
		}
		//================================================
		/// Hold Gesture
		///
		/// One contact point registered for a short 
		/// period of time.
		//================================================
		//----------------------------------------------------
		/// Constructor
		///
		/// @param Surface
		/// @param Active gesture bounds in screen space
		//-----------------------------------------------------
		CHoldGesture::CHoldGesture(GUI::GUIView* inpView)
		: Gesture(inpView), mfMaxDistanceAllowedSqrd(100), mfHoldDuration(0.8f), mbIsGestureActive(false), mudwNumberOfTouch(0), mfInitialHoldTime(0.2f), mbIsGestureStarted(false)
		{
		}
        //----------------------------------------------------
		/// Constructor
		///
		/// @param Touch screen device
		/// @param Active gesture bounds in screen space
		//-----------------------------------------------------
		CHoldGesture::CHoldGesture(TouchScreen* inpTouchDevice) 
		: Gesture(inpTouchDevice), mfMaxDistanceAllowedSqrd(100), mfHoldDuration(0.8f), mbIsGestureActive(false), mudwNumberOfTouch(0), mfInitialHoldTime(0.2f), mbIsGestureStarted(false)
		{
		}
        
		//---Touch Delegates
		//----------------------------------------------------
		/// On Touch Delegates
		///
		/// Triggered by the input manager when the device
		/// receives touch notifications
		//----------------------------------------------------
		void CHoldGesture::OnTouchBegan(const TouchInfo &Info)
		{
			++mudwNumberOfTouch;
			if (mudwNumberOfTouch == 1)
			{
				// Start the timer
				mTimer.Reset();
				mTimer.Start();
				
				mTimer.RegisterPeriodicUpdateDelegate(Core::MakeDelegate(this, &CHoldGesture::OnGestureUpdate), 0);
				
				// Set the starting location
				mvLocation = Info.vLocation;
             
                //Set the gesture to started but it is not active until the minimum time limit has been reached
                mbIsGestureStarted = true;
			}
			else
			{
				CancelGesture();
			}
		}
		
		void CHoldGesture::OnGestureUpdate(f32 infDT)
		{
			if (mbIsGestureActive)
			{
				// The hold timer reached the required time
				if(mTimer.GetTimeElapsed() > mfHoldDuration)
				{
					// Trigger the registered delegates
                    NotifyGestureTriggered();
                    
					// Once triggered, the gesture is done
                    EndGesture();
				}
			}
            else
            {
                //We are still waiting to reach the minimum time before triggering
                if(mTimer.GetTimeElapsed() > mfInitialHoldTime)
                {
                    // Trigger the registered delegates
                    m_gestureBeganEvent.NotifyConnections(this);
                    
                    mbIsGestureActive = true;
                }
            }
		}
		
		void CHoldGesture::OnTouchMoved(const TouchInfo &Info)
		{
			if (mbIsGestureStarted)
			{
				Core::Vector2 vDistance = Info.vLocation - mvLocation;
				
				// Check the movement of the touch
				if(vDistance.LengthSquared() > mfMaxDistanceAllowedSqrd)
				{
					// The threshold has been reached, the gesture is cancelled
					CancelGesture();
				}
			}
		}
		
        //Gesture was cancelled early
		void CHoldGesture::CancelGesture()
		{
            if(mbIsGestureStarted)
            {
                m_gestureCancelledEvent.NotifyConnections(this);
                
                ResetGesture();
            }
		}
		
        //Gesture ends successfully
        void CHoldGesture::EndGesture()
        {
            if(mbIsGestureStarted)
            {
                m_gestureEndedEvent.NotifyConnections(this);
                
                ResetGesture();
            }
        }
        
        //----------------------------------------------------
        // ResetGesture
        //----------------------------------------------------
        void CHoldGesture::ResetGesture()
        {
            mbIsGestureActive = false;
			mbIsGestureStarted = false;
            
            mTimer.DeregisterPeriodicUpdateDelegate(Core::MakeDelegate(this, &CHoldGesture::OnGestureUpdate));
			mTimer.Reset();
        }
        
		void CHoldGesture::OnTouchEnded(const TouchInfo &Info)
		{
            if(mudwNumberOfTouch > 0)
            {
                mudwNumberOfTouch --;
                
                if(mudwNumberOfTouch == 0)
                {
                    //Update the location of the hold
                    mvLocation = Info.vLocation;
                    
                    CancelGesture();
                }
            }
            
		}
	}
}

