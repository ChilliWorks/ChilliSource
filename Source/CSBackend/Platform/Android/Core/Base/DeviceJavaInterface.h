//
//  DeviceJavaInterface.h
//  Chilli Source
//  Created by Scott Downie on 24/09/2014.
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

#ifndef _CSBACKEND_PLATFORM_ANDROID_CORE_BASE_DEVICEJAVAINTERFACE_H_
#define _CSBACKEND_PLATFORM_ANDROID_CORE_BASE_DEVICEJAVAINTERFACE_H_

#include <ChilliSource/ChilliSource.h>
#include <CSBackend/Platform/Android/Core/JNI/JavaInterface.h>

#include <jni.h>

namespace CSBackend
{
	namespace Android
	{
		//--------------------------------------------------------------------------------------
		/// Allows device stats to be retrieved from the Android Java SDK
		///
		/// @author S Downie
		//--------------------------------------------------------------------------------------
		class DeviceJavaInterface : public IJavaInterface
		{
		public:
			CS_DECLARE_NAMEDTYPE(DeviceJavaInterface);
			//--------------------------------------------------------------------------------------
			/// Constructor
			///
			/// @author S Downie
			//--------------------------------------------------------------------------------------
			DeviceJavaInterface();
			//--------------------------------------------------------------------------------------
			/// @author S Downie
			///
			/// @param Interface ID
			///
			/// @return Whether or not this object implements the given interface.
			//--------------------------------------------------------------------------------------
			bool IsA(CSCore::InterfaceIDType in_interfaceId) const override;
			//--------------------------------------------------------------------------------------
			/// @author S Downie
			///
			/// @return returns the default locale code as acquired from the CoreNativeInterface
			/// Java class.
			//--------------------------------------------------------------------------------------
			std::string GetDefaultLocaleCode();
			//--------------------------------------------------------------------------------------
			/// @author S Downie
			///
			/// @return returns the device model as acquired from the CoreNativeInterface
			/// Java class.
			//--------------------------------------------------------------------------------------
			std::string GetDeviceModel();
			//--------------------------------------------------------------------------------------
			/// @author S Downie
			///
			/// @return returns the device model type as acquired from the CoreNativeInterface
			/// Java class.
			//--------------------------------------------------------------------------------------
			std::string GetDeviceModelType();
			//--------------------------------------------------------------------------------------
			/// @author S Downie
			///
			/// @return returns the device manufacturer as acquired from the CoreNativeInterface
			/// Java class.
			//--------------------------------------------------------------------------------------
			std::string GetDeviceManufacturer();
			//--------------------------------------------------------------------------------------
			/// @author S Downie
			///
			/// @return returns the os version number as acquired from the CoreNativeInterface
			/// Java class.
			//--------------------------------------------------------------------------------------
			s32 GetOSVersionCode();
			//--------------------------------------------------------------------------------------
			/// @author S Downie
			///
			/// @return returns the number of cores as acquired from the CoreNativeInterface
			/// Java class.
			//--------------------------------------------------------------------------------------
			s32 GetNumberOfCores();
			//--------------------------------------------------------------
			/// @author S Downie
			///
			/// @return String containing the id or empty if none can be obtained
			//--------------------------------------------------------------
			std::string GetUniqueId();
            
		private:

			std::string m_deviceModel;
			std::string m_deviceModelType;
			std::string m_deviceManufacturer;
			std::string m_localeCode;
			s32 m_OSVersionCode;
			s32 m_numberOfCores;
		};
	}
}

#endif

#endif
