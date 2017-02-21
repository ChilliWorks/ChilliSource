//
//  DeviceButtonSystem.mm
//  ChilliSource
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

#import <CSBackend/Platform/iOS/Input/DeviceButtons/DeviceButtonSystem.h>

namespace CSBackend
{
	namespace iOS
	{
        CS_DEFINE_NAMEDTYPE(DeviceButtonSystem);
        //----------------------------------------------------
        //----------------------------------------------------
        bool DeviceButtonSystem::IsA(ChilliSource::InterfaceIDType in_interfaceID) const
        {
            return (in_interfaceID == ChilliSource::DeviceButtonSystem::InterfaceID || in_interfaceID == DeviceButtonSystem::InterfaceID);
        }
        //----------------------------------------------------
        //----------------------------------------------------
        ChilliSource::IConnectableEvent<DeviceButtonSystem::TriggeredDelegate>& DeviceButtonSystem::GetTriggeredEvent()
        {
            return m_triggeredEvent;
        }
	}
}

#endif
