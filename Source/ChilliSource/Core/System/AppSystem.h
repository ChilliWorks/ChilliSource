//
//  AppSystem.h
//  Chilli Source
//  Created by Ian Copland on 07/03/2014.
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

#ifndef _CHILLISOURCE_CORE_SYSTEM_APPSYSTEM_H_
#define _CHILLISOURCE_CORE_SYSTEM_APPSYSTEM_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/QueryableInterface.h>

namespace ChilliSource
{
    namespace Core
    {
        //-------------------------------------------------------
        /// An application level system. An app system is created
        /// when an application begins and exists through the
        /// entire life of the app.
        //-------------------------------------------------------
        class AppSystem : public QueryableInterface
        {
        public:
            CS_DECLARE_NOCOPY(AppSystem);
            //------------------------------------------------
            /// Destructor
            ///
            /// @author Ian Copland
            //------------------------------------------------
            virtual ~AppSystem() {};
        protected:
            friend class Application;
            
            //------------------------------------------------
            /// Default constructor.
            ///
            /// @author Ian Copland
            //------------------------------------------------
            AppSystem() = default;
            //------------------------------------------------
            /// Initialisation method called at a time when
            /// all App Systems have been created. System
            /// initialisation occurs in the order they were
            /// created.
            ///
            /// @author Ian Copland
            //------------------------------------------------
            virtual void OnInit() {};
            //------------------------------------------------
            /// Called when the application is resumed after
            /// having been suspended. This is also called
            /// on first run immediately after On Initialise()
            /// System resume is called in the order that
            /// the systems were created.
            ///
            /// @author Ian Copland
            //------------------------------------------------
            virtual void OnResume() {};
            //------------------------------------------------
            /// Called when the application becomes the active
            /// application. This is also called on first run
            /// immediately following On Resume. System
            /// foregrounded is called in the order the systems
            /// were created.
            ///
            /// @author Ian Copland
            //------------------------------------------------
            virtual void OnForeground() {};
            //------------------------------------------------
            /// Update method. This is called once for every
            /// update frame. Systems update is called in the
            /// order the systems where created.
            ///
            /// @author Ian Copland
            ///
            /// @param The delta time.
            //------------------------------------------------
            virtual void OnUpdate(f32 in_deltaTime) {};
            //------------------------------------------------
            /// An update method called at a fixed interval.
            /// The time between fixed updates is defined in
            /// Application. System update is called in the
            /// order the systems where created.
            ///
            /// @author Ian Copland
            ///
            /// @param The delta time.
            //------------------------------------------------
            virtual void OnFixedUpdate(f32 in_deltaTime) {};
            //------------------------------------------------
            /// Called when the application transitions from
            /// being active app into the background. This
            /// is also called when the application is
            /// exiting just prior to calling On Suspend.
            /// System backgrounded is called in the reverse
            /// order to which they were created.
            ///
            /// @author Ian Copland
            //------------------------------------------------
            virtual void OnBackground() {};
            //------------------------------------------------
            /// Called when the application is suspended. This
            /// is also called when the application is exiting
            /// just prior to calling On Destroy. System
            /// suspend is called in the reverse order to
            /// which they were created.
            ///
            /// @author Ian Copland
            //------------------------------------------------
            virtual void OnSuspend() {};
            //------------------------------------------------
            /// Called when the application is being destroyed.
            /// This should be used to cleanup memory and
            /// references to other systems. System destruction
            /// occurs in the reverse order to which they
            /// were created
            ///
            /// @author Ian Copland
            //------------------------------------------------
            virtual void OnDestroy() {};
            //------------------------------------------------
            /// Called when the application receives a memory
            /// warning. This should be used to clean up any
            /// non-essensial memory.
            ///
            /// @author Ian Copland
            //------------------------------------------------
            virtual void OnMemoryWarning() {};
        };
    }
}

#endif
