//
//  StateSystem.h
//  Chilli Source
//
//  Created by I Copland on 07/03/2014.
//  Copyright (c) 2014 Tag Games Ltd. All rights reserved.
//

#ifndef _CHILLISOURCE_CORE_SYSTEM_STATESYSTEM_H_
#define _CHILLISOURCE_CORE_SYSTEM_STATESYSTEM_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/QueryableInterface.h>

namespace ChilliSource
{
    namespace Core
    {
        //-------------------------------------------------------
        /// A state level system. A state system is created
        /// at the same time as the state that owns it and lives
        /// for the same length of time.
        //-------------------------------------------------------
        class StateSystem : public QueryableInterface
        {
        public:
            //------------------------------------------------
            /// Initialisation method called at a time when
            /// all State Systems have been created. System
            /// initialisation occurs in the order they were
            /// created.
            ///
            /// @author I Copland
            //------------------------------------------------
            virtual void OnInit() {};
            //------------------------------------------------
            /// Called when the state is started after
            /// having been stopped. This is also called when
            /// the owning state is initialised immediately after
            /// On Initialise(). System resume is called in the
            /// order that the systems were created.
            ///
            /// @author I Copland
            //------------------------------------------------
            virtual void OnStart() {};
            //------------------------------------------------
            /// Called when the state is resumed after
            /// having been suspended. This is also called when
            /// the owning state is initialised immediately after
            /// On Start(). System resume is called in the
            /// order that the systems were created.
            ///
            /// @author I Copland
            //------------------------------------------------
            virtual void OnResume() {};
            //------------------------------------------------
            /// Called when the state becomes the active
            /// application. This is also called on when the
            /// owning state is initialised, immediately following
            /// On Resume. System foregrounded is called in the
            /// order the systems were created.
            ///
            /// @author I Copland
            //------------------------------------------------
            virtual void OnForeground() {};
            //------------------------------------------------
            /// Update method. This is called once for every
            /// update frame that the owning state is active.
            /// Systems update is called in the order the
            /// systems where created.
            ///
            /// @author I Copland
            ///
            /// @param The delta time.
            //------------------------------------------------
            virtual void OnUpdate(f32 in_deltaTime) {};
            //------------------------------------------------
            /// An update method called at a fixed interval
            /// while the owning state is active. The time between
            /// fixed updates is defined in Application. System
            /// update is called in the order the systems where
            /// created.
            ///
            /// @author I Copland
            //------------------------------------------------
            virtual void OnFixedUpdate(f32 in_deltaTime) {};
            //------------------------------------------------
            /// Called when the state transitions from
            /// being active app into the background. This
            /// is also called when the state is destroyed,
            /// just prior to calling On Suspend. System
            /// backgrounded is called in the reverse order
            /// to which they were created.
            ///
            /// @author I Copland
            //------------------------------------------------
            virtual void OnBackground() {};
            //------------------------------------------------
            /// Called when the state is suspended. This
            /// is also called when the owning state is destroyed,
            /// just prior to calling On Suspend. System
            /// suspend is called in the reverse order to
            /// which they were created.
            ///
            /// @author I Copland
            //------------------------------------------------
            virtual void OnSuspend() {};
            //------------------------------------------------
            /// Called when the state is stopped. This
            /// is also called when the owning state is destroyed,
            /// just prior to calling On Destroy. System
            /// suspend is called in the reverse order to
            /// which they were created.
            ///
            /// @author I Copland
            //------------------------------------------------
            virtual void OnStop() {};
            //------------------------------------------------
            /// Called when the owning state is being destroyed.
            /// This should be used to cleanup memory and
            /// references to other systems. System destruction
            /// occurs in the reverse order to which they
            /// were created
            ///
            /// @author I Copland
            //------------------------------------------------
            virtual void OnDestroy() {};
            //------------------------------------------------
            /// Called when the state receives a memory
            /// warning while the owning state is active. This
            /// should be used to clean up any non-essensial
            /// memory.
            ///
            /// @author I Copland
            //------------------------------------------------
            virtual void OnMemoryWarning() {};
            //------------------------------------------------
            /// Destructor
            ///
            /// @author I Copland
            //------------------------------------------------
            virtual ~StateSystem() {};
        };
    }
}

#endif
