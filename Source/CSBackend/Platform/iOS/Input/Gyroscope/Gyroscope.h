//
//  Gyroscope.h
//  ChilliSource
//  Created by Jordan Brown on 19/01/2017
//
//  The MIT License (MIT)
//
//  Copyright (c) 2017 Tag Games Limited
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

#ifdef CS_TARGETPLATFORM_IOS

#ifndef _CSBACKEND_IOS_INPUT_GYROSCOPE_GYROSCOPE_H_
#define _CSBACKEND_IOS_INPUT_GYROSCOPE_GYROSCOPE_H_

#include <ChilliSource/ChilliSource.h>
#include <CSBackend/Platform/iOS/ForwardDeclarations.h>
#include <ChilliSource/Input/Gyroscope/Gyroscope.h>

@class CMDeviceMotion;
@class CMMotionManager;
@class NSError;

namespace CSBackend
{
    namespace iOS
    {
        //------------------------------------------------------------
        /// The iOS backend implementation of the Gyroscope.
        ///
        /// @author J Brown
        //------------------------------------------------------------
        class Gyroscope final : public ChilliSource::Gyroscope
        {
        public:
            CS_DECLARE_NAMEDTYPE(Gyroscope);
            //----------------------------------------------------
            /// Returns whether or not this is supported on this
            /// device. This should be called prior to allocating
            /// the system.
            ///
            /// @author J Brown
            ///
            /// @return Whether or not the Gyroscope is supported
            /// on this device.
            //----------------------------------------------------
            static bool IsSupportedByDevice();
            //----------------------------------------------------
            /// Queries whether or not this system implements the
            /// interface with the given ID.
            ///
            /// @author J Brown
            ///
            /// @param The interface Id.
            ///
            /// @return Whether or not the interface is implemented.
            //----------------------------------------------------
            bool IsA(ChilliSource::InterfaceIDType in_interfaceId) const override;
            //----------------------------------------------------
            /// @author J Brown
            ///
            /// @return whether or not the gyroscope is
            /// currently updating.
            //----------------------------------------------------
            bool IsUpdating() const override;
            //----------------------------------------------------
            /// Start listening for gyroscope changes.
            ///
            /// @author J Brown
            //----------------------------------------------------
            void StartUpdating() override;
            
            //----------------------------------------------------
            /// @author J Brown
            ///
            /// @return The physical orientation of the device.
            //----------------------------------------------------
            ChilliSource::Quaternion GetOrientation() const override;
            //----------------------------------------------------
            /// @author J Brown
            ///
            /// @return An event that is invoked every time the
            /// orientation is updated. The orientation will not
            /// necessarily have changed between updates.
            //----------------------------------------------------
            ChilliSource::IConnectableEvent<OrientationUpdatedDelegate>& GetOrientationUpdatedEvent() override;
            //----------------------------------------------------
            /// Stop listening for orientation changes.
            ///
            /// @author J Brown
            //----------------------------------------------------
            void StopUpdating() override;
        private:
            friend ChilliSource::GyroscopeUPtr ChilliSource::Gyroscope::Create();
            
            //----------------------------------------------------
            /// Constructor. Declared private to force the use of
            /// the factory method.
            ///
            /// @author J Brown
            //----------------------------------------------------
            Gyroscope();
            //----------------------------------------------------
            /// Initialises the system. This is called at a time
            /// when all systems have already been added.
            ///
            /// @author J Brown
            //----------------------------------------------------
            void OnInit() override;
            
            ///
            /// Grabs the data from the motion device and calls
            /// the event. Also updates the data used for polling
            ///
            /// @param deltaSecs
            ///     Time since last frame in seconds
            ///
            void OnUpdate(f32 deltaSecs) override;
            
            //----------------------------------------------------
            /// This is called whenever the devicemotion is
            /// updated through iOS CMMotionManager events
            ///
            /// @author Jordan Brown
            ///
            /// @param deviceMotionData
            ///     New orientation data generated by event.
            //----------------------------------------------------
            void OnDeviceMotionUpdated(CMDeviceMotion* deviceMotionData) noexcept;
            //----------------------------------------------------
            /// Destroys the system immediately before systems
            /// are removed from the application.
            ///
            /// @author J Brown
            //----------------------------------------------------
            void OnDestroy() override;
            
            bool m_isUpdating;
            CMMotionManager* m_motionManager;
            ChilliSource::Event<OrientationUpdatedDelegate> m_orientationUpdatedEvent;
            ChilliSource::Quaternion m_orientation;
        };
    }
}

#endif

#endif
