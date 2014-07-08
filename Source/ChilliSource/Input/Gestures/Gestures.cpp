//
//  Gestures.cpp
//  Chilli Source
//  Created by Scott Downie on 18/10/2010
//
//  The MIT License (MIT)
//
//  Copyright (c) 2010 Tag Games Limited
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

#include <ChilliSource/Input/Gestures/Gestures.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Delegate/MakeDelegate.h>

#include <cmath>

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
		Gesture::Gesture(PointerSystem* in_pointerSystem) : mbIsGestureInvalid(false), mpView(nullptr), m_pointerSystem(in_pointerSystem)
		{
			//Register for touch notifications
			m_pointerDownConnection = m_pointerSystem->GetPointerDownEvent().OpenConnection(Core::MakeDelegate(this, &Gesture::OnPointerDown));
			m_pointerMovedConnection = m_pointerSystem->GetPointerMovedEvent().OpenConnection(Core::MakeDelegate(this, &Gesture::OnPointerMoved));
			m_pointerUpConnection = m_pointerSystem->GetPointerUpEvent().OpenConnection(Core::MakeDelegate(this, &Gesture::OnPointerUp));
		}
        //----------------------------------------------------
		/// Constructor
		///
		/// @param Surface
		/// @param Active gesture bounds in screen space
		//----------------------------------------------------
		Gesture::Gesture(GUI::GUIView* inpView) : mbIsGestureInvalid(false), mpView(inpView), m_pointerSystem(nullptr)
		{
			//Register for touch notifications
			m_pointerDownConnection = mpView->GetPointerDownEvent().OpenConnection(Core::MakeDelegate(this, &Gesture::OnPointerDown));
			m_pointerMovedConnection = mpView->GetPointerMovedEvent().OpenConnection(Core::MakeDelegate(this, &Gesture::OnPointerMoved));
			m_pointerUpConnection = mpView->GetPointerUpEvent().OpenConnection(Core::MakeDelegate(this, &Gesture::OnPointerUp));
		}
        //----------------------------------------------------
		/// Surface Destroyed
		///
        /// The surface we are listenting too is now gone
		//----------------------------------------------------
		void Gesture::SurfaceDestroyed()
        {
            mpView = nullptr;
            m_pointerSystem = nullptr;
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
		SwipeGesture::SwipeGesture(PointerSystem* in_pointerSystem)
		: Gesture(in_pointerSystem), mMinDistanceRequiredSqrd(4000), mNumContactPointsRequired(1), mCurrentNumContactPoints(0), mMaxNumContactPoints(0), mfMaximumSwipeDuration(0.5f)
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
		//----------------------------------------------------
		void SwipeGesture::OnPointerDown(const Pointer& in_pointer, f64 in_timestamp, Pointer::InputType in_inputType)
		{
            if (in_inputType == PointerSystem::GetDefaultInputType())
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
                mvStartPos = in_pointer.GetPosition();
            }
		}
        //----------------------------------------------------
		//----------------------------------------------------
		void SwipeGesture::OnPointerUp(const Pointer& in_pointer, f64 in_timestamp, Pointer::InputType in_inputType)
		{
            if (in_inputType == PointerSystem::GetDefaultInputType())
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
                mvEndPos = in_pointer.GetPosition();
                //Average all the contact point velocities and us that to determine if we have swiped
                mvVelocity = mvEndPos - mvStartPos;
                
                if(mCurrentNumContactPoints == 0 && mTimer.GetElapsedTime() < mfMaximumSwipeDuration && mvVelocity.LengthSquared() > mMinDistanceRequiredSqrd)
                {
                    //Fire off the delegates
                    NotifyGestureTriggered();
                    
                    mMaxNumContactPoints = 0;
                }
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
		PinchGesture::PinchGesture(PointerSystem* in_pointerSystem)
		: Gesture(in_pointerSystem), mMinDistanceRequiredSqrd(6000), mfStartDisplacement(0.0f), mfCurrentDisplacement(0.0f), mbFirstTouchBegan(false), mbSecondTouchBegan(false), mCurrentTouches(0), mTouchID1(0), mfRatio(0.0f),
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
		//----------------------------------------------------
		void PinchGesture::OnPointerDown(const Pointer& in_pointer, f64 in_timestamp, Pointer::InputType in_inputType)
		{
            if (in_inputType == PointerSystem::GetDefaultInputType())
            {
                mCurrentTouches++;
                
                if(!mbFirstTouchBegan)
                {
                    mbIsGestureInvalid = true;
                    mbFirstTouchBegan = true;
                    mvStartPos1 = mvCurrentPos1 = in_pointer.GetPosition();
                    mTouchID1 = in_pointer.GetId();
                }
                else if(!mbSecondTouchBegan)
                {
                    // Need to set the first touch to the current first touch position
                    PointerSystem* pointer = m_pointerSystem;
                    
                    if(mpView)
                    {
                        pointer = Core::Application::Get()->GetSystem<PointerSystem>();
                    }
                    
                    std::vector<Pointer> pointers = pointer->GetPointers();
                    for (u32 i = 0; i < pointers.size(); ++i)
                    {
                        if (pointers[i].GetId() == mTouchID1)
                        {
                            mvStartPos1 = pointers[i].GetPosition();
                            break;
                        }
                    }
                    
                    mbSecondTouchBegan = true;
                    mvStartPos2 = mvCurrentPos2 = in_pointer.GetPosition();
                    mTouchID2 = in_pointer.GetId();
                    
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
		}
		//----------------------------------------------------
		//----------------------------------------------------
		void PinchGesture::OnPointerMoved(const Pointer& in_pointer, f64 in_timestamp)
		{
            if (in_pointer.IsInputDown(PointerSystem::GetDefaultInputType()) == true)
            {
                //Get the initial distance between the touches
                //Once that distance has changed sufficiently then we better recognize!
                if (mbFirstTouchBegan && mbSecondTouchBegan)
                {
                    //Update the velocity
                    if (in_pointer.GetId() == mTouchID1)
                        mvCurrentPos1 = in_pointer.GetPosition();
                    else if (in_pointer.GetId() == mTouchID2)
                        mvCurrentPos2 = in_pointer.GetPosition();
                    
                    mfCurrentDisplacement = (mvCurrentPos2 - mvCurrentPos1).Length();
                    if (mfStartDisplacement)
                        mfRatio = mfCurrentDisplacement / mfStartDisplacement;
                    else
                        mfRatio = 0.0f;
                    
                    // Determine the start angle
                    Core::Vector2 cV = mvCurrentPos2 - mvCurrentPos1;
                    mfCurrentAngle = atan2f(cV.y, cV.x);
                    mfDAngle = mfCurrentAngle - mfStartAngle;
                    
                    if (!mbIsGestureInvalid || (u32)std::abs(mfCurrentDisplacement * mfCurrentDisplacement) > mMinDistanceRequiredSqrd)
                    {
                        mbIsGestureInvalid = false;
                        
                        NotifyGestureTriggered();
                    }
                }
            }
		}
        //----------------------------------------------------
		//----------------------------------------------------
		void PinchGesture::OnPointerUp(const Pointer& in_pointer, f64 in_timestamp, Pointer::InputType in_inputType)
		{
            if (in_inputType == PointerSystem::GetDefaultInputType())
            {
                if(mCurrentTouches > 0)
                {
                    mCurrentTouches--;
                    
                    if (mbFirstTouchBegan && mbSecondTouchBegan && !mbIsGestureInvalid)
                    {
                        //Update the velocity
                        if (in_pointer.GetId() == mTouchID1)
                            mvCurrentPos1 = in_pointer.GetPosition();
                        else if (in_pointer.GetId() == mTouchID2)
                            mvCurrentPos2 = in_pointer.GetPosition();
                        
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
		}
        
        void PinchGesture::PopulateStartPositions(CSCore::Vector2& outvFirstPosition, CSCore::Vector2& outvSecondPosition) const
        {
            outvFirstPosition = mvStartPos1;
            outvSecondPosition = mvStartPos2;
        }
        
        void PinchGesture::PopulateCurrentPositions(CSCore::Vector2& outvFirstPosition, CSCore::Vector2& outvSecondPosition) const
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
		TapGesture::TapGesture(GUI::GUIView* inpView)
		: Gesture(inpView), mNumTapsRequired(1), mfMaximumTapDuration(0.15f), mfMaxTimeBetweenTaps(0.25f), mCurrentNumTaps(0), mfLastTapTime(0.0f), mfLastBeganTime(0.0f), mudwMaxDistAllowedSqrd(1000)
		{
		}
        //----------------------------------------------------
		/// Constructor
		///
		/// @param Touch screen device
		/// @param Active gesture bounds in screen space
		//----------------------------------------------------
		TapGesture::TapGesture(PointerSystem* in_pointerSystem)
		: Gesture(in_pointerSystem), mNumTapsRequired(1), mfMaximumTapDuration(0.15f), mfMaxTimeBetweenTaps(0.25f), mCurrentNumTaps(0), mfLastTapTime(0.0f), mfLastBeganTime(0.0f), mudwMaxDistAllowedSqrd(1000)
		{
		}
		//----------------------------------------------------
		/// Set Number of Taps Required
		///
		/// The number of taps before the gesture is recognized.
		/// Use this for double tapping etc
		/// @param Number of taps required
		//----------------------------------------------------
		void TapGesture::SetNumTapsRequired(const u32 inNumTaps)
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
        void TapGesture::SetMaximumTapDuration(const f32 infMaximumTapDuration)
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
        void TapGesture::SetMaxDistanceAllowedSqrd(const u32 inudwMaxDistanceAllowedSqrd)
        {
            mudwMaxDistAllowedSqrd = inudwMaxDistanceAllowedSqrd;
        }
		//---Touch Delegates
		//----------------------------------------------------
		//----------------------------------------------------
		void TapGesture::OnPointerDown(const Pointer& in_pointer, f64 in_timestamp, Pointer::InputType in_inputType)
		{
            if (in_inputType == PointerSystem::GetDefaultInputType())
            {
                if(mCurrentNumTaps == 0)
                {
                    //Clean the pipes
                    mTimer.Reset();
                    //Start the timer. If we let go within the max duration then we have tapped. Easy!
                    mTimer.Start();
                    mvStartPos = in_pointer.GetPosition();
                }
                
                mfLastBeganTime = mTimer.GetElapsedTime();
            }
		}
        //----------------------------------------------------
		//----------------------------------------------------
		void TapGesture::OnPointerUp(const Pointer& in_pointer, f64 in_timestamp, Pointer::InputType in_inputType)
		{
            if (in_inputType == PointerSystem::GetDefaultInputType())
            {
                //Ok we know by now whether we had a single tap (gesture will be valid)! Let's check if we have met the multi tap criteria
                //That is that the taps happen within the given time
                Core::Vector2 v = (in_pointer.GetPosition() - mvStartPos);
                if(v.LengthSquared() < mudwMaxDistAllowedSqrd && CheckForTap() && ((mNumTapsRequired == 1) || CheckForMultiTap()))
                {				
                    mCurrentNumTaps++;
                }
                else 
                {
                    mCurrentNumTaps = 0;
                    
                    if (mNumTapsRequired > 1)
                    {
                        OnPointerDown(in_pointer, in_timestamp, in_inputType);
                        mCurrentNumTaps = 1;
                    }
                }
                
                //Have we amassed enough taps quickly enough
                if(mCurrentNumTaps >= mNumTapsRequired)
                {
                    mvLocation = in_pointer.GetPosition();
                    
                    //Fire off the delegates
                    NotifyGestureTriggered();
                    
                    mCurrentNumTaps = 0;
                }
            }
		}
		//----------------------------------------------------
		/// Check For Tap
		/// @return Whether a single tap has been recognized
		//----------------------------------------------------
		bool TapGesture::CheckForTap()
		{
			f32 fCurrentTapTime = mTimer.GetElapsedTime();
			
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
		bool TapGesture::CheckForMultiTap()
		{
			f32 fCurrentTapTime = mTimer.GetElapsedTime();
			
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
		DragGesture::DragGesture(PointerSystem* in_pointerSystem)
		: Gesture(in_pointerSystem), mMinDistanceRequiredSqrd(2000), mfInitialHoldDuration(0.8f), mbFirstRun(true), mCurrentTouches(0), mbIsGestureActive(false), mCurrentID(0)
		{
		}
		//---Touch Delegates
		//----------------------------------------------------
		//----------------------------------------------------
		void DragGesture::OnPointerDown(const Pointer& in_pointer, f64 in_timestamp, Pointer::InputType in_inputType)
		{
			mbIsGestureInvalid = true;
			mCurrentTouches++;
			
			// Gesture becomes active when we have but 1 touch
			if (mCurrentTouches == 1)
            {
				mbIsGestureActive = true;

				// Keep track of the touch that started the gesture
                mCurrentID = in_pointer.GetId();
            }
		}
        //----------------------------------------------------
		//----------------------------------------------------
		void DragGesture::OnPointerMoved(const Pointer& in_pointer, f64 in_timestamp)
		{
            if (in_pointer.IsInputDown(PointerSystem::GetDefaultInputType()) == true)
            {
                // Only recognise the touch that started the gesture
                if (mbIsGestureActive && mCurrentID == in_pointer.GetId())
                {
                    //Get it's initial position so we can work out it's velocity later
                    if(mbFirstRun)
                    {
                        //Clean the pipes
                        mTimer.Reset();
                        
                        //Start the timer. If we move after the initial hold duration and have covered the given distance we have dragged. Easy!
                        mTimer.Start();
                        
                        mvStartPos = in_pointer.GetPosition();
                        mvLocation = mvStartPos;
                        mvPreviousLocation = in_pointer.GetPreviousPosition();
                        
                        mbFirstRun = false;
                        
                        m_gestureBeganEvent.NotifyConnections(this);
                    }			
                    
                    //Make sure we are holding down before we declare a drag gesture recognized
                    if (mTimer.GetElapsedTime() > mfInitialHoldDuration && mCurrentTouches == 1)
                    {
                        // Gesture is now valid
                        mbIsGestureInvalid = false;
                        
                        //Update location from touch
                        mvLocation = in_pointer.GetPosition();
                        mvPreviousLocation = in_pointer.GetPreviousPosition();
                        
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
		}
		
		void DragGesture::OnPointerUp(const Pointer& in_pointer, f64 in_timestamp, Pointer::InputType in_inputType)
		{
			if (in_inputType == PointerSystem::GetDefaultInputType())
			{
				if(mCurrentTouches > 0)
				{
					if(--mCurrentTouches == 0)
					{
						//Update the location of the drag
						mvLocation = in_pointer.GetPosition();
						mvPreviousLocation = in_pointer.GetPreviousPosition();

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
		HoldGesture::HoldGesture(GUI::GUIView* inpView)
		: Gesture(inpView), mfMaxDistanceAllowedSqrd(100), mfHoldDuration(0.8f), mbIsGestureActive(false), mudwNumberOfTouch(0), mfInitialHoldTime(0.2f), mbIsGestureStarted(false)
		{
		}
        //----------------------------------------------------
		/// Constructor
		///
		/// @param Touch screen device
		/// @param Active gesture bounds in screen space
		//-----------------------------------------------------
		HoldGesture::HoldGesture(PointerSystem* in_pointerSystem)
		: Gesture(in_pointerSystem), mfMaxDistanceAllowedSqrd(100), mfHoldDuration(0.8f), mbIsGestureActive(false), mudwNumberOfTouch(0), mfInitialHoldTime(0.2f), mbIsGestureStarted(false)
		{
		}
        
		//---Touch Delegates
		//----------------------------------------------------
		//----------------------------------------------------
		void HoldGesture::OnPointerDown(const Pointer& in_pointer, f64 in_timestamp, Pointer::InputType in_inputType)
		{
			if (in_inputType == PointerSystem::GetDefaultInputType())
			{
				++mudwNumberOfTouch;
				if (mudwNumberOfTouch == 1)
				{
					// Start the timer
					mTimer.Reset();
					mTimer.Start();

					m_periodicTimerConnection = mTimer.OpenConnection(Core::MakeDelegate(this, &HoldGesture::OnGestureUpdate), 0);

					// Set the starting location
					mvLocation = in_pointer.GetPosition();

					//Set the gesture to started but it is not active until the minimum time limit has been reached
					mbIsGestureStarted = true;
				}
				else
				{
					CancelGesture();
				}
			}
		}
		
		void HoldGesture::OnGestureUpdate()
		{
			if (mbIsGestureActive)
			{
				// The hold timer reached the required time
				if(mTimer.GetElapsedTime() > mfHoldDuration)
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
                if(mTimer.GetElapsedTime() > mfInitialHoldTime)
                {
                    // Trigger the registered delegates
                    m_gestureBeganEvent.NotifyConnections(this);
                    
                    mbIsGestureActive = true;
                }
            }
		}
        //----------------------------------------------------
		//----------------------------------------------------
		void HoldGesture::OnPointerMoved(const Pointer& in_pointer, f64 in_timestamp)
		{
			if (in_pointer.IsInputDown(PointerSystem::GetDefaultInputType()) == true && mbIsGestureStarted)
			{
				Core::Vector2 vDistance = in_pointer.GetPosition() - mvLocation;
				
				// Check the movement of the touch
				if(vDistance.LengthSquared() > mfMaxDistanceAllowedSqrd)
				{
					// The threshold has been reached, the gesture is cancelled
					CancelGesture();
				}
			}
		}
		
        //Gesture was cancelled early
		void HoldGesture::CancelGesture()
		{
            if(mbIsGestureStarted)
            {
                m_gestureCancelledEvent.NotifyConnections(this);
                
                ResetGesture();
            }
		}
		
        //Gesture ends successfully
        void HoldGesture::EndGesture()
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
        void HoldGesture::ResetGesture()
        {
            mbIsGestureActive = false;
			mbIsGestureStarted = false;
            
            m_periodicTimerConnection = nullptr;
			mTimer.Reset();
        }
        //----------------------------------------------------
		//----------------------------------------------------
		void HoldGesture::OnPointerUp(const Pointer& in_pointer, f64 in_timestamp, Pointer::InputType in_inputType)
		{
            if (in_inputType == PointerSystem::GetDefaultInputType() && mudwNumberOfTouch > 0)
            {
                mudwNumberOfTouch --;
                
                if(mudwNumberOfTouch == 0)
                {
                    //Update the location of the hold
                    mvLocation = in_pointer.GetPosition();
                    
                    CancelGesture();
                }
            }
		}
	}
}

