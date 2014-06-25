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

#ifdef CS_TARGETPLATFORM_IOS

#import <ChilliSource/ChilliSource.h>
#import <CSBackend/Platform/iOS/ForwardDeclarations.h>
#import <ChilliSource/Input/DeviceButtons/DeviceButtonSystem.h>

namespace CSBackend
{
	namespace iOS
	{
        //------------------------------------------------------------
        /// The iOS backend implementation of the Device Button System.
        ///
        /// @author Ian Copland
        //------------------------------------------------------------
		class DeviceButtonSystem final : public CSInput::DeviceButtonSystem
		{
		public:
            CS_DECLARE_NAMEDTYPE(DeviceButtonSystem);
            //----------------------------------------------------
			/// Queries whether or not this system implements the
            /// interface with the given ID.
            ///
            /// @author Ian Copland
            ///
            /// @param The interface Id.
            ///
			/// @return Whether or not the interface is implemented.
			//----------------------------------------------------
            bool IsA(CSCore::InterfaceIDType in_interfaceId) const override;
            //----------------------------------------------------
			/// @author Ian Copland
            ///
            /// @return An event that will be called whenever a
            /// device button is triggered.
			//----------------------------------------------------
            CSCore::IConnectableEvent<TriggeredDelegate>& GetTriggeredEvent() override;
		private:
            friend CSInput::DeviceButtonSystemUPtr CSInput::DeviceButtonSystem::Create();
            
            //----------------------------------------------------
			/// Constructor. Declared private to force the use of
            /// the factory method.
            ///
            /// @author Ian Copland
			//----------------------------------------------------
			DeviceButtonSystem() = default;

            CSCore::Event<TriggeredDelegate> m_triggeredEvent;
		};
	}
}

#endif