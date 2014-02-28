/** 
 * File: Touchscreen.h
 * Date: 5 Oct 2010
 * Description: Interface for a TouchScreen input device
 */

/** 
 *
 * Author Stuart 
 * Version 1.0 - moFlo
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#ifndef _MOFLO_INPUT_TOUCHSCREEN_H_
#define _MOFLO_INPUT_TOUCHSCREEN_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Input/Base/InputDevice.h>
#include <ChilliSource/Core/Event/Event.h>
#include <ChilliSource/Core/Math/Vector2.h>
#include <ChilliSource/Core/Math/Matrix4x4.h>

#include <functional>
#include <thread>

namespace ChilliSource
{
	namespace Input
	{
        enum class TouchInputType
        {
            k_began,
            k_moved,
            k_ended
        };
		struct TouchInfo
		{
			bool operator ==(const TouchInfo & inOther);
			
			Core::Vector2 vLocation; //Where the touch is now in screenspace
			Core::Vector2 vPreviousLocation; //Where it was last update cycle in screenspace
			f64 TimeStamp;
            u32 ID; //Unique identifier for this touch
            TouchInputType eType;
		};
		
		//Delegates
		
		typedef std::function<void(const TouchInfo&)> TouchEventDelegate;
		
		class TouchScreen : public InputDevice
		{
		public:
			TouchScreen();
			typedef std::vector<TouchInfo> TouchList;
			typedef TouchList::iterator TouchListItr;

			virtual ~TouchScreen();
			
			CS_DECLARE_NAMEDTYPE(TouchScreen);
			
			Core::IConnectableEvent<TouchEventDelegate> & GetTouchBeganEvent();
			Core::IConnectableEvent<TouchEventDelegate> & GetTouchMovedEvent();
			Core::IConnectableEvent<TouchEventDelegate> & GetTouchEndEvent();
			
            void Enable();
            void Disable();
            
			void SetTouchTransformMatrix(const Core::Matrix4x4 & inTouchTransformMat);
			void SetCurrentAppTime(f64 inffTimeStamp);
			void SetScreenHeight(u32 inudwScreenHeight);
            
            //-----------------------------------------------------------
            /// Flush Buffered Input
            ///
            /// Have the input elements notify listeners of each
            /// buffered value then clear the buffered input
            //-----------------------------------------------------------
            void FlushBufferedInput();

			u32 StartTouch(const Core::Vector2 & invTouchLocation, f64 inTimeStamp);
			void MoveTouch(u32 inID, const Core::Vector2 & invNewTouchLocation, f64 inTimeStamp);
			void EndTouch(u32 inID, f64 inTimeStamp);

			f64 GetLastTimeStamp() const{return mffLastTimeStamp;}
			TouchInfo& GetTouch(u32 inudwID) { return maActiveTouches[inudwID]; }
			TouchList& GetTouchList() { return maActiveTouches; }
		protected:
            //-----------------------------------------------------------
            /// Add Active Touch
            ///
            /// Adds a new touch to the active touch list.
            ///
            /// @param The touch info.
            //-----------------------------------------------------------
            void AddActiveTouch(const TouchInfo& inTouchInfo);
            //-----------------------------------------------------------
            /// Update Active Touch
            ///
            /// Updates an existing touch in the active touch list.
            ///
            /// @param The touch info.
            //-----------------------------------------------------------
            void UpdateActiveTouch(const TouchInfo& inTouchInfo);
            //-----------------------------------------------------------
            /// Remove Active Touch
            ///
            /// Removes an existing touch from the active list.
            ///
            /// @param The touch info.
            //-----------------------------------------------------------
            void RemoveActiveTouch(const TouchInfo& inTouchInfo);
            
			Core::Event<TouchEventDelegate> mTouchBeganEvent;
			Core::Event<TouchEventDelegate> mTouchMovedEvent;
			Core::Event<TouchEventDelegate> mTouchEndedEvent;
			
            
			TouchList mOpenTouches;
            TouchList maActiveTouches;
            TouchList mBufferedTouches;
			
			u32 mLastTouchIndex;
			f64 mffLastTimeStamp;
			
			Core::Matrix4x4 mTouchTransformMatrix;
			
			u32 mudwOrientedScreenHeight;
			std::mutex mMutex;
            
            bool mbIsEnabled;
		};
		
	}
}

#endif
