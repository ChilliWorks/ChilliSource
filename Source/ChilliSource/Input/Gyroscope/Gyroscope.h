//
//  Gyroscope.h
//  ChilliSource
//  Created by Jordan Brown on 10/01/2017
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

#ifndef _CHILLISOURCE_INPUT_GYROSCOPE_GYROSCOPE_H_
#define _CHILLISOURCE_INPUT_GYROSCOPE_GYROSCOPE_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Event/Event.h>
#include <ChilliSource/Core/Math/Quaternion.h>
#include <ChilliSource/Core/System/AppSystem.h>

#include <functional>

namespace ChilliSource
{
    //------------------------------------------------------------
    /// Listens to and stores the orientation/attitude read from the
    /// hardware gyroscope.
    ///
    /// @author J Brown
    //------------------------------------------------------------
    class Gyroscope : public AppSystem
    {
    public:
        CS_DECLARE_NAMEDTYPE(Gyroscope);
        //----------------------------------------------------
        /// A delegate for receiving orientation updates
        /// from the gyroscope.
        ///
        /// @author J Brown
        ///
        /// @param The new orientation. This wont neccessarily
        /// have changed since the last update.
        //----------------------------------------------------
        typedef std::function<void(const Quaternion&)> OrientationUpdatedDelegate;
        //----------------------------------------------------
        /// Creates a new platform specific instance of the
        /// system.
        ///
        /// @author J Brown
        //----------------------------------------------------
        static GyroscopeUPtr Create();
        //----------------------------------------------------
        /// @author J Brown
        ///
        /// @return whether or not the gyroscope is
        /// currently updating.
        //----------------------------------------------------
        virtual bool IsUpdating() const = 0;
        //----------------------------------------------------
        /// Start listening for orientation changes.
        ///
        /// @author J Brown
        //----------------------------------------------------
        virtual void StartUpdating() = 0;
        //----------------------------------------------------
        /// @author J Brown
        ///
        /// @return The physical orientation of the device,
        /// as a quaternion.
        //----------------------------------------------------
        virtual Quaternion GetOrientation() const = 0;
        //----------------------------------------------------
        /// @author J Brown
        ///
        /// @return An event that is invoked every time the
        /// orientation is updated. The orientation will not
        /// necessarily have changed between updates.
        //----------------------------------------------------
        virtual IConnectableEvent<OrientationUpdatedDelegate>& GetOrientationUpdatedEvent() = 0;
        //----------------------------------------------------
        /// Stop listening for orientation changes.
        ///
        /// @author J Brown
        //----------------------------------------------------
        virtual void StopUpdating() = 0;
        //----------------------------------------------------
        /// Destructor
        ///
        /// @author J Brown
        //----------------------------------------------------
        virtual ~Gyroscope() {}
    };
}

#endif
