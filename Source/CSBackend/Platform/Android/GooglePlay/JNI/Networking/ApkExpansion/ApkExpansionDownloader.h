//
//  ApkExpansionDownloader.h
//  Chilli Source
//  Created by Ian Copland on 22/04/2015.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2015 Tag Games Limited
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

#ifdef CS_ANDROIDFLAVOUR_GOOGLEPLAY

#ifndef _CSBACKEND_PLATFORM_ANDROID_GOOGLEPLAY_JNI_NETWORKING_APKEXPANSION_APKEXPANSIONDOWNLOADER_H_
#define _CSBACKEND_PLATFORM_ANDROID_GOOGLEPLAY_JNI_NETWORKING_APKEXPANSION_APKEXPANSIONDOWNLOADER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/System/AppSystem.h>

#include <CSBackend/Platform/Android/Main/JNI/ForwardDeclarations.h>
#include <CSBackend/Platform/Android/Main/JNI/Core/Java/JavaSystem.h>
#include <CSBackend/Platform/Android/GooglePlay/JNI/ForwardDeclarations.h>

namespace CSBackend
{
	namespace Android
	{
		//------------------------------------------------------------------------------
		/// TODO
		///
		/// @author Ian Copland
		//------------------------------------------------------------------------------
		class ApkExpansionDownloader final : public CSCore::AppSystem
		{
		public:
            CS_DECLARE_NAMEDTYPE(ApkExpansionDownloader);
            //------------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @param Interface ID to compare
            ///
            /// @return Whether the class is of the given type
            //------------------------------------------------------------------------------
            bool IsA(CSCore::InterfaceIDType in_interfaceId) const override;

		private:
		    friend class CSCore::Application;
            //------------------------------------------------------------------------------
            /// A factory method for creating a new instance of the system. Declared private
            /// to ensure this can only be created through Application::CreateSystem().
            ///
            /// @author Ian Copland
            ///
            /// @return The new instance.
            //------------------------------------------------------------------------------
            static ApkExpansionDownloaderUPtr Create();
            //------------------------------------------------------------------------------
            /// Default constructor. Declared private to ensure the factory method is used.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
			ApkExpansionDownloader() = default;
            //------------------------------------------------------------------------------
            /// Initialises the system during the Application On Init life cycle event.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            void OnInit() override;
            //------------------------------------------------------------------------------
            /// Cleans up the system during the application On Destroy life cycle event.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            void OnDestroy() override;

            JavaSystemUPtr m_javaSystem;
		};
	}
}

#endif

#endif

#endif
