//
//  CoreJavaInterface.h
//  ChilliSource
//  Created by Ian Copland on 17/03/2011.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2011 Tag Games Limited
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

#ifndef _CSBACKEND_PLATFORM_ANDROID_CORE_BASE_COREJAVAINTERFACE_H_
#define _CSBACKEND_PLATFORM_ANDROID_CORE_BASE_COREJAVAINTERFACE_H_

#include <ChilliSource/ChilliSource.h>
#include <CSBackend/Platform/Android/Main/JNI/Core/Java/JavaInterface.h>

#include <atomic>
#include <jni.h>

namespace CSBackend
{
	namespace Android
	{
	    //--------------------------------------------------------------------------------------
	    /// This provides the main lifecycle events into an application, as well as some
	    /// important information on the application, such as screen size.
	    ///
	    /// Note this is in a legacy format and should be updated such that it doesn't
	    /// use IJavaInterface, and so that the lifecycle events and things such as screen
	    /// size are handled by different classes.
	    //--------------------------------------------------------------------------------------
		class CoreJavaInterface : public IJavaInterface
		{
		public:
			CS_DECLARE_NAMEDTYPE(CoreJavaInterface);

			CoreJavaInterface();
			//--------------------------------------------------------------------------------------
			/// @author Ian Copland
			///
			/// @param Interface ID
			///
			/// @return Whether or not this object implements the given interface.
			//--------------------------------------------------------------------------------------
			bool IsA(ChilliSource::InterfaceIDType in_interfaceId) const override;
			//--------------------------------------------------------------------------------------
			/// @author Ian Copland
			///
			/// @return The current activity.
			//--------------------------------------------------------------------------------------
			jobject GetActivity() const;
			//--------------------------------------------------------------------------------------
			/// @author Ian Copland
			///
			/// @param The maximum frames per second to clamp to. This should be in multiples
			/// of 15 (15, 30, 60)
			//--------------------------------------------------------------------------------------
			void SetPreferredFPS(u32 in_maxFPS);
			//--------------------------------------------------------------------------------------
			/// @author Ian Copland
			///
			/// @return returns the External Storage Directory as acquired from the CoreNativeInterface
			/// Java class.
			//--------------------------------------------------------------------------------------
			std::string GetExternalStorageDirectory();
			//--------------------------------------------------------------------------------------
			/// @author Ian Copland
			///
			/// @return returns the Application name as acquired from the CoreNativeInterface
			/// Java class.
			//--------------------------------------------------------------------------------------
			std::string GetApplicationName();
			//--------------------------------------------------------------------------------------
			/// @author Ian Copland
			///
			/// @return returns the Application version code as acquired from the CoreNativeInterface
			/// Java class.
			//--------------------------------------------------------------------------------------
			u32 GetApplicationVersionCode();
			//--------------------------------------------------------------------------------------
			/// @author Ian Copland
			///
			/// @return returns the Application version name as acquired from the CoreNativeInterface
			/// Java class.
			//--------------------------------------------------------------------------------------
			std::string GetApplicationVersionName();
			//--------------------------------------------------------------------------------------
			/// @author Ian Copland
			///
			/// @return returns the package name as acquired from the CoreNativeInterface
			/// Java class.
			//--------------------------------------------------------------------------------------
			std::string GetPackageName();
			//--------------------------------------------------------------------------------------
			/// @author Ian Copland
			///
			/// @return returns the APK directory as acquired from the CoreNativeInterface
			/// Java class.
			//--------------------------------------------------------------------------------------
			std::string GetAPKDirectory();
			//--------------------------------------------------------------------------------------
			/// @author Ian Copland
			///
			/// @return returns the screen width as acquired from the CoreNativeInterface
			/// Java class.
			//--------------------------------------------------------------------------------------
			s32 GetScreenWidth();
			//--------------------------------------------------------------------------------------
			/// @author Ian Copland
			///
			/// @return returns the screen height as acquired from the CoreNativeInterface
			/// Java class.
			//--------------------------------------------------------------------------------------
			s32 GetScreenHeight();
			//--------------------------------------------------------------------------------------
			/// @author Ian Copland
			///
			/// @return returns the screen density as acquired from the CoreNativeInterface
			/// Java class.
			//--------------------------------------------------------------------------------------
			f32 GetScreenDensity();
            //--------------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// Kill the application process
            //--------------------------------------------------------------------------------------
            void ForceQuit();
            //--------------------------------------------------------------------------------------
            /// Sends the init lifecycle event.
            ///
            /// @author Ian Copland
            //--------------------------------------------------------------------------------------
            void Init() noexcept;
            //--------------------------------------------------------------------------------------
            /// Sends the resume lifecycle event.
            ///
            /// @author Ian Copland
            //--------------------------------------------------------------------------------------
            void Resume() noexcept;
            //--------------------------------------------------------------------------------------
            /// Sends the foreground lifecycle event.
            ///
            /// @author Ian Copland
            //--------------------------------------------------------------------------------------
            void Foreground() noexcept;
            //--------------------------------------------------------------------------------------
            /// Sends the render lifecycle event.
            ///
            /// @author Ian Copland
            //--------------------------------------------------------------------------------------
            void Render() noexcept;
            //--------------------------------------------------------------------------------------
            /// Executes system thread tasks.
            ///
            /// @author Jordan Brown
            //--------------------------------------------------------------------------------------
            void SystemUpdate() noexcept;
            //--------------------------------------------------------------------------------------
            /// Sends the background lifecycle event.
            ///
            /// @author Ian Copland
            //--------------------------------------------------------------------------------------
            void Background() noexcept;
            //--------------------------------------------------------------------------------------
            /// Sends the suspend lifecycle event.
            ///
            /// @author Ian Copland
            //--------------------------------------------------------------------------------------
            void Suspend() noexcept;
            //--------------------------------------------------------------------------------------
            /// Sends the destroy lifecycle event.
            ///
            /// @author Ian Copland
            //--------------------------------------------------------------------------------------
            void Destroy() noexcept;
            //--------------------------------------------------------------------------------------
            /// Sends the memory warning lifecycle event.
            ///
            /// @author Ian Copland
            //--------------------------------------------------------------------------------------
            void MemoryWarning() noexcept;
		private:

			ChilliSource::ApplicationUPtr m_application;
			ChilliSource::LifecycleManagerUPtr m_lifecycleManager;
			s32 m_screenWidth;
			s32 m_screenHeight;
			f32 m_screenDensity;
			f32 m_physicalScreenSize;

			std::atomic<bool> m_isActive;
		};
	}
}

#endif

#endif
