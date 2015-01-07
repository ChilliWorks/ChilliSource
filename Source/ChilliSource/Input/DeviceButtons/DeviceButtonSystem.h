//
//  DeviceButtonSystem.h
//  Chilli Source
//  Created by Ian Copland on 16/05/2014.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2014 Tag Games Limited
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

#ifndef _CHILLISOURCE_INPUT_DEVICEBUTTONS_DEVICEBUTTONSYSTEM_H_
#define _CHILLISOURCE_INPUT_DEVICEBUTTONS_DEVICEBUTTONSYSTEM_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Event/Event.h>
#include <ChilliSource/Core/System/AppSystem.h>

#include <functional>

namespace ChilliSource
{
	namespace Input
	{
		//------------------------------------------------------------
		/// A system that provides access to hard buttons on the device.
        /// This includes buttons like the back button on Android
        /// devices. Different devices and platforms may have different
        /// sets of hard buttons meaning there is no guarantee that
        /// an event for that button will ever be received.
        ///
        /// @author Ian Copland
		//------------------------------------------------------------
		class DeviceButtonSystem : public Core::AppSystem
		{
		public:
			CS_DECLARE_NAMEDTYPE(DeviceButtons);
            //----------------------------------------------------
			/// An enum describing the possible Device Buttons.
            ///
            /// @author Ian Copland
			//----------------------------------------------------
            enum class DeviceButton
            {
                k_backButton
            };
            //----------------------------------------------------
			/// A delegate which will be called when a device
            /// button pressed event is received.
            ///
            /// @author Ian Copland
            ///
            /// @param The received button.
			//----------------------------------------------------
            using TriggeredDelegate = std::function<void(DeviceButton)>;
            //----------------------------------------------------
			/// @author Ian Copland
            ///
            /// @return An event that will be called whenever a
            /// device button is triggered.
			//----------------------------------------------------
            virtual Core::IConnectableEvent<TriggeredDelegate>& GetTriggeredEvent() = 0;
			//----------------------------------------------------
			/// Destructor
            ///
            /// @author Ian Copland
			//----------------------------------------------------
			virtual ~DeviceButtonSystem(){}
        protected:
            friend class Core::Application;
            
            //----------------------------------------------------
            /// Creates a new platfrom specific instance of pointer
            /// system.
            ///
            /// @author Ian Copland
            ///
            /// @return The new system instance.
            //----------------------------------------------------
            static DeviceButtonSystemUPtr Create();
            //----------------------------------------------------
            /// Constructor. Declared protected to force the
            /// use of the factory creation method.
            ///
            /// @author Ian Copland
            //----------------------------------------------------
            DeviceButtonSystem() = default;
		};
	}
}

#endif
