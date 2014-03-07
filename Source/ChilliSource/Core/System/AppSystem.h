//
//  AppSystem.h
//  Chilli Source
//
//  Created by Ian Copland on 07/03/2014.
//  Copyright (c) 2014 Tag Games Ltd. All rights reserved.
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
            //------------------------------------------------
            /// Initialisation method called at a time when
            /// all App Systems have been created. System
            /// initialisation occurs in the order they were
            /// created.
            ///
            /// @author I Copland
            //------------------------------------------------
            virtual void OnInitialise() {};
            //------------------------------------------------
            /// Called when the application is resumed after
            /// having been suspended. This is also called
            /// on first run immediately after On Initialise()
            /// System resume is called in the order that
            /// the systems were created.
            ///
            /// @author I Copland
            //------------------------------------------------
            virtual void OnResume() {};
            //------------------------------------------------
            /// Called when the application becomes the active
            /// application. This is also called on first run
            /// immediately following On Resume. System
            /// foregrounded is called in the order the systems
            /// were created.
            ///
            /// @author I Copland
            //------------------------------------------------
            virtual void OnForgrounded() {};
            //------------------------------------------------
            /// Update method. This is called once for every
            /// update frame. Systems update is called in the
            /// order the systems where created.
            ///
            /// @author I Copland
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
            /// @author I Copland
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
            /// @author I Copland
            //------------------------------------------------
            virtual void OnBackgrounded() {};
            //------------------------------------------------
            /// Called when the application is suspended. This
            /// is also called when the application is exiting
            /// just prior to calling On Destroy. System
            /// suspend is called in the reverse order to
            /// which they were created.
            ///
            /// @author I Copland
            //------------------------------------------------
            virtual void OnSuspend() {};
            //------------------------------------------------
            /// Called when the application is being destroyed.
            /// This should be used to cleanup memory and
            /// references to other systems. System destruction
            /// occurs in the reverse order to which they
            /// were created
            ///
            /// @author I Copland
            //------------------------------------------------
            virtual void OnDestroy() {};
            //------------------------------------------------
            /// Called when the application receives a memory
            /// warning. This should be used to clean up any
            /// non-essensial memory.
            ///
            /// @author I Copland
            //------------------------------------------------
            virtual void OnMemoryWarning() {};
            //------------------------------------------------
            /// Destructor
            ///
            /// @author I Copland
            //------------------------------------------------
            virtual ~AppSystem() {};
        };
    }
}

#endif
