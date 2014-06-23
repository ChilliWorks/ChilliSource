//
//  DeviceButtonJavaInterface.h
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

#ifdef CS_TARGETPLATFORM_ANDROID

#ifndef _CHILLISOURCE_PLATFORM_ANDROID_INPUT_DEVICEBUTTONS_DEVICEBUTTONJAVAINTERFACE_H_
#define _CHILLISOURCE_PLATFORM_ANDROID_INPUT_DEVICEBUTTONS_DEVICEBUTTONJAVAINTERFACE_H_

#include <CSBackend/Platform/Android/ForwardDeclarations.h>
#include <CSBackend/Platform/Android/Core/JNI/JavaInterface.h>

#include <functional>

namespace CSBackend
{
	namespace Android
	{
		//----------------------------------------------------------------
		/// Provides access to information about Device Buttons on the
		/// java side of the engine.
		///
		/// @author Ian Copland
		//----------------------------------------------------------------
		class DeviceButtonJavaInterface : public IJavaInterface
		{
		public:
			CS_DECLARE_NAMEDTYPE(DeviceButtonJavaInterface);
			//-----------------------------------------------
			/// A delegate called when the java side of the
			/// engine receives a device button pressed event.
			///
			/// @author Ian Copland
			///
			/// @param An integer representing the button
			/// pressed.
			//-----------------------------------------------
			using TriggeredDelegate = std::function<void(s32)>;
			//-----------------------------------------------
			/// Constructor
			///
			/// @author Ian Copland
			//-----------------------------------------------
			DeviceButtonJavaInterface();
            //----------------------------------------------------
			/// Queries whether or not this implements the interface
			/// with the given ID.
            ///
            /// @author Ian Copland
            ///
            /// @param The interface Id.
            ///
			/// @return Whether or not the interface is implemented.
			//----------------------------------------------------
			bool IsA(CSCore::InterfaceIDType in_interfaceId) const;
			//-----------------------------------------------
			/// @author Ian Copland
			///
			/// @param The delegate that will be called when
			/// a device button triggered event is received.
			//-----------------------------------------------
			void SetTriggeredDelegate(const TriggeredDelegate& in_delegate);
			//-----------------------------------------------
			/// Called when a device button triggered event
			/// is received. This is for internal use and
			/// should not be called manually by the user.
			///
			/// @author Ian Copland
			///
			/// @param An integer representing the button
			/// triggered.
			//-----------------------------------------------
			void OnTriggered(s32 in_button);
		private:
			TriggeredDelegate m_triggeredDelegate;
		};
	}
}

#endif

#endif