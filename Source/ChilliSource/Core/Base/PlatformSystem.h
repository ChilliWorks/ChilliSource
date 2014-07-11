//
//  PlatformSystem.h
//  Chilli Source
//  Created by Scott Downie on 24/09/2010.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2010 Tag Games Limited
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

#ifndef _CHILLISOURCE_CORE_BASE_PLATFORMSYSTEM_H_
#define _CHILLISOURCE_CORE_BASE_PLATFORMSYSTEM_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/System/AppSystem.h>

namespace ChilliSource
{
    namespace Core
    {
        //-----------------------------------------------------------
        /// Provides an interface for initialising the platform
        /// specific application backend, creating platform specific
        /// default systems and managing the platform specific
        /// updater.
        ///
        /// @author S Downie
        //-----------------------------------------------------------
        class PlatformSystem : public AppSystem
        {
        public:
            CS_DECLARE_NAMEDTYPE(PlatformSystem);
            //---------------------------------------------------
            /// NOTE: Setting this will disable VSync
            ///
            /// @author S Downie
            ///
            /// @param The maximum frames per second
            /// to clamp to. This should be in multiples
            /// of 15 (15, 30, 60)
            //---------------------------------------------------
            virtual void SetPreferredFPS(u32 in_fps) = 0;
            //---------------------------------------------------
            /// This will turn VSync on or off on platforms that
            /// support it. NOTE: This will override the preferred FPS
            ///
            /// @author S Downie
            ///
            /// @param Enable/Disable
            //---------------------------------------------------
            virtual void SetVSyncEnabled(bool in_enabled) = 0;
            //---------------------------------------------------
            /// Terminate the app
            ///
            /// @author S Downie
            //---------------------------------------------------
            virtual void Quit() = 0;
            //---------------------------------------------------
            /// Adds default systems to the applications system
			/// list.
            ///
            /// @author S Downie
			///
			/// @param Application instance to add the default
            /// platform systems to.
            //---------------------------------------------------
            virtual void CreateDefaultSystems(Application* in_application) = 0;
            //---------------------------------------------------
            /// @author S Downie
            ///
            /// @return the current time in milliseconds
            //---------------------------------------------------
            virtual TimeIntervalMs GetSystemTimeMS() const = 0;
            //---------------------------------------------------
            /// @author S Downie
            ///
            /// @return The version of the application
            //---------------------------------------------------
            virtual std::string GetAppVersion() const = 0;
            //---------------------------------------------------
            /// Destructor
            ///
            /// @author S Downie
            //---------------------------------------------------
            virtual ~PlatformSystem(){}
        protected:
            friend class Application;
            //---------------------------------------------------
            /// Creates a new platform specfic instance of the
            /// platform system.
            ///
            /// @author Ian Copland
            ///
            /// @return The new instance.
            //---------------------------------------------------
            static PlatformSystemUPtr Create();
        };
    }
}

#endif
