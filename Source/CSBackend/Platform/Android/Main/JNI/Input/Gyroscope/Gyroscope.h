//
//  Gyroscope.h
//  ChilliSource
//  Created by Jordan Brown on 20/01/2017.
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

#ifdef CS_TARGETPLATFORM_ANDROID

#ifndef _CSBACKEND_PLATFORM_ANDROID_INPUT_GYROSCOPE_GYROSCOPE_H_
#define _CSBACKEND_PLATFORM_ANDROID_INPUT_GYROSCOPE_GYROSCOPE_H_

#include <ChilliSource/ChilliSource.h>
#include <CSBackend/Platform/Android/Main/JNI/ForwardDeclarations.h>
#include <ChilliSource/Input/Gyroscope/Gyroscope.h>

namespace CSBackend
{
	namespace Android
	{
		//------------------------------------------------------------
		/// Listens to and stores the acceleration read from the
		/// hardware gyroscope.
        ///
        /// The methods in this class aren't thread-safe, and must
        /// be accessed from the main thread.
		///
		/// @author J Brown
		//------------------------------------------------------------
		class Gyroscope final : public ChilliSource::Gyroscope
		{
		public:
			CS_DECLARE_NAMEDTYPE(Gyroscope);
			//----------------------------------------------------
			/// @author J Brown
			///
			/// @return whether or not this object implements
			/// the given interface.
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
			/// Stop listening for gyroscope changes.
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
            //----------------------------------------------------
            /// Resume gyroscope events if enabled
            ///
            /// @author J Brown
			//----------------------------------------------------
            void OnResume() override;
            //----------------------------------------------------
            /// Prevents gyroscope events when the app is
            /// backgrounded
            ///
            /// @author J Brown
			//----------------------------------------------------
            void OnSuspend() override;
            //----------------------------------------------------
			/// Destroys the system immediately before systems
            /// are removed from the application.
            ///
            /// @author J Brown
			//----------------------------------------------------
            void OnDestroy() override;
			//------------------------------------------------
			/// Called whenever the orientation changes.
			///
			/// @author J Brown
			///
			/// @param The new orientation.
			//------------------------------------------------
			void OnOrientationChanged(const ChilliSource::Quaternion& in_orientation);

			ChilliSource::Quaternion m_orientation;
			GyroscopeJavaInterfaceSPtr m_gyroscopeJI;
			bool m_isUpdating;
			ChilliSource::Event<OrientationUpdatedDelegate> m_orientationUpdatedEvent;
		};
	}
}

#endif

#endif
