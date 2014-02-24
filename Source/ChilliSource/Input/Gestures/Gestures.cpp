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
		CGesture::CGesture(ITouchScreen* inpTouchDevice) : mbIsGestureInvalid(false), mpView(nullptr), mpTouchDevice(inpTouchDevice)
		{
			//Register for touch notifications
			inpTouchDevice->GetTouchBeganEvent() += TouchEventDelegate(this, &CGesture::OnTouchBegan);
			inpTouchDevice->GetTouchMovedEvent() += TouchEventDelegate(this, &CGesture::OnTouchMoved);
			inpTouchDevice->GetTouchEndEvent() += TouchEventDelegate(this, &CGesture::OnTouchEnded);
		}
        //----------------------------------------------------
		/// Constructor
		///
		/// @param Surface
		/// @param Active gesture bounds in screen space
		//----------------------------------------------------
		CGesture::CGesture(Rendering::CGUIView* inpView) : mbIsGestureInvalid(false), mpView(inpView), mpTouchDevice(NULL)
		{
			//Register for touch notifications
			mpView->GetTouchBeganEvent() += TouchEventDelegate(this, &CGesture::OnTouchBegan);
			mpView->GetTouchMovedEvent() += TouchEventDelegate(this, &CGesture::OnTouchMoved);
			mpView->GetTouchEndEvent() += TouchEventDelegate(this, &CGesture::OnTouchEnded);
		}
		//----------------------------------------------------
		/// Register Gesture Delegate
		///
		/// Triggered when the delegate of object type is
		/// recognized.
		/// @param Event listener
		//----------------------------------------------------
		void CGesture::RegisterGestureDelegate(GestureEventDelegate inEventDelegate)
		{
			mGestureDelegates.push_back(inEventDelegate);
		}
		//----------------------------------------------------
		/// Deregister Gesture Delegate
		///
		/// Usubscribe the listener for event notifications
		/// @param Event listener
		//----------------------------------------------------
		void CGesture::DeregisterGestureDelegate(GestureEventDelegate inEventDelegate)
		{
			for(GestureDelegatesList::iterator it = mGestureDelegates.begin(); it != mGestureDelegates.end(); ++it)
            {
                if(*it == inEventDelegate)
                {
                    mGestureDelegates.erase(it);
                    return;
                }
            }
		}
        //----------------------------------------------------
		/// Surface Destroyed
		///
        /// The surface we are listenting too is now gone
		//----------------------------------------------------
		void CGesture::SurfaceDestroyed()
        {
            mpView = NULL;
            mpTouchDevice = NULL;
        }
        //----------------------------------------------------
		/// Notify Gesture Triggered
		///
        /// Notify listeners that a gesture action has
        /// occurred
		//----------------------------------------------------
        void CGesture::NotifyGestureTriggered()
        {
            for(GestureDelegatesListItr it = mGestureDelegates.begin(); it != mGestureDelegates.end(); ++it)
            {
                (*it)(*this);
            }
        }
		//----------------------------------------------------
		/// Destructor
		///
		//----------------------------------------------------
		CGesture::~CGesture()
		{
            if(mpView)
            {
                mpView->GetTouchBeganEvent() -= TouchEventDelegate(this, &CGesture::OnTouchBegan);
                mpView->GetTouchMovedEvent() -= TouchEventDelegate(this, &CGesture::OnTouchMoved);
                mpView->GetTouchEndEvent() -= TouchEventDelegate(this, &CGesture::OnTouchEnded);
                mpView = NULL;
            }
            else if(mpTouchDevice)
            {
                mpTouchDevice->GetTouchBeganEvent() -= TouchEventDelegate(this, &CGesture::OnTouchBegan);
                mpTouchDevice->GetTouchMovedEvent() -= TouchEventDelegate(this, &CGesture::OnTouchMoved);
                mpTouchDevice->GetTouchEndEvent() -= TouchEventDelegate(this, &CGesture::OnTouchEnded);
                mpTouchDevice = NULL;
            }
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
		CSwipeGesture::CSwipeGesture(Rendering::CGUIView* inpView)
		: CGesture(inpView), mMinDistanceRequiredSqrd(4000), mNumContactPointsRequired(1), mCurrentNumContactPoints(0), mMaxNumContactPoints(0), mfMaximumSwipeDuration(0.5f)
		{
		}
        //----------------------------------------------------
		/// Constructor
		///
		/// @param Touch screen device
		/// @param Active gesture bounds in screen space
		//----------------------------------------------------
		CSwipeGesture::CSwipeGesture(ITouchScreen* inpTouchDevice)
		: CGesture(inpTouchDevice), mMinDistanceRequiredSqrd(4000), mNumContactPointsRequired(1), mCurrentNumContactPoints(0), mMaxNumContactPoints(0), mfMaximumSwipeDuration(0.5f)
		{
		}
		//----------------------------------------------------
		/// Set Number of Contact Points Required
		///
		/// The number of contacts before the gesture is recognized.
		/// @param Number of points required
		//----------------------------------------------------
		void CSwipeGesture::SetNumContactPointsRequired(const u32 inNumContacts)
		{
			mNumContactPointsRequired = inNumContacts;
		}
		//----------------------------------------------------
		/// Set Maximum Swipe Duration
		///
		/// The maximum duration during which the swipe will be recognised.
		/// @param Max duration of the swipe gesture
		//----------------------------------------------------
		void CSwipeGesture::SetMaximumSwipeDuration(const f32 infMaximumSwipeDuration)
		{
			mfMaximumSwipeDuration = infMaximumSwipeDuration;
		}
		//----------------------------------------------------
		/// SetMinimumDistance
		///
		/// Set the minimum distance touches must move to trigger the gesture
		/// @param Minimum Distance
		//----------------------------------------------------
		void CSwipeGesture::SetMinimumDistance(u32 inudwMinDistance)
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
		void CSwipeGesture::OnTouchBegan(const TouchInfo &Info)
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
		void CSwipeGesture::OnTouchEnded(const TouchInfo &Info)
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
		CPinchGesture::CPinchGesture(Rendering::CGUIView* inpView)
		: CGesture(inpView), mMinDistanceRequiredSqrd(6000), mfStartDisplacement(0.0f), mfCurrentDisplacement(0.0f), mbFirstTouchBegan(false), mbSecondTouchBegan(false), mCurrentTouches(0), mTouchID1(0), mfRatio(0.0f),
		mTouchID2(0)
		{
		}
        //----------------------------------------------------
		/// Constructor
		///
		/// @param Touch screen device
		/// @param Active gesture bounds in screen space
		//----------------------------------------------------
		CPinchGesture::CPinchGesture(ITouchScreen* inpTouchDevice) 
		: CGesture(inpTouchDevice), mMinDistanceRequiredSqrd(6000), mfStartDisplacement(0.0f), mfCurrentDisplacement(0.0f), mbFirstTouchBegan(false), mbSecondTouchBegan(false), mCurrentTouches(0), mTouchID1(0), mfRatio(0.0f),
		mTouchID2(0)
		{
		}
		//----------------------------------------------------
		/// Register Gesture Began Delegate
		///
		/// Triggered when the delegate of object type is
		/// recognized.
		/// @param Event listener
		//----------------------------------------------------
		void CPinchGesture::RegisterGestureBeganDelegate(GestureEventDelegate inEventDelegate)
		{
			mGestureBeganDelegates.push_back(inEventDelegate);
		}
		//----------------------------------------------------
		/// Deregister Gesture Began Delegate
		///
		/// Usubscribe the listener for event notifications
		/// @param Event listener
		//----------------------------------------------------
		void CPinchGesture::DeregisterGestureBeganDelegate(GestureEventDelegate inEventDelegate)
		{
            for(GestureDelegatesList::iterator it = mGestureBeganDelegates.begin(); it != mGestureBeganDelegates.end(); ++it)
            {
                if(*it == inEventDelegate)
                {
                    mGestureBeganDelegates.erase(it);
                    return;
                }
            }
		}
		//----------------------------------------------------
		/// Register Gesture Delegate
		///
		/// Triggered when the delegate of object type is
		/// recognized.
		/// @param Event listener
		//----------------------------------------------------
		void CPinchGesture::RegisterGestureEndedDelegate(GestureEventDelegate inEventDelegate)
		{
			mGestureEndedDelegates.push_back(inEventDelegate);
		}
		//----------------------------------------------------
		/// Deregister Gesture Delegate
		///
		/// Usubscribe the listener for event notifications
		/// @param Event listener
		//----------------------------------------------------
		void CPinchGesture::DeregisterGestureEndedDelegate(GestureEventDelegate inEventDelegate)
		{
            for(GestureDelegatesList::iterator it = mGestureEndedDelegates.begin(); it != mGestureEndedDelegates.end(); ++it)
            {
                if(*it == inEventDelegate)
                {
                    mGestureEndedDelegates.erase(it);
                    return;
                }
            }
		}
		//----------------------------------------------------
		/// Get Start Displacement
		///
		/// @return Start Displacement
		//----------------------------------------------------
		const f32 CPinchGesture::GetStartDisplacement() const
		{
			return mfStartDisplacement;
		}
		//----------------------------------------------------
		/// Get Current Displacement
		///
		/// @return Current Displacement
		//----------------------------------------------------
		const f32 CPinchGesture::GetCurrentDisplacement() const
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
		void CPinchGesture::OnTouchBegan(const TouchInfo &Info)
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
                ITouchScreen* pTouchScreen = mpTouchDevice;
                
                if(mpView)
                {
                    pTouchScreen = Core::CApplication::GetInputSystemPtr()->GetTouchScreenPtr();
                }
                
				ITouchScreen::TouchList& cTouchList = pTouchScreen->GetTouchList();
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
				Core::CVector2 cV = mvStartPos2 - mvStartPos1;
				mfStartAngle = atan2f(cV.y, cV.x);
				mfDAngle = 0.0f;
				
				// Call the gesture began delegate
				for (GestureDelegatesListItr it = mGestureBeganDelegates.begin(); it != mGestureBeganDelegates.end(); ++it)
					(*it)(*this);
			}
		}
		void CPinchGesture::OnTouchMoved(const TouchInfo &Info)
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
				Core::CVector2 cV = mvCurrentPos2 - mvCurrentPos1;
				mfCurrentAngle = atan2f(cV.y, cV.x);
				mfDAngle = mfCurrentAngle - mfStartAngle;
				
				if (!mbIsGestureInvalid || (u32)abs(mfCurrentDisplacement * mfCurrentDisplacement) > mMinDistanceRequiredSqrd)
				{
					mbIsGestureInvalid = false;
					
                    NotifyGestureTriggered();
				}
			}
		}
		void CPinchGesture::OnTouchEnded(const TouchInfo &Info)
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
                    
                    for (GestureDelegatesListItr it = mGestureEndedDelegates.begin(); it != mGestureEndedDelegates.end(); ++it)
                    {
                        (*it)(*this);
                    }
                }
                
                mbFirstTouchBegan = mbSecondTouchBegan = false;
            }
		}
        
        void CPinchGesture::PopulateStartPositions(ChilliSource::Core::CVector2& outvFirstPosition, ChilliSource::Core::CVector2& outvSecondPosition) const
        {
            outvFirstPosition = mvStartPos1;
            outvSecondPosition = mvStartPos2;
        }
        
        void CPinchGesture::PopulateCurrentPositions(ChilliSource::Core::CVector2& outvFirstPosition, ChilliSource::Core::CVector2& outvSecondPosition) const
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
		CTapGesture::CTapGesture(Rendering::CGUIView* inpView)
		: CGesture(inpView), mNumTapsRequired(1), mfMaximumTapDuration(0.15f), mfMaxTimeBetweenTaps(0.25f), mCurrentNumTaps(0), mfLastTapTime(0.0f), mfLastBeganTime(0.0f), mudwMaxDistAllowedSqrd(1000)
		{
		}
        //----------------------------------------------------
		/// Constructor
		///
		/// @param Touch screen device
		/// @param Active gesture bounds in screen space
		//----------------------------------------------------
		CTapGesture::CTapGesture(ITouchScreen* inpTouchDevice) 
		: CGesture(inpTouchDevice), mNumTapsRequired(1), mfMaximumTapDuration(0.15f), mfMaxTimeBetweenTaps(0.25f), mCurrentNumTaps(0), mfLastTapTime(0.0f), mfLastBeganTime(0.0f), mudwMaxDistAllowedSqrd(1000)
		{
		}
		//----------------------------------------------------
		/// Set Number of Taps Required
		///
		/// The number of taps before the gesture is recognized.
		/// Use this for double tapping etc
		/// @param Number of taps required
		//----------------------------------------------------
		void CTapGesture::SetNumTapsRequired(const u32 inNumTaps)
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
        void CTapGesture::SetMaximumTapDuration(const f32 infMaximumTapDuration)
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
        void CTapGesture::SetMaxDistanceAllowedSqrd(const u32 inudwMaxDistanceAllowedSqrd)
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
		void CTapGesture::OnTouchBegan(const TouchInfo &Info)
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
		void CTapGesture::OnTouchEnded(const TouchInfo &Info)
		{
			//Ok we know by now whether we had a single tap (gesture will be valid)! Let's check if we have met the multi tap criteria
			//That is that the taps happen within the given time
			Core::CVector2 v = (Info.vLocation - mvStartPos);
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
		bool CTapGesture::CheckForTap()
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
		bool CTapGesture::CheckForMultiTap()
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
		CDragGesture::CDragGesture(Rendering::CGUIView* inpView)
		: CGesture(inpView), mMinDistanceRequiredSqrd(2000), mfInitialHoldDuration(0.8f), mbFirstRun(true), mCurrentTouches(0), mbIsGestureActive(false), mCurrentID(0)
		{
		}
        //----------------------------------------------------
		/// Constructor
		///
		/// @param Touch screen device
		/// @param Active gesture bounds in screen space
		//-----------------------------------------------------
		CDragGesture::CDragGesture(ITouchScreen* inpTouchDevice) 
		: CGesture(inpTouchDevice), mMinDistanceRequiredSqrd(2000), mfInitialHoldDuration(0.8f), mbFirstRun(true), mCurrentTouches(0), mbIsGestureActive(false), mCurrentID(0)
		{
		}
		//----------------------------------------------------
		/// RegisterGestureBeganDelegate
		///
		/// Triggered when the delegate of object type is
		/// recognized.
		/// @param Event listener
		//----------------------------------------------------
		void CDragGesture::RegisterGestureBeganDelegate(GestureEventDelegate inEventDelegate)
		{
			mGestureBeganDelegates.push_back(inEventDelegate);
		}
		//----------------------------------------------------
		/// DeregisterGestureBeganDelegate
		///
		/// Usubscribe the listener for event notifications
		/// @param Event listener
		//----------------------------------------------------
		void CDragGesture::DeregisterGestureBeganDelegate(GestureEventDelegate inEventDelegate)
		{
            for(GestureDelegatesList::iterator it = mGestureBeganDelegates.begin(); it != mGestureBeganDelegates.end(); ++it)
            {
                if(*it == inEventDelegate)
                {
                    mGestureBeganDelegates.erase(it);
                    return;
                }
            }
		}
		//----------------------------------------------------
		/// Register Gesture Delegate
		///
		/// Triggered when the delegate of object type is
		/// recognized.
		/// @param Event listener
		//----------------------------------------------------
		void CDragGesture::RegisterGestureEndedDelegate(GestureEventDelegate inEventDelegate)
		{
			mGestureEndedDelegates.push_back(inEventDelegate);
		}
		//----------------------------------------------------
		/// Deregister Gesture Delegate
		///
		/// Usubscribe the listener for event notifications
		/// @param Event listener
		//----------------------------------------------------
		void CDragGesture::DeregisterGestureEndedDelegate(GestureEventDelegate inEventDelegate)
		{
            for(GestureDelegatesList::iterator it = mGestureEndedDelegates.begin(); it != mGestureEndedDelegates.end(); ++it)
            {
                if(*it == inEventDelegate)
                {
                    mGestureEndedDelegates.erase(it);
                    return;
                }
            }
		}
		//---Touch Delegates
		//----------------------------------------------------
		/// On Touch Delegates
		///
		/// Triggered by the input manager when the device
		/// receives touch notifications
		//----------------------------------------------------
		void CDragGesture::OnTouchBegan(const TouchInfo &Info)
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
		
		void CDragGesture::OnTouchMoved(const TouchInfo &Info)
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
					
					for(GestureDelegatesListItr it = mGestureBeganDelegates.begin(); it != mGestureBeganDelegates.end(); ++it)
					{
						(*it)(*this);
					}
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
					Core::CVector2 vVelocity = mvLocation - mvStartPos;
					
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
		
		void CDragGesture::OnTouchEnded(const TouchInfo &Info)
		{
            if(mCurrentTouches > 0)//if(mbIsGestureActive && mCurrentTouches > 0)
            {
                if(--mCurrentTouches == 0)
                {
                    //Update the location of the drag
                    mvLocation = Info.vLocation;
                    mvPreviousLocation = Info.vPreviousLocation;
                        
                    //Fire off the delegates
                    for(GestureDelegatesListItr it = mGestureEndedDelegates.begin(); it != mGestureEndedDelegates.end(); ++it)
                    {
                        (*it)(*this);
                    }
                    
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
		CHoldGesture::CHoldGesture(Rendering::CGUIView* inpView)
		: CGesture(inpView), mfMaxDistanceAllowedSqrd(100), mfHoldDuration(0.8f), mbIsGestureActive(false), mudwNumberOfTouch(0), mfInitialHoldTime(0.2f), mbIsGestureStarted(false)
		{
		}
        //----------------------------------------------------
		/// Constructor
		///
		/// @param Touch screen device
		/// @param Active gesture bounds in screen space
		//-----------------------------------------------------
		CHoldGesture::CHoldGesture(ITouchScreen* inpTouchDevice) 
		: CGesture(inpTouchDevice), mfMaxDistanceAllowedSqrd(100), mfHoldDuration(0.8f), mbIsGestureActive(false), mudwNumberOfTouch(0), mfInitialHoldTime(0.2f), mbIsGestureStarted(false)
		{
		}
		//----------------------------------------------------
		/// RegisterGestureBeganDelegate
		///
		/// Triggered when the delegate of object type is
		/// recognized.
		/// @param Event listener
		//----------------------------------------------------
		void CHoldGesture::RegisterGestureBeganDelegate(GestureEventDelegate inEventDelegate)
		{
			mGestureBeganDelegates.push_back(inEventDelegate);
		}
		//----------------------------------------------------
		/// DeregisterGestureBeganDelegate
		///
		/// Usubscribe the listener for event notifications
		/// @param Event listener
		//----------------------------------------------------
		void CHoldGesture::DeregisterGestureBeganDelegate(GestureEventDelegate inEventDelegate)
		{
            for(GestureDelegatesList::iterator it = mGestureBeganDelegates.begin(); it != mGestureBeganDelegates.end(); ++it)
            {
                if(*it == inEventDelegate)
                {
                    mGestureBeganDelegates.erase(it);
                    return;
                }
            }
		}
		//----------------------------------------------------
		/// Register Gesture Delegate
		///
		/// Triggered when the delegate of object type is
		/// recognized.
		/// @param Event listener
		//----------------------------------------------------
		void CHoldGesture::RegisterGestureEndedDelegate(GestureEventDelegate inEventDelegate)
		{
			mGestureEndedDelegates.push_back(inEventDelegate);
		}
		//----------------------------------------------------
		/// Deregister Gesture Delegate
		///
		/// Usubscribe the listener for event notifications
		/// @param Event listener
		//----------------------------------------------------
		void CHoldGesture::DeregisterGestureEndedDelegate(GestureEventDelegate inEventDelegate)
		{
            for(GestureDelegatesList::iterator it = mGestureEndedDelegates.begin(); it != mGestureEndedDelegates.end(); ++it)
            {
                if(*it == inEventDelegate)
                {
                    mGestureEndedDelegates.erase(it);
                    return;
                }
            }
		}
        
        //----------------------------------------------------
		/// Register Gesture Delegate
		///
		/// Triggered when the Hold gesture is aborted before completion is
        /// recognized.
		/// @param Event listener
		//----------------------------------------------------
        void CHoldGesture::RegisterGestureCancelledDelegate(GestureEventDelegate inEventDelegate)
        {
            mGestureCancelledDelegates.push_back(inEventDelegate);
        }
        
        //----------------------------------------------------
		/// Deregister Gesture Delegate
		///
		/// Usubscribe the listener for event notifications
		/// @param Event listener
		//----------------------------------------------------
		void CHoldGesture::DeregisterGestureCancelledDelegate(GestureEventDelegate inEventDelegate)
		{
            for(GestureDelegatesList::iterator it = mGestureCancelledDelegates.begin(); it != mGestureCancelledDelegates.end(); ++it)
            {
                if(*it == inEventDelegate)
                {
                    mGestureCancelledDelegates.erase(it);
                    return;
                }
            }
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
				
				mTimer.RegisterPeriodicUpdateDelegate(ChilliSource::Core::TimeEventDelegate(this, &CHoldGesture::OnGestureUpdate), 0);
				
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
                    for(GestureDelegatesListItr it = mGestureBeganDelegates.begin(); it != mGestureBeganDelegates.end(); ++it)
                    {
                        (*it)(*this);
                    }
                    
                    mbIsGestureActive = true;
                }
            }
		}
		
		void CHoldGesture::OnTouchMoved(const TouchInfo &Info)
		{
			if (mbIsGestureStarted)
			{
				Core::CVector2 vDistance = Info.vLocation - mvLocation;
				
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
                for(GestureDelegatesListItr it = mGestureCancelledDelegates.begin(); it != mGestureCancelledDelegates.end(); ++it)
                {
                    (*it)(*this);
                }
                
                ResetGesture();
            }
		}
		
        //Gesture ends successfully
        void CHoldGesture::EndGesture()
        {
            if(mbIsGestureStarted)
            {
                //Fire off the delegates
                for(GestureDelegatesListItr it = mGestureEndedDelegates.begin(); it != mGestureEndedDelegates.end(); ++it)
                {
                    (*it)(*this);
                }

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
            
            mTimer.DeregisterPeriodicUpdateDelegate(ChilliSource::Core::TimeEventDelegate(this, &CHoldGesture::OnGestureUpdate));
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

